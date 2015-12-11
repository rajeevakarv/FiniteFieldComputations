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
 * Implementation of the memory-management routines for temporary double
 * precision digit vectors.
 *
 * @version $Id: relic_dv_mem.c 113 2009-09-07 06:47:05Z dfaranha $
 * @ingroup dv
 */

#include <stdlib.h>
#include <errno.h>

#include "relic_core.h"
#include "relic_conf.h"
#include "relic_dv.h"
#include "relic_pool.h"

/*============================================================================*/
/* Public definitions                                                         */
/*============================================================================*/

#if ALLOC == DYNAMIC

void dv_new_dynam(dv_t *a, int digits) {
	int r;

	if (digits > DV_DIGS) {
		THROW(ERR_NO_PRECISION);
	}

	if (ALIGN == 1) {
		*a = malloc(digits * sizeof(dig_t));
	} else {
		r = posix_memalign((void **)&a, ALIGN,
				digits * sizeof(dig_t));
		if (r == ENOMEM) {
			THROW(ERR_NO_MEMORY);
		}
		if (r == EINVAL) {
			THROW(ERR_INVALID);
		}
	}

	if (*a == NULL) {
		free(*a);
		THROW(ERR_NO_MEMORY);
	}
}

void dv_free_dynam(dv_t *a) {
	if ((*a) != NULL) {
		free(*a);
	}
	(*a) = NULL;
}

#elif ALLOC == STATIC

void dv_new_statc(dv_t *a, int digits) {
	if (digits > DV_DIGS) {
		THROW(ERR_NO_PRECISION);
	}

	(*a) = pool_get();
	if ((*a) == NULL) {
		THROW(ERR_NO_MEMORY);
	}
}

void dv_free_statc(dv_t *a) {
	if ((*a) != NULL) {
		pool_put((*a));
	}
	(*a) = NULL;
}

#endif
