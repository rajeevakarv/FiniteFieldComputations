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
 * Implementation of the sextic extension binary field arithmetic module.
 *
 * @version $Id: relic_pp_fp6.c 214 2010-01-25 03:09:40Z dfaranha $
 * @ingroup fp6
 */

#include <string.h>

#include "relic_core.h"
#include "relic_conf.h"
#include "relic_fp.h"
#include "relic_pp.h"
#include "relic_util.h"

/*============================================================================*/
/* Public definitions                                                         */
/*============================================================================*/

void fp6_dbl(fp6_t c, fp6_t a) {
	/* 2 * (a0 + a1 * v + a2 * v^2) = 2 * a0 + 2 * a1 * v + 2 * a2 * v^2. */
	/*fp2_dbl(c[0], a[0]);
	fp2_dbl(c[1], a[1]);
	fp2_dbl(c[2], a[2]);*/
	fp6_add(c,a,a);
}
/*
void fp6_mul(fp6_t c, fp6_t a, fp6_t b) {
// Full sextic extension field multiplication using Karatsuba
	fp2_t v0, v1, v2, t0, t1, t2;

	fp2_null(v0);
	fp2_null(v1);
	fp2_null(v2);
	fp2_null(t0);
	fp2_null(t1);
	fp2_null(t2);

	TRY {
		fp2_new(v0);
		fp2_new(v1);
		fp2_new(v2);
		fp2_new(t0);
		fp2_new(t1);
		fp2_new(t2);

		// v0 = a0b0 
		fp2_mul(v0, a[0], b[0]);

		// v1 = a1b1 
		fp2_mul(v1, a[1], b[1]);

		// v2 = a2b2 
		fp2_mul(v2, a[2], b[2]);

		// t2 (c0) = v0 + E((a1 + a2)(b1 + b2) - v1 - v2) 
		fp2_add(t0, a[1], a[2]);
		fp2_add(t1, b[1], b[2]);
		fp2_mul(t2, t0, t1);
		fp2_sub(t2, t2, v1);
		fp2_sub(t2, t2, v2);
		fp2_mul_nor(t2, t2);
		fp2_add(t2, t2, v0);

                // c1 = (a0 + a1)(b0 + b1) - v0 - v1 + Ev2 
		fp2_add(t0, a[0], a[1]);
		fp2_add(t1, b[0], b[1]);
		fp2_mul(c[1], t0, t1);
		fp2_sub(c[1], c[1], v0);
		fp2_sub(c[1], c[1], v1);
		fp2_mul_nor(t0, v2);
		fp2_add(c[1], c[1], t0);

                // c2 = (a0 + a2)(b0 + b2) - v0 + v1 - v2 
		fp2_add(t0, a[0], a[2]);
		fp2_add(t1, b[0], b[2]);
		fp2_mul(c[2], t0, t1);
		fp2_sub(c[2], c[2], v0);
		fp2_add(c[2], c[2], v1);
		fp2_sub(c[2], c[2], v2);

		// c0 = t2 
		fp2_copy(c[0], t2);
	} CATCH_ANY {
		THROW(ERR_CAUGHT);
	} FINALLY {
		fp2_free(t2);
		fp2_free(t1);
		fp2_free(t0);
		fp2_free(v2);
		fp2_free(v1);
		fp2_free(v0);
	}
}
*/

/*
void fp6_mul(fp6_t c, fp6_t a, fp6_t b) {
// Full sextic extension field multiplication using Karatsuba and lazy reduction
        fp2_t t0, t1, t2;
        dv2_t T0, T1, T2, T3, T4;
      
        fp2_null(t0);
        fp2_null(t1);
        fp2_null(t2);
        dv2_null(T0);
        dv2_null(T1);
        dv2_null(T2);
        dv2_null(T3);
        dv2_null(T4);

        TRY {
                 fp2_new(t0);
                 fp2_new(t1);;
                 fp2_new(t2);
		 dv2_new(T0);
		 dv2_new(T1);
		 dv2_new(T2);
		 dv2_new(T3);
		 dv2_new(T4);
               
                 // T0 = a0b0 
                 fp2_mul_nr(T0, a[0], b[0]);

                 // T1 = a1b1 
                 fp2_mul_nr(T1, a[1], b[1]);

                 // T2 = a2b2 
                 fp2_mul_nr(T2, a[2], b[2]);

                 // t2 (c0) = v0 + E((a1 + a2)(b1 + b2) - v1 - v2) 
                 fp2_add(t0, a[1], a[2]);
                 fp2_add(t1, b[1], b[2]);
                 fp2_mul_nr(T3, t0, t1);
                 fp2_subd(T3, T3, T1);
                 fp2_subd(T3, T3, T2);
                 fp2_mul_nord(T3, T3);
                 fp2_addd(T3, T3, T0);
                 fp2_rdc(t2, T3)

		 // c1 = (a0 + a1)(b0 + b1) - v0 - v1 + Ev2 
                 fp2_add(t0, a[0], a[1]);
                 fp2_add(t1, b[0], b[1]);
                 fp2_mul_nr(T3, t0, t1);
                 fp2_subd(T3, T3, T0);
                 fp2_subd(T3, T3, T1);
                 fp2_mul_nord(T4, T2);
                 fp2_addd(T3, T3, T4);
                 fp2_rdc(c[1], T3)

		 // c2 = (a0 + a2)(b0 + b2) - v0 + v1 - v2 
                 fp2_add(t0, a[0], a[2]);
                 fp2_add(t1, b[0], b[2]);
                 fp2_mul_nr(T3, t0, t1);
                 fp2_subd(T3, T3, T0);
                 fp2_addd(T3, T3, T1);
                 fp2_subd(T3, T3, T2);
                 fp2_rdc(c[2], T3)

		 // c0 = t2 
                 fp2_copy(c[0], t2);
         } CATCH_ANY {
                 THROW(ERR_CAUGHT);
         } FINALLY {
                 fp2_free(t0);
                 fp2_free(t1);
                 fp2_free(t2);
		 dv2_free(T0);
		 dv2_free(T1);
		 dv2_free(T2);
		 dv2_free(T3);
		 dv2_free(T4);
         }
}

void fp6_mul_nr(dv6_t C, fp6_t a, fp6_t b) {
// Full sextic extension field multiplication using Karatsuba and lazy reduction
         fp2_t t0, t1;
	 dv2_t T0, T1, T2, T3, T4;

	 fp2_null(t0);
	 fp2_null(t1);
	 dv2_null(T0);
	 dv2_null(T1);
	 dv2_null(T2);
	 dv2_null(T3);
	 dv2_null(T4);

	 TRY {
	         fp2_new(t0);
		 fp2_new(t1);
		 dv2_new(T0);
		 dv2_new(T1);
		 dv2_new(T2);
		 dv2_new(T3);
		 dv2_new(T4);

		 // T0 = a0b0
		 fp2_mul_nr(T0, a[0], b[0]);

		 // T1 = a1b1
		 fp2_mul_nr(T1, a[1], b[1]);

		 // T2 = a2b2
		 fp2_mul_nr(T2, a[2], b[2]);

		 // t2 (c0) = v0 + E((a1 + a2)(b1 + b2) - v1 - v2)
		 fp2_add(t0, a[1], a[2]);
		 fp2_add(t1, b[1], b[2]);
		 fp2_mul_nr(T3, t0, t1);
		 fp2_subd(T3, T3, T1);
		 fp2_subd(T3, T3, T2);
		 fp2_mul_nord(T3, T3);
		 fp2_addd(T3, T3, T0);
 
		 // C1 = (a0 + a1)(b0 + b1) - v0 - v1 + Ev2
		 fp2_add(t0, a[0], a[1]);
		 fp2_add(t1, b[0], b[1]);
		 fp2_mul_nr(C[1], t0, t1);
		 fp2_subd(C[1], C[1], T0);
		 fp2_subd(C[1], C[1], T1);
		 fp2_mul_nord(T4, T2);
		 fp2_addd(C[1], C[1], T4);

		 // C2 = (a0 + a2)(b0 + b2) - v0 + v1 - v2
		 fp2_add(t0, a[0], a[2]);
		 fp2_add(t1, b[0], b[2]);
		 fp2_mul_nr(C[2], t0, t1);
		 fp2_subd(C[2], C[2], T0);
		 fp2_addd(C[2], C[2], T1);
		 fp2_subd(C[2], C[2], T2);
		
		 // C0 = T3
		 dv2_copy(C[0], T3);
	 } CATCH_ANY {
	           THROW(ERR_CAUGHT);
	 } FINALLY {
	           fp2_free(t0);
		   fp2_free(t1);
		   dv2_free(T0);
		   dv2_free(T1);
		   dv2_free(T2);
		   dv2_free(T3);
		   dv2_free(T4);
	 }
}
*/

static inline void fp6_mulkar_nr(dv6_t C, fp6_t a, fp6_t b) {
// Sextic extension field multiplication using Karatsuba and lazy reduction
         fp2_t t0, t1;
	 dv2_t T0, T1, T2, T3, T4;

	 fp2_null(t0);
	 fp2_null(t1);
	 dv2_null(T0);
	 dv2_null(T1);
	 dv2_null(T2);
	 dv2_null(T3);
	 dv2_null(T4);

	 TRY {
	          fp2_new(t0);
		  fp2_new(t1);
		  dv2_new(T0);
	  	  dv2_new(T1);
		  dv2_new(T2);
		  dv2_new(T3);
		  dv2_new(T4);

		  // T0 = a0b0
		  fp2_mul_nr(T0, a[0], b[0]);

		  // T1 = a1b1
		  fp2_mul_nr(T1, a[1], b[1]);

		  // T2 = a2b2
		  fp2_mul_nr(T2, a[2], b[2]);

		  // t2 (c0) = v0 + E((a1 + a2)(b1 + b2) - v1 - v2)
		  fp2_add(t0, a[1], a[2]);
		  fp2_add(t1, b[1], b[2]);
		  fp2_mul_nr(T3, t0, t1);
		  fp2_subd(T3, T3, T1);
		  fp2_subd(T3, T3, T2);
		  //dv_print(T3[0],60);
		  fp2_mul_nord(T3, T3);
		  fp2_addd(T3, T3, T0);

		  // C1 = (a0 + a1)(b0 + b1) - v0 - v1 + Ev2
		  fp2_add(t0, a[0], a[1]);
		  fp2_add(t1, b[0], b[1]);
		  fp2_mul_nr(C[1], t0, t1);
		  fp2_subd(C[1], C[1], T0);
		  fp2_subd(C[1], C[1], T1);
		  fp2_mul_nord(T4, T2);
		  fp2_addd(C[1], C[1], T4);

		  // C2 = (a0 + a2)(b0 + b2) - v0 + v1 - v2
		  fp2_add(t0, a[0], a[2]);
		  fp2_add(t1, b[0], b[2]);
		  fp2_mul_nr(C[2], t0, t1);
		  fp2_subd(C[2], C[2], T0);
		  fp2_addd(C[2], C[2], T1);
		  fp2_subd(C[2], C[2], T2);

		  // C0 = T3
		  dv2_copy(C[0], T3);
	 } CATCH_ANY {
	          THROW(ERR_CAUGHT);
	 } FINALLY {
	          fp2_free(t0);
		  fp2_free(t1);
		  dv2_free(T0);
		  dv2_free(T1);
		  dv2_free(T2);
		  dv2_free(T3);
		  dv2_free(T4);
	 }
}


void fp6_mul(fp6_t c, fp6_t a, fp6_t b) {
// Full sextic extension field multiplication
         dv6_t C;

	 dv6_null(C);

	 TRY {
                  dv6_new(C);

		  fp6_mulkar_nr(C, a, b);      // Karatsuba multiplication
	#if USE_NEON_FP6
		fp2_rdc_NEON(c[0][0], c[0][1], C[0][0], C[0][1]);
		fp2_rdc_NEON(c[1][0], c[1][1], C[1][0], C[1][1]);
		fp2_rdc_NEON(c[2][0], c[2][1], C[2][0], C[2][1]);
	#else
		  fp6_rdc(c,C);
	#endif
	 }
	 CATCH_ANY {
	          THROW(ERR_CAUGHT);
	 }
	 FINALLY {
	          dv6_free(C);
	 }
}

void fp6_mul_nr(dv6_t C, fp6_t a, fp6_t b) {
// Sextic extension field multiplication w/o reduction
         TRY {
	         fp6_mulkar_nr(C, a, b);      // Karatsuba multiplication
	 }
	 CATCH_ANY {
	         THROW(ERR_CAUGHT);
	 }
	 FINALLY {
	 }
}

/*
void fp6_mul_dxs(fp6_t c, fp6_t a, fp6_t b) {
	fp2_t v0, v1, t0, t1, t2;

	fp2_null(v0);
	fp2_null(v1);
	fp2_null(t0);
	fp2_null(t1);
	fp2_null(t2);

	TRY {
		fp2_new(v0);
		fp2_new(v1);
		fp2_new(t0);
		fp2_new(t1);
		fp2_new(t2);

		// v0 = a0b0
		fp2_mul(v0, a[0], b[0]);

		// v1 = a1b1 
		fp2_mul(v1, a[1], b[1]);

		// v2 = a2b2 = 0 

		// t2 (c0) = v0 + E((a1 + a2)(b1 + b2) - v1 - v2) 
		fp2_add(t0, a[1], a[2]);
		fp2_mul(t2, t0, b[1]);
		fp2_sub(t2, t2, v1);
		fp2_mul_nor(t2, t2);
		fp2_add(t2, t2, v0);

		// c1 = (a0 + a1)(b0 + b1) - v0 - v1 + Ev2 
		fp2_add(t0, a[0], a[1]);
		fp2_add(t1, b[0], b[1]);
		fp2_mul(c[1], t0, t1);
		fp2_sub(c[1], c[1], v0);
		fp2_sub(c[1], c[1], v1);

		// c2 = (a0 + a2)(b0 + b2) - v0 + v1 - v2 
		fp2_add(t0, a[0], a[2]);
		fp2_mul(c[2], t0, b[0]);
		fp2_sub(c[2], c[2], v0);
		fp2_add(c[2], c[2], v1);

		// c0 = t2 
		fp2_copy(c[0], t2);
	} CATCH_ANY {
		THROW(ERR_CAUGHT);
	} FINALLY {
		fp2_free(v0);
		fp2_free(v1);
		fp2_free(t0);
		fp2_free(t1);
		fp2_free(t2);
	}
}
*/

void fp6_mul_dxs(fp6_t c, fp6_t a, fp6_t b) {
// "Simplified" sextic extension field multiplication using Karatsuba and lazy reduction
  fp2_t t0, t1, t2;
                dv2_t T0, T1, T2;

                fp2_null(t0);
                fp2_null(t1);
                fp2_null(t2);
                dv2_null(T0);
                dv2_null(T1);
                dv2_null(T2);

                TRY {
                       fp2_new(t0);
                       fp2_new(t1);
                       fp2_new(t2);
                       dv2_new(T0);
                       dv2_new(T1);
                       dv2_new(T2);

                       // v0 = a0b0
                       fp2_mul_nr(T0, a[0], b[0]);

                       // v1 = a1b1
                       fp2_mul_nr(T1, a[1], b[1]);

                       // v2 = a2b2 = 0, b2 = 0
                       // t2 (c0) = v0 + E((a1 + a2)b1) - v1)
                       fp2_add(t0, a[1], a[2]);
                       fp2_mul_nr(T2, t0, b[1]);
                       fp2_subd(T2, T2, T1);
                       fp2_mul_nord(T2, T2);
                       fp2_addd(T2, T2, T0);
                       fp2_rdc(t2, T2);

                       // c1 = (a0 + a1)(b0 + b1) - v0 - v1
                       fp2_add(t0, a[0], a[1]);
                       fp2_add(t1, b[0], b[1]);
                       fp2_mul_nr(T2, t0, t1);
                       fp2_subd(T2, T2, T0);
                       fp2_subd(T2, T2, T1);
                       fp2_rdc(c[1], T2);

                       // c2 = (a0 + a2)b0 - v0 + v1
                       fp2_add(t0, a[0], a[2]);
                       fp2_mul_nr(T2, t0, b[0]);
                       fp2_subd(T2, T2, T0);
                       fp2_addd(T2, T2, T1);
                       fp2_rdc(c[2], T2);

                       // c0 = t2
                       fp2_copy(c[0], t2);
         } CATCH_ANY {
                       THROW(ERR_CAUGHT);
         } FINALLY {
		       fp2_free(t0);
		       fp2_free(t1);
		       fp2_free(t2);
		       dv2_free(T0);
		       dv2_free(T1);
		       dv2_free(T2);
          }
}

void fp6_mul_dxs_nr(dv6_t C, fp6_t a, fp6_t b) {
// "Simplified" sextic extension field multiplication w/o reduction using Karatsuba and lazy reduction
          fp2_t t0, t1;
	  dv2_t T0, T1, T2;

	  fp2_null(t0);
	  fp2_null(t1);
	  dv2_null(T0);
	  dv2_null(T1);
	  dv2_null(T2);

	  TRY {
	    fp2_new(t0);
	    fp2_new(t1);
	    dv2_new(T0);
	    dv2_new(T1);
	    dv2_new(T2);

	    // v0 = a0b0
	    fp2_mul_nr(T0, a[0], b[0]);

	    // v1 = a1b1
	    fp2_mul_nr(T1, a[1], b[1]);

	    // T2 (C0) = v0 + E(a2b1)
	    fp2_mul_nr(T2, a[2], b[1]);
	    fp2_mul_nord(T2, T2);
	    fp2_addd(T2, T2, T0);	    
	    
	    // C1 = (a0 + a1)(b0 + b1) - v0 - v1
	    fp2_add(t0, a[0], a[1]);
	    fp2_add(t1, b[0], b[1]);
	    fp2_mul_nr(C[1], t0, t1);
	    fp2_subd(C[1], C[1], T0);
	    fp2_subd(C[1], C[1], T1);
	   
	    // C2 = a2b0  + v1
	    fp2_mul_nr(C[2], a[2], b[0]);
	    fp2_addd(C[2],C[2], T1);

	    // C0 = T2
	    dv2_copy(C[0], T2);
  } CATCH_ANY {
    THROW(ERR_CAUGHT);
  } FINALLY {
    fp2_free(t0);
    fp2_free(t1);
    dv2_free(T0);
    dv2_free(T1);
    dv2_free(T2);
  }
}

void fp6_mul_dxq(fp6_t c, fp6_t a, fp2_t b) {
	fp2_mul(c[0], a[0], b);
	fp2_mul(c[1], a[1], b);
	fp2_mul(c[2], a[2], b);
}

void fp6_mul_dxqq(fp6_t c, fp6_t a, fp_t b) {
//	fp_mul(c[0][0], a[0][0], b);
//	fp_mul(c[0][1], a[0][1], b);
//	fp_mul(c[1][0], a[1][0], b);
//	fp_mul(c[1][1], a[1][1], b);
//	fp_mul(c[2][0], a[2][0], b);
//	fp_mul(c[2][1], a[2][1], b);
	MUL638NEONMOD(c[0][0], c[0][1], a[0][0], b, a[0][1], b );
	MUL638NEONMOD(c[1][0], c[1][1], a[1][0], b, a[1][1], b );
	MUL638NEONMOD(c[2][0], c[2][1], a[2][0], b, a[2][1], b );
}

void fp6_mul_dxq_nr(dv6_t C, fp6_t a, fp2_t b) {
  fp2_mul_nr(C[0], a[0], b);
  fp2_mul_nr(C[1], a[1], b);
  fp2_mul_nr(C[2], a[2], b);
}

void fp6_mul_dxqq_nr(dv6_t C, fp6_t a, fp_t b) {
	/*fp_muln_low(C[0][0], a[0][0], b);
	fp_muln_low(C[0][1], a[0][1], b);
	fp_muln_low(C[1][0], a[1][0], b);
	fp_muln_low(C[1][1], a[1][1], b);
	fp_muln_low(C[2][0], a[2][0], b);
	fp_muln_low(C[2][1], a[2][1], b);*/
	
	MUL638NEON(C[0][0], C[0][1], a[0][0], b, a[0][1], b);
	MUL638NEON(C[1][0], C[1][1], a[1][0], b, a[1][1], b);
	MUL638NEON(C[2][0], C[2][1], a[2][0], b, a[2][1], b);
}

void fp6_mul_dxq2_nr(dv6_t C, fp6_t a, fp2_t b) {
  fp2_mul_nr(C[1], a[0], b);
  fp2_mul_nr(C[2], a[1], b);
  fp2_mul_nr(C[0], a[2], b);
  fp2_mul_nord(C[0], C[0]);
}

void fp6_mul_dxqq2_nr(dv6_t C, fp6_t a, fp_t b) {
  /*fp_muln_low(C[1][0], a[0][0], b);
  fp_muln_low(C[1][1], a[0][1], b);
  fp_muln_low(C[2][0], a[1][0], b);
  fp_muln_low(C[2][1], a[1][1], b);
  fp_muln_low(C[0][0], a[2][0], b);
  fp_muln_low (C[0][1], a[2][1], b);
  */
  
  MUL638NEON(C[1][0], C[1][1], a[0][0], b, a[0][1], b);
  MUL638NEON(C[2][0], C[2][1], a[1][0], b, a[1][1], b);
  MUL638NEON(C[0][0], C[0][1], a[2][0], b, a[2][1], b);
  
  fp2_mul_nord(C[0], C[0]);
}

void fp6_mul_art(fp6_t c, fp6_t a) {
	fp2_t t0;

	fp2_null(t0);

	TRY {
		fp2_new(t0);

		/* (a0 + a1 * v + a2 * v^2) * v = a2 + a0 * v + a1 * v^2 */
		fp2_copy(t0, a[0]);
		fp2_mul_nor(c[0], a[2]);
		fp2_copy(c[2], a[1]);
		fp2_copy(c[1], t0);
	} CATCH_ANY {
		THROW(ERR_CAUGHT);
	} FINALLY {
		fp2_free(t0);
	}
}

void fp6_mul_artd(dv6_t C, dv6_t A) {
        dv2_t T0;

	dv2_null(T0);

	TRY {
               dv2_new(T0);

	       /* (a0 + a1 * v + a2 * v^2) * v = a2 + a0 * v + a1 * v^2 */
	       dv2_copy(T0, A[0]);
	       fp2_mul_nord(C[0], A[2]);
	       dv2_copy(C[2], A[1]);
	       dv2_copy(C[1], T0);
	} CATCH_ANY {
               THROW(ERR_CAUGHT);
	} FINALLY {
	       dv2_free(T0);
	}
}

void fp6_sqr(fp6_t c, fp6_t a) {
	fp2_t t0, t1, t2, t3, t4;

	fp2_null(t0);
	fp2_null(t1);
	fp2_null(t3);
	fp2_null(t4);

	TRY {
		fp2_new(t0);
		fp2_new(t1);
		fp2_new(t2);
		fp2_new(t3);
		fp2_new(t4);

		// t0 = a0^2
		fp2_sqr(t0, a[0]);

		// t1 = 2 * a0 * a1 
		fp2_mul(t1, a[0], a[1]);
		fp2_dbl(t1, t1);

		// t2 = (a0 - a1 + a2)^2 
		fp2_sub(t2, a[0], a[1]);
		fp2_add(t2, t2, a[2]);
		fp2_sqr(t2, t2);

		// t3 = 2 * a1 * a2 
		fp2_mul(t3, a[1], a[2]);
		fp2_dbl(t3, t3);

		// t4 = a2^2 
		fp2_sqr(t4, a[2]);

		// c0 = t0 + E * t3 
		fp2_mul_nor(c[0], t3);
		fp2_add(c[0], c[0], t0);

		// c1 = t1 + E * t4 
		fp2_mul_nor(c[1], t4);
		fp2_add(c[1], c[1], t1);

		// c2 = t1 + t2 + t3 - t0 - t4 
		fp2_add(c[2], t1, t2);
		fp2_add(c[2], c[2], t3);
		fp2_sub(c[2], c[2], t0);
		fp2_sub(c[2], c[2], t4);
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

void fp6_sqr_nr(dv6_t C, fp6_t a) {
	fp2_t t2;
	dv2_t T0, T1, T2, T3, T4;


	fp2_null(t2);
	dv2_null(T0);
	dv2_null(T1);
dv2_null(T2);
dv2_null(T3);
dv2_null(T4);

	TRY {

		fp2_new(t2);
		dv2_new(T0);
		dv2_new(T1);
		dv2_new(T2);
		dv2_new(T3);
		dv2_new(T4);

		// t0 = a0^2
		fp2_sqr_nr(T0, a[0]);

		// t1 = 2 * a0 * a1 
		fp2_mul_nr(T1, a[0], a[1]);
		fp2_addd(T1, T1, T1);

		// t2 = (a0 - a1 + a2)^2 
		fp2_sub(t2, a[0], a[1]);
		fp2_add(t2, t2, a[2]);
		fp2_sqr_nr(T2, t2);

		// t3 = 2 * a1 * a2 
		fp2_mul_nr(T3, a[1], a[2]);
		fp2_addd(T3, T3, T3);

		// t4 = a2^2 
		fp2_sqr_nr(T4, a[2]);

		// c0 = t0 + E * t3 
		fp2_mul_nord(C[0], T3);
		fp2_addd(C[0], C[0], T0);

		// c1 = t1 + E * t4 
		fp2_mul_nord(C[1], T4);
		fp2_addd(C[1], C[1], T1);

		// c2 = t1 + t2 + t3 - t0 - t4 
		fp2_addd(C[2], T1, T2);
		fp2_addd(C[2], C[2], T3);
		fp2_subd(C[2], C[2], T0);
		fp2_subd(C[2], C[2], T4);
	} CATCH_ANY {
		THROW(ERR_CAUGHT);
	} FINALLY {

		fp2_free(t2);
		dv2_free(T0);
dv2_free(T1);
dv2_free(T2);
dv2_free(T3);
dv2_free(T4);
	}
}

void fp6_exp(fp6_t c, fp6_t a, bn_t b) {
	fp6_t t;

	fp6_null(t);

	TRY {
		fp6_new(t);

		fp6_copy(t, a);

		for (int i = bn_bits(b) - 2; i >= 0; i--) {
			fp6_sqr(t, t);
			if (bn_test_bit(b, i)) {
				fp6_mul(t, t, a);
			}
		}
		fp6_copy(c, t);
	}
	CATCH_ANY {
		THROW(ERR_CAUGHT);
	}
	FINALLY {
		fp6_free(t);
	}
}

/*
void fp6_frb(fp6_t c, fp6_t a) {
	fp2_t t;

	TRY {
		fp2_new(t);

		fp2_const_get(t);
		fp2_sqr(t, t);

		fp2_frb(c[0], a[0]);
		fp2_frb(c[1], a[1]);
		fp2_frb(c[2], a[2]);

		fp2_mul(c[1], c[1], t);
		fp2_sqr(t, t);
		fp2_mul(c[2], c[2], t);

	} CATCH_ANY {
		THROW(ERR_CAUGHT);
	} FINALLY {
		fp2_free(t);
	}
}
*/

void fp6_frb(fp6_t c, fp6_t a) {
        fp2_frb(c[0], a[0]);
	fp2_frb(c[1], a[1]);
	fp2_frb(c[2], a[2]);

	fp2_mul_frb(c[1], c[1], 2);
	fp2_mul_frb(c[2], c[2], 4);
}

void fp6_inv(fp6_t c, fp6_t a) {
	fp2_t v0;
	fp2_t v1;
	fp2_t v2;
	fp2_t t0;
	dv2_t V0;
	dv2_t V1;
	dv2_t V2;
	dv2_t T0;

	fp2_null(v0);
	fp2_null(v1);
	fp2_null(v2);
	fp2_null(t0);
	dv2_null(V0);
	dv2_null(V1);
	dv2_null(V2);
	dv2_null(T0);

	TRY {
		fp2_new(v0);
		fp2_new(v1);
		fp2_new(v2);
		fp2_new(t0);
		dv2_new(V0);
		dv2_new(V1);
		dv2_new(V2);
		dv2_new(T0);

		fp2_sqr_nr(T0, a[0]);
		fp2_mul_nr(V0, a[1], a[2]);
		fp2_mul_nord(V0, V0);
		fp2_subd(V0, T0, V0);
		fp2_rdc(v0, V0);

		fp2_sqr_nr(T0, a[2]);
		fp2_mul_nord(T0, T0);
		fp2_mul_nr(V1, a[0], a[1]);
		fp2_subd(V1, T0, V1);
		fp2_rdc(v1, V1);

		fp2_sqr_nr(T0, a[1]);
		fp2_mul_nr(V2, a[0], a[2]);
		fp2_subd(V2, T0, V2);
		fp2_rdc(v2, V2);

		fp2_mul(c[1], a[1], v2);
		fp2_mul_nor(c[1], c[1]);

		fp2_mul(c[0], a[0], v0);

		fp2_mul(c[2], a[2], v1);
		fp2_mul_nor(c[2], c[2]);

		fp2_add(t0, c[0], c[1]);
		fp2_add(t0, t0, c[2]);
		fp2_inv(t0, t0);

		fp2_mul(c[0], v0, t0);
		fp2_mul(c[1], v1, t0);
		fp2_mul(c[2], v2, t0);
	} CATCH_ANY {
		THROW(ERR_CAUGHT);
	} FINALLY {
		fp2_free(v0);
		fp2_free(v1);
		fp2_free(v2);
		fp2_free(t0);
		dv2_free(V0);
		dv2_free(V1);
		dv2_free(V2);
		dv2_free(T0);
	}
}
