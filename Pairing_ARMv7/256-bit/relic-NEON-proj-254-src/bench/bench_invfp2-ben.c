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
 * MODIFIED BY: Patrick Longa, 05/2011.
 * 
 */

/**
 * @file
 *
 * Benchmarks for the binary elliptic curve module.
 *
 * @version $Id: bench_pp.c 274 2010-02-06 05:34:38Z dfaranha $
 * @ingroup bench
 */

#include <stdio.h>

#include "relic.h"


int main(void) {
	core_init();
	conf_print();



	util_print_banner("Benchmarks for the PP module:", 0);

	fp_param_set_any_tower();
	fp_param_print();
	
	int i;
	fp2_t a, b, c;
	fp2_null(a);
	fp2_null(b);
	fp2_null(c);
	fp2_new(a);
	fp2_new(b);
	fp2_new(c);
	
	for (i=0;i<100000;i++){
	
		fp2_rand(a);
		//fp2_rand(b);
		//fp2_mul(c, a, b);
		//fp2_inv(c, a);
	}

	fp2_free(a);
	fp2_free(b);
	fp2_free(c);

	core_clean();
	return 0;
}
