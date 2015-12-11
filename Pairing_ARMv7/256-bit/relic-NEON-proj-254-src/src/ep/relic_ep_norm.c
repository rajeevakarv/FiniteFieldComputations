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
 * Implementation of the point normalization on prime elliptic curves.
 *
 * @version $Id: relic_ep_norm.c 207 2009-12-25 20:15:28Z dfaranha $
 * @ingroup ep
 */

#include "string.h"

#include "relic_core.h"
#include "relic_ep.h"
#include "relic_error.h"

/*============================================================================*/
/* Private definitions                                                        */
/*============================================================================*/

#if EP_ADD == PROJC || defined(EP_MIXED)

/**
 * Normalizes a point represented in projective coordinates.
 *
 * @param r			- the result.
 * @param p			- the point to normalize.
 */
void ep_norm_impl(ep_t r, ep_t p) {
	if (!p->norm) {
		fp_t t0, t1;

		fp_null(t0);
		fp_null(t1);

		TRY {

			fp_new(t0);
			fp_new(t1);

			fp_inv(t1, p->z);
			fp_sqr(t0, t1);
			fp_mul(r->x, p->x, t0);
			fp_mul(t0, t0, t1);
			fp_mul(r->y, p->y, t0);
			fp_set_dig(r->z, 1);
		} CATCH_ANY {
			THROW(ERR_CAUGHT);
		} FINALLY {
			fp_free(t0);
			fp_free(t1);
		}
	}

	r->norm = 1;
}

#endif /* EP_ADD == PROJC || EP_MIXED */

/*============================================================================*/
/* Public definitions                                                         */
/*============================================================================*/

void ep_norm(ep_t r, ep_t p) {
	if (ep_is_infty(p)) {
		ep_set_infty(r);
		return;
	}

	if (p->norm) {
		/* If the point is represented in affine coordinates, we just copy it. */
		ep_copy(r, p);
		return;
	}
#if EP_ADD == PROJC || !defined(STRIP)
	ep_norm_impl(r, p);
#endif /* EP_ADD == PROJC */
}
