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
 * Implementation of the prime field modulus manipulation.
 *
 * @version $Id: relic_fp_param.c 219 2010-01-25 03:13:14Z dfaranha $
 * @ingroup fb
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "relic_core.h"
#include "relic_conf.h"
#include "relic_dv.h"
#include "relic_fp.h"
#include "relic_fp_low.h"
#include "relic_error.h"

/*============================================================================*/
/* Private definitions                                                        */
/*============================================================================*/

/**
 * Current configured prime field identifier.
 */
static int param_id;

/*============================================================================*/
/* Public definitions                                                         */
/*============================================================================*/

bn_t PRIMEXPRIME;

int fp_param_get(void) {
	return param_id;
}

void fp_param_get_bn(bn_t x) {
        bn_t a;

        bn_null(a);

        TRY {
                bn_new(a);

                switch (param_id) {
                        case BN_158:
                                /* x = 4000000031. */
                                bn_set_2b(x, 38);
                                bn_add_dig(x, x, 0x31);
                                break;
                        /*case BN_254:
                                /* x = -4080000000000001. */
                                /*bn_set_2b(x, 62);
                                bn_set_2b(a, 55);
                                bn_add(x, x, a);
                                bn_add_dig(x, x, 1);
                                bn_neg(x, x);
                                break;
                        case BN_256:
                                /* x = 600000000000219B. */
                                /*bn_set_2b(x, 62);
                                bn_set_2b(a, 61);
                                bn_add(x, x, a);
                                bn_set_dig(a, 0x21);
                                bn_lsh(a, a, 8);
                                bn_add(x, x, a);
                                bn_add_dig(x, x, 0x9B);
                                bn_neg(x, x);
                                break;*/
			case BN_446:
				/* x = 2^110 + 2^36 + 1 */
				bn_set_2b(x, 110);
				bn_set_2b(a, 36);
				bn_add(x, x, a);
				bn_add_dig(x, x, 1);
				break;
			case BN_286:
                                /* x = -(2^70+2^58+2^38+1). */
                                bn_set_2b(x, 70);
                                bn_set_2b(a, 58);
                                bn_add(x, x, a);
				bn_set_2b(a, 38);
				bn_add(x, x, a);
                                bn_add_dig(x, x, 1);
                                bn_neg(x, x);
                                break;
			case BN_478:
                                /* x = -(2^118-2^55-2^19+1). */
                                bn_set_2b(x, 118);
                                bn_set_2b(a, 55);
                                bn_sub(x, x, a);
				bn_set_2b(a, 19);
				bn_sub(x, x, a);
                                bn_add_dig(x, x, 1);
                                bn_neg(x, x);
                                break;
			case BN_606:
                                /* x = -(2^150-2^95+2^8+1). */
                                bn_set_2b(x, 150);
                                bn_set_2b(a, 95);
                                bn_sub(x, x, a);
				bn_set_2b(a, 8);
				bn_add(x, x, a);
                                bn_add_dig(x, x, 1);
                                bn_neg(x, x);
                                break;
			case BN_638:
                                /* x = 2^158-2^128-2^68+1. */
                                bn_set_2b(x, 158);
                                bn_set_2b(a, 128);
                                bn_sub(x, x, a);
				bn_set_2b(a, 68);
				bn_sub(x, x, a);
                                bn_add_dig(x, x, 1);
                                break;
                        default:
                                THROW(ERR_INVALID);
                                break;
                }
        }
        CATCH_ANY {
                THROW(ERR_CAUGHT);
        }
        FINALLY {
                bn_free(a);
        }
}


void fp_param_set(int param) {
  bn_t t0, t1, p, prime;
	int f[10] = { 0 };
	int generated = 0;

	TRY {
		bn_new(t0);
		bn_new(t1);
		bn_new(p);

		param_id = param;

		switch (param) {
#if FP_PRIME == 158
                        case BN_158:
                                /* x = 4000000031. */
                                fp_param_get_bn(t0);
                                /* p = 36 * x^4 + 36 * x^3 + 24 * x^2 + 6 * x + 1. */
                                bn_set_dig(p, 1);
                                bn_mul_dig(t1, t0, 6);
                                bn_add(p, p, t1);
                                bn_mul(t1, t0, t0);
                                bn_mul_dig(t1, t1, 24);
                                bn_add(p, p, t1);
                                bn_mul(t1, t0, t0);
                                bn_mul(t1, t1, t0);
                                bn_mul_dig(t1, t1, 36);
                                bn_add(p, p, t1);
                                bn_mul(t0, t0, t0);
                                bn_mul(t1, t0, t0);
                                bn_mul_dig(t1, t1, 36);
                                bn_add(p, p, t1);
                                fp_prime_set_dense(p);
                                break;
#elif FP_PRIME == 160
			case SECG_160:
				/* p = 2^160 - 2^31 + 1. */
				f[0] = -1;
				f[1] = -31;
				fp_prime_set_spars(f, 2);
				break;
#elif FP_PRIME == 192
			case NIST_192:
				/* p = 2^192 - 2^64 - 1. */
				f[0] = -1;
				f[1] = -64;
				fp_prime_set_spars(f, 2);
				break;
#elif FP_PRIME == 224
			case NIST_224:
				/* p = 2^224 - 2^96 + 1. */
				f[0] = 1;
				f[1] = -96;
				fp_prime_set_spars(f, 2);
				break;
#elif FP_PRIME == 256
			case NIST_256:
				/* p = 2^256 - 2^224 + 2^192 + 2^96 - 1. */
				f[0] = -1;
				f[1] = 96;
				f[2] = 192;
				f[3] = -224;
				fp_prime_set_spars(f, 4);
				break;
			case BNN_256:
				/* x = -4080000000000001. */
				bn_set_2b(t0, 62);
				bn_set_2b(t1, 55);
				bn_add(t0, t0, t1);
				bn_add_dig(t0, t0, 1);
				bn_neg(t0, t0);
				/* p = 36 * x^4 + 36 * x^3 + 24 * x^2 + 6 * x + 1. */
				bn_set_dig(p, 1);
				bn_mul_dig(t1, t0, 6);
				bn_add(p, p, t1);
				bn_mul(t1, t0, t0);
				bn_mul_dig(t1, t1, 24);
				bn_add(p, p, t1);
				bn_mul(t1, t0, t0);
				bn_mul(t1, t1, t0);
				bn_mul_dig(t1, t1, 36);
				bn_add(p, p, t1);
				bn_mul(t0, t0, t0);
				bn_mul(t1, t0, t0);
				bn_mul_dig(t1, t1, 36);
				bn_add(p, p, t1);
				fp_prime_set_dense(p);
				break;
			case BNP_256:
				/* x = 6000000000001F2D. */
				bn_set_2b(t0, 62);
				bn_set_2b(t1, 61);
				bn_add(t0, t0, t1);
				bn_set_dig(t1, 0x1F);
				bn_lsh(t1, t1, 8);
				bn_add(t0, t0, t1);
				bn_add_dig(t0, t0, 0x2D);
				/* p = 36 * x^4 + 36 * x^3 + 24 * x^2 + 6 * x + 1. */
				bn_set_dig(p, 1);
				bn_mul_dig(t1, t0, 6);
				bn_add(p, p, t1);
				bn_mul(t1, t0, t0);
				bn_mul_dig(t1, t1, 24);
				bn_add(p, p, t1);
				bn_mul(t1, t0, t0);
				bn_mul(t1, t1, t0);
				bn_mul_dig(t1, t1, 36);
				bn_add(p, p, t1);
				bn_mul(t0, t0, t0);
				bn_mul(t1, t0, t0);
				bn_mul_dig(t1, t1, 36);
				bn_add(p, p, t1);
				fp_prime_set_dense(p);
				break;
#elif FP_PRIME == 384
			case NIST_384:
				/* p = 2^384 - 2^128 - 2^96 + 2^32 - 1. */
				f[0] = -1;
				f[1] = 32;
				f[2] = -96;
				f[3] = -128;
				fp_prime_set_spars(f, 4);
				break;
#elif FP_PRIME == 521
			case NIST_521:
				/* p = 2^521 - 1. */
				f[0] = -1;
				fp_prime_set_spars(f, 1);
				break;
#elif FP_PRIME == 446
			case BN_446:
				fp_param_get_bn(t0);
                                /* p = 36 * x^4 + 36 * x^3 + 24 * x^2 + 6 * x + 1. */
                                bn_set_dig(p, 1);
                                bn_mul_dig(t1, t0, 6);
                                bn_add(p, p, t1);
                                bn_mul(t1, t0, t0);
                                bn_mul_dig(t1, t1, 24);
                                bn_add(p, p, t1);
                                bn_mul(t1, t0, t0);
                                bn_mul(t1, t1, t0);
                                bn_mul_dig(t1, t1, 36);
                                bn_add(p, p, t1);
                                bn_mul(t0, t0, t0);
                                bn_mul(t1, t0, t0);
                                bn_mul_dig(t1, t1, 36);
                                bn_add(p, p, t1);
                                fp_prime_set_dense(p);
                                break;
#elif FP_PRIME == 286
			case BN_286:
				fp_param_get_bn(t0);
                                /* p = 36 * x^4 + 36 * x^3 + 24 * x^2 + 6 * x + 1. */
                                bn_set_dig(p, 1);
                                bn_mul_dig(t1, t0, 6);
                                bn_add(p, p, t1);
                                bn_mul(t1, t0, t0);
                                bn_mul_dig(t1, t1, 24);
                                bn_add(p, p, t1);
                                bn_mul(t1, t0, t0);
                                bn_mul(t1, t1, t0);
                                bn_mul_dig(t1, t1, 36);
                                bn_add(p, p, t1);
                                bn_mul(t0, t0, t0);
                                bn_mul(t1, t0, t0);
                                bn_mul_dig(t1, t1, 36);
                                bn_add(p, p, t1);
                                fp_prime_set_dense(p);
                                break;
#elif FP_PRIME == 478
			case BN_478:
				fp_param_get_bn(t0);
                                /* p = 36 * x^4 + 36 * x^3 + 24 * x^2 + 6 * x + 1. */
                                bn_set_dig(p, 1);
                                bn_mul_dig(t1, t0, 6);
                                bn_add(p, p, t1);
                                bn_mul(t1, t0, t0);
                                bn_mul_dig(t1, t1, 24);
                                bn_add(p, p, t1);
                                bn_mul(t1, t0, t0);
                                bn_mul(t1, t1, t0);
                                bn_mul_dig(t1, t1, 36);
                                bn_add(p, p, t1);
                                bn_mul(t0, t0, t0);
                                bn_mul(t1, t0, t0);
                                bn_mul_dig(t1, t1, 36);
                                bn_add(p, p, t1);
                                fp_prime_set_dense(p);
                                break;
#elif FP_PRIME == 606
			case BN_606:
				fp_param_get_bn(t0);
                                /* p = 36 * x^4 + 36 * x^3 + 24 * x^2 + 6 * x + 1. */
                                bn_set_dig(p, 1);
                                bn_mul_dig(t1, t0, 6);
                                bn_add(p, p, t1);
                                bn_mul(t1, t0, t0);
                                bn_mul_dig(t1, t1, 24);
                                bn_add(p, p, t1);
                                bn_mul(t1, t0, t0);
                                bn_mul(t1, t1, t0);
                                bn_mul_dig(t1, t1, 36);
                                bn_add(p, p, t1);
                                bn_mul(t0, t0, t0);
                                bn_mul(t1, t0, t0);
                                bn_mul_dig(t1, t1, 36);
                                bn_add(p, p, t1);
                                fp_prime_set_dense(p);
                                break;
#elif FP_PRIME == 638
			case BN_638:
				fp_param_get_bn(t0);
                                /* p = 36 * x^4 + 36 * x^3 + 24 * x^2 + 6 * x + 1. */
                                bn_set_dig(p, 1);
                                bn_mul_dig(t1, t0, 6);
                                bn_add(p, p, t1);
                                bn_mul(t1, t0, t0);
                                bn_mul_dig(t1, t1, 24);
                                bn_add(p, p, t1);
                                bn_mul(t1, t0, t0);
                                bn_mul(t1, t1, t0);
                                bn_mul_dig(t1, t1, 36);
                                bn_add(p, p, t1);
                                bn_mul(t0, t0, t0);
                                bn_mul(t1, t0, t0);
                                bn_mul_dig(t1, t1, 36);
                                bn_add(p, p, t1);
                                fp_prime_set_dense(p);
                                break;
#endif
			default:
				bn_gen_prime(p, FP_BITS);
				fp_prime_set_dense(p);
				generated = 1;
				break;
		}

		if (!generated) {
			param_id = param;
		}

	        prime->used = FP_DIGS;
                dv_copy(prime->dp, fp_prime_get(), FP_DIGS);
                bn_sqr(PRIMEXPRIME, prime);

	}
	CATCH_ANY {
		THROW(ERR_CAUGHT);
	}
	FINALLY {
		bn_free(t0);
		bn_free(t1);
		bn_free(p);
	}
}

int fp_param_set_any(void) {
#if FP_PRIME == 158
        fp_param_set(BN_158);
#elif FP_PRIME == 160
	fp_param_set(SECG_160);
#elif FP_PRIME == 192
	fp_param_set(NIST_192);
#elif FP_PRIME == 224
	fp_param_set(NIST_224);
#elif FP_PRIME == 256
#ifdef FP_PMERS
	fp_param_set(NIST_256);
#else
	fp_param_set(BNN_256);
#endif
#elif FP_PRIME == 384
	fp_param_set(NIST_384);
#elif FP_PRIME == 446
	fp_param_set(BN_446);
#elif FP_PRIME == 521
	fp_param_set(NIST_521);
#elif FP_PRIME == 286
	fp_param_set(BN_286);
#elif FP_PRIME == 478
	fp_param_set(BN_478);
#elif FP_PRIME == 606
	fp_param_set(BN_606);
#elif FP_PRIME == 638
	fp_param_set(BN_638);
#else
	return fp_param_set_any_dense();
#endif
	return STS_OK;
}

int fp_param_set_any_dense() {
	bn_t modulus;
	int result = STS_OK;

	bn_null(modulus);

	TRY {
		bn_new(modulus);
		bn_gen_prime(modulus, FP_BITS);
		if (!bn_is_prime(modulus)) {
			result = STS_ERR;
		} else {
			fp_prime_set_dense(modulus);
			bn_sqr(PRIMEXPRIME, modulus);
		}
	}
	CATCH_ANY {
		THROW(ERR_CAUGHT);
	}
	FINALLY {
		bn_free(modulus);
	}
	return result;
}

int fp_param_set_any_spars(void) {
#if FP_PRIME == 160
        fp_param_set(SECG_160);
#elif FP_PRIME == 192
	fp_param_set(NIST_192);
#elif FP_PRIME == 224
	fp_param_set(NIST_224);
#elif FP_PRIME == 256
	fp_param_set(NIST_256);
#elif FP_PRIME == 384
	fp_param_set(NIST_384);
#elif FP_PRIME == 521
	fp_param_set(NIST_521);
#else
	return STS_ERR;
#endif
	return STS_OK;
}

int fp_param_set_any_tower() {
#if FP_PRIME == 158
        fp_param_set(BN_158);
#elif FP_PRIME == 256
	fp_param_set(BNN_256);
#elif FP_PRIME == 446
	fp_param_set(BN_446);
#elif FP_PRIME == 286
	fp_param_set(BN_286);
#elif FP_PRIME == 478
	fp_param_set(BN_478);
#elif FP_PRIME == 606
	fp_param_set(BN_606);
#elif FP_PRIME == 638
	fp_param_set(BN_638);
#else
	do {
		fp_param_set_any_dense();
	} while (fp_prime_get_mod5() == 1 || fp_prime_get_mod5() == 4 ||
			fp_prime_get_mod8() == 1);
#endif
	if (fp_prime_get_mod5() == 1 || fp_prime_get_mod5() == 4 ||
			fp_prime_get_mod8() == 1) {
		return STS_ERR;
	}
	return STS_OK;
}

void fp_param_print(void) {
	util_print_banner("Prime modulus:", 0);
	util_print("   ");
	fp_print(fp_prime_get());
}
