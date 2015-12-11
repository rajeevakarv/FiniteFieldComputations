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
 * Implementation of the prime elliptic curve utilities.
 *
 * @version $Id: relic_ep_param.c 278 2010-02-06 07:53:40Z dfaranha $
 * @ingroup ep
 */

#include <string.h>

#include "relic_core.h"
#include "relic_ep.h"
#include "relic_pp.h"
#include "relic_error.h"
#include "relic_conf.h"

#if defined(EP_STAND)

/*============================================================================*/
/* Private definitions                                                        */
/*============================================================================*/

#if defined(EP_ORDIN) && FP_PRIME == 160
/**
 * Parameters for the NIST P-192 binary elliptic curve.
 */
/** @{ */
#define SECG_P160_A		"FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF7FFFFFFC"
#define SECG_P160_B		"1C97BEFC54BD7A8B65ACF89F81D4D4ADC565FA45"
#define SECG_P160_X		"4A96B5688EF573284664698968C38BB913CBFC82"
#define SECG_P160_Y		"23A628553168947D59DCC912042351377AC5FB32"
#define SECG_P160_R		"100000000000000000001F4C8F927AED3CA752257"
/** @} */
#endif

#if defined(EP_ORDIN) && FP_PRIME == 192
/**
 * Parameters for the NIST P-192 binary elliptic curve.
 */
/** @{ */
#define NIST_P192_A		"FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFFFFFFFFFFFC"
#define NIST_P192_B		"64210519E59C80E70FA7E9AB72243049FEB8DEECC146B9B1"
#define NIST_P192_X		"188DA80EB03090F67CBF20EB43A18800F4FF0AFD82FF1012"
#define NIST_P192_Y		"07192B95FFC8DA78631011ED6B24CDD573F977A11E794811"
#define NIST_P192_R		"FFFFFFFFFFFFFFFFFFFFFFFF99DEF836146BC9B1B4D22831"
/** @} */
#endif

#if defined(EP_ORDIN) && FP_PRIME == 224
/**
 * Parameters for the NIST P-192 binary elliptic curve.
 */
/** @{ */
#define NIST_P224_A		"FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFFFFFFFFFFFFFFFFFFFE"
#define NIST_P224_B		"B4050A850C04B3ABF54132565044B0B7D7BFD8BA270B39432355FFB4"
#define NIST_P224_X		"B70E0CBD6BB4BF7F321390B94A03C1D356C21122343280D6115C1D21"
#define NIST_P224_Y		"BD376388B5F723FB4C22DFE6CD4375A05A07476444D5819985007E34"
#define NIST_P224_R		"FFFFFFFFFFFFFFFFFFFFFFFFFFFF16A2E0B8F03E13DD29455C5C2A3D"
/** @} */
#endif

#if defined(EP_ORDIN) && FP_PRIME == 256
/**
 * Parameters for the NIST P-192 binary elliptic curve.
 */
/** @{ */
#define NIST_P256_A		"FFFFFFFF00000001000000000000000000000000FFFFFFFFFFFFFFFFFFFFFFFC"
#define NIST_P256_B		"5AC635D8AA3A93E7B3EBBD55769886BC651D06B0CC53B0F63BCE3C3E27D2604B"
#define NIST_P256_X		"6B17D1F2E12C4247F8BCE6E563A440F277037D812DEB33A0F4A13945D898C296"
#define NIST_P256_Y		"4FE342E2FE1A7F9B8EE7EB4A7C0F9E162BCE33576B315ECECBB6386837BF51F5"
#define NIST_P256_R		"FFFFFFFF00000000FFFFFFFFFFFFFFFFBCE6FAADA7179E84F3B9CAC2FC632551"
/** @} */
#endif

#if defined(EP_ORDIN) && FP_PRIME == 384
/**
 * Parameters for the NIST P-192 binary elliptic curve.
 */
/** @{ */
#define NIST_P384_A		"FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFFFF0000000000000000FFFFFFFC"
#define NIST_P384_B		"B3312FA7E23EE7E4988E056BE3F82D19181D9C6EFE8141120314088F5013875AC656398D8A2ED19D2A85C8EDD3EC2AEF"
#define NIST_P384_X		"AA87CA22BE8B05378EB1C71EF320AD746E1D3B628BA79B9859F741E082542A385502F25DBF55296C3A545E3872760AB7"
#define NIST_P384_Y		"3617DE4A96262C6F5D9E98BF9292DC29F8F41DBD289A147CE9DA3113B5F0B8C00A60B1CE1D7E819D7A431D7C90EA0E5F"
#define NIST_P384_R		"FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFC7634D81F4372DDF581A0DB248B0A77AECEC196ACCC52973"
/** @} */
#endif

#if defined(EP_ORDIN) && FP_PRIME == 521
/**
 * Parameters for the NIST P-192 binary elliptic curve.
 */
/** @{ */
#define NIST_P521_A		"1FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFC"
#define NIST_P521_B		"51953EB9618E1C9A1F929A21A0B68540EEA2DA725B99B315F3B8B489918EF109E156193951EC7E937B1652C0BD3BB1BF073573DF883D2C34F1EF451FD46B503F00"
#define NIST_P521_X		"C6858E06B70404E9CD9E3ECB662395B4429C648139053FB521F828AF606B4D3DBAA14B5E77EFE75928FE1DC127A2FFA8DE3348B3C1856A429BF97E7E31C2E5BD66"
#define NIST_P521_Y		"11839296A789A3BC0045C8A5FB42C7D1BD998F54449579B446817AFBD17273E662C97EE72995EF42638C550B9013FAD0761353C7086A272C24088BE94769FD16650"
#define NIST_P521_R		"1FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFA51868783BF2F966B7FCC0148F709A5D03BB5C9B8899C47AEBB6FB71E91386389"
/** @} */
#endif

#if defined(EP_ORDIN) && FP_PRIME == 256
/**
 * Parameters for a pairing-friendly prime curve.
 */
/** @{ */
#define BNN_P256_A		"0"
//#define BNN_P256_B		"16"
#define BNN_P256_B		"2"
//#define BNN_P256_X		"1"
//#define BNN_P256_Y		"C7424FC261B627189A14E3433B4713E9C2413FCF89B8E2B178FB6322EFB2AB3"
#define BNN_P256_X		"2523648240000001BA344D80000000086121000000000013A700000000000012"
#define BNN_P256_Y		"1"
#define BNN_P256_R		"2523648240000001BA344D8000000007FF9F800000000010A10000000000000D"
/** @} */
#endif

#if defined(EP_ORDIN) && FP_PRIME == 158
/**
 * Parameters for a 158-bit pairing-friendly prime curve.
 */
/** @{ */
#define BN_P158_A               "0"
#define BN_P158_B               "3"
#define BN_P158_X               "1"
#define BN_P158_Y               "2"
#define BN_P158_R               "240000006ED000007FE96000419F59800C9FFD81"
#define BN_P158_BETA    "240000006E8800007F434000411F93C00C7F3B08"
#define BN_P158_LAMB    "240000006E4000007E9D800040A063800C5EB323"
/** @} */
#endif

#if defined(EP_ORDIN) && FP_PRIME == 446
/**
 * Parameters for a pairing-friendly prime curve.
 */
/** @{ */
#define BN_P446_A		"0"
#define BN_P446_B		"101"
#define BN_P446_X		"2400000000000000002400000002D00000000D800000021C0000001800000000870000000B0400000057C00000015C000000132000000066"
#define BN_P446_Y		"10"
#define BN_P446_R		"2400000000000000002400000002D00000000D800000021C00000017A0000000870000000AD400000054C000000156000000126000000061"
/** @} */
#endif

#if defined(EP_ORDIN) && FP_PRIME == 286
/**
 * Parameters for a pairing-friendly prime curve.
 */
/** @{ */
#define BN_P286_A		"0"
#define BN_P286_B		"2"
#define BN_P286_X		"240900D8991B25B0E2CB51DDA534A205391892080A008108000853813800138000000012"
#define BN_P286_Y		"1"
#define BN_P286_R		"240900D8991B25B0E2CB51DDA534A205391831FC099FC0FC0007F081080010800000000D"
/** @} */
#endif

#if defined(EP_ORDIN) && FP_PRIME == 478
/**
 * Parameters for a pairing-friendly prime curve.
 */
/** @{ */
#define BN_P478_A		"0"
#define BN_P478_B		"2"
#define BN_P478_X		"23FFFFFFFFFFFFFEDFFFFFFFEE0001B3600000006BFFF5DB835FFF5D28085442328002888F96F2944D7DED781430FFD780065FFF010020FFFD900012"
#define BN_P478_Y		"1"
#define BN_P478_R		"23FFFFFFFFFFFFFEDFFFFFFFEE0001B3600000006BFFF5DB835FFF5D2807F442328002888F9872944D7E0578112F7FD780062FFF07001F7FFDF0000D"
/** @} */
#endif

#if defined(EP_ORDIN) && FP_PRIME == 606
/**
 * Parameters for a pairing-friendly prime curve.
 */
/** @{ */
#define BN_P606_A		"0"
#define BN_P606_B		"2"
#define BN_P606_X		"23FFFFFFFFFFFEE00000000000036000000241AFFB7FFFFFF275E0024000001B1440000D94482DF27FFFC9AEDF0000000036512100245142137FFFFFB75D7BD900000000000000246C844E12"
#define BN_P606_Y		"1"
#define BN_P606_R		"23FFFFFFFFFFFEE00000000000036000000241AFFB7FFFFFF275E0024000001B1440000D9447CDF27FFFC9AEE08000000036511F8024513F107FFFFFB75D81DF00000000000000246C7E420D"
/** @} */
#endif

#if defined(EP_ORDIN) && FP_PRIME == 638
/**
 * Parameters for a pairing-friendly prime curve.
 */
/** @{ */
#define BN_P638_A		"0"
#define BN_P638_B		"101"
#define BN_P638_X		"23FFFFFDC000000D7FFFFFB8000001D3FFFFF942D000165E3FFF94870000D52FFFFDD0E00008DE55C00086520021E55BFFFFF51FFFF4EB800000004C80015ACDFFFFFFFFFFFFECE00000000000000066"
#define BN_P638_Y		"10"
#define BN_P638_R		"23FFFFFDC000000D7FFFFFB8000001D3FFFFF942D000165E3FFF94870000D52FFFFDD0E00008DE55600086550021E555FFFFF54FFFF4EAC000000049800154D9FFFFFFFFFFFFEDA00000000000000061"
/** @} */
#endif

#if ARCH == AVR

#include <avr/pgmspace.h>

/**
 * Copies a string from the text section to the destination vector.
 *
 * @param[out] dest		- the destination vector.
 * @param[in] src		- the pointer to the string stored on the text section.
 */
static void copy_from_rom(char *dest, const char *src) {
	char c;
	while ((c = pgm_read_byte(src++)))
		*dest++ = c;
	*dest = 0;
}

#endif

/**
 * Prepares a set of elliptic curve parameters.
 *
 * @param[out] STR		- the resulting prepared parameter.
 * @param[in] ID		- the parameter represented as a string.
 */
#if ARCH == AVR
#define PREPARE(STR, ID)													\
	copy_from_rom(STR, PSTR(ID));
#else
#define PREPARE(STR, ID)													\
	str = ID;
#endif

/**
 * Assigns a set of ordinary elliptic curve parameters.
 *
 * @param[in] CURVE		- the curve parameters to assign.
 * @param[in] FIELD		- the finite field identifier.
 */
#define ASSIGN(CURVE, FIELD)												\
	fp_param_set(FIELD);													\
	PREPARE(str, CURVE##_A);												\
	fp_read(a, str, strlen(str), 16);										\
	PREPARE(str, CURVE##_B);												\
	fp_read(b, str, strlen(str), 16);										\
	PREPARE(str, CURVE##_X);												\
	fp_read(g->x, str, strlen(str), 16);									\
	PREPARE(str, CURVE##_Y);												\
	fp_read(g->y, str, strlen(str), 16);									\
	PREPARE(str, CURVE##_R);												\
	bn_read_str(r, str, strlen(str), 16);									\

/**
 * Assigns a set of Koblitz elliptic curve parameters.
 *
 * @param[in] CURVE             - the curve parameters to assign.
 * @param[in] FIELD             - the finite field identifier.
 */
#define ASSIGNK(CURVE, FIELD)                                                                                           \
        ASSIGN(CURVE, FIELD);                                                                                                   \
        PREPARE(str, CURVE##_BETA);                                                                                             \
        fp_read(beta, str, strlen(str), 16);                                                                    \
        PREPARE(str, CURVE##_LAMB);                                                                                             \
        bn_read_str(lamb, str, strlen(str), 16);                        \

/**
 * Current configured elliptic curve parameters.
 */
static int param_id;

/*============================================================================*/
/* Public definitions                                                         */
/*============================================================================*/

int ep_param_get() {
	return param_id;
}

void ep_param_set(int param) {
	int ordin = 0;
#if ARCH == AVR
	char str[2 * FB_DIGS + 1];
#else
	char *str;
#endif
	fp_t a, b, beta;
	ep_t g;
	bn_t r, lamb;

	fp_null(a);
	fp_null(b);
        fp_null(beta);
        bn_null(lamb);
	ep_null(g);
	bn_null(r);

	TRY {
		fp_new(a);
		fp_new(b);
		fp_new(beta);
                bn_new(lamb);
		ep_new(g);
		bn_new(r);

		switch (param) {
#if defined(EP_ORDIN) && FP_PRIME == 158
                        case BN_P158:
                                ASSIGNK(BN_P158, BN_158);
                                ordin = 1;
                                break;
#endif
#if defined(EP_ORDIN) && FP_PRIME == 160
			case SECG_P160:
				ASSIGN(SECG_P160, SECG_160);
				ordin = 1;
				break;
#endif
#if defined(EP_ORDIN) && FP_PRIME == 192
			case NIST_P192:
				ASSIGN(NIST_P192, NIST_192);
				ordin = 1;
				break;
#endif
#if defined(EP_ORDIN) && FP_PRIME == 224
			case NIST_P224:
				ASSIGN(NIST_P224, NIST_224);
				ordin = 1;
				break;
#endif
#if defined(EP_ORDIN) && FP_PRIME == 256
			case NIST_P256:
				ASSIGN(NIST_P256, NIST_256);
				ordin = 1;
				break;
#endif
#if defined(EP_ORDIN) && FP_PRIME == 384
			case NIST_P384:
				ASSIGN(NIST_P384, NIST_384);
				ordin = 1;
				break;
#endif
#if defined(EP_ORDIN) && FP_PRIME == 521
			case NIST_P521:
				ASSIGN(NIST_P521, NIST_521);
				ordin = 1;
				break;
#endif
#if defined(EP_ORDIN) && FP_PRIME == 256
			case BNN_P256:
				ASSIGN(BNN_P256, BNN_256);
				ordin = 1;
				break;
#endif
#if defined(EP_ORDIN) && FP_PRIME == 446
			case BN_P446:
				ASSIGN(BN_P446, BN_446);
				ordin = 1;
				break;
#endif
#if defined(EP_ORDIN) && FP_PRIME == 286
			case BN_P286:
				ASSIGN(BN_P286, BN_286);
				ordin = 1;
				break;
#endif
#if defined(EP_ORDIN) && FP_PRIME == 478
			case BN_P478:
				ASSIGN(BN_P478, BN_478);
				ordin = 1;
				break;
#endif
#if defined(EP_ORDIN) && FP_PRIME == 606
			case BN_P606:
				ASSIGN(BN_P606, BN_606);
				ordin = 1;
				break;
#endif
#if defined(EP_ORDIN) && FP_PRIME == 638
			case BN_P638:
				ASSIGN(BN_P638, BN_638);
				ordin = 1;
				break;
#endif
			default:
				(void)str;
				THROW(ERR_INVALID);
				break;
		}

		param_id = param;

		fp_zero(g->z);
		fp_set_dig(g->z, 1);
		g->norm = 1;

		ep_curve_set_ordin(a, b, g, r);
	}
	CATCH_ANY {
		THROW(ERR_CAUGHT);
	}
	FINALLY {
		fp_free(a);
		fp_free(b);
		ep_free(g);
		bn_free(r);
	}
}

#endif /* EP_STAND */

int ep_param_set_any() {
	int r0, r1;

	r0 = ep_param_set_any_ordin();
	if (r0 == STS_ERR) {
		r1 = ep_param_set_any_pairf();
		if (r1 == STS_ERR) {
			return STS_ERR;
		}
	}
	return STS_OK;
}

int ep_param_set_any_ordin() {
	int r = STS_OK;
#if FP_PRIME == 158
        ep_param_set(BN_P158);
#elif FP_PRIME == 160
	ep_param_set(SECG_P160);
#elif FP_PRIME == 192
	ep_param_set(NIST_P192);
#elif FP_PRIME == 224
	ep_param_set(NIST_P224);
#elif FP_PRIME == 256
	ep_param_set(NIST_P256);
#elif FP_PRIME == 384
	ep_param_set(NIST_P384);
#elif FP_PRIME == 446
	ep_param_set(BN_P446);
#elif FP_PRIME == 521
	ep_param_set(NIST_P521);
#elif FP_PRIME == 286
	ep_param_set(BN_P286);
#elif FP_PRIME == 478
	ep_param_set(BN_P478);
#elif FP_PRIME == 606
	ep_param_set(BN_P606);
#elif FP_PRIME == 638
	ep_param_set(BN_P638);
#else
	r = STS_ERR;
#endif
	return r;
}

int ep_param_set_any_pairf() {
	int r = STS_OK;
#if FP_PRIME == 256
	ep_param_set(BNN_256);
	ep2_curve_set(1);
#elif FP_PRIME == 446
	ep_param_set(BN_446);
	ep2_curve_set(2);
#elif FP_PRIME == 286
	ep_param_set(BN_286);
	ep2_curve_set(1);
#elif FP_PRIME == 478
	ep_param_set(BN_478);
	ep2_curve_set(1);
#elif FP_PRIME == 606
	ep_param_set(BN_606);
	ep2_curve_set(1);
#elif FP_PRIME == 638
	ep_param_set(BN_638);
	ep2_curve_set(2);
#else
	r = STS_ERR;
#endif
	return r;
}

void ep_param_print() {
	switch (param_id) {
		case SECG_P160:
			util_print_banner("Curve SECG-P160:", 0);
			break;
		case NIST_P192:
			util_print_banner("Curve NIST-P192:", 0);
			break;
		case NIST_P224:
			util_print_banner("Curve NIST-P224:", 0);
			break;
		case NIST_P256:
			util_print_banner("Curve NIST-P256:", 0);
			break;
		case NIST_P384:
			util_print_banner("Curve NIST-P384:", 0);
			break;
		case NIST_P521:
			util_print_banner("Curve NIST-P521:", 0);
			break;
		case BNN_P256:
			util_print_banner("Curve BNN-P256:", 0);
			break;
		case BNP_P256:
			util_print_banner("Curve BNP-P256:", 0);
			break;
		case BN_P158:
                        util_print_banner("Curve BN-P158:", 0);
                        break;
		case BN_P446:
                        util_print_banner("Curve BN-P446:", 0);
                        break;
		case BN_P286:
                        util_print_banner("Curve BN-P286:", 0);
                        break;
		case BN_P478:
                        util_print_banner("Curve BN-P478:", 0);
                        break;
		case BN_P606:
                        util_print_banner("Curve BN-P606:", 0);
                        break;
		case BN_P638:
                        util_print_banner("Curve BN-P638:", 0);
                        break;
	}
}
