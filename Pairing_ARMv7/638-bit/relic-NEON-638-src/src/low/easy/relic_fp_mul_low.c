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
 * Implementation of the low-level prime field multiplication functions.
 *
 * @version $Id: relic_fp_mul_low.c 119 2009-09-11 22:38:00Z dfaranha $
 * @ingroup bn
 */

#include "relic_fp.h"
#include "relic_fp_low.h"

/*============================================================================*/
/* Private definitions                                                        */
/*============================================================================*/

/**
 * Accumulates a double precision digit in a triple register variable.
 *
 * @param[in,out] R2		- most significant word of the triple register.
 * @param[in,out] R1		- middle word of the triple register.
 * @param[in,out] R0		- lowest significant word of the triple register.
 * @param[in] A				- the first digit to multiply.
 * @param[in] B				- the second digit to multiply.
 */
#define COMBA_STEP(R2, R1, R0, A, B)										\
	dbl_t r = (dbl_t)(A) * (dbl_t)(B);										\
	dig_t _r = (R1);														\
	(R0) += (dig_t)(r);														\
	(R1) += (R0) < (dig_t)(r);												\
	(R2) += (R1) < _r;														\
	(R1) += (dig_t)((r) >> (dbl_t)BN_DIGIT);								\
	(R2) += (R1) < (dig_t)((r) >> (dbl_t)BN_DIGIT);							\

/*============================================================================*/
/* Public definitions                                                         */
/*============================================================================*/

dig_t fp_muladd_low(dig_t *c, dig_t *a, dig_t digit) {
	int i;
	dig_t carry;
	dbl_t r;

	carry = 0;
	for (i = 0; i < FP_DIGS; i++, a++, c++) {
		/* Multiply the digit *tmpa by b and accumulate with the previous
		 * result in the same columns and the propagated carry. */
		r = (dbl_t)(*c) + (dbl_t)(*a) * (dbl_t)(digit) + (dbl_t)(carry);
		/* Increment the column and assign the result. */
		*c = (dig_t)r;
		/* Update the carry. */
		carry = (dig_t)(r >> (dbl_t)FP_DIGIT);
	}
	return carry;
}

void fp_muln_low(dig_t *c, dig_t *a, dig_t *b) {
	int i, j;
	dig_t *tmpa, *tmpb, r0, r1, r2;

	r0 = r1 = r2 = 0;
	for (i = 0; i < FP_DIGS; i++, c++) {
		tmpa = a;
		tmpb = b + i;
		for (j = 0; j <= i; j++, tmpa++, tmpb--) {
			COMBA_STEP(r2, r1, r0, *tmpa, *tmpb);
		}
		*c = r0;
		r0 = r1;
		r1 = r2;
		r2 = 0;
	}
	for (i = 0; i < FP_DIGS; i++, c++) {
		tmpa = a + i + 1;
		tmpb = b + (FP_DIGS - 1);
		for (j = 0; j < FP_DIGS - (i + 1); j++, tmpa++, tmpb--) {
			COMBA_STEP(r2, r1, r0, *tmpa, *tmpb);
		}
		*c = r0;
		r0 = r1;
		r1 = r2;
		r2 = 0;
	}
}

void fp_mulm_low(dig_t *c, dig_t *a, dig_t *b) {
	dig_t align t[2 * FP_DIGS];

	fp_muln_low(t, a, b);
	fp_rdc(c, t);
}
