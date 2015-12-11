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
 * Implementation of the multiple precision integer arithmetic multiplication
 * functions.
 *
 * @version $Id: relic_bn_mul_low.c 88 2009-09-06 21:27:19Z dfaranha $
 * @ingroup bn
 */

#include "relic_bn.h"
#include "relic_bn_low.h"
#include "relic_util.h"

/*============================================================================*/
/* Public definitions                                                         */
/*============================================================================*/

dig_t bn_muladd_low(dig_t *c, dig_t *a, dig_t digit, int size) {
	return mpn_addmul_1(c, a, size, digit);
}

dig_t bn_mul1_low(dig_t *c, dig_t *a, dig_t digit, int size) {
	return mpn_mul_1(c, a, size, digit);
}

void bn_muln_low(dig_t *c, dig_t *a, dig_t *b, int size) {
	mpn_mul_n(c, a, b, size);
}

void bn_muld_low(dig_t *c, dig_t *a, int sizea, dig_t *b, int sizeb,
		int low, int high) {
	(void) low;
	(void) high;
	mpn_mul(c, a, sizea, b, sizeb);
}
