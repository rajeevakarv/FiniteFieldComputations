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
 * Benchmarks for the binary elliptic curve module.
 *
 * @version $Id: bench_eb.c 274 2010-02-06 05:34:38Z dfaranha $
 * @ingroup bench
 */

#include <stdio.h>

#include "relic.h"
#include "relic_bench.h"

static void memory(void) {
	eb_t a[BENCH];

	BENCH_SMALL("eb_null", eb_null(a[i]));

	BENCH_SMALL("eb_new", eb_new(a[i]));
	for (int i = 0; i < BENCH; i++) {
		eb_free(a[i]);
	}

	for (int i = 0; i < BENCH; i++) {
		eb_new(a[i]);
	}
	BENCH_SMALL("eb_free", eb_free(a[i]));

	(void)a;
}

static void util(void) {
	eb_t p, q;

	eb_null(p);
	eb_null(q);

	eb_new(p);
	eb_new(q);

	BENCH_BEGIN("eb_is_infty") {
		eb_rand(p);
		BENCH_ADD(eb_is_infty(p));
	}
	BENCH_END;

	BENCH_BEGIN("eb_set_infty") {
		eb_rand(p);
		BENCH_ADD(eb_set_infty(p));
	}
	BENCH_END;

	BENCH_BEGIN("eb_copy") {
		eb_rand(p);
		eb_rand(q);
		BENCH_ADD(eb_copy(p, q));
	}
	BENCH_END;

	BENCH_BEGIN("eb_cmp") {
		eb_rand(p);
		eb_rand(q);
		BENCH_ADD(eb_cmp(p, q));
	}
	BENCH_END;

	BENCH_BEGIN("eb_rand") {
		BENCH_ADD(eb_rand(p));
	}
	BENCH_END;
}

static void arith(void) {
	eb_t p, q, r, t[EB_TABLE_BASIC];
	bn_t k, l, n;

	eb_null(p);
	eb_null(q);
	eb_null(r);
	for (int i = 0; i < EB_TABLE_BASIC; i++) {
		eb_null(t[i]);
	}

	eb_new(p);
	eb_new(q);
	eb_new(r);
	bn_new(k);
	bn_new(n);
	bn_new(l);

	eb_curve_get_ord(n);

	BENCH_BEGIN("eb_add") {
		eb_rand(p);
		eb_rand(q);
		eb_add(p, p, q);
		eb_rand(q);
		eb_rand(p);
		eb_add(q, q, p);
		BENCH_ADD(eb_add(r, p, q));
	}
	BENCH_END;

#if EB_ADD == BASIC || !defined(STRIP)
	BENCH_BEGIN("eb_add_basic") {
		eb_rand(p);
		eb_rand(q);
		BENCH_ADD(eb_add_basic(r, p, q));
	}
	BENCH_END;
#endif

#if EB_ADD == PROJC || !defined(STRIP)
	BENCH_BEGIN("eb_add_projc") {
		eb_rand(p);
		eb_rand(q);
		eb_add_projc(p, p, q);
		eb_rand(q);
		eb_rand(p);
		eb_add_projc(q, q, p);
		BENCH_ADD(eb_add_projc(r, p, q));
	}
	BENCH_END;

	BENCH_BEGIN("eb_add_projc (z2 = 1)") {
		eb_rand(p);
		eb_rand(q);
		eb_add_projc(p, p, q);
		eb_rand(q);
		eb_norm(q, q);
		BENCH_ADD(eb_add_projc(r, p, q));
	}
	BENCH_END;

	BENCH_BEGIN("eb_add_projc (z1,z2 = 1)") {
		eb_rand(p);
		eb_norm(p, p);
		eb_rand(q);
		eb_norm(q, q);
		BENCH_ADD(eb_add_projc(r, p, q));
	}
	BENCH_END;
#endif

	BENCH_BEGIN("eb_sub") {
		eb_rand(p);
		eb_rand(q);
		eb_add(p, p, q);
		eb_rand(q);
		eb_rand(p);
		eb_add(q, q, p);
		BENCH_ADD(eb_sub(r, p, q));
	}
	BENCH_END;

#if EB_ADD == BASIC || !defined(STRIP)
	BENCH_BEGIN("eb_sub_basic") {
		eb_rand(p);
		eb_rand(q);
		BENCH_ADD(eb_sub_basic(r, p, q));
	}
	BENCH_END;
#endif

#if EB_ADD == PROJC || !defined(STRIP)
	BENCH_BEGIN("eb_sub_projc") {
		eb_rand(p);
		eb_rand(q);
		eb_add_projc(p, p, q);
		eb_rand(q);
		eb_rand(p);
		eb_add_projc(q, q, p);
		BENCH_ADD(eb_sub_projc(r, p, q));
	}
	BENCH_END;

	BENCH_BEGIN("eb_sub_projc (z2 = 1)") {
		eb_rand(p);
		eb_rand(q);
		eb_add_projc(p, p, q);
		eb_rand(q);
		eb_norm(q, q);
		BENCH_ADD(eb_sub_projc(r, p, q));
	}
	BENCH_END;

	BENCH_BEGIN("eb_sub_projc (z1,z2 = 1)") {
		eb_rand(p);
		eb_norm(p, p);
		eb_rand(q);
		eb_norm(q, q);
		BENCH_ADD(eb_sub_projc(r, p, q));
	}
	BENCH_END;
#endif

	BENCH_BEGIN("eb_dbl") {
		eb_rand(p);
		eb_rand(q);
		eb_add(p, p, q);
		BENCH_ADD(eb_dbl(r, p));
	}
	BENCH_END;

#if EB_ADD == BASIC || !defined(STRIP)
	BENCH_BEGIN("eb_dbl_basic") {
		eb_rand(p);
		BENCH_ADD(eb_dbl_basic(r, p));
	}
	BENCH_END;
#endif

#if EB_ADD == PROJC || !defined(STRIP)
	BENCH_BEGIN("eb_dbl_projc") {
		eb_rand(p);
		eb_rand(q);
		eb_add_projc(p, p, q);
		BENCH_ADD(eb_dbl_projc(r, p));
	}
	BENCH_END;

	BENCH_BEGIN("eb_dbl_projc (z1 = 1)") {
		eb_rand(p);
		eb_norm(p, p);
		BENCH_ADD(eb_dbl_projc(r, p));
	}
	BENCH_END;
#endif

#if defined(EB_KBLTZ)
	if (eb_curve_is_kbltz()) {
		BENCH_BEGIN("eb_frb") {
			eb_rand(p);
			eb_rand(q);
			eb_frb(p, q);
			BENCH_ADD(eb_frb(r, p));
		}
		BENCH_END;
	}

#if EB_ADD == BASIC || !defined(STRIP)
	if (eb_curve_is_kbltz()) {
		BENCH_BEGIN("eb_frb_basic") {
			eb_rand(p);
			BENCH_ADD(eb_frb_basic(r, p));
		}
		BENCH_END;
	}
#endif

#if EB_ADD == PROJC || !defined(STRIP)
	if (eb_curve_is_kbltz()) {
		BENCH_BEGIN("eb_frb_projc") {
			eb_rand(p);
			eb_rand(q);
			eb_add_projc(p, p, q);
			BENCH_ADD(eb_frb_projc(r, p));
		}
		BENCH_END;
	}
#endif
#endif

	BENCH_BEGIN("eb_neg") {
		eb_rand(p);
		eb_rand(q);
		eb_add(p, p, q);
		BENCH_ADD(eb_neg(r, p));
	}
	BENCH_END;

#if EB_ADD == BASIC || !defined(STRIP)
	BENCH_BEGIN("eb_neg_basic") {
		eb_rand(p);
		BENCH_ADD(eb_neg_basic(r, p));
	}
	BENCH_END;
#endif

#if EB_ADD == PROJC || !defined(STRIP)
	BENCH_BEGIN("eb_neg_projc") {
		eb_rand(p);
		eb_rand(q);
		eb_add_projc(p, p, q);
		BENCH_ADD(eb_neg_projc(r, p));
	}
	BENCH_END;
#endif

	BENCH_BEGIN("eb_mul") {
		bn_rand(k, BN_POS, bn_bits(n));
		bn_mod(k, k, n);
		BENCH_ADD(eb_mul(q, p, k));
	}
	BENCH_END;

#if EB_MUL == BASIC || !defined(STRIP)
	BENCH_BEGIN("eb_mul_basic") {
		bn_rand(k, BN_POS, bn_bits(n));
		bn_mod(k, k, n);
		BENCH_ADD(eb_mul_basic(q, p, k));
	}
	BENCH_END;
#endif

#if defined(EB_ORDIN) && (EB_MUL == CONST || !defined(STRIP))
	if (!eb_curve_is_super()) {
		BENCH_BEGIN("eb_mul_const") {
			bn_rand(k, BN_POS, bn_bits(n));
			bn_mod(k, k, n);
			eb_rand(p);
			BENCH_ADD(eb_mul_const(q, p, k));
		}
		BENCH_END;
	}
#endif

#if EB_MUL == WTNAF || !defined(STRIP)
	BENCH_BEGIN("eb_mul_wtnaf") {
		bn_rand(k, BN_POS, bn_bits(n));
		bn_mod(k, k, n);
		eb_rand(p);
		BENCH_ADD(eb_mul_wtnaf(q, p, k));
	}
	BENCH_END;
#endif

	BENCH_BEGIN("eb_mul_gen") {
		bn_rand(k, BN_POS, bn_bits(n));
		bn_mod(k, k, n);
		BENCH_ADD(eb_mul_gen(q, k));
	}
	BENCH_END;

	for (int i = 0; i < EB_TABLE_BASIC; i++) {
		eb_new(t[i]);
	}

	BENCH_BEGIN("eb_mul_pre") {
		BENCH_ADD(eb_mul_pre(t, p));
	}
	BENCH_END;

	BENCH_BEGIN("eb_mul_fix") {
		bn_rand(k, BN_POS, bn_bits(n));
		bn_mod(k, k, n);
		eb_mul_pre(t, p);
		eb_rand(p);
		BENCH_ADD(eb_mul_fix(q, t, k));
	}
	BENCH_END;

#if EB_FIX == BASIC || !defined(STRIP)
		BENCH_BEGIN("eb_mul_pre_basic") {
		BENCH_ADD(eb_mul_pre_basic(t, p));
	}
	BENCH_END;

	BENCH_BEGIN("eb_mul_fix_basic") {
		bn_rand(k, BN_POS, bn_bits(n));
		bn_mod(k, k, n);
		eb_rand(p);
		eb_mul_pre_basic(t, p);
		BENCH_ADD(eb_mul_fix_basic(q, t, k));
	}
	BENCH_END;
#endif

#if EB_FIX == YAOWI || !defined(STRIP)
	BENCH_BEGIN("eb_mul_pre_yaowi") {
		BENCH_ADD(eb_mul_pre_yaowi(t, p));
	}
	BENCH_END;

	BENCH_BEGIN("eb_mul_fix_yaowi") {
		bn_rand(k, BN_POS, bn_bits(n));
		bn_mod(k, k, n);
		eb_mul_pre_yaowi(t, p);
		BENCH_ADD(eb_mul_fix_yaowi(q, t, k));
	}
	BENCH_END;
#endif

#if EB_FIX == NAFWI || !defined(STRIP)
	BENCH_BEGIN("eb_mul_pre_nafwi") {
		BENCH_ADD(eb_mul_pre_nafwi(t, p));
	}
	BENCH_END;

	BENCH_BEGIN("eb_mul_fix_nafwi") {
		bn_rand(k, BN_POS, bn_bits(n));
		bn_mod(k, k, n);
		eb_mul_pre_nafwi(t, p);
		BENCH_ADD(eb_mul_fix_nafwi(q, t, k));
	}
	BENCH_END;
#endif

#if EB_FIX == COMBS || !defined(STRIP)
	BENCH_BEGIN("eb_mul_pre_combs") {
		BENCH_ADD(eb_mul_pre_combs(t, p));
	}
	BENCH_END;

	BENCH_BEGIN("eb_mul_fix_combs") {
		bn_rand(k, BN_POS, bn_bits(n));
		bn_mod(k, k, n);
		eb_rand(p);
		eb_mul_pre_combs(t, p);
		BENCH_ADD(eb_mul_fix_combs(q, t, k));
	}
	BENCH_END;
#endif

#if EB_FIX == COMBD || !defined(STRIP)
	BENCH_BEGIN("eb_mul_pre_combd") {
		BENCH_ADD(eb_mul_pre_combd(t, p));
	}
	BENCH_END;

	BENCH_BEGIN("eb_mul_fix_combd") {
		bn_rand(k, BN_POS, bn_bits(n));
		bn_mod(k, k, n);
		eb_mul_pre_combd(t, p);
		BENCH_ADD(eb_mul_fix_combd(q, t, k));
	}
	BENCH_END;
#endif

#if EB_FIX == WTNAF || !defined(STRIP)
	BENCH_BEGIN("eb_mul_pre_wtnaf") {
		BENCH_ADD(eb_mul_pre_wtnaf(t, p));
	}
	BENCH_END;

	BENCH_BEGIN("eb_mul_fix_wtnaf") {
		bn_rand(k, BN_POS, bn_bits(n));
		bn_mod(k, k, n);
		eb_mul_pre_wtnaf(t, p);
		BENCH_ADD(eb_mul_fix_wtnaf(q, t, k));
	}
	BENCH_END;
#endif

	BENCH_BEGIN("eb_mul_sim") {
		bn_rand(k, BN_POS, bn_bits(n));
		bn_mod(k, k, n);
		bn_rand(l, BN_POS, bn_bits(n));
		bn_mod(l, l, n);
		eb_mul(q, p, k);
		BENCH_ADD(eb_mul_sim(r, p, k, q, l));
	}
	BENCH_END;

#if EB_SIM == BASIC || !defined(STRIP)
	BENCH_BEGIN("eb_mul_sim_basic") {
		bn_rand(k, BN_POS, bn_bits(n));
		bn_mod(k, k, n);
		bn_rand(l, BN_POS, bn_bits(n));
		bn_mod(l, l, n);
		eb_mul(q, p, k);
		BENCH_ADD(eb_mul_sim_basic(r, p, k, q, l));
	}
	BENCH_END;
#endif

#if EB_SIM == TRICK || !defined(STRIP)
	BENCH_BEGIN("eb_mul_sim_trick") {
		bn_rand(k, BN_POS, bn_bits(n));
		bn_mod(k, k, n);
		bn_rand(l, BN_POS, bn_bits(n));
		bn_mod(l, l, n);
		eb_mul(q, p, k);
		BENCH_ADD(eb_mul_sim_trick(r, p, k, q, l));
	}
	BENCH_END;
#endif

#if EB_SIM == INTER || !defined(STRIP)
	BENCH_BEGIN("eb_mul_sim_inter") {
		bn_rand(k, BN_POS, bn_bits(n));
		bn_mod(k, k, n);
		bn_rand(l, BN_POS, bn_bits(n));
		bn_mod(l, l, n);
		eb_mul(q, p, k);
		BENCH_ADD(eb_mul_sim_inter(r, p, k, q, l));
	}
	BENCH_END;
#endif

#if EB_SIM == JOINT || !defined(STRIP)
	BENCH_BEGIN("eb_mul_sim_joint") {
		bn_rand(k, BN_POS, bn_bits(n));
		bn_mod(k, k, n);
		bn_rand(l, BN_POS, bn_bits(n));
		bn_mod(l, l, n);
		eb_mul(q, p, k);
		BENCH_ADD(eb_mul_sim_joint(r, p, k, q, l));
	}
	BENCH_END;
#endif

	BENCH_BEGIN("eb_mul_sim_gen") {
		bn_rand(k, BN_POS, bn_bits(n));
		bn_mod(k, k, n);
		bn_rand(l, BN_POS, bn_bits(n));
		bn_mod(l, l, n);
		eb_mul(q, p, k);
		BENCH_ADD(eb_mul_sim_gen(r, k, q, l));
	}
	BENCH_END;

	eb_free(p);
	eb_free(q);
	bn_free(k);
	bn_free(l);
	bn_free(n);
	for (int i = 0; i < EB_TABLE_BASIC; i++) {
		eb_free(t[i]);
	}
}

static void bench(void) {
	eb_param_print();
	util_print_banner("Utilities:", 1);
	memory();
	util();
	util_print_banner("Arithmetic:", 1);
	arith();
}

int main(void) {
	int r0, r1, r2;

	core_init();
	conf_print();
	util_print_banner("Benchmarks for the EB module:", 0);

#if defined(EB_STAND) && defined(EB_ORDIN)
	r0 = eb_param_set_any_ordin();
	if (r0 == STS_OK) {
		bench();
	}
#endif

#if defined(EB_STAND) && defined(EB_KBLTZ)
	r1 = eb_param_set_any_kbltz();
	if (r1 == STS_OK) {
		bench();	}
#endif

#if defined(EB_SUPER)
	r2 = eb_param_set_any_super();
	if (r2 == STS_OK) {
		bench();	}
#endif

	if (r0 == STS_ERR && r1 == STS_ERR && r2 == STS_ERR) {
		eb_param_set_any();
	}
	core_clean();
	return 0;
}
