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
 * Implementation of elliptic prime curves over quadratic extensions.
 *
 * @version $Id: relic_pp_pair.c 214 2010-01-25 03:09:40Z dfaranha $
 * @ingroup pp
 */

#include "relic_core.h"
#include "relic_pp.h"
#include "relic_error.h"
#include "relic_conf.h"

/*============================================================================*/
/* Public definitions                                                         */
/*============================================================================*/

void pp_pair_init(void) {
	fp2_const_init();
	ep2_curve_init();
}

void pp_pair_clean(void) {
	fp2_const_clean();
	ep2_curve_clean();
}
