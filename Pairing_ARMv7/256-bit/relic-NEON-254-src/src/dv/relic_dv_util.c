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
 * Implementation of the basic functions to temporary double precision digit
 * vectors.
 *
 * @version $Id: relic_dv_util.c 120 2009-09-11 22:39:26Z dfaranha $
 * @ingroup dv
 */

#include "relic_core.h"
#include "relic_dv.h"
#include "relic_fp.h"
#include "relic_fp_low.h"
#include "relic_rand.h"
#include "relic_error.h"

#define USE_NEON_DV 0
/*============================================================================*/
/* Public definitions                                                         */
/*============================================================================*/

void dv_print(dv_t a, int digits) {
	int i;

	/* Suppress possible unused parameter warning. */
	(void) a;
	for (i = digits - 1; i >= 0; i--) {
		util_print("%.*lX", (int)(2 * sizeof(dig_t)), (unsigned long int)a[i]);
	}
	util_print("\n");

	return;
}

void dv_zero(dv_t a, int digits) {
#if USE_NEON_DV
	asm(
	"veor q0,q0,q0 \n\t"
	"veor q1,q1,q1 \n\t"
	"veor q2,q2,q2 \n\t"
	"veor q3,q3,q3 \n\t"
	"vstmia %0, {q0-q3} \n\t"
	:
	:"r"(&a[0])
	:"memory","r3","r4","r5","q0","q1","q2","q3","q4","q5","q6","q7","q8","q9","q10","q11","q12","q13","q14","q15");
#else
	int i;

	if (digits > DV_DIGS) {
		THROW(ERR_NO_PRECISION);
	}

	for (i = 0; i < digits; i++, a++)
		(*a) = 0;

	return;
#endif
	
}

void dv_copy(dv_t c, dv_t a, int digits) {
#if USE_NEON_DV
if(digits==FP_DIGS)	
{	
	asm(
	"vldmia %1, {q0-q1} \n\t"
	"vstmia %0, {q0-q1} \n\t"
	:
	:"r"(&c[0]), "r"(&a[0])
	:"memory","r3","r4","r5","q0","q1","q2","q3","q4","q5","q6","q7","q8","q9","q10","q11","q12","q13","q14","q15");
}
else if(digits==2*FP_DIGS)
{	
	asm(
	"vldmia %1, {q0-q3} \n\t"
	"vstmia %0, {q0-q3} \n\t"
	:
	:"r"(&c[0]), "r"(&a[0])
	:"memory","r3","r4","r5","q0","q1","q2","q3","q4","q5","q6","q7","q8","q9","q10","q11","q12","q13","q14","q15");
	}
else
	for (int i = 0; i < digits; i++, c++, a++) {
		*c = *a;
	}
#else
	for (int i = 0; i < digits; i++, c++, a++) {
		*c = *a;
	}
#endif
}
