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
 *
 *
 * MODIFIED BY: Patrick Longa, 06/2011.
 * 
 */

/**
 * @file
 *
 * Implementation of the dodecic extension prime field arithmetic module.
 *
 * @version $Id: relic_pp_fp12.c 214 2010-01-25 03:09:40Z dfaranha $
 * @ingroup fp12
 */

#include <string.h>

#include "relic_core.h"
#include "relic_conf.h"
#include "relic_pp.h"
#include "relic_util.h"

/*============================================================================*/
/* Public definitions                                                         */
/*============================================================================*/

/*
static void fp4_sqr(fp2_t c, fp2_t d, fp2_t a, fp2_t b) {
        fp2_t t0, t1;
 
        fp2_null(t0);
	fp2_null(t1);
 
	TRY {
              fp2_new(t0);
	      fp2_new(t1);
 
	      fp2_add(t0, a, b);          // t0 = a+b
	      fp2_mul_nor(t1, b);         // t1 = E.b
	      fp2_add(t1, t1, a);         // t1 = a + E.b
	      fp2_mul(t0, t0, t1);        // t0 = (a+b)(a+E.b)
	      fp2_mul(d, a, b);           // d = a.b
	      fp2_sub(t0, t0, d);         // t0 = (a+b)(a+E.b) - a.b
	      fp2_mul_nor(t1, d);         // t1 = Ea.b
	      fp2_sub(c, t0, t1);         // c = (a+b)(a+E.b) - a.b - Ea.b
	      fp2_dbl(d, d);              // d = 2a.b
	} CATCH_ANY {
	      THROW(ERR_CAUGHT);
	} FINALLY {
	      fp2_free(t0);
	      fp2_free(t1);
	}
}
*/

static void fp4_sqr(fp2_t c, fp2_t d, fp2_t a, fp2_t b) {
        dv2_t T0, T1, T2;

	TRY {
	      // t0 = a^2. 
	      fp2_sqr_nr(T0, a);
	      // t1 = b^2. 
	      fp2_sqr_nr(T1, b);

	      fp2_add(d, a, b);

	      // c = a^2  + b^2 * E. 
	      fp2_mul_nord(T2, T1);
	      fp2_addd(T2, T2, T0);
	      fp2_rdc(c, T2);

	      // d = (a + b)^2 - a^2 - b^2 = 2 * a * b.
	      fp2_addd(T1, T1, T0); 
	      fp2_sqr_nr(T0, d);
	      fp2_subd(T0, T0, T1);
	      fp2_rdc(d, T0);
	} CATCH_ANY {
   	      THROW(ERR_CAUGHT);
	} 
}

/*
void fp12_mul(fp12_t c, fp12_t a, fp12_t b) {
// Full dodecic extension field multiplication
	fp6_t t0, t1, t2;

	fp6_null(t0);
	fp6_null(t1);
	fp6_null(t2);

	TRY {
		fp6_new(t0);
		fp6_new(t1);
		fp6_new(t2);

		// Karatsuba algorithm
		fp6_mul(t0, a[0], b[0]);
		fp6_mul(t1, a[1], b[1]);
		fp6_add(t2, b[0], b[1]);
		fp6_add(c[1], a[1], a[0]);
		fp6_mul(c[1], c[1], t2);
		fp6_sub(c[1], c[1], t0);
		fp6_sub(c[1], c[1], t1);
		fp6_mul_art(t1, t1);
		fp6_add(c[0], t0, t1);
	} CATCH_ANY {
		THROW(ERR_CAUGHT);
	} FINALLY {
		fp6_free(t0);
		fp6_free(t1);
		fp6_free(t2);
	}
}
*/

void fp12_mul(fp12_t c, fp12_t a, fp12_t b) {
// Full dodecic extension field multiplication using lazy reduction
  fp6_t t0;
        dv6_t T0, T1, T2;

        fp6_null(t0);
        dv6_null(T0);
        dv6_null(T1);
        dv6_null(T2);

        TRY {
                fp6_new(t0);
                dv6_new(T0);
                dv6_new(T1);
                dv6_new(T2);

		// Karatsuba algorithm
		fp6_mul_nr(T0, a[0], b[0]);
		fp6_mul_nr(T1, a[1], b[1]);
		fp6_add(t0, b[0], b[1]);
		fp6_add(c[1], a[1], a[0]);
		fp6_mul_nr(T2, c[1], t0);
		fp6_subd(T2, T2, T0);
		fp6_subd(T2, T2, T1);
		fp6_rdc(c[1], T2);
                fp6_mul_artd(T1, T1);
		fp6_addd(T2, T0, T1);
		fp6_rdc(c[0], T2);
	} CATCH_ANY {
	        THROW(ERR_CAUGHT);
	} FINALLY {
	        fp6_free(t0);
		dv6_free(T0);
                dv6_free(T1);
                dv6_free(T2);
	}
}

/*
void fp12_mul_dxs(fp12_t c, fp12_t a, fp12_t b) {
	fp6_t v0, v1, t0;

	fp6_null(v0);
	fp6_null(v1);
	fp6_null(t0);

	TRY {
		fp6_new(v0);
		fp6_new(v1);
		fp6_new(t0);

		// c1 = (a0 + a1)(b0 + b1) 
		fp6_add(v0, a[0], a[1]);
		fp2_add(v1[0], b[0][0], b[1][0]);
		fp2_copy(v1[1], b[1][1]);
		fp6_mul_dxs(t0, v0, v1);

                // v0 = a0b0 
		fp6_mul_dxq(v0, a[0], b[0][0]);

                // v1 = a1b1 
		fp6_mul(v1, a[1], b[1]);

                // c1 = c1 - v0 - v1 
		fp6_sub(c[1], t0, v0);
		fp6_sub(c[1], c[1], v1);

                // c0 = v0 + v * v1 
		fp6_mul_art(v1, v1);
		fp6_add(c[0], v0, v1);
	} CATCH_ANY {
		THROW(ERR_CAUGHT);
	} FINALLY {
		fp6_free(v0);
		fp6_free(v1);
		fp6_free(t0);
	}
}
*/

/*void fp12_mul_dxs(fp12_t c, fp12_t a, fp12_t b) {
        fp6_t v0, v1;
        dv6_t V0, V1, T0;

	fp6_null(v0);
	fp6_null(v1);
        dv6_null(V0);
        dv6_null(V1);
        dv6_null(T0);

	TRY {
   	        fp6_new(v0);
		fp6_new(v1);
		dv6_new(V0);
                dv6_new(V1);
                dv6_new(T0);

		// c1 = (a0 + a1)(b0 + b1) 
		fp6_add(v0, a[0], a[1]);
		fp2_add(v1[0], b[0][0], b[1][0]);
		fp2_copy(v1[1], b[1][1]);
		fp6_mul_dxs_nr(T0, v0, v1);

		// v0 = a0b0 
		fp6_mul_dxq_nr(V0, a[0], b[0][0]);

		// v1 = a1b1 
		fp6_mul_nr(V1, a[1], b[1]);

		// c1 = T0 - V0 - V1 
		fp6_subd(T0, T0, V0);
		fp6_subd(T0, T0, V1);
                fp6_rdc(c[1], T0);

		// c0 = v0 + v * v1
		fp6_mul_artd(V1, V1);
		fp6_addd(T0, V0, V1);
                fp6_rdc(c[0], T0);
	} CATCH_ANY {
	        THROW(ERR_CAUGHT);
	} FINALLY {
                fp6_free(v0);
		fp6_free(v1);
		dv6_free(V0);
                dv6_free(V1);
                dv6_free(T0);
	}
}*/

void fp12_mul_dxs(fp12_t c, fp12_t a, fp12_t b) {
        fp6_t v0, v1;
        dv6_t V0, V1, T0;

	fp6_null(v0);
	fp6_null(v1);
        dv6_null(V0);
        dv6_null(V1);
        dv6_null(T0);

	TRY {
   	        fp6_new(v0);
		fp6_new(v1);
		dv6_new(V0);
                dv6_new(V1);
                dv6_new(T0);

		// c1 = (a0 + a1)(b0 + b1) 
		fp6_add(v0, a[0], a[1]);
		fp_add(v1[0][0], b[0][0][0], b[1][0][0]);
		fp_copy(v1[0][1], b[1][0][1]);
		fp2_copy(v1[1], b[1][1]);
		fp6_mul_dxs_nr(T0, v0, v1);
		// v0 = a0b0 
		fp6_mul_dxqq_nr(V0, a[0], b[0][0][0]);
		//v1 = a1b1
		fp6_mul_dxs_nr(V1, a[1], b[1]);

		// c1 = T0 - V0 - V1 
		fp6_subd(T0, T0, V0);
		fp6_subd(T0, T0, V1);
                fp6_rdc(c[1], T0);

		// c0 = v0 + v * v1
		fp6_mul_artd(V1, V1);
		fp6_addd(T0, V0, V1);
                fp6_rdc(c[0], T0);
	} CATCH_ANY {
	        THROW(ERR_CAUGHT);
	} FINALLY {
                fp6_free(v0);
		fp6_free(v1);
		dv6_free(V0);
                dv6_free(V1);
                dv6_free(T0);
	}
}

void fp12_mul_sxs(fp12_t c, fp12_t a, fp12_t b) {
        fp2_t v0, v1, v2, v4, v5, v6, v7;


	fp2_null(v0);
	fp2_null(v1);
	fp2_null(v2);
	fp2_null(v4);
	fp2_null(v5);
	fp2_null(v6);
	fp2_null(v7);


   	        fp2_new(v0);
		fp2_new(v1);
		fp2_new(v2);
		fp2_new(v4);
		fp2_new(v5);
		fp2_new(v6);
		fp2_new(v7);

		fp2_mul(v0, a[0][0], b[0][0]);
		fp2_mul(v1, a[1][0], b[1][0]);
		fp2_mul(v2, a[1][1], b[1][1]);

		fp2_mul_nor(v4, v2);

		fp4_mul(v5, a[0][0], a[1][0], b[0][0], b[1][0]);
		fp2_sub(v5, v5, v1);

		fp4_mul(v6, a[0][0], a[1][1], b[0][0], b[1][1]);
		fp2_sub(v6, v6, v2);

		fp4_mul(v7, a[1][0], a[1][1], b[1][0], b[1][1]);
		fp2_sub(v7, v7, v2);

		fp2_sub(c[1][0], v5, v0);
		fp2_sub(c[1][1], v6, v0);
		fp2_sub(c[0][2], v7, v1);

		fp2_zero(c[1][2]);
		fp2_add(c[0][0], v4, v0);
		fp2_copy(c[0][1], v1);


                fp2_free(v0);
		fp2_free(v1);
                fp2_free(v2);
		fp2_free(v4);
		fp2_free(v5);
		fp2_free(v6);
		fp2_free(v7);
}

void fp4_mul(fp2_t c, fp2_t a0, fp2_t a1, fp2_t b0, fp2_t b1)
{
	fp2_t v0, v1;
	fp2_null(v0);
	fp2_null(v1);

	fp2_new(v0);
	fp2_new(v1);
	
	fp2_add(v0, a0, a1);
	fp2_add(v1, b0, b1);
	fp2_mul(c, v0, v1);

	fp2_free(v0);
	fp2_free(v1);
}


void fp12_mul_dxsm(fp12_t c, fp12_t a, fp12_t b) {
        fp6_t v0, v1;
        dv6_t V0, V1, T0;

	fp6_null(v0);
	fp6_null(v1);
        dv6_null(V0);
        dv6_null(V1);
        dv6_null(T0);

	TRY {
   	        fp6_new(v0);
		fp6_new(v1);
		dv6_new(V0);
                dv6_new(V1);
                dv6_new(T0);

		// c1 = (a0 + a1)(b0 + b1) 
		fp6_add(v0, a[0], a[1]);
		fp2_copy(v1[0], b[0][0]);
		fp_add(v1[1][0], b[0][1][0], b[1][1][0]);
		fp_copy(v1[1][1], b[0][1][1]);
		fp6_mul_dxs_nr(T0, v0, v1);
		//v0=a0b0
		fp6_mul_dxs_nr(V0, a[0], b[0]);
		// v1 = a1b1 
		fp6_mul_dxqq2_nr(V1, a[1], b[1][1][0]);

		// c1 = T0 - V0 - V1 
		fp6_subd(T0, T0, V0);
		fp6_subd(T0, T0, V1);
                fp6_rdc(c[1], T0);

		// c0 = v0 + v * v1
		fp6_mul_artd(V1, V1);
		fp6_addd(T0, V0, V1);
                fp6_rdc(c[0], T0);
	} CATCH_ANY {
	        THROW(ERR_CAUGHT);
	} FINALLY {
                fp6_free(v0);
		fp6_free(v1);
		dv6_free(V0);
                dv6_free(V1);
                dv6_free(T0);
	}
}


void fp12_mul_sxsm(fp12_t c, fp12_t a, fp12_t b) {
        fp2_t v0, v1, v2, v4, v5, v6, v7;


	fp2_null(v0);
	fp2_null(v1);
	fp2_null(v2);
	fp2_null(v4);
	fp2_null(v5);
	fp2_null(v6);
	fp2_null(v7);


   	        fp2_new(v0);
		fp2_new(v1);
		fp2_new(v2);
		fp2_new(v4);
		fp2_new(v5);
		fp2_new(v6);
		fp2_new(v7);

		fp2_mul(v0, a[0][0], b[0][0]);
		fp2_mul(v1, a[0][1], b[0][1]);
		fp2_mul(v2, a[1][1], b[1][1]);

		fp2_mul_nor(v4, v2);

		fp4_mul(v5, a[0][0], a[0][1], b[0][0], b[0][1]);
		fp2_sub(v5, v5, v1);

		fp4_mul(v6, a[0][0], a[1][1], b[0][0], b[1][1]);
		fp2_sub(v6, v6, v2);

		fp4_mul(v7, a[0][1], a[1][1], b[0][1], b[1][1]);
		fp2_sub(v7, v7, v2);

		fp2_sub(c[0][1], v5, v0);
		fp2_sub(c[1][1], v6, v0);
		fp2_sub(c[1][2], v7, v1);

		fp2_zero(c[1][0]);
		fp2_add(c[0][0], v4, v0);
		fp2_copy(c[0][2], v1);

                fp2_free(v0);
		fp2_free(v1);
                fp2_free(v2);
		fp2_free(v4);
		fp2_free(v5);
		fp2_free(v6);
		fp2_free(v7);
}

void fp12_sqr(fp12_t c, fp12_t a) {
	fp6_t t0, t1;

	fp6_null(t0);
	fp6_null(t1);

	TRY {
		fp6_new(t0);
		fp6_new(t1);

		fp6_add(t0, a[0], a[1]);      // t0 = a0+a1
		fp6_mul_art(t1, a[1]);        // t1 = v.a1
		fp6_add(t1, a[0], t1);        // t1 = a0+v.a1
		fp6_mul(t0, t0, t1);          // t0 = (a0+a1)(a0+v.a1)
		fp6_mul(c[1], a[0], a[1]);    // c1 = a0*a1
		fp6_sub(c[0], t0, c[1]);      // c0 = (a0+a1)(a0+v.a1) - a0*a1
		fp6_mul_art(t1, c[1]);        // t1 = v.a0*a1
		fp6_sub(c[0], c[0], t1);      // c0 = (a0+a1)(a0+v.a1) - a0*a1 - v.a0*a1
		fp6_dbl(c[1], c[1]);          // c1 = 2a0*a1
	} CATCH_ANY {
		THROW(ERR_CAUGHT);
	} FINALLY {
		fp6_free(t0);
		fp6_free(t1);
	}
}

/*
void fp12_sqr_uni(fp12_t c, fp12_t a) {
	fp6_t t0, t1;
	fp_t one;

	fp6_null(t0);
	fp6_null(t1);

	TRY {
		fp6_new(t0);
		fp6_new(t1);
		fp_new(one);

		// (a0 + a1 * w)^2 = (2b^2v + 1) + ((a + b)^2 - b^2 - b^2v - 1) * w
		fp6_sqr(t0, a[1]);
		fp6_add(t1, a[0], a[1]);
		fp6_sqr(t1, t1);
		fp6_sub(t1, t1, t0);
		fp6_mul_art(c[0], t0);
		fp6_sub(c[1], t1, c[0]);
		fp_set_dig(one, 1);
		fp6_dbl(c[0], c[0]);
		fp_add_dig(c[0][0][0], c[0][0][0], 1);
		fp_sub(c[1][0][0], c[1][0][0], one);
	} CATCH_ANY {
		THROW(ERR_CAUGHT);
	} FINALLY {
		fp6_free(t0);
		fp6_free(t1);
		fp_free(one);
	}
}
*/

void fp12_sqr_uni(fp12_t c, fp12_t a) {
        fp2_t t0, t1, t2, t3;

	// Define z = sqrt(E)

	// Now a is seen as (t0,t1) + (t2,t3) * w + (t4,t5) * w^2
	
	// (t0, t1) = (a00 + a11*z)^2
	fp4_sqr(t0, t1, a[0][0], a[1][1]);

	fp2_sub(c[0][0], t0, a[0][0]);
	fp2_add(c[0][0], c[0][0], c[0][0]);
	fp2_add(c[0][0], t0, c[0][0]);

	fp2_add(c[1][1], t1, a[1][1]);
	fp2_add(c[1][1], c[1][1], c[1][1]);
	fp2_add(c[1][1], t1, c[1][1]);

	// (t4, t5) = (a01 + a12*z)^2
	fp4_sqr(t2, t0, a[0][1], a[1][2]);
	fp2_mul_nor(t3, t0);
	// (t2, t3) = (a10 + a02*z)^2
	fp4_sqr(t0, t1, a[1][0], a[0][2]);

	fp2_sub(c[0][1], t0, a[0][1]);
	fp2_add(c[0][1], c[0][1], c[0][1]);
	fp2_add(c[0][1], t0, c[0][1]);

	fp2_add(c[1][2], t1, a[1][2]);
	fp2_add(c[1][2], c[1][2], c[1][2]);
	fp2_add(c[1][2], t1, c[1][2]);

	fp2_sub(c[0][2], t2, a[0][2]);
	fp2_add(c[0][2], c[0][2], c[0][2]);
	fp2_add(c[0][2], t2, c[0][2]);

	fp2_add(c[1][0], t3, a[1][0]);
	fp2_add(c[1][0], c[1][0], c[1][0]);
	fp2_add(c[1][0], t3, c[1][0]);
}

void fp12_inv(fp12_t c, fp12_t a) {
	fp6_t t0;
	dv6_t T0;
	dv6_t T1;

	fp6_null(t0);
	fp6_null(T0);
	fp6_null(T1);

	TRY {
		fp6_new(t0);
		dv6_new(T0);
		dv6_new(T1);

		fp6_sqr_nr(T0, a[0]);
		fp6_sqr_nr(T1, a[1]);
		fp6_mul_artd(T1, T1);
		fp6_subd(T0, T0, T1);
		fp6_rdc(t0, T0);
		fp6_inv(t0, t0);

		fp6_mul(c[0], a[0], t0);
		fp6_neg(c[1], a[1]);
		fp6_mul(c[1], c[1], t0);
	} CATCH_ANY {
		THROW(ERR_CAUGHT);
	} FINALLY {
		fp6_free(t0);
		dv6_free(T0);
		dv6_free(T1);
	}
}

void fp12_inv_uni(fp12_t c, fp12_t a) {
	/* In this case, it's a simple conjugate. */
	fp6_copy(c[0], a[0]);
	fp6_neg(c[1], a[1]);
}

/*
void fp12_frb(fp12_t c, fp12_t a) {
	fp2_t t;

	fp2_null(t);

	TRY {
		fp2_new(t);

		fp2_const_get(t);
		fp6_frb(c[0], a[0]);
		fp6_frb(c[1], a[1]);
		fp6_mul_dxq(c[1], c[1], t);
	} CATCH_ANY {
		THROW(ERR_CAUGHT);
	} FINALLY {
		fp2_free(t);
	}
}
*/

void fp12_frb(fp12_t c, fp12_t a) {
        // t1 = conj(a00)
        fp2_frb(c[0][0], a[0][0]);
	// t2 = conj(a10)
	fp2_frb(c[1][0], a[1][0]);
	// t3 = conj(a01)
	fp2_frb(c[0][1], a[0][1]);
	// t4 = conj(a11)
	fp2_frb(c[1][1], a[1][1]);
	// t5 = conj(a02)
	fp2_frb(c[0][2], a[0][2]);
	// t6 = conj(a12)
	fp2_frb(c[1][2], a[1][2]);

	fp2_mul_frb(c[1][0], c[1][0], 1);
	fp2_mul_frb(c[0][1], c[0][1], 2);
	fp2_mul_frb(c[1][1], c[1][1], 3);
	fp2_mul_frb(c[0][2], c[0][2], 4);
	fp2_mul_frb(c[1][2], c[1][2], 5);
}

void fp12_frb_sqr(fp12_t c, fp12_t a) {
        fp2_copy(c[0][0], a[0][0]);
	fp2_mul_frb_sqr(c[0][1], a[0][1], 2);
	fp2_mul_frb_sqr(c[0][2], a[0][2], 1);
	fp2_neg(c[0][2], c[0][2]);
	fp2_mul_frb_sqr(c[1][0], a[1][0], 1);
	fp2_mul_frb_sqr(c[1][1], a[1][1], 3);
	fp2_mul_frb_sqr(c[1][2], a[1][2], 2);
	fp2_neg(c[1][2], c[1][2]);
}

void fp12_exp(fp12_t c, fp12_t a, bn_t b) {
	fp12_t t;

	fp12_null(t);

	TRY {
		fp12_new(t);

		fp12_copy(t, a);

		for (int i = bn_bits(b) - 2; i >= 0; i--) {
			fp12_sqr(t, t);
			if (bn_test_bit(b, i)) {
				fp12_mul(t, t, a);
			}
		}
		fp12_copy(c, t);
	}
	CATCH_ANY {
		THROW(ERR_CAUGHT);
	}
	FINALLY {
		fp12_free(t);
	}
}

/*
void fp12_exp_uni(fp12_t c, fp12_t a, bn_t b) {
// When using "traditional" cyclotomic squarings 
	fp12_t t;

	fp12_null(t);

	TRY {
		fp12_new(t);

		fp12_copy(t, a);

		for (int i = bn_bits(b) - 2; i >= 0; i--) {
			fp12_sqr_uni(t, t);
			if (bn_test_bit(b, i)) {
				fp12_mul(t, t, a);
			}
		}
		fp12_copy(c, t);
	}
	CATCH_ANY {
		THROW(ERR_CAUGHT);
	}
	FINALLY {
		fp12_free(t);
	}
}
*/

void fp12_exp_uni(fp12_t c, fp12_t a, bn_t b) {
// When using new compressed squarings
        fp12_t t0, t1, t2;
	int i;
	
	fp12_null(t0);
	fp12_null(t1);
	fp12_null(t2);

	TRY {
	        fp12_new(t0);
		fp12_new(t1);
		fp12_new(t2);
		
		switch (fp_param_get()){
			case BNN_256:
		
			fp12_copy(t0, a);
			for (i = 0; i < 55; i++)
		        fp12_sqr_cyc(t0, t0);
			fp12_copy(t1, t0);
			for (; i < 62; i++)
		        fp12_sqr_cyc(t1, t1);
		
			fp12_conv_back_sim(t1, t0, t1, t0);
			fp12_mul(c, t0, a);
			fp12_mul(c, c, t1);
			break;
			case BN_286:

			fp12_copy(t0, a);
			for (i = 0; i < 38; i++)
		        fp12_sqr_cyc(t0, t0);
			fp12_copy(t1, t0);
			for (; i < 58; i++)
		        fp12_sqr_cyc(t1, t1);
			fp12_copy(t2, t1);
			for (; i < 70; i++)
			fp12_sqr_cyc(t2, t2);
		
			fp12_conv_back_sim(t1, t0, t1, t0);
			fp12_conv_back(t2, t2);
			//fp12_conv_back3_sim(t2, t1, t0, t2, t1, t0);
			fp12_mul(c, t0, a);
			fp12_mul(c, c, t1);
			fp12_mul(c, c, t2);
			break;
			case BN_478:

			fp12_copy(t0, a);
			for (i = 0; i < 19; i++)
		        fp12_sqr_cyc(t0, t0);
			fp12_copy(t1, t0);
			for (; i < 55; i++)
		        fp12_sqr_cyc(t1, t1);
			fp12_copy(t2, t1);
			for (; i < 118; i++)
			fp12_sqr_cyc(t2, t2);
		
			fp12_conv_back_sim(t1, t0, t1, t0);
			fp12_conv_back(t2, t2);
			//fp12_conv_back3_sim(t2, t1, t0, t2, t1, t0);
			fp12_mul(c, t1, t0);
			fp12_inv_uni(c, c);
			fp12_mul(c, c, a);
			fp12_mul(c, c, t2);
			break;
			case BN_446:

			fp12_copy(t0, a);
			for (i = 0; i < 36; i++)
		        fp12_sqr_cyc(t0, t0);
			fp12_copy(t1, t0);
			for (; i < 110; i++)
		        fp12_sqr_cyc(t1, t1);
		
			fp12_conv_back_sim(t1, t0, t1, t0);
			fp12_mul(c, t0, a);
			fp12_mul(c, c, t1);
			break;
			case BN_606:

			fp12_copy(t0, a);
			for (i = 0; i < 8; i++)
		        fp12_sqr_cyc(t0, t0);
			fp12_copy(t1, t0);
			for (; i < 95; i++)
		        fp12_sqr_cyc(t1, t1);
			fp12_copy(t2, t1);
			for (; i < 150; i++)
			fp12_sqr_cyc(t2, t2);
		
			fp12_conv_back_sim(t1, t0, t1, t0);
			fp12_conv_back(t2, t2);
			//fp12_conv_back3_sim(t2, t1, t0, t2, t1, t0);
			fp12_inv_uni(t1, t1);
			fp12_mul(c, t0, a);
			fp12_mul(c, c, t1);
			fp12_mul(c, c, t2);
			break;
			case BN_638:

			fp12_copy(t0, a);
			for (i = 0; i < 68; i++)
		        fp12_sqr_cyc(t0, t0);
			fp12_copy(t1, t0);
			for (; i < 128; i++)
		        fp12_sqr_cyc(t1, t1);
			fp12_copy(t2, t1);
			for (; i < 158; i++)
			fp12_sqr_cyc(t2, t2);
		
			fp12_conv_back_sim(t1, t0, t1, t0);
			fp12_conv_back(t2, t2);
			//fp12_conv_back3_sim(t2, t1, t0, t2, t1, t0);
			fp12_inv_uni(t1, t1);
			fp12_inv_uni(t0, t0);
			fp12_mul(c, t0, a);
			fp12_mul(c, c, t1);
			fp12_mul(c, c, t2);
			break;
		}
	}
	CATCH_ANY {
	        THROW(ERR_CAUGHT);
	}
	FINALLY {
	        fp12_free(t0);
                fp12_free(t1);
	}
}

void fp12_exp_cyc(fp12_t c2, fp12_t c1, fp12_t c0, fp12_t a, bn_t b) {
        fp12_t t0, t1;
	int i;

	fp12_null(t0);
	fp12_null(t1);

	TRY {
	       fp12_new(t0);
	       fp12_new(t1);

	       fp12_sqr_cyc(t0, a);
	       for (i = 1; i < 55; i++)
		      fp12_sqr_cyc(t0, t0);
	       fp12_sqr_cyc(t1, t0);
	       for (; i < 61; i++)
		      fp12_sqr_cyc(t1, t1);
	       
	       fp12_conv_back_sim(t1, t0, t1, t0);
	       fp12_mul(c0, t0, a);
	       fp12_mul(c0, c0, t1);
	       
	       fp12_sqr_cyc(t0, c0);
	       for (i = 1; i < 55; i++)
		      fp12_sqr_cyc(t0, t0);
	       fp12_sqr_cyc(t1, t0);
	       for (; i < 61; i++)
		      fp12_sqr_cyc(t1, t1);
	       
	       fp12_conv_back_sim(t1, t0, t1, t0);
	       fp12_mul(c1, t0, c0);
	       fp12_mul(c1, c1, t1);
	       
	       fp12_sqr_cyc(t0, c1);
	       for (i = 1; i < 55; i++)
		      fp12_sqr_cyc(t0, t0);
	       fp12_sqr_cyc(t1, t0);
	       for (; i < 61; i++)
		      fp12_sqr_cyc(t1, t1);
	       
	       fp12_conv_back_sim(t1, t0, t1, t0);
	       fp12_mul(c2, t0, c1);
	       fp12_mul(c2, c2, t1);
	       
	}
	CATCH_ANY {
	       THROW(ERR_CAUGHT);
	}
	FINALLY {
	       fp12_free(t0);
	       fp12_free(t1);
  }
}

/*
void fp12_sqr_cyc(fp12_t c, fp12_t a) {
// New compressed squaring
        fp2_t t0, t1, t2, t3, t4, t5, t6;
 
	fp2_sqr(t0, a[0][1]);
	fp2_sqr(t1, a[1][2]);
	fp2_add(t5, a[0][1], a[1][2]);
	fp2_sqr(t2, t5);
	
	fp2_add(t3, t0, t1);
	fp2_sub(t5, t2, t3); 
	
	fp2_add(t6, a[1][0], a[0][2]);
	fp2_sqr(t3, t6);
	fp2_sqr(t2, a[1][0]);
	
	fp2_mul_nor(t6, t5);
	fp2_add(t5, t6, a[1][0]);
	fp2_dbl(t5, t5);
	fp2_add(c[1][0], t5, t6);
	
	fp2_mul_nor(t4, t1);
	fp2_add(t5, t0, t4);
	fp2_sub(t6, t5, a[0][2]);
	
	fp2_sqr(t1, a[0][2]);
	
	fp2_dbl(t6, t6);
	fp2_add(c[0][2], t6, t5); 
	
	fp2_mul_nor(t4, t1);
	fp2_add(t5, t2, t4);
	fp2_sub(t6, t5, a[0][1]);
	fp2_dbl(t6, t6);
	fp2_add(c[0][1], t6, t5);
	
	fp2_add(t0, t2, t1);
	fp2_sub(t5, t3, t0);
	fp2_add(t6, t5, a[1][2]);
	fp2_dbl(t6, t6);
	fp2_add(c[1][2], t5, t6);
}
*/

void fp12_sqr_cyc(fp12_t c, fp12_t a) {
// New compressed squaring using lazy reduction 
       fp2_t t0, t1;
       dv2_t T0, T1, T2, T3, T4;
 
       fp2_sqr_nr(T0, a[0][1]);
       fp2_sqr_nr(T1, a[1][2]);
       fp2_add(t0, a[0][1], a[1][2]);
       fp2_sqr_nr(T2, t0);
  
       fp2_addd(T3, T0, T1);
       fp2_subd(T3, T2, T3); 
       fp2_rdc(t0, T3);

       fp2_add(t1, a[1][0], a[0][2]);
       fp2_sqr_nr(T3, t1);
       fp2_sqr_nr(T2, a[1][0]);

       fp2_mul_nor(t1, t0);
       fp2_add(t0, t1, a[1][0]);
       fp2_dbl(t0, t0);
       fp2_add(c[1][0], t0, t1);
  
       fp2_mul_nord(T4, T1);
       fp2_addd(T4, T0, T4);
       fp2_rdc(t0, T4);
       fp2_sub(t1, t0, a[0][2]);

       fp2_sqr_nr(T1, a[0][2]);
       
       fp2_dbl(t1, t1);
       fp2_add(c[0][2], t1, t0); 

       fp2_mul_nord(T4, T1);
       fp2_addd(T4, T2, T4);
       fp2_rdc(t0, T4);
       fp2_sub(t1, t0, a[0][1]);
       fp2_dbl(t1, t1);
       fp2_add(c[0][1], t1, t0);
 
       fp2_addd(T0, T2, T1);
       fp2_subd(T3, T3, T0);
       fp2_rdc(t0, T3);
       fp2_add(t1, t0, a[1][2]);
       fp2_dbl(t1, t1);
       fp2_add(c[1][2], t0, t1);
}

void fp12_conv_back(fp12_t c, fp12_t a) {
        fp2_t t0, t1, t2;

	fp2_null(t0);
	fp2_null(t1);
	fp2_null(t2);
	
	TRY {
	        fp2_new(t0);
		fp2_new(t1);
		fp2_new(t2);
		
		// t0 = g4^2.
		fp2_sqr(t0, a[0][1]);
		// t1 = 3 * g4^2 - 2 * g3. 
		fp2_sub(t1, t0, a[0][2]);
		fp2_dbl(t1, t1);
		fp2_add(t1, t1, t0);
		// t0 = E * g5^2 + t1. 
		fp2_sqr(t2, a[1][2]);
		fp2_mul_nor(t0, t2);
		fp2_add(t0, t0, t1);
		// t1 = 1/(4 * g2). 
		fp2_dbl(t1, a[1][0]);
		fp2_dbl(t1, t1);
		fp2_inv(t1, t1);
		// t0 = g1. 
		fp2_mul(c[1][1], t0, t1);
		
		// t1 = g3 * g4. 
		fp2_mul(t1, a[0][2], a[0][1]);
		// t2 = 2 * g1^2 - 3 * g3 * g4. 
		fp2_sqr(t2, c[1][1]);
		fp2_sub(t2, t2, t1);
		fp2_dbl(t2, t2);
		fp2_sub(t2, t2, t1);
		// t1 = g2 * g5. 
		fp2_mul(t1, a[1][0], a[1][2]);
		// t2 = E * (2 * g1^2 + g2 * g5 - 3 * g3 * g4) + 1. 
		fp2_add(t2, t2, t1);
		fp2_mul_nor(c[0][0], t2);
		fp_add_dig(c[0][0][0], c[0][0][0], 1);
	} CATCH_ANY {
	        THROW(ERR_CAUGHT);
	} FINALLY {
	        fp2_free(t0);
		fp2_free(t1);
		fp2_free(t2);
  }
}

void fp12_conv_back_sim(fp12_t c1, fp12_t c0, fp12_t a1, fp12_t a0) {
        fp2_t t0, t1, t2, t3, t4;

	fp2_null(t0);
	fp2_null(t1);
	fp2_null(t2);
	fp2_null(t3);
	fp2_null(t4);

	TRY {
	        fp2_new(t0);
		fp2_new(t1);
		fp2_new(t2);
		fp2_new(t3);
		fp2_new(t4);
		fp2_new(t5);
		
		/* t0 = g4^2. */
		fp2_sqr(t0, a0[0][1]);
		/* t1 = 3 * g4^2 - 2 * g3. */
		fp2_sub(t1, t0, a0[0][2]);
		fp2_dbl(t1, t1);
		fp2_add(t1, t1, t0);
		/* t0 = E * g5^2 + t1. */
		fp2_sqr(t2, a0[1][2]);
		fp2_mul_nor(t0, t2);
		fp2_add(t0, t0, t1);
		/* t1 = 1/(4 * g2). */
		
		/* t2 = g4^2. */
		fp2_sqr(t2, a1[0][1]);
		/* t3 = 3 * g4^2 - 2 * g3. */
		fp2_sub(t3, t2, a1[0][2]);
		fp2_dbl(t3, t3);
			
		fp2_add(t3, t3, t2);
		/* t2 = E * g5^2 + t3. */
		fp2_sqr(t4, a1[1][2]);
		fp2_mul_nor(t2, t4);
		fp2_add(t2, t2, t3);
		/* t3 = 1/(4 * g2). */
		
		fp2_mul(t4, a0[1][0], a1[1][0]);
		fp2_dbl(t4, t4);
		fp2_dbl(t4, t4);
		
		fp2_inv(t4, t4);
		fp2_mul(t1, t4, a1[1][0]);
		fp2_mul(t3, t4, a0[1][0]);
		
		/* t0 = g1. */
		fp2_mul(c0[1][1], t0, t1);
		/* t2 = g1. */
		fp2_mul(c1[1][1], t2, t3);
		
		/* t1 = g3 * g4. */
		fp2_mul(t1, a0[0][2], a0[0][1]);
		/* t2 = 2 * g1^2 - 3 * g3 * g4. */
		fp2_sqr(t2, c0[1][1]);
		fp2_sub(t2, t2, t1);
		fp2_dbl(t2, t2);
		fp2_sub(t2, t2, t1);
		/* t1 = g2 * g5. */
		fp2_mul(t1, a0[1][0], a0[1][2]);
		/* t2 = E * (2 * g1^2 + g2 * g5 - 3 * g3 * g4) + 1. */
		fp2_add(t2, t2, t1);
		fp2_mul_nor(c0[0][0], t2);
		fp_add_dig(c0[0][0][0], c0[0][0][0], 1);
		
		/* t1 = g3 * g4. */
		fp2_mul(t1, a1[0][2], a1[0][1]);
		/* t2 = 2 * g1^2 - 3 * g3 * g4. */
		fp2_sqr(t2, c1[1][1]);
		fp2_sub(t2, t2, t1);
		fp2_dbl(t2, t2);
		fp2_sub(t2, t2, t1);
		/* t1 = g2 * g5. */
		fp2_mul(t1, a1[1][0], a1[1][2]);
		/* t2 = E * (2 * g1^2 + g2 * g5 - 3 * g3 * g4) + 1. */
		fp2_add(t2, t2, t1);
		fp2_mul_nor(c1[0][0], t2);
		fp_add_dig(c1[0][0][0], c1[0][0][0], 1);
	} CATCH_ANY {
	        THROW(ERR_CAUGHT);
	} FINALLY {
	        fp2_free(t0);
		fp2_free(t1);
		fp2_free(t2);
		fp2_free(t3);
		fp2_free(t4);
	}
}
