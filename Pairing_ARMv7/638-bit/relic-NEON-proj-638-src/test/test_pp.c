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
 * MODIFIED BY:  Patrick Longa, 06/2011.
 * 
 */

/**
 * @file
 *
 * Tests for the quartic extension binary field arithmetic module.
 *
 * @version $Id: test_pp.c 274 2010-02-06 05:34:38Z dfaranha $
 * @ingroup test
 */

#include <stdio.h>

#include "relic.h"
#include "relic_test.h"
#include "relic_bench.h"


static int memory2(void) {
	err_t e;
	int code = STS_ERR;
	fp2_t a;

	TRY {
		TEST_BEGIN("memory can be allocated") {
			fp2_new(a);
			fp2_free(a);
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
	fp2_t a, b, c;

	fp2_null(a);
	fp2_null(b);
	fp2_null(c);

	TRY {
		fp2_new(a);
		fp2_new(b);
		fp2_new(c);

		TEST_BEGIN("comparison is consistent") {
			fp2_rand(a);
			fp2_rand(b);
			if (fp2_cmp(a, b) != CMP_EQ) {
				TEST_ASSERT(fp2_cmp(b, a) == CMP_NE, end);
			}
		}
		TEST_END;

		TEST_BEGIN("copy and comparison are consistent") {
			fp2_rand(a);
			fp2_rand(b);
			fp2_rand(c);
			if (fp2_cmp(a, c) != CMP_EQ) {
				fp2_copy(c, a);
				TEST_ASSERT(fp2_cmp(c, a) == CMP_EQ, end);
			}
			if (fp2_cmp(b, c) != CMP_EQ) {
				fp2_copy(c, b);
				TEST_ASSERT(fp2_cmp(b, c) == CMP_EQ, end);
			}
		}
		TEST_END;

		TEST_BEGIN("negation is consistent") {
			fp2_rand(a);
			fp2_neg(b, a);
			if (fp2_cmp(a, b) != CMP_EQ) {
				TEST_ASSERT(fp2_cmp(b, a) == CMP_NE, end);
			}
			fp2_neg(b, b);
			TEST_ASSERT(fp2_cmp(a, b) == CMP_EQ, end);
		}
		TEST_END;

		TEST_BEGIN("assignment to zero and comparison are consistent") {
			fp2_rand(a);
			fp2_zero(c);
			TEST_ASSERT(fp2_cmp(a, c) == CMP_NE, end);
			TEST_ASSERT(fp2_cmp(c, a) == CMP_NE, end);
		}
		TEST_END;

		TEST_BEGIN("assignment to random and comparison are consistent") {
			fp2_rand(a);
			fp2_zero(c);
			TEST_ASSERT(fp2_cmp(a, c) == CMP_NE, end);
		}
		TEST_END;

		TEST_BEGIN("assignment to zero and zero test are consistent") {
			fp2_zero(a);
			TEST_ASSERT(fp2_is_zero(a), end);
		}
		TEST_END;

	}
	CATCH_ANY {
		ERROR(end);
	}
	code = STS_OK;
  end:
	fp2_free(a);
	fp2_free(b);
	fp2_free(c);
	return code;
}

static int addition2(void) {
	int code = STS_ERR;
	fp2_t a, b, c, d, e;

	TRY {
		fp2_new(a);
		fp2_new(b);
		fp2_new(c);
		fp2_new(d);
		fp2_new(e);

		TEST_BEGIN("addition is commutative") {
			fp2_rand(a);
			fp2_rand(b);
			//fp_set_dig(a[0], 1);
			//fp_set_dig(a[1], 1);
			//fp_set_dig(b[0], 1);
			//fp_set_dig(b[1], 1);
			fp2_add(d, a, b);
			fp2_add(e, b, a);
			//fp2_print(a);
			//fp2_print(b);
			//fp2_print(d);
			TEST_ASSERT(fp2_cmp(d, e) == CMP_EQ, end);
		} TEST_END;

		TEST_BEGIN("addition is associative") {
			fp2_rand(a);
			fp2_rand(b);
			fp2_rand(c);
			fp2_add(d, a, b);
			fp2_add(d, d, c);
			fp2_add(e, b, c);
			fp2_add(e, a, e);
			TEST_ASSERT(fp2_cmp(d, e) == CMP_EQ, end);
		} TEST_END;

		TEST_BEGIN("addition has identity") {
			fp2_rand(a);
			fp2_zero(d);
			fp2_add(e, a, d);
			TEST_ASSERT(fp2_cmp(e, a) == CMP_EQ, end);
		} TEST_END;

		TEST_BEGIN("addition has inverse") {
			fp2_rand(a);
			fp2_neg(d, a);
			fp2_add(e, a, d);
			TEST_ASSERT(fp2_is_zero(e), end);
		} TEST_END;
	}
	CATCH_ANY {
		util_print("FATAL ERROR!\n");
		ERROR(end);
	}
	code = STS_OK;
  end:
	fp2_free(a);
	fp2_free(b);
	fp2_free(c);
	fp2_free(d);
	fp2_free(e);
	return code;
}

static int subtraction2(void) {
	int code = STS_ERR;
	fp2_t a, b, c, d;

	TRY {
		fp2_new(a);
		fp2_new(b);
		fp2_new(c);
		fp2_new(d);

		TEST_BEGIN("subtraction is anti-commutative") {
			fp2_rand(a);
			fp2_rand(b);
			fp2_sub(c, a, b);
			fp2_sub(d, b, a);
			fp2_neg(d, d);
			TEST_ASSERT(fp2_cmp(c, d) == CMP_EQ, end);
		}
		TEST_END;

		TEST_BEGIN("subtraction has identity") {
			fp2_rand(a);
			fp2_zero(c);
			fp2_sub(d, a, c);
			TEST_ASSERT(fp2_cmp(d, a) == CMP_EQ, end);
		}
		TEST_END;

		TEST_BEGIN("subtraction has inverse") {
			fp2_rand(a);
			fp2_sub(c, a, a);
			TEST_ASSERT(fp2_is_zero(c), end);
		}
		TEST_END;
	}
	CATCH_ANY {
		util_print("FATAL ERROR!\n");
		ERROR(end);
	}
	code = STS_OK;
  end:
	fp2_free(a);
	fp2_free(b);
	fp2_free(c);
	fp2_free(d);
	return code;
}

static int doubling2(void) {
	int code = STS_ERR;
	fp2_t a, b, c;

	TRY {
		fp2_new(a);
		fp2_new(b);
		fp2_new(c);

		TEST_BEGIN("doubling is correct") {
			fp2_rand(a);
			fp2_dbl(b, a);
			fp2_add(c, a, a);
			TEST_ASSERT(fp2_cmp(b, c) == CMP_EQ, end);
		} TEST_END;
	}
	CATCH_ANY {
		util_print("FATAL ERROR!\n");
		ERROR(end);
	}
	code = STS_OK;
  end:
	fp2_free(a);
	fp2_free(b);
	fp2_free(c);
	return code;
}

static int division2(void) {
  int code = STS_ERR;
  fp2_t a, b, c;

  TRY {
    fp2_new(a);
    fp2_new(b);
    fp2_new(c);

    TEST_BEGIN("division by 2 is correct") {
      fp2_rand(a);
      fp2_div2(b, a);
      fp2_add(c, b, b);
      TEST_ASSERT(fp2_cmp(a, c) == CMP_EQ, end);
    } TEST_END;
  }
  CATCH_ANY {
    util_print("FATAL ERROR!\n");
    ERROR(end);
  }
  code = STS_OK;
 end:
  fp2_free(a);
  fp2_free(b);
  fp2_free(c);
  return code;
}

static int multiplication2(void) {
	int code = STS_ERR;
	fp2_t a, b, c, d, e, f;

	TRY {
		fp2_new(a);
		fp2_new(b);
		fp2_new(c);
		fp2_new(d);
		fp2_new(e);
		fp2_new(f);

		TEST_BEGIN("multiplication is commutative") {
			fp2_rand(a);
			fp2_rand(b);
			//fp_set_dig(a[0], 1);
			//fp_set_dig(a[1], 1);
			//fp_set_dig(b[0], 1);
			//fp_set_dig(b[1], 1);
			fp2_mul(d, a, b);
			fp2_mul(e, b, a);
			//fp2_print(a);
			//fp2_print(b);
			//fp2_print(d);
			TEST_ASSERT(fp2_cmp(d, e) == CMP_EQ, end);
		} TEST_END;

		TEST_BEGIN("multiplication is associative") {
			fp2_rand(a);
			fp2_rand(b);
			fp2_rand(c);
			fp2_mul(d, a, b);
			fp2_mul(d, d, c);
			fp2_mul(e, b, c);
			fp2_mul(e, a, e);
			TEST_ASSERT(fp2_cmp(d, e) == CMP_EQ, end);
		} TEST_END;

		TEST_BEGIN("multiplication is distributive") {
			fp2_rand(a);
			fp2_rand(b);
			fp2_rand(c);
			fp2_add(d, a, b);
			fp2_mul(d, c, d);
			fp2_mul(e, c, a);
			fp2_mul(f, c, b);
			fp2_add(e, e, f);
			TEST_ASSERT(fp2_cmp(d, e) == CMP_EQ, end);
		} TEST_END;

		TEST_BEGIN("multiplication has identity") {
			fp2_rand(a);
			fp2_zero(d);
			fp_set_dig(d[0], 1);
			fp2_mul(e, a, d);
			TEST_ASSERT(fp2_cmp(e, a) == CMP_EQ, end);
		} TEST_END;

		TEST_BEGIN("multiplication has zero property") {
			fp2_rand(a);
			fp2_zero(d);
			fp2_mul(e, a, d);
			TEST_ASSERT(fp2_is_zero(e), end);
		} TEST_END;

		TEST_BEGIN("multiplication by adjoined root is correct") {
			fp2_rand(a);
			fp2_zero(b);
			fp_set_dig(b[1], 1);
			fp2_mul(c, a, b);
			fp2_mul_art(d, a);
			TEST_ASSERT(fp2_cmp(c, d) == CMP_EQ, end);
		} TEST_END;

/*		TEST_BEGIN("multiplication by quadratic/cubic non-residue is correct") {
			fp2_rand(a);
			//fp_set_dig(a[0],1);
			fp2_mul_nor(b, a);
			//fp2_print(b);
			//printf("\n");
			switch (fp_prime_get_mod8()) {
				case 5:
					fp2_mul_art(c, a);
					break;
				case 3:
					fp2_mul_art(c, a);
					fp2_add(c, c, a);
					break;
				case 7:
					fp2_mul_art(c, a);
					fp2_add(c, c, a);
					//fp2_add(c, c, a);
					//fp2_add(c, c, a);
					//fp2_add(c, c, a);
					//fp2_add(c, c, a);
					//fp2_add(c, c, a);
					//fp2_add(c, c, a);
					//fp2_add(c, c, a);
					//fp2_add(c, c, a);
					//fp2_add(c, c, a);
					//fp2_add(c, c, a);
					//fp2_add(c, c, a);
					break;
			}
			//fp2_print(c);
			//printf("\n");
			TEST_ASSERT(fp2_cmp(b, c) == CMP_EQ, end);
		} TEST_END;*/
	}
	CATCH_ANY {
		util_print("FATAL ERROR!\n");
		ERROR(end);
	}
	code = STS_OK;
  end:
	fp2_free(a);
	fp2_free(b);
	fp2_free(c);
	fp2_free(d);
	fp2_free(e);
	fp2_free(f);
	return code;
}

static int squaring2(void) {
	int code = STS_ERR;
	fp2_t a, b, c;

	TRY {
		fp2_new(a);
		fp2_new(b);
		fp2_new(c);

		TEST_BEGIN("squaring is correct") {
			fp2_rand(a);
			fp2_mul(b, a, a);
			fp2_sqr(c, a);
			TEST_ASSERT(fp2_cmp(b, c) == CMP_EQ, end);
		} TEST_END;
	}
	CATCH_ANY {
		util_print("FATAL ERROR!\n");
		ERROR(end);
	}
	code = STS_OK;
  end:
	fp2_free(a);
	fp2_free(b);
	fp2_free(c);
	return code;
}

static int inversion2(void) {
	int code = STS_ERR;
	fp2_t a, b, c;

	TRY {
		fp2_new(a);
		fp2_new(b);
		fp2_new(c);

		TEST_BEGIN("inversion is correct") {
			fp2_rand(a);
			//fp_set_dig(a[0], 1);
			//fp_set_dig(a[1], 1);
			//fp2_print(a);
			fp2_inv(b, a);
			//fp2_print(b);
			fp2_mul(c, a, b);
			fp2_zero(b);
			fp_set_dig(b[0], 1);
			TEST_ASSERT(fp2_cmp(c, b) == CMP_EQ, end);
		} TEST_END;
	}
	CATCH_ANY {
		util_print("FATAL ERROR!\n");
		ERROR(end);
	}
	code = STS_OK;
  end:
	fp2_free(a);
	fp2_free(b);
	fp2_free(c);
	return code;
}

static int exponentiation2(void) {
	int code = STS_ERR;
	fp2_t a, b, c;
	bn_t d;

	fp2_null(a);
	fp2_null(b);
	fp2_null(c);
	bn_null(d);

	TRY {
		fp2_new(a);
		fp2_new(b);
		fp2_new(c);
		bn_new(d);

		TEST_BEGIN("frobenius and exponentiation are consistent") {
			fp2_rand(a);
			fp2_frb(b, a);
			d->used = FP_DIGS;
			dv_copy(d->dp, fp_prime_get(), FP_DIGS);
			fp2_exp(c, a, d);
			TEST_ASSERT(fp2_cmp(c, b) == CMP_EQ, end);
		} TEST_END;
	}
	CATCH_ANY {
		util_print("FATAL ERROR!\n");
		ERROR(end);
	}
	code = STS_OK;
  end:
	fp2_free(a);
	fp2_free(b);
	fp2_free(c);
	bn_free(d);
	return code;
}

static int memory6(void) {
	err_t e;
	int code = STS_ERR;
	fp6_t a;

	TRY {
		TEST_BEGIN("memory can be allocated") {
			fp6_new(a);
			fp6_free(a);
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

static int util6(void) {
	int code = STS_ERR;
	fp6_t a, b, c;

	fp6_null(a);
	fp6_null(b);
	fp6_null(c);

	TRY {
		fp6_new(a);
		fp6_new(b);
		fp6_new(c);

		TEST_BEGIN("comparison is consistent") {
			fp6_rand(a);
			fp6_rand(b);
			if (fp6_cmp(a, b) != CMP_EQ) {
				TEST_ASSERT(fp6_cmp(b, a) == CMP_NE, end);
			}
		}
		TEST_END;

		TEST_BEGIN("copy and comparison are consistent") {
			fp6_rand(a);
			fp6_rand(b);
			fp6_rand(c);
			if (fp6_cmp(a, c) != CMP_EQ) {
				fp6_copy(c, a);
				TEST_ASSERT(fp6_cmp(c, a) == CMP_EQ, end);
			}
			if (fp6_cmp(b, c) != CMP_EQ) {
				fp6_copy(c, b);
				TEST_ASSERT(fp6_cmp(b, c) == CMP_EQ, end);
			}
		}
		TEST_END;

		TEST_BEGIN("negation is consistent") {
			fp6_rand(a);
			fp6_neg(b, a);
			if (fp6_cmp(a, b) != CMP_EQ) {
				TEST_ASSERT(fp6_cmp(b, a) == CMP_NE, end);
			}
			fp6_neg(b, b);
			TEST_ASSERT(fp6_cmp(a, b) == CMP_EQ, end);
		}
		TEST_END;

		TEST_BEGIN("assignment to zero and comparison are consistent") {
			fp6_rand(a);
			fp6_zero(c);
			TEST_ASSERT(fp6_cmp(a, c) == CMP_NE, end);
			TEST_ASSERT(fp6_cmp(c, a) == CMP_NE, end);
		}
		TEST_END;

		TEST_BEGIN("assignment to random and comparison are consistent") {
			fp6_rand(a);
			fp6_zero(c);
			TEST_ASSERT(fp6_cmp(a, c) == CMP_NE, end);
		}
		TEST_END;

		TEST_BEGIN("assignment to zero and zero test are consistent") {
			fp6_zero(a);
			TEST_ASSERT(fp6_is_zero(a), end);
		}
		TEST_END;

	}
	CATCH_ANY {
		ERROR(end);
	}
	code = STS_OK;
  end:
	fp6_free(a);
	fp6_free(b);
	fp6_free(c);
	return code;
}

static int addition6(void) {
	int code = STS_ERR;
	fp6_t a, b, c, d, e;

	TRY {
		fp6_new(a);
		fp6_new(b);
		fp6_new(c);
		fp6_new(d);
		fp6_new(e);

		TEST_BEGIN("addition is commutative") {
			fp6_rand(a);
			fp6_rand(b);
			fp6_add(d, a, b);
			fp6_add(e, b, a);
			TEST_ASSERT(fp6_cmp(d, e) == CMP_EQ, end);
		} TEST_END;

		TEST_BEGIN("addition is associative") {
			fp6_rand(a);
			fp6_rand(b);
			fp6_rand(c);
			fp6_add(d, a, b);
			fp6_add(d, d, c);
			fp6_add(e, b, c);
			fp6_add(e, a, e);
			TEST_ASSERT(fp6_cmp(d, e) == CMP_EQ, end);
		} TEST_END;

		TEST_BEGIN("addition has identity") {
			fp6_rand(a);
			fp6_zero(d);
			fp6_add(e, a, d);
			TEST_ASSERT(fp6_cmp(e, a) == CMP_EQ, end);
		} TEST_END;

		TEST_BEGIN("addition has inverse") {
			fp6_rand(a);
			fp6_neg(d, a);
			fp6_add(e, a, d);
			TEST_ASSERT(fp6_is_zero(e), end);
		} TEST_END;
	}
	CATCH_ANY {
		util_print("FATAL ERROR!\n");
		ERROR(end);
	}
	code = STS_OK;
  end:
	fp6_free(a);
	fp6_free(b);
	fp6_free(c);
	fp6_free(d);
	fp6_free(e);
	return code;
}

static int subtraction6(void) {
	int code = STS_ERR;
	fp6_t a, b, c, d;

	TRY {
		fp6_new(a);
		fp6_new(b);
		fp6_new(c);
		fp6_new(d);

		TEST_BEGIN("subtraction is anti-commutative") {
			fp6_rand(a);
			fp6_rand(b);
			fp6_sub(c, a, b);
			fp6_sub(d, b, a);
			fp6_neg(d, d);
			TEST_ASSERT(fp6_cmp(c, d) == CMP_EQ, end);
		}
		TEST_END;

		TEST_BEGIN("subtraction has identity") {
			fp6_rand(a);
			fp6_zero(c);
			fp6_sub(d, a, c);
			TEST_ASSERT(fp6_cmp(d, a) == CMP_EQ, end);
		}
		TEST_END;

		TEST_BEGIN("subtraction has inverse") {
			fp6_rand(a);
			fp6_sub(c, a, a);
			TEST_ASSERT(fp6_is_zero(c), end);
		}
		TEST_END;
	}
	CATCH_ANY {
		util_print("FATAL ERROR!\n");
		ERROR(end);
	}
	code = STS_OK;
  end:
	fp6_free(a);
	fp6_free(b);
	fp6_free(c);
	fp6_free(d);
	return code;
}

static int doubling6(void) {
	int code = STS_ERR;
	fp6_t a, b, c;

	TRY {
		fp6_new(a);
		fp6_new(b);
		fp6_new(c);

		TEST_BEGIN("doubling is correct") {
			fp6_rand(a);
			fp6_dbl(b, a);
			fp6_add(c, a, a);
			TEST_ASSERT(fp6_cmp(b, c) == CMP_EQ, end);
		} TEST_END;
	}
	CATCH_ANY {
		util_print("FATAL ERROR!\n");
		ERROR(end);
	}
	code = STS_OK;
  end:
	fp6_free(a);
	fp6_free(b);
	fp6_free(c);
	return code;
}

static int multiplication6(void) {
	int code = STS_ERR;
	fp6_t a, b, c, d, e, f;

	TRY {
		fp6_new(a);
		fp6_new(b);
		fp6_new(c);
		fp6_new(d);
		fp6_new(e);
		fp6_new(f);

		TEST_BEGIN("multiplication is commutative") {
			fp6_rand(a);
			fp6_rand(b);
			/*fp_set_dig(a[0][0], 1);
			fp_set_dig(a[0][1], 1);
			fp_set_dig(a[1][0], 1);
			fp_set_dig(a[1][1], 1);
			fp_set_dig(a[2][0], 1);
			fp_set_dig(a[2][1], 1);
			fp_set_dig(b[0][0], 1);
			fp_set_dig(b[0][1], 1);
			fp_set_dig(b[1][0], 1);
			fp_set_dig(b[1][1], 1);
			fp_set_dig(b[2][0], 1);
			fp_set_dig(b[2][1], 1);*/
			fp6_mul(d, a, b);
			fp6_mul(e, b, a);
			//fp6_print(a);
			//printf("\n");
			//fp6_print(b);
			//printf("\n");
			//fp6_print(d);
			//printf("\n");
			TEST_ASSERT(fp6_cmp(d, e) == CMP_EQ, end);
		} TEST_END;

		TEST_BEGIN("multiplication is associative") {
			fp6_rand(a);
			fp6_rand(b);
			fp6_rand(c);
			fp6_mul(d, a, b);
			fp6_mul(d, d, c);
			fp6_mul(e, b, c);
			fp6_mul(e, a, e);
			TEST_ASSERT(fp6_cmp(d, e) == CMP_EQ, end);
		} TEST_END;

		TEST_BEGIN("multiplication is distributive") {
			fp6_rand(a);
			fp6_rand(b);
			fp6_rand(c);
			fp6_add(d, a, b);
			fp6_mul(d, c, d);
			fp6_mul(e, c, a);
			fp6_mul(f, c, b);
			fp6_add(e, e, f);
			TEST_ASSERT(fp6_cmp(d, e) == CMP_EQ, end);
		} TEST_END;

		TEST_BEGIN("multiplication has identity") {
			fp6_zero(d);
			fp_set_dig(d[0][0], 1);
			fp6_mul(e, a, d);
			TEST_ASSERT(fp6_cmp(e, a) == CMP_EQ, end);
		} TEST_END;

		TEST_BEGIN("multiplication has zero property") {
			fp6_zero(d);
			fp6_mul(e, a, d);
			TEST_ASSERT(fp6_is_zero(e), end);
		} TEST_END;

		TEST_BEGIN("sparse multiplication is correct") {
			fp6_rand(a);
			fp6_rand(b);
			fp2_zero(b[2]);
			fp6_mul(d, a, b);
			fp6_mul_dxs(e, a, b);
			TEST_ASSERT(fp6_cmp(d, e) == CMP_EQ, end);
		} TEST_END;

		TEST_BEGIN("multiplication by quadratic extension element is correct") {
			fp6_rand(a);
			fp6_zero(b);
			fp2_rand(b[0]);
			fp6_mul(d, a, b);
			fp6_mul_dxq(e, a, b[0]);
			TEST_ASSERT(fp6_cmp(d, e) == CMP_EQ, end);
		} TEST_END;

		TEST_BEGIN("multiplication by adjoined root is correct") {
			fp6_rand(a);
			fp6_zero(b);
			fp_set_dig(b[1][0], 1);
			fp6_mul(c, a, b);
			fp6_mul_art(d, a);
			TEST_ASSERT(fp6_cmp(c, d) == CMP_EQ, end);
		} TEST_END;
	}
	CATCH_ANY {
		util_print("FATAL ERROR!\n");
		ERROR(end);
	}
	code = STS_OK;
  end:
	fp6_free(a);
	fp6_free(b);
	fp6_free(c);
	fp6_free(d);
	fp6_free(e);
	fp6_free(f);
	return code;
}

static int squaring6(void) {
	int code = STS_ERR;
	fp6_t a, b, c;

	TRY {
		fp6_new(a);
		fp6_new(b);
		fp6_new(c);

		TEST_BEGIN("squaring is correct") {
			fp6_rand(a);
			fp6_mul(b, a, a);
			fp6_sqr(c, a);
			TEST_ASSERT(fp6_cmp(b, c) == CMP_EQ, end);
		} TEST_END;
	}
	CATCH_ANY {
		util_print("FATAL ERROR!\n");
		ERROR(end);
	}
	code = STS_OK;
  end:
	fp6_free(a);
	fp6_free(b);
	fp6_free(c);
	return code;
}

static int inversion6(void) {
	int code = STS_ERR;
	fp6_t a, b, c;

	TRY {
		fp6_new(a);
		fp6_new(b);
		fp6_new(c);

		TEST_BEGIN("inversion is correct") {
			fp6_rand(a);
			fp6_inv(b, a);
			fp6_mul(c, a, b);
			fp6_zero(b);
			fp_set_dig(b[0][0], 1);
			TEST_ASSERT(fp6_cmp(c, b) == CMP_EQ, end);
		} TEST_END;
	}
	CATCH_ANY {
		util_print("FATAL ERROR!\n");
		ERROR(end);
	}
	code = STS_OK;
  end:
	fp6_free(a);
	fp6_free(b);
	fp6_free(c);
	return code;
}

static int memory12(void) {
	err_t e;
	int code = STS_ERR;
	fp12_t a;

	TRY {
		TEST_BEGIN("memory can be allocated") {
			fp12_new(a);
			fp12_free(a);
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

static int exponentiation6(void) {
	int code = STS_ERR;
	fp2_t a, b, c;
	bn_t d;

	fp2_null(a);
	fp2_null(b);
	fp2_null(c);
	bn_null(d);

	TRY {
		fp2_new(a);
		fp2_new(b);
		fp2_new(c);
		bn_new(d);

		TEST_BEGIN("frobenius and exponentiation are consistent") {
			fp2_rand(a);
			fp2_frb(b, a);
			d->used = FP_DIGS;
			dv_copy(d->dp, fp_prime_get(), FP_DIGS);
			fp2_exp(c, a, d);
			TEST_ASSERT(fp2_cmp(c, b) == CMP_EQ, end);
		} TEST_END;
	}
	CATCH_ANY {
		util_print("FATAL ERROR!\n");
		ERROR(end);
	}
	code = STS_OK;
  end:
	fp2_free(a);
	fp2_free(b);
	fp2_free(c);
	bn_free(d);
	return code;
}

static int util12(void) {
	int code = STS_ERR;
	fp12_t a, b, c;

	fp12_null(a);
	fp12_null(b);
	fp12_null(c);

	TRY {
		fp12_new(a);
		fp12_new(b);
		fp12_new(c);

		TEST_BEGIN("comparison is consistent") {
			fp12_rand(a);
			fp12_rand(b);
			if (fp12_cmp(a, b) != CMP_EQ) {
				TEST_ASSERT(fp12_cmp(b, a) == CMP_NE, end);
			}
		}
		TEST_END;

		TEST_BEGIN("copy and comparison are consistent") {
			fp12_rand(a);
			fp12_rand(b);
			fp12_rand(c);
			if (fp12_cmp(a, c) != CMP_EQ) {
				fp12_copy(c, a);
				TEST_ASSERT(fp12_cmp(c, a) == CMP_EQ, end);
			}
			if (fp12_cmp(b, c) != CMP_EQ) {
				fp12_copy(c, b);
				TEST_ASSERT(fp12_cmp(b, c) == CMP_EQ, end);
			}
		}
		TEST_END;

		TEST_BEGIN("negation is consistent") {
			fp12_rand(a);
			fp12_neg(b, a);
			if (fp12_cmp(a, b) != CMP_EQ) {
				TEST_ASSERT(fp12_cmp(b, a) == CMP_NE, end);
			}
			fp12_neg(b, b);
			TEST_ASSERT(fp12_cmp(a, b) == CMP_EQ, end);
		}
		TEST_END;

		TEST_BEGIN("assignment to zero and comparison are consistent") {
			fp12_rand(a);
			fp12_zero(c);
			TEST_ASSERT(fp12_cmp(a, c) == CMP_NE, end);
			TEST_ASSERT(fp12_cmp(c, a) == CMP_NE, end);
		}
		TEST_END;

		TEST_BEGIN("assignment to random and comparison are consistent") {
			fp12_rand(a);
			fp12_zero(c);
			TEST_ASSERT(fp12_cmp(a, c) == CMP_NE, end);
		}
		TEST_END;

		TEST_BEGIN("assignment to zero and zero test are consistent") {
			fp12_zero(a);
			TEST_ASSERT(fp12_is_zero(a), end);
		}
		TEST_END;

	}
	CATCH_ANY {
		ERROR(end);
	}
	code = STS_OK;
  end:
	fp12_free(a);
	fp12_free(b);
	fp12_free(c);
	return code;
}

static int addition12(void) {
	int code = STS_ERR;
	fp12_t a, b, c, d, e;

	TRY {
		fp12_new(a);
		fp12_new(b);
		fp12_new(c);
		fp12_new(d);
		fp12_new(e);

		TEST_BEGIN("addition is commutative") {
			fp12_rand(a);
			fp12_rand(b);
			fp12_add(d, a, b);
			fp12_add(e, b, a);
			TEST_ASSERT(fp12_cmp(d, e) == CMP_EQ, end);
		} TEST_END;

		TEST_BEGIN("addition is associative") {
			fp12_rand(a);
			fp12_rand(b);
			fp12_rand(c);
			fp12_add(d, a, b);
			fp12_add(d, d, c);
			fp12_add(e, b, c);
			fp12_add(e, a, e);
			TEST_ASSERT(fp12_cmp(d, e) == CMP_EQ, end);
		} TEST_END;

		TEST_BEGIN("addition has identity") {
			fp12_rand(a);
			fp12_zero(d);
			fp12_add(e, a, d);
			TEST_ASSERT(fp12_cmp(e, a) == CMP_EQ, end);
		} TEST_END;

		TEST_BEGIN("addition has inverse") {
			fp12_rand(a);
			fp12_neg(d, a);
			fp12_add(e, a, d);
			TEST_ASSERT(fp12_is_zero(e), end);
		} TEST_END;
	}
	CATCH_ANY {
		util_print("FATAL ERROR!\n");
		ERROR(end);
	}
	code = STS_OK;
  end:
	fp12_free(a);
	fp12_free(b);
	fp12_free(c);
	fp12_free(d);
	fp12_free(e);
	return code;
}

static int subtraction12(void) {
	int code = STS_ERR;
	fp12_t a, b, c, d;

	TRY {
		fp12_new(a);
		fp12_new(b);
		fp12_new(c);
		fp12_new(d);

		TEST_BEGIN("subtraction is anti-commutative") {
			fp12_rand(a);
			fp12_rand(b);
			fp12_sub(c, a, b);
			fp12_sub(d, b, a);
			fp12_neg(d, d);
			TEST_ASSERT(fp12_cmp(c, d) == CMP_EQ, end);
		}
		TEST_END;

		TEST_BEGIN("subtraction has identity") {
			fp12_rand(a);
			fp12_zero(c);
			fp12_sub(d, a, c);
			TEST_ASSERT(fp12_cmp(d, a) == CMP_EQ, end);
		}
		TEST_END;

		TEST_BEGIN("subtraction has inverse") {
			fp12_rand(a);
			fp12_sub(c, a, a);
			TEST_ASSERT(fp12_is_zero(c), end);
		}
		TEST_END;
	}
	CATCH_ANY {
		util_print("FATAL ERROR!\n");
		ERROR(end);
	}
	code = STS_OK;
  end:
	fp12_free(a);
	fp12_free(b);
	fp12_free(c);
	fp12_free(d);
	return code;
}

static int multiplication12(void) {
	int code = STS_ERR;
	fp12_t a, b, c, d, e, f;

	TRY {
		fp12_new(a);
		fp12_new(b);
		fp12_new(c);
		fp12_new(d);
		fp12_new(e);
		fp12_new(f);

		TEST_BEGIN("multiplication is commutative") {
			fp12_rand(a);
			fp12_rand(b);
			/*fp_set_dig(a[0][0][0], 1);
			fp_set_dig(a[0][0][1], 1);
			fp_set_dig(a[0][1][0], 1);
			fp_set_dig(a[0][1][1], 1);
			fp_set_dig(a[0][2][0], 1);
			fp_set_dig(a[0][2][1], 1);
			fp_set_dig(a[1][0][0], 1);
			fp_set_dig(a[1][0][1], 1);
			fp_set_dig(a[1][1][0], 1);
			fp_set_dig(a[1][1][1], 1);
			fp_set_dig(a[1][2][0], 1);
			fp_set_dig(a[1][2][1], 1);

			fp_set_dig(b[0][0][0], 1);
			fp_set_dig(b[0][0][1], 1);
			fp_set_dig(b[0][1][0], 1);
			fp_set_dig(b[0][1][1], 1);
			fp_set_dig(b[0][2][0], 1);
			fp_set_dig(b[0][2][1], 1);
			fp_set_dig(b[1][0][0], 1);
			fp_set_dig(b[1][0][1], 1);
			fp_set_dig(b[1][1][0], 1);
			fp_set_dig(b[1][1][1], 1);
			fp_set_dig(b[1][2][0], 1);
			fp_set_dig(b[1][2][1], 1);*/
			fp12_mul(d, a, b);
			fp12_mul(e, b, a);
			/*fp12_print(a);
			printf("\n");
			fp12_print(b);
			printf("\n");
			fp12_print(d);*/
			TEST_ASSERT(fp12_cmp(d, e) == CMP_EQ, end);
		} TEST_END;

		TEST_BEGIN("multiplication is associative") {
			fp12_rand(a);
			fp12_rand(b);
			fp12_rand(c);
			fp12_mul(d, a, b);
			fp12_mul(d, d, c);
			fp12_mul(e, b, c);
			fp12_mul(e, a, e);
			TEST_ASSERT(fp12_cmp(d, e) == CMP_EQ, end);
		} TEST_END;

		TEST_BEGIN("multiplication is distributive") {
			fp12_rand(a);
			fp12_rand(b);
			fp12_rand(c);
			fp12_add(d, a, b);
			fp12_mul(d, c, d);
			fp12_mul(e, c, a);
			fp12_mul(f, c, b);
			fp12_add(e, e, f);
			TEST_ASSERT(fp12_cmp(d, e) == CMP_EQ, end);
		} TEST_END;

		TEST_BEGIN("multiplication has identity") {
			fp12_zero(d);
			fp_set_dig(d[0][0][0], 1);
			fp12_mul(e, a, d);
			TEST_ASSERT(fp12_cmp(e, a) == CMP_EQ, end);
		} TEST_END;

		TEST_BEGIN("multiplication has zero property") {
			fp12_zero(d);
			fp12_mul(e, a, d);
			TEST_ASSERT(fp12_is_zero(e), end);
		} TEST_END;

		/*TEST_BEGIN("sparse multiplication is correct") {
			fp12_rand(a);
			fp12_zero(b);
			fp2_rand(b[0][0]);
			fp2_rand(b[1][0]);
			fp2_rand(b[1][1]);
			fp12_mul(d, a, b);
			fp12_mul_dxs(e, a, b);
			TEST_ASSERT(fp12_cmp(d, e) == CMP_EQ, end);
		} TEST_END;*/
	} CATCH_ANY {
		util_print("FATAL ERROR!\n");
		ERROR(end);
	}
	code = STS_OK;
  end:
	fp12_free(a);
	fp12_free(b);
	fp12_free(c);
	fp12_free(d);
	fp12_free(e);
	fp12_free(f);
	return code;
}

static int squaring12(void) {
	int code = STS_ERR;
	fp12_t a, b, c;

	TRY {
		fp12_new(a);
		fp12_new(b);
		fp12_new(c);

		TEST_BEGIN("squaring is correct") {
			fp12_rand(a);
			fp12_mul(b, a, a);
			fp12_sqr(c, a);
			TEST_ASSERT(fp12_cmp(b, c) == CMP_EQ, end);
		} TEST_END;

		TEST_BEGIN("squaring of an unitary element is correct") {
			fp12_rand(a);
			fp12_inv(b, a);
			fp12_inv_uni(a, a);
			fp12_mul(a, a, b);
			fp12_sqr(b, a);
			fp12_sqr(c, a);
			TEST_ASSERT(fp12_cmp(b, c) == CMP_EQ, end);
		} TEST_END;
	}
	CATCH_ANY {
		util_print("FATAL ERROR!\n");
		ERROR(end);
	}
	code = STS_OK;
  end:
	fp12_free(a);
	fp12_free(b);
	fp12_free(c);
	return code;
}

static int inversion12(void) {
	int code = STS_ERR;
	fp12_t a, b, c;

	TRY {
		fp12_new(a);
		fp12_new(b);
		fp12_new(c);

		TEST_BEGIN("inversion is correct") {
			fp12_rand(a);
			fp12_inv(b, a);
			fp12_mul(c, a, b);
			fp12_zero(b);
			fp_set_dig(b[0][0][0], 1);
			TEST_ASSERT(fp12_cmp(c, b) == CMP_EQ, end);
		} TEST_END;

		TEST_BEGIN("inversion of an unitary element is correct") {
			fp12_rand(a);
			fp12_inv(b, a);
			fp12_inv_uni(a, a);
			fp12_mul(a, a, b);
			fp12_inv(b, a);
			fp12_inv_uni(c, a);
			TEST_ASSERT(fp12_cmp(b, c) == CMP_EQ, end);
		} TEST_END;
	}
	CATCH_ANY {
		util_print("FATAL ERROR!\n");
		ERROR(end);
	}
	code = STS_OK;
  end:
	fp12_free(a);
	fp12_free(b);
	fp12_free(c);
	return code;
}

static int exponentiation12(void) {
	int code = STS_ERR;
	fp12_t a, b, c;
	bn_t d, e;

	fp12_null(a);
	fp12_null(b);
	fp12_null(c);
	bn_null(d);
	bn_null(e);

	TRY {
		fp12_new(a);
		fp12_new(b);
		fp12_new(c);
		bn_new(d);
		bn_new(e);
		
		/*TEST_BEGIN("frobenius and exponentiation are consistent") {
			fp12_rand(a);
			fp12_frb(b, a);
			d->used = FP_DIGS;
			dv_copy(d->dp, fp_prime_get(), FP_DIGS);
			fp12_exp(c, a, d);
			TEST_ASSERT(fp12_cmp(c, b) == CMP_EQ, end);
		} TEST_END;
		
		TEST_BEGIN("exponentiation of an unitary element is correct") {
			fp12_rand(a);
			fp12_inv(b, a);
			fp12_inv_uni(a, a);
			fp12_mul(a, a, b);
			d->used = FP_DIGS;
			dv_copy(d->dp, fp_prime_get(), FP_DIGS);
			bn_rand(e, BN_POS, bn_bits(d));
			fp12_exp(b, a, e);
			fp12_exp_uni(c, a, e);
			TEST_ASSERT(fp12_cmp(b, c) == CMP_EQ, end);
		} TEST_END;
		*/
	}
	CATCH_ANY {
		util_print("FATAL ERROR!\n");
		ERROR(end);
	}
	code = STS_OK;
  end:
	fp12_free(a);
	fp12_free(b);
	fp12_free(c);
	bn_free(d);
	bn_free(e);
	return code;
}

static int memory(void) {
	err_t e;
	int code = STS_ERR;
	ep2_t a;

	ep2_null(a);

	TRY {
		TEST_BEGIN("memory can be allocated") {
			ep2_new(a);
			ep2_free(a);
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

int util(void) {
	int code = STS_ERR;
	ep2_t a, b, c;

	ep2_null(a);
	ep2_null(b);
	ep2_null(c);

	TRY {
		ep2_new(a);
		ep2_new(b);
		ep2_new(c);

		TEST_BEGIN("comparison is consistent") {
			ep2_rand(a);
			ep2_rand(b);
			TEST_ASSERT(ep2_cmp(a, b) != CMP_EQ, end);
		}
		TEST_END;

		TEST_BEGIN("copy and comparison are consistent") {
			ep2_rand(a);
			ep2_rand(b);
			if (ep2_cmp(a, c) != CMP_EQ) {
				ep2_copy(c, a);
				TEST_ASSERT(ep2_cmp(c, a) == CMP_EQ, end);
			}
			if (ep2_cmp(b, c) != CMP_EQ) {
				ep2_copy(c, b);
				TEST_ASSERT(ep2_cmp(b, c) == CMP_EQ, end);
			}
		}
		TEST_END;

		TEST_BEGIN("negation and comparison are consistent") {
			ep2_rand(a);
			ep2_neg(b, a);
			TEST_ASSERT(ep2_cmp(a, b) != CMP_EQ, end);
		}
		TEST_END;

		TEST_BEGIN
				("assignment to random/infinity and comparison are consistent")
		{
			ep2_rand(a);
			ep2_set_infty(c);
			TEST_ASSERT(ep2_cmp(a, c) != CMP_EQ, end);
			TEST_ASSERT(ep2_cmp(c, a) != CMP_EQ, end);
		}
		TEST_END;

		TEST_BEGIN("assignment to infinity and infinity test are consistent") {
			ep2_set_infty(a);
			TEST_ASSERT(ep2_is_infty(a), end);
		}
		TEST_END;
	}
	CATCH_ANY {
		util_print("FATAL ERROR!\n");
		ERROR(end);
	}
	code = STS_OK;
  end:
	ep_free(a);
	ep_free(b);
	ep_free(c);
	return code;
}

int addition(void) {
	int code = STS_ERR;
	ep2_t a, b, c, d, e;

	ep2_null(a);
	ep2_null(b);
	ep2_null(c);
	ep2_null(d);
	ep2_null(e);

	TRY {
		ep2_new(a);
		ep2_new(b);
		ep2_new(c);
		ep2_new(d);
		ep2_new(e);

		TEST_BEGIN("point addition is commutative") {
			ep2_rand(a);
			ep2_rand(b);
			ep2_add(d, a, b);
			ep2_add(e, b, a);
			ep2_norm(d, d);
			ep2_norm(e, e);
			TEST_ASSERT(ep2_cmp(d, e) == CMP_EQ, end);
		} TEST_END;

		TEST_BEGIN("point addition is associative") {
			ep2_rand(a);
			ep2_rand(b);
			ep2_rand(c);
			ep2_add(d, a, b);
			ep2_add(d, d, c);
			ep2_add(e, b, c);
			ep2_add(e, e, a);
			ep2_norm(d, d);
			ep2_norm(e, e);
			TEST_ASSERT(ep2_cmp(d, e) == CMP_EQ, end);
		} TEST_END;

		TEST_BEGIN("point addition has identity") {
			ep2_rand(a);
			ep2_set_infty(d);
			ep2_add(e, a, d);
			TEST_ASSERT(ep2_cmp(e, a) == CMP_EQ, end);
			ep2_add(e, d, a);
			TEST_ASSERT(ep2_cmp(e, a) == CMP_EQ, end);
		} TEST_END;

		TEST_BEGIN("point addition has inverse") {
			ep2_rand(a);
			ep2_neg(d, a);
			ep2_add(e, a, d);
			TEST_ASSERT(ep2_is_infty(e), end);
		} TEST_END;

#if EP_ADD == BASIC || !defined(STRIP)
		TEST_BEGIN("point addition in affine coordinates is correct") {
			ep2_rand(a);
			ep2_rand(b);
			ep2_add(d, a, b);
			ep2_norm(d, d);
			ep2_add_basic(e, a, b);
			TEST_ASSERT(ep2_cmp(e, d) == CMP_EQ, end);
		} TEST_END;
#endif

#if EP_ADD == PROJC || !defined(STRIP)
		TEST_BEGIN("point addition in projective coordinates is correct") {
			ep2_rand(a);
			ep2_rand(b);
			ep2_add_projc(a, a, b);
			ep2_rand(b);
			ep2_rand(c);
			ep2_add_projc(b, b, c);
			/* a and b in projective coordinates. */
			ep2_add_projc(d, a, b);
			ep2_norm(d, d);
			ep2_norm(a, a);
			ep2_norm(b, b);
			ep2_add(e, a, b);
			ep2_norm(e, e);
			TEST_ASSERT(ep2_cmp(e, d) == CMP_EQ, end);
		} TEST_END;

		TEST_BEGIN("point addition in mixed coordinates (z2 = 1) is correct") {
			ep2_rand(a);
			ep2_rand(b);
			ep2_add_projc(a, a, b);
			ep2_rand(b);
			/* a and b in projective coordinates. */
			ep2_add_projc(d, a, b);
			ep2_norm(d, d);
			/* a in affine coordinates. */
			ep2_norm(a, a);
			ep2_add(e, a, b);
			ep2_norm(e, e);
			TEST_ASSERT(ep2_cmp(e, d) == CMP_EQ, end);
		} TEST_END;

		TEST_BEGIN("point addition in mixed coordinates (z1,z2 = 1) is correct") {
			ep2_rand(a);
			ep2_rand(b);
			ep2_norm(a, a);
			ep2_norm(b, b);
			/* a and b in affine coordinates. */
			ep2_add(d, a, b);
			ep2_norm(d, d);
			ep2_add_projc(e, a, b);
			ep2_norm(e, e);
			TEST_ASSERT(ep2_cmp(e, d) == CMP_EQ, end);
		} TEST_END;
#endif

	}
	CATCH_ANY {
		ERROR(end);
	}
	code = STS_OK;
  end:
	ep2_free(a);
	ep2_free(b);
	ep2_free(c);
	ep2_free(d);
	ep2_free(e);
	return code;
}

int subtraction(void) {
	int code = STS_ERR;
	ep2_t a, b, c, d;

	ep2_null(a);
	ep2_null(b);
	ep2_null(c);
	ep2_null(d);

	TRY {
		ep2_new(a);
		ep2_new(b);
		ep2_new(c);
		ep2_new(d);

		TEST_BEGIN("point subtraction is anti-commutative") {
			ep2_rand(a);
			ep2_rand(b);
			ep2_sub(c, a, b);
			ep2_sub(d, b, a);
			ep2_norm(c, c);
			ep2_norm(d, d);
			ep2_neg(d, d);
			TEST_ASSERT(ep2_cmp(c, d) == CMP_EQ, end);
		}
		TEST_END;

		TEST_BEGIN("point subtraction has identity") {
			ep2_rand(a);
			ep2_set_infty(c);
			ep2_sub(d, a, c);
			ep2_norm(d, d);
			TEST_ASSERT(ep2_cmp(d, a) == CMP_EQ, end);
		}
		TEST_END;

		TEST_BEGIN("point subtraction has inverse") {
			ep2_rand(a);
			ep2_sub(c, a, a);
			ep2_norm(c, c);
			TEST_ASSERT(ep2_is_infty(c), end);
		}
		TEST_END;

#if EP_ADD == BASIC || !defined(STRIP)
		TEST_BEGIN("point subtraction in affine coordinates is correct") {
			ep2_rand(a);
			ep2_rand(b);
			ep2_sub(c, a, b);
			ep2_norm(c, c);
			ep2_sub_basic(d, a, b);
			TEST_ASSERT(ep2_cmp(c, d) == CMP_EQ, end);
		} TEST_END;
#endif

#if EP_ADD == PROJC || !defined(STRIP)
		TEST_BEGIN("point subtraction in projective coordinates is correct") {
			ep2_rand(a);
			ep2_rand(b);
			ep2_add_projc(a, a, b);
			ep2_rand(b);
			ep2_rand(c);
			ep2_add_projc(b, b, c);
			/* a and b in projective coordinates. */
			ep2_sub_projc(c, a, b);
			ep2_norm(c, c);
			ep2_norm(a, a);
			ep2_norm(b, b);
			ep2_sub(d, a, b);
			ep2_norm(d, d);
			TEST_ASSERT(ep2_cmp(c, d) == CMP_EQ, end);
		} TEST_END;

		TEST_BEGIN("point subtraction in mixed coordinates (z2 = 1) is correct") {
			ep2_rand(a);
			ep2_rand(b);
			ep2_add_projc(a, a, b);
			ep2_rand(b);
			/* a and b in projective coordinates. */
			ep2_sub_projc(c, a, b);
			ep2_norm(c, c);
			/* a in affine coordinates. */
			ep2_norm(a, a);
			ep2_sub(d, a, b);
			ep2_norm(d, d);
			TEST_ASSERT(ep2_cmp(c, d) == CMP_EQ, end);
		} TEST_END;

		TEST_BEGIN("point subtraction in mixed coordinates (z1,z2 = 1) is correct") {
			ep2_rand(a);
			ep2_rand(b);
			ep2_norm(a, a);
			ep2_norm(b, b);
			/* a and b in affine coordinates. */
			ep2_sub(c, a, b);
			ep2_norm(c, c);
			ep2_sub_projc(d, a, b);
			ep2_norm(d, d);
			TEST_ASSERT(ep2_cmp(c, d) == CMP_EQ, end);
		} TEST_END;
#endif
	}
	CATCH_ANY {
		ERROR(end);
	}
	code = STS_OK;
  end:
	ep2_free(a);
	ep2_free(b);
	ep2_free(c);
	ep2_free(d);
	return code;
}

int doubling(void) {
	int code = STS_ERR;
	ep2_t a, b, c;

	ep2_null(a);
	ep2_null(b);
	ep2_null(c);

	TRY {
		ep2_new(a);
		ep2_new(b);
		ep2_new(c);

		TEST_BEGIN("point doubling is correct") {
			ep2_rand(a);
			ep2_add(b, a, a);
			ep2_norm(b, b);
			ep2_dbl(c, a);
			ep2_norm(c, c);
			TEST_ASSERT(ep2_cmp(b, c) == CMP_EQ, end);
		} TEST_END;

#if EP_ADD == BASIC || !defined(STRIP)
		TEST_BEGIN("point doubling in affine coordinates is correct") {
			ep2_rand(a);
			ep2_dbl(b, a);
			ep2_norm(b, b);
			ep2_dbl_basic(c, a);
			TEST_ASSERT(ep2_cmp(b, c) == CMP_EQ, end);
		} TEST_END;
#endif

#if EP_ADD == PROJC || !defined(STRIP)
		/*TEST_BEGIN("point doubling in projective coordinates is correct") {
			ep2_rand(a);
			ep2_dbl_projc(a, a);
			// a in projective coordinates. 
			ep2_dbl_projc(b, a);
			ep2_norm(b, b);
			ep2_norm(a, a);
			ep2_dbl(c, a);
			ep2_norm(c, c);
			TEST_ASSERT(ep2_cmp(b, c) == CMP_EQ, end);
		} TEST_END;

		TEST_BEGIN("point doubling in mixed coordinates (z1 = 1) is correct") {
			ep2_rand(a);
			ep2_dbl_projc(b, a);
			ep2_norm(b, b);
			ep2_dbl(c, a);
			ep2_norm(c, c);
			TEST_ASSERT(ep2_cmp(b, c) == CMP_EQ, end);
		} TEST_END;*/
#endif
	}
	CATCH_ANY {
		ERROR(end);
	}
	code = STS_OK;
  end:
	ep2_free(a);
	ep2_free(b);
	ep2_free(c);
	return code;
}

static int multiplication(void) {
	int code = STS_ERR;
	ep2_t p, q, r;
	bn_t n, k;

	bn_null(n);
	bn_null(k);
	ep2_null(p);
	ep2_null(q);
	ep2_null(r);

	TRY {
		ep2_new(p);
		ep2_new(q);
		ep2_new(r);
		bn_new(n);
		bn_new(k);

		ep2_curve_get_gen(p);
		ep2_curve_get_ord(n);

		TEST_BEGIN("generator has the right order") {
			ep2_mul(r, p, n);
			//ep_print(r);
			//bn_print(n);
			//ep_print(p);
			TEST_ASSERT(ep2_is_infty(r) == 1, end);
		} TEST_END;

		TEST_BEGIN("generator multiplication is correct") {
			bn_rand(k, BN_POS, bn_bits(n));
			bn_mod(k, k, n);
			ep2_mul(q, p, k);
			ep2_mul_gen(r, k);
			TEST_ASSERT(ep2_cmp(q, r) == CMP_EQ, end);
		} TEST_END;
	}
	CATCH_ANY {
		util_print("FATAL ERROR!\n");
		ERROR(end);
	}
	code = STS_OK;
  end:
	ep2_free(p);
	ep2_free(q);
	ep2_free(r);
	bn_free(n);
	bn_free(k);
	return code;
}

static int o_ate_pairing(void) {
	int code = STS_ERR;
	fp12_t e1, e2;
	ep2_t p;
	ep_t q, r;
	bn_t k, n;

	fp12_null(e1);
	fp12_null(e2);
	ep2_null(p);
	ep_null(q);
	ep_null(r);
	bn_null(k);
	bn_null(n);

	TRY {
		fp12_new(e1);
		fp12_new(e2);
		ep2_new(p);
		ep_new(q);
		ep_new(r);
		bn_new(n);
		bn_new(k);

		ep_curve_get_ord(n);

		TEST_BEGIN("optimal ate pairing is bilinear") {
		  ep2_rand(p);
		  ep_rand(q);
		  bn_rand(k, BN_POS, bn_bits(n));
		  bn_mod(k, k, n);
		  ep_mul(r, q, k);
		  fp12_zero(e1);
		  fp12_zero(e2);
		  //ep2_curve_get_gen(q);
		  //ep_curve_get_gen(p);
		  pp_map_o_ate(e1, p, r);
		  ep2_mul(p, p, k);
		  pp_map_o_ate(e2, p, q);
		  TEST_ASSERT(fp12_cmp(e1, e2) == CMP_EQ, end);
                } TEST_END;

	}
	CATCH_ANY {
		util_print("FATAL ERROR!\n");
		ERROR(end);
	}
	code = STS_OK;
  end:
	fp12_free(e1);
	fp12_free(e2);
	ep2_free(p);
	ep_free(q);
	ep_free(r);
	bn_free(n);
	bn_free(k);
	return code;
}

static int rate_pairing(void) {
  int code = STS_ERR;
  fp12_t e1, e2;
  ep2_t p;
  ep_t q, r;
  bn_t k, n;

  fp12_null(e1);
  fp12_null(e2);
  ep2_null(p);
  ep_null(q);
  ep_null(r);
  bn_null(k);
  bn_null(n);

  TRY {
    fp12_new(e1);
    fp12_new(e2);
    ep2_new(p);
    ep_new(q);
    ep_new(r);
    bn_new(n);
    bn_new(k);

    ep_curve_get_ord(n);

    TEST_BEGIN("rate pairing is bilinear") {
      ep2_rand(p);
      ep_rand(q);
      bn_rand(k, BN_POS, bn_bits(n));
      bn_mod(k, k, n);
      ep_mul(r, q, k);
      fp12_zero(e1);
      fp12_zero(e2);
      pp_map_rate(e1, p, r);
      ep2_mul(p, p, k);
      pp_map_rate(e2, p, q);
      TEST_ASSERT(fp12_cmp(e1, e2) == CMP_EQ, end);
    } TEST_END;
  }
  CATCH_ANY {
    util_print("FATAL ERROR!\n");
    ERROR(end);
  }
  code = STS_OK;
 end:
  fp12_free(e1);
  fp12_free(e2);
  ep2_free(p);
  ep_free(q);
  ep_free(r);
  bn_free(n);
  bn_free(k);
  return code;
}

    
int main(void) {
        
       core_init();

	if (fp_param_set_any_tower() != STS_OK) {
		THROW(ERR_NO_FIELD);
		core_clean();
		return 0;
	}
	
	util_print_banner("Tests for the PP module", 0);

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

	if (doubling2() != STS_OK) {
		core_clean();
		return 1;
	}

        if (division2() != STS_OK) {
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

	if (exponentiation2() != STS_OK) {
		core_clean();
		return 1;
	}

	util_print_banner("Sextic extension:", 0);
	util_print_banner("Utilities:", 1);

	if (memory6() != STS_OK) {
		core_clean();
		return 1;
	}

	if (util6() != STS_OK) {
		core_clean();
		return 1;
	}

	util_print_banner("Arithmetic:", 1);

	if (addition6() != STS_OK) {
		core_clean();
		return 1;
	}

	if (subtraction6() != STS_OK) {
		core_clean();
		return 1;
	}

	if (doubling6() != STS_OK) {
		core_clean();
		return 1;
	}

	if (multiplication6() != STS_OK) {
		core_clean();
		return 1;
	}

	if (squaring6() != STS_OK) {
		core_clean();
		return 1;
	}

	if (inversion6() != STS_OK) {
		core_clean();
		return 1;
	}

	if (exponentiation6() != STS_OK) {
		core_clean();
		return 1;
	}

/*	util_print_banner("Dodecic extension:", 0);
	util_print_banner("Utilities:", 1);

	if (memory12() != STS_OK) {
		core_clean();
		return 1;
	}

	if (util12() != STS_OK) {
		core_clean();
		return 1;
	}

	util_print_banner("Arithmetic:", 1);

	if (addition12() != STS_OK) {
		core_clean();
		return 1;
	}

	if (subtraction12() != STS_OK) {
		core_clean();
		return 1;
	}

	if (multiplication12() != STS_OK) {
		core_clean();
		return 1;
	}

	if (squaring12() != STS_OK) {
		core_clean();
		return 1;
	}

	if (inversion12() != STS_OK) {
		core_clean();
		return 1;
	}

	if (exponentiation12() != STS_OK) {
		core_clean();
		return 1;
	}*/

	if (ep_param_set_any_pairf() == STS_ERR) {
		THROW(ERR_NO_CURVE);
		core_clean();
		return 0;
	}

	util_print_banner("Quadratic twist:", 0);
	util_print_banner("Utilities:", 1);

	if (memory() != STS_OK) {
		core_clean();
		return 1;
	}

	if (util() != STS_OK) {
		core_clean();
		return 1;
	}

	util_print_banner("Arithmetic:", 1);

	if (addition() != STS_OK) {
		core_clean();
		return 1;
	}

	if (subtraction() != STS_OK) {
		core_clean();
		return 1;
	}

	if (doubling() != STS_OK) {
		core_clean();
		return 1;
	}

	if (multiplication() != STS_OK) {
		core_clean();
		return 1;
	}

	util_print_banner("Bilinear pairing:\n", 0);

/*	if (rate_pairing() != STS_OK) {
		core_clean();
		return 1;
	}*/

        if (o_ate_pairing() != STS_OK) {
	  core_clean();
	  return 1;
        }

	core_clean();
	return 0;
}
