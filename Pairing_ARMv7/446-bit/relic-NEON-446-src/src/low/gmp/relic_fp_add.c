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
 * MODIFIED BY: Patrick Longa, 05/2011.
 * 
 */

/**
 * @file
 *
 * Implementation of the prime field addition, subtraction and division by 2 functions.
 *
 * @version $Id$
 * @ingroup fp
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "relic_core.h"
#include "relic_fp.h"
#include "relic_fp_low.h"
#include "relic_error.h"

#define __ADDITION_IN_ASM__		0

/*============================================================================*/
/* Public definitions                                                         */
/*============================================================================*/
void fp_add_low(fp_t, fp_t, fp_t, fp_t);
void fp_sub_low(fp_t, fp_t, fp_t, fp_t);
void fp_addd_low(dv_t, dv_t, dv_t, dv_t); 
void fp_subd_low(dv_t, dv_t, dv_t, dv_t);

void fp_add(fp_t c, fp_t a, fp_t b) {
// Modular addition in assembly, prime specific                
#if __ADDITION_IN_ASM__ == 1

          fp_add_low(c, a, b, fp_prime_get());                   

#else

			// just for timing test code
			dig_t carry;
			carry = fp_addn_low(c, a, b);   
			if (carry || fp_cmp(c, fp_prime_get()) != CMP_LT) {
				carry = fp_subn_low(c, c, fp_prime_get());
			}
			
#endif
		
}

void fp_addd(dv_t c, dv_t a, dv_t b) {
          dig_t carry;
	  extern bn_t PRIMEXPRIME;

	  //fp_addd_low(c, a, b, PRIMEXPRIME->dp);

	  carry = mpn_add_n(c, a, b, 2*FP_DIGS);

	  if (carry || (mpn_cmp(c, PRIMEXPRIME->dp, 2*FP_DIGS) != CMP_LT)) {
	       carry = mpn_sub_n(c, c, PRIMEXPRIME->dp, 2*FP_DIGS);
	  }
}

void fp_add_dig(fp_t c, fp_t a, dig_t b) {
#if FP_RDC == MONTY
	fp_t t;

	fp_null(t);

	TRY {
		fp_new(t);

		fp_set_dig(t, b);
		fp_add(c, a, t);
	} CATCH_ANY {
		THROW(ERR_CAUGHT);
	} FINALLY {
		fp_free(t);
	}
#else
	dig_t carry;

	carry = fp_add1_low(c, a, b);
	if (carry || fp_cmp(c, fp_prime_get()) != CMP_LT) {
		carry = fp_subn_low(c, c, fp_prime_get());
	}
#endif
}

void fp_sub(fp_t c, fp_t a, fp_t b) {

#if __ADDITION_IN_ASM__ == 1

        fp_sub_low(c, a, b, fp_prime_get());

#else
		
				// just for timing test
				dig_t carry;
				carry = mpn_sub_n(c, a, b, FP_DIGS);	

				if (carry) {
					mpn_add_n(c, c, fp_prime_get(), FP_DIGS);
				}
				
#endif
}

void fp_subd(dv_t c, dv_t a, dv_t b) {
    extern bn_t PRIMEXPRIME;

#if __ADDITION_IN_ASM__ == 1
	
	fp_subd_low(c, a, b, PRIMEXPRIME->dp);
	
#else

				// just for timing test
				dig_t carry=0;
				carry = mpn_sub_n(c, a, b, 2*FP_DIGS);
			 
					if (carry) {
						mpn_add_n(c, c, PRIMEXPRIME->dp, 2*FP_DIGS);
					}	
					
#endif
}

void fp_sub_dig(fp_t c, fp_t a, dig_t b) {
#if FP_RDC == MONTY
	fp_t t;

	fp_null(t);

	TRY {
		fp_new(t);

		fp_set_dig(t, b);
		fp_sub(c, a, t);
	} CATCH_ANY {
		THROW(ERR_CAUGHT);
	} FINALLY {
		fp_free(t);
	}
#else
	dig_t carry;

	carry = fp_sub1_low(c, a, b);
	if (carry) {
		fp_addn_low(c, c, fp_prime_get());
	}
#endif
}

void fp_div2(fp_t c, fp_t a) {
        dig_t carry, carry2;

        carry = mpn_rshift(c, a, FP_DIGS, 1);

        if (carry != OPT_ZERO) {
                carry2 = mpn_add_n(c, a, fp_prime_get(), FP_DIGS);
                mpn_rshift(c, c, FP_DIGS, 1);
                if (carry2 != OPT_ZERO) c[FP_DIGS-1] = carry | c[FP_DIGS-1];
  }
}
