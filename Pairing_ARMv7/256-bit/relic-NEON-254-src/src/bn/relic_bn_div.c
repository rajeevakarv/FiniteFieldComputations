/*
 * RELIC is an Efficient LIbrary for Cryptography
 * Copyright (C) 2007, 2008, 2009 RELIC Authors
 *
 * This file is part of RELIC. RELIC is legal property of its developers,
 * whose names are not listed here. Please refer to the COPYRIGHT file
 * for contact information.
 *
 * RELIC is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * RELIC is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with RELIC. If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * @file
 *
 * Implementation of the multiple precision division functions.
 *
 * @version $Id: relic_bn_div.c 173 2009-11-07 23:05:30Z dfaranha $
 * @ingroup bn
 */

#include "relic_core.h"
#include "relic_bn.h"
#include "relic_bn_low.h"
#include "relic_util.h"

/*============================================================================*/
/* Private definitions                                                        */
/*============================================================================*/

/**
 * Divides two multiple precision integers, computing the quotient and the
 * remainder.
 *
 * @param[out] c		- the quotient.
 * @param[out] d		- the remainder.
 * @param[in] a			- the dividend.
 * @param[in] b			- the the divisor.
 */
static void bn_div_impl(bn_t c, bn_t d, bn_t a, bn_t b) {
	bn_t q, x, y, r;
	int sign;

	bn_null(q);
	bn_null(x);
	bn_null(y);
	bn_null(r);

	/* If a < b, we're done. */
	if (bn_cmp_abs(a, b) == CMP_LT) {
		if (d != NULL) {
			bn_copy(d, a);
		}
		if (c != NULL) {
			bn_zero(c);
		}
		return;
	}

	TRY {
		bn_new(x);
		bn_new(y);
		bn_new_size(q, a->used + 1);
		bn_new(r);
		bn_zero(q);
		bn_zero(r);
		bn_abs(x, a);
		bn_abs(y, b);

		/* Find the sign. */
		sign = (a->sign == b->sign ? BN_POS : BN_NEG);

		bn_divn_low(q->dp, r->dp, x->dp, a->used, y->dp, b->used);

		/* We have the quotient in q and the remainder in r. */
		if (c != NULL) {
			q->used = a->used - b->used + 1;
			q->sign = sign;
			bn_trim(q);
			bn_copy(c, q);
		}

		if (d != NULL) {
			r->used = b->used;
			r->sign = a->sign;
			bn_trim(r);
			bn_copy(d, r);
		}
	}
	CATCH_ANY {
		THROW(ERR_CAUGHT);
	}
	FINALLY {
		bn_free(r);
		bn_free(q);
		bn_free(x);
		bn_free(y);
	}
}

/*============================================================================*/
/* Public definitions                                                         */
/*============================================================================*/

void bn_div(bn_t c, bn_t a, bn_t b) {
	if (bn_is_zero(b)) {
		THROW(ERR_INVALID);
	}
	bn_div_impl(c, NULL, a, b);
}

void bn_div_rem(bn_t c, bn_t d, bn_t a, bn_t b) {
	if (bn_is_zero(b)) {
		THROW(ERR_INVALID);
	}
	bn_div_impl(c, d, a, b);
}

void bn_div_dig(bn_t c, bn_t a, dig_t b) {
	bn_t q;
	dig_t r;

	bn_null(q);

	if (b == 0) {
		THROW(ERR_INVALID);
	}

	if (b == 1 || bn_is_zero(a) == 1) {
		if (c != NULL) {
			bn_copy(c, a);
		}
		return;
	}

	TRY {
		bn_new(q);
		int size = a->used;
		dig_t *ap = a->dp;

		bn_div1_low(q->dp, &r, ap, size, b);

		if (c != NULL) {
			q->used = a->used;
			q->sign = a->sign;
			bn_trim(q);
			bn_copy(c, q);
		}
	}
	CATCH_ANY {
		THROW(ERR_CAUGHT);
	}
	FINALLY {
		bn_free(q);
	}
}

void bn_div_rem_dig(bn_t c, dig_t *d, bn_t a, dig_t b) {
	bn_t q;
	dig_t r;

	bn_null(q);

	if (b == 0) {
		THROW(ERR_INVALID);
	}

	if (b == 1 || bn_is_zero(a) == 1) {
		if (d != NULL) {
			*d = 0;
		}
		if (c != NULL) {
			bn_copy(c, a);
		}
		return;
	}

	TRY {
		bn_new(q);
		int size = a->used;
		dig_t *ap = a->dp;

		bn_div1_low(q->dp, &r, ap, size, b);

		if (c != NULL) {
			q->used = a->used;
			q->sign = a->sign;
			bn_trim(q);
			bn_copy(c, q);
		}

		if (d != NULL) {
			*d = r;
		}
	}
	CATCH_ANY {
		THROW(ERR_CAUGHT);
	}
	FINALLY {
		bn_free(q);
	}
}

