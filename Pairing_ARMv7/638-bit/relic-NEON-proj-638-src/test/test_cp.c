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
 * Tests for the binary elliptic curve arithmetic module.
 *
 * @version $Id: test_cp.c 278 2010-02-06 07:53:40Z dfaranha $
 * @ingroup test
 */

#include <stdio.h>
#include<string.h>
#include<math.h>
#include<stdlib.h>
#include<stdint.h>

#include "relic.h"
#include "relic_test.h"

static int rsa(void) {
	int code = STS_ERR;
	rsa_t pub, prv;
	unsigned char in[10];
	unsigned char out[BN_BITS / 8 + 1];
	int in_len, out_len;
	int result;

	rsa_null(pub);
	rsa_null(prv);

	TRY {
		rsa_new(pub);
		rsa_new(prv);

		result = cp_rsa_gen(pub, prv, BN_BITS);

		TEST_BEGIN("rsa encryption/decryption is correct") {
			TEST_ASSERT(result == STS_OK, end);
			in_len = 10;
			out_len = BN_BITS / 8 + 1;
			rand_bytes(in, in_len);
			TEST_ASSERT(cp_rsa_enc(out, &out_len, in, in_len, pub) == STS_OK,
					end);
			TEST_ASSERT(cp_rsa_dec(out, &out_len, out, out_len, prv) == STS_OK,
					end);
			TEST_ASSERT(memcmp(in, out, out_len) == 0, end);
		} TEST_END;

#if CP_RSA == BASIC || !defined(STRIP)
		result = cp_rsa_gen_basic(pub, prv, BN_BITS);

		TEST_BEGIN("basic rsa encryption/decryption is correct") {
			TEST_ASSERT(result == STS_OK, end);
			in_len = 10;
			out_len = BN_BITS / 8 + 1;
			rand_bytes(in, in_len);
			TEST_ASSERT(cp_rsa_enc(out, &out_len, in, in_len, pub) == STS_OK,
					end);
			TEST_ASSERT(cp_rsa_dec_basic(out, &out_len, out, out_len,
							prv) == STS_OK, end);
			TEST_ASSERT(memcmp(in, out, out_len) == 0, end);
		} TEST_END;
#endif

#if CP_RSA == QUICK || !defined(STRIP)
		result = cp_rsa_gen_quick(pub, prv, BN_BITS);

		TEST_BEGIN("fast rsa encryption/decryption is correct") {
			TEST_ASSERT(result == STS_OK, end);
			in_len = 10;
			out_len = BN_BITS / 8 + 1;
			rand_bytes(in, in_len);
			TEST_ASSERT(cp_rsa_enc(out, &out_len, in, in_len, pub) == STS_OK,
					end);
			TEST_ASSERT(cp_rsa_dec_quick(out, &out_len, out, out_len,
							prv) == STS_OK, end);
			TEST_ASSERT(memcmp(in, out, out_len) == 0, end);
		} TEST_END;
#endif

		result = cp_rsa_gen(pub, prv, BN_BITS);

		TEST_BEGIN("rsa signature/verification is correct") {
			TEST_ASSERT(result == STS_OK, end);
			in_len = 10;
			out_len = BN_BITS / 8 + 1;
			rand_bytes(in, in_len);
			TEST_ASSERT(cp_rsa_sign(out, &out_len, in, in_len, prv) == STS_OK,
					end);
			TEST_ASSERT(cp_rsa_ver(out, out_len, in, in_len, pub) == 1, end);
		} TEST_END;

#if CP_RSA == BASIC || !defined(STRIP)
		result = cp_rsa_gen_basic(pub, prv, BN_BITS);

		TEST_BEGIN("basic rsa signature/verification is correct") {
			TEST_ASSERT(result == STS_OK, end);
			in_len = 10;
			out_len = BN_BITS / 8 + 1;
			rand_bytes(in, in_len);
			TEST_ASSERT(cp_rsa_sign_basic(out, &out_len, in, in_len,
							prv) == STS_OK, end);
			TEST_ASSERT(cp_rsa_ver(out, out_len, in, in_len, pub) == 1, end);
		} TEST_END;
#endif

#if CP_RSA == QUICK || !defined(STRIP)
		result = cp_rsa_gen_quick(pub, prv, BN_BITS);

		TEST_BEGIN("fast rsa signature/verification is correct") {
			TEST_ASSERT(result == STS_OK, end);
			in_len = 10;
			out_len = BN_BITS / 8 + 1;
			rand_bytes(in, in_len);
			TEST_ASSERT(cp_rsa_sign_quick(out, &out_len, in, in_len,
							prv) == STS_OK, end);
			TEST_ASSERT(cp_rsa_ver(out, out_len, in, in_len, pub) == 1, end);
		} TEST_END;
#endif

	} CATCH_ANY {
		ERROR(end);
	}
	code = STS_OK;

  end:
	rsa_free(pub);
	rsa_free(prv);
	return code;
}

static int rabin(void) {
	int code = STS_ERR;
	rabin_t pub, prv;
	unsigned char in[10];
	unsigned char out[BN_BITS / 8 + 1];
	int in_len, out_len;
	int result;

	rabin_null(pub);
	rabin_null(prv);

	TRY {
		rabin_new(pub);
		rabin_new(prv);

		result = cp_rabin_gen(pub, prv, BN_BITS);

		TEST_BEGIN("rabin encryption/decryption is correct") {
			TEST_ASSERT(result == STS_OK, end);
			in_len = 10;
			out_len = BN_BITS / 8 + 1;
			rand_bytes(in, in_len);
			TEST_ASSERT(cp_rabin_enc(out, &out_len, in, in_len, pub) == STS_OK,
					end);
			TEST_ASSERT(cp_rabin_dec(out, &out_len, out, out_len,
							prv) == STS_OK, end);
			TEST_ASSERT(memcmp(in, out, out_len) == 0, end);
		} TEST_END;
	} CATCH_ANY {
		ERROR(end);
	}
	code = STS_OK;

  end:
	rabin_free(pub);
	rabin_free(prv);
	return code;
}

#if defined(WITH_EC)

static int ecdsa(void) {
	int code = STS_ERR;
	bn_t d, r;
	ec_t q;
	unsigned char msg[5] = { 0, 1, 2, 3, 4 };

	bn_null(d);
	bn_null(r);
	ec_null(q);

	TRY {
		bn_new(d);
		bn_new(r);
		ec_new(q);

		TEST_BEGIN("ecdsa is correct") {
			cp_ecdsa_gen(d, q);
			cp_ecdsa_sign(r, d, msg, 5, d);
			TEST_ASSERT(cp_ecdsa_ver(r, d, msg, 5, q) == 1, end);
		} TEST_END;
	} CATCH_ANY {
		ERROR(end);
	}
	code = STS_OK;

  end:
	bn_free(d);
	bn_free(r);
	ec_free(q);
	return code;
}

#endif

#if defined(WITH_PC)

static int sokaka(void) {
	int code = STS_ERR;
	sokaka_t s_a, s_b;
	bn_t s;
	unsigned char key1[MD_LEN], key2[MD_LEN];

	sokaka_null(s_a);
	sokaka_null(s_b);

	TRY {
		sokaka_new(s_a);
		sokaka_new(s_b);
		bn_new(s);

		cp_sokaka_gen(s);

		TEST_BEGIN
				("sakai-ohgishi-kasahara authenticated key agreement is correct")
		{
			cp_sokaka_gen_prv(s_a, "Alice", strlen("Alice"), s);
			cp_sokaka_gen_prv(s_b, "Bob", strlen("Bob"), s);
			cp_sokaka_key(key1, MD_LEN, "Alice", 5, s_a, "Bob", 3);
			cp_sokaka_key(key2, MD_LEN, "Bob", 3, s_b, "Alice", 5);
			TEST_ASSERT(memcmp(key1, key2, MD_LEN) == 0, end);
		} TEST_END;

	} CATCH_ANY {
		ERROR(end);
	}
	code = STS_OK;

  end:
	sokaka_free(s_a);
	sokaka_free(s_b);
	return code;
}

#endif

int main(void) {
	core_init();

	util_print_banner("Tests for the CP module", 0);

#if defined(WITH_BN)
	util_print_banner("Protocols based on prime factorization:\n", 0);

	if (rsa() != STS_OK) {
		core_clean();
		return 1;
	}

	if (rabin() != STS_OK) {
		core_clean();
		return 1;
	}
#endif

#if defined(WITH_EC)
	util_print_banner("Protocols based on elliptic curves:\n", 0);
	if (ec_param_set_any() == STS_OK) {
		if (ecdsa() != STS_OK) {
			core_clean();
			return 1;
		}
	} else {
		THROW(ERR_NO_CURVE);
	}
#endif

#if defined(WITH_PC)
	util_print_banner("Protocols based on pairings:\n", 0);
	if (pc_param_set_any() == STS_OK) {
		if (sokaka() != STS_OK) {
			core_clean();
			return 1;
		}
	} else {
		THROW(ERR_NO_CURVE);
	}
#endif

	core_clean();
	return 0;
}
