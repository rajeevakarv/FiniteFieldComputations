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
 * Implementation of quadratic equation solution on binary fields.
 *
 * @version $Id: relic_fb_slv.c 160 2009-11-07 17:17:34Z dfaranha $
 * @ingroup fb
 */

#include <string.h>

#include "relic_core.h"
#include "relic_conf.h"
#include "relic_fb.h"
#include "relic_fb_low.h"
#include "relic_util.h"

/*============================================================================*/
/* Public definitions                                                         */
/*============================================================================*/

#if FB_SLV == BASIC || !defined(STRIP)

void fb_slv_basic(fb_t c, fb_t a) {
	int i;
	fb_t t0;

	fb_null(t0);

	TRY {
		fb_new(t0);

		fb_copy(t0, a);
		fb_copy(c, a);

		for (i = 0; i < (FB_BITS - 1) / 2; i++) {
			fb_sqr(c, c);
			fb_sqr(c, c);
			fb_add(c, c, t0);
		}

		fb_trc(t0, c);

		if (t0[0] == 1) {
			fb_add_dig(c, c, 1);
		}
	}
	CATCH_ANY {
		THROW(ERR_CAUGHT);
	}
	FINALLY {
		fb_free(t0);
	}
}

#endif

#if FB_SLV == QUICK || !defined(STRIP)

void fb_slv_quick(fb_t c, fb_t a) {
	fb_t t;

	fb_null(t);

	TRY {
		fb_new(t);
		fb_slvn_low(c, a);
		fb_trc(t, c);

		if (t[0] == 1) {
			fb_add_dig(c, c, 1);
		}
	} CATCH_ANY {
		THROW(ERR_CAUGHT);
	} FINALLY {
		fb_free(t);
	}
}

#endif
