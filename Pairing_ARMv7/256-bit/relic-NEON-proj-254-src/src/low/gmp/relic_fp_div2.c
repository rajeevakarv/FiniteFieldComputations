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
 * Author: Patrick Longa, 06/2011 
 * 
 */

/**
 * @file
 *
 * Implementation of the prime field division by 2 functions.
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

typedef uint32_t Fp[9];
typedef Fp Fp2[2];
typedef uint32_t Mul[17];

/*============================================================================*/
/* Public definitions                                                         */
/*============================================================================*/
void fp_div2(fp_t c, fp_t a) {
  dig_t carry, carry2;

        carry = mpn_rshift(c, a, FP_DIGS, 1);

	if (carry != "0") {
	        carry2 = mpn_add_n(c, a, fp_prime_get(), FP_DIGS);
		mpn_rshift(c, c, FP_DIGS, 1);
		if (carry2 != "0") c = carry | c[FP_DIGS-1];
	}
}
