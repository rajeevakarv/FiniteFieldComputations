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
 * Implementation of simultaneous point multiplication on binary elliptic
 * curves.
 *
 * @version $Id: relic_ep_mul_sim.c 274 2010-02-06 05:34:38Z dfaranha $
 * @ingroup ep
 */

#include "string.h"

#include "relic_core.h"
#include "relic_ep.h"
#include "relic_util.h"

/*============================================================================*/
/* Private definitions                                                        */
/*============================================================================*/

#if EP_SIM == INTER || !defined(STRIP)

/**
 * Precomputes a table for a point multiplication on an ordinary curve.
 *
 * @param[out] t				- the destination table.
 * @param[in] p					- the point to multiply.
 */
static void table_init_ordin(ep_t *t, ep_t p) {
	int i;

	for (i = 0; i < (1 << (EP_WIDTH - 2)); i++) {
		ep_set_infty(t[i]);
		fp_set_dig(t[i]->z, 1);
		t[i]->norm = 1;
	}

	ep_dbl_tab(t[0], p);

#if EP_WIDTH > 2
	ep_add_tab(t[1], t[0], p);
	for (i = 2; i < (1 << (EP_WIDTH - 2)); i++) {
		ep_add_tab(t[i], t[i - 1], t[0]);
	}
#endif

	ep_copy(t[0], p);
}

static void ep_mul_sim_ordin(ep_t r, ep_t p, bn_t k, ep_t q, bn_t l, int gen) {
	int len, l0, l1, i, n0, n1, w;
	signed char naf0[FP_BITS + 1], naf1[FP_BITS + 1], *t0, *t1;
	ep_t table0[1 << (EP_WIDTH - 2)];
	ep_t table1[1 << (EP_WIDTH - 2)];
	ep_t *t = NULL;

	for (i = 0; i < (1 << (EP_WIDTH - 2)); i++) {
		ep_null(table0[i]);
		ep_null(table1[i]);
	}

	if (gen) {
#if defined(EP_PRECO)
		t = ep_curve_get_tab();
#endif
	} else {
		for (i = 0; i < (1 << (EP_WIDTH - 2)); i++) {
			ep_new(table0[i]);
		}
		table_init_ordin(table0, p);
		t = table0;
	}

	/* Prepare the precomputation table. */
	for (i = 0; i < (1 << (EP_WIDTH - 2)); i++) {
		ep_new(table1[i]);
	}
	/* Compute the precomputation table. */
	table_init_ordin(table1, q);

	/* Compute the w-TNAF representation of k. */
	if (gen) {
		w = EP_DEPTH;
	} else {
		w = EP_WIDTH;
	}
	bn_rec_naf(naf0, &l0, k, w);
	bn_rec_naf(naf1, &l1, l, EP_WIDTH);

	len = MAX(l0, l1);
	t0 = naf0 + len - 1;
	t1 = naf1 + len - 1;
	for (i = l0; i < len; i++)
		naf0[i] = 0;
	for (i = l1; i < len; i++)
		naf1[i] = 0;

	ep_set_infty(r);
	for (i = len - 1; i >= 0; i--, t0--, t1--) {
		ep_dbl(r, r);

		n0 = *t0;
		n1 = *t1;
		if (n0 > 0) {
			ep_add(r, r, t[n0 / 2]);
		}
		if (n0 < 0) {
			ep_sub(r, r, t[-n0 / 2]);
		}
		if (n1 > 0) {
			ep_add(r, r, table1[n1 / 2]);
		}
		if (n1 < 0) {
			ep_sub(r, r, table1[-n1 / 2]);
		}
	}
	/* Convert r to affine coordinates. */
	ep_norm(r, r);

	/* Free the precomputation table. */
	for (i = 0; i < 1 << (EP_WIDTH - 2); i++) {
		ep_free(table0[i]);
		ep_free(table1[i]);
	}
}

#endif /* EP_SIM == INTER */

/*============================================================================*/
/* Public definitions                                                         */
/*============================================================================*/

#if EP_SIM == BASIC || !defined(STRIP)

void ep_mul_sim_basic(ep_t r, ep_t p, bn_t k, ep_t q, bn_t l) {
	ep_t t;

	ep_null(t);

	TRY {
		ep_new(t);
		ep_mul(t, q, l);
		ep_mul(r, p, k);
		ep_add(t, t, r);
		ep_norm(r, t);

	} CATCH_ANY {
		THROW(ERR_CAUGHT);
	}
	FINALLY {
		ep_free(t);
	}
}

#endif

#if EP_SIM == TRICK || !defined(STRIP)

void ep_mul_sim_trick(ep_t r, ep_t p, bn_t k, ep_t q, bn_t l) {
	ep_t t0[1 << (EP_WIDTH / 2)];
	ep_t t1[1 << (EP_WIDTH / 2)];
	ep_t t[1 << EP_WIDTH];
	bn_t n;
	int d, l0, l1, w;
	unsigned char w0[FP_BITS + 1], w1[FP_BITS + 1];

	bn_null(n);

	for (int i = 0; i < 1 << EP_WIDTH; i++) {
		ep_null(t[i]);
	}

	for (int i = 0; i < 1 << (EP_WIDTH / 2); i++) {
		ep_null(t0[i]);
		ep_null(t1[i]);
	}

	w = EP_WIDTH / 2;


	TRY {
		bn_new(n);

		ep_curve_get_ord(n);
		d = bn_bits(n);
		d = ((d % w) == 0 ? (d / w) : (d / w) + 1);
		for (int i = 0; i < (1 << w); i++) {
			ep_new(t0[i]);
			ep_new(t1[i]);
		}
		for (int i = 0; i < (1 << EP_WIDTH); i++) {
			ep_new(t[i]);
		}

		ep_set_infty(t0[0]);
		for (int i = 1; i < (1 << w); i++) {
			ep_add_tab(t0[i], t0[i - 1], p);
		}

		ep_set_infty(t1[0]);
		for (int i = 1; i < (1 << w); i++) {
			ep_add_tab(t1[i], t1[i - 1], q);
		}

		for (int i = 0; i < (1 << w); i++) {
			for (int j = 0; j < (1 << w); j++) {
				ep_add_tab(t[(i << w) + j], t0[i], t1[j]);
			}
		}

		bn_rec_win(w0, &l0, k, w);
		bn_rec_win(w1, &l1, l, w);

		for (int i = l0; i < l1; i++) {
			w0[i] = 0;
		}
		for (int i = l1; i < l0; i++) {
			w1[i] = 0;
		}

		ep_set_infty(r);
		for (int i = MAX(l0, l1) - 1; i >= 0; i--) {
			for (int j = 0; j < w; j++) {
				ep_dbl(r, r);
			}
			ep_add(r, r, t[(w0[i] << w) + w1[i]]);
		}
		ep_norm(r, r);
	} CATCH_ANY {
		THROW(ERR_CAUGHT);
	}
	FINALLY {
		bn_free(n);
		for (int i = 0; i < (1 << w); i++) {
			ep_free(t0[i]);
			ep_free(t1[i]);
		}
		for (int i = 0; i < (1 << EP_WIDTH); i++) {
			ep_free(t[i]);
		}
	}
}
#endif

#if EP_SIM == INTER || !defined(STRIP)

void ep_mul_sim_inter(ep_t r, ep_t p, bn_t k, ep_t q, bn_t l) {
#if defined(EP_KBLTZ)
	if (ep_curve_is_kbltz()) {
		ep_mul_sim_kbltz(r, p, k, q, l, 0);
		return;
	}
#endif

#if defined(EP_ORDIN)
	ep_mul_sim_ordin(r, p, k, q, l, 0);
#endif
}

#endif

#if EP_SIM == JOINT || !defined(STRIP)

void ep_mul_sim_joint(ep_t r, ep_t p, bn_t k, ep_t q, bn_t l) {
	ep_t t[5];
	int u_i, len, offset;
	signed char jsf[2 * (FP_BITS + 1)];
	int i;

	ep_null(t[0]);
	ep_null(t[1]);
	ep_null(t[2]);
	ep_null(t[3]);
	ep_null(t[4]);

	TRY {
		for (i = 0; i < 5; i++) {
			ep_new(t[i]);
		}

		ep_set_infty(t[0]);
		ep_copy(t[1], q);
		ep_copy(t[2], p);
		ep_add_tab(t[3], p, q);
		ep_sub_tab(t[4], p, q);

		bn_rec_jsf(jsf, &len, k, l);

		ep_set_infty(r);

		i = bn_bits(k);
		offset = MAX(i, bn_bits(l)) + 1;
		for (i = len - 1; i >= 0; i--) {
			ep_dbl(r, r);
			if (jsf[i] != 0 && jsf[i] == -jsf[i + offset]) {
				u_i = jsf[i] * 2 + jsf[i + offset];
				if (u_i < 0) {
					ep_sub(r, r, t[4]);
				} else {
					ep_add(r, r, t[4]);
				}
			} else {
				u_i = jsf[i] * 2 + jsf[i + offset];
				if (u_i < 0) {
					ep_sub(r, r, t[-u_i]);
				} else {
					ep_add(r, r, t[u_i]);
				}
			}
		}
		ep_norm(r, r);
	}
	CATCH_ANY {
		THROW(ERR_CAUGHT);
	}
	FINALLY {
		for (i = 0; i < 5; i++) {
			ep_free(t[i]);
		}
	}
}

#endif

void ep_mul_sim_gen(ep_t r, bn_t k, ep_t q, bn_t l) {
	ep_t gen;

	ep_null(gen);

	TRY {
		ep_new(gen);

		ep_curve_get_gen(gen);
#if EP_FIX == WTNAF && defined(EP_PRECO)
		ep_mul_sim_ordin(r, gen, k, q, l, 1);
#else
		ep_mul_sim(r, gen, k, q, l);
#endif
	}
	CATCH_ANY {
		THROW(ERR_CAUGHT);
	}
	FINALLY {
		ep_free(gen);
	}
}
