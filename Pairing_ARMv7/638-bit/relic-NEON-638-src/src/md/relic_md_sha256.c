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
 * Implementation of the SHA-256 hash function.
 *
 * @version $Id: relic_md_sha256.c 110 2009-09-07 04:05:39Z dfaranha $
 * @ingroup md
 */

#include "relic_conf.h"
#include "relic_core.h"
#include "relic_error.h"
#include "relic_md.h"
#include "sha.h"

/*============================================================================*/
/* Private definitions                                                        */
/*============================================================================*/

/**
 * SHA-256 internal context.
 */
static SHA256Context ctx;

/*============================================================================*/
/* Public definitions                                                         */
/*============================================================================*/

void md_map_sh256(unsigned char *hash, unsigned char *msg, int len) {
	if (SHA256Reset(&ctx) != shaSuccess) {
		THROW(ERR_INVALID);
	}
	if (SHA256Input(&ctx, msg, len) != shaSuccess) {
		THROW(ERR_INVALID);
	}
	if (SHA256Result(&ctx, hash) != shaSuccess) {
		THROW(ERR_INVALID);
	}
}
