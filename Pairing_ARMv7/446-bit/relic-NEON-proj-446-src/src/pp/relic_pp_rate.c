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
 * MODIFIED BY: Patrick longa, 06/2011.
 *
 */

/**
 * @file
 *
 * Implementation of the R-Ate bilinear pairing.
 *
 * @version $Id: relic_pp_rate.c 214 2010-01-25 03:09:40Z dfaranha $
 * @ingroup pp
 */

#include "relic_core.h"
#include "relic_pp.h"
#include "relic_error.h"

/*============================================================================*/
/* Private definitions                                                        */
/*============================================================================*/

void fp2_mulb(fp2_t c, fp2_t a)
{
	fp_add(c[0], a[0], a[1]);
	fp_sub(c[1], a[1], a[0]);
}



void fp2_mulbm(fp2_t c, fp2_t a)
{
	fp2_t b;

	fp2_null(b);
	fp2_new(b);

	fp2_mul_nor(b, a);
	fp2_dbl(c, b);
	fp2_dbl(c, c);
	fp2_dbl(c, c);
	fp2_dbl(c, c);
	fp2_dbl(c, c);
	fp2_dbl(c, c);
	fp2_dbl(c, c);
	fp2_dbl(c, c);
	fp2_add(c, c, b);	

}

void pp_add(fp12_t l, ep2_t r, ep2_t q, ep_t p) {

	fp2_t t1, t2, t3, t4, t5;
	dv2_t T0, T1;

	fp2_null(t1);
	fp2_null(t2);
	fp2_null(t3);
	fp2_null(t4);
	fp2_null(t5);	
	dv2_null(T0);
	dv2_null(T1);

	//		t1:=Z1*X2: t1:=X1-t1: t2:=Z1*Y2: t2:=Y1-t2: gADD:=c1*t2-t1*Y2+t1*yQ:
	TRY{
		fp2_new(t1);
		fp2_new(t2);
		fp2_new(t3);
		fp2_new(t4);
		fp2_new(t5);
		dv2_new(T0);
		dv2_new(T1);

		fp2_mul(t1, r->z, q->x);
		fp2_sub(t1, r->x, t1);
		fp2_mul(t2, r->z, q->y);
		fp2_sub(t2, r->y, t2);
		//		t3:=t1^2: t3:=c*t3: X3:=t3*X1: t3:=t1*t3: t4:=t2^2: t4:=t4*Z1: t4:=t3+t4:
		fp2_sqr(t3, t1);
		fp2_mul(r->x, t3, r->x);
		fp2_mul(t3, t1, t3);
		fp2_sqr(t4, t2);
		fp2_mul(t4, t4, r->z);
		fp2_add(t4, t3, t4);

		fp_mul(l[1][0][0], t2[0], p->x);
		fp_mul(l[1][0][1], t2[1], p->x);
		fp2_neg(l[1][0], l[1][0]);

		// l00 = x2 * t2 - y2 * t1. 
		

		//		t4:=t4-X3: t4:=t4-X3: X3:=X3-t4: t2:=t2*X3: Y3:=t3*Y1: Y3:=t2-Y3: X3:=t1*t4: Z3:=Z1*t3:
		fp2_sub(t4, t4, r->x);
		fp2_sub(t4, t4, r->x);
		fp2_sub(r->x, r->x, t4);

		fp2_mul_nr(T0, t2, r->x);
		fp2_mul_nr(T1, t3, r->y);
		fp2_subd(T0, T0, T1);
		fp2_rdc(r->y, T0);	

		fp2_mul(r->x, t1, t4);
		fp2_mul(r->z, r->z, t3);

		fp2_mul_nr(T0, q->x, t2);
		fp2_mul_nr(T1, q->y, t1);
		fp2_subd(T0, T0, T1);
		fp2_rdc(l[1][1], T0);


		// l01 = t1 * yp. 
		fp_mul(l[0][0][0], t1[0], p->y);
		fp_mul(l[0][0][1], t1[1], p->y);

		r->norm = 0;
	}
	CATCH_ANY {
		THROW(ERR_CAUGHT);
	}
	FINALLY {
		fp2_free(t1);
		fp2_free(t2);
		fp2_free(t3);
		fp2_free(t4);
		fp2_free(t5);
		dv2_free(T0);
		dv2_free(T1);		
	}
}

void pp_dbl(fp12_t l, ep2_t r, ep2_t q, ep_t p) {

	fp2_t t0, t1, t2, t3, t4, t5;
	dv2_t T0, T1;

	fp2_null(t0);
	fp2_null(t1);
	fp2_null(t2);
	fp2_null(t3);
	fp2_null(t4);
	fp2_null(t5);
	dv2_null(T0);
	dv2_null(T1);

	TRY{
		fp2_new(t0);
		fp2_new(t1);
		fp2_new(t2);
		fp2_new(t3);
		fp2_new(t4);
		fp2_new(t5);
		dv2_new(T0);
		dv2_new(T1);

		fp2_sqr(t0, q->x); // t0 = x1^2
		fp2_sqr(t1, q->y); // t1 = y1^2
		fp2_sqr(t2, q->z); // t2 = z1^2
		fp2_add(t5, t1, t2);
			
#if ARCH == ARM
		fp2_add(t4, q->x, q->y);
		fp2_sqr(t4, t4);
		fp2_sub(t4, t4, t1);
		fp2_sub(t4, t4, t0);
		fp2_div2(t3, t4);
		fp2_div2(t4, t3); 	
#else
		fp2_mul(t3, q->x, q->y); // t3 = x1y1	
		fp2_div2(t4, t3); // t4 = x1y1/2
#endif

		fp2_dbl(t3, t2); // t3 = 2z1^2
		fp2_add(t2, t2, t3); // t2 = 3z1^2
		
		fp2_mulb(t3, t2);

		fp2_dbl(t2, t3);
		fp2_add(t2, t3, t2); // t2 = 9b.z1^2
		fp2_sub(r->x, t1, t2); // x3 = y1^2 - 9b.z1^2
		fp2_mul(r->x, r->x, t4); // x3 = (y1^2 - 9b.z1^2)x1y1/2

		fp2_add(t4, t1, t2); // t4 = y1^2 + 9b.z1^2

		fp2_add(t2, q->y, q->z);
		fp2_sqr(t2, t2);		
		fp2_sub(t5, t2, t5);

		fp2_div2(t2, t4); // t2 = (y1^2 + 9b.z1^2)/2
		
		fp2_sqr_nr(T0, t3);
		fp2_addd(T1, T0, T0);
		fp2_addd(T1, T0, T1);

		fp2_sqr_nr(T0, t2);	
		fp2_subd(T0, T0, T1);
		fp2_rdc(r->y, T0);		

		fp2_mul(r->z, t5, t1); // z3 = 2y1^3.z1
		fp2_sub(l[1][1], t3, t1); // t3 = 3b.z1^2 - y1^2

		fp2_add(l[1][0], t0, t0);
		fp2_add(l[1][0], l[1][0], t0);
		fp_mul(l[1][0][0], l[1][0][0], p->x);
		fp_mul(l[1][0][1], l[1][0][1], p->x); // l02 = (3x1^2)xp

		fp_mul(l[0][0][0], t5[0], p->y);
		fp_mul(l[0][0][1], t5[1], p->y); // l11 = (2y1z1)yp
		//fp2_neg(l[0][0], l[0][0]);		// l11 = -(2y1z1)yp

		r->norm = 0;
	}
	CATCH_ANY {
		THROW(ERR_CAUGHT);
	}
	FINALLY {
		fp2_free(t0);
		fp2_free(t1);
		fp2_free(t2);
		fp2_free(t3);
		fp2_free(t4);
		fp2_free(t5);
		dv2_free(T0);
		dv2_free(T1);
		
	}
}

void pp_dblf(fp12_t l, ep2_t r, ep2_t q, ep_t p) {

	fp2_t t0, t1, t2, t3, t4, t5;

	fp2_null(t0);
	fp2_null(t1);
	fp2_null(t2);
	fp2_null(t3);
	fp2_null(t4);
	fp2_null(t5);

	TRY{
		fp2_new(t0);
		fp2_new(t1);
		fp2_new(t2);
		fp2_new(t3);
		fp2_new(t4);
		fp2_new(t5);

		fp2_sqr(t0, q->x); // t0 = x1^2
		fp2_sqr(t1, q->y); // t1 = y1^2
		fp2_mul(t3, q->x, q->y); // t3 = x1y1

//	
		fp2_div2(t4, t3); // t4 = x1y1/2				
		
		fp_zero(t3[1]);
		fp_set_dig(t3[0], 3);		
		fp2_mulb(t3, t3);

		fp2_dbl(t2, t3);
		fp2_add(t2, t3, t2); // t2 = 9b.z1^2
		fp2_sub(r->x, t1, t2); // x3 = y1^2 - 9b.z1^2
		fp2_mul(r->x, r->x, t4); // x3 = (y1^2 - 9b.z1^2)x1y1/2

		fp2_add(t4, t1, t2); // t4 = y1^2 + 9b.z1^2
//
		fp2_div2(t2, t4); // t2 = (y1^2 + 9b.z1^2)/2
		fp2_sqr(t2, t2); // t2 = [(y1^2 + 9b.z1^2)/2]^2
		fp2_sqr(t4, t3); // t4 = 9b^2.z1^4
		fp2_copy(t5, q->y); // t5 = y1z1
		fp2_dbl(r->y, t4); // y3 = 18b^2.z1^4
		fp2_add(r->y, r->y, t4); // y3 = 27b^2.z1^4
		fp2_sub(r->y, t2, r->y); // y3 = [(y1^2 + 9b.z1^2)/2]^2 - 27b^2.z1^4

		fp2_dbl(t2, t5); // t2 = 2y1z1
		fp2_mul(r->z, t2, t1); // z3 = 2y1^3.z1

		fp2_sub(l[1][1], t3, t1); // t3 = 3b.z1^2 - y1^2

		fp2_add(l[1][0], t0, t0);
		fp2_add(l[1][0], l[1][0], t0);
		fp_mul(l[1][0][0], l[1][0][0], p->x);
		fp_mul(l[1][0][1], l[1][0][1], p->x); // l02 = (3x1^2)xp

		fp_mul(l[0][0][0], t2[0], p->y);
		fp_mul(l[0][0][1], t2[1], p->y); // l11 = (2y1z1)yp
		//fp2_neg(l[0][0], l[0][0]);		// l11 = -(2y1z1)yp

		r->norm = 0;
	}
	CATCH_ANY {
		THROW(ERR_CAUGHT);
	}
	FINALLY {
		fp2_free(t0);
		fp2_free(t1);
		fp2_free(t2);
		fp2_free(t3);
		fp2_free(t4);
		fp2_free(t5);
		
	}
}

void pp_addm(fp12_t l, ep2_t r, ep2_t q, ep_t p) {

	fp2_t t1, t2, t3, t4, t5;
	dv2_t T0, T1;

	fp2_null(t1);
	fp2_null(t2);
	fp2_null(t3);
	fp2_null(t4);
	fp2_null(t5);
	dv2_null(T0);
	dv2_null(T1);	

	//		t1:=Z1*X2: t1:=X1-t1: t2:=Z1*Y2: t2:=Y1-t2: gADD:=c1*t2-t1*Y2+t1*yQ:
	TRY{
		fp2_new(t1);
		fp2_new(t2);
		fp2_new(t3);
		fp2_new(t4);
		fp2_new(t5);
		dv2_new(T0);
		dv2_new(T1);

		fp2_mul(t1, r->z, q->x);
		fp2_sub(t1, r->x, t1);
		fp2_mul(t2, r->z, q->y);
		fp2_sub(t2, r->y, t2);
		//		t3:=t1^2: t3:=c*t3: X3:=t3*X1: t3:=t1*t3: t4:=t2^2: t4:=t4*Z1: t4:=t3+t4:
		fp2_sqr(t3, t1);
		fp2_mul(r->x, t3, r->x);
		fp2_mul(t3, t1, t3);
		fp2_sqr(t4, t2);
		fp2_mul(t4, t4, r->z);
		fp2_add(t4, t3, t4);

		fp_mul(l[0][1][0], t2[0], p->x);
		fp_mul(l[0][1][1], t2[1], p->x);
		fp2_neg(l[0][1], l[0][1]);

		// l00 = x2 * t2 - y2 * t1. 
		

		//		t4:=t4-X3: t4:=t4-X3: X3:=X3-t4: t2:=t2*X3: Y3:=t3*Y1: Y3:=t2-Y3: X3:=t1*t4: Z3:=Z1*t3:
		fp2_sub(t4, t4, r->x);
		fp2_sub(t4, t4, r->x);
		fp2_sub(r->x, r->x, t4);

		fp2_mul_nr(T0, t2, r->x);
		fp2_mul_nr(T1, t3, r->y);
		fp2_subd(T0, T0, T1);
		fp2_rdc(r->y, T0);	

		fp2_mul(r->x, t1, t4);
		fp2_mul(r->z, r->z, t3);

		fp2_mul_nr(T0, q->x, t2);
		fp2_mul_nr(T1, q->y, t1);
		fp2_subd(T0, T0, T1);
		fp2_rdc(l[0][0], T0);

		// l01 = t1 * yp. 
		fp_mul(l[1][1][0], t1[0], p->y);
		fp_mul(l[1][1][1], t1[1], p->y);

		r->norm = 0;
	}
	CATCH_ANY {
		THROW(ERR_CAUGHT);
	}
	FINALLY {
		fp2_free(t1);
		fp2_free(t2);
		fp2_free(t3);
		fp2_free(t4);
		fp2_free(t5);
		dv2_free(T0);
		dv2_free(T1);
		
	}
}

void pp_dblm(fp12_t l, ep2_t r, ep2_t q, ep_t p) {

	fp2_t t0, t1, t2, t3, t4, t5;
	dv2_t T0, T1;

	fp2_null(t0);
	fp2_null(t1);
	fp2_null(t2);
	fp2_null(t3);
	fp2_null(t4);
	fp2_null(t5);
	dv2_null(T0);
	dv2_null(T1);

	TRY{
		fp2_new(t0);
		fp2_new(t1);
		fp2_new(t2);
		fp2_new(t3);
		fp2_new(t4);
		fp2_new(t5);
		dv2_new(T0);
		dv2_new(T1);

		fp2_sqr(t0, q->x); // t0 = x1^2
		fp2_sqr(t1, q->y); // t1 = y1^2
		fp2_sqr(t2, q->z); // t2 = z1^2
		fp2_add(t5, t1, t2);
#if ARCH == ARM
		fp2_add(t4, q->x, q->y);
		fp2_sqr(t4, t4);
		fp2_sub(t4, t4, t1);
		fp2_sub(t4, t4, t0);
		fp2_div2(t3, t4);
		fp2_div2(t4, t3); 	
#else
		fp2_mul(t3, q->x, q->y); // t3 = x1y1	
		fp2_div2(t4, t3); // t4 = x1y1/2
#endif		
		fp2_dbl(t3, t2); // t3 = 2z1^2
		fp2_add(t2, t2, t3); // t2 = 3z1^2
		
		fp2_mulbm(t3, t2);

		fp2_dbl(t2, t3);
		fp2_add(t2, t3, t2); // t2 = 9b.z1^2
		fp2_sub(r->x, t1, t2); // x3 = y1^2 - 9b.z1^2
		fp2_mul(r->x, r->x, t4); // x3 = (y1^2 - 9b.z1^2)x1y1/2

		fp2_add(t4, t1, t2); // t4 = y1^2 + 9b.z1^2

		fp2_add(t2, q->y, q->z);
		fp2_sqr(t2, t2);		
		fp2_sub(t5, t2, t5);

		fp2_div2(t2, t4); // t2 = (y1^2 + 9b.z1^2)/2
		
		fp2_sqr_nr(T0, t3);
		fp2_addd(T1, T0, T0);
		fp2_addd(T1, T0, T1);

		fp2_sqr_nr(T0, t2);	
		fp2_subd(T0, T0, T1);
		fp2_rdc(r->y, T0);		

		fp2_mul(r->z, t5, t1); // z3 = 2y1^3.z1

		fp2_sub(l[0][0], t3, t1); // t3 = 3b.z1^2 - y1^2

		fp2_add(l[0][1], t0, t0);
		fp2_add(l[0][1], l[0][1], t0);
		fp_mul(l[0][1][0], l[0][1][0], p->x);
		fp_mul(l[0][1][1], l[0][1][1], p->x); // l02 = (3x1^2)xp

		fp_mul(l[1][1][0], t5[0], p->y);
		fp_mul(l[1][1][1], t5[1], p->y); // l11 = (2y1z1)yp
		//fp2_neg(l[1][1], l[1][1]);		// l11 = -(2y1z1)yp

		r->norm = 0;
	}
	CATCH_ANY {
		THROW(ERR_CAUGHT);
	}
	FINALLY {
		fp2_free(t0);
		fp2_free(t1);
		fp2_free(t2);
		fp2_free(t3);
		fp2_free(t4);
		fp2_free(t5);
		
	}
}

void pp_dblmf(fp12_t l, ep2_t r, ep2_t q, ep_t p) {

	fp2_t t0, t1, t2, t3, t4, t5;

	fp2_null(t0);
	fp2_null(t1);
	fp2_null(t2);
	fp2_null(t3);
	fp2_null(t4);
	fp2_null(t5);

	TRY{
		fp2_new(t0);
		fp2_new(t1);
		fp2_new(t2);
		fp2_new(t3);
		fp2_new(t4);
		fp2_new(t5);

		fp2_sqr(t0, q->x); // t0 = x1^2
		fp2_sqr(t1, q->y); // t1 = y1^2
		fp2_mul(t3, q->x, q->y); // t3 = x1y1

//	
		fp2_div2(t4, t3); // t4 = x1y1/2				
		
		fp_zero(t3[1]);
		fp_set_dig(t3[0], 3);		
		fp2_mulbm(t3, t3);

		fp2_dbl(t2, t3);
		fp2_add(t2, t3, t2); // t2 = 9b.z1^2
		fp2_sub(r->x, t1, t2); // x3 = y1^2 - 9b.z1^2
		fp2_mul(r->x, r->x, t4); // x3 = (y1^2 - 9b.z1^2)x1y1/2

		fp2_add(t4, t1, t2); // t4 = y1^2 + 9b.z1^2
//
		fp2_div2(t2, t4); // t2 = (y1^2 + 9b.z1^2)/2
		fp2_sqr(t2, t2); // t2 = [(y1^2 + 9b.z1^2)/2]^2
		fp2_sqr(t4, t3); // t4 = 9b^2.z1^4
		fp2_copy(t5, q->y); // t5 = y1z1
		fp2_dbl(r->y, t4); // y3 = 18b^2.z1^4
		fp2_add(r->y, r->y, t4); // y3 = 27b^2.z1^4
		fp2_sub(r->y, t2, r->y); // y3 = [(y1^2 + 9b.z1^2)/2]^2 - 27b^2.z1^4

		fp2_dbl(t2, t5); // t2 = 2y1z1
		fp2_mul(r->z, t2, t1); // z3 = 2y1^3.z1

		fp2_sub(l[0][0], t3, t1); // t3 = 3b.z1^2 - y1^2

		fp2_add(l[0][1], t0, t0);
		fp2_add(l[0][1], l[0][1], t0);
		fp_mul(l[0][1][0], l[0][1][0], p->x);
		fp_mul(l[0][1][1], l[0][1][1], p->x); // l02 = (3x1^2)xp

		fp_mul(l[1][1][0], t2[0], p->y);
		fp_mul(l[1][1][1], t2[1], p->y); // l11 = (2y1z1)yp
		//fp2_neg(l[1][1], l[1][1]);		// l11 = -(2y1z1)yp

		r->norm = 0;
	}
	CATCH_ANY {
		THROW(ERR_CAUGHT);
	}
	FINALLY {
		fp2_free(t0);
		fp2_free(t1);
		fp2_free(t2);
		fp2_free(t3);
		fp2_free(t4);
		fp2_free(t5);
		
	}
}

/**
 * Compute the Miller loop for Ate variants over the bits of r.
 * 
 * @param[out] res			- the result.
 * @param[out] t			- the point rQ.
 * @param[in] q				- the first point of the pairing, in G_2.
 * @param[in] r				- the length of the loop.
 * @param[in] p				- the second point of the pairing, in G_1.
 */
void pp_miller(fp12_t res, ep2_t t, ep2_t q, bn_t r, ep_t p) {
	fp12_t tmp;
	unsigned int i;
	ep_t pn;

	fp12_null(tmp);
	ep_null(pn);

	TRY {
		fp12_new(tmp);
		ep_new(pn);

		fp12_zero(res);
		ep_neg(pn, p);
		fp_set_dig(res[0][0][0], 1);
		ep2_copy(t, q);
		pp_dblf(res, t, t, pn);


		if (bn_test_bit(r, bn_bits(r) - 2)) {
		        pp_add(tmp, t, q, p);
			fp12_mul_sxs(res, res, tmp);
		}

		for (i = bn_bits(r) - 2; i > 0; i--) {
			pp_dbl(tmp, t, t, pn);


			fp12_sqr(res, res);
			fp12_mul_dxs(res, res, tmp);

			if (bn_test_bit(r, i - 1)) {
				pp_add(tmp, t, q, p);
				fp12_mul_dxs(res, res, tmp);
			}
		}
	}
	CATCH_ANY {
		THROW(ERR_CAUGHT);
	}
	FINALLY {
		fp12_free(tmp);
		ep_free(pn);
	}
}

void pp_millerm(fp12_t res, ep2_t t, ep2_t q, bn_t r, ep_t p) {
	fp12_t tmp;
	unsigned int i;
	ep_t pn;

	fp12_null(tmp);
	ep_null(pn);


		fp12_new(tmp);
		ep_new(pn);

		fp12_zero(res);
		ep_neg(pn, p);
		fp_set_dig(res[0][0][0], 1);
		ep2_copy(t, q);
		pp_dblmf(res, t, t, pn);


		if (bn_test_bit(r, bn_bits(r) - 2)) {
		        pp_addm(tmp, t, q, p);
			fp12_mul_sxsm(res, res, tmp);
		}

		for (i = bn_bits(r) - 2; i > 0; i--) {
			pp_dblm(tmp, t, t, pn);


			fp12_sqr(res, res);
			fp12_mul_dxsm(res, res, tmp);

			if (bn_test_bit(r, i - 1)) {
				pp_addm(tmp, t, q, p);
				fp12_mul_dxsm(res, res, tmp);
			}
		}
	

		fp12_free(tmp);
		ep_free(pn);
	
}

//naf miller
void pp_miller_naf(fp12_t res, ep2_t t, ep2_t q, bn_t r, ep_t p) {
	fp12_t tmp;
	ep2_t nq;	
	ep_t pn;
	unsigned int i;
	int len;
	signed char naf[bn_bits(r) + 1];

	fp12_null(tmp);
	ep_null(pn);
	ep2_null(nq);

	TRY {
		fp12_new(tmp);
		ep2_new(nq);
		ep_new(pn);

		fp12_zero(res);
		ep_neg(pn, p);
		ep2_copy(t, q);
		ep2_neg(nq, q);

		/* Compute the w-NAF representation of r. */
		bn_rec_naf(naf, &len, r, 2);

		pp_dblmf(res, t, t, pn);
		if (naf[len-2] == 1) {
		        pp_addm(tmp, t, q, p);
			fp12_mul_sxsm(res, res, tmp);
		}
		else if (naf[len-2] == -1) {
		        pp_addm(tmp, t, nq, p);
			fp12_mul_sxsm(res, res, tmp);
		}

		for (i = len - 2; i > 0; i--) {
			pp_dblm(tmp, t, t, pn);

			fp12_sqr(res, res);
			fp12_mul_dxsm(res, res, tmp);

			if (naf[i-1] == 1) {
				pp_addm(tmp, t, q, p);
				fp12_mul_dxsm(res, res, tmp);
			}
			else if (naf[i-1] == -1) {
				pp_addm(tmp, t, nq, p);
				fp12_mul_dxsm(res, res, tmp);
			}
		}
	}
	CATCH_ANY {
		THROW(ERR_CAUGHT);
	}
	FINALLY {
		fp12_free(tmp);
		ep2_free(nq);
	}
}

/**
 * Compute the additional multiplication required by the R-Ate pairing.
 * 
 * @param[in,out] res		- the result.
 * @param[in] t				- the elliptic point produced by the Miller loop.
 * @param[in] q				- the first point of the pairing, in G_2. 
 * @param[in] p				- the second point of the pairing, in G_1.
 */
void pp_rate_mult(fp12_t res, ep2_t t, ep2_t q, ep_t p) {
	ep2_t q1, r1q;
	fp12_t tmp1;
	fp12_t tmp2;

	fp12_null(tmp1);
	fp12_null(tmp2);
	ep2_null(q1);
	ep2_null(r1q);

	TRY {
		ep2_new(q1);
		ep2_new(r1q);
		fp12_new(tmp1);
		fp12_new(tmp2);

		ep2_copy(r1q, t);

		pp_add(tmp1, r1q, q, p);
		fp12_mul_dxs(tmp2, res, tmp1);
		fp12_frb(tmp2, tmp2);
		fp12_mul(res, res, tmp2);

		r1q->norm = 0;
		ep2_norm2(r1q, r1q);

		ep2_frb(q1, r1q);

		ep2_copy(r1q, t);

		pp_add(tmp1, r1q, q1, p);
		fp12_mul_dxs(res, res, tmp1);
	} CATCH_ANY {
		THROW(ERR_CAUGHT);
	} FINALLY {
		fp12_free(tmp1);
		fp12_free(tmp2);
		ep2_free(q1);
		ep2_free(r1q);
	}
}

/**
 * Compute the additional multiplication required by the Optimal Ate pairing.
 *
 * @param[in,out] res- the result.
 * @param[in] t- the elliptic point produced by the Miller loop.
 * @param[in] q- the first point of the pairing, in G_2.
 * @param[in] p- the second point of the pairing, in G_1.
 */
void pp_o_ate_mul(fp12_t res, ep2_t t, ep2_t q, ep_t p) {
  	ep2_t q1, q2;
  	fp12_t tmp, tmp2;

  	fp12_null(tmp);
	fp12_null(tmp2);
        ep2_null(q1);
        ep2_null(q2);

        TRY {
                ep2_new(q1);
                ep2_new(q2);
                fp12_new(tmp);
		fp12_new(tmp2);

                fp_set_dig(q1->z[0], 1);
                fp_zero(q1->z[1]);
                fp_set_dig(q2->z[0], 1);
                fp_zero(q2->z[1]);
              
		ep2_frb(q1, q);
                ep2_frb_sqr(q2, q);
                ep2_neg(q2, q2);

                pp_add(tmp, t, q1, p);
                pp_add(tmp2, t, q2, p);
		fp12_mul_sxs(tmp, tmp2, tmp);
                fp12_mul(res, tmp, res);
        } CATCH_ANY {
                THROW(ERR_CAUGHT);
        } FINALLY {
                fp12_free(tmp);
		fp12_free(tmp2);
                ep2_free(q1);
                ep2_free(q2);
        }
}

/**
 * Compute the additional multiplication required by the R-Ate pairing.
 * 
 * @param[in,out] res		- the result.
 * @param[in] t				- the elliptic point produced by the Miller loop.
 * @param[in] q				- the first point of the pairing, in G_2. 
 * @param[in] p				- the second point of the pairing, in G_1.
 */
void pp_rate_multm(fp12_t res, ep2_t t, ep2_t q, ep_t p) {
	ep2_t q1, r1q;
	fp12_t tmp1;
	fp12_t tmp2;

	fp12_null(tmp1);
	fp12_null(tmp2);
	ep2_null(q1);
	ep2_null(r1q);

	TRY {
		ep2_new(q1);
		ep2_new(r1q);
		fp12_new(tmp1);
		fp12_new(tmp2);

		ep2_copy(r1q, t);

		pp_addm(tmp1, r1q, q, p);
		fp12_mul_dxs(tmp2, res, tmp1);
		fp12_frb(tmp2, tmp2);
		fp12_mul(res, res, tmp2);

		r1q->norm = 0;
		ep2_norm2(r1q, r1q);

		ep2_frbm(q1, r1q);

		ep2_copy(r1q, t);

		pp_addm(tmp1, r1q, q1, p);
		fp12_mul_dxsm(res, res, tmp1);
	} CATCH_ANY {
		THROW(ERR_CAUGHT);
	} FINALLY {
		fp12_free(tmp1);
		fp12_free(tmp2);
		ep2_free(q1);
		ep2_free(r1q);
	}
}

/**
 * Compute the additional multiplication required by the Optimal Ate pairing.
 *
 * @param[in,out] res- the result.
 * @param[in] t- the elliptic point produced by the Miller loop.
 * @param[in] q- the first point of the pairing, in G_2.
 * @param[in] p- the second point of the pairing, in G_1.
 */
void pp_o_ate_mulm(fp12_t res, ep2_t t, ep2_t q, ep_t p) {
  	ep2_t q1, q2;
  	fp12_t tmp, tmp2;

  	fp12_null(tmp);
	fp12_null(tmp2);
        ep2_null(q1);
        ep2_null(q2);

        TRY {
                ep2_new(q1);
                ep2_new(q2);
                fp12_new(tmp);
		fp12_new(tmp2);

                fp_set_dig(q1->z[0], 1);
                fp_zero(q1->z[1]);
                fp_set_dig(q2->z[0], 1);
                fp_zero(q2->z[1]);
              
		ep2_frbm(q1, q);
                ep2_frbm(q2, q1);
                ep2_neg(q2, q2);

                pp_addm(tmp, t, q1, p);
                pp_addm(tmp2, t, q2, p);
		fp12_mul_sxsm(tmp, tmp2, tmp);
                fp12_mul(res, tmp, res);
        } CATCH_ANY {
                THROW(ERR_CAUGHT);
        } FINALLY {
                fp12_free(tmp);
		fp12_free(tmp2);
                ep2_free(q1);
                ep2_free(q2);
        }
}

/**
 * Compute the final exponentiation of the rate pairing in a BN curve.
 * 
 * @param[in,out] m			- the result.
 * @param[in] x				- the parameter used to generate the curve.
 */
/*
void pp_exp(fp12_t m, bn_t x) {
	fp12_t v0;
	fp12_t v1;
	fp12_t v2;
	fp12_t v3;

	fp12_null(v0);
	fp12_null(v1);
	fp12_null(v2);
	fp12_null(v3);

	TRY {
		fp12_new(v0);
		fp12_new(v1);
		fp12_new(v2);
		fp12_new(v3);

		// First, compute m^(p^6 - 1). 
		// tmp = m^{-1}. 
		fp12_inv(v0, m);
		// m' = m^(p^6). 
		fp12_inv_uni(m, m);
		// m' = m^(p^6 - 1). 
		fp12_mul(m, m, v0);

		// Second, compute m^(p^2 + 1). 
		// t = m^(p^2). 
		fp12_frb(v0, m);
		fp12_frb(v0, v0);
		// m' = m^(p^2 + 1). 
		fp12_mul(m, m, v0);

		// Third, compute m^((p^4 - p^2 + 1) / r). 
		// From here on we work with x' = -x, therefore if x is positive
		// we need inversions.
		if (bn_sign(x) == BN_POS) {
		  // TODO: these inversions are probably just conjugations, test
			fp12_inv(v3, m);
			fp12_exp_uni(v0, v3, x);
			fp12_inv(v3, v0);
			fp12_exp_uni(v1, v3, x);
			fp12_inv(v3, v1);
			fp12_exp_uni(v2, v3, x);
		} else {
			fp12_exp_uni(v0, m, x);
			fp12_exp_uni(v1, v0, x);
			fp12_exp_uni(v2, v1, x);
		}

		fp12_frb(v3, v2);
		fp12_mul(v2, v2, v3);
		fp12_sqr_uni(v2, v2);
		fp12_frb(v3, v1);
		fp12_inv_uni(v3, v3);
		fp12_mul(v3, v3, v0);
		fp12_mul(v2, v2, v3);
		fp12_frb(v0, v0);
		fp12_inv_uni(v3, v1);
		fp12_mul(v2, v2, v3);
		fp12_mul(v0, v0, v3);
		fp12_frb(v1, v1);
		fp12_frb(v1, v1);
		fp12_mul(v0, v0, v2);
		fp12_mul(v2, v2, v1);
		fp12_sqr_uni(v0, v0);
		fp12_mul(v0, v0, v2);
		fp12_sqr_uni(v0, v0);
		fp12_inv_uni(v1, m);
		fp12_mul(v2, v0, v1);
		fp12_frb(v1, m);
		fp12_frb(v1, v1);
		fp12_frb(v3, v1);
		fp12_mul(v1, v1, v3);
		fp12_frb(v3, m);
		fp12_mul(v1, v1, v3);
		fp12_mul(v0, v0, v1);
		fp12_sqr_uni(v2, v2);
		fp12_mul(m, v2, v0);
	}
	CATCH_ANY {
		THROW(ERR_CAUGHT);
	}
	FINALLY {
		fp12_free(v0);
		fp12_free(v1);
		fp12_free(v2);
		fp12_free(v3);
	}
}*/


/*void pp_exp(fp12_t m, bn_t x) {
  fp12_t v0;
  fp12_t v1;
  fp12_t v2;
  fp12_t v3;

  fp12_null(v0);
  fp12_null(v1);
  fp12_null(v2);
  fp12_null(v3);

  TRY {
    fp12_new(v0);
    fp12_new(v1);
    fp12_new(v2);
    fp12_new(v3);

    //fp12_print(m);
    //printf("\n");
    // First, compute m^(p^6 - 1). 
    // tmp = m^{-1}. 
    fp12_inv(v0, m);
    // m' = m^(p^6). 
    fp12_inv_uni(m, m);
    // m' = m^(p^6 - 1). 
    fp12_mul(m, m, v0);
    //fp12_print(m);
    //printf("\n");

    // Second, compute m^(p^2 + 1). 
    // t = m^(p^2). 
    fp12_frb_sqr(v0, m);
    // m' = m^(p^2 + 1). 
    fp12_mul(m, m, v0);
    //fp12_print(m);
    //printf("\n");

    // Third, compute m^((p^4 - p^2 + 1) / r). 
    // From here on we work with x' = -x, therefore if x is positive
    // we need inversions. 
    if (bn_sign(x) == BN_POS) {
      // We are now on the cyclotomic subgroup, so inversion are
      // * conjugations. 
      fp12_inv_uni(v3, m);
      fp12_exp_uni(v0, v3, x);
      fp12_inv_uni(v3, v0);
      fp12_exp_uni(v1, v3, x);
      fp12_inv_uni(v3, v1);
      fp12_exp_uni(v2, v3, x);
    } else {
      fp12_exp_uni(v0, m, x);
      fp12_exp_uni(v1, v0, x);
      fp12_exp_uni(v2, v1, x);
    }

    fp12_frb(v3, v2);
    fp12_mul(v2, v2, v3);
    fp12_sqr_uni(v2, v2);
    fp12_frb(v3, v1);
    fp12_inv_uni(v3, v3);
    fp12_mul(v3, v3, v0);
    fp12_mul(v2, v2, v3);
    fp12_frb(v0, v0);
    fp12_inv_uni(v3, v1);
    fp12_mul(v2, v2, v3);
    fp12_mul(v0, v0, v3);
    fp12_frb_sqr(v1, v1);
    fp12_mul(v0, v0, v2);
    fp12_mul(v2, v2, v1);
    fp12_sqr_uni(v0, v0);
    fp12_mul(v0, v0, v2);
    fp12_sqr_uni(v0, v0);
    fp12_inv_uni(v1, m);
    fp12_mul(v2, v0, v1);
    fp12_frb_sqr(v1, m);
    fp12_frb(v3, v1);
    fp12_mul(v1, v1, v3);
    fp12_frb(v3, m);
    fp12_mul(v1, v1, v3);
    fp12_mul(v0, v0, v1);
    fp12_sqr_uni(v2, v2);
    fp12_mul(m, v2, v0);
    //fp12_print(m);
    //printf("\n");
  }
  CATCH_ANY {
    THROW(ERR_CAUGHT);
  }
  FINALLY {
    fp12_free(v0);
    fp12_free(v1);
    fp12_free(v2);
    fp12_free(v3);
  }
}*/

//with ed's improvement
void pp_exp(fp12_t m, bn_t x) {
  fp12_t v0;
  fp12_t v1;
  fp12_t v2;
  fp12_t v3;
  fp12_t v4;

  fp12_null(v0);
  fp12_null(v1);
  fp12_null(v2);
  fp12_null(v3);
  fp12_null(v4);

  TRY {
    fp12_new(v0);
    fp12_new(v1);
    fp12_new(v2);
    fp12_new(v3);
    fp12_new(v4);

    //fp12_print(m);
    //printf("\n");
    // First, compute m^(p^6 - 1). 
    // tmp = m^{-1}. 
    fp12_inv(v0, m);
    // m' = m^(p^6). 
    fp12_inv_uni(m, m);
    // m' = m^(p^6 - 1). 
    fp12_mul(m, m, v0);

    // Second, compute m^(p^2 + 1). 
    // t = m^(p^2). 
    fp12_frb_sqr(v0, m);
    // m' = m^(p^2 + 1). 
    fp12_mul(m, m, v0);
    //fp12_print(m);
    //printf("\n");

    // Third, compute m^((p^4 - p^2 + 1) / r)(2x)(6x^2+3x+1). 
    // From here on we work with x' = -x, therefore if x is positive
    // we need inversions. 
    if (bn_sign(x) == BN_POS) {
      // We are now on the cyclotomic subgroup, so inversion are
      // * conjugations. 
      fp12_exp_uni(v0, m, x);      
      fp12_copy(v4, m);

      fp12_sqr_uni(v0, v0);
      fp12_sqr_uni(v1, v0);
      fp12_mul(v1, v1, v0);
      fp12_exp_uni(v2, v1, x);
      fp12_sqr_uni(v3, v2);
      fp12_exp_uni(v3, v3, x);
    } else {
      fp12_exp_uni(v0, m, x);
      fp12_inv_uni(v0, v0);
      fp12_copy(v4, m);
      //first exponentiations	
      fp12_sqr_uni(v0, v0);
      fp12_sqr_uni(v1, v0);
      fp12_mul(v1, v1, v0);
      fp12_exp_uni(v2, v1, x);
      fp12_inv_uni(v2, v2);
    
      fp12_sqr_uni(v3, v2);
      fp12_exp_uni(v3, v3, x);
      fp12_inv_uni(v3, v3);
    }

    //a
    fp12_mul(v3, v3, v2);
    fp12_mul(v3, v3, v1);

    //b
    fp12_inv_uni(v0, v0);
    fp12_mul(v0, v0, v3);
    //fp12_print(v0);
    //printf("\n");

    //final result
    fp12_mul(m, m, v2);
    fp12_mul(m, m, v3);
    //fp12_print(v0);
    //printf("\n");
    fp12_frb(v2, v0);
    //fp12_print(v2);
    //printf("\n");
    fp12_mul(m, m, v2);
    //fp12_print(m);
    //printf("\n");
    fp12_frb_sqr(v3, v3);
    fp12_mul(m, m, v3);
    //fp12_print(m);
    //printf("\n");
    fp12_inv_uni(v4, v4);
    fp12_mul(v0, v0, v4);
    //fp12_print(v0);
    //printf("\n");
    fp12_frb_sqr(v0, v0);
    //fp12_print(v0);
    //printf("\n");
    fp12_frb(v0, v0);
    fp12_mul(m, m, v0);
    //fp12_print(m);
    //printf("\n");
  }
  CATCH_ANY {
    THROW(ERR_CAUGHT);
  }
  FINALLY {
    fp12_free(v0);
    fp12_free(v1);
    fp12_free(v2);
    fp12_free(v3);
    fp12_free(v4);
  }
}

/*void pp_exp(fp12_t m, bn_t x) {
  fp12_t v0;
  fp12_t v1;
  fp12_t v2;
  fp12_t v3;
  bn_t d, e, t2, t3, t4;

  fp12_null(v0);
  fp12_null(v1);
  fp12_null(v2);
  fp12_null(v3);
  bn_null(d);
  bn_null(e);
  bn_null(t2);
  bn_null(t3);
  bn_null(t4);

  TRY {
    fp12_new(v0);
    fp12_new(v1);
    fp12_new(v2);
    fp12_new(v3);

    //fp12_print(m);
    //printf("\n");
    // First, compute m^(p^6 - 1). 
    // tmp = m^{-1}. 
    fp12_inv(v0, m);
    // m' = m^(p^6). 
    fp12_inv_uni(m, m);
    // m' = m^(p^6 - 1). 
    fp12_mul(m, m, v0);
    //fp12_print(m);
    //printf("\n");

    // Second, compute m^(p^2 + 1). 
    // t = m^(p^2). 
    fp12_frb_sqr(v0, m);
    // m' = m^(p^2 + 1). 
    fp12_mul(m, m, v0);
    //fp12_print(m);
    //printf("\n");

    // Third, compute m^((p^4 - p^2 + 1) / r). 
    bn_new(d);
    bn_new(e);
    bn_new(t2);
    bn_new(t3);
    bn_new(t4);
    d->used=FP_DIGS;
    dv_copy(d->dp, fp_prime_get(), FP_DIGS);
    bn_sqr(d, d);
    bn_sqr(e, d);
    bn_sub(e, e, d);
    bn_add_dig(e, e, 1);
    //bn_print(e);
    //printf("\n");
    
    //compute r
    bn_sqr(t2, x);
    bn_mul(t3, t2, x);
    bn_mul(t4, t3, x);

    bn_set_dig(d, 1);
    bn_mul_dig(t4, t4, 36);
    bn_mul_dig(t3, t3, 36);
    bn_mul_dig(t2, t2, 18);
    bn_add(t4, t4, t3);
    bn_add(t4, t4, t2);
    bn_mul_dig(t3, x, 6);
    bn_add(t4, t4, t3);
    bn_add(d, d, t4);
    //bn_print(d);
    //printf("\n");

    bn_div(e, e, d);
    //bn_print(e);
    //printf("\n");

    fp12_exp(m, m, e);
    //fp12_print(m);
    //printf("\n");
  }
  CATCH_ANY {
    THROW(ERR_CAUGHT);
  }
  FINALLY {
    fp12_free(v0);
    fp12_free(v1);
    fp12_free(v2);
    fp12_free(v3);
  }
}*/

/*void pp_exp(fp12_t m, bn_t x) {
  fp12_t v0;
  fp12_t v1;
  fp12_t v2;
  fp12_t v3;
  bn_t d, e, t2, t3, t4;

  fp12_null(v0);
  fp12_null(v1);
  fp12_null(v2);
  fp12_null(v3);
  bn_null(d);
  bn_null(e);
  bn_null(t2);
  bn_null(t3);
  bn_null(t4);

  TRY {
    fp12_new(v0);
    fp12_new(v1);
    fp12_new(v2);
    fp12_new(v3);

    //fp12_print(m);
    //printf("\n");
    // First, compute m^(p^6 - 1). 
    // tmp = m^{-1}. 
    fp12_inv(v0, m);
    // m' = m^(p^6). 
    fp12_inv_uni(m, m);
    // m' = m^(p^6 - 1). 
    fp12_mul(m, m, v0);
    //fp12_print(m);
    //printf("\n");

    // Second, compute m^(p^2 + 1). 
    // t = m^(p^2). 
    fp12_frb_sqr(v0, m);
    // m' = m^(p^2 + 1). 
    fp12_mul(m, m, v0);
    //fp12_print(m);
    //printf("\n");

    // Third, compute m^((p^4 - p^2 + 1) / r). 
    bn_new(d);
    bn_new(e);
    bn_new(t2);
    bn_new(t3);
    bn_new(t4);
    d->used=FP_DIGS;
    dv_copy(d->dp, fp_prime_get(), FP_DIGS);
    bn_sqr(d, d);
    bn_sqr(e, d);
    bn_sub(e, e, d);
    bn_add_dig(e, e, 1);
    //bn_print(e);
    //printf("\n");
    
    //compute r
    bn_sqr(t2, x);
    bn_mul(t3, t2, x);
    bn_mul(t4, t3, x);

    bn_set_dig(d, 1);
    bn_mul_dig(t4, t4, 36);
    bn_mul_dig(t3, t3, 36);
    bn_mul_dig(t2, t2, 18);
    bn_add(t4, t4, t3);
    bn_add(t4, t4, t2);
    bn_mul_dig(t3, x, 6);
    bn_add(t4, t4, t3);
    bn_add(d, d, t4);
    //bn_print(d);
    //printf("\n");

    bn_div(e, e, d);
    //bn_print(e);
    //printf("\n");

    fp12_exp(m, m, e);
    //fp12_print(m);
    //printf("\n");
  }
  CATCH_ANY {
    THROW(ERR_CAUGHT);
  }
  FINALLY {
    fp12_free(v0);
    fp12_free(v1);
    fp12_free(v2);
    fp12_free(v3);
  }
}*/


/*============================================================================*/
/* Public definitions                                                         */
/*============================================================================*/

void pp_map_rate(fp12_t r, ep2_t q, ep_t p) {
	ep2_t t;
	bn_t a, x;

	ep2_null(t);
	bn_null(a);
	bn_null(x);

	TRY {
		ep2_new(t);
		bn_new(a);
		bn_new(x);
		
		switch (fp_param_get()) {
			case BNN_256:
				/* x = -4080000000000001. */
				bn_set_2b(x, 62);
				bn_set_2b(a, 55);
				bn_add(x, x, a);
				bn_add_dig(x, x, 1);
				bn_neg(x, x);
				break;
			case BNP_256:
				/* x = 6000000000001F2D. */
				bn_set_2b(x, 62);
				bn_set_2b(a, 61);
				bn_add(x, x, a);
				bn_set_dig(a, 0x1F);
				bn_lsh(a, a, 8);
				bn_add(x, x, a);
				bn_add_dig(x, x, 0x2D);
				break;
			case BN_446:
				/* x = 2^110 + 2^36 + 1 */
				bn_set_2b(x, 110);
				bn_set_2b(a, 36);
				bn_add(x, x, a);
				bn_add_dig(x, x, 1);
				break;
		        case BN_286:
                     		/* x = -(2^70+2^58+2^38+1). */
                    		bn_set_2b(x, 70);
                     		bn_set_2b(a, 58);
                     		bn_add(x, x, a);
		     		bn_set_2b(a, 38);
		     		bn_add(x, x, a);
                     		bn_add_dig(x, x, 1);
                     		bn_neg(x, x);
		     		break;
		        case BN_478:
                     		/* x = -(2^118-2^55-2^19+1). */
                                bn_set_2b(x, 118);
                                bn_set_2b(a, 55);
                                bn_sub(x, x, a);
				bn_set_2b(a, 19);
				bn_sub(x, x, a);
                                bn_add_dig(x, x, 1);
                                bn_neg(x, x);
                                break;
		        case BN_606:
                     		/* x = -(2^150-2^95+2^8+1). */
                                bn_set_2b(x, 150);
                                bn_set_2b(a, 95);
                                bn_sub(x, x, a);
				bn_set_2b(a, 8);
				bn_add(x, x, a);
                                bn_add_dig(x, x, 1);
                                bn_neg(x, x);
                                break;
		        case BN_638:
                     		/* x = 2^158-2^128-2^68+1. */
                                bn_set_2b(x, 158);
                                bn_set_2b(a, 128);
                                bn_sub(x, x, a);
				bn_set_2b(a, 68);
				bn_sub(x, x, a);
                                bn_add_dig(x, x, 1);
                                break;
		}

		bn_mul_dig(a, x, 6);
		bn_add_dig(a, a, 2);
		if (bn_sign(x) == BN_NEG) {
			bn_neg(a, a);
		}

		/* r = f_{r,Q}(P). */

		if (ep2_curve_is_twist() == 1){
			pp_miller(r, t, q, a, p);

			if (bn_sign(x) == BN_NEG) {
				/* Since f_{-r,Q}(P) = 1/f_{r,Q}(P), we must invert the result. */
				fp12_inv(r, r);
				ep2_neg(t, t);
			}

			pp_rate_mult(r, t, q, p);
		}
		else
		{
			pp_miller_naf(r, t, q, a, p);
			

			if (bn_sign(x) == BN_NEG) {
				/* Since f_{-r,Q}(P) = 1/f_{r,Q}(P), we must invert the result. */
				fp12_inv(r, r);
				ep2_neg(t, t);
			}

			pp_rate_multm(r, t, q, p);
		}
		
		pp_exp(r, x);

	}
	CATCH_ANY {
		THROW(ERR_CAUGHT);
	}
	FINALLY {
		ep2_free(t);
		bn_free(a);
		bn_free(x);
	}
}

void pp_map_o_ate(fp12_t r, ep2_t q, ep_t p) {
  	ep2_t t;
        bn_t a, x;

        ep2_null(t);
        bn_null(a);
        bn_null(x);

        TRY {
                ep2_new(t);
    		bn_new(a);
                bn_new(x);

		switch (fp_param_get()) {
			case BNN_256:
				/* x = -4080000000000001. */
				bn_set_2b(x, 62);
				bn_set_2b(a, 55);
				bn_add(x, x, a);
				bn_add_dig(x, x, 1);
				bn_neg(x, x);
				break;
			case BNP_256:
				/* x = 6000000000001F2D. */
				bn_set_2b(x, 62);
				bn_set_2b(a, 61);
				bn_add(x, x, a);
				bn_set_dig(a, 0x1F);
				bn_lsh(a, a, 8);
				bn_add(x, x, a);
				bn_add_dig(x, x, 0x2D);
				break;
			case BN_446:
				/* x = 2^110 + 2^36 + 1 */
				bn_set_2b(x, 110);
				bn_set_2b(a, 36);
				bn_add(x, x, a);
				bn_add_dig(x, x, 1);
				break;
		        case BN_286:
                     		/* x = -(2^70+2^58+2^38+1). */
                    		bn_set_2b(x, 70);
                     		bn_set_2b(a, 58);
                     		bn_add(x, x, a);
		     		bn_set_2b(a, 38);
		     		bn_add(x, x, a);
                     		bn_add_dig(x, x, 1);
                     		bn_neg(x, x);
		     		break;
		        case BN_478:
                     		/* x = -(2^118-2^55-2^19+1). */
                                bn_set_2b(x, 118);
                                bn_set_2b(a, 55);
                                bn_sub(x, x, a);
				bn_set_2b(a, 19);
				bn_sub(x, x, a);
                                bn_add_dig(x, x, 1);
                                bn_neg(x, x);
                                break;
		        case BN_606:
                     		/* x = -(2^150-2^95+2^8+1). */
                                bn_set_2b(x, 150);
                                bn_set_2b(a, 95);
                                bn_sub(x, x, a);
				bn_set_2b(a, 8);
				bn_add(x, x, a);
                                bn_add_dig(x, x, 1);
                                bn_neg(x, x);
                                break;
		        case BN_638:
                     		/* x = 2^158-2^128-2^68+1. */
                                bn_set_2b(x, 158);
                                bn_set_2b(a, 128);
                                bn_sub(x, x, a);
				bn_set_2b(a, 68);
				bn_sub(x, x, a);
                                bn_add_dig(x, x, 1);
                                break;
		}

		bn_mul_dig(a, x, 6);
		bn_add_dig(a, a, 2);

                if (bn_sign(x) == BN_NEG) {
                     bn_neg(a, a);
                }

                if(ep2_curve_is_twist()==1){
			pp_miller(r, t, q, a, p);

		        if (bn_sign(x) == BN_NEG) {
		        /* Since f_{-r,Q}(P) = 1/f_{r,Q}(P), we must invert the result. */
		             fp12_inv_uni(r, r);
		             ep2_neg(t, t);
		        }

		        pp_o_ate_mul(r, t, q, p);
		}
		else
		{
			if (fp_param_get() == BN_446){
				pp_millerm(r,t,q,a,p);
			}
			else{
				pp_miller_naf(r, t, q, a, p);}

		        if (bn_sign(x) == BN_NEG) {
		        /* Since f_{-r,Q}(P) = 1/f_{r,Q}(P), we must invert the result. */
		             fp12_inv_uni(r, r);
		             ep2_neg(t, t);
		        }

		        pp_o_ate_mulm(r, t, q, p);
		}
                
		pp_exp(r, x);

         }
         CATCH_ANY {
                THROW(ERR_CAUGHT);
         }
         FINALLY {
                ep2_free(t);
                bn_free(a);
                bn_free(x);
         }
}
