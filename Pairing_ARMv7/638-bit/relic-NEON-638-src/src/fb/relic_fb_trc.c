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
 * Implementation of binary field trace function.
 *
 * @version $Id: relic_fb_trc.c 142 2009-10-06 04:24:37Z dfaranha $
 * @ingroup fb
 */

#include "relic_core.h"
#include "relic_fb.h"
#include "relic_fb_low.h"
#include "relic_error.h"

/*============================================================================*/
/* Public definitions                                                         */
/*============================================================================*/

void fb_trc_basic(fb_t c, fb_t a) {
	fb_t t0;

	fb_null(t0);

	TRY {
		fb_new(t0);

		fb_copy(t0, a);
		fb_copy(c, a);
		for (int i = 1; i < FB_BITS; i++) {
			fb_sqr(t0, t0);
			fb_add(c, c, t0);
		}
	}
	CATCH_ANY {
		THROW(ERR_CAUGHT);
	}
	FINALLY {
		fb_free(t0);
	}
}

void fb_trc_quick(fb_t c, fb_t a) {
	int ta, tb, tc;
	dig_t r;

	fb_poly_get_trc(&ta, &tb, &tc);

	r = fb_get_bit(a, ta);
	if (tb != -1) {
		r ^= fb_get_bit(a, tb);
	}
	if (tc != -1) {
		r ^= fb_get_bit(a, tc);
	}
	c[0] = r;
	for (int i = 1; i < FB_DIGS; i++) {
		c[i] = 0;
	}
}
