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
 * Implementation of the low-level prime field multiplication functions.
 *
 * @version $Id: relic_fp_mul_low.c 161 2009-11-07 17:18:26Z dfaranha $
 * @ingroup bn
 */

#include "relic_fp.h"
#include "relic_fp_low.h"

#define __MULTIPLICATION_IN_ASM__	   1
//#define __FP_MUL__	   COMBA

/*============================================================================*/
/* Private definitions                                                         */
/*============================================================================*/
void fp_muln_low_asm_comba_256(dig_t *c, dig_t *a, dig_t *b){
	asm(
			//256-bit load
			//when there is no carry.

			//q0-q1 operand a
			//q2-q3 operand b
			//q4-q7
			//q15   m_inverse
			//q14-q15 m

			"vldmia %1,  {q0-q1} \n\t"		//operand a load
			"vldmia %2,  {q2-q3} \n\t"		//operand b load



			"vtrn.32 q0, q1 \n\t "			//shuffle 6 2 4 0
											//shuffle 7 3 5 1


			//

			//q0-q1 operand a
			//q2-q9 operand b

			//"vstmia %3, {q4-q5} \n\t"

			//q0,q1 operand a
			//q2,q3 operand b
			//q14-q15    m_inverse + m


			//step 1
			"vmull.u32 q13, d0, d4[0] \n\t"	//A4A0 X B0B0
			"vmull.u32 q12, d2, d4[0] \n\t"	//A5A1 X B0B0
			"vmull.u32 q11, d1, d4[0] \n\t"	//A6A2 X B0B0
			"vmull.u32 q10, d3, d4[0] \n\t"	//A7A3 X B0B0


			"veor q5,q5,q5 \n\t"
			"veor q6,q6,q6 \n\t"
			"veor q7,q7,q7 \n\t"
			"veor q8,q8,q8 \n\t"


			"vtrn.32 q13, q5 \n\t"
			"vtrn.32 q12, q6 \n\t"
			"vtrn.32 q11, q7 \n\t"
			"vtrn.32 q10, q8 \n\t"

			"vadd.i64  q12, q12, q5 \n\t"		//q
			"vadd.i64  q11, q11, q6 \n\t"		//q
			"vadd.i64  q10, q10, q7 \n\t"		//q

			"vqadd.u64 d16, d16, d27 \n\t"		//d

			"vext.8 d28, d28, d26, #4 \n\t"


			//step 2

			"vmlal.u32 q12, d0, d4[1] \n\t"	//A4A0 X B0B0
			"vmlal.u32 q11, d2, d4[1] \n\t"	//A5A1 X B0B0
			"vmlal.u32 q10, d1, d4[1] \n\t"	//A6A2 X B0B0
			"vmlal.u32 q8 , d3, d4[1] \n\t"	//A7A3 X B0B0

			"veor q5,q5,q5 \n\t"
			"veor q6,q6,q6 \n\t"
			"veor q7,q7,q7 \n\t"
			"veor q9,q9,q9 \n\t"


			"vtrn.32 q12, q5 \n\t"
			"vtrn.32 q11, q6 \n\t"
			"vtrn.32 q10, q7 \n\t"
			"vtrn.32 q8 , q9 \n\t"

			"vadd.i64  q11, q11, q5 \n\t"		//q
			"vadd.i64  q10, q10, q6 \n\t"		//q
			"vadd.i64  q8 , q8 , q7 \n\t"		//q
			"vqadd.u64 d18, d18, d25 \n\t"		//d

			"vext.8 d28, d28, d24, #4 \n\t"

			//step 3

			"vmlal.u32 q11, d0, d5[0] \n\t"	//A4A0 X B0B0
			"vmlal.u32 q10, d2, d5[0] \n\t"	//A5A1 X B0B0
			"vmlal.u32 q8,  d1, d5[0] \n\t"	//A6A2 X B0B0
			"vmlal.u32 q9,  d3, d5[0] \n\t"	//A7A3 X B0B0

			"veor q5,q5,q5 \n\t"
			"veor q6,q6,q6 \n\t"
			"veor q7,q7,q7 \n\t"
			"veor q12,q12,q12 \n\t"

			"vtrn.32 q11, q5  \n\t"
			"vtrn.32 q10, q6  \n\t"
			"vtrn.32 q8,  q7  \n\t"
			"vtrn.32 q9,  q12 \n\t"

			"vadd.i64  q10, q10, q5 \n\t"		//q
			"vadd.i64  q8 , q8 , q6 \n\t"		//q
			"vadd.i64  q9,  q9, q7 \n\t"		//q
			"vqadd.u64 d24, d24, d23 \n\t"		//d

			"vext.8 d29, d29, d22, #4 \n\t"

			//step 4

			"vmlal.u32 q10, d0, d5[1] \n\t"	//A4A0 X B0B0
			"vmlal.u32 q8,  d2, d5[1] \n\t"	//A5A1 X B0B0
			"vmlal.u32 q9,  d1, d5[1] \n\t"	//A6A2 X B0B0
			"vmlal.u32 q12, d3, d5[1] \n\t"	//A7A3 X B0B0

			"veor q5,q5,q5 \n\t"
			"veor q6,q6,q6 \n\t"
			"veor q7,q7,q7 \n\t"
			"veor q11,q11,q11 \n\t"

			"vtrn.32 q10, q5 \n\t"
			"vtrn.32 q8 , q6 \n\t"
			"vtrn.32 q9,  q7 \n\t"
			"vtrn.32 q12, q11 \n\t"

			"vadd.i64  q8,  q8, q5 \n\t"		//q
			"vadd.i64  q9,  q9, q6 \n\t"		//q
			"vadd.i64  q12, q12, q7 \n\t"		//q
			"vqadd.u64 d22, d22, d21 \n\t"		//d

			"vext.8 d29, d29, d20, #4 \n\t"

			//step 5

			"vmlal.u32 q8 , d0, d6[0] \n\t"	//A4A0 X B0B0
			"vmlal.u32 q9 , d2, d6[0] \n\t"	//A5A1 X B0B0
			"vmlal.u32 q12, d1, d6[0] \n\t"	//A6A2 X B0B0
			"vmlal.u32 q11, d3, d6[0] \n\t"	//A7A3 X B0B0

			"veor q5,q5,q5 \n\t"
			"veor q6,q6,q6 \n\t"
			"veor q7,q7,q7 \n\t"
			"veor q10,q10,q10 \n\t"

			"vtrn.32 q8,  q5 \n\t"
			"vtrn.32 q9,  q6 \n\t"
			"vtrn.32 q12, q7 \n\t"
			"vtrn.32 q11, q10 \n\t"

			"vadd.i64  q9, q9, q5 \n\t"		//q
			"vadd.i64  q12, q12, q6 \n\t"		//q
			"vadd.i64  q11, q11, q7 \n\t"		//q
			"vqadd.u64 d20, d20, d17 \n\t"		//d

			"vext.8 d30, d30, d16, #4 \n\t"


			//step 6

			"vmlal.u32 q9 , d0, d6[1] \n\t"	//A4A0 X B0B0
			"vmlal.u32 q12, d2, d6[1] \n\t"	//A5A1 X B0B0
			"vmlal.u32 q11, d1, d6[1] \n\t"	//A6A2 X B0B0
			"vmlal.u32 q10, d3, d6[1] \n\t"	//A7A3 X B0B0

			"veor q5,q5,q5 \n\t"
			"veor q6,q6,q6 \n\t"
			"veor q7,q7,q7 \n\t"
			"veor q8,q8,q8 \n\t"

			"vtrn.32 q9 , q5 \n\t"
			"vtrn.32 q12, q6 \n\t"
			"vtrn.32 q11, q7 \n\t"
			"vtrn.32 q10, q8 \n\t"

			"vadd.i64  q12, q12, q5 \n\t"		//q
			"vadd.i64  q11, q11, q6 \n\t"		//q
			"vadd.i64  q10, q10, q7 \n\t"		//q
			"vqadd.u64 d16, d16, d19 \n\t"		//d

			"vext.8 d30, d30, d18, #4 \n\t"

			//step 7

			"vmlal.u32 q12, d0, d7[0] \n\t"	//A4A0 X B0B0
			"vmlal.u32 q11, d2, d7[0] \n\t"	//A5A1 X B0B0
			"vmlal.u32 q10, d1, d7[0] \n\t"	//A6A2 X B0B0
			"vmlal.u32 q8 , d3, d7[0] \n\t"	//A7A3 X B0B0

			"veor q5,q5,q5 \n\t"
			"veor q6,q6,q6 \n\t"
			"veor q7,q7,q7 \n\t"
			"veor q9,q9,q9 \n\t"

			"vtrn.32 q12, q5 \n\t"
			"vtrn.32 q11, q6 \n\t"
			"vtrn.32 q10, q7 \n\t"
			"vtrn.32 q8 , q9 \n\t"

			"vadd.i64  q11, q11, q5 \n\t"		//q
			"vadd.i64  q10, q10, q6 \n\t"		//q
			"vadd.i64  q8 , q8 , q7 \n\t"		//q
			"vqadd.u64 d18, d18, d25 \n\t"		//d

			"vext.8 d31, d31, d24, #4 \n\t"








			//step 8

			"vmlal.u32 q11, d0, d7[1] \n\t"	//A4A0 X B0B0
			"vmlal.u32 q10, d2, d7[1] \n\t"	//A5A1 X B0B0
			"vmlal.u32 q8 , d1, d7[1] \n\t"	//A6A2 X B0B0
			"vmlal.u32 q9, d3, d7[1] \n\t"	//A7A3 X B0B0

			"veor q5,q5,q5 \n\t"
			"veor q6,q6,q6 \n\t"
			"veor q7,q7,q7 \n\t"
			"veor q13,q13,q13 \n\t"

			"vtrn.32 q11, q5 \n\t"
			"vtrn.32 q10, q6 \n\t"
			"vtrn.32 q8,  q7 \n\t"
			"vtrn.32 q9,  q13 \n\t"

			"vadd.i64  q10, q10, q5 \n\t"		//q
			"vadd.i64  q8 , q8 , q6 \n\t"		//q
			"vadd.i64  q9 , q9 , q7 \n\t"		//q
			"vqadd.u64 d26, d26, d23 \n\t"		//d

			"vext.8 d31, d31, d22, #4 \n\t"





			//final
			//"vmov d28, d20 \n\t"		//d	q14, q15
			//"vmov d30, d21 \n\t"		//d	q14, q15

			//"vldmia %3,  {q14-q15} \n\t"	//m_inverse + operand m load


			"veor q5,q5,q5 \n\t"
			"veor q6,q6,q6 \n\t"
			"veor q7,q7,q7 \n\t"
			"veor q0,q0,q0 \n\t"


			"vtrn.32 d20, d10 \n\t"
			"vqadd.u64 d16, d16, d10 \n\t"
			"vext.8 d2, d2, d20, #4 \n\t"

			"vtrn.32 d16, d11 \n\t"
			"vqadd.u64 d18, d18, d11 \n\t"
			"vext.8 d2, d2, d16, #4 \n\t"


			"vtrn.32 d18, d12 \n\t"
			"vqadd.u64 d26, d26, d12 \n\t"
			"vext.8 d3, d3, d18, #4 \n\t"


			"vtrn.32 d26, d13 \n\t"
			"vqadd.u64 d21, d21, d13 \n\t"
			"vext.8 d3, d3, d26, #4 \n\t"


			"vtrn.32 d21, d14 \n\t"
			"vqadd.u64 d17, d17, d14 \n\t"
			"vext.8 d4, d4, d21, #4 \n\t"


			"vtrn.32 d17, d15 \n\t"
			"vqadd.u64 d19, d19, d15 \n\t"
			"vext.8 d4, d4, d17, #4 \n\t"


			"vtrn.32 d19, d0 \n\t"
			"vqadd.u64 d27, d27, d0 \n\t"
			"vext.8 d5, d5, d19, #4 \n\t"
			"vext.8 d5, d5, d27, #4 \n\t"

			/**/





			//"vstmia %0!, {d2} \n\t"
			//"vstmia %0!, {d3} \n\t"
			//"vstmia %0!, {q9} \n\t"
			//"vstmia %0!, {q13} \n\t"
			"vstmia %0!, {q14-q15} \n\t"
			"vstmia %0!, {q1-q2} \n\t"






			:
			:"r"(&c[0]),"r"(&a[0]),"r"(&b[0])
			:"memory","r3","r4","r5","q0","q1","q2","q3","q4","q5","q6","q7","q8","q9","q10","q11","q12","q13","q14","q15");

}

/*============================================================================*/
/* Public definitions                                                         */
/*============================================================================*/

dig_t fp_muladd_low(dig_t *c, dig_t *a, dig_t digit) {
	return mpn_addmul_1(c, a, FP_DIGS, digit);
}

void fp_muln_low(dig_t *c, dig_t *a, dig_t *b) {
#if __MULTIPLICATION_IN_ASM__ == 0
	mpn_mul_n(c, a, b, FP_DIGS);
#elif FP_MUL == COMBA
	fp_muln_low_asm_comba_256(c, a, b); //i think this is the one used df
#endif
}

/*
void fp_muln_low_basic(dig_t *c, dig_t *a, dig_t *b) {
#if FP_PRIME == 256 
	fp_muln_low_asm_basic_256(c, a, b);
#else
//	fp_muln_low_asm_basic_gen(c, a, b);
	fp_muln_low_asm_basic_256(c, a, b); // todo
#endif	
}

void fp_muln_low_comba(dig_t *c, dig_t *a, dig_t *b) {
	fp_muln_low_basic(c, a, b); // todo
}
*/

//key field multiplication function ?
void fp_mulm_low(dig_t *c, dig_t *a, dig_t *b) {
	dig_t align t[2 * FP_DIGS];

	fp_muln_low(t, a, b);
	fp_rdc(c, t);
}
