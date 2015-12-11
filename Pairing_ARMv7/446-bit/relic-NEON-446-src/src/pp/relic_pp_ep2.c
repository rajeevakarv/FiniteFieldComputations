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
 * Implementation of elliptic prime curves over quadratic extensions.
 *
 * @version $Id: relic_pp_ep2.c 275 2010-02-06 06:16:03Z dfaranha $
 * @ingroup pp
 */

#include "relic_core.h"
#include "relic_md.h"
#include "relic_pp.h"
#include "relic_error.h"
#include "relic_conf.h"

/*============================================================================*/
/* Private definitions                                                        */
/*============================================================================*/

#if defined(EP_ORDIN) && FP_PRIME == 256
/**
 * Parameters for a pairing-friendly prime curve over a quadratic extension.
 */
/** @{ */
/*#define BNN_P256_X0		"1822AA754FAFAFF95FE37842D7D5DECE88305EC19B363F6681DF06BF405F02B4"
#define BNN_P256_X1		"1AB4CC8A133A7AA970AADAE37C20D1C7191279CBA02830AFC64C19B50E8B1997"
#define BNN_P256_Y0		"16737CF6F9DEC5895A7E5A6D60316763FB6638A0A82F26888E909DA86F7F84BA"
#define BNN_P256_Y1		"05B6DB6FF5132FB917E505627E7CCC12E0CE9FCC4A59805B3B730EE0EC44E43C"
#define BNN_P256_R		"5633FF249938445904D63EF07C4DCC56A3D53BC318AC022A68794AE6A80008F79BB4B3140000188AFC77D00000002B2EBA10000000002C68200000000000145L"*/
/** @} */
#define BNN_P256_X0              "061A10BB519EB62FEB8D8C7E8C61EDB6A4648BBB4898BF0D91EE4224C803FB2B"
#define BNN_P256_X1              "0516AAF9BA737833310AA78C5982AA5B1F4D746BAE3784B70D8C34C1E7D54CF3"
#define BNN_P256_Y0              "021897A06BAF93439A90E096698C822329BD0AE6BDBE09BD19F0E07891CD2B9A"
#define BNN_P256_Y1              "0EBB2B0E7C8B15268F6D4456F5F38D37B09006FFD739C9578A2D1AEC6B3ACE9B"
#define BNN_P256_R               "2523648240000001BA344D8000000007FF9F800000000010A10000000000000D"
#endif

#if defined(EP_ORDIN) && FP_PRIME == 446
/**
 * Parameters for a pairing-friendly prime curve over a quadratic extension.
 */
/** @{ */
/*#define BN_P446_X1		"1F46FE29C7F552AC9B3F0C3FFF9B815419875A50B370291F9B08638126DB871A67CCBC0A6A2AF061825BF7D0D8C02DE1EE6A9CA4AE4C0683"
#define BN_P446_X0		"1A98007AE15DBA2256DE3662B85E5E5A9319AA9B9ADCBCE2276A756622B7482D60689952DE95D0A43536893C1AC286988877AB18CB43EBDF"
#define BN_P446_Y1		"45AF86732C3FC512C1DDB035DF7D5A388B856ECA9968236E8C28B2491CDC748B1FAD8484255D737D9C6586E049731A711FEEF75A509FB31"
#define BN_P446_Y0		"530D42C30350BB3CF38579D63FA1574C2D98C48AD61264C659D98C4DD18EB273DC54288B051E4F49AE4D41834E806CEC770A06323C823AF"*/

/*#define BN_P446_X1		"E0FFC7E23E0E09CE8D0B3890017A6991D5CD450CA145F86AE5F3A4BCFFB6B5C1C7E9F8067607B639AFEC2836B26B45DC3C4141863C83C07"
#define BN_P446_X0		"953F726D4B2336CED4B155030088E066AAC4761911505E1116FB38EE1902417B48561325323FEF9AB8A02DF14312ED50C63BDF09F8B664C"
#define BN_P446_Y1		"17D3F89D57617B58EAE1E093557D4976597B229F17A608C122303BE8F3F645A43BC2B7CCDFC7136795F944E3E271F6C4F3F88D891D8E32C8"
#define BN_P446_Y0		"190832DC09CC26FC80A2D7769F3C3D1F62962D4159FDC957072949A320FDDD969B95F75C84DEF303A5A958519468780FF4CECC594B5B165F"*/
#define BN_P446_X1		"2062D524CCDCC7D89FD53580B3753D64F5F457E4BD9104DC487355A6227A1AAA3E3ACC754EBB9E7E7D132C0B3DEE1C98982FEAE444280F58"
#define BN_P446_X0		"114CD7A6941099C8038C090B392A409FAD4ED757FEB8DE0233C00536F43B9ABE965A7BD0C3F2603BE179CF46F746F97462C62EF5ADFAB3D8"
#define BN_P446_Y1		"3DC5A2F22D12812DC806AFAB349B2712914AD6F7CCFF0C878780110A17CF16CAC8F4CE2CF63574EB3CD80BDFABAC8BFAD0D252BF044260D"
#define BN_P446_Y0		"1027AE0E33BD4C8F9A7A032B10FC24E45E4AB790CF25B6073CB1EE61A38347B166BE5024E50C702D3B54F9400AB3A11A7782FF9081761A3F"
#define BN_P446_R		"510000000000000000A20000000CA80000008DC00000162600000127500000109C80000166CE00000FE9A00000D6D700000CF828000080F4000005E10A00004773F00002613D8000172A310000E431E00006372F000030021000017CBD400007A6E80000284F400000F5AC00000294DL"
//#define BN_P446_R		"2400000000000000002400000002D00000000D800000021C00000017A0000000870000000AD400000054C000000156000000126000000061"
/** @} */
#endif

#if defined(EP_ORDIN) && FP_PRIME == 286
/**
 * Parameters for a pairing-friendly prime curve over a quadratic extension.
 */
/** @{ */
#define BN_P286_X1		"97E7854FCBC1A6C2DC584E513C560D45D47555170423A58B8D46955B6F766A56843B0A3"
#define BN_P286_X0		"53E6C2005647B02C8B6495359A99A244595890A5A46E458F9D785BEDAE58A8D172A59F"
#define BN_P286_Y1		"22043B9B72404714410E17D7FBE45465177DAB9531EF5CA38E91037D0F13E8F0CFBEF895"
#define BN_P286_Y0		"1F3140102C724CFAEF589CE1BFFD42320DFEDE2089FCAEEAA8564FA3E6FB23798616AECD"
#define BN_P286_R		"512888DFA4B1DC51F9A31BF371A0BD13F9E58EBF2C7EB4C5C2DC77BD21C038CCEDA1E944CF302FDC833B6058F7A587BEA2EE10E75D0830C2D30D50802AB012C1002C10000000145L"
/** @} */
#endif

#if defined(EP_ORDIN) && FP_PRIME == 478
/**
 * Parameters for a pairing-friendly prime curve over a quadratic extension.
 */
/** @{ */
#define BN_P478_X1		"230790D7BECB6812ABAACDCF87674FF11E6CE425B17ABBE5AABCD36A34542628498F27AF9767B1C305566360DFA548669579B4CA45C58263D40DBB33"
#define BN_P478_X0		"17D42AA724406C8EA4393CDDA9695CADF88CD52D0B88C63D2E98E1CD9529A3BFBB80ABF81C75E71A685EBD16ACE6DB2BAFCFDF42D165B7CD4C3B3673"
#define BN_P478_Y1		"D132D7AF8570E941A9E043EB20F7ADFF81EF6EA51DA6C707B745B3AD778CA5306F25874E72159C68CE6739FFF3D03A3C4CB848008D7EEC26E8B85AE"
#define BN_P478_Y0		"221AAC19CB331DAE87C46573A09207E4EBA31D862FABA541E86EA8876DD86499E7673B76BD2797581683BEDF5F9B20203A529F5BCB79687C48B28A16"
#define BN_P478_R		"50FFFFFFFFFFFFFAEFFFFFFFAF0007BB700000046DFF9522636FF9406C552F40B6C050148B3B720F2E006CA55149C5B61DD2AF595171D4C6D6EE31F1039308FBA75BAE5FCF30B48312BF8ADF1D3AB0CDD1BBA7A09BDF172B2A39EEE9B27C1C46AE745B0BEBB101642B099610A055F3AAE0AA0FFA7E00145L"
/** @} */
#endif

#if defined(EP_ORDIN) && FP_PRIME == 606
/**
 * Parameters for a pairing-friendly prime curve over a quadratic extension.
 */
/** @{ */
#define BN_P606_X1		"23CB176534606B0F53EBE5BBF2B88BCA5CE98168EF57704D5B84B245E77DBAD4C9C868440630B5A128BF0EF166F18B39341E36B8B96A1E98E4713E0002201A0C349116D8F464BF93D5D9935A"
#define BN_P606_X0		"B607C11B3611825C9F2EA45C75E035DAFB985D3E7F77579A4FBBD43373A0BEAB152796126501080D7344D32C05522F810B01953A5DF1EBC0FD7774121C492AEB690291A77C750E78CDF914"
#define BN_P606_Y1		"18D0630A25B54EB265C33BC0F81BC42A485E3AF1035088B9BED9BFF7D0D0057C813B7745C73F6DE172D83594E83587D5C1C54135353D5411265F21BD3E95E808224D25A882E03E90E899930C"
#define BN_P606_Y0		"AD89E0CCD808E1B4B141A5F2D4760F286AD5B21C15D8842EC51A03561524A8B992E7C6770B2052DA0C40FC59A2C6BE484692AC6A3F8138AEC69AA9FD950638219F58B6773BF8594BA38D327"
#define BN_P606_R		"50FFFFFFFFFFFAF00000000000237000000A2797723FFFFF71D5B16260000354FDDDC98E94E794B1C2305104B5F69C7EDD6AE8C69C88FF3A782863989A07EBD723809EF3E1FC78B2BA4931EC944DF6AFEFF23AD0BDD1F7D1F0A11148BB36221A5398F398F3127A1CD0E6765273490861F1A99539DE71A078310AF62233B9D502F5C033015B6A1A147F9767E0000052EB335DB74128F0545L"
/** @} */
#endif

#if defined(EP_ORDIN) && FP_PRIME == 638
/**
 * Parameters for a pairing-friendly prime curve over a quadratic extension.
 */
/** @{ */
#define BN_P638_X1		"2382CFC1019E7DEB5C40C361BEBC7857F47E66CAF9CFA5D110D62FADBDDCA7743B5EC6E906F78AC25C7BB927AB6832E1DB2073209013C10787A7C2095F53370C7EE162284BF78A45F87838CFE4E44D33"
#define BN_P638_X0		"DF452D4AE6902EF9617BC439FCED97A2C0BA4A5EF92D87D3DE4E509FD645EFCE7A238652793EE9C48D624F9D9B1778EC07DFFCDC3FE84153608E689F4CE9484D0FFD865ECAC8E5690E60F0D019E830"
#define BN_P638_Y1		"23E868EA4FED1FDE4E46880C455D69165FC4EC273312D64B0CF05DB9E2D06B6B06423513A9C94834293D058490EC1C21CC8E5DC8A357C6423675F1DBC77CD5344EA990B9053B8AEB0D5DA5E2330C0848"
#define BN_P638_Y0		"147A2C89963300DD977ACD795A3699B474266788CE475C347E3775C892D720E42A1E12645F42F05BD4696F55864CBD70A7084CDE2A6E85404A1E46045504F6B92B83446021C32AC5CAB4D00A87287067"
#define BN_P638_R		"23FFFFFDC000000D7FFFFFB8000001D3FFFFF942D000165E3FFF94870000D52FFFFDD0E00008DE55600086550021E555FFFFF54FFFF4EAC000000049800154D9FFFFFFFFFFFFEDA00000000000000061"
/** @} */
#endif

/**
 * The generator of the elliptic curve.
 */
static ep2_st curve_g;

/**
 * The first coordinate of the generator.
 */
static fp2_st curve_gx;

/**
 * The second coordinate of the generator.
 */
static fp2_st curve_gy;

/**
 * The third coordinate of the generator.
 */
static fp2_st curve_gz;

/**
 * The order of the group of points in the elliptic curve.
 */
static bn_st curve_r;

/**
 * Flag that stores if the configured prime elliptic curve is twisted.
 */
static int curve_is_twist;

#if EP_ADD == BASIC || defined(EP_MIXED) || !defined(STRIP)

/**
 * Adds two points represented in affine coordinates on an ordinary prime
 * elliptic curve.
 *
 * @param r					- the result.
 * @param s					- the resulting slope.
 * @param p					- the first point to add.
 * @param q					- the second point to add.
 */
static void ep2_add_basic_impl(ep2_t r, fp2_t s, ep2_t p, ep2_t q) {
	fp2_t t0, t1, t2;

	fp2_null(t0);
	fp2_null(t1);
	fp2_null(t2);

	TRY {
		fp2_new(t0);
		fp2_new(t1);
		fp2_new(t2);

		/* t0 = x2 - x1. */
		fp2_sub(t0, q->x, p->x);
		/* t1 = y2 - y1. */
		fp2_sub(t1, q->y, p->y);

		/* If t0 is zero. */
		if (fp2_is_zero(t0)) {
			if (fp2_is_zero(t1)) {
				/* If t1 is zero, q = p, should have doubled. */
				ep2_dbl_basic(r, p);
			} else {
				/* If t1 is not zero and t0 is zero, q = -p and r = infty. */
				ep2_set_infty(r);
			}
		} else {

			/* t2 = 1/(x2 - x1). */
			fp2_inv(t2, t0);
			/* t2 = lambda = (y2 - y1)/(x2 - x1). */
			fp2_mul(t2, t1, t2);

			/* x3 = lambda^2 - x2 - x1. */
			fp2_sqr(t1, t2);
			fp2_sub(t0, t1, p->x);
			fp2_sub(t0, t0, q->x);

			/* y3 = lambda * (x1 - x3) - y1. */
			fp2_sub(t1, p->x, t0);
			fp2_mul(t1, t2, t1);
			fp2_sub(r->y, t1, p->y);

			fp2_copy(r->x, t0);
			fp2_copy(r->z, p->z);

			if (s != NULL) {
				fp2_copy(s, t2);
			}

			r->norm = 1;
		}
	}
	CATCH_ANY {
		THROW(ERR_CAUGHT);
	}
	FINALLY {
		fp2_free(t0);
		fp2_free(t1);
		fp2_free(t2);
	}
}

/**
 * Doubles a point represented in affine coordinates on an ordinary prime
 * elliptic curve.
 *
 * @param r					- the result.
 * @param s					- the resulting slope.
 * @param p					- the point to double.
 */
static void ep2_dbl_basic_impl(ep2_t r, fp2_t s, fp2_t e, ep2_t p) {
	fp2_t t0, t1, t2;

	fp2_null(t0);
	fp2_null(t1);
	fp2_null(t2);

	TRY {
		//printf("start doubling");
		//ep2_print(p);
		fp2_new(t0);
		fp2_new(t1);
		fp2_new(t2);

		/* t0 = 1/(2 * y1). */
		fp2_dbl(t0, p->y);
		//fp2_print(t0);
		fp2_inv(t0, t0);
		//fp2_print(t0);
		//printf("\n");

		/* t1 = 3 * x1^2 + a. */
		fp2_sqr(t1, p->x);
		fp2_copy(t2, t1);
		fp2_dbl(t1, t1);
		fp2_add(t1, t1, t2);

		if (ep2_curve_is_twist()) {
			switch (ep_curve_opt_a()) {
				case OPT_ZERO:
					break;
				case OPT_ONE:
					fp_set_dig(t2[0], 1);
					fp2_mul_art(t2, t2);
					fp2_mul_art(t2, t2);
					fp2_add(t1, t1, t2);
					break;
				case OPT_DIGIT:
					fp_set_dig(t2[0], ep_curve_get_a()[0]);
					fp2_mul_art(t2, t2);
					fp2_mul_art(t2, t2);
					fp2_add(t1, t1, t2);
					break;
				default:
					fp_copy(t2[0], ep_curve_get_a());
					fp_zero(t2[1]);
					fp2_mul_art(t2, t2);
					fp2_mul_art(t2, t2);
					fp2_add(t1, t1, t2);
					break;
			}
		}

		/* t1 = (3 * x1^2 + a)/(2 * y1). */
		fp2_mul(t1, t1, t0);

		if (s != NULL) {
			fp2_copy(s, t1);
		}

		/* t2 = t1^2. */
		fp2_sqr(t2, t1);

		/* x3 = t1^2 - 2 * x1. */
		fp2_dbl(t0, p->x);
		fp2_sub(t0, t2, t0);

		/* y3 = t1 * (x1 - x3) - y1. */
		fp2_sub(t2, p->x, t0);
		fp2_mul(t1, t1, t2);

		fp2_sub(r->y, t1, p->y);

		fp2_copy(r->x, t0);
		fp2_copy(r->z, p->z);

		r->norm = 1;
	}
	CATCH_ANY {
		THROW(ERR_CAUGHT);
	}
	FINALLY {
		fp2_free(t0);
		fp2_free(t1);
		fp2_free(t2);
	}
}

#endif /* EP_ADD == BASIC */

#if EP_ADD == PROJC || defined(EP_MIXED) || !defined(STRIP)

/**
 * Adds two points represented in projective coordinates on an ordinary prime
 * elliptic curve.
 *
 * @param r					- the result.
 * @param s					- the resulting slope.
 * @param p					- the first point to add.
 * @param q					- the second point to add.
 */
static void ep2_add_projc_impl(ep2_t r, fp2_t s, ep2_t p, ep2_t q) {
	fp2_t t0, t1, t3, t4;

	fp2_null(t0);
	fp2_null(t1);
	fp2_null(t3);
	fp2_null(t4);

	TRY {
		fp2_new(t0);
		fp2_new(t1);
		fp2_new(t3);
		fp2_new(t4);

		fp2_copy(t3, q->x);
		fp2_copy(t4, q->y);
		fp2_sqr(t1, p->z);
		fp2_mul(t3, t3, t1);
		fp2_mul(t1, t1, p->z);
		fp2_mul(t4, t4, t1);

		fp2_sub(t3, t3, p->x);
		fp2_sub(t0, t4, p->y);
		fp2_mul(r->z, p->z, t3);
		fp2_sqr(t1, t3);
		fp2_mul(t4, t1, t3);
		fp2_mul(t1, t1, p->x);
		fp2_copy(t3, t1);
		fp2_add(t3, t3, t3);
		fp2_sqr(r->x, t0);
		fp2_sub(r->x, r->x, t3);
		fp2_sub(r->x, r->x, t4);
		fp2_sub(t1, t1, r->x);
		fp2_mul(t1, t1, t0);
		fp2_mul(t4, t4, p->y);
		fp2_sub(r->y, t1, t4);

		if (s != NULL) {
			fp2_copy(s, t0);
		}

		r->norm = 0;
	}
	CATCH_ANY {
		THROW(ERR_CAUGHT);
	}
	FINALLY {
		fp2_free(t0);
		fp2_free(t1);
		fp2_free(t3);
		fp2_free(t4);
	}
}

/**
 * Doubles a point rep2resented in affine coordinates on an ordinary prime
 * elliptic curve.
 *
 * @param r					- the result.
 * @param p					- the point to double.
 */
static void ep2_dbl_projc_impl(ep2_t r, fp2_t s, fp2_t e, ep2_t p) {
	fp2_t t0, t1, t2, t3;

	fp2_null(t0);
	fp2_null(t1);
	fp2_null(t2);
	fp2_null(t3);

	TRY {
		fp2_new(t0);
		fp2_new(t1);
		fp2_new(t2);
		fp2_new(t3);

		fp2_sqr(t0, p->x);
		fp2_add(t2, t0, t0);
		fp2_add(t0, t2, t0);

		fp2_sqr(t3, p->y);
		fp2_mul(t1, t3, p->x);
		fp2_add(t1, t1, t1);
		fp2_add(t1, t1, t1);
		fp2_sqr(r->x, t0);
		fp2_add(t2, t1, t1);
		fp2_sub(r->x, r->x, t2);
		fp2_mul(r->z, p->z, p->y);
		fp2_add(r->z, r->z, r->z);
		fp2_add(t3, t3, t3);
		if (s != NULL) {
			fp2_copy(s, t0);
		}
		if (e != NULL) {
			fp2_copy(e, t3)
		}
		fp2_sqr(t3, t3);
		fp2_add(t3, t3, t3);
		fp2_sub(t1, t1, r->x);
		fp2_mul(r->y, t0, t1);
		fp2_sub(r->y, r->y, t3);

		r->norm = 0;
	}
	CATCH_ANY {
		THROW(ERR_CAUGHT);
	}
	FINALLY {
		fp2_free(t0);
		fp2_free(t1);
		fp2_free(t2);
		fp2_free(t3);
	}
}

#endif /* EP_ADD == BASIC */

#if EP_ADD == PROJC || defined(EP_MIXED)

/**
 * Normalizes a point represented in projective coordinates.
 *
 * @param r			- the result.
 * @param p			- the point to normalize.
 */
void ep2_norm2(ep2_t r, ep2_t p) {
	if (!p->norm) {
		fp2_t t0, t1;

		fp2_null(t0);
		fp2_null(t1);

		TRY {
			fp2_new(t0);
			fp2_new(t1);

			fp2_inv(t1, p->z);
			fp2_mul(r->x, p->x, t1);
			fp2_mul(r->y, p->y, t1);
			fp_zero(r->z[0]);
			fp_zero(r->z[1]);
			fp_set_dig(r->z[0], 1);
		}
		CATCH_ANY {
			THROW(ERR_CAUGHT);
		}
		FINALLY {
			fp2_free(t0);
			fp2_free(t1);
		}
	}

	r->norm = 1;
}

void ep2_norm_impl(ep2_t r, ep2_t p) {
	if (!p->norm) {
		fp2_t t0, t1;

		fp2_null(t0);
		fp2_null(t1);

		TRY {
			fp2_new(t0);
			fp2_new(t1);

			fp2_inv(t1, p->z);
			fp2_sqr(t0, t1);
			fp2_mul(r->x, p->x, t0);
			fp2_mul(t0, t0, t1);
			fp2_mul(r->y, p->y, t0);
			fp_zero(r->z[0]);
			fp_zero(r->z[1]);
		}
		CATCH_ANY {
			THROW(ERR_CAUGHT);
		}
		FINALLY {
			fp2_free(t0);
			fp2_free(t1);
		}
	}

	r->norm = 1;
}

#endif /* EP_ADD == PROJC || EP_MIXED */

/*============================================================================*/
	/* Public definitions                                                         */
/*============================================================================*/

void ep2_curve_init(void) {
#if ALLOC == STATIC
	fp2_new(curve_gx);
	fp2_new(curve_gy);
	fp2_new(curve_gz);
#endif
#if ALLOC == AUTO
	fp2_copy(curve_g.x, curve_gx);
	fp2_copy(curve_g.y, curve_gy);
	fp2_copy(curve_g.z, curve_gz);
#else
	curve_g.x[0] = curve_gx[0];
	curve_g.x[1] = curve_gx[1];
	curve_g.y[0] = curve_gy[0];
	curve_g.y[1] = curve_gy[1];
	curve_g.z[0] = curve_gz[0];
	curve_g.z[1] = curve_gz[1];
#endif
	ep2_set_infty(&curve_g);
	bn_init(&curve_r, FP_DIGS);
}

void ep2_curve_clean(void) {
#if ALLOC == STATIC
	fp2_free(curve_gx);
	fp2_free(curve_gy);
	fp2_free(curve_gz);
#endif
	bn_clean(&curve_r);
}

int ep2_curve_is_twist() {
	return curve_is_twist;
}

void ep2_curve_get_gen(ep2_t g) {
	ep2_copy(g, &curve_g);
}

void ep2_curve_get_ord(bn_t n) {
	if (curve_is_twist) {
		ep_curve_get_ord(n);
		//printf("here");
	} else {
		bn_copy(n, &curve_r);
	}
}

void ep2_curve_set(int twist) {
	int param;
	char *str;
	ep2_t g;
	bn_t r;

	ep2_null(g);
	bn_null(r);

	TRY {
		ep2_new(g);
		bn_new(r);

		param = ep_param_get();

		switch (param) {
#if FP_PRIME == 256
			case BNN_P256:
				fp_read(g->x[0], BNN_P256_X0, strlen(BNN_P256_X0), 16);
				fp_read(g->x[1], BNN_P256_X1, strlen(BNN_P256_X1), 16);
				fp_read(g->y[0], BNN_P256_Y0, strlen(BNN_P256_Y0), 16);
				fp_read(g->y[1], BNN_P256_Y1, strlen(BNN_P256_Y1), 16);
				bn_read_str(r, BNN_P256_R, strlen(BNN_P256_R), 16);
				break;
#elif FP_PRIME ==446
			case BN_P446:
				fp_read(g->x[0], BN_P446_X0, strlen(BN_P446_X0), 16);
				fp_read(g->x[1], BN_P446_X1, strlen(BN_P446_X1), 16);
				fp_read(g->y[0], BN_P446_Y0, strlen(BN_P446_Y0), 16);
				fp_read(g->y[1], BN_P446_Y1, strlen(BN_P446_Y1), 16);
				bn_read_str(r, BN_P446_R, strlen(BN_P446_R), 16);
				break;
#elif FP_PRIME ==286
			case BN_P286:
				fp_read(g->x[0], BN_P286_X0, strlen(BN_P286_X0), 16);
				fp_read(g->x[1], BN_P286_X1, strlen(BN_P286_X1), 16);
				fp_read(g->y[0], BN_P286_Y0, strlen(BN_P286_Y0), 16);
				fp_read(g->y[1], BN_P286_Y1, strlen(BN_P286_Y1), 16);
				bn_read_str(r, BN_P286_R, strlen(BN_P286_R), 16);
				break;
#elif FP_PRIME ==478
			case BN_P478:
				fp_read(g->x[0], BN_P478_X0, strlen(BN_P478_X0), 16);
				fp_read(g->x[1], BN_P478_X1, strlen(BN_P478_X1), 16);
				fp_read(g->y[0], BN_P478_Y0, strlen(BN_P478_Y0), 16);
				fp_read(g->y[1], BN_P478_Y1, strlen(BN_P478_Y1), 16);
				bn_read_str(r, BN_P478_R, strlen(BN_P478_R), 16);
				break;
#elif FP_PRIME ==606
			case BN_P606:
				fp_read(g->x[0], BN_P606_X0, strlen(BN_P606_X0), 16);
				fp_read(g->x[1], BN_P606_X1, strlen(BN_P606_X1), 16);
				fp_read(g->y[0], BN_P606_Y0, strlen(BN_P606_Y0), 16);
				fp_read(g->y[1], BN_P606_Y1, strlen(BN_P606_Y1), 16);
				bn_read_str(r, BN_P606_R, strlen(BN_P606_R), 16);
				break;
#elif FP_PRIME ==638
			case BN_P638:
				fp_read(g->x[0], BN_P638_X0, strlen(BN_P638_X0), 16);
				fp_read(g->x[1], BN_P638_X1, strlen(BN_P638_X1), 16);
				fp_read(g->y[0], BN_P638_Y0, strlen(BN_P638_Y0), 16);
				fp_read(g->y[1], BN_P638_Y1, strlen(BN_P638_Y1), 16);
				bn_read_str(r, BN_P638_R, strlen(BN_P638_R), 16);
				break;
#endif
			default:
				(void)str;
				THROW(ERR_INVALID);
				break;
		}

		fp2_zero(g->z);
		fp_set_dig(g->z[0], 1);
		g->norm = 1;

		ep2_copy(&curve_g, g);
		bn_copy(&curve_r, r);

		curve_is_twist = twist;
	}
	CATCH_ANY {
		THROW(ERR_CAUGHT);
	}
	FINALLY {
		ep2_free(g);
		bn_free(r);
	}
}

int ep2_is_infty(ep2_t p) {
	return (fp2_is_zero(p->z) == 1);
}

void ep2_set_infty(ep2_t p) {
	fp2_zero(p->x);
	fp2_zero(p->y);
	fp2_zero(p->z);
}

void ep2_copy(ep2_t r, ep2_t p) {
	fp2_copy(r->x, p->x);
	fp2_copy(r->y, p->y);
	fp2_copy(r->z, p->z);
	r->norm = p->norm;
}

int ep2_cmp(ep2_t p, ep2_t q) {
	if (fp2_cmp(p->x, q->x) != CMP_EQ) {
		return CMP_NE;
	}

	if (fp2_cmp(p->y, q->y) != CMP_EQ) {
		return CMP_NE;
	}

	if (fp2_cmp(p->z, q->z) != CMP_EQ) {
		return CMP_NE;
	}

	return CMP_EQ;
}

void ep2_rand(ep2_t p) {
	bn_t n, k;
	ep2_t gen;

	bn_null(k);
	bn_null(n);
	ep2_null(gen);

	TRY {
		bn_new(k);
		bn_new(n);
		ep2_new(gen);

		ep2_curve_get_ord(n);

		bn_rand(k, BN_POS, bn_bits(n));
		bn_mod(k, k, n);

		ep2_curve_get_gen(gen);
		ep2_mul(p, gen, k);
	}
	CATCH_ANY {
		THROW(ERR_CAUGHT);
	}
	FINALLY {
		bn_free(k);
		bn_free(n);
		ep2_free(gen);
	}
}

void ep2_print(ep2_t p) {
	fp2_print(p->x);
	fp2_print(p->y);
	fp2_print(p->z);
}

#if EP_ADD == BASIC || defined(EP_MIXED) || !defined(STRIP)

void ep2_neg_basic(ep2_t r, ep2_t p) {
	if (ep2_is_infty(p)) {
		ep2_set_infty(r);
		return;
	}

	if (r != p) {
		fp2_copy(r->x, p->x);
		fp2_copy(r->z, p->z);
	}

	fp2_neg(r->y, p->y);

	r->norm = 1;
}

void ep2_add_basic(ep2_t r, ep2_t p, ep2_t q) {
	if (ep2_is_infty(p)) {
		ep2_copy(r, q);
		return;
	}

	if (ep2_is_infty(q)) {
		ep2_copy(r, p);
		return;
	}

	ep2_add_basic_impl(r, NULL, p, q);
}

void ep2_add_slp_basic(ep2_t r, fp2_t s, ep2_t p, ep2_t q) {
	if (ep2_is_infty(p)) {
		ep2_copy(r, q);
		return;
	}

	if (ep2_is_infty(q)) {
		ep2_copy(r, p);
		return;
	}

	ep2_add_basic_impl(r, s, p, q);
}

void ep2_sub_basic(ep2_t r, ep2_t p, ep2_t q) {
	ep2_t t;

	ep2_null(t);

	if (p == q) {
		ep2_set_infty(r);
		return;
	}

	TRY {
		ep2_new(t);

		ep2_neg_basic(t, q);
		ep2_add_basic(r, p, t);

		r->norm = 1;
	}
	CATCH_ANY {
		THROW(ERR_CAUGHT);
	}
	FINALLY {
		ep2_free(t);
	}
}

void ep2_dbl_basic(ep2_t r, ep2_t p) {
	if (ep2_is_infty(p)) {
		ep2_set_infty(r);
		return;
	}

	ep2_dbl_basic_impl(r, NULL, NULL, p);
}

void ep2_dbl_slp_basic(ep2_t r, fp2_t s, fp2_t e, ep2_t p) {
	if (ep2_is_infty(p)) {
		ep2_set_infty(r);
		return;
	}

	ep2_dbl_basic_impl(r, s, e, p);
}

#endif

#if EP_ADD == PROJC || defined(EP_MIXED) || !defined(STRIP)

void ep2_neg_projc(ep2_t r, ep2_t p) {
	if (ep2_is_infty(p)) {
		ep2_set_infty(r);
		return;
	}

	if (p->norm) {
		ep2_neg_basic(r, p);
		return;
	}

	if (r != p) {
		fp2_copy(r->x, p->x);
		fp2_copy(r->z, p->z);
	}

	fp2_neg(r->y, p->y);

	r->norm = 0;
}

void ep2_add_projc(ep2_t r, ep2_t p, ep2_t q) {
	if (ep2_is_infty(p)) {
		ep2_copy(r, q);
		return;
	}

	if (ep2_is_infty(q)) {
		ep2_copy(r, p);
		return;
	}

	/*
	 * TODO: Change this to ep2_add_proj_impl and sort the large code problem
	 * with add_projc functions.
	 */
	ep2_add_basic_impl(r, NULL, p, q);
}

void ep2_add_slp_projc(ep2_t r, fp2_t s, ep2_t p, ep2_t q) {
	if (ep2_is_infty(p)) {
		ep2_copy(r, q);
		return;
	}

	if (ep2_is_infty(q)) {
		ep2_copy(r, p);
		return;
	}

	ep2_add_projc_impl(r, s, p, q);
}

void ep2_sub_projc(ep2_t r, ep2_t p, ep2_t q) {
	ep2_t t;

	ep2_null(t);

	if (p == q) {
		ep2_set_infty(r);
		return;
	}

	TRY {
		ep2_new(t);

		ep2_neg_projc(t, q);
		ep2_add_projc(r, p, t);
	}
	CATCH_ANY {
		THROW(ERR_CAUGHT);
	}
	FINALLY {
		ep2_free(t);
	}
}

void ep2_dbl_projc(ep2_t r, ep2_t p) {
	if (ep2_is_infty(p)) {
		ep2_set_infty(r);
		return;
	}

	ep2_dbl_projc_impl(r, NULL, NULL, p);
}

void ep2_dbl_slp_projc(ep2_t r, fp2_t s, fp2_t e, ep2_t p) {
	if (ep2_is_infty(p)) {
		ep2_set_infty(r);
		return;
	}

	ep2_dbl_projc_impl(r, s, e, p);
}

#endif

void ep2_mul(ep2_t r, ep2_t p, bn_t k) {
	int i, l;
	ep2_t t;

	ep2_null(t);
	TRY {
		ep2_new(t);
		l = bn_bits(k);

		if (bn_test_bit(k, l - 1)) {
			ep2_copy(t, p);
		} else {
			ep2_set_infty(t);
		}

		for (i = l - 2; i >= 0; i--) {
			ep2_dbl(t, t);
			if (bn_test_bit(k, i)) {
				ep2_add(t, t, p);
			}
		}

		ep2_copy(r, t);
		ep2_norm(r, r);
	}
	CATCH_ANY {
		THROW(ERR_CAUGHT);
	}
	FINALLY {
		ep2_free(t);
	}
}

/*void ep2_mul(ep2_t r, ep2_t p, bn_t k) {
	int i, l;
	ep2_t t, s;

	ep2_null(t);
	ep2_null(s);
	TRY {
		ep2_new(t);
		ep2_new(s);
		l = bn_bits(k);
		//printf("bits of order are %d \n", l);

		ep2_copy(t, p);
		if (bn_test_bit(k, 0)) {
			ep2_copy(s, p);
			//printf("%d bit is 1 \n", l);
		} else {
			ep2_set_infty(s);
			//printf("%d bit is 0 \n", l);
		}
		//ep2_print(t);

		for (i = 1; i <= l-1; i++) {
			ep2_dbl(t, t);
			//printf("%d \n", i+1);
			//ep2_print(t);
			if (bn_test_bit(k, i)) {
				ep2_add(s, t, s);
				//printf("%d bit is 1 \n", i+1);
				//ep2_print(s);
			}
		}

		ep2_copy(r, s);
		ep2_norm(r, r);
		//ep_print(t);
	}
	CATCH_ANY {
		THROW(ERR_CAUGHT);
	}
	FINALLY {
		ep2_free(t);
		ep2_free(s);
	}
}*/

void ep2_norm(ep2_t r, ep2_t p) {
	if (ep2_is_infty(p)) {
		ep2_set_infty(r);
		return;
	}

	if (p->norm) {
		/* If the point is represented in affine coordinates, we just copy it. */
		ep2_copy(r, p);
	}
#if EP_ADD == PROJC || !defined(STRIP)
	ep2_norm_impl(r, p);
#endif
}

/*
void ep2_frb(ep2_t r, ep2_t p) {
	fp2_t t;

	fp2_null(t);

	TRY {
		fp2_new(t);

		fp2_const_get(t);

		fp2_frb(r->x, p->x);
		fp2_frb(r->y, p->y);
		fp2_mul(r->y, r->y, t);
		fp2_sqr(t, t);
		fp2_mul(r->x, r->x, t);
		fp2_mul(r->y, r->y, t);
	} CATCH_ANY {
		THROW(ERR_CAUGHT);
	} FINALLY {
		fp2_free(t);
	}
}
*/

void ep2_frb(ep2_t r, ep2_t p) {

        fp2_frb(r->x, p->x);
	fp2_frb(r->y, p->y);
	fp2_mul_frb(r->x, r->x, 2);
	fp2_mul_frb(r->y, r->y, 3);


	r->norm = 1;
	fp_set_dig(r->z[0], 1);
	fp_zero(r->z[1]);
}

void ep2_frb_sqr(ep2_t r, ep2_t p) {
	fp2_mul_frb_sqr(r->x, p->x, 2);
	fp2_mul_frb_sqr(r->y, p->y, 3);

	r->norm = 1;
	fp_set_dig(r->z[0], 1);
	fp_zero(r->z[1]);
}

void ep2_frbm(ep2_t r, ep2_t p) {

	fp2_mul_frb(r->x, p->x, 8);
	fp2_frb(r->x, r->x);
	fp2_mul_frb(r->y, p->y, 8);
	fp2_frb(r->y, r->y);

	fp2_mul_frb(r->x, r->x, 6);
	fp2_mul_frb(r->y, r->y, 7);

	r->norm = 1;
	fp_set_dig(r->z[0], 1);
	fp_zero(r->z[1]);
}

void ep2_mul_gen(ep2_t r, bn_t k) {
	ep2_t gen;

	ep2_null(gen);

	TRY {
		ep2_new(gen);
		ep2_curve_get_gen(gen);
		ep2_mul(r, gen, k);
	}
	CATCH_ANY {
		THROW(ERR_CAUGHT);
	}
	FINALLY {
		ep2_free(gen);
	}
}

void ep2_map(ep2_t p, unsigned char *msg, int len) {
	bn_t n, k;
	unsigned char digest[MD_LEN];

	bn_null(n);
	bn_null(k);

	TRY {
		bn_new(n);
		bn_new(k);

		ep2_curve_get_ord(n);

		md_map(digest, msg, len);
		bn_read_bin(k, digest, MD_LEN, BN_POS);
		bn_mod(k, k, n);

		ep2_curve_get_ord(n);

		ep2_mul_gen(p, k);
	} CATCH_ANY {
		THROW(ERR_CAUGHT);
	} FINALLY {
		bn_free(n);
		bn_free(k);
	}
}
