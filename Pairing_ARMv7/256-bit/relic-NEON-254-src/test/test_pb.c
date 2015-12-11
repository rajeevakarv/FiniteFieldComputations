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
 * Tests for the pairings over binary elliptic curves module.
 *
 * @version $Id: test_pb.c 274 2010-02-06 05:34:38Z dfaranha $
 * @ingroup test
 */

#include <stdio.h>

#include "relic.h"
#include "relic_test.h"

static int memory2(void) {
	err_t e;
	int code = STS_ERR;
	fb2_t a;

	fb2_null(a);

	TRY {
		TEST_BEGIN("memory can be allocated") {
			fb2_new(a);
			fb2_free(a);
		} TEST_END;
	} CATCH(e) {
		switch (e) {
			case ERR_NO_MEMORY:
				util_print("FATAL ERROR!\n");
				ERROR(end);
				break;
		}
	}
	(void)a;
	code = STS_OK;
  end:
	return code;
}

static int util2(void) {
	int code = STS_ERR;
	fb2_t a, b, c;

	fb2_null(a);
	fb2_null(b);
	fb2_null(c);

	TRY {
		fb2_new(a);
		fb2_new(b);
		fb2_new(c);

		TEST_BEGIN("comparison is consistent") {
			fb2_rand(a);
			fb2_rand(b);
			if (fb2_cmp(a, b) != CMP_EQ) {
				TEST_ASSERT(fb2_cmp(b, a) == CMP_NE, end);
			}
		}
		TEST_END;

		TEST_BEGIN("copy and comparison are consistent") {
			fb2_rand(a);
			fb2_rand(b);
			fb2_rand(c);
			if (fb2_cmp(a, c) != CMP_EQ) {
				fb2_copy(c, a);
				TEST_ASSERT(fb2_cmp(c, a) == CMP_EQ, end);
			}
			if (fb2_cmp(b, c) != CMP_EQ) {
				fb2_copy(c, b);
				TEST_ASSERT(fb2_cmp(b, c) == CMP_EQ, end);
			}
		}
		TEST_END;

		TEST_BEGIN("negation is consistent") {
			fb2_rand(a);
			fb2_neg(b, a);
			if (fb2_cmp(a, b) != CMP_EQ) {
				TEST_ASSERT(fb2_cmp(b, a) == CMP_NE, end);
			}
			fb2_neg(b, b);
			TEST_ASSERT(fb2_cmp(a, b) == CMP_EQ, end);
		}
		TEST_END;

		TEST_BEGIN("assignment to zero and comparison are consistent") {
			fb2_rand(a);
			fb2_zero(c);
			TEST_ASSERT(fb2_cmp(a, c) == CMP_NE, end);
			TEST_ASSERT(fb2_cmp(c, a) == CMP_NE, end);
		}
		TEST_END;

		TEST_BEGIN("assignment to random and comparison are consistent") {
			fb2_rand(a);
			fb2_zero(c);
			TEST_ASSERT(fb2_cmp(a, c) == CMP_NE, end);
		}
		TEST_END;

		TEST_BEGIN("assignment to zero and zero test are consistent") {
			fb2_zero(a);
			TEST_ASSERT(fb2_is_zero(a), end);
		}
		TEST_END;

	}
	CATCH_ANY {
		ERROR(end);
	}
	code = STS_OK;
  end:
	fb2_free(a);
	fb2_free(b);
	fb2_free(c);
	return code;
}

static int addition2(void) {
	int code = STS_ERR;
	fb2_t a, b, c, d, e;

	fb2_null(a);
	fb2_null(b);
	fb2_null(c);
	fb2_null(d);
	fb2_null(e);

	TRY {
		fb2_new(a);
		fb2_new(b);
		fb2_new(c);
		fb2_new(d);
		fb2_new(e);

		TEST_BEGIN("addition is commutative") {
			fb2_rand(a);
			fb2_rand(b);
			fb2_add(d, a, b);
			fb2_add(e, b, a);
			TEST_ASSERT(fb2_cmp(d, e) == CMP_EQ, end);
		} TEST_END;

		TEST_BEGIN("addition is associative") {
			fb2_rand(a);
			fb2_rand(b);
			fb2_rand(c);
			fb2_add(d, a, b);
			fb2_add(d, d, c);
			fb2_add(e, b, c);
			fb2_add(e, a, e);
			TEST_ASSERT(fb2_cmp(d, e) == CMP_EQ, end);
		} TEST_END;

		TEST_BEGIN("addition has identity") {
			fb2_rand(a);
			fb2_zero(d);
			fb2_add(e, a, d);
			TEST_ASSERT(fb2_cmp(e, a) == CMP_EQ, end);
		} TEST_END;

		TEST_BEGIN("addition has inverse") {
			fb2_rand(a);
			fb2_neg(d, a);
			fb2_add(e, a, d);
			TEST_ASSERT(fb2_is_zero(e), end);
		} TEST_END;
	}
	CATCH_ANY {
		util_print("FATAL ERROR!\n");
		ERROR(end);
	}
	code = STS_OK;
  end:
	fb2_free(a);
	fb2_free(b);
	fb2_free(c);
	fb2_free(d);
	fb2_free(e);
	return code;
}

static int subtraction2(void) {
	int code = STS_ERR;
	fb2_t a, b, c, d;

	fb2_null(a);
	fb2_null(b);
	fb2_null(c);
	fb2_null(d);

	TRY {
		fb2_new(a);
		fb2_new(b);
		fb2_new(c);
		fb2_new(d);

		TEST_BEGIN("subtraction is anti-commutative") {
			fb2_rand(a);
			fb2_rand(b);
			fb2_sub(c, a, b);
			fb2_sub(d, b, a);
			fb2_neg(d, d);
			TEST_ASSERT(fb2_cmp(c, d) == CMP_EQ, end);
		}
		TEST_END;

		TEST_BEGIN("subtraction has identity") {
			fb2_rand(a);
			fb2_zero(c);
			fb2_sub(d, a, c);
			TEST_ASSERT(fb2_cmp(d, a) == CMP_EQ, end);
		}
		TEST_END;

		TEST_BEGIN("subtraction has inverse") {
			fb2_rand(a);
			fb2_sub(c, a, a);
			TEST_ASSERT(fb2_is_zero(c), end);
		}
		TEST_END;
	}
	CATCH_ANY {
		util_print("FATAL ERROR!\n");
		ERROR(end);
	}
	code = STS_OK;
  end:
	fb2_free(a);
	fb2_free(b);
	fb2_free(c);
	fb2_free(d);
	return code;
}

static int multiplication2(void) {
	int code = STS_ERR;
	fb2_t a, b, c, d, e, f;

	fb2_null(a);
	fb2_null(b);
	fb2_null(c);
	fb2_null(d);
	fb2_null(e);
	fb2_null(f);

	TRY {
		fb2_new(a);
		fb2_new(b);
		fb2_new(c);
		fb2_new(d);
		fb2_new(e);
		fb2_new(f);

		TEST_BEGIN("multiplication is commutative") {
			fb2_rand(a);
			fb2_rand(b);
			fb2_mul(d, a, b);
			fb2_mul(e, b, a);
			TEST_ASSERT(fb2_cmp(d, e) == CMP_EQ, end);
		} TEST_END;

		TEST_BEGIN("multiplication is associative") {
			fb2_rand(a);
			fb2_rand(b);
			fb2_rand(c);
			fb2_mul(d, a, b);
			fb2_mul(d, d, c);
			fb2_mul(e, b, c);
			fb2_mul(e, a, e);
			TEST_ASSERT(fb2_cmp(d, e) == CMP_EQ, end);
		} TEST_END;

		TEST_BEGIN("multiplication is distributive") {
			fb2_rand(a);
			fb2_rand(b);
			fb2_rand(c);
			fb2_add(d, a, b);
			fb2_mul(d, c, d);
			fb2_mul(e, c, a);
			fb2_mul(f, c, b);
			fb2_add(e, e, f);
			TEST_ASSERT(fb2_cmp(d, e) == CMP_EQ, end);
		} TEST_END;

		TEST_BEGIN("multiplication has identity") {
			fb2_zero(d);
			fb_set_bit(d[0], 0, 1);
			fb2_mul(e, a, d);
			TEST_ASSERT(fb2_cmp(e, a) == CMP_EQ, end);
		} TEST_END;

		TEST_BEGIN("multiplication has zero property") {
			fb2_zero(d);
			fb2_mul(e, a, d);
			TEST_ASSERT(fb2_is_zero(e), end);
		} TEST_END;
	}
	CATCH_ANY {
		util_print("FATAL ERROR!\n");
		ERROR(end);
	}
	code = STS_OK;
  end:
	fb2_free(a);
	fb2_free(b);
	fb2_free(c);
	fb2_free(d);
	fb2_free(e);
	fb2_free(f);
	return code;
}

static int squaring2(void) {
	int code = STS_ERR;
	fb2_t a, b, c;

	fb2_null(a);
	fb2_null(b);
	fb2_null(c);

	TRY {
		fb2_new(a);
		fb2_new(b);
		fb2_new(c);

		TEST_BEGIN("squaring is correct") {
			fb2_rand(a);
			fb2_mul(b, a, a);
			fb2_sqr(c, a);
			TEST_ASSERT(fb2_cmp(b, c) == CMP_EQ, end);
		} TEST_END;
	}
	CATCH_ANY {
		util_print("FATAL ERROR!\n");
		ERROR(end);
	}
	code = STS_OK;
  end:
	fb2_free(a);
	fb2_free(b);
	fb2_free(c);
	return code;
}

static int inversion2(void) {
	int code = STS_ERR;
	fb2_t a, b, c;

	fb2_null(a);
	fb2_null(b);
	fb2_null(c);

	TRY {
		fb2_new(a);
		fb2_new(b);
		fb2_new(c);

		TEST_BEGIN("inversion is correct") {
			fb2_rand(a);
			fb2_inv(b, a);
			fb2_mul(c, a, b);
			TEST_ASSERT(fb_cmp_dig(c[0], 1) == CMP_EQ, end);
		} TEST_END;
	}
	CATCH_ANY {
		util_print("FATAL ERROR!\n");
		ERROR(end);
	}
	code = STS_OK;
  end:
	fb2_free(a);
	fb2_free(b);
	fb2_free(c);
	return code;
}

static int memory4(void) {
	err_t e;
	int code = STS_ERR;
	fb4_t a;

	fb4_null(a);

	TRY {
		TEST_BEGIN("memory can be allocated") {
			fb4_new(a);
			fb4_free(a);
		} TEST_END;
	} CATCH(e) {
		switch (e) {
			case ERR_NO_MEMORY:
				util_print("FATAL ERROR!\n");
				ERROR(end);
				break;
		}
	}
	(void)a;
	code = STS_OK;
  end:
	return code;
}

static int util4(void) {
	int code = STS_ERR;
	fb4_t a, b, c;

	fb4_null(a);
	fb4_null(b);
	fb4_null(c);

	TRY {
		fb4_new(a);
		fb4_new(b);
		fb4_new(c);

		TEST_BEGIN("comparison is consistent") {
			fb4_rand(a);
			fb4_rand(b);
			if (fb4_cmp(a, b) != CMP_EQ) {
				TEST_ASSERT(fb4_cmp(b, a) == CMP_NE, end);
			}
		}
		TEST_END;

		TEST_BEGIN("copy and comparison are consistent") {
			fb4_rand(a);
			fb4_rand(b);
			fb4_rand(c);
			if (fb4_cmp(a, c) != CMP_EQ) {
				fb4_copy(c, a);
				TEST_ASSERT(fb4_cmp(c, a) == CMP_EQ, end);
			}
			if (fb4_cmp(b, c) != CMP_EQ) {
				fb4_copy(c, b);
				TEST_ASSERT(fb4_cmp(b, c) == CMP_EQ, end);
			}
		}
		TEST_END;

		TEST_BEGIN("negation is consistent") {
			fb4_rand(a);
			fb4_neg(b, a);
			if (fb4_cmp(a, b) != CMP_EQ) {
				TEST_ASSERT(fb4_cmp(b, a) == CMP_NE, end);
			}
			fb4_neg(b, b);
			TEST_ASSERT(fb4_cmp(a, b) == CMP_EQ, end);
		}
		TEST_END;

		TEST_BEGIN("assignment to zero and comparison are consistent") {
			fb4_rand(a);
			fb4_zero(c);
			TEST_ASSERT(fb4_cmp(a, c) == CMP_NE, end);
			TEST_ASSERT(fb4_cmp(c, a) == CMP_NE, end);
		}
		TEST_END;

		TEST_BEGIN("assignment to random and comparison are consistent") {
			fb4_rand(a);
			fb4_zero(c);
			TEST_ASSERT(fb4_cmp(a, c) == CMP_NE, end);
		}
		TEST_END;

		TEST_BEGIN("assignment to zero and zero test are consistent") {
			fb4_zero(a);
			TEST_ASSERT(fb4_is_zero(a), end);
		}
		TEST_END;

	}
	CATCH_ANY {
		ERROR(end);
	}
	code = STS_OK;
  end:
	fb4_free(a);
	fb4_free(b);
	fb4_free(c);
	return code;
}

static int addition4(void) {
	int code = STS_ERR;
	fb4_t a, b, c, d, e;

	fb4_null(a);
	fb4_null(b);
	fb4_null(c);
	fb4_null(d);
	fb4_null(e);

	TRY {
		fb4_new(a);
		fb4_new(b);
		fb4_new(c);
		fb4_new(d);
		fb4_new(e);

		TEST_BEGIN("addition is commutative") {
			fb4_rand(a);
			fb4_rand(b);
			fb4_add(d, a, b);
			fb4_add(e, b, a);
			TEST_ASSERT(fb4_cmp(d, e) == CMP_EQ, end);
		} TEST_END;

		TEST_BEGIN("addition is associative") {
			fb4_rand(a);
			fb4_rand(b);
			fb4_rand(c);
			fb4_add(d, a, b);
			fb4_add(d, d, c);
			fb4_add(e, b, c);
			fb4_add(e, a, e);
			TEST_ASSERT(fb4_cmp(d, e) == CMP_EQ, end);
		} TEST_END;

		TEST_BEGIN("addition has identity") {
			fb4_rand(a);
			fb4_zero(d);
			fb4_add(e, a, d);
			TEST_ASSERT(fb4_cmp(e, a) == CMP_EQ, end);
		} TEST_END;

		TEST_BEGIN("addition has inverse") {
			fb4_rand(a);
			fb4_neg(d, a);
			fb4_add(e, a, d);
			TEST_ASSERT(fb4_is_zero(e), end);
		} TEST_END;
	}
	CATCH_ANY {
		util_print("FATAL ERROR!\n");
		ERROR(end);
	}
	code = STS_OK;
  end:
	fb4_free(a);
	fb4_free(b);
	fb4_free(c);
	fb4_free(d);
	fb4_free(e);
	return code;
}

static int subtraction4(void) {
	int code = STS_ERR;
	fb4_t a, b, c, d;

	fb4_null(a);
	fb4_null(b);
	fb4_null(c);
	fb4_null(d);

	TRY {
		fb4_new(a);
		fb4_new(b);
		fb4_new(c);
		fb4_new(d);

		TEST_BEGIN("subtraction is anti-commutative") {
			fb4_rand(a);
			fb4_rand(b);
			fb4_sub(c, a, b);
			fb4_sub(d, b, a);
			fb4_neg(d, d);
			TEST_ASSERT(fb4_cmp(c, d) == CMP_EQ, end);
		}
		TEST_END;

		TEST_BEGIN("subtraction has identity") {
			fb4_rand(a);
			fb4_zero(c);
			fb4_sub(d, a, c);
			TEST_ASSERT(fb4_cmp(d, a) == CMP_EQ, end);
		}
		TEST_END;

		TEST_BEGIN("subtraction has inverse") {
			fb4_rand(a);
			fb4_sub(c, a, a);
			TEST_ASSERT(fb4_is_zero(c), end);
		}
		TEST_END;
	}
	CATCH_ANY {
		util_print("FATAL ERROR!\n");
		ERROR(end);
	}
	code = STS_OK;
  end:
	fb4_free(a);
	fb4_free(b);
	fb4_free(c);
	fb4_free(d);
	return code;
}

static int multiplication4(void) {
	int code = STS_ERR;
	fb4_t a, b, c, d, e, f;
	fb_t beta;

	fb4_null(a);
	fb4_null(b);
	fb4_null(c);
	fb4_null(d);
	fb4_null(e);
	fb4_null(f);

	TRY {
		fb4_new(a);
		fb4_new(b);
		fb4_new(c);
		fb4_new(d);
		fb4_new(e);
		fb4_new(f);
		fb_new(beta);

		TEST_BEGIN("multiplication is commutative") {
			fb4_rand(a);
			fb4_rand(b);
			fb4_mul(d, a, b);
			fb4_mul(e, b, a);
			TEST_ASSERT(fb4_cmp(d, e) == CMP_EQ, end);
		} TEST_END;

		TEST_BEGIN("multiplication is associative") {
			fb4_rand(a);
			fb4_rand(b);
			fb4_rand(c);
			fb4_mul(d, a, b);
			fb4_mul(d, d, c);
			fb4_mul(e, b, c);
			fb4_mul(e, a, e);
			TEST_ASSERT(fb4_cmp(d, e) == CMP_EQ, end);
		} TEST_END;

		TEST_BEGIN("multiplication is distributive") {
			fb4_rand(a);
			fb4_rand(b);
			fb4_rand(c);
			fb4_add(d, a, b);
			fb4_mul(d, c, d);
			fb4_mul(e, c, a);
			fb4_mul(f, c, b);
			fb4_add(e, e, f);
			TEST_ASSERT(fb4_cmp(d, e) == CMP_EQ, end);
		} TEST_END;

		TEST_BEGIN("dense x dense multiplication is correct") {
			fb_zero(beta);
			fb_set_bit(beta, 3, 1);
			fb_set_bit(beta, 5, 1);
			fb_set_bit(beta, 6, 1);
			fb_set_bit(beta, 8, 1);
			fb4_rand(a);
			fb4_rand(b);
			fb4_mul(c, a, b);
			fb_mul(c[0], c[0], beta);
			fb_mul(c[1], c[1], beta);
			fb_mul(c[2], c[2], beta);
			fb_mul(c[3], c[3], beta);
			fb_mul(c[0], c[0], beta);
			fb_mul(c[1], c[1], beta);
			fb_mul(c[2], c[2], beta);
			fb_mul(c[3], c[3], beta);
			fb4_mul_dxd(d, a, b);
			TEST_ASSERT(fb4_cmp(c, d) == CMP_EQ, end);
		} TEST_END;

		TEST_BEGIN("dense x sparse multiplication is correct") {
			fb4_rand(a);
			fb4_rand(b);
			fb_zero(b[2]);
			fb_set_bit(b[2], 0, 1);
			fb_zero(b[3]);
			fb4_mul(c, a, b);
			fb4_mul_dxs(d, a, b);
			TEST_ASSERT(fb4_cmp(c, d) == CMP_EQ, end);
		} TEST_END;

		TEST_BEGIN("sparse x sparse multiplication is correct") {
			fb4_rand(a);
			fb_zero(a[2]);
			fb_set_bit(a[2], 0, 1);
			fb_zero(a[3]);
			fb4_rand(b);
			fb_zero(b[2]);
			fb_set_bit(b[2], 0, 1);
			fb_zero(b[3]);
			fb4_mul(c, a, b);
			fb4_mul_sxs(d, a, b);
			TEST_ASSERT(fb4_cmp(c, d) == CMP_EQ, end);
		} TEST_END;

		TEST_BEGIN("multiplication has identity") {
			fb4_zero(d);
			fb_set_bit(d[0], 0, 1);
			fb4_mul(e, a, d);
			TEST_ASSERT(fb4_cmp(e, a) == CMP_EQ, end);
		} TEST_END;

		TEST_BEGIN("multiplication has zero property") {
			fb4_zero(d);
			fb4_mul(e, a, d);
			TEST_ASSERT(fb4_is_zero(e), end);
		} TEST_END;
	}
	CATCH_ANY {
		util_print("FATAL ERROR!\n");
		ERROR(end);
	}
	code = STS_OK;
  end:
	fb4_free(a);
	fb4_free(b);
	fb4_free(c);
	fb4_free(d);
	fb4_free(e);
	fb4_free(f);
	fb_free(beta);
	return code;
}

static int squaring4(void) {
	int code = STS_ERR;
	fb4_t a, b, c;

	fb4_null(a);
	fb4_null(b);
	fb4_null(c);

	TRY {
		fb4_new(a);
		fb4_new(b);
		fb4_new(c);

		TEST_BEGIN("squaring is correct") {
			fb4_rand(a);
			fb4_mul(b, a, a);
			fb4_sqr(c, a);
			TEST_ASSERT(fb4_cmp(b, c) == CMP_EQ, end);
		} TEST_END;
	}
	CATCH_ANY {
		util_print("FATAL ERROR!\n");
		ERROR(end);
	}
	code = STS_OK;
  end:
	fb4_free(a);
	fb4_free(b);
	fb4_free(c);
	return code;
}

static int exponentiation4(void) {
	int code = STS_ERR;
	fb4_t a, b, c;

	fb4_null(a);
	fb4_null(b);
	fb4_null(c);

	TRY {
		fb4_new(a);
		fb4_new(b);
		fb4_new(c);

		TEST_BEGIN("exponentiation to 2^m is correct") {
			fb4_rand(a);
			fb4_exp_2m(b, a);
			fb4_copy(c, a);
			for (int j = 0; j < FB_BITS; j++) {
				fb4_sqr(c, c);
			}
			TEST_ASSERT(fb4_cmp(b, c) == CMP_EQ, end);
		} TEST_END;
	}
	CATCH_ANY {
		util_print("FATAL ERROR!\n");
		ERROR(end);
	}
	code = STS_OK;
  end:
	fb4_free(a);
	fb4_free(b);
	fb4_free(c);
	return code;
}

static int pairing(void) {
	int code = STS_ERR;
	fb4_t e1, e2;
	eb_t p, q, r;
	bn_t k, n;

	fb4_null(e1);
	fb4_null(e2);
	eb_null(p);
	eb_null(q);
	eb_null(r);
	bn_null(k);
	bn_null(n);

	TRY {
		fb4_new(e1);
		fb4_new(e2);
		eb_new(p);
		eb_new(q);
		eb_new(r);
		bn_new(k);
		bn_new(n);

		eb_curve_get_ord(n);

		TEST_BEGIN("etat pairing is bilinear") {
			eb_rand(p);
			eb_rand(q);
			bn_rand(k, BN_POS, bn_bits(n));
			bn_mod(k, k, n);
			eb_mul(r, q, k);
			fb4_zero(e1);
			fb4_zero(e2);
			pb_map(e1, p, r);
			eb_mul(r, p, k);
			pb_map(e2, r, q);
			TEST_ASSERT(fb4_cmp(e1, e2) == CMP_EQ, end);
		} TEST_END;

#if PB_MAP == ETATS || !defined(STRIP)
		TEST_BEGIN("etat pairing with square roots is correct") {
			eb_rand(p);
			eb_rand(q);
			pb_map(e1, p, q);
			pb_map_etats(e2, p, q);
			TEST_ASSERT(fb4_cmp(e1, e2) == CMP_EQ, end);
		} TEST_END;
#endif

#if PB_MAP == ETATN || !defined(STRIP)
		TEST_BEGIN("etat pairing without square roots is correct") {
			eb_rand(p);
			eb_rand(q);
			pb_map(e1, p, q);
			pb_map_etatn(e2, p, q);
			TEST_ASSERT(fb4_cmp(e1, e2) == CMP_EQ, end);
		} TEST_END;
#endif
	}
	CATCH_ANY {
		util_print("FATAL ERROR!\n");
		ERROR(end);
	}
	code = STS_OK;
  end:
	fb4_free(e1);
	fb4_free(e2);
	eb_free(p);
	eb_free(q);
	eb_free(r);
	bn_free(k);
	bn_free(n);
	return code;
}

int main(void) {
	core_init();

	fb_param_set_any();

	util_print_banner("Tests for the PB module", 0);

	util_print_banner("Quadratic extension:", 0);
	util_print_banner("Utilities:", 1);

	if (memory2() != STS_OK) {
		core_clean();
		return 1;
	}

	if (util2() != STS_OK) {
		core_clean();
		return 1;
	}

	util_print_banner("Arithmetic:", 1);

	if (addition2() != STS_OK) {
		core_clean();
		return 1;
	}

	if (subtraction2() != STS_OK) {
		core_clean();
		return 1;
	}

	if (multiplication2() != STS_OK) {
		core_clean();
		return 1;
	}

	if (squaring2() != STS_OK) {
		core_clean();
		return 1;
	}

	if (inversion2() != STS_OK) {
		core_clean();
		return 1;
	}

	util_print_banner("Quartic extension:", 0);
	util_print_banner("Utilities", 1)

			if (memory4() != STS_OK) {
		core_clean();
		return 1;
	}

	if (util4() != STS_OK) {
		core_clean();
		return 1;
	}

	util_print_banner("Arithmetic:", 1);

	if (addition4() != STS_OK) {
		core_clean();
		return 1;
	}

	if (subtraction4() != STS_OK) {
		core_clean();
		return 1;
	}

	if (multiplication4() != STS_OK) {
		core_clean();
		return 1;
	}

	if (squaring4() != STS_OK) {
		core_clean();
		return 1;
	}

	if (exponentiation4() != STS_OK) {
		core_clean();
		return 1;
	}

	if (eb_param_set_any_super() == STS_ERR) {
		THROW(ERR_NO_CURVE);
		core_clean();
		return 0;
	}

	util_print_banner("Bilinear pairing:\n", 0);

	if (pairing() != STS_OK) {
		core_clean();
		return 1;
	}

	core_clean();

	return 0;
}
