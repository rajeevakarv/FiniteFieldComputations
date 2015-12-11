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
 * Implementation of least common multiple.
 *
 * @version $Id: relic_bn_lcm.c 199 2009-12-15 19:36:10Z dfaranha $
 * @ingroup bn
 */

#include "relic_core.h"
#include "relic_bn.h"
#include "relic_bn_low.h"
#include "relic_error.h"

/*============================================================================*/
/* Public definitions                                                         */
/*============================================================================*/

void bn_lcm(bn_t c, bn_t a, bn_t b) {
	bn_t u, v;

	bn_null(u);
	bn_null(v);

	TRY {
		bn_new(u);
		bn_new(v);

		bn_gcd(u, a, b);

		if (bn_cmp_abs(a, b) == CMP_LT) {
			bn_div(v, a, u);
			bn_mul(c, b, v);
		} else {
			bn_div(v, b, u);
			bn_mul(c, a, v);
		}
		c->sign = BN_POS;
	}
	CATCH_ANY {
		THROW(ERR_CAUGHT);
	}
	FINALLY {
		bn_free(u);
		bn_free(v);
	}
}
