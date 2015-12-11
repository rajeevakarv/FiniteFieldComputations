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
 * @version $Id: relic_bn_sqr_low.c 88 2009-09-06 21:27:19Z dfaranha $
 * @ingroup bn
 */

#include "relic_bn.h"
#include "relic_bn_low.h"
#include "relic_util.h"

/*============================================================================*/
/* Public definitions                                                         */
/*============================================================================*/

void bn_sqradd_low(dig_t *c, dig_t *a, int size) {
	dig_t carry;
	dig_t digit;

	digit = *a;

	carry = mpn_addmul_1(c, a, size, digit);
	mpn_add_1(c+size, c+size, size, carry);
	if (size - 1 > 0) {
		carry = mpn_addmul_1(c+1, a+1, size-1, digit);
		mpn_add_1(c+size, c+size, size, carry);
	}
}

void bn_sqrn_low(dig_t *c, dig_t *a, int size) {
	mpn_mul_n(c, a, a, size);
}
