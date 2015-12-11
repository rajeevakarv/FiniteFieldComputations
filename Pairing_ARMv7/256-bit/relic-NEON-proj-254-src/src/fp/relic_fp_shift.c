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
 * Implementation of the prime field arithmetic shift functions.
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
#include "relic_util.h"
#include "relic_error.h"

#define USE_NEON_SHIFT 0
/*============================================================================*/
/* Public definitions                                                         */
/*============================================================================*/
/*void fp_dbl_low(fp_t c, fp_t a)
{
	asm(
	"VLDM 		%1,{q0, q1}     \n\t"
	"VTRN.32		q0, q3  \n\t"
	"vshl.u64 	q4, q0, #1  \n\t"
	"vshl.u64 	q5, q3, #1  \n\t"
	"vtrn.32 	q4, q5  \n\t"
	"vshl.u64 	q6, q5, #32  \n\t"
	"vorr		q4, q6  \n\t"
	"vshr.u64 	q6, q5, #32  \n\t"
	"vorr		d9, d12  \n\t"

	"veor		q3,q3,q3  \n\t"
	"vtrn.32	q1, q3  \n\t"
	"vshl.u64 	q5, q1, #1  \n\t"
	"vshl.u64 	q8, q3, #1  \n\t"
	"vtrn.32 	q5, q8  \n\t"
	"vshl.u64 	q9, q8, #32  \n\t"
	"vorr		q5, q9  \n\t"
	"vshr.u64 	q9, q8, #32  \n\t"
	"vorr		d11, d18  \n\t"
	"vorr		d10, d13  \n\t"
	"vstm		%0!, {q4, q5}  \n\t"
	"vstm		%0, {d19}  \n\t"
	
	:
	:"r"(&a[0]),"r"(&c[0])
	:"memory","r3","r4","r5","q0","q1","q2","q3","q4","q5","q6","q7","q8","q9","q10","q11","q12","q13","q14","q15");
	
}
*/
void fp_dbl_low(dig_t *c, dig_t *a);
void fp_dbl(fp_t c, fp_t a) {
	fp_add(c,a,a);
}

#if 0
void fp_dbl(fp_t c, fp_t a) {
#if USE_NEON_SHIFT
	uint32_t temp[9]={0,0,0,0,0,0,0,0,0};
	fp_dbl_low(temp,a);
	fp_copy(c,temp);
	if ((temp[8]==1) ||(fp_cmp(c, fp_prime_get()) != CMP_LT)) {
		fp_subn_low(c, c, fp_prime_get());
	}
#else
	dig_t carry;

	carry = fp_lsh1_low(c, a);

	/* If there is an additional carry. */
	if (carry || (fp_cmp(c, fp_prime_get()) != CMP_LT)) {
		fp_subn_low(c, c, fp_prime_get());
	}
#endif
}
#endif

void fp_hlv(fp_t c, fp_t a) {
	dig_t carry = 0;

	if (a[0] & 1) {
		carry = fp_addn_low(c, a, fp_prime_get());
	} else {
		dv_copy(c, a, FP_DIGS);
	}
	fp_rsh1_low(c, c);
	if (carry) {
		c[FP_DIGS - 1] ^= ((dig_t)1 << (FP_DIGIT - 1));
	}
}

void fp_lsh(fp_t c, fp_t a, int bits) {
	int digits;

	SPLIT(bits, digits, bits, FP_DIG_LOG);

	if (digits) {
		fp_lshd_low(c, a, digits);
	} else {
		if (c != a) {
			fp_copy(c, a);
		}
	}

	switch (bits) {
		case 0:
			break;
		case 1:
			fp_lsh1_low(c, c);
			break;
		default:
			fp_lshb_low(c, c, bits);
			break;
	}

}

void fp_rsh(fp_t c, fp_t a, int bits) {
	int digits;

	SPLIT(bits, digits, bits, FP_DIG_LOG);

	if (digits) {
		fp_rshd_low(c, a, digits);
	} else {
		if (c != a) {
			fp_copy(c, a);
		}
	}

	switch (bits) {
		case 0:
			break;
		case 1:
			fp_rsh1_low(c, c);
			break;
		default:
			fp_rshb_low(c, c, bits);
			break;
	}
}
