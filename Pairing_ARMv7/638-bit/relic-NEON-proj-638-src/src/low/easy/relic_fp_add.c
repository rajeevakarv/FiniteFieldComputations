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
 * Implementation of the prime field addition and subtraction functions.
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

/*============================================================================*/
/* Public definitions                                                         */
/*============================================================================*/

void fp_add(fp_t c, fp_t a, fp_t b) {
 	dig_t carry;

  	carry = fp_addn_low(c, a, b);
        
	if (carry || (fp_cmp(c, fp_prime_get()) != CMP_LT)) {
     		carry = fp_subn_low(c, c, fp_prime_get());
	}
}

void fp_addd(dv_t c, dv_t a, dv_t b) {
	  dig_t carry;
	  extern bn_t PRIMEXPRIME;

	  carry = bn_addn_low(c, a, b, 2*FP_DIGS);

	  //if (carry || (mpn_cmp(c, PRIMEXPRIME->dp, 2*FP_DIGS) != CMP_LT)) {
	  if (carry || (bn_cmpn_low(c, PRIMEXPRIME->dp, 2*FP_DIGS) != CMP_LT)) {
    	         carry = bn_subn_low(c, c, PRIMEXPRIME->dp, 2*FP_DIGS);
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
	dig_t carry;

     	carry = fp_subn_low(c, a, b);
        
	if (carry) {
	        fp_addn_low(c, c, fp_prime_get());	        
	}
}

void fp_subd(dv_t c, dv_t a, dv_t b) {
        dig_t carry=0;
        extern bn_t PRIMEXPRIME;

	carry = bn_subn_low(c, a, b, 2*FP_DIGS);
 
        if (carry) {
	        bn_addn_low(c, c, PRIMEXPRIME->dp, 2*FP_DIGS);
        }
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

        carry = fp_rsh1_low(c, a);

        if (carry != OPT_ZERO) {
                carry2 = fp_addn_low(c, a, fp_prime_get());
                fp_rsh1_low(c, c);
                if (carry2 != OPT_ZERO) c[FP_DIGS-1] = carry | c[FP_DIGS-1];
        }
}

/*void fp_div2(fp_t c, fp_t a) {
	fp_t t, s;
	
	fp_null(t);
	fp_new(t);

	fp_null(s);
	fp_new(t);

	fp_set_dig(t, 2);
	
	fp_inv(s, t);
	fp_mul(c, a, s);
}*/
