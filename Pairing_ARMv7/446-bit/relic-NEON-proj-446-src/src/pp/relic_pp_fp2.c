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
 * Implementation of arithmetic in a quadratic extension over a prime field.
 *
 * @version $Id: relic_pp_fp2.c 263 2010-02-05 03:02:42Z conradoplg $
 * @ingroup fp2
 */

#include "relic_core.h"
#include "relic_conf.h"
#include "relic_fp.h"
#include "relic_pp.h"
#include "relic_util.h"
#include "arm_neon.h"

#define USE_NEON_FP2 1
/*============================================================================*/
/* Private definitions                                                        */
/*============================================================================*/

/**
 * Constant used to compute the Frobenius map in higher extensions.
 */
fp2_st const_frb[7];

/**
 * Constant used to compute consecutive Frobenius maps in higher extensions.
 */
static fp_st const_sqr[3];

/*============================================================================*/
/* Public definitions                                                         */
/*============================================================================*/

//24000000 00000000 00240000 0002D000 00000D80 0000021C 00000018 00000000 87000000 0B040000 0057C000 00015C00 00001320 00000067
//0x16E99E74 0xF1B350AF 0x5CC714D0 0x4790B1A2 0x4D242117 0xB3E461B7 0xF334088F 0x00D2E0B4 0xEBB9E391 0xB1D75D5F 0x16174109 0x540379BB 0x6BD6C902 0x2CBCE4A9
dig_t p[] = {0x00000067,  0x00001320, 0x00015C00, 0x0057C000, 0x0B040000, 0x87000000, 0x00000000, 0x00000018, 0x0000021C, 0x00000D80, 0x0002D000, 0x00240000, 0x00000000, 0x24000000};
dig_t pi[]  = {0x2CBCE4A9, 0x6BD6C902, 0x540379BB, 0x16174109, 0xB1D75D5F, 0xEBB9E391, 0x00D2E0B4, 0xF334088F, 0xB3E461B7, 0x4D242117, 0x4790B1A2, 0x5CC714D0, 0xF1B350AF, 0x16E99E74};

int compare_digs(dig_t *A, dig_t *B, int tam){
	int i;
	for(i=tam-1; i>=0; i--){
		if(A[i]<B[i]) return -1;
		if(A[i]>B[i]) return 1;
	}
	return 0;
}

void fp_mul_446_dual_NEON_nr(dig_t *T, dig_t *T2, dig_t *A, dig_t *B, dig_t *A1, dig_t *B1){
	uint32x2_t b[14],a;
	uint64x2_t mul[15], mC, l;
	uint32x2_t t0;
	dig_t B2[]={B[0],B1[0],B[1],B1[1],B[2],B1[2],B[3],B1[3],B[4],B1[4],B[5],B1[5],B[6],B1[6],B[7],B1[7],B[8],B1[8],B[9],B1[9],B[10],B1[10],B[11],B1[11],B[12],B1[12],B[13],B1[13]};
	dig_t A2[]={A[0],A1[0],A[1],A1[1],A[2],A1[2],A[3],A1[3],A[4],A1[4],A[5],A1[5],A[6],A1[6],A[7],A1[7],A[8],A1[8],A[9],A1[9],A[10],A1[10],A[11],A1[11],A[12],A1[12],A[13],A1[13]};
	uint64_t low[]={0x00000000FFFFFFFFUL,0x00000000FFFFFFFFUL};
	b[0]=vld1_u32(B2);
	b[1]=vld1_u32(&B2[2]);
	b[2]=vld1_u32(&B2[4]);
	b[3]=vld1_u32(&B2[6]);
	b[4]=vld1_u32(&B2[8]);
	b[5]=vld1_u32(&B2[10]);
	b[6]=vld1_u32(&B2[12]);
	b[7]=vld1_u32(&B2[14]);
	b[8]=vld1_u32(&B2[16]);
	b[9]=vld1_u32(&B2[18]);
	b[10]=vld1_u32(&B2[20]);
	b[11]=vld1_u32(&B2[22]);
	b[12]=vld1_u32(&B2[24]);
	b[13]=vld1_u32(&B2[26]);
	l=vld1q_u64(low);

//Primera Iteracion
	a=vld1_u32(A2);
	mul[0]=vmull_u32(a,b[0]);
	mC=vshrq_n_u64(mul[0],32);
	mul[1]=vmlal_u32(mC,a,b[1]);
	mC=vshrq_n_u64(mul[1],32);
	mul[2]=vmlal_u32(mC,a,b[2]);
	mC=vshrq_n_u64(mul[2],32);
	mul[3]=vmlal_u32(mC,a,b[3]);
	mC=vshrq_n_u64(mul[3],32);
	mul[4]=vmlal_u32(mC,a,b[4]);
	mC=vshrq_n_u64(mul[4],32);
	mul[5]=vmlal_u32(mC,a,b[5]);
	mC=vshrq_n_u64(mul[5],32);
	mul[6]=vmlal_u32(mC,a,b[6]);
	mC=vshrq_n_u64(mul[6],32);
	mul[7]=vmlal_u32(mC,a,b[7]);
	mC=vshrq_n_u64(mul[7],32);
	mul[8]=vmlal_u32(mC,a,b[8]);
	mC=vshrq_n_u64(mul[8],32);
	mul[9]=vmlal_u32(mC,a,b[9]);
	mC=vshrq_n_u64(mul[9],32);
	mul[10]=vmlal_u32(mC,a,b[10]);
	mC=vshrq_n_u64(mul[10],32);
	mul[11]=vmlal_u32(mC,a,b[11]);
	mC=vshrq_n_u64(mul[11],32);
	mul[12]=vmlal_u32(mC,a,b[12]);
	mC=vshrq_n_u64(mul[12],32);
	mul[13]=vmlal_u32(mC,a,b[13]);
	mul[14]=vshrq_n_u64(mul[13],32);
	t0=vmovn_u64(mul[0]);
	T[0]=vget_lane_u32(t0,0);
	T2[0]=vget_lane_u32(t0,1);

	//Segunta It

	a=vld1_u32(&A2[2]);
	mul[0]=vandq_u64(mul[0],l);
	mul[1]=vandq_u64(mul[1],l);
	mul[2]=vandq_u64(mul[2],l);
	mul[3]=vandq_u64(mul[3],l);
	mul[4]=vandq_u64(mul[4],l);
	mul[5]=vandq_u64(mul[5],l);
	mul[6]=vandq_u64(mul[6],l);
	mul[7]=vandq_u64(mul[7],l);
	mul[8]=vandq_u64(mul[8],l);
	mul[9]=vandq_u64(mul[9],l);
	mul[10]=vandq_u64(mul[10],l);
	mul[11]=vandq_u64(mul[11],l);
	mul[12]=vandq_u64(mul[12],l);
	mul[13]=vandq_u64(mul[13],l);
	
	mul[0]=vmlal_u32(mul[1],a,b[0]);
	mC=vshrq_n_u64(mul[0],32);
	mul[1]=vmlal_u32(mul[2],a,b[1]);
	mul[1]=vaddq_u64(mul[1],mC);
	mC=vshrq_n_u64(mul[1],32);
	mul[2]=vmlal_u32(mul[3],a,b[2]);
	mul[2]=vaddq_u64(mul[2],mC);
	mC=vshrq_n_u64(mul[2],32);
	mul[3]=vmlal_u32(mul[4],a,b[3]);
	mul[3]=vaddq_u64(mul[3],mC);
	mC=vshrq_n_u64(mul[3],32);
	mul[4]=vmlal_u32(mul[5],a,b[4]);
	mul[4]=vaddq_u64(mul[4],mC);
	mC=vshrq_n_u64(mul[4],32);
	mul[5]=vmlal_u32(mul[6],a,b[5]);
	mul[5]=vaddq_u64(mul[5],mC);
	mC=vshrq_n_u64(mul[5],32);
	mul[6]=vmlal_u32(mul[7],a,b[6]);
	mul[6]=vaddq_u64(mul[6],mC);
	mC=vshrq_n_u64(mul[6],32);
	mul[7]=vmlal_u32(mul[8],a,b[7]);
	mul[7]=vaddq_u64(mul[7],mC);
	mC=vshrq_n_u64(mul[7],32);
	mul[8]=vmlal_u32(mul[9],a,b[8]);
	mul[8]=vaddq_u64(mul[8],mC);
	mC=vshrq_n_u64(mul[8],32);
	mul[9]=vmlal_u32(mul[10],a,b[9]);
	mul[9]=vaddq_u64(mul[9],mC);
	mC=vshrq_n_u64(mul[9],32);
	mul[10]=vmlal_u32(mul[11],a,b[10]);
	mul[10]=vaddq_u64(mul[10],mC);
	mC=vshrq_n_u64(mul[10],32);
	mul[11]=vmlal_u32(mul[12],a,b[11]);
	mul[11]=vaddq_u64(mul[11],mC);
	mC=vshrq_n_u64(mul[11],32);
	mul[12]=vmlal_u32(mul[13],a,b[12]);
	mul[12]=vaddq_u64(mul[12],mC);
	mC=vshrq_n_u64(mul[12],32);
	mul[13]=vmlal_u32(mul[14],a,b[13]);
	mul[13]=vaddq_u64(mul[13],mC);
	mul[14]=vshrq_n_u64(mul[13],32);
	t0=vmovn_u64(mul[0]);
	T[1]=vget_lane_u32(t0,0);
	T2[1]=vget_lane_u32(t0,1);
	

	//Tercera It
	a=vld1_u32(&A2[4]);
	mul[0]=vandq_u64(mul[0],l);
	mul[1]=vandq_u64(mul[1],l);
	mul[2]=vandq_u64(mul[2],l);
	mul[3]=vandq_u64(mul[3],l);
	mul[4]=vandq_u64(mul[4],l);
	mul[5]=vandq_u64(mul[5],l);
	mul[6]=vandq_u64(mul[6],l);
	mul[7]=vandq_u64(mul[7],l);
	mul[8]=vandq_u64(mul[8],l);
	mul[9]=vandq_u64(mul[9],l);
	mul[10]=vandq_u64(mul[10],l);
	mul[11]=vandq_u64(mul[11],l);
	mul[12]=vandq_u64(mul[12],l);
	mul[13]=vandq_u64(mul[13],l);
	
	mul[0]=vmlal_u32(mul[1],a,b[0]);
	mC=vshrq_n_u64(mul[0],32);
	mul[1]=vmlal_u32(mul[2],a,b[1]);
	mul[1]=vaddq_u64(mul[1],mC);
	mC=vshrq_n_u64(mul[1],32);
	mul[2]=vmlal_u32(mul[3],a,b[2]);
	mul[2]=vaddq_u64(mul[2],mC);
	mC=vshrq_n_u64(mul[2],32);
	mul[3]=vmlal_u32(mul[4],a,b[3]);
	mul[3]=vaddq_u64(mul[3],mC);
	mC=vshrq_n_u64(mul[3],32);
	mul[4]=vmlal_u32(mul[5],a,b[4]);
	mul[4]=vaddq_u64(mul[4],mC);
	mC=vshrq_n_u64(mul[4],32);
	mul[5]=vmlal_u32(mul[6],a,b[5]);
	mul[5]=vaddq_u64(mul[5],mC);
	mC=vshrq_n_u64(mul[5],32);
	mul[6]=vmlal_u32(mul[7],a,b[6]);
	mul[6]=vaddq_u64(mul[6],mC);
	mC=vshrq_n_u64(mul[6],32);
	mul[7]=vmlal_u32(mul[8],a,b[7]);
	mul[7]=vaddq_u64(mul[7],mC);
	mC=vshrq_n_u64(mul[7],32);
	mul[8]=vmlal_u32(mul[9],a,b[8]);
	mul[8]=vaddq_u64(mul[8],mC);
	mC=vshrq_n_u64(mul[8],32);
	mul[9]=vmlal_u32(mul[10],a,b[9]);
	mul[9]=vaddq_u64(mul[9],mC);
	mC=vshrq_n_u64(mul[9],32);
	mul[10]=vmlal_u32(mul[11],a,b[10]);
	mul[10]=vaddq_u64(mul[10],mC);
	mC=vshrq_n_u64(mul[10],32);
	mul[11]=vmlal_u32(mul[12],a,b[11]);
	mul[11]=vaddq_u64(mul[11],mC);
	mC=vshrq_n_u64(mul[11],32);
	mul[12]=vmlal_u32(mul[13],a,b[12]);
	mul[12]=vaddq_u64(mul[12],mC);
	mC=vshrq_n_u64(mul[12],32);
	mul[13]=vmlal_u32(mul[14],a,b[13]);
	mul[13]=vaddq_u64(mul[13],mC);
	mul[14]=vshrq_n_u64(mul[13],32);
	t0=vmovn_u64(mul[0]);
	T[2]=vget_lane_u32(t0,0);
	T2[2]=vget_lane_u32(t0,1);



	//Cuarta It
	a=vld1_u32(&A2[6]);
	mul[0]=vandq_u64(mul[0],l);
	mul[1]=vandq_u64(mul[1],l);
	mul[2]=vandq_u64(mul[2],l);
	mul[3]=vandq_u64(mul[3],l);
	mul[4]=vandq_u64(mul[4],l);
	mul[5]=vandq_u64(mul[5],l);
	mul[6]=vandq_u64(mul[6],l);
	mul[7]=vandq_u64(mul[7],l);
	mul[8]=vandq_u64(mul[8],l);
	mul[9]=vandq_u64(mul[9],l);
	mul[10]=vandq_u64(mul[10],l);
	mul[11]=vandq_u64(mul[11],l);
	mul[12]=vandq_u64(mul[12],l);
	mul[13]=vandq_u64(mul[13],l);
	
	mul[0]=vmlal_u32(mul[1],a,b[0]);
	mC=vshrq_n_u64(mul[0],32);
	mul[1]=vmlal_u32(mul[2],a,b[1]);
	mul[1]=vaddq_u64(mul[1],mC);
	mC=vshrq_n_u64(mul[1],32);
	mul[2]=vmlal_u32(mul[3],a,b[2]);
	mul[2]=vaddq_u64(mul[2],mC);
	mC=vshrq_n_u64(mul[2],32);
	mul[3]=vmlal_u32(mul[4],a,b[3]);
	mul[3]=vaddq_u64(mul[3],mC);
	mC=vshrq_n_u64(mul[3],32);
	mul[4]=vmlal_u32(mul[5],a,b[4]);
	mul[4]=vaddq_u64(mul[4],mC);
	mC=vshrq_n_u64(mul[4],32);
	mul[5]=vmlal_u32(mul[6],a,b[5]);
	mul[5]=vaddq_u64(mul[5],mC);
	mC=vshrq_n_u64(mul[5],32);
	mul[6]=vmlal_u32(mul[7],a,b[6]);
	mul[6]=vaddq_u64(mul[6],mC);
	mC=vshrq_n_u64(mul[6],32);
	mul[7]=vmlal_u32(mul[8],a,b[7]);
	mul[7]=vaddq_u64(mul[7],mC);
	mC=vshrq_n_u64(mul[7],32);
	mul[8]=vmlal_u32(mul[9],a,b[8]);
	mul[8]=vaddq_u64(mul[8],mC);
	mC=vshrq_n_u64(mul[8],32);
	mul[9]=vmlal_u32(mul[10],a,b[9]);
	mul[9]=vaddq_u64(mul[9],mC);
	mC=vshrq_n_u64(mul[9],32);
	mul[10]=vmlal_u32(mul[11],a,b[10]);
	mul[10]=vaddq_u64(mul[10],mC);
	mC=vshrq_n_u64(mul[10],32);
	mul[11]=vmlal_u32(mul[12],a,b[11]);
	mul[11]=vaddq_u64(mul[11],mC);
	mC=vshrq_n_u64(mul[11],32);
	mul[12]=vmlal_u32(mul[13],a,b[12]);
	mul[12]=vaddq_u64(mul[12],mC);
	mC=vshrq_n_u64(mul[12],32);
	mul[13]=vmlal_u32(mul[14],a,b[13]);
	mul[13]=vaddq_u64(mul[13],mC);
	mul[14]=vshrq_n_u64(mul[13],32);
	t0=vmovn_u64(mul[0]);
	T[3]=vget_lane_u32(t0,0);
	T2[3]=vget_lane_u32(t0,1);



	//Quinta It
	a=vld1_u32(&A2[8]);
	mul[0]=vandq_u64(mul[0],l);
	mul[1]=vandq_u64(mul[1],l);
	mul[2]=vandq_u64(mul[2],l);
	mul[3]=vandq_u64(mul[3],l);
	mul[4]=vandq_u64(mul[4],l);
	mul[5]=vandq_u64(mul[5],l);
	mul[6]=vandq_u64(mul[6],l);
	mul[7]=vandq_u64(mul[7],l);
	mul[8]=vandq_u64(mul[8],l);
	mul[9]=vandq_u64(mul[9],l);
	mul[10]=vandq_u64(mul[10],l);
	mul[11]=vandq_u64(mul[11],l);
	mul[12]=vandq_u64(mul[12],l);
	mul[13]=vandq_u64(mul[13],l);
	
	mul[0]=vmlal_u32(mul[1],a,b[0]);
	mC=vshrq_n_u64(mul[0],32);
	mul[1]=vmlal_u32(mul[2],a,b[1]);
	mul[1]=vaddq_u64(mul[1],mC);
	mC=vshrq_n_u64(mul[1],32);
	mul[2]=vmlal_u32(mul[3],a,b[2]);
	mul[2]=vaddq_u64(mul[2],mC);
	mC=vshrq_n_u64(mul[2],32);
	mul[3]=vmlal_u32(mul[4],a,b[3]);
	mul[3]=vaddq_u64(mul[3],mC);
	mC=vshrq_n_u64(mul[3],32);
	mul[4]=vmlal_u32(mul[5],a,b[4]);
	mul[4]=vaddq_u64(mul[4],mC);
	mC=vshrq_n_u64(mul[4],32);
	mul[5]=vmlal_u32(mul[6],a,b[5]);
	mul[5]=vaddq_u64(mul[5],mC);
	mC=vshrq_n_u64(mul[5],32);
	mul[6]=vmlal_u32(mul[7],a,b[6]);
	mul[6]=vaddq_u64(mul[6],mC);
	mC=vshrq_n_u64(mul[6],32);
	mul[7]=vmlal_u32(mul[8],a,b[7]);
	mul[7]=vaddq_u64(mul[7],mC);
	mC=vshrq_n_u64(mul[7],32);
	mul[8]=vmlal_u32(mul[9],a,b[8]);
	mul[8]=vaddq_u64(mul[8],mC);
	mC=vshrq_n_u64(mul[8],32);
	mul[9]=vmlal_u32(mul[10],a,b[9]);
	mul[9]=vaddq_u64(mul[9],mC);
	mC=vshrq_n_u64(mul[9],32);
	mul[10]=vmlal_u32(mul[11],a,b[10]);
	mul[10]=vaddq_u64(mul[10],mC);
	mC=vshrq_n_u64(mul[10],32);
	mul[11]=vmlal_u32(mul[12],a,b[11]);
	mul[11]=vaddq_u64(mul[11],mC);
	mC=vshrq_n_u64(mul[11],32);
	mul[12]=vmlal_u32(mul[13],a,b[12]);
	mul[12]=vaddq_u64(mul[12],mC);
	mC=vshrq_n_u64(mul[12],32);
	mul[13]=vmlal_u32(mul[14],a,b[13]);
	mul[13]=vaddq_u64(mul[13],mC);
	mul[14]=vshrq_n_u64(mul[13],32);
	t0=vmovn_u64(mul[0]);
	T[4]=vget_lane_u32(t0,0);
	T2[4]=vget_lane_u32(t0,1);

	//Sexta It
	a=vld1_u32(&A2[10]);
	mul[0]=vandq_u64(mul[0],l);
	mul[1]=vandq_u64(mul[1],l);
	mul[2]=vandq_u64(mul[2],l);
	mul[3]=vandq_u64(mul[3],l);
	mul[4]=vandq_u64(mul[4],l);
	mul[5]=vandq_u64(mul[5],l);
	mul[6]=vandq_u64(mul[6],l);
	mul[7]=vandq_u64(mul[7],l);
	mul[8]=vandq_u64(mul[8],l);
	mul[9]=vandq_u64(mul[9],l);
	mul[10]=vandq_u64(mul[10],l);
	mul[11]=vandq_u64(mul[11],l);
	mul[12]=vandq_u64(mul[12],l);
	mul[13]=vandq_u64(mul[13],l);
	
	mul[0]=vmlal_u32(mul[1],a,b[0]);
	mC=vshrq_n_u64(mul[0],32);
	mul[1]=vmlal_u32(mul[2],a,b[1]);
	mul[1]=vaddq_u64(mul[1],mC);
	mC=vshrq_n_u64(mul[1],32);
	mul[2]=vmlal_u32(mul[3],a,b[2]);
	mul[2]=vaddq_u64(mul[2],mC);
	mC=vshrq_n_u64(mul[2],32);
	mul[3]=vmlal_u32(mul[4],a,b[3]);
	mul[3]=vaddq_u64(mul[3],mC);
	mC=vshrq_n_u64(mul[3],32);
	mul[4]=vmlal_u32(mul[5],a,b[4]);
	mul[4]=vaddq_u64(mul[4],mC);
	mC=vshrq_n_u64(mul[4],32);
	mul[5]=vmlal_u32(mul[6],a,b[5]);
	mul[5]=vaddq_u64(mul[5],mC);
	mC=vshrq_n_u64(mul[5],32);
	mul[6]=vmlal_u32(mul[7],a,b[6]);
	mul[6]=vaddq_u64(mul[6],mC);
	mC=vshrq_n_u64(mul[6],32);
	mul[7]=vmlal_u32(mul[8],a,b[7]);
	mul[7]=vaddq_u64(mul[7],mC);
	mC=vshrq_n_u64(mul[7],32);
	mul[8]=vmlal_u32(mul[9],a,b[8]);
	mul[8]=vaddq_u64(mul[8],mC);
	mC=vshrq_n_u64(mul[8],32);
	mul[9]=vmlal_u32(mul[10],a,b[9]);
	mul[9]=vaddq_u64(mul[9],mC);
	mC=vshrq_n_u64(mul[9],32);
	mul[10]=vmlal_u32(mul[11],a,b[10]);
	mul[10]=vaddq_u64(mul[10],mC);
	mC=vshrq_n_u64(mul[10],32);
	mul[11]=vmlal_u32(mul[12],a,b[11]);
	mul[11]=vaddq_u64(mul[11],mC);
	mC=vshrq_n_u64(mul[11],32);
	mul[12]=vmlal_u32(mul[13],a,b[12]);
	mul[12]=vaddq_u64(mul[12],mC);
	mC=vshrq_n_u64(mul[12],32);
	mul[13]=vmlal_u32(mul[14],a,b[13]);
	mul[13]=vaddq_u64(mul[13],mC);
	mul[14]=vshrq_n_u64(mul[13],32);
	t0=vmovn_u64(mul[0]);
	T[5]=vget_lane_u32(t0,0);
	T2[5]=vget_lane_u32(t0,1);



	//Septima It
	a=vld1_u32(&A2[12]);
	mul[0]=vandq_u64(mul[0],l);
	mul[1]=vandq_u64(mul[1],l);
	mul[2]=vandq_u64(mul[2],l);
	mul[3]=vandq_u64(mul[3],l);
	mul[4]=vandq_u64(mul[4],l);
	mul[5]=vandq_u64(mul[5],l);
	mul[6]=vandq_u64(mul[6],l);
	mul[7]=vandq_u64(mul[7],l);
	mul[8]=vandq_u64(mul[8],l);
	mul[9]=vandq_u64(mul[9],l);
	mul[10]=vandq_u64(mul[10],l);
	mul[11]=vandq_u64(mul[11],l);
	mul[12]=vandq_u64(mul[12],l);
	mul[13]=vandq_u64(mul[13],l);
	
	mul[0]=vmlal_u32(mul[1],a,b[0]);
	mC=vshrq_n_u64(mul[0],32);
	mul[1]=vmlal_u32(mul[2],a,b[1]);
	mul[1]=vaddq_u64(mul[1],mC);
	mC=vshrq_n_u64(mul[1],32);
	mul[2]=vmlal_u32(mul[3],a,b[2]);
	mul[2]=vaddq_u64(mul[2],mC);
	mC=vshrq_n_u64(mul[2],32);
	mul[3]=vmlal_u32(mul[4],a,b[3]);
	mul[3]=vaddq_u64(mul[3],mC);
	mC=vshrq_n_u64(mul[3],32);
	mul[4]=vmlal_u32(mul[5],a,b[4]);
	mul[4]=vaddq_u64(mul[4],mC);
	mC=vshrq_n_u64(mul[4],32);
	mul[5]=vmlal_u32(mul[6],a,b[5]);
	mul[5]=vaddq_u64(mul[5],mC);
	mC=vshrq_n_u64(mul[5],32);
	mul[6]=vmlal_u32(mul[7],a,b[6]);
	mul[6]=vaddq_u64(mul[6],mC);
	mC=vshrq_n_u64(mul[6],32);
	mul[7]=vmlal_u32(mul[8],a,b[7]);
	mul[7]=vaddq_u64(mul[7],mC);
	mC=vshrq_n_u64(mul[7],32);
	mul[8]=vmlal_u32(mul[9],a,b[8]);
	mul[8]=vaddq_u64(mul[8],mC);
	mC=vshrq_n_u64(mul[8],32);
	mul[9]=vmlal_u32(mul[10],a,b[9]);
	mul[9]=vaddq_u64(mul[9],mC);
	mC=vshrq_n_u64(mul[9],32);
	mul[10]=vmlal_u32(mul[11],a,b[10]);
	mul[10]=vaddq_u64(mul[10],mC);
	mC=vshrq_n_u64(mul[10],32);
	mul[11]=vmlal_u32(mul[12],a,b[11]);
	mul[11]=vaddq_u64(mul[11],mC);
	mC=vshrq_n_u64(mul[11],32);
	mul[12]=vmlal_u32(mul[13],a,b[12]);
	mul[12]=vaddq_u64(mul[12],mC);
	mC=vshrq_n_u64(mul[12],32);
	mul[13]=vmlal_u32(mul[14],a,b[13]);
	mul[13]=vaddq_u64(mul[13],mC);
	mul[14]=vshrq_n_u64(mul[13],32);
	t0=vmovn_u64(mul[0]);
	T[6]=vget_lane_u32(t0,0);
	T2[6]=vget_lane_u32(t0,1);



	//Octava It
	a=vld1_u32(&A2[14]);
	mul[0]=vandq_u64(mul[0],l);
	mul[1]=vandq_u64(mul[1],l);
	mul[2]=vandq_u64(mul[2],l);
	mul[3]=vandq_u64(mul[3],l);
	mul[4]=vandq_u64(mul[4],l);
	mul[5]=vandq_u64(mul[5],l);
	mul[6]=vandq_u64(mul[6],l);
	mul[7]=vandq_u64(mul[7],l);
	mul[8]=vandq_u64(mul[8],l);
	mul[9]=vandq_u64(mul[9],l);
	mul[10]=vandq_u64(mul[10],l);
	mul[11]=vandq_u64(mul[11],l);
	mul[12]=vandq_u64(mul[12],l);
	mul[13]=vandq_u64(mul[13],l);
	
	mul[0]=vmlal_u32(mul[1],a,b[0]);
	mC=vshrq_n_u64(mul[0],32);
	mul[1]=vmlal_u32(mul[2],a,b[1]);
	mul[1]=vaddq_u64(mul[1],mC);
	mC=vshrq_n_u64(mul[1],32);
	mul[2]=vmlal_u32(mul[3],a,b[2]);
	mul[2]=vaddq_u64(mul[2],mC);
	mC=vshrq_n_u64(mul[2],32);
	mul[3]=vmlal_u32(mul[4],a,b[3]);
	mul[3]=vaddq_u64(mul[3],mC);
	mC=vshrq_n_u64(mul[3],32);
	mul[4]=vmlal_u32(mul[5],a,b[4]);
	mul[4]=vaddq_u64(mul[4],mC);
	mC=vshrq_n_u64(mul[4],32);
	mul[5]=vmlal_u32(mul[6],a,b[5]);
	mul[5]=vaddq_u64(mul[5],mC);
	mC=vshrq_n_u64(mul[5],32);
	mul[6]=vmlal_u32(mul[7],a,b[6]);
	mul[6]=vaddq_u64(mul[6],mC);
	mC=vshrq_n_u64(mul[6],32);
	mul[7]=vmlal_u32(mul[8],a,b[7]);
	mul[7]=vaddq_u64(mul[7],mC);
	mC=vshrq_n_u64(mul[7],32);
	mul[8]=vmlal_u32(mul[9],a,b[8]);
	mul[8]=vaddq_u64(mul[8],mC);
	mC=vshrq_n_u64(mul[8],32);
	mul[9]=vmlal_u32(mul[10],a,b[9]);
	mul[9]=vaddq_u64(mul[9],mC);
	mC=vshrq_n_u64(mul[9],32);
	mul[10]=vmlal_u32(mul[11],a,b[10]);
	mul[10]=vaddq_u64(mul[10],mC);
	mC=vshrq_n_u64(mul[10],32);
	mul[11]=vmlal_u32(mul[12],a,b[11]);
	mul[11]=vaddq_u64(mul[11],mC);
	mC=vshrq_n_u64(mul[11],32);
	mul[12]=vmlal_u32(mul[13],a,b[12]);
	mul[12]=vaddq_u64(mul[12],mC);
	mC=vshrq_n_u64(mul[12],32);
	mul[13]=vmlal_u32(mul[14],a,b[13]);
	mul[13]=vaddq_u64(mul[13],mC);
	mul[14]=vshrq_n_u64(mul[13],32);
	t0=vmovn_u64(mul[0]);
	T[7]=vget_lane_u32(t0,0);
	T2[7]=vget_lane_u32(t0,1);

		//Octava It
	a=vld1_u32(&A2[16]);
	mul[0]=vandq_u64(mul[0],l);
	mul[1]=vandq_u64(mul[1],l);
	mul[2]=vandq_u64(mul[2],l);
	mul[3]=vandq_u64(mul[3],l);
	mul[4]=vandq_u64(mul[4],l);
	mul[5]=vandq_u64(mul[5],l);
	mul[6]=vandq_u64(mul[6],l);
	mul[7]=vandq_u64(mul[7],l);
	mul[8]=vandq_u64(mul[8],l);
	mul[9]=vandq_u64(mul[9],l);
	mul[10]=vandq_u64(mul[10],l);
	mul[11]=vandq_u64(mul[11],l);
	mul[12]=vandq_u64(mul[12],l);
	mul[13]=vandq_u64(mul[13],l);
	
	mul[0]=vmlal_u32(mul[1],a,b[0]);
	mC=vshrq_n_u64(mul[0],32);
	mul[1]=vmlal_u32(mul[2],a,b[1]);
	mul[1]=vaddq_u64(mul[1],mC);
	mC=vshrq_n_u64(mul[1],32);
	mul[2]=vmlal_u32(mul[3],a,b[2]);
	mul[2]=vaddq_u64(mul[2],mC);
	mC=vshrq_n_u64(mul[2],32);
	mul[3]=vmlal_u32(mul[4],a,b[3]);
	mul[3]=vaddq_u64(mul[3],mC);
	mC=vshrq_n_u64(mul[3],32);
	mul[4]=vmlal_u32(mul[5],a,b[4]);
	mul[4]=vaddq_u64(mul[4],mC);
	mC=vshrq_n_u64(mul[4],32);
	mul[5]=vmlal_u32(mul[6],a,b[5]);
	mul[5]=vaddq_u64(mul[5],mC);
	mC=vshrq_n_u64(mul[5],32);
	mul[6]=vmlal_u32(mul[7],a,b[6]);
	mul[6]=vaddq_u64(mul[6],mC);
	mC=vshrq_n_u64(mul[6],32);
	mul[7]=vmlal_u32(mul[8],a,b[7]);
	mul[7]=vaddq_u64(mul[7],mC);
	mC=vshrq_n_u64(mul[7],32);
	mul[8]=vmlal_u32(mul[9],a,b[8]);
	mul[8]=vaddq_u64(mul[8],mC);
	mC=vshrq_n_u64(mul[8],32);
	mul[9]=vmlal_u32(mul[10],a,b[9]);
	mul[9]=vaddq_u64(mul[9],mC);
	mC=vshrq_n_u64(mul[9],32);
	mul[10]=vmlal_u32(mul[11],a,b[10]);
	mul[10]=vaddq_u64(mul[10],mC);
	mC=vshrq_n_u64(mul[10],32);
	mul[11]=vmlal_u32(mul[12],a,b[11]);
	mul[11]=vaddq_u64(mul[11],mC);
	mC=vshrq_n_u64(mul[11],32);
	mul[12]=vmlal_u32(mul[13],a,b[12]);
	mul[12]=vaddq_u64(mul[12],mC);
	mC=vshrq_n_u64(mul[12],32);
	mul[13]=vmlal_u32(mul[14],a,b[13]);
	mul[13]=vaddq_u64(mul[13],mC);
	mul[14]=vshrq_n_u64(mul[13],32);
	t0=vmovn_u64(mul[0]);
	T[8]=vget_lane_u32(t0,0);
	T2[8]=vget_lane_u32(t0,1);
	
		//Octava It
	a=vld1_u32(&A2[18]);
	mul[0]=vandq_u64(mul[0],l);
	mul[1]=vandq_u64(mul[1],l);
	mul[2]=vandq_u64(mul[2],l);
	mul[3]=vandq_u64(mul[3],l);
	mul[4]=vandq_u64(mul[4],l);
	mul[5]=vandq_u64(mul[5],l);
	mul[6]=vandq_u64(mul[6],l);
	mul[7]=vandq_u64(mul[7],l);
	mul[8]=vandq_u64(mul[8],l);
	mul[9]=vandq_u64(mul[9],l);
	mul[10]=vandq_u64(mul[10],l);
	mul[11]=vandq_u64(mul[11],l);
	mul[12]=vandq_u64(mul[12],l);
	mul[13]=vandq_u64(mul[13],l);
	
	mul[0]=vmlal_u32(mul[1],a,b[0]);
	mC=vshrq_n_u64(mul[0],32);
	mul[1]=vmlal_u32(mul[2],a,b[1]);
	mul[1]=vaddq_u64(mul[1],mC);
	mC=vshrq_n_u64(mul[1],32);
	mul[2]=vmlal_u32(mul[3],a,b[2]);
	mul[2]=vaddq_u64(mul[2],mC);
	mC=vshrq_n_u64(mul[2],32);
	mul[3]=vmlal_u32(mul[4],a,b[3]);
	mul[3]=vaddq_u64(mul[3],mC);
	mC=vshrq_n_u64(mul[3],32);
	mul[4]=vmlal_u32(mul[5],a,b[4]);
	mul[4]=vaddq_u64(mul[4],mC);
	mC=vshrq_n_u64(mul[4],32);
	mul[5]=vmlal_u32(mul[6],a,b[5]);
	mul[5]=vaddq_u64(mul[5],mC);
	mC=vshrq_n_u64(mul[5],32);
	mul[6]=vmlal_u32(mul[7],a,b[6]);
	mul[6]=vaddq_u64(mul[6],mC);
	mC=vshrq_n_u64(mul[6],32);
	mul[7]=vmlal_u32(mul[8],a,b[7]);
	mul[7]=vaddq_u64(mul[7],mC);
	mC=vshrq_n_u64(mul[7],32);
	mul[8]=vmlal_u32(mul[9],a,b[8]);
	mul[8]=vaddq_u64(mul[8],mC);
	mC=vshrq_n_u64(mul[8],32);
	mul[9]=vmlal_u32(mul[10],a,b[9]);
	mul[9]=vaddq_u64(mul[9],mC);
	mC=vshrq_n_u64(mul[9],32);
	mul[10]=vmlal_u32(mul[11],a,b[10]);
	mul[10]=vaddq_u64(mul[10],mC);
	mC=vshrq_n_u64(mul[10],32);
	mul[11]=vmlal_u32(mul[12],a,b[11]);
	mul[11]=vaddq_u64(mul[11],mC);
	mC=vshrq_n_u64(mul[11],32);
	mul[12]=vmlal_u32(mul[13],a,b[12]);
	mul[12]=vaddq_u64(mul[12],mC);
	mC=vshrq_n_u64(mul[12],32);
	mul[13]=vmlal_u32(mul[14],a,b[13]);
	mul[13]=vaddq_u64(mul[13],mC);
	mul[14]=vshrq_n_u64(mul[13],32);
	t0=vmovn_u64(mul[0]);
	T[9]=vget_lane_u32(t0,0);
	T2[9]=vget_lane_u32(t0,1);
	
		//Octava It
	a=vld1_u32(&A2[20]);
	mul[0]=vandq_u64(mul[0],l);
	mul[1]=vandq_u64(mul[1],l);
	mul[2]=vandq_u64(mul[2],l);
	mul[3]=vandq_u64(mul[3],l);
	mul[4]=vandq_u64(mul[4],l);
	mul[5]=vandq_u64(mul[5],l);
	mul[6]=vandq_u64(mul[6],l);
	mul[7]=vandq_u64(mul[7],l);
	mul[8]=vandq_u64(mul[8],l);
	mul[9]=vandq_u64(mul[9],l);
	mul[10]=vandq_u64(mul[10],l);
	mul[11]=vandq_u64(mul[11],l);
	mul[12]=vandq_u64(mul[12],l);
	mul[13]=vandq_u64(mul[13],l);
	
	mul[0]=vmlal_u32(mul[1],a,b[0]);
	mC=vshrq_n_u64(mul[0],32);
	mul[1]=vmlal_u32(mul[2],a,b[1]);
	mul[1]=vaddq_u64(mul[1],mC);
	mC=vshrq_n_u64(mul[1],32);
	mul[2]=vmlal_u32(mul[3],a,b[2]);
	mul[2]=vaddq_u64(mul[2],mC);
	mC=vshrq_n_u64(mul[2],32);
	mul[3]=vmlal_u32(mul[4],a,b[3]);
	mul[3]=vaddq_u64(mul[3],mC);
	mC=vshrq_n_u64(mul[3],32);
	mul[4]=vmlal_u32(mul[5],a,b[4]);
	mul[4]=vaddq_u64(mul[4],mC);
	mC=vshrq_n_u64(mul[4],32);
	mul[5]=vmlal_u32(mul[6],a,b[5]);
	mul[5]=vaddq_u64(mul[5],mC);
	mC=vshrq_n_u64(mul[5],32);
	mul[6]=vmlal_u32(mul[7],a,b[6]);
	mul[6]=vaddq_u64(mul[6],mC);
	mC=vshrq_n_u64(mul[6],32);
	mul[7]=vmlal_u32(mul[8],a,b[7]);
	mul[7]=vaddq_u64(mul[7],mC);
	mC=vshrq_n_u64(mul[7],32);
	mul[8]=vmlal_u32(mul[9],a,b[8]);
	mul[8]=vaddq_u64(mul[8],mC);
	mC=vshrq_n_u64(mul[8],32);
	mul[9]=vmlal_u32(mul[10],a,b[9]);
	mul[9]=vaddq_u64(mul[9],mC);
	mC=vshrq_n_u64(mul[9],32);
	mul[10]=vmlal_u32(mul[11],a,b[10]);
	mul[10]=vaddq_u64(mul[10],mC);
	mC=vshrq_n_u64(mul[10],32);
	mul[11]=vmlal_u32(mul[12],a,b[11]);
	mul[11]=vaddq_u64(mul[11],mC);
	mC=vshrq_n_u64(mul[11],32);
	mul[12]=vmlal_u32(mul[13],a,b[12]);
	mul[12]=vaddq_u64(mul[12],mC);
	mC=vshrq_n_u64(mul[12],32);
	mul[13]=vmlal_u32(mul[14],a,b[13]);
	mul[13]=vaddq_u64(mul[13],mC);
	mul[14]=vshrq_n_u64(mul[13],32);
	t0=vmovn_u64(mul[0]);
	T[10]=vget_lane_u32(t0,0);
	T2[10]=vget_lane_u32(t0,1);
	
		//Octava It
	a=vld1_u32(&A2[22]);
	mul[0]=vandq_u64(mul[0],l);
	mul[1]=vandq_u64(mul[1],l);
	mul[2]=vandq_u64(mul[2],l);
	mul[3]=vandq_u64(mul[3],l);
	mul[4]=vandq_u64(mul[4],l);
	mul[5]=vandq_u64(mul[5],l);
	mul[6]=vandq_u64(mul[6],l);
	mul[7]=vandq_u64(mul[7],l);
	mul[8]=vandq_u64(mul[8],l);
	mul[9]=vandq_u64(mul[9],l);
	mul[10]=vandq_u64(mul[10],l);
	mul[11]=vandq_u64(mul[11],l);
	mul[12]=vandq_u64(mul[12],l);
	mul[13]=vandq_u64(mul[13],l);
	
	mul[0]=vmlal_u32(mul[1],a,b[0]);
	mC=vshrq_n_u64(mul[0],32);
	mul[1]=vmlal_u32(mul[2],a,b[1]);
	mul[1]=vaddq_u64(mul[1],mC);
	mC=vshrq_n_u64(mul[1],32);
	mul[2]=vmlal_u32(mul[3],a,b[2]);
	mul[2]=vaddq_u64(mul[2],mC);
	mC=vshrq_n_u64(mul[2],32);
	mul[3]=vmlal_u32(mul[4],a,b[3]);
	mul[3]=vaddq_u64(mul[3],mC);
	mC=vshrq_n_u64(mul[3],32);
	mul[4]=vmlal_u32(mul[5],a,b[4]);
	mul[4]=vaddq_u64(mul[4],mC);
	mC=vshrq_n_u64(mul[4],32);
	mul[5]=vmlal_u32(mul[6],a,b[5]);
	mul[5]=vaddq_u64(mul[5],mC);
	mC=vshrq_n_u64(mul[5],32);
	mul[6]=vmlal_u32(mul[7],a,b[6]);
	mul[6]=vaddq_u64(mul[6],mC);
	mC=vshrq_n_u64(mul[6],32);
	mul[7]=vmlal_u32(mul[8],a,b[7]);
	mul[7]=vaddq_u64(mul[7],mC);
	mC=vshrq_n_u64(mul[7],32);
	mul[8]=vmlal_u32(mul[9],a,b[8]);
	mul[8]=vaddq_u64(mul[8],mC);
	mC=vshrq_n_u64(mul[8],32);
	mul[9]=vmlal_u32(mul[10],a,b[9]);
	mul[9]=vaddq_u64(mul[9],mC);
	mC=vshrq_n_u64(mul[9],32);
	mul[10]=vmlal_u32(mul[11],a,b[10]);
	mul[10]=vaddq_u64(mul[10],mC);
	mC=vshrq_n_u64(mul[10],32);
	mul[11]=vmlal_u32(mul[12],a,b[11]);
	mul[11]=vaddq_u64(mul[11],mC);
	mC=vshrq_n_u64(mul[11],32);
	mul[12]=vmlal_u32(mul[13],a,b[12]);
	mul[12]=vaddq_u64(mul[12],mC);
	mC=vshrq_n_u64(mul[12],32);
	mul[13]=vmlal_u32(mul[14],a,b[13]);
	mul[13]=vaddq_u64(mul[13],mC);
	mul[14]=vshrq_n_u64(mul[13],32);
	t0=vmovn_u64(mul[0]);
	T[11]=vget_lane_u32(t0,0);
	T2[11]=vget_lane_u32(t0,1);
	
		//Octava It
	a=vld1_u32(&A2[24]);
	mul[0]=vandq_u64(mul[0],l);
	mul[1]=vandq_u64(mul[1],l);
	mul[2]=vandq_u64(mul[2],l);
	mul[3]=vandq_u64(mul[3],l);
	mul[4]=vandq_u64(mul[4],l);
	mul[5]=vandq_u64(mul[5],l);
	mul[6]=vandq_u64(mul[6],l);
	mul[7]=vandq_u64(mul[7],l);
	mul[8]=vandq_u64(mul[8],l);
	mul[9]=vandq_u64(mul[9],l);
	mul[10]=vandq_u64(mul[10],l);
	mul[11]=vandq_u64(mul[11],l);
	mul[12]=vandq_u64(mul[12],l);
	mul[13]=vandq_u64(mul[13],l);
	
	mul[0]=vmlal_u32(mul[1],a,b[0]);
	mC=vshrq_n_u64(mul[0],32);
	mul[1]=vmlal_u32(mul[2],a,b[1]);
	mul[1]=vaddq_u64(mul[1],mC);
	mC=vshrq_n_u64(mul[1],32);
	mul[2]=vmlal_u32(mul[3],a,b[2]);
	mul[2]=vaddq_u64(mul[2],mC);
	mC=vshrq_n_u64(mul[2],32);
	mul[3]=vmlal_u32(mul[4],a,b[3]);
	mul[3]=vaddq_u64(mul[3],mC);
	mC=vshrq_n_u64(mul[3],32);
	mul[4]=vmlal_u32(mul[5],a,b[4]);
	mul[4]=vaddq_u64(mul[4],mC);
	mC=vshrq_n_u64(mul[4],32);
	mul[5]=vmlal_u32(mul[6],a,b[5]);
	mul[5]=vaddq_u64(mul[5],mC);
	mC=vshrq_n_u64(mul[5],32);
	mul[6]=vmlal_u32(mul[7],a,b[6]);
	mul[6]=vaddq_u64(mul[6],mC);
	mC=vshrq_n_u64(mul[6],32);
	mul[7]=vmlal_u32(mul[8],a,b[7]);
	mul[7]=vaddq_u64(mul[7],mC);
	mC=vshrq_n_u64(mul[7],32);
	mul[8]=vmlal_u32(mul[9],a,b[8]);
	mul[8]=vaddq_u64(mul[8],mC);
	mC=vshrq_n_u64(mul[8],32);
	mul[9]=vmlal_u32(mul[10],a,b[9]);
	mul[9]=vaddq_u64(mul[9],mC);
	mC=vshrq_n_u64(mul[9],32);
	mul[10]=vmlal_u32(mul[11],a,b[10]);
	mul[10]=vaddq_u64(mul[10],mC);
	mC=vshrq_n_u64(mul[10],32);
	mul[11]=vmlal_u32(mul[12],a,b[11]);
	mul[11]=vaddq_u64(mul[11],mC);
	mC=vshrq_n_u64(mul[11],32);
	mul[12]=vmlal_u32(mul[13],a,b[12]);
	mul[12]=vaddq_u64(mul[12],mC);
	mC=vshrq_n_u64(mul[12],32);
	mul[13]=vmlal_u32(mul[14],a,b[13]);
	mul[13]=vaddq_u64(mul[13],mC);
	mul[14]=vshrq_n_u64(mul[13],32);
	t0=vmovn_u64(mul[0]);
	T[12]=vget_lane_u32(t0,0);
	T2[12]=vget_lane_u32(t0,1);
	
	
		//Octava It
	a=vld1_u32(&A2[26]);
	mul[0]=vandq_u64(mul[0],l);
	mul[1]=vandq_u64(mul[1],l);
	mul[2]=vandq_u64(mul[2],l);
	mul[3]=vandq_u64(mul[3],l);
	mul[4]=vandq_u64(mul[4],l);
	mul[5]=vandq_u64(mul[5],l);
	mul[6]=vandq_u64(mul[6],l);
	mul[7]=vandq_u64(mul[7],l);
	mul[8]=vandq_u64(mul[8],l);
	mul[9]=vandq_u64(mul[9],l);
	mul[10]=vandq_u64(mul[10],l);
	mul[11]=vandq_u64(mul[11],l);
	mul[12]=vandq_u64(mul[12],l);
	mul[13]=vandq_u64(mul[13],l);
	
	mul[0]=vmlal_u32(mul[1],a,b[0]);
	mC=vshrq_n_u64(mul[0],32);
	mul[1]=vmlal_u32(mul[2],a,b[1]);
	mul[1]=vaddq_u64(mul[1],mC);
	mC=vshrq_n_u64(mul[1],32);
	mul[2]=vmlal_u32(mul[3],a,b[2]);
	mul[2]=vaddq_u64(mul[2],mC);
	mC=vshrq_n_u64(mul[2],32);
	mul[3]=vmlal_u32(mul[4],a,b[3]);
	mul[3]=vaddq_u64(mul[3],mC);
	mC=vshrq_n_u64(mul[3],32);
	mul[4]=vmlal_u32(mul[5],a,b[4]);
	mul[4]=vaddq_u64(mul[4],mC);
	mC=vshrq_n_u64(mul[4],32);
	mul[5]=vmlal_u32(mul[6],a,b[5]);
	mul[5]=vaddq_u64(mul[5],mC);
	mC=vshrq_n_u64(mul[5],32);
	mul[6]=vmlal_u32(mul[7],a,b[6]);
	mul[6]=vaddq_u64(mul[6],mC);
	mC=vshrq_n_u64(mul[6],32);
	mul[7]=vmlal_u32(mul[8],a,b[7]);
	mul[7]=vaddq_u64(mul[7],mC);
	mC=vshrq_n_u64(mul[7],32);
	mul[8]=vmlal_u32(mul[9],a,b[8]);
	mul[8]=vaddq_u64(mul[8],mC);
	mC=vshrq_n_u64(mul[8],32);
	mul[9]=vmlal_u32(mul[10],a,b[9]);
	mul[9]=vaddq_u64(mul[9],mC);
	mC=vshrq_n_u64(mul[9],32);
	mul[10]=vmlal_u32(mul[11],a,b[10]);
	mul[10]=vaddq_u64(mul[10],mC);
	mC=vshrq_n_u64(mul[10],32);
	mul[11]=vmlal_u32(mul[12],a,b[11]);
	mul[11]=vaddq_u64(mul[11],mC);
	mC=vshrq_n_u64(mul[11],32);
	mul[12]=vmlal_u32(mul[13],a,b[12]);
	mul[12]=vaddq_u64(mul[12],mC);
	mC=vshrq_n_u64(mul[12],32);
	mul[13]=vmlal_u32(mul[14],a,b[13]);
	mul[13]=vaddq_u64(mul[13],mC);
	mul[14]=vshrq_n_u64(mul[13],32);
	t0=vmovn_u64(mul[0]);
	T[13]=vget_lane_u32(t0,0);
	T2[13]=vget_lane_u32(t0,1);
	
	
	
	b[0]=vmovn_u64(mul[1]);
	b[1]=vmovn_u64(mul[2]);
	b[2]=vmovn_u64(mul[3]);
	b[3]=vmovn_u64(mul[4]);
	b[4]=vmovn_u64(mul[5]);
	b[5]=vmovn_u64(mul[6]);
	b[6]=vmovn_u64(mul[7]);
	b[7]=vmovn_u64(mul[8]);
	b[8]=vmovn_u64(mul[9]);
	b[9]=vmovn_u64(mul[10]);
	b[10]=vmovn_u64(mul[11]);
	b[11]=vmovn_u64(mul[12]);
	b[12]=vmovn_u64(mul[13]);
	b[13]=vmovn_u64(mul[14]);

	
	T[14]=vget_lane_u32(b[0],0);
	T2[14]=vget_lane_u32(b[0],1);
	T[15]=vget_lane_u32(b[1],0);
	T2[15]=vget_lane_u32(b[1],1);
	T[16]=vget_lane_u32(b[2],0);
	T2[16]=vget_lane_u32(b[2],1);
	T[17]=vget_lane_u32(b[3],0);
	T2[17]=vget_lane_u32(b[3],1);
	T[18]=vget_lane_u32(b[4],0);
	T2[18]=vget_lane_u32(b[4],1);
	T[19]=vget_lane_u32(b[5],0);
	T2[19]=vget_lane_u32(b[5],1);
	T[20]=vget_lane_u32(b[6],0);
	T2[20]=vget_lane_u32(b[6],1);
	T[21]=vget_lane_u32(b[7],0);
	T2[21]=vget_lane_u32(b[7],1);
	T[22]=vget_lane_u32(b[8],0);
	T2[22]=vget_lane_u32(b[8],1);
	T[23]=vget_lane_u32(b[9],0);
	T2[23]=vget_lane_u32(b[9],1);
	T[24]=vget_lane_u32(b[10],0);
	T2[24]=vget_lane_u32(b[10],1);
	T[25]=vget_lane_u32(b[11],0);
	T2[25]=vget_lane_u32(b[11],1);
	T[26]=vget_lane_u32(b[12],0);
	T2[26]=vget_lane_u32(b[12],1);
	T[27]=vget_lane_u32(b[13],0);
	T2[27]=vget_lane_u32(b[13],1);

}

void fp2_rdc_NEON(dig_t *T,dig_t *T2, dig_t *t, dig_t *t2){
	uint64x2_t mul[15], mC;
	uint32x2_t b[15];
	uint32x2_t m;
	dig_t B[]={t[0],t2[0],t[1],t2[1],t[2],t2[2],t[3],t2[3],t[4],t2[4],t[5],t2[5],t[6],t2[6],t[7],t2[7],t[8],t2[8],t[9],t2[9],t[10],t2[10],t[11],t2[11],t[12],t2[12],t[13],t2[13]};
	dig_t A[]={t[14],t2[14],t[15],t2[15],t[16],t2[16],t[17],t2[17],t[18],t2[18],t[19],t2[19],t[20],t2[20],t[21],t2[21],t[22],t2[22],t[23],t2[23],t[24],t2[24],t[25],t2[25],t[26],t2[26],t[27],t2[27]};

	b[0]=vld1_u32(B);
	b[1]=vld1_u32(&B[2]);
	b[2]=vld1_u32(&B[4]);
	b[3]=vld1_u32(&B[6]);
	b[4]=vld1_u32(&B[8]);
	b[5]=vld1_u32(&B[10]);
	b[6]=vld1_u32(&B[12]);
	b[7]=vld1_u32(&B[14]);
	b[8]=vld1_u32(&B[16]);
	b[9]=vld1_u32(&B[18]);
	b[10]=vld1_u32(&B[20]);
	b[11]=vld1_u32(&B[22]);
	b[12]=vld1_u32(&B[24]);
	b[13]=vld1_u32(&B[26]);
	b[14]=vld1_u32(A);


	//Primera ITeracion
	mC=vmull_n_u32(b[0],pi[0]);
	m=vmovn_u64(mC);
	mul[0]=vmull_n_u32(m,p[0]);
	mul[0]=vaddw_u32(mul[0],b[0]);
	mC=vshrq_n_u64(mul[0],32);
	mul[1]=vmlal_n_u32(mC,m,p[1]);
	mul[1]=vaddw_u32(mul[1],b[1]);
	mC=vshrq_n_u64(mul[1],32);
	b[0]=vmovn_u64(mul[1]);
	mul[2]=vmlal_n_u32(mC,m,p[2]);
	mul[2]=vaddw_u32(mul[2],b[2]);
	mC=vshrq_n_u64(mul[2],32);
	b[1]=vmovn_u64(mul[2]);
	mul[3]=vmlal_n_u32(mC,m,p[3]);
	mul[3]=vaddw_u32(mul[3],b[3]);
	mC=vshrq_n_u64(mul[3],32);
	b[2]=vmovn_u64(mul[3]);
	mul[4]=vmlal_n_u32(mC,m,p[4]);
	mul[4]=vaddw_u32(mul[4],b[4]);
	mC=vshrq_n_u64(mul[4],32);
	b[3]=vmovn_u64(mul[4]);
	mul[5]=vmlal_n_u32(mC,m,p[5]);
	mul[5]=vaddw_u32(mul[5],b[5]);
	mC=vshrq_n_u64(mul[5],32);
	b[4]=vmovn_u64(mul[5]);
	mul[6]=vmlal_n_u32(mC,m,p[6]);
	mul[6]=vaddw_u32(mul[6],b[6]);
	mC=vshrq_n_u64(mul[6],32);
	b[5]=vmovn_u64(mul[6]);
	mul[7]=vmlal_n_u32(mC,m,p[7]);
	mul[7]=vaddw_u32(mul[7],b[7]);
	mC=vshrq_n_u64(mul[7],32);
	b[6]=vmovn_u64(mul[7]);
	mul[8]=vmlal_n_u32(mC,m,p[8]);
	mul[8]=vaddw_u32(mul[8],b[8]);
	mC=vshrq_n_u64(mul[8],32);
	b[7]=vmovn_u64(mul[8]);
	mul[9]=vmlal_n_u32(mC,m,p[9]);
	mul[9]=vaddw_u32(mul[9],b[9]);
	mC=vshrq_n_u64(mul[9],32);
	b[8]=vmovn_u64(mul[9]);
	mul[10]=vmlal_n_u32(mC,m,p[10]);
	mul[10]=vaddw_u32(mul[10],b[10]);
	mC=vshrq_n_u64(mul[10],32);
	b[9]=vmovn_u64(mul[10]);
	mul[11]=vmlal_n_u32(mC,m,p[11]);
	mul[11]=vaddw_u32(mul[11],b[11]);
	mC=vshrq_n_u64(mul[11],32);
	b[10]=vmovn_u64(mul[11]);
	mul[12]=vmlal_n_u32(mC,m,p[12]);
	mul[12]=vaddw_u32(mul[12],b[12]);
	mC=vshrq_n_u64(mul[12],32);
	b[11]=vmovn_u64(mul[12]);
	mul[13]=vmlal_n_u32(mC,m,p[13]);
	mul[13]=vaddw_u32(mul[13],b[13]);
	b[12]=vmovn_u64(mul[13]);
	mul[14]=vshrq_n_u64(mul[13],32);
	mul[14]=vaddw_u32(mul[14],b[14]);
	b[13]=vmovn_u64(mul[14]);
	mul[14]=vshrq_n_u64(mul[14],32);
	b[14]=vld1_u32(&A[2]);

	//Segunda ITeracion
	mC=vmull_n_u32(b[0],pi[0]);
	m=vmovn_u64(mC);
	mul[0]=vmull_n_u32(m,p[0]);
	mul[0]=vaddw_u32(mul[0],b[0]);
	mC=vshrq_n_u64(mul[0],32);
	mul[1]=vmlal_n_u32(mC,m,p[1]);
	mul[1]=vaddw_u32(mul[1],b[1]);
	mC=vshrq_n_u64(mul[1],32);
	b[0]=vmovn_u64(mul[1]);
	mul[2]=vmlal_n_u32(mC,m,p[2]);
	mul[2]=vaddw_u32(mul[2],b[2]);
	mC=vshrq_n_u64(mul[2],32);
	b[1]=vmovn_u64(mul[2]);
	mul[3]=vmlal_n_u32(mC,m,p[3]);
	mul[3]=vaddw_u32(mul[3],b[3]);
	mC=vshrq_n_u64(mul[3],32);
	b[2]=vmovn_u64(mul[3]);
	mul[4]=vmlal_n_u32(mC,m,p[4]);
	mul[4]=vaddw_u32(mul[4],b[4]);
	mC=vshrq_n_u64(mul[4],32);
	b[3]=vmovn_u64(mul[4]);
	mul[5]=vmlal_n_u32(mC,m,p[5]);
	mul[5]=vaddw_u32(mul[5],b[5]);
	mC=vshrq_n_u64(mul[5],32);
	b[4]=vmovn_u64(mul[5]);
	mul[6]=vmlal_n_u32(mC,m,p[6]);
	mul[6]=vaddw_u32(mul[6],b[6]);
	mC=vshrq_n_u64(mul[6],32);
	b[5]=vmovn_u64(mul[6]);
	mul[7]=vmlal_n_u32(mC,m,p[7]);
	mul[7]=vaddw_u32(mul[7],b[7]);
	mC=vshrq_n_u64(mul[7],32);
	b[6]=vmovn_u64(mul[7]);
	mul[8]=vmlal_n_u32(mC,m,p[8]);
	mul[8]=vaddw_u32(mul[8],b[8]);
	mC=vshrq_n_u64(mul[8],32);
	b[7]=vmovn_u64(mul[8]);
	mul[9]=vmlal_n_u32(mC,m,p[9]);
	mul[9]=vaddw_u32(mul[9],b[9]);
	mC=vshrq_n_u64(mul[9],32);
	b[8]=vmovn_u64(mul[9]);
	mul[10]=vmlal_n_u32(mC,m,p[10]);
	mul[10]=vaddw_u32(mul[10],b[10]);
	mC=vshrq_n_u64(mul[10],32);
	b[9]=vmovn_u64(mul[10]);
	mul[11]=vmlal_n_u32(mC,m,p[11]);
	mul[11]=vaddw_u32(mul[11],b[11]);
	mC=vshrq_n_u64(mul[11],32);
	b[10]=vmovn_u64(mul[11]);
	mul[12]=vmlal_n_u32(mC,m,p[12]);
	mul[12]=vaddw_u32(mul[12],b[12]);
	mC=vshrq_n_u64(mul[12],32);
	b[11]=vmovn_u64(mul[12]);
	mul[13]=vmlal_n_u32(mC,m,p[13]);
	mul[13]=vaddw_u32(mul[13],b[13]);
	b[12]=vmovn_u64(mul[13]);
	mC=vshrq_n_u64(mul[13],32);
	mul[14]=vaddq_u64(mul[14],mC);
	mul[14]=vaddw_u32(mul[14],b[14]);
	b[13]=vmovn_u64(mul[14]);
	mul[14]=vshrq_n_u64(mul[14],32);
	b[14]=vld1_u32(&A[4]);
	
	//Tercera ITeracion
	mC=vmull_n_u32(b[0],pi[0]);
	m=vmovn_u64(mC);
	mul[0]=vmull_n_u32(m,p[0]);
	mul[0]=vaddw_u32(mul[0],b[0]);
	mC=vshrq_n_u64(mul[0],32);
	mul[1]=vmlal_n_u32(mC,m,p[1]);
	mul[1]=vaddw_u32(mul[1],b[1]);
	mC=vshrq_n_u64(mul[1],32);
	b[0]=vmovn_u64(mul[1]);
	mul[2]=vmlal_n_u32(mC,m,p[2]);
	mul[2]=vaddw_u32(mul[2],b[2]);
	mC=vshrq_n_u64(mul[2],32);
	b[1]=vmovn_u64(mul[2]);
	mul[3]=vmlal_n_u32(mC,m,p[3]);
	mul[3]=vaddw_u32(mul[3],b[3]);
	mC=vshrq_n_u64(mul[3],32);
	b[2]=vmovn_u64(mul[3]);
	mul[4]=vmlal_n_u32(mC,m,p[4]);
	mul[4]=vaddw_u32(mul[4],b[4]);
	mC=vshrq_n_u64(mul[4],32);
	b[3]=vmovn_u64(mul[4]);
	mul[5]=vmlal_n_u32(mC,m,p[5]);
	mul[5]=vaddw_u32(mul[5],b[5]);
	mC=vshrq_n_u64(mul[5],32);
	b[4]=vmovn_u64(mul[5]);
	mul[6]=vmlal_n_u32(mC,m,p[6]);
	mul[6]=vaddw_u32(mul[6],b[6]);
	mC=vshrq_n_u64(mul[6],32);
	b[5]=vmovn_u64(mul[6]);
	mul[7]=vmlal_n_u32(mC,m,p[7]);
	mul[7]=vaddw_u32(mul[7],b[7]);
	mC=vshrq_n_u64(mul[7],32);
	b[6]=vmovn_u64(mul[7]);
	mul[8]=vmlal_n_u32(mC,m,p[8]);
	mul[8]=vaddw_u32(mul[8],b[8]);
	mC=vshrq_n_u64(mul[8],32);
	b[7]=vmovn_u64(mul[8]);
	mul[9]=vmlal_n_u32(mC,m,p[9]);
	mul[9]=vaddw_u32(mul[9],b[9]);
	mC=vshrq_n_u64(mul[9],32);
	b[8]=vmovn_u64(mul[9]);
	mul[10]=vmlal_n_u32(mC,m,p[10]);
	mul[10]=vaddw_u32(mul[10],b[10]);
	mC=vshrq_n_u64(mul[10],32);
	b[9]=vmovn_u64(mul[10]);
	mul[11]=vmlal_n_u32(mC,m,p[11]);
	mul[11]=vaddw_u32(mul[11],b[11]);
	mC=vshrq_n_u64(mul[11],32);
	b[10]=vmovn_u64(mul[11]);
	mul[12]=vmlal_n_u32(mC,m,p[12]);
	mul[12]=vaddw_u32(mul[12],b[12]);
	mC=vshrq_n_u64(mul[12],32);
	b[11]=vmovn_u64(mul[12]);
	mul[13]=vmlal_n_u32(mC,m,p[13]);
	mul[13]=vaddw_u32(mul[13],b[13]);
	b[12]=vmovn_u64(mul[13]);
	mC=vshrq_n_u64(mul[13],32);
	mul[14]=vaddq_u64(mul[14],mC);
	mul[14]=vaddw_u32(mul[14],b[14]);
	b[13]=vmovn_u64(mul[14]);
	mul[14]=vshrq_n_u64(mul[14],32);
	b[14]=vld1_u32(&A[6]);



	//Cuarta ITeracion
	mC=vmull_n_u32(b[0],pi[0]);
	m=vmovn_u64(mC);
	mul[0]=vmull_n_u32(m,p[0]);
	mul[0]=vaddw_u32(mul[0],b[0]);
	mC=vshrq_n_u64(mul[0],32);
	mul[1]=vmlal_n_u32(mC,m,p[1]);
	mul[1]=vaddw_u32(mul[1],b[1]);
	mC=vshrq_n_u64(mul[1],32);
	b[0]=vmovn_u64(mul[1]);
	mul[2]=vmlal_n_u32(mC,m,p[2]);
	mul[2]=vaddw_u32(mul[2],b[2]);
	mC=vshrq_n_u64(mul[2],32);
	b[1]=vmovn_u64(mul[2]);
	mul[3]=vmlal_n_u32(mC,m,p[3]);
	mul[3]=vaddw_u32(mul[3],b[3]);
	mC=vshrq_n_u64(mul[3],32);
	b[2]=vmovn_u64(mul[3]);
	mul[4]=vmlal_n_u32(mC,m,p[4]);
	mul[4]=vaddw_u32(mul[4],b[4]);
	mC=vshrq_n_u64(mul[4],32);
	b[3]=vmovn_u64(mul[4]);
	mul[5]=vmlal_n_u32(mC,m,p[5]);
	mul[5]=vaddw_u32(mul[5],b[5]);
	mC=vshrq_n_u64(mul[5],32);
	b[4]=vmovn_u64(mul[5]);
	mul[6]=vmlal_n_u32(mC,m,p[6]);
	mul[6]=vaddw_u32(mul[6],b[6]);
	mC=vshrq_n_u64(mul[6],32);
	b[5]=vmovn_u64(mul[6]);
	mul[7]=vmlal_n_u32(mC,m,p[7]);
	mul[7]=vaddw_u32(mul[7],b[7]);
	mC=vshrq_n_u64(mul[7],32);
	b[6]=vmovn_u64(mul[7]);
	mul[8]=vmlal_n_u32(mC,m,p[8]);
	mul[8]=vaddw_u32(mul[8],b[8]);
	mC=vshrq_n_u64(mul[8],32);
	b[7]=vmovn_u64(mul[8]);
	mul[9]=vmlal_n_u32(mC,m,p[9]);
	mul[9]=vaddw_u32(mul[9],b[9]);
	mC=vshrq_n_u64(mul[9],32);
	b[8]=vmovn_u64(mul[9]);
	mul[10]=vmlal_n_u32(mC,m,p[10]);
	mul[10]=vaddw_u32(mul[10],b[10]);
	mC=vshrq_n_u64(mul[10],32);
	b[9]=vmovn_u64(mul[10]);
	mul[11]=vmlal_n_u32(mC,m,p[11]);
	mul[11]=vaddw_u32(mul[11],b[11]);
	mC=vshrq_n_u64(mul[11],32);
	b[10]=vmovn_u64(mul[11]);
	mul[12]=vmlal_n_u32(mC,m,p[12]);
	mul[12]=vaddw_u32(mul[12],b[12]);
	mC=vshrq_n_u64(mul[12],32);
	b[11]=vmovn_u64(mul[12]);
	mul[13]=vmlal_n_u32(mC,m,p[13]);
	mul[13]=vaddw_u32(mul[13],b[13]);
	b[12]=vmovn_u64(mul[13]);
	mC=vshrq_n_u64(mul[13],32);
	mul[14]=vaddq_u64(mul[14],mC);
	mul[14]=vaddw_u32(mul[14],b[14]);
	b[13]=vmovn_u64(mul[14]);
	mul[14]=vshrq_n_u64(mul[14],32);
	b[14]=vld1_u32(&A[8]);


	//Quinta ITeracion
	mC=vmull_n_u32(b[0],pi[0]);
	m=vmovn_u64(mC);
	mul[0]=vmull_n_u32(m,p[0]);
	mul[0]=vaddw_u32(mul[0],b[0]);
	mC=vshrq_n_u64(mul[0],32);
	mul[1]=vmlal_n_u32(mC,m,p[1]);
	mul[1]=vaddw_u32(mul[1],b[1]);
	mC=vshrq_n_u64(mul[1],32);
	b[0]=vmovn_u64(mul[1]);
	mul[2]=vmlal_n_u32(mC,m,p[2]);
	mul[2]=vaddw_u32(mul[2],b[2]);
	mC=vshrq_n_u64(mul[2],32);
	b[1]=vmovn_u64(mul[2]);
	mul[3]=vmlal_n_u32(mC,m,p[3]);
	mul[3]=vaddw_u32(mul[3],b[3]);
	mC=vshrq_n_u64(mul[3],32);
	b[2]=vmovn_u64(mul[3]);
	mul[4]=vmlal_n_u32(mC,m,p[4]);
	mul[4]=vaddw_u32(mul[4],b[4]);
	mC=vshrq_n_u64(mul[4],32);
	b[3]=vmovn_u64(mul[4]);
	mul[5]=vmlal_n_u32(mC,m,p[5]);
	mul[5]=vaddw_u32(mul[5],b[5]);
	mC=vshrq_n_u64(mul[5],32);
	b[4]=vmovn_u64(mul[5]);
	mul[6]=vmlal_n_u32(mC,m,p[6]);
	mul[6]=vaddw_u32(mul[6],b[6]);
	mC=vshrq_n_u64(mul[6],32);
	b[5]=vmovn_u64(mul[6]);
	mul[7]=vmlal_n_u32(mC,m,p[7]);
	mul[7]=vaddw_u32(mul[7],b[7]);
	mC=vshrq_n_u64(mul[7],32);
	b[6]=vmovn_u64(mul[7]);
	mul[8]=vmlal_n_u32(mC,m,p[8]);
	mul[8]=vaddw_u32(mul[8],b[8]);
	mC=vshrq_n_u64(mul[8],32);
	b[7]=vmovn_u64(mul[8]);
	mul[9]=vmlal_n_u32(mC,m,p[9]);
	mul[9]=vaddw_u32(mul[9],b[9]);
	mC=vshrq_n_u64(mul[9],32);
	b[8]=vmovn_u64(mul[9]);
	mul[10]=vmlal_n_u32(mC,m,p[10]);
	mul[10]=vaddw_u32(mul[10],b[10]);
	mC=vshrq_n_u64(mul[10],32);
	b[9]=vmovn_u64(mul[10]);
	mul[11]=vmlal_n_u32(mC,m,p[11]);
	mul[11]=vaddw_u32(mul[11],b[11]);
	mC=vshrq_n_u64(mul[11],32);
	b[10]=vmovn_u64(mul[11]);
	mul[12]=vmlal_n_u32(mC,m,p[12]);
	mul[12]=vaddw_u32(mul[12],b[12]);
	mC=vshrq_n_u64(mul[12],32);
	b[11]=vmovn_u64(mul[12]);
	mul[13]=vmlal_n_u32(mC,m,p[13]);
	mul[13]=vaddw_u32(mul[13],b[13]);
	b[12]=vmovn_u64(mul[13]);
	mC=vshrq_n_u64(mul[13],32);
	mul[14]=vaddq_u64(mul[14],mC);
	mul[14]=vaddw_u32(mul[14],b[14]);
	b[13]=vmovn_u64(mul[14]);
	mul[14]=vshrq_n_u64(mul[14],32);
	b[14]=vld1_u32(&A[10]);

	//Sexta ITeracion
	mC=vmull_n_u32(b[0],pi[0]);
	m=vmovn_u64(mC);
	mul[0]=vmull_n_u32(m,p[0]);
	mul[0]=vaddw_u32(mul[0],b[0]);
	mC=vshrq_n_u64(mul[0],32);
	mul[1]=vmlal_n_u32(mC,m,p[1]);
	mul[1]=vaddw_u32(mul[1],b[1]);
	mC=vshrq_n_u64(mul[1],32);
	b[0]=vmovn_u64(mul[1]);
	mul[2]=vmlal_n_u32(mC,m,p[2]);
	mul[2]=vaddw_u32(mul[2],b[2]);
	mC=vshrq_n_u64(mul[2],32);
	b[1]=vmovn_u64(mul[2]);
	mul[3]=vmlal_n_u32(mC,m,p[3]);
	mul[3]=vaddw_u32(mul[3],b[3]);
	mC=vshrq_n_u64(mul[3],32);
	b[2]=vmovn_u64(mul[3]);
	mul[4]=vmlal_n_u32(mC,m,p[4]);
	mul[4]=vaddw_u32(mul[4],b[4]);
	mC=vshrq_n_u64(mul[4],32);
	b[3]=vmovn_u64(mul[4]);
	mul[5]=vmlal_n_u32(mC,m,p[5]);
	mul[5]=vaddw_u32(mul[5],b[5]);
	mC=vshrq_n_u64(mul[5],32);
	b[4]=vmovn_u64(mul[5]);
	mul[6]=vmlal_n_u32(mC,m,p[6]);
	mul[6]=vaddw_u32(mul[6],b[6]);
	mC=vshrq_n_u64(mul[6],32);
	b[5]=vmovn_u64(mul[6]);
	mul[7]=vmlal_n_u32(mC,m,p[7]);
	mul[7]=vaddw_u32(mul[7],b[7]);
	mC=vshrq_n_u64(mul[7],32);
	b[6]=vmovn_u64(mul[7]);
	mul[8]=vmlal_n_u32(mC,m,p[8]);
	mul[8]=vaddw_u32(mul[8],b[8]);
	mC=vshrq_n_u64(mul[8],32);
	b[7]=vmovn_u64(mul[8]);
	mul[9]=vmlal_n_u32(mC,m,p[9]);
	mul[9]=vaddw_u32(mul[9],b[9]);
	mC=vshrq_n_u64(mul[9],32);
	b[8]=vmovn_u64(mul[9]);
	mul[10]=vmlal_n_u32(mC,m,p[10]);
	mul[10]=vaddw_u32(mul[10],b[10]);
	mC=vshrq_n_u64(mul[10],32);
	b[9]=vmovn_u64(mul[10]);
	mul[11]=vmlal_n_u32(mC,m,p[11]);
	mul[11]=vaddw_u32(mul[11],b[11]);
	mC=vshrq_n_u64(mul[11],32);
	b[10]=vmovn_u64(mul[11]);
	mul[12]=vmlal_n_u32(mC,m,p[12]);
	mul[12]=vaddw_u32(mul[12],b[12]);
	mC=vshrq_n_u64(mul[12],32);
	b[11]=vmovn_u64(mul[12]);
	mul[13]=vmlal_n_u32(mC,m,p[13]);
	mul[13]=vaddw_u32(mul[13],b[13]);
	b[12]=vmovn_u64(mul[13]);
	mC=vshrq_n_u64(mul[13],32);
	mul[14]=vaddq_u64(mul[14],mC);
	mul[14]=vaddw_u32(mul[14],b[14]);
	b[13]=vmovn_u64(mul[14]);
	mul[14]=vshrq_n_u64(mul[14],32);
	b[14]=vld1_u32(&A[12]);


	//Septima ITeracion
	mC=vmull_n_u32(b[0],pi[0]);
	m=vmovn_u64(mC);
	mul[0]=vmull_n_u32(m,p[0]);
	mul[0]=vaddw_u32(mul[0],b[0]);
	mC=vshrq_n_u64(mul[0],32);
	mul[1]=vmlal_n_u32(mC,m,p[1]);
	mul[1]=vaddw_u32(mul[1],b[1]);
	mC=vshrq_n_u64(mul[1],32);
	b[0]=vmovn_u64(mul[1]);
	mul[2]=vmlal_n_u32(mC,m,p[2]);
	mul[2]=vaddw_u32(mul[2],b[2]);
	mC=vshrq_n_u64(mul[2],32);
	b[1]=vmovn_u64(mul[2]);
	mul[3]=vmlal_n_u32(mC,m,p[3]);
	mul[3]=vaddw_u32(mul[3],b[3]);
	mC=vshrq_n_u64(mul[3],32);
	b[2]=vmovn_u64(mul[3]);
	mul[4]=vmlal_n_u32(mC,m,p[4]);
	mul[4]=vaddw_u32(mul[4],b[4]);
	mC=vshrq_n_u64(mul[4],32);
	b[3]=vmovn_u64(mul[4]);
	mul[5]=vmlal_n_u32(mC,m,p[5]);
	mul[5]=vaddw_u32(mul[5],b[5]);
	mC=vshrq_n_u64(mul[5],32);
	b[4]=vmovn_u64(mul[5]);
	mul[6]=vmlal_n_u32(mC,m,p[6]);
	mul[6]=vaddw_u32(mul[6],b[6]);
	mC=vshrq_n_u64(mul[6],32);
	b[5]=vmovn_u64(mul[6]);
	mul[7]=vmlal_n_u32(mC,m,p[7]);
	mul[7]=vaddw_u32(mul[7],b[7]);
	mC=vshrq_n_u64(mul[7],32);
	b[6]=vmovn_u64(mul[7]);
	mul[8]=vmlal_n_u32(mC,m,p[8]);
	mul[8]=vaddw_u32(mul[8],b[8]);
	mC=vshrq_n_u64(mul[8],32);
	b[7]=vmovn_u64(mul[8]);
	mul[9]=vmlal_n_u32(mC,m,p[9]);
	mul[9]=vaddw_u32(mul[9],b[9]);
	mC=vshrq_n_u64(mul[9],32);
	b[8]=vmovn_u64(mul[9]);
	mul[10]=vmlal_n_u32(mC,m,p[10]);
	mul[10]=vaddw_u32(mul[10],b[10]);
	mC=vshrq_n_u64(mul[10],32);
	b[9]=vmovn_u64(mul[10]);
	mul[11]=vmlal_n_u32(mC,m,p[11]);
	mul[11]=vaddw_u32(mul[11],b[11]);
	mC=vshrq_n_u64(mul[11],32);
	b[10]=vmovn_u64(mul[11]);
	mul[12]=vmlal_n_u32(mC,m,p[12]);
	mul[12]=vaddw_u32(mul[12],b[12]);
	mC=vshrq_n_u64(mul[12],32);
	b[11]=vmovn_u64(mul[12]);
	mul[13]=vmlal_n_u32(mC,m,p[13]);
	mul[13]=vaddw_u32(mul[13],b[13]);
	b[12]=vmovn_u64(mul[13]);
	mC=vshrq_n_u64(mul[13],32);
	mul[14]=vaddq_u64(mul[14],mC);
	mul[14]=vaddw_u32(mul[14],b[14]);
	b[13]=vmovn_u64(mul[14]);
	mul[14]=vshrq_n_u64(mul[14],32);
	b[14]=vld1_u32(&A[14]);

		mC=vmull_n_u32(b[0],pi[0]);
	m=vmovn_u64(mC);
	mul[0]=vmull_n_u32(m,p[0]);
	mul[0]=vaddw_u32(mul[0],b[0]);
	mC=vshrq_n_u64(mul[0],32);
	mul[1]=vmlal_n_u32(mC,m,p[1]);
	mul[1]=vaddw_u32(mul[1],b[1]);
	mC=vshrq_n_u64(mul[1],32);
	b[0]=vmovn_u64(mul[1]);
	mul[2]=vmlal_n_u32(mC,m,p[2]);
	mul[2]=vaddw_u32(mul[2],b[2]);
	mC=vshrq_n_u64(mul[2],32);
	b[1]=vmovn_u64(mul[2]);
	mul[3]=vmlal_n_u32(mC,m,p[3]);
	mul[3]=vaddw_u32(mul[3],b[3]);
	mC=vshrq_n_u64(mul[3],32);
	b[2]=vmovn_u64(mul[3]);
	mul[4]=vmlal_n_u32(mC,m,p[4]);
	mul[4]=vaddw_u32(mul[4],b[4]);
	mC=vshrq_n_u64(mul[4],32);
	b[3]=vmovn_u64(mul[4]);
	mul[5]=vmlal_n_u32(mC,m,p[5]);
	mul[5]=vaddw_u32(mul[5],b[5]);
	mC=vshrq_n_u64(mul[5],32);
	b[4]=vmovn_u64(mul[5]);
	mul[6]=vmlal_n_u32(mC,m,p[6]);
	mul[6]=vaddw_u32(mul[6],b[6]);
	mC=vshrq_n_u64(mul[6],32);
	b[5]=vmovn_u64(mul[6]);
	mul[7]=vmlal_n_u32(mC,m,p[7]);
	mul[7]=vaddw_u32(mul[7],b[7]);
	mC=vshrq_n_u64(mul[7],32);
	b[6]=vmovn_u64(mul[7]);
	mul[8]=vmlal_n_u32(mC,m,p[8]);
	mul[8]=vaddw_u32(mul[8],b[8]);
	mC=vshrq_n_u64(mul[8],32);
	b[7]=vmovn_u64(mul[8]);
	mul[9]=vmlal_n_u32(mC,m,p[9]);
	mul[9]=vaddw_u32(mul[9],b[9]);
	mC=vshrq_n_u64(mul[9],32);
	b[8]=vmovn_u64(mul[9]);
	mul[10]=vmlal_n_u32(mC,m,p[10]);
	mul[10]=vaddw_u32(mul[10],b[10]);
	mC=vshrq_n_u64(mul[10],32);
	b[9]=vmovn_u64(mul[10]);
	mul[11]=vmlal_n_u32(mC,m,p[11]);
	mul[11]=vaddw_u32(mul[11],b[11]);
	mC=vshrq_n_u64(mul[11],32);
	b[10]=vmovn_u64(mul[11]);
	mul[12]=vmlal_n_u32(mC,m,p[12]);
	mul[12]=vaddw_u32(mul[12],b[12]);
	mC=vshrq_n_u64(mul[12],32);
	b[11]=vmovn_u64(mul[12]);
	mul[13]=vmlal_n_u32(mC,m,p[13]);
	mul[13]=vaddw_u32(mul[13],b[13]);
	b[12]=vmovn_u64(mul[13]);
	mC=vshrq_n_u64(mul[13],32);
	mul[14]=vaddq_u64(mul[14],mC);
	mul[14]=vaddw_u32(mul[14],b[14]);
	b[13]=vmovn_u64(mul[14]);
	mul[14]=vshrq_n_u64(mul[14],32);
	b[14]=vld1_u32(&A[16]);
	
		mC=vmull_n_u32(b[0],pi[0]);
	m=vmovn_u64(mC);
	mul[0]=vmull_n_u32(m,p[0]);
	mul[0]=vaddw_u32(mul[0],b[0]);
	mC=vshrq_n_u64(mul[0],32);
	mul[1]=vmlal_n_u32(mC,m,p[1]);
	mul[1]=vaddw_u32(mul[1],b[1]);
	mC=vshrq_n_u64(mul[1],32);
	b[0]=vmovn_u64(mul[1]);
	mul[2]=vmlal_n_u32(mC,m,p[2]);
	mul[2]=vaddw_u32(mul[2],b[2]);
	mC=vshrq_n_u64(mul[2],32);
	b[1]=vmovn_u64(mul[2]);
	mul[3]=vmlal_n_u32(mC,m,p[3]);
	mul[3]=vaddw_u32(mul[3],b[3]);
	mC=vshrq_n_u64(mul[3],32);
	b[2]=vmovn_u64(mul[3]);
	mul[4]=vmlal_n_u32(mC,m,p[4]);
	mul[4]=vaddw_u32(mul[4],b[4]);
	mC=vshrq_n_u64(mul[4],32);
	b[3]=vmovn_u64(mul[4]);
	mul[5]=vmlal_n_u32(mC,m,p[5]);
	mul[5]=vaddw_u32(mul[5],b[5]);
	mC=vshrq_n_u64(mul[5],32);
	b[4]=vmovn_u64(mul[5]);
	mul[6]=vmlal_n_u32(mC,m,p[6]);
	mul[6]=vaddw_u32(mul[6],b[6]);
	mC=vshrq_n_u64(mul[6],32);
	b[5]=vmovn_u64(mul[6]);
	mul[7]=vmlal_n_u32(mC,m,p[7]);
	mul[7]=vaddw_u32(mul[7],b[7]);
	mC=vshrq_n_u64(mul[7],32);
	b[6]=vmovn_u64(mul[7]);
	mul[8]=vmlal_n_u32(mC,m,p[8]);
	mul[8]=vaddw_u32(mul[8],b[8]);
	mC=vshrq_n_u64(mul[8],32);
	b[7]=vmovn_u64(mul[8]);
	mul[9]=vmlal_n_u32(mC,m,p[9]);
	mul[9]=vaddw_u32(mul[9],b[9]);
	mC=vshrq_n_u64(mul[9],32);
	b[8]=vmovn_u64(mul[9]);
	mul[10]=vmlal_n_u32(mC,m,p[10]);
	mul[10]=vaddw_u32(mul[10],b[10]);
	mC=vshrq_n_u64(mul[10],32);
	b[9]=vmovn_u64(mul[10]);
	mul[11]=vmlal_n_u32(mC,m,p[11]);
	mul[11]=vaddw_u32(mul[11],b[11]);
	mC=vshrq_n_u64(mul[11],32);
	b[10]=vmovn_u64(mul[11]);
	mul[12]=vmlal_n_u32(mC,m,p[12]);
	mul[12]=vaddw_u32(mul[12],b[12]);
	mC=vshrq_n_u64(mul[12],32);
	b[11]=vmovn_u64(mul[12]);
	mul[13]=vmlal_n_u32(mC,m,p[13]);
	mul[13]=vaddw_u32(mul[13],b[13]);
	b[12]=vmovn_u64(mul[13]);
	mC=vshrq_n_u64(mul[13],32);
	mul[14]=vaddq_u64(mul[14],mC);
	mul[14]=vaddw_u32(mul[14],b[14]);
	b[13]=vmovn_u64(mul[14]);
	mul[14]=vshrq_n_u64(mul[14],32);
	b[14]=vld1_u32(&A[18]);
	
		mC=vmull_n_u32(b[0],pi[0]);
	m=vmovn_u64(mC);
	mul[0]=vmull_n_u32(m,p[0]);
	mul[0]=vaddw_u32(mul[0],b[0]);
	mC=vshrq_n_u64(mul[0],32);
	mul[1]=vmlal_n_u32(mC,m,p[1]);
	mul[1]=vaddw_u32(mul[1],b[1]);
	mC=vshrq_n_u64(mul[1],32);
	b[0]=vmovn_u64(mul[1]);
	mul[2]=vmlal_n_u32(mC,m,p[2]);
	mul[2]=vaddw_u32(mul[2],b[2]);
	mC=vshrq_n_u64(mul[2],32);
	b[1]=vmovn_u64(mul[2]);
	mul[3]=vmlal_n_u32(mC,m,p[3]);
	mul[3]=vaddw_u32(mul[3],b[3]);
	mC=vshrq_n_u64(mul[3],32);
	b[2]=vmovn_u64(mul[3]);
	mul[4]=vmlal_n_u32(mC,m,p[4]);
	mul[4]=vaddw_u32(mul[4],b[4]);
	mC=vshrq_n_u64(mul[4],32);
	b[3]=vmovn_u64(mul[4]);
	mul[5]=vmlal_n_u32(mC,m,p[5]);
	mul[5]=vaddw_u32(mul[5],b[5]);
	mC=vshrq_n_u64(mul[5],32);
	b[4]=vmovn_u64(mul[5]);
	mul[6]=vmlal_n_u32(mC,m,p[6]);
	mul[6]=vaddw_u32(mul[6],b[6]);
	mC=vshrq_n_u64(mul[6],32);
	b[5]=vmovn_u64(mul[6]);
	mul[7]=vmlal_n_u32(mC,m,p[7]);
	mul[7]=vaddw_u32(mul[7],b[7]);
	mC=vshrq_n_u64(mul[7],32);
	b[6]=vmovn_u64(mul[7]);
	mul[8]=vmlal_n_u32(mC,m,p[8]);
	mul[8]=vaddw_u32(mul[8],b[8]);
	mC=vshrq_n_u64(mul[8],32);
	b[7]=vmovn_u64(mul[8]);
	mul[9]=vmlal_n_u32(mC,m,p[9]);
	mul[9]=vaddw_u32(mul[9],b[9]);
	mC=vshrq_n_u64(mul[9],32);
	b[8]=vmovn_u64(mul[9]);
	mul[10]=vmlal_n_u32(mC,m,p[10]);
	mul[10]=vaddw_u32(mul[10],b[10]);
	mC=vshrq_n_u64(mul[10],32);
	b[9]=vmovn_u64(mul[10]);
	mul[11]=vmlal_n_u32(mC,m,p[11]);
	mul[11]=vaddw_u32(mul[11],b[11]);
	mC=vshrq_n_u64(mul[11],32);
	b[10]=vmovn_u64(mul[11]);
	mul[12]=vmlal_n_u32(mC,m,p[12]);
	mul[12]=vaddw_u32(mul[12],b[12]);
	mC=vshrq_n_u64(mul[12],32);
	b[11]=vmovn_u64(mul[12]);
	mul[13]=vmlal_n_u32(mC,m,p[13]);
	mul[13]=vaddw_u32(mul[13],b[13]);
	b[12]=vmovn_u64(mul[13]);
	mC=vshrq_n_u64(mul[13],32);
	mul[14]=vaddq_u64(mul[14],mC);
	mul[14]=vaddw_u32(mul[14],b[14]);
	b[13]=vmovn_u64(mul[14]);
	mul[14]=vshrq_n_u64(mul[14],32);
	b[14]=vld1_u32(&A[20]);
	
		mC=vmull_n_u32(b[0],pi[0]);
	m=vmovn_u64(mC);
	mul[0]=vmull_n_u32(m,p[0]);
	mul[0]=vaddw_u32(mul[0],b[0]);
	mC=vshrq_n_u64(mul[0],32);
	mul[1]=vmlal_n_u32(mC,m,p[1]);
	mul[1]=vaddw_u32(mul[1],b[1]);
	mC=vshrq_n_u64(mul[1],32);
	b[0]=vmovn_u64(mul[1]);
	mul[2]=vmlal_n_u32(mC,m,p[2]);
	mul[2]=vaddw_u32(mul[2],b[2]);
	mC=vshrq_n_u64(mul[2],32);
	b[1]=vmovn_u64(mul[2]);
	mul[3]=vmlal_n_u32(mC,m,p[3]);
	mul[3]=vaddw_u32(mul[3],b[3]);
	mC=vshrq_n_u64(mul[3],32);
	b[2]=vmovn_u64(mul[3]);
	mul[4]=vmlal_n_u32(mC,m,p[4]);
	mul[4]=vaddw_u32(mul[4],b[4]);
	mC=vshrq_n_u64(mul[4],32);
	b[3]=vmovn_u64(mul[4]);
	mul[5]=vmlal_n_u32(mC,m,p[5]);
	mul[5]=vaddw_u32(mul[5],b[5]);
	mC=vshrq_n_u64(mul[5],32);
	b[4]=vmovn_u64(mul[5]);
	mul[6]=vmlal_n_u32(mC,m,p[6]);
	mul[6]=vaddw_u32(mul[6],b[6]);
	mC=vshrq_n_u64(mul[6],32);
	b[5]=vmovn_u64(mul[6]);
	mul[7]=vmlal_n_u32(mC,m,p[7]);
	mul[7]=vaddw_u32(mul[7],b[7]);
	mC=vshrq_n_u64(mul[7],32);
	b[6]=vmovn_u64(mul[7]);
	mul[8]=vmlal_n_u32(mC,m,p[8]);
	mul[8]=vaddw_u32(mul[8],b[8]);
	mC=vshrq_n_u64(mul[8],32);
	b[7]=vmovn_u64(mul[8]);
	mul[9]=vmlal_n_u32(mC,m,p[9]);
	mul[9]=vaddw_u32(mul[9],b[9]);
	mC=vshrq_n_u64(mul[9],32);
	b[8]=vmovn_u64(mul[9]);
	mul[10]=vmlal_n_u32(mC,m,p[10]);
	mul[10]=vaddw_u32(mul[10],b[10]);
	mC=vshrq_n_u64(mul[10],32);
	b[9]=vmovn_u64(mul[10]);
	mul[11]=vmlal_n_u32(mC,m,p[11]);
	mul[11]=vaddw_u32(mul[11],b[11]);
	mC=vshrq_n_u64(mul[11],32);
	b[10]=vmovn_u64(mul[11]);
	mul[12]=vmlal_n_u32(mC,m,p[12]);
	mul[12]=vaddw_u32(mul[12],b[12]);
	mC=vshrq_n_u64(mul[12],32);
	b[11]=vmovn_u64(mul[12]);
	mul[13]=vmlal_n_u32(mC,m,p[13]);
	mul[13]=vaddw_u32(mul[13],b[13]);
	b[12]=vmovn_u64(mul[13]);
	mC=vshrq_n_u64(mul[13],32);
	mul[14]=vaddq_u64(mul[14],mC);
	mul[14]=vaddw_u32(mul[14],b[14]);
	b[13]=vmovn_u64(mul[14]);
	mul[14]=vshrq_n_u64(mul[14],32);
	b[14]=vld1_u32(&A[22]);
	
		mC=vmull_n_u32(b[0],pi[0]);
	m=vmovn_u64(mC);
	mul[0]=vmull_n_u32(m,p[0]);
	mul[0]=vaddw_u32(mul[0],b[0]);
	mC=vshrq_n_u64(mul[0],32);
	mul[1]=vmlal_n_u32(mC,m,p[1]);
	mul[1]=vaddw_u32(mul[1],b[1]);
	mC=vshrq_n_u64(mul[1],32);
	b[0]=vmovn_u64(mul[1]);
	mul[2]=vmlal_n_u32(mC,m,p[2]);
	mul[2]=vaddw_u32(mul[2],b[2]);
	mC=vshrq_n_u64(mul[2],32);
	b[1]=vmovn_u64(mul[2]);
	mul[3]=vmlal_n_u32(mC,m,p[3]);
	mul[3]=vaddw_u32(mul[3],b[3]);
	mC=vshrq_n_u64(mul[3],32);
	b[2]=vmovn_u64(mul[3]);
	mul[4]=vmlal_n_u32(mC,m,p[4]);
	mul[4]=vaddw_u32(mul[4],b[4]);
	mC=vshrq_n_u64(mul[4],32);
	b[3]=vmovn_u64(mul[4]);
	mul[5]=vmlal_n_u32(mC,m,p[5]);
	mul[5]=vaddw_u32(mul[5],b[5]);
	mC=vshrq_n_u64(mul[5],32);
	b[4]=vmovn_u64(mul[5]);
	mul[6]=vmlal_n_u32(mC,m,p[6]);
	mul[6]=vaddw_u32(mul[6],b[6]);
	mC=vshrq_n_u64(mul[6],32);
	b[5]=vmovn_u64(mul[6]);
	mul[7]=vmlal_n_u32(mC,m,p[7]);
	mul[7]=vaddw_u32(mul[7],b[7]);
	mC=vshrq_n_u64(mul[7],32);
	b[6]=vmovn_u64(mul[7]);
	mul[8]=vmlal_n_u32(mC,m,p[8]);
	mul[8]=vaddw_u32(mul[8],b[8]);
	mC=vshrq_n_u64(mul[8],32);
	b[7]=vmovn_u64(mul[8]);
	mul[9]=vmlal_n_u32(mC,m,p[9]);
	mul[9]=vaddw_u32(mul[9],b[9]);
	mC=vshrq_n_u64(mul[9],32);
	b[8]=vmovn_u64(mul[9]);
	mul[10]=vmlal_n_u32(mC,m,p[10]);
	mul[10]=vaddw_u32(mul[10],b[10]);
	mC=vshrq_n_u64(mul[10],32);
	b[9]=vmovn_u64(mul[10]);
	mul[11]=vmlal_n_u32(mC,m,p[11]);
	mul[11]=vaddw_u32(mul[11],b[11]);
	mC=vshrq_n_u64(mul[11],32);
	b[10]=vmovn_u64(mul[11]);
	mul[12]=vmlal_n_u32(mC,m,p[12]);
	mul[12]=vaddw_u32(mul[12],b[12]);
	mC=vshrq_n_u64(mul[12],32);
	b[11]=vmovn_u64(mul[12]);
	mul[13]=vmlal_n_u32(mC,m,p[13]);
	mul[13]=vaddw_u32(mul[13],b[13]);
	b[12]=vmovn_u64(mul[13]);
	mC=vshrq_n_u64(mul[13],32);
	mul[14]=vaddq_u64(mul[14],mC);
	mul[14]=vaddw_u32(mul[14],b[14]);
	b[13]=vmovn_u64(mul[14]);
	mul[14]=vshrq_n_u64(mul[14],32);
	b[14]=vld1_u32(&A[24]);
	
		mC=vmull_n_u32(b[0],pi[0]);
	m=vmovn_u64(mC);
	mul[0]=vmull_n_u32(m,p[0]);
	mul[0]=vaddw_u32(mul[0],b[0]);
	mC=vshrq_n_u64(mul[0],32);
	mul[1]=vmlal_n_u32(mC,m,p[1]);
	mul[1]=vaddw_u32(mul[1],b[1]);
	mC=vshrq_n_u64(mul[1],32);
	b[0]=vmovn_u64(mul[1]);
	mul[2]=vmlal_n_u32(mC,m,p[2]);
	mul[2]=vaddw_u32(mul[2],b[2]);
	mC=vshrq_n_u64(mul[2],32);
	b[1]=vmovn_u64(mul[2]);
	mul[3]=vmlal_n_u32(mC,m,p[3]);
	mul[3]=vaddw_u32(mul[3],b[3]);
	mC=vshrq_n_u64(mul[3],32);
	b[2]=vmovn_u64(mul[3]);
	mul[4]=vmlal_n_u32(mC,m,p[4]);
	mul[4]=vaddw_u32(mul[4],b[4]);
	mC=vshrq_n_u64(mul[4],32);
	b[3]=vmovn_u64(mul[4]);
	mul[5]=vmlal_n_u32(mC,m,p[5]);
	mul[5]=vaddw_u32(mul[5],b[5]);
	mC=vshrq_n_u64(mul[5],32);
	b[4]=vmovn_u64(mul[5]);
	mul[6]=vmlal_n_u32(mC,m,p[6]);
	mul[6]=vaddw_u32(mul[6],b[6]);
	mC=vshrq_n_u64(mul[6],32);
	b[5]=vmovn_u64(mul[6]);
	mul[7]=vmlal_n_u32(mC,m,p[7]);
	mul[7]=vaddw_u32(mul[7],b[7]);
	mC=vshrq_n_u64(mul[7],32);
	b[6]=vmovn_u64(mul[7]);
	mul[8]=vmlal_n_u32(mC,m,p[8]);
	mul[8]=vaddw_u32(mul[8],b[8]);
	mC=vshrq_n_u64(mul[8],32);
	b[7]=vmovn_u64(mul[8]);
	mul[9]=vmlal_n_u32(mC,m,p[9]);
	mul[9]=vaddw_u32(mul[9],b[9]);
	mC=vshrq_n_u64(mul[9],32);
	b[8]=vmovn_u64(mul[9]);
	mul[10]=vmlal_n_u32(mC,m,p[10]);
	mul[10]=vaddw_u32(mul[10],b[10]);
	mC=vshrq_n_u64(mul[10],32);
	b[9]=vmovn_u64(mul[10]);
	mul[11]=vmlal_n_u32(mC,m,p[11]);
	mul[11]=vaddw_u32(mul[11],b[11]);
	mC=vshrq_n_u64(mul[11],32);
	b[10]=vmovn_u64(mul[11]);
	mul[12]=vmlal_n_u32(mC,m,p[12]);
	mul[12]=vaddw_u32(mul[12],b[12]);
	mC=vshrq_n_u64(mul[12],32);
	b[11]=vmovn_u64(mul[12]);
	mul[13]=vmlal_n_u32(mC,m,p[13]);
	mul[13]=vaddw_u32(mul[13],b[13]);
	b[12]=vmovn_u64(mul[13]);
	mC=vshrq_n_u64(mul[13],32);
	mul[14]=vaddq_u64(mul[14],mC);
	mul[14]=vaddw_u32(mul[14],b[14]);
	b[13]=vmovn_u64(mul[14]);
	mul[14]=vshrq_n_u64(mul[14],32);
	b[14]=vld1_u32(&A[26]);
	
		mC=vmull_n_u32(b[0],pi[0]);
	m=vmovn_u64(mC);
	mul[0]=vmull_n_u32(m,p[0]);
	mul[0]=vaddw_u32(mul[0],b[0]);
	mC=vshrq_n_u64(mul[0],32);
	mul[1]=vmlal_n_u32(mC,m,p[1]);
	mul[1]=vaddw_u32(mul[1],b[1]);
	mC=vshrq_n_u64(mul[1],32);
	b[0]=vmovn_u64(mul[1]);
	mul[2]=vmlal_n_u32(mC,m,p[2]);
	mul[2]=vaddw_u32(mul[2],b[2]);
	mC=vshrq_n_u64(mul[2],32);
	b[1]=vmovn_u64(mul[2]);
	mul[3]=vmlal_n_u32(mC,m,p[3]);
	mul[3]=vaddw_u32(mul[3],b[3]);
	mC=vshrq_n_u64(mul[3],32);
	b[2]=vmovn_u64(mul[3]);
	mul[4]=vmlal_n_u32(mC,m,p[4]);
	mul[4]=vaddw_u32(mul[4],b[4]);
	mC=vshrq_n_u64(mul[4],32);
	b[3]=vmovn_u64(mul[4]);
	mul[5]=vmlal_n_u32(mC,m,p[5]);
	mul[5]=vaddw_u32(mul[5],b[5]);
	mC=vshrq_n_u64(mul[5],32);
	b[4]=vmovn_u64(mul[5]);
	mul[6]=vmlal_n_u32(mC,m,p[6]);
	mul[6]=vaddw_u32(mul[6],b[6]);
	mC=vshrq_n_u64(mul[6],32);
	b[5]=vmovn_u64(mul[6]);
	mul[7]=vmlal_n_u32(mC,m,p[7]);
	mul[7]=vaddw_u32(mul[7],b[7]);
	mC=vshrq_n_u64(mul[7],32);
	b[6]=vmovn_u64(mul[7]);
	mul[8]=vmlal_n_u32(mC,m,p[8]);
	mul[8]=vaddw_u32(mul[8],b[8]);
	mC=vshrq_n_u64(mul[8],32);
	b[7]=vmovn_u64(mul[8]);
	mul[9]=vmlal_n_u32(mC,m,p[9]);
	mul[9]=vaddw_u32(mul[9],b[9]);
	mC=vshrq_n_u64(mul[9],32);
	b[8]=vmovn_u64(mul[9]);
	mul[10]=vmlal_n_u32(mC,m,p[10]);
	mul[10]=vaddw_u32(mul[10],b[10]);
	mC=vshrq_n_u64(mul[10],32);
	b[9]=vmovn_u64(mul[10]);
	mul[11]=vmlal_n_u32(mC,m,p[11]);
	mul[11]=vaddw_u32(mul[11],b[11]);
	mC=vshrq_n_u64(mul[11],32);
	b[10]=vmovn_u64(mul[11]);
	mul[12]=vmlal_n_u32(mC,m,p[12]);
	mul[12]=vaddw_u32(mul[12],b[12]);
	mC=vshrq_n_u64(mul[12],32);
	b[11]=vmovn_u64(mul[12]);
	mul[13]=vmlal_n_u32(mC,m,p[13]);
	mul[13]=vaddw_u32(mul[13],b[13]);
	b[12]=vmovn_u64(mul[13]);
	mC=vshrq_n_u64(mul[13],32);
	mul[14]=vaddq_u64(mul[14],mC);
	mul[14]=vaddw_u32(mul[14],b[14]);
	b[13]=vmovn_u64(mul[14]);

	T[0]=vget_lane_u32(b[0],0);
	T2[0]=vget_lane_u32(b[0],1);
	T[1]=vget_lane_u32(b[1],0);
	T2[1]=vget_lane_u32(b[1],1);
	T[2]=vget_lane_u32(b[2],0);
	T2[2]=vget_lane_u32(b[2],1);
	T[3]=vget_lane_u32(b[3],0);
	T2[3]=vget_lane_u32(b[3],1);
	T[4]=vget_lane_u32(b[4],0);
	T2[4]=vget_lane_u32(b[4],1);
	T[5]=vget_lane_u32(b[5],0);
	T2[5]=vget_lane_u32(b[5],1);
	T[6]=vget_lane_u32(b[6],0);
	T2[6]=vget_lane_u32(b[6],1);
	T[7]=vget_lane_u32(b[7],0);
	T2[7]=vget_lane_u32(b[7],1);
	T[8]=vget_lane_u32(b[8],0);
	T2[8]=vget_lane_u32(b[8],1);
	T[9]=vget_lane_u32(b[9],0);
	T2[9]=vget_lane_u32(b[9],1);
	T[10]=vget_lane_u32(b[10],0);
	T2[10]=vget_lane_u32(b[10],1);
	T[11]=vget_lane_u32(b[11],0);
	T2[11]=vget_lane_u32(b[11],1);
	T[12]=vget_lane_u32(b[12],0);
	T2[12]=vget_lane_u32(b[12],1);
	T[13]=vget_lane_u32(b[13],0);
	T2[13]=vget_lane_u32(b[13],1);
	
	while(compare_digs(T,p,14)>=0){
		dig_t carry;
		carry = mpn_sub_n(T, T, p, FP_DIGS);
	}

	while(compare_digs(T2,p,14)>=0){
		dig_t carry;
		carry = mpn_sub_n(T2, T2, p, FP_DIGS);
		}

}

void fp_mul_446_dual_NEON(dig_t *T, dig_t *T2, dig_t *A, dig_t *B, dig_t *A1, dig_t *B1){
	uint32x2_t b[15],a;
	uint64x2_t mul[15], mC, l;
	uint32x2_t t0;
	dig_t B2[]={B[0],B1[0],B[1],B1[1],B[2],B1[2],B[3],B1[3],B[4],B1[4],B[5],B1[5],B[6],B1[6],B[7],B1[7],B[8],B1[8],B[9],B1[9],B[10],B1[10],B[11],B1[11],B[12],B1[12],B[13],B1[13]};
	dig_t A2[]={A[0],A1[0],A[1],A1[1],A[2],A1[2],A[3],A1[3],A[4],A1[4],A[5],A1[5],A[6],A1[6],A[7],A1[7],A[8],A1[8],A[9],A1[9],A[10],A1[10],A[11],A1[11],A[12],A1[12],A[13],A1[13]};
	uint64_t low[]={0x00000000FFFFFFFFUL,0x00000000FFFFFFFFUL};
	
	b[0]=vld1_u32(B2);
	b[1]=vld1_u32(&B2[2]);
	b[2]=vld1_u32(&B2[4]);
	b[3]=vld1_u32(&B2[6]);
	b[4]=vld1_u32(&B2[8]);
	b[5]=vld1_u32(&B2[10]);
	b[6]=vld1_u32(&B2[12]);
	b[7]=vld1_u32(&B2[14]);
	b[8]=vld1_u32(&B2[16]);
	b[9]=vld1_u32(&B2[18]);
	b[10]=vld1_u32(&B2[20]);
	b[11]=vld1_u32(&B2[22]);
	b[12]=vld1_u32(&B2[24]);
	b[13]=vld1_u32(&B2[26]);
	l=vld1q_u64(low);

	//Primera ITeracion
	a=vld1_u32(A2);
	mul[0]=vmull_u32(a,b[0]);
	mC=vshrq_n_u64(mul[0],32);
	mul[1]=vmlal_u32(mC,a,b[1]);
	mC=vshrq_n_u64(mul[1],32);
	mul[2]=vmlal_u32(mC,a,b[2]);
	mC=vshrq_n_u64(mul[2],32);
	mul[3]=vmlal_u32(mC,a,b[3]);
	mC=vshrq_n_u64(mul[3],32);
	mul[4]=vmlal_u32(mC,a,b[4]);
	mC=vshrq_n_u64(mul[4],32);
	mul[5]=vmlal_u32(mC,a,b[5]);
	mC=vshrq_n_u64(mul[5],32);
	mul[6]=vmlal_u32(mC,a,b[6]);
	mC=vshrq_n_u64(mul[6],32);
	mul[7]=vmlal_u32(mC,a,b[7]);
	mC=vshrq_n_u64(mul[7],32);
	mul[8]=vmlal_u32(mC,a,b[8]);
	mC=vshrq_n_u64(mul[8],32);
	mul[9]=vmlal_u32(mC,a,b[9]);
	mC=vshrq_n_u64(mul[9],32);
	mul[10]=vmlal_u32(mC,a,b[10]);
	mC=vshrq_n_u64(mul[10],32);
	mul[11]=vmlal_u32(mC,a,b[11]);
	mC=vshrq_n_u64(mul[11],32);
	mul[12]=vmlal_u32(mC,a,b[12]);
	mC=vshrq_n_u64(mul[12],32);
	mul[13]=vmlal_u32(mC,a,b[13]);
	mul[14]=vshrq_n_u64(mul[13],32);
	
	mul[0]=vandq_u64(mul[0],l);
	mul[1]=vandq_u64(mul[1],l);
	mul[2]=vandq_u64(mul[2],l);
	mul[3]=vandq_u64(mul[3],l);
	mul[4]=vandq_u64(mul[4],l);
	mul[5]=vandq_u64(mul[5],l);
	mul[6]=vandq_u64(mul[6],l);
	mul[7]=vandq_u64(mul[7],l);
	mul[8]=vandq_u64(mul[8],l);
	mul[9]=vandq_u64(mul[9],l);
	mul[10]=vandq_u64(mul[10],l);
	mul[11]=vandq_u64(mul[11],l);
	mul[12]=vandq_u64(mul[12],l);
	mul[13]=vandq_u64(mul[13],l);
	
	t0=vmovn_u64(mul[0]);
	mC=vmull_n_u32(t0,pi[0]);
	a=vmovn_u64(mC);
	mC=vmlal_n_u32(mul[0],a,p[0]);
	mC=vshrq_n_u64(mC,32);
	mul[0]=vmlal_n_u32(mul[1],a,p[1]);
	mul[0]=vaddq_u64(mul[0],mC);
	mC=vshrq_n_u64(mul[0],32);
	mul[1]=vmlal_n_u32(mul[2],a,p[2]);
	mul[1]=vaddq_u64(mul[1],mC);
	mC=vshrq_n_u64(mul[1],32);
	mul[2]=vmlal_n_u32(mul[3],a,p[3]);
	mul[2]=vaddq_u64(mul[2],mC);
	mC=vshrq_n_u64(mul[2],32);
	mul[3]=vmlal_n_u32(mul[4],a,p[4]);
	mul[3]=vaddq_u64(mul[3],mC);
	mC=vshrq_n_u64(mul[3],32);
	mul[4]=vmlal_n_u32(mul[5],a,p[5]);
	mul[4]=vaddq_u64(mul[4],mC);
	mC=vshrq_n_u64(mul[4],32);
	mul[5]=vmlal_n_u32(mul[6],a,p[6]);
	mul[5]=vaddq_u64(mul[5],mC);
	mC=vshrq_n_u64(mul[5],32);
	mul[6]=vmlal_n_u32(mul[7],a,p[7]);
	mul[6]=vaddq_u64(mul[6],mC);
	mC=vshrq_n_u64(mul[6],32);
	mul[7]=vmlal_n_u32(mul[8],a,p[8]);
	mul[7]=vaddq_u64(mul[7],mC);
	mC=vshrq_n_u64(mul[7],32);
	mul[8]=vmlal_n_u32(mul[9],a,p[9]);
	mul[8]=vaddq_u64(mul[8],mC);
	mC=vshrq_n_u64(mul[8],32);
	mul[9]=vmlal_n_u32(mul[10],a,p[10]);
	mul[9]=vaddq_u64(mul[9],mC);
	mC=vshrq_n_u64(mul[9],32);
	mul[10]=vmlal_n_u32(mul[11],a,p[11]);
	mul[10]=vaddq_u64(mul[10],mC);
	mC=vshrq_n_u64(mul[10],32);
	mul[11]=vmlal_n_u32(mul[12],a,p[12]);
	mul[11]=vaddq_u64(mul[11],mC);
	mC=vshrq_n_u64(mul[11],32);
	mul[12]=vmlal_n_u32(mul[13],a,p[13]);
	mul[12]=vaddq_u64(mul[12],mC);
	mC=vshrq_n_u64(mul[12],32);
	mul[13]=vaddq_u64(mul[14],mC);
	mul[14]=vshrq_n_u64(mul[13],32);


	//Segunda ITeracion
	a=vld1_u32(&A2[2]);
	mul[0]=vandq_u64(mul[0],l);
	mul[1]=vandq_u64(mul[1],l);
	mul[2]=vandq_u64(mul[2],l);
	mul[3]=vandq_u64(mul[3],l);
	mul[4]=vandq_u64(mul[4],l);
	mul[5]=vandq_u64(mul[5],l);
	mul[6]=vandq_u64(mul[6],l);
	mul[7]=vandq_u64(mul[7],l);
	mul[8]=vandq_u64(mul[8],l);
	mul[9]=vandq_u64(mul[9],l);
	mul[10]=vandq_u64(mul[10],l);
	mul[11]=vandq_u64(mul[11],l);
	mul[12]=vandq_u64(mul[12],l);
	mul[13]=vandq_u64(mul[13],l);
	
	mul[0]=vmlal_u32(mul[0],a,b[0]);
	mC=vshrq_n_u64(mul[0],32);
	mul[1]=vmlal_u32(mul[1],a,b[1]);
	mul[1]=vaddq_u64(mul[1],mC);
	mC=vshrq_n_u64(mul[1],32);
	mul[2]=vmlal_u32(mul[2],a,b[2]);
	mul[2]=vaddq_u64(mul[2],mC);
	mC=vshrq_n_u64(mul[2],32);
	mul[3]=vmlal_u32(mul[3],a,b[3]);
	mul[3]=vaddq_u64(mul[3],mC);
	mC=vshrq_n_u64(mul[3],32);
	mul[4]=vmlal_u32(mul[4],a,b[4]);
	mul[4]=vaddq_u64(mul[4],mC);
	mC=vshrq_n_u64(mul[4],32);
	mul[5]=vmlal_u32(mul[5],a,b[5]);
	mul[5]=vaddq_u64(mul[5],mC);
	mC=vshrq_n_u64(mul[5],32);
	mul[6]=vmlal_u32(mul[6],a,b[6]);
	mul[6]=vaddq_u64(mul[6],mC);
	mC=vshrq_n_u64(mul[6],32);
	mul[7]=vmlal_u32(mul[7],a,b[7]);
	mul[7]=vaddq_u64(mul[7],mC);
	mC=vshrq_n_u64(mul[7],32);
	mul[8]=vmlal_u32(mul[8],a,b[8]);
	mul[8]=vaddq_u64(mul[8],mC);
	mC=vshrq_n_u64(mul[8],32);
	mul[9]=vmlal_u32(mul[9],a,b[9]);
	mul[9]=vaddq_u64(mul[9],mC);
	mC=vshrq_n_u64(mul[9],32);
	mul[10]=vmlal_u32(mul[10],a,b[10]);
	mul[10]=vaddq_u64(mul[10],mC);
	mC=vshrq_n_u64(mul[10],32);
	mul[11]=vmlal_u32(mul[11],a,b[11]);
	mul[11]=vaddq_u64(mul[11],mC);
	mC=vshrq_n_u64(mul[11],32);
	mul[12]=vmlal_u32(mul[12],a,b[12]);
	mul[12]=vaddq_u64(mul[12],mC);
	mC=vshrq_n_u64(mul[12],32);
	mul[13]=vmlal_u32(mul[13],a,b[13]);
	mul[13]=vaddq_u64(mul[13],mC);
	mC=vshrq_n_u64(mul[13],32);
	mul[14]=vaddq_u64(mul[14],mC);
	mul[0]=vandq_u64(mul[0],l);
	mul[1]=vandq_u64(mul[1],l);
	mul[2]=vandq_u64(mul[2],l);
	mul[3]=vandq_u64(mul[3],l);
	mul[4]=vandq_u64(mul[4],l);
	mul[5]=vandq_u64(mul[5],l);
	mul[6]=vandq_u64(mul[6],l);
	mul[7]=vandq_u64(mul[7],l);
	mul[8]=vandq_u64(mul[8],l);
	mul[9]=vandq_u64(mul[9],l);
	mul[10]=vandq_u64(mul[10],l);
	mul[11]=vandq_u64(mul[11],l);
	mul[12]=vandq_u64(mul[12],l);
	mul[13]=vandq_u64(mul[13],l);
	t0=vmovn_u64(mul[0]);
	mC=vmull_n_u32(t0,pi[0]);
	a=vmovn_u64(mC);
	mC=vmlal_n_u32(mul[0],a,p[0]);
	mC=vshrq_n_u64(mC,32);
	mul[0]=vmlal_n_u32(mul[1],a,p[1]);
	mul[0]=vaddq_u64(mul[0],mC);
	mC=vshrq_n_u64(mul[0],32);
	mul[1]=vmlal_n_u32(mul[2],a,p[2]);
	mul[1]=vaddq_u64(mul[1],mC);
	mC=vshrq_n_u64(mul[1],32);
	mul[2]=vmlal_n_u32(mul[3],a,p[3]);
	mul[2]=vaddq_u64(mul[2],mC);
	mC=vshrq_n_u64(mul[2],32);
	mul[3]=vmlal_n_u32(mul[4],a,p[4]);
	mul[3]=vaddq_u64(mul[3],mC);
	mC=vshrq_n_u64(mul[3],32);
	mul[4]=vmlal_n_u32(mul[5],a,p[5]);
	mul[4]=vaddq_u64(mul[4],mC);
	mC=vshrq_n_u64(mul[4],32);
	mul[5]=vmlal_n_u32(mul[6],a,p[6]);
	mul[5]=vaddq_u64(mul[5],mC);
	mC=vshrq_n_u64(mul[5],32);
	mul[6]=vmlal_n_u32(mul[7],a,p[7]);
	mul[6]=vaddq_u64(mul[6],mC);
	mC=vshrq_n_u64(mul[6],32);
	mul[7]=vmlal_n_u32(mul[8],a,p[8]);
	mul[7]=vaddq_u64(mul[7],mC);
	mC=vshrq_n_u64(mul[7],32);
	mul[8]=vmlal_n_u32(mul[9],a,p[9]);
	mul[8]=vaddq_u64(mul[8],mC);
	mC=vshrq_n_u64(mul[8],32);
	mul[9]=vmlal_n_u32(mul[10],a,p[10]);
	mul[9]=vaddq_u64(mul[9],mC);
	mC=vshrq_n_u64(mul[9],32);
	mul[10]=vmlal_n_u32(mul[11],a,p[11]);
	mul[10]=vaddq_u64(mul[10],mC);
	mC=vshrq_n_u64(mul[10],32);
	mul[11]=vmlal_n_u32(mul[12],a,p[12]);
	mul[11]=vaddq_u64(mul[11],mC);
	mC=vshrq_n_u64(mul[11],32);
	mul[12]=vmlal_n_u32(mul[13],a,p[13]);
	mul[12]=vaddq_u64(mul[12],mC);
	mC=vshrq_n_u64(mul[12],32);
	mul[13]=vaddq_u64(mul[14],mC);
	mul[14]=vshrq_n_u64(mul[13],32);
	
	




	//Tercera ITeracion
	a=vld1_u32(&A2[4]);
	mul[0]=vandq_u64(mul[0],l);
	mul[1]=vandq_u64(mul[1],l);
	mul[2]=vandq_u64(mul[2],l);
	mul[3]=vandq_u64(mul[3],l);
	mul[4]=vandq_u64(mul[4],l);
	mul[5]=vandq_u64(mul[5],l);
	mul[6]=vandq_u64(mul[6],l);
	mul[7]=vandq_u64(mul[7],l);
	mul[8]=vandq_u64(mul[8],l);
	mul[9]=vandq_u64(mul[9],l);
	mul[10]=vandq_u64(mul[10],l);
	mul[11]=vandq_u64(mul[11],l);
	mul[12]=vandq_u64(mul[12],l);
	mul[13]=vandq_u64(mul[13],l);
	
	mul[0]=vmlal_u32(mul[0],a,b[0]);
	mC=vshrq_n_u64(mul[0],32);
	mul[1]=vmlal_u32(mul[1],a,b[1]);
	mul[1]=vaddq_u64(mul[1],mC);
	mC=vshrq_n_u64(mul[1],32);
	mul[2]=vmlal_u32(mul[2],a,b[2]);
	mul[2]=vaddq_u64(mul[2],mC);
	mC=vshrq_n_u64(mul[2],32);
	mul[3]=vmlal_u32(mul[3],a,b[3]);
	mul[3]=vaddq_u64(mul[3],mC);
	mC=vshrq_n_u64(mul[3],32);
	mul[4]=vmlal_u32(mul[4],a,b[4]);
	mul[4]=vaddq_u64(mul[4],mC);
	mC=vshrq_n_u64(mul[4],32);
	mul[5]=vmlal_u32(mul[5],a,b[5]);
	mul[5]=vaddq_u64(mul[5],mC);
	mC=vshrq_n_u64(mul[5],32);
	mul[6]=vmlal_u32(mul[6],a,b[6]);
	mul[6]=vaddq_u64(mul[6],mC);
	mC=vshrq_n_u64(mul[6],32);
	mul[7]=vmlal_u32(mul[7],a,b[7]);
	mul[7]=vaddq_u64(mul[7],mC);
	mC=vshrq_n_u64(mul[7],32);
	mul[8]=vmlal_u32(mul[8],a,b[8]);
	mul[8]=vaddq_u64(mul[8],mC);
	mC=vshrq_n_u64(mul[8],32);
	mul[9]=vmlal_u32(mul[9],a,b[9]);
	mul[9]=vaddq_u64(mul[9],mC);
	mC=vshrq_n_u64(mul[9],32);
	mul[10]=vmlal_u32(mul[10],a,b[10]);
	mul[10]=vaddq_u64(mul[10],mC);
	mC=vshrq_n_u64(mul[10],32);
	mul[11]=vmlal_u32(mul[11],a,b[11]);
	mul[11]=vaddq_u64(mul[11],mC);
	mC=vshrq_n_u64(mul[11],32);
	mul[12]=vmlal_u32(mul[12],a,b[12]);
	mul[12]=vaddq_u64(mul[12],mC);
	mC=vshrq_n_u64(mul[12],32);
	mul[13]=vmlal_u32(mul[13],a,b[13]);
	mul[13]=vaddq_u64(mul[13],mC);
	mC=vshrq_n_u64(mul[13],32);
	mul[14]=vaddq_u64(mul[14],mC);
	mul[0]=vandq_u64(mul[0],l);
	mul[1]=vandq_u64(mul[1],l);
	mul[2]=vandq_u64(mul[2],l);
	mul[3]=vandq_u64(mul[3],l);
	mul[4]=vandq_u64(mul[4],l);
	mul[5]=vandq_u64(mul[5],l);
	mul[6]=vandq_u64(mul[6],l);
	mul[7]=vandq_u64(mul[7],l);
	mul[8]=vandq_u64(mul[8],l);
	mul[9]=vandq_u64(mul[9],l);
	mul[10]=vandq_u64(mul[10],l);
	mul[11]=vandq_u64(mul[11],l);
	mul[12]=vandq_u64(mul[12],l);
	mul[13]=vandq_u64(mul[13],l);
	t0=vmovn_u64(mul[0]);
	mC=vmull_n_u32(t0,pi[0]);
	a=vmovn_u64(mC);
	mC=vmlal_n_u32(mul[0],a,p[0]);
	mC=vshrq_n_u64(mC,32);
	mul[0]=vmlal_n_u32(mul[1],a,p[1]);
	mul[0]=vaddq_u64(mul[0],mC);
	mC=vshrq_n_u64(mul[0],32);
	mul[1]=vmlal_n_u32(mul[2],a,p[2]);
	mul[1]=vaddq_u64(mul[1],mC);
	mC=vshrq_n_u64(mul[1],32);
	mul[2]=vmlal_n_u32(mul[3],a,p[3]);
	mul[2]=vaddq_u64(mul[2],mC);
	mC=vshrq_n_u64(mul[2],32);
	mul[3]=vmlal_n_u32(mul[4],a,p[4]);
	mul[3]=vaddq_u64(mul[3],mC);
	mC=vshrq_n_u64(mul[3],32);
	mul[4]=vmlal_n_u32(mul[5],a,p[5]);
	mul[4]=vaddq_u64(mul[4],mC);
	mC=vshrq_n_u64(mul[4],32);
	mul[5]=vmlal_n_u32(mul[6],a,p[6]);
	mul[5]=vaddq_u64(mul[5],mC);
	mC=vshrq_n_u64(mul[5],32);
	mul[6]=vmlal_n_u32(mul[7],a,p[7]);
	mul[6]=vaddq_u64(mul[6],mC);
	mC=vshrq_n_u64(mul[6],32);
	mul[7]=vmlal_n_u32(mul[8],a,p[8]);
	mul[7]=vaddq_u64(mul[7],mC);
	mC=vshrq_n_u64(mul[7],32);
	mul[8]=vmlal_n_u32(mul[9],a,p[9]);
	mul[8]=vaddq_u64(mul[8],mC);
	mC=vshrq_n_u64(mul[8],32);
	mul[9]=vmlal_n_u32(mul[10],a,p[10]);
	mul[9]=vaddq_u64(mul[9],mC);
	mC=vshrq_n_u64(mul[9],32);
	mul[10]=vmlal_n_u32(mul[11],a,p[11]);
	mul[10]=vaddq_u64(mul[10],mC);
	mC=vshrq_n_u64(mul[10],32);
	mul[11]=vmlal_n_u32(mul[12],a,p[12]);
	mul[11]=vaddq_u64(mul[11],mC);
	mC=vshrq_n_u64(mul[11],32);
	mul[12]=vmlal_n_u32(mul[13],a,p[13]);
	mul[12]=vaddq_u64(mul[12],mC);
	mC=vshrq_n_u64(mul[12],32);
	mul[13]=vaddq_u64(mul[14],mC);
	mul[14]=vshrq_n_u64(mul[13],32);

	//Cuarta ITeracion
	a=vld1_u32(&A2[6]);
	mul[0]=vandq_u64(mul[0],l);
	mul[1]=vandq_u64(mul[1],l);
	mul[2]=vandq_u64(mul[2],l);
	mul[3]=vandq_u64(mul[3],l);
	mul[4]=vandq_u64(mul[4],l);
	mul[5]=vandq_u64(mul[5],l);
	mul[6]=vandq_u64(mul[6],l);
	mul[7]=vandq_u64(mul[7],l);
	mul[8]=vandq_u64(mul[8],l);
	mul[9]=vandq_u64(mul[9],l);
	mul[10]=vandq_u64(mul[10],l);
	mul[11]=vandq_u64(mul[11],l);
	mul[12]=vandq_u64(mul[12],l);
	mul[13]=vandq_u64(mul[13],l);
	
	mul[0]=vmlal_u32(mul[0],a,b[0]);
	mC=vshrq_n_u64(mul[0],32);
	mul[1]=vmlal_u32(mul[1],a,b[1]);
	mul[1]=vaddq_u64(mul[1],mC);
	mC=vshrq_n_u64(mul[1],32);
	mul[2]=vmlal_u32(mul[2],a,b[2]);
	mul[2]=vaddq_u64(mul[2],mC);
	mC=vshrq_n_u64(mul[2],32);
	mul[3]=vmlal_u32(mul[3],a,b[3]);
	mul[3]=vaddq_u64(mul[3],mC);
	mC=vshrq_n_u64(mul[3],32);
	mul[4]=vmlal_u32(mul[4],a,b[4]);
	mul[4]=vaddq_u64(mul[4],mC);
	mC=vshrq_n_u64(mul[4],32);
	mul[5]=vmlal_u32(mul[5],a,b[5]);
	mul[5]=vaddq_u64(mul[5],mC);
	mC=vshrq_n_u64(mul[5],32);
	mul[6]=vmlal_u32(mul[6],a,b[6]);
	mul[6]=vaddq_u64(mul[6],mC);
	mC=vshrq_n_u64(mul[6],32);
	mul[7]=vmlal_u32(mul[7],a,b[7]);
	mul[7]=vaddq_u64(mul[7],mC);
	mC=vshrq_n_u64(mul[7],32);
	mul[8]=vmlal_u32(mul[8],a,b[8]);
	mul[8]=vaddq_u64(mul[8],mC);
	mC=vshrq_n_u64(mul[8],32);
	mul[9]=vmlal_u32(mul[9],a,b[9]);
	mul[9]=vaddq_u64(mul[9],mC);
	mC=vshrq_n_u64(mul[9],32);
	mul[10]=vmlal_u32(mul[10],a,b[10]);
	mul[10]=vaddq_u64(mul[10],mC);
	mC=vshrq_n_u64(mul[10],32);
	mul[11]=vmlal_u32(mul[11],a,b[11]);
	mul[11]=vaddq_u64(mul[11],mC);
	mC=vshrq_n_u64(mul[11],32);
	mul[12]=vmlal_u32(mul[12],a,b[12]);
	mul[12]=vaddq_u64(mul[12],mC);
	mC=vshrq_n_u64(mul[12],32);
	mul[13]=vmlal_u32(mul[13],a,b[13]);
	mul[13]=vaddq_u64(mul[13],mC);
	mC=vshrq_n_u64(mul[13],32);
	mul[14]=vaddq_u64(mul[14],mC);
	mul[0]=vandq_u64(mul[0],l);
	mul[1]=vandq_u64(mul[1],l);
	mul[2]=vandq_u64(mul[2],l);
	mul[3]=vandq_u64(mul[3],l);
	mul[4]=vandq_u64(mul[4],l);
	mul[5]=vandq_u64(mul[5],l);
	mul[6]=vandq_u64(mul[6],l);
	mul[7]=vandq_u64(mul[7],l);
	mul[8]=vandq_u64(mul[8],l);
	mul[9]=vandq_u64(mul[9],l);
	mul[10]=vandq_u64(mul[10],l);
	mul[11]=vandq_u64(mul[11],l);
	mul[12]=vandq_u64(mul[12],l);
	mul[13]=vandq_u64(mul[13],l);
	t0=vmovn_u64(mul[0]);
	mC=vmull_n_u32(t0,pi[0]);
	a=vmovn_u64(mC);
	mC=vmlal_n_u32(mul[0],a,p[0]);
	mC=vshrq_n_u64(mC,32);
	mul[0]=vmlal_n_u32(mul[1],a,p[1]);
	mul[0]=vaddq_u64(mul[0],mC);
	mC=vshrq_n_u64(mul[0],32);
	mul[1]=vmlal_n_u32(mul[2],a,p[2]);
	mul[1]=vaddq_u64(mul[1],mC);
	mC=vshrq_n_u64(mul[1],32);
	mul[2]=vmlal_n_u32(mul[3],a,p[3]);
	mul[2]=vaddq_u64(mul[2],mC);
	mC=vshrq_n_u64(mul[2],32);
	mul[3]=vmlal_n_u32(mul[4],a,p[4]);
	mul[3]=vaddq_u64(mul[3],mC);
	mC=vshrq_n_u64(mul[3],32);
	mul[4]=vmlal_n_u32(mul[5],a,p[5]);
	mul[4]=vaddq_u64(mul[4],mC);
	mC=vshrq_n_u64(mul[4],32);
	mul[5]=vmlal_n_u32(mul[6],a,p[6]);
	mul[5]=vaddq_u64(mul[5],mC);
	mC=vshrq_n_u64(mul[5],32);
	mul[6]=vmlal_n_u32(mul[7],a,p[7]);
	mul[6]=vaddq_u64(mul[6],mC);
	mC=vshrq_n_u64(mul[6],32);
	mul[7]=vmlal_n_u32(mul[8],a,p[8]);
	mul[7]=vaddq_u64(mul[7],mC);
	mC=vshrq_n_u64(mul[7],32);
	mul[8]=vmlal_n_u32(mul[9],a,p[9]);
	mul[8]=vaddq_u64(mul[8],mC);
	mC=vshrq_n_u64(mul[8],32);
	mul[9]=vmlal_n_u32(mul[10],a,p[10]);
	mul[9]=vaddq_u64(mul[9],mC);
	mC=vshrq_n_u64(mul[9],32);
	mul[10]=vmlal_n_u32(mul[11],a,p[11]);
	mul[10]=vaddq_u64(mul[10],mC);
	mC=vshrq_n_u64(mul[10],32);
	mul[11]=vmlal_n_u32(mul[12],a,p[12]);
	mul[11]=vaddq_u64(mul[11],mC);
	mC=vshrq_n_u64(mul[11],32);
	mul[12]=vmlal_n_u32(mul[13],a,p[13]);
	mul[12]=vaddq_u64(mul[12],mC);
	mC=vshrq_n_u64(mul[12],32);
	mul[13]=vaddq_u64(mul[14],mC);
	mul[14]=vshrq_n_u64(mul[13],32);

	//Quinta ITeracion
	a=vld1_u32(&A2[8]);
	mul[0]=vandq_u64(mul[0],l);
	mul[1]=vandq_u64(mul[1],l);
	mul[2]=vandq_u64(mul[2],l);
	mul[3]=vandq_u64(mul[3],l);
	mul[4]=vandq_u64(mul[4],l);
	mul[5]=vandq_u64(mul[5],l);
	mul[6]=vandq_u64(mul[6],l);
	mul[7]=vandq_u64(mul[7],l);
	mul[8]=vandq_u64(mul[8],l);
	mul[9]=vandq_u64(mul[9],l);
	mul[10]=vandq_u64(mul[10],l);
	mul[11]=vandq_u64(mul[11],l);
	mul[12]=vandq_u64(mul[12],l);
	mul[13]=vandq_u64(mul[13],l);
	
	mul[0]=vmlal_u32(mul[0],a,b[0]);
	mC=vshrq_n_u64(mul[0],32);
	mul[1]=vmlal_u32(mul[1],a,b[1]);
	mul[1]=vaddq_u64(mul[1],mC);
	mC=vshrq_n_u64(mul[1],32);
	mul[2]=vmlal_u32(mul[2],a,b[2]);
	mul[2]=vaddq_u64(mul[2],mC);
	mC=vshrq_n_u64(mul[2],32);
	mul[3]=vmlal_u32(mul[3],a,b[3]);
	mul[3]=vaddq_u64(mul[3],mC);
	mC=vshrq_n_u64(mul[3],32);
	mul[4]=vmlal_u32(mul[4],a,b[4]);
	mul[4]=vaddq_u64(mul[4],mC);
	mC=vshrq_n_u64(mul[4],32);
	mul[5]=vmlal_u32(mul[5],a,b[5]);
	mul[5]=vaddq_u64(mul[5],mC);
	mC=vshrq_n_u64(mul[5],32);
	mul[6]=vmlal_u32(mul[6],a,b[6]);
	mul[6]=vaddq_u64(mul[6],mC);
	mC=vshrq_n_u64(mul[6],32);
	mul[7]=vmlal_u32(mul[7],a,b[7]);
	mul[7]=vaddq_u64(mul[7],mC);
	mC=vshrq_n_u64(mul[7],32);
	mul[8]=vmlal_u32(mul[8],a,b[8]);
	mul[8]=vaddq_u64(mul[8],mC);
	mC=vshrq_n_u64(mul[8],32);
	mul[9]=vmlal_u32(mul[9],a,b[9]);
	mul[9]=vaddq_u64(mul[9],mC);
	mC=vshrq_n_u64(mul[9],32);
	mul[10]=vmlal_u32(mul[10],a,b[10]);
	mul[10]=vaddq_u64(mul[10],mC);
	mC=vshrq_n_u64(mul[10],32);
	mul[11]=vmlal_u32(mul[11],a,b[11]);
	mul[11]=vaddq_u64(mul[11],mC);
	mC=vshrq_n_u64(mul[11],32);
	mul[12]=vmlal_u32(mul[12],a,b[12]);
	mul[12]=vaddq_u64(mul[12],mC);
	mC=vshrq_n_u64(mul[12],32);
	mul[13]=vmlal_u32(mul[13],a,b[13]);
	mul[13]=vaddq_u64(mul[13],mC);
	mC=vshrq_n_u64(mul[13],32);
	mul[14]=vaddq_u64(mul[14],mC);
	mul[0]=vandq_u64(mul[0],l);
	mul[1]=vandq_u64(mul[1],l);
	mul[2]=vandq_u64(mul[2],l);
	mul[3]=vandq_u64(mul[3],l);
	mul[4]=vandq_u64(mul[4],l);
	mul[5]=vandq_u64(mul[5],l);
	mul[6]=vandq_u64(mul[6],l);
	mul[7]=vandq_u64(mul[7],l);
	mul[8]=vandq_u64(mul[8],l);
	mul[9]=vandq_u64(mul[9],l);
	mul[10]=vandq_u64(mul[10],l);
	mul[11]=vandq_u64(mul[11],l);
	mul[12]=vandq_u64(mul[12],l);
	mul[13]=vandq_u64(mul[13],l);
	t0=vmovn_u64(mul[0]);
	mC=vmull_n_u32(t0,pi[0]);
	a=vmovn_u64(mC);
	mC=vmlal_n_u32(mul[0],a,p[0]);
	mC=vshrq_n_u64(mC,32);
	mul[0]=vmlal_n_u32(mul[1],a,p[1]);
	mul[0]=vaddq_u64(mul[0],mC);
	mC=vshrq_n_u64(mul[0],32);
	mul[1]=vmlal_n_u32(mul[2],a,p[2]);
	mul[1]=vaddq_u64(mul[1],mC);
	mC=vshrq_n_u64(mul[1],32);
	mul[2]=vmlal_n_u32(mul[3],a,p[3]);
	mul[2]=vaddq_u64(mul[2],mC);
	mC=vshrq_n_u64(mul[2],32);
	mul[3]=vmlal_n_u32(mul[4],a,p[4]);
	mul[3]=vaddq_u64(mul[3],mC);
	mC=vshrq_n_u64(mul[3],32);
	mul[4]=vmlal_n_u32(mul[5],a,p[5]);
	mul[4]=vaddq_u64(mul[4],mC);
	mC=vshrq_n_u64(mul[4],32);
	mul[5]=vmlal_n_u32(mul[6],a,p[6]);
	mul[5]=vaddq_u64(mul[5],mC);
	mC=vshrq_n_u64(mul[5],32);
	mul[6]=vmlal_n_u32(mul[7],a,p[7]);
	mul[6]=vaddq_u64(mul[6],mC);
	mC=vshrq_n_u64(mul[6],32);
	mul[7]=vmlal_n_u32(mul[8],a,p[8]);
	mul[7]=vaddq_u64(mul[7],mC);
	mC=vshrq_n_u64(mul[7],32);
	mul[8]=vmlal_n_u32(mul[9],a,p[9]);
	mul[8]=vaddq_u64(mul[8],mC);
	mC=vshrq_n_u64(mul[8],32);
	mul[9]=vmlal_n_u32(mul[10],a,p[10]);
	mul[9]=vaddq_u64(mul[9],mC);
	mC=vshrq_n_u64(mul[9],32);
	mul[10]=vmlal_n_u32(mul[11],a,p[11]);
	mul[10]=vaddq_u64(mul[10],mC);
	mC=vshrq_n_u64(mul[10],32);
	mul[11]=vmlal_n_u32(mul[12],a,p[12]);
	mul[11]=vaddq_u64(mul[11],mC);
	mC=vshrq_n_u64(mul[11],32);
	mul[12]=vmlal_n_u32(mul[13],a,p[13]);
	mul[12]=vaddq_u64(mul[12],mC);
	mC=vshrq_n_u64(mul[12],32);
	mul[13]=vaddq_u64(mul[14],mC);
	mul[14]=vshrq_n_u64(mul[13],32);



	//Sexta ITeracion
	a=vld1_u32(&A2[10]);
	mul[0]=vandq_u64(mul[0],l);
	mul[1]=vandq_u64(mul[1],l);
	mul[2]=vandq_u64(mul[2],l);
	mul[3]=vandq_u64(mul[3],l);
	mul[4]=vandq_u64(mul[4],l);
	mul[5]=vandq_u64(mul[5],l);
	mul[6]=vandq_u64(mul[6],l);
	mul[7]=vandq_u64(mul[7],l);
	mul[8]=vandq_u64(mul[8],l);
	mul[9]=vandq_u64(mul[9],l);
	mul[10]=vandq_u64(mul[10],l);
	mul[11]=vandq_u64(mul[11],l);
	mul[12]=vandq_u64(mul[12],l);
	mul[13]=vandq_u64(mul[13],l);
	
	mul[0]=vmlal_u32(mul[0],a,b[0]);
	mC=vshrq_n_u64(mul[0],32);
	mul[1]=vmlal_u32(mul[1],a,b[1]);
	mul[1]=vaddq_u64(mul[1],mC);
	mC=vshrq_n_u64(mul[1],32);
	mul[2]=vmlal_u32(mul[2],a,b[2]);
	mul[2]=vaddq_u64(mul[2],mC);
	mC=vshrq_n_u64(mul[2],32);
	mul[3]=vmlal_u32(mul[3],a,b[3]);
	mul[3]=vaddq_u64(mul[3],mC);
	mC=vshrq_n_u64(mul[3],32);
	mul[4]=vmlal_u32(mul[4],a,b[4]);
	mul[4]=vaddq_u64(mul[4],mC);
	mC=vshrq_n_u64(mul[4],32);
	mul[5]=vmlal_u32(mul[5],a,b[5]);
	mul[5]=vaddq_u64(mul[5],mC);
	mC=vshrq_n_u64(mul[5],32);
	mul[6]=vmlal_u32(mul[6],a,b[6]);
	mul[6]=vaddq_u64(mul[6],mC);
	mC=vshrq_n_u64(mul[6],32);
	mul[7]=vmlal_u32(mul[7],a,b[7]);
	mul[7]=vaddq_u64(mul[7],mC);
	mC=vshrq_n_u64(mul[7],32);
	mul[8]=vmlal_u32(mul[8],a,b[8]);
	mul[8]=vaddq_u64(mul[8],mC);
	mC=vshrq_n_u64(mul[8],32);
	mul[9]=vmlal_u32(mul[9],a,b[9]);
	mul[9]=vaddq_u64(mul[9],mC);
	mC=vshrq_n_u64(mul[9],32);
	mul[10]=vmlal_u32(mul[10],a,b[10]);
	mul[10]=vaddq_u64(mul[10],mC);
	mC=vshrq_n_u64(mul[10],32);
	mul[11]=vmlal_u32(mul[11],a,b[11]);
	mul[11]=vaddq_u64(mul[11],mC);
	mC=vshrq_n_u64(mul[11],32);
	mul[12]=vmlal_u32(mul[12],a,b[12]);
	mul[12]=vaddq_u64(mul[12],mC);
	mC=vshrq_n_u64(mul[12],32);
	mul[13]=vmlal_u32(mul[13],a,b[13]);
	mul[13]=vaddq_u64(mul[13],mC);
	mC=vshrq_n_u64(mul[13],32);
	mul[14]=vaddq_u64(mul[14],mC);
	mul[0]=vandq_u64(mul[0],l);
	mul[1]=vandq_u64(mul[1],l);
	mul[2]=vandq_u64(mul[2],l);
	mul[3]=vandq_u64(mul[3],l);
	mul[4]=vandq_u64(mul[4],l);
	mul[5]=vandq_u64(mul[5],l);
	mul[6]=vandq_u64(mul[6],l);
	mul[7]=vandq_u64(mul[7],l);
	mul[8]=vandq_u64(mul[8],l);
	mul[9]=vandq_u64(mul[9],l);
	mul[10]=vandq_u64(mul[10],l);
	mul[11]=vandq_u64(mul[11],l);
	mul[12]=vandq_u64(mul[12],l);
	mul[13]=vandq_u64(mul[13],l);
	t0=vmovn_u64(mul[0]);
	mC=vmull_n_u32(t0,pi[0]);
	a=vmovn_u64(mC);
	mC=vmlal_n_u32(mul[0],a,p[0]);
	mC=vshrq_n_u64(mC,32);
	mul[0]=vmlal_n_u32(mul[1],a,p[1]);
	mul[0]=vaddq_u64(mul[0],mC);
	mC=vshrq_n_u64(mul[0],32);
	mul[1]=vmlal_n_u32(mul[2],a,p[2]);
	mul[1]=vaddq_u64(mul[1],mC);
	mC=vshrq_n_u64(mul[1],32);
	mul[2]=vmlal_n_u32(mul[3],a,p[3]);
	mul[2]=vaddq_u64(mul[2],mC);
	mC=vshrq_n_u64(mul[2],32);
	mul[3]=vmlal_n_u32(mul[4],a,p[4]);
	mul[3]=vaddq_u64(mul[3],mC);
	mC=vshrq_n_u64(mul[3],32);
	mul[4]=vmlal_n_u32(mul[5],a,p[5]);
	mul[4]=vaddq_u64(mul[4],mC);
	mC=vshrq_n_u64(mul[4],32);
	mul[5]=vmlal_n_u32(mul[6],a,p[6]);
	mul[5]=vaddq_u64(mul[5],mC);
	mC=vshrq_n_u64(mul[5],32);
	mul[6]=vmlal_n_u32(mul[7],a,p[7]);
	mul[6]=vaddq_u64(mul[6],mC);
	mC=vshrq_n_u64(mul[6],32);
	mul[7]=vmlal_n_u32(mul[8],a,p[8]);
	mul[7]=vaddq_u64(mul[7],mC);
	mC=vshrq_n_u64(mul[7],32);
	mul[8]=vmlal_n_u32(mul[9],a,p[9]);
	mul[8]=vaddq_u64(mul[8],mC);
	mC=vshrq_n_u64(mul[8],32);
	mul[9]=vmlal_n_u32(mul[10],a,p[10]);
	mul[9]=vaddq_u64(mul[9],mC);
	mC=vshrq_n_u64(mul[9],32);
	mul[10]=vmlal_n_u32(mul[11],a,p[11]);
	mul[10]=vaddq_u64(mul[10],mC);
	mC=vshrq_n_u64(mul[10],32);
	mul[11]=vmlal_n_u32(mul[12],a,p[12]);
	mul[11]=vaddq_u64(mul[11],mC);
	mC=vshrq_n_u64(mul[11],32);
	mul[12]=vmlal_n_u32(mul[13],a,p[13]);
	mul[12]=vaddq_u64(mul[12],mC);
	mC=vshrq_n_u64(mul[12],32);
	mul[13]=vaddq_u64(mul[14],mC);
	mul[14]=vshrq_n_u64(mul[13],32);

	//Septima ITeracion
	a=vld1_u32(&A2[12]);
	mul[0]=vandq_u64(mul[0],l);
	mul[1]=vandq_u64(mul[1],l);
	mul[2]=vandq_u64(mul[2],l);
	mul[3]=vandq_u64(mul[3],l);
	mul[4]=vandq_u64(mul[4],l);
	mul[5]=vandq_u64(mul[5],l);
	mul[6]=vandq_u64(mul[6],l);
	mul[7]=vandq_u64(mul[7],l);
	mul[8]=vandq_u64(mul[8],l);
	mul[9]=vandq_u64(mul[9],l);
	mul[10]=vandq_u64(mul[10],l);
	mul[11]=vandq_u64(mul[11],l);
	mul[12]=vandq_u64(mul[12],l);
	mul[13]=vandq_u64(mul[13],l);
	
	mul[0]=vmlal_u32(mul[0],a,b[0]);
	mC=vshrq_n_u64(mul[0],32);
	mul[1]=vmlal_u32(mul[1],a,b[1]);
	mul[1]=vaddq_u64(mul[1],mC);
	mC=vshrq_n_u64(mul[1],32);
	mul[2]=vmlal_u32(mul[2],a,b[2]);
	mul[2]=vaddq_u64(mul[2],mC);
	mC=vshrq_n_u64(mul[2],32);
	mul[3]=vmlal_u32(mul[3],a,b[3]);
	mul[3]=vaddq_u64(mul[3],mC);
	mC=vshrq_n_u64(mul[3],32);
	mul[4]=vmlal_u32(mul[4],a,b[4]);
	mul[4]=vaddq_u64(mul[4],mC);
	mC=vshrq_n_u64(mul[4],32);
	mul[5]=vmlal_u32(mul[5],a,b[5]);
	mul[5]=vaddq_u64(mul[5],mC);
	mC=vshrq_n_u64(mul[5],32);
	mul[6]=vmlal_u32(mul[6],a,b[6]);
	mul[6]=vaddq_u64(mul[6],mC);
	mC=vshrq_n_u64(mul[6],32);
	mul[7]=vmlal_u32(mul[7],a,b[7]);
	mul[7]=vaddq_u64(mul[7],mC);
	mC=vshrq_n_u64(mul[7],32);
	mul[8]=vmlal_u32(mul[8],a,b[8]);
	mul[8]=vaddq_u64(mul[8],mC);
	mC=vshrq_n_u64(mul[8],32);
	mul[9]=vmlal_u32(mul[9],a,b[9]);
	mul[9]=vaddq_u64(mul[9],mC);
	mC=vshrq_n_u64(mul[9],32);
	mul[10]=vmlal_u32(mul[10],a,b[10]);
	mul[10]=vaddq_u64(mul[10],mC);
	mC=vshrq_n_u64(mul[10],32);
	mul[11]=vmlal_u32(mul[11],a,b[11]);
	mul[11]=vaddq_u64(mul[11],mC);
	mC=vshrq_n_u64(mul[11],32);
	mul[12]=vmlal_u32(mul[12],a,b[12]);
	mul[12]=vaddq_u64(mul[12],mC);
	mC=vshrq_n_u64(mul[12],32);
	mul[13]=vmlal_u32(mul[13],a,b[13]);
	mul[13]=vaddq_u64(mul[13],mC);
	mC=vshrq_n_u64(mul[13],32);
	mul[14]=vaddq_u64(mul[14],mC);
	mul[0]=vandq_u64(mul[0],l);
	mul[1]=vandq_u64(mul[1],l);
	mul[2]=vandq_u64(mul[2],l);
	mul[3]=vandq_u64(mul[3],l);
	mul[4]=vandq_u64(mul[4],l);
	mul[5]=vandq_u64(mul[5],l);
	mul[6]=vandq_u64(mul[6],l);
	mul[7]=vandq_u64(mul[7],l);
	mul[8]=vandq_u64(mul[8],l);
	mul[9]=vandq_u64(mul[9],l);
	mul[10]=vandq_u64(mul[10],l);
	mul[11]=vandq_u64(mul[11],l);
	mul[12]=vandq_u64(mul[12],l);
	mul[13]=vandq_u64(mul[13],l);
	t0=vmovn_u64(mul[0]);
	mC=vmull_n_u32(t0,pi[0]);
	a=vmovn_u64(mC);
	mC=vmlal_n_u32(mul[0],a,p[0]);
	mC=vshrq_n_u64(mC,32);
	mul[0]=vmlal_n_u32(mul[1],a,p[1]);
	mul[0]=vaddq_u64(mul[0],mC);
	mC=vshrq_n_u64(mul[0],32);
	mul[1]=vmlal_n_u32(mul[2],a,p[2]);
	mul[1]=vaddq_u64(mul[1],mC);
	mC=vshrq_n_u64(mul[1],32);
	mul[2]=vmlal_n_u32(mul[3],a,p[3]);
	mul[2]=vaddq_u64(mul[2],mC);
	mC=vshrq_n_u64(mul[2],32);
	mul[3]=vmlal_n_u32(mul[4],a,p[4]);
	mul[3]=vaddq_u64(mul[3],mC);
	mC=vshrq_n_u64(mul[3],32);
	mul[4]=vmlal_n_u32(mul[5],a,p[5]);
	mul[4]=vaddq_u64(mul[4],mC);
	mC=vshrq_n_u64(mul[4],32);
	mul[5]=vmlal_n_u32(mul[6],a,p[6]);
	mul[5]=vaddq_u64(mul[5],mC);
	mC=vshrq_n_u64(mul[5],32);
	mul[6]=vmlal_n_u32(mul[7],a,p[7]);
	mul[6]=vaddq_u64(mul[6],mC);
	mC=vshrq_n_u64(mul[6],32);
	mul[7]=vmlal_n_u32(mul[8],a,p[8]);
	mul[7]=vaddq_u64(mul[7],mC);
	mC=vshrq_n_u64(mul[7],32);
	mul[8]=vmlal_n_u32(mul[9],a,p[9]);
	mul[8]=vaddq_u64(mul[8],mC);
	mC=vshrq_n_u64(mul[8],32);
	mul[9]=vmlal_n_u32(mul[10],a,p[10]);
	mul[9]=vaddq_u64(mul[9],mC);
	mC=vshrq_n_u64(mul[9],32);
	mul[10]=vmlal_n_u32(mul[11],a,p[11]);
	mul[10]=vaddq_u64(mul[10],mC);
	mC=vshrq_n_u64(mul[10],32);
	mul[11]=vmlal_n_u32(mul[12],a,p[12]);
	mul[11]=vaddq_u64(mul[11],mC);
	mC=vshrq_n_u64(mul[11],32);
	mul[12]=vmlal_n_u32(mul[13],a,p[13]);
	mul[12]=vaddq_u64(mul[12],mC);
	mC=vshrq_n_u64(mul[12],32);
	mul[13]=vaddq_u64(mul[14],mC);
	mul[14]=vshrq_n_u64(mul[13],32);


	//Octava ITeracion
	a=vld1_u32(&A2[14]);
	mul[0]=vandq_u64(mul[0],l);
	mul[1]=vandq_u64(mul[1],l);
	mul[2]=vandq_u64(mul[2],l);
	mul[3]=vandq_u64(mul[3],l);
	mul[4]=vandq_u64(mul[4],l);
	mul[5]=vandq_u64(mul[5],l);
	mul[6]=vandq_u64(mul[6],l);
	mul[7]=vandq_u64(mul[7],l);
	mul[8]=vandq_u64(mul[8],l);
	mul[9]=vandq_u64(mul[9],l);
	mul[10]=vandq_u64(mul[10],l);
	mul[11]=vandq_u64(mul[11],l);
	mul[12]=vandq_u64(mul[12],l);
	mul[13]=vandq_u64(mul[13],l);
	
	mul[0]=vmlal_u32(mul[0],a,b[0]);
	mC=vshrq_n_u64(mul[0],32);
	mul[1]=vmlal_u32(mul[1],a,b[1]);
	mul[1]=vaddq_u64(mul[1],mC);
	mC=vshrq_n_u64(mul[1],32);
	mul[2]=vmlal_u32(mul[2],a,b[2]);
	mul[2]=vaddq_u64(mul[2],mC);
	mC=vshrq_n_u64(mul[2],32);
	mul[3]=vmlal_u32(mul[3],a,b[3]);
	mul[3]=vaddq_u64(mul[3],mC);
	mC=vshrq_n_u64(mul[3],32);
	mul[4]=vmlal_u32(mul[4],a,b[4]);
	mul[4]=vaddq_u64(mul[4],mC);
	mC=vshrq_n_u64(mul[4],32);
	mul[5]=vmlal_u32(mul[5],a,b[5]);
	mul[5]=vaddq_u64(mul[5],mC);
	mC=vshrq_n_u64(mul[5],32);
	mul[6]=vmlal_u32(mul[6],a,b[6]);
	mul[6]=vaddq_u64(mul[6],mC);
	mC=vshrq_n_u64(mul[6],32);
	mul[7]=vmlal_u32(mul[7],a,b[7]);
	mul[7]=vaddq_u64(mul[7],mC);
	mC=vshrq_n_u64(mul[7],32);
	mul[8]=vmlal_u32(mul[8],a,b[8]);
	mul[8]=vaddq_u64(mul[8],mC);
	mC=vshrq_n_u64(mul[8],32);
	mul[9]=vmlal_u32(mul[9],a,b[9]);
	mul[9]=vaddq_u64(mul[9],mC);
	mC=vshrq_n_u64(mul[9],32);
	mul[10]=vmlal_u32(mul[10],a,b[10]);
	mul[10]=vaddq_u64(mul[10],mC);
	mC=vshrq_n_u64(mul[10],32);
	mul[11]=vmlal_u32(mul[11],a,b[11]);
	mul[11]=vaddq_u64(mul[11],mC);
	mC=vshrq_n_u64(mul[11],32);
	mul[12]=vmlal_u32(mul[12],a,b[12]);
	mul[12]=vaddq_u64(mul[12],mC);
	mC=vshrq_n_u64(mul[12],32);
	mul[13]=vmlal_u32(mul[13],a,b[13]);
	mul[13]=vaddq_u64(mul[13],mC);
	mC=vshrq_n_u64(mul[13],32);
	mul[14]=vaddq_u64(mul[14],mC);
	mul[0]=vandq_u64(mul[0],l);
	mul[1]=vandq_u64(mul[1],l);
	mul[2]=vandq_u64(mul[2],l);
	mul[3]=vandq_u64(mul[3],l);
	mul[4]=vandq_u64(mul[4],l);
	mul[5]=vandq_u64(mul[5],l);
	mul[6]=vandq_u64(mul[6],l);
	mul[7]=vandq_u64(mul[7],l);
	mul[8]=vandq_u64(mul[8],l);
	mul[9]=vandq_u64(mul[9],l);
	mul[10]=vandq_u64(mul[10],l);
	mul[11]=vandq_u64(mul[11],l);
	mul[12]=vandq_u64(mul[12],l);
	mul[13]=vandq_u64(mul[13],l);
	t0=vmovn_u64(mul[0]);
	mC=vmull_n_u32(t0,pi[0]);
	a=vmovn_u64(mC);
	mC=vmlal_n_u32(mul[0],a,p[0]);
	mC=vshrq_n_u64(mC,32);
	mul[0]=vmlal_n_u32(mul[1],a,p[1]);
	mul[0]=vaddq_u64(mul[0],mC);
	mC=vshrq_n_u64(mul[0],32);
	mul[1]=vmlal_n_u32(mul[2],a,p[2]);
	mul[1]=vaddq_u64(mul[1],mC);
	mC=vshrq_n_u64(mul[1],32);
	mul[2]=vmlal_n_u32(mul[3],a,p[3]);
	mul[2]=vaddq_u64(mul[2],mC);
	mC=vshrq_n_u64(mul[2],32);
	mul[3]=vmlal_n_u32(mul[4],a,p[4]);
	mul[3]=vaddq_u64(mul[3],mC);
	mC=vshrq_n_u64(mul[3],32);
	mul[4]=vmlal_n_u32(mul[5],a,p[5]);
	mul[4]=vaddq_u64(mul[4],mC);
	mC=vshrq_n_u64(mul[4],32);
	mul[5]=vmlal_n_u32(mul[6],a,p[6]);
	mul[5]=vaddq_u64(mul[5],mC);
	mC=vshrq_n_u64(mul[5],32);
	mul[6]=vmlal_n_u32(mul[7],a,p[7]);
	mul[6]=vaddq_u64(mul[6],mC);
	mC=vshrq_n_u64(mul[6],32);
	mul[7]=vmlal_n_u32(mul[8],a,p[8]);
	mul[7]=vaddq_u64(mul[7],mC);
	mC=vshrq_n_u64(mul[7],32);
	mul[8]=vmlal_n_u32(mul[9],a,p[9]);
	mul[8]=vaddq_u64(mul[8],mC);
	mC=vshrq_n_u64(mul[8],32);
	mul[9]=vmlal_n_u32(mul[10],a,p[10]);
	mul[9]=vaddq_u64(mul[9],mC);
	mC=vshrq_n_u64(mul[9],32);
	mul[10]=vmlal_n_u32(mul[11],a,p[11]);
	mul[10]=vaddq_u64(mul[10],mC);
	mC=vshrq_n_u64(mul[10],32);
	mul[11]=vmlal_n_u32(mul[12],a,p[12]);
	mul[11]=vaddq_u64(mul[11],mC);
	mC=vshrq_n_u64(mul[11],32);
	mul[12]=vmlal_n_u32(mul[13],a,p[13]);
	mul[12]=vaddq_u64(mul[12],mC);
	mC=vshrq_n_u64(mul[12],32);
	mul[13]=vaddq_u64(mul[14],mC);
	mul[14]=vshrq_n_u64(mul[13],32);

	
	
	a=vld1_u32(&A2[16]);
	mul[0]=vandq_u64(mul[0],l);
	mul[1]=vandq_u64(mul[1],l);
	mul[2]=vandq_u64(mul[2],l);
	mul[3]=vandq_u64(mul[3],l);
	mul[4]=vandq_u64(mul[4],l);
	mul[5]=vandq_u64(mul[5],l);
	mul[6]=vandq_u64(mul[6],l);
	mul[7]=vandq_u64(mul[7],l);
	mul[8]=vandq_u64(mul[8],l);
	mul[9]=vandq_u64(mul[9],l);
	mul[10]=vandq_u64(mul[10],l);
	mul[11]=vandq_u64(mul[11],l);
	mul[12]=vandq_u64(mul[12],l);
	mul[13]=vandq_u64(mul[13],l);
	
	mul[0]=vmlal_u32(mul[0],a,b[0]);
	mC=vshrq_n_u64(mul[0],32);
	mul[1]=vmlal_u32(mul[1],a,b[1]);
	mul[1]=vaddq_u64(mul[1],mC);
	mC=vshrq_n_u64(mul[1],32);
	mul[2]=vmlal_u32(mul[2],a,b[2]);
	mul[2]=vaddq_u64(mul[2],mC);
	mC=vshrq_n_u64(mul[2],32);
	mul[3]=vmlal_u32(mul[3],a,b[3]);
	mul[3]=vaddq_u64(mul[3],mC);
	mC=vshrq_n_u64(mul[3],32);
	mul[4]=vmlal_u32(mul[4],a,b[4]);
	mul[4]=vaddq_u64(mul[4],mC);
	mC=vshrq_n_u64(mul[4],32);
	mul[5]=vmlal_u32(mul[5],a,b[5]);
	mul[5]=vaddq_u64(mul[5],mC);
	mC=vshrq_n_u64(mul[5],32);
	mul[6]=vmlal_u32(mul[6],a,b[6]);
	mul[6]=vaddq_u64(mul[6],mC);
	mC=vshrq_n_u64(mul[6],32);
	mul[7]=vmlal_u32(mul[7],a,b[7]);
	mul[7]=vaddq_u64(mul[7],mC);
	mC=vshrq_n_u64(mul[7],32);
	mul[8]=vmlal_u32(mul[8],a,b[8]);
	mul[8]=vaddq_u64(mul[8],mC);
	mC=vshrq_n_u64(mul[8],32);
	mul[9]=vmlal_u32(mul[9],a,b[9]);
	mul[9]=vaddq_u64(mul[9],mC);
	mC=vshrq_n_u64(mul[9],32);
	mul[10]=vmlal_u32(mul[10],a,b[10]);
	mul[10]=vaddq_u64(mul[10],mC);
	mC=vshrq_n_u64(mul[10],32);
	mul[11]=vmlal_u32(mul[11],a,b[11]);
	mul[11]=vaddq_u64(mul[11],mC);
	mC=vshrq_n_u64(mul[11],32);
	mul[12]=vmlal_u32(mul[12],a,b[12]);
	mul[12]=vaddq_u64(mul[12],mC);
	mC=vshrq_n_u64(mul[12],32);
	mul[13]=vmlal_u32(mul[13],a,b[13]);
	mul[13]=vaddq_u64(mul[13],mC);
	mC=vshrq_n_u64(mul[13],32);
	mul[14]=vaddq_u64(mul[14],mC);
	mul[0]=vandq_u64(mul[0],l);
	mul[1]=vandq_u64(mul[1],l);
	mul[2]=vandq_u64(mul[2],l);
	mul[3]=vandq_u64(mul[3],l);
	mul[4]=vandq_u64(mul[4],l);
	mul[5]=vandq_u64(mul[5],l);
	mul[6]=vandq_u64(mul[6],l);
	mul[7]=vandq_u64(mul[7],l);
	mul[8]=vandq_u64(mul[8],l);
	mul[9]=vandq_u64(mul[9],l);
	mul[10]=vandq_u64(mul[10],l);
	mul[11]=vandq_u64(mul[11],l);
	mul[12]=vandq_u64(mul[12],l);
	mul[13]=vandq_u64(mul[13],l);
	t0=vmovn_u64(mul[0]);
	mC=vmull_n_u32(t0,pi[0]);
	a=vmovn_u64(mC);
	mC=vmlal_n_u32(mul[0],a,p[0]);
	mC=vshrq_n_u64(mC,32);
	mul[0]=vmlal_n_u32(mul[1],a,p[1]);
	mul[0]=vaddq_u64(mul[0],mC);
	mC=vshrq_n_u64(mul[0],32);
	mul[1]=vmlal_n_u32(mul[2],a,p[2]);
	mul[1]=vaddq_u64(mul[1],mC);
	mC=vshrq_n_u64(mul[1],32);
	mul[2]=vmlal_n_u32(mul[3],a,p[3]);
	mul[2]=vaddq_u64(mul[2],mC);
	mC=vshrq_n_u64(mul[2],32);
	mul[3]=vmlal_n_u32(mul[4],a,p[4]);
	mul[3]=vaddq_u64(mul[3],mC);
	mC=vshrq_n_u64(mul[3],32);
	mul[4]=vmlal_n_u32(mul[5],a,p[5]);
	mul[4]=vaddq_u64(mul[4],mC);
	mC=vshrq_n_u64(mul[4],32);
	mul[5]=vmlal_n_u32(mul[6],a,p[6]);
	mul[5]=vaddq_u64(mul[5],mC);
	mC=vshrq_n_u64(mul[5],32);
	mul[6]=vmlal_n_u32(mul[7],a,p[7]);
	mul[6]=vaddq_u64(mul[6],mC);
	mC=vshrq_n_u64(mul[6],32);
	mul[7]=vmlal_n_u32(mul[8],a,p[8]);
	mul[7]=vaddq_u64(mul[7],mC);
	mC=vshrq_n_u64(mul[7],32);
	mul[8]=vmlal_n_u32(mul[9],a,p[9]);
	mul[8]=vaddq_u64(mul[8],mC);
	mC=vshrq_n_u64(mul[8],32);
	mul[9]=vmlal_n_u32(mul[10],a,p[10]);
	mul[9]=vaddq_u64(mul[9],mC);
	mC=vshrq_n_u64(mul[9],32);
	mul[10]=vmlal_n_u32(mul[11],a,p[11]);
	mul[10]=vaddq_u64(mul[10],mC);
	mC=vshrq_n_u64(mul[10],32);
	mul[11]=vmlal_n_u32(mul[12],a,p[12]);
	mul[11]=vaddq_u64(mul[11],mC);
	mC=vshrq_n_u64(mul[11],32);
	mul[12]=vmlal_n_u32(mul[13],a,p[13]);
	mul[12]=vaddq_u64(mul[12],mC);
	mC=vshrq_n_u64(mul[12],32);
	mul[13]=vaddq_u64(mul[14],mC);
	mul[14]=vshrq_n_u64(mul[13],32);
	
	
	
	a=vld1_u32(&A2[18]);
	mul[0]=vandq_u64(mul[0],l);
	mul[1]=vandq_u64(mul[1],l);
	mul[2]=vandq_u64(mul[2],l);
	mul[3]=vandq_u64(mul[3],l);
	mul[4]=vandq_u64(mul[4],l);
	mul[5]=vandq_u64(mul[5],l);
	mul[6]=vandq_u64(mul[6],l);
	mul[7]=vandq_u64(mul[7],l);
	mul[8]=vandq_u64(mul[8],l);
	mul[9]=vandq_u64(mul[9],l);
	mul[10]=vandq_u64(mul[10],l);
	mul[11]=vandq_u64(mul[11],l);
	mul[12]=vandq_u64(mul[12],l);
	mul[13]=vandq_u64(mul[13],l);
	
	mul[0]=vmlal_u32(mul[0],a,b[0]);
	mC=vshrq_n_u64(mul[0],32);
	mul[1]=vmlal_u32(mul[1],a,b[1]);
	mul[1]=vaddq_u64(mul[1],mC);
	mC=vshrq_n_u64(mul[1],32);
	mul[2]=vmlal_u32(mul[2],a,b[2]);
	mul[2]=vaddq_u64(mul[2],mC);
	mC=vshrq_n_u64(mul[2],32);
	mul[3]=vmlal_u32(mul[3],a,b[3]);
	mul[3]=vaddq_u64(mul[3],mC);
	mC=vshrq_n_u64(mul[3],32);
	mul[4]=vmlal_u32(mul[4],a,b[4]);
	mul[4]=vaddq_u64(mul[4],mC);
	mC=vshrq_n_u64(mul[4],32);
	mul[5]=vmlal_u32(mul[5],a,b[5]);
	mul[5]=vaddq_u64(mul[5],mC);
	mC=vshrq_n_u64(mul[5],32);
	mul[6]=vmlal_u32(mul[6],a,b[6]);
	mul[6]=vaddq_u64(mul[6],mC);
	mC=vshrq_n_u64(mul[6],32);
	mul[7]=vmlal_u32(mul[7],a,b[7]);
	mul[7]=vaddq_u64(mul[7],mC);
	mC=vshrq_n_u64(mul[7],32);
	mul[8]=vmlal_u32(mul[8],a,b[8]);
	mul[8]=vaddq_u64(mul[8],mC);
	mC=vshrq_n_u64(mul[8],32);
	mul[9]=vmlal_u32(mul[9],a,b[9]);
	mul[9]=vaddq_u64(mul[9],mC);
	mC=vshrq_n_u64(mul[9],32);
	mul[10]=vmlal_u32(mul[10],a,b[10]);
	mul[10]=vaddq_u64(mul[10],mC);
	mC=vshrq_n_u64(mul[10],32);
	mul[11]=vmlal_u32(mul[11],a,b[11]);
	mul[11]=vaddq_u64(mul[11],mC);
	mC=vshrq_n_u64(mul[11],32);
	mul[12]=vmlal_u32(mul[12],a,b[12]);
	mul[12]=vaddq_u64(mul[12],mC);
	mC=vshrq_n_u64(mul[12],32);
	mul[13]=vmlal_u32(mul[13],a,b[13]);
	mul[13]=vaddq_u64(mul[13],mC);
	mC=vshrq_n_u64(mul[13],32);
	mul[14]=vaddq_u64(mul[14],mC);
	mul[0]=vandq_u64(mul[0],l);
	mul[1]=vandq_u64(mul[1],l);
	mul[2]=vandq_u64(mul[2],l);
	mul[3]=vandq_u64(mul[3],l);
	mul[4]=vandq_u64(mul[4],l);
	mul[5]=vandq_u64(mul[5],l);
	mul[6]=vandq_u64(mul[6],l);
	mul[7]=vandq_u64(mul[7],l);
	mul[8]=vandq_u64(mul[8],l);
	mul[9]=vandq_u64(mul[9],l);
	mul[10]=vandq_u64(mul[10],l);
	mul[11]=vandq_u64(mul[11],l);
	mul[12]=vandq_u64(mul[12],l);
	mul[13]=vandq_u64(mul[13],l);
	t0=vmovn_u64(mul[0]);
	mC=vmull_n_u32(t0,pi[0]);
	a=vmovn_u64(mC);
	mC=vmlal_n_u32(mul[0],a,p[0]);
	mC=vshrq_n_u64(mC,32);
	mul[0]=vmlal_n_u32(mul[1],a,p[1]);
	mul[0]=vaddq_u64(mul[0],mC);
	mC=vshrq_n_u64(mul[0],32);
	mul[1]=vmlal_n_u32(mul[2],a,p[2]);
	mul[1]=vaddq_u64(mul[1],mC);
	mC=vshrq_n_u64(mul[1],32);
	mul[2]=vmlal_n_u32(mul[3],a,p[3]);
	mul[2]=vaddq_u64(mul[2],mC);
	mC=vshrq_n_u64(mul[2],32);
	mul[3]=vmlal_n_u32(mul[4],a,p[4]);
	mul[3]=vaddq_u64(mul[3],mC);
	mC=vshrq_n_u64(mul[3],32);
	mul[4]=vmlal_n_u32(mul[5],a,p[5]);
	mul[4]=vaddq_u64(mul[4],mC);
	mC=vshrq_n_u64(mul[4],32);
	mul[5]=vmlal_n_u32(mul[6],a,p[6]);
	mul[5]=vaddq_u64(mul[5],mC);
	mC=vshrq_n_u64(mul[5],32);
	mul[6]=vmlal_n_u32(mul[7],a,p[7]);
	mul[6]=vaddq_u64(mul[6],mC);
	mC=vshrq_n_u64(mul[6],32);
	mul[7]=vmlal_n_u32(mul[8],a,p[8]);
	mul[7]=vaddq_u64(mul[7],mC);
	mC=vshrq_n_u64(mul[7],32);
	mul[8]=vmlal_n_u32(mul[9],a,p[9]);
	mul[8]=vaddq_u64(mul[8],mC);
	mC=vshrq_n_u64(mul[8],32);
	mul[9]=vmlal_n_u32(mul[10],a,p[10]);
	mul[9]=vaddq_u64(mul[9],mC);
	mC=vshrq_n_u64(mul[9],32);
	mul[10]=vmlal_n_u32(mul[11],a,p[11]);
	mul[10]=vaddq_u64(mul[10],mC);
	mC=vshrq_n_u64(mul[10],32);
	mul[11]=vmlal_n_u32(mul[12],a,p[12]);
	mul[11]=vaddq_u64(mul[11],mC);
	mC=vshrq_n_u64(mul[11],32);
	mul[12]=vmlal_n_u32(mul[13],a,p[13]);
	mul[12]=vaddq_u64(mul[12],mC);
	mC=vshrq_n_u64(mul[12],32);
	mul[13]=vaddq_u64(mul[14],mC);
	mul[14]=vshrq_n_u64(mul[13],32);
	
	
	
	a=vld1_u32(&A2[20]);
	mul[0]=vandq_u64(mul[0],l);
	mul[1]=vandq_u64(mul[1],l);
	mul[2]=vandq_u64(mul[2],l);
	mul[3]=vandq_u64(mul[3],l);
	mul[4]=vandq_u64(mul[4],l);
	mul[5]=vandq_u64(mul[5],l);
	mul[6]=vandq_u64(mul[6],l);
	mul[7]=vandq_u64(mul[7],l);
	mul[8]=vandq_u64(mul[8],l);
	mul[9]=vandq_u64(mul[9],l);
	mul[10]=vandq_u64(mul[10],l);
	mul[11]=vandq_u64(mul[11],l);
	mul[12]=vandq_u64(mul[12],l);
	mul[13]=vandq_u64(mul[13],l);
	
	mul[0]=vmlal_u32(mul[0],a,b[0]);
	mC=vshrq_n_u64(mul[0],32);
	mul[1]=vmlal_u32(mul[1],a,b[1]);
	mul[1]=vaddq_u64(mul[1],mC);
	mC=vshrq_n_u64(mul[1],32);
	mul[2]=vmlal_u32(mul[2],a,b[2]);
	mul[2]=vaddq_u64(mul[2],mC);
	mC=vshrq_n_u64(mul[2],32);
	mul[3]=vmlal_u32(mul[3],a,b[3]);
	mul[3]=vaddq_u64(mul[3],mC);
	mC=vshrq_n_u64(mul[3],32);
	mul[4]=vmlal_u32(mul[4],a,b[4]);
	mul[4]=vaddq_u64(mul[4],mC);
	mC=vshrq_n_u64(mul[4],32);
	mul[5]=vmlal_u32(mul[5],a,b[5]);
	mul[5]=vaddq_u64(mul[5],mC);
	mC=vshrq_n_u64(mul[5],32);
	mul[6]=vmlal_u32(mul[6],a,b[6]);
	mul[6]=vaddq_u64(mul[6],mC);
	mC=vshrq_n_u64(mul[6],32);
	mul[7]=vmlal_u32(mul[7],a,b[7]);
	mul[7]=vaddq_u64(mul[7],mC);
	mC=vshrq_n_u64(mul[7],32);
	mul[8]=vmlal_u32(mul[8],a,b[8]);
	mul[8]=vaddq_u64(mul[8],mC);
	mC=vshrq_n_u64(mul[8],32);
	mul[9]=vmlal_u32(mul[9],a,b[9]);
	mul[9]=vaddq_u64(mul[9],mC);
	mC=vshrq_n_u64(mul[9],32);
	mul[10]=vmlal_u32(mul[10],a,b[10]);
	mul[10]=vaddq_u64(mul[10],mC);
	mC=vshrq_n_u64(mul[10],32);
	mul[11]=vmlal_u32(mul[11],a,b[11]);
	mul[11]=vaddq_u64(mul[11],mC);
	mC=vshrq_n_u64(mul[11],32);
	mul[12]=vmlal_u32(mul[12],a,b[12]);
	mul[12]=vaddq_u64(mul[12],mC);
	mC=vshrq_n_u64(mul[12],32);
	mul[13]=vmlal_u32(mul[13],a,b[13]);
	mul[13]=vaddq_u64(mul[13],mC);
	mC=vshrq_n_u64(mul[13],32);
	mul[14]=vaddq_u64(mul[14],mC);
	mul[0]=vandq_u64(mul[0],l);
	mul[1]=vandq_u64(mul[1],l);
	mul[2]=vandq_u64(mul[2],l);
	mul[3]=vandq_u64(mul[3],l);
	mul[4]=vandq_u64(mul[4],l);
	mul[5]=vandq_u64(mul[5],l);
	mul[6]=vandq_u64(mul[6],l);
	mul[7]=vandq_u64(mul[7],l);
	mul[8]=vandq_u64(mul[8],l);
	mul[9]=vandq_u64(mul[9],l);
	mul[10]=vandq_u64(mul[10],l);
	mul[11]=vandq_u64(mul[11],l);
	mul[12]=vandq_u64(mul[12],l);
	mul[13]=vandq_u64(mul[13],l);
	t0=vmovn_u64(mul[0]);
	mC=vmull_n_u32(t0,pi[0]);
	a=vmovn_u64(mC);
	mC=vmlal_n_u32(mul[0],a,p[0]);
	mC=vshrq_n_u64(mC,32);
	mul[0]=vmlal_n_u32(mul[1],a,p[1]);
	mul[0]=vaddq_u64(mul[0],mC);
	mC=vshrq_n_u64(mul[0],32);
	mul[1]=vmlal_n_u32(mul[2],a,p[2]);
	mul[1]=vaddq_u64(mul[1],mC);
	mC=vshrq_n_u64(mul[1],32);
	mul[2]=vmlal_n_u32(mul[3],a,p[3]);
	mul[2]=vaddq_u64(mul[2],mC);
	mC=vshrq_n_u64(mul[2],32);
	mul[3]=vmlal_n_u32(mul[4],a,p[4]);
	mul[3]=vaddq_u64(mul[3],mC);
	mC=vshrq_n_u64(mul[3],32);
	mul[4]=vmlal_n_u32(mul[5],a,p[5]);
	mul[4]=vaddq_u64(mul[4],mC);
	mC=vshrq_n_u64(mul[4],32);
	mul[5]=vmlal_n_u32(mul[6],a,p[6]);
	mul[5]=vaddq_u64(mul[5],mC);
	mC=vshrq_n_u64(mul[5],32);
	mul[6]=vmlal_n_u32(mul[7],a,p[7]);
	mul[6]=vaddq_u64(mul[6],mC);
	mC=vshrq_n_u64(mul[6],32);
	mul[7]=vmlal_n_u32(mul[8],a,p[8]);
	mul[7]=vaddq_u64(mul[7],mC);
	mC=vshrq_n_u64(mul[7],32);
	mul[8]=vmlal_n_u32(mul[9],a,p[9]);
	mul[8]=vaddq_u64(mul[8],mC);
	mC=vshrq_n_u64(mul[8],32);
	mul[9]=vmlal_n_u32(mul[10],a,p[10]);
	mul[9]=vaddq_u64(mul[9],mC);
	mC=vshrq_n_u64(mul[9],32);
	mul[10]=vmlal_n_u32(mul[11],a,p[11]);
	mul[10]=vaddq_u64(mul[10],mC);
	mC=vshrq_n_u64(mul[10],32);
	mul[11]=vmlal_n_u32(mul[12],a,p[12]);
	mul[11]=vaddq_u64(mul[11],mC);
	mC=vshrq_n_u64(mul[11],32);
	mul[12]=vmlal_n_u32(mul[13],a,p[13]);
	mul[12]=vaddq_u64(mul[12],mC);
	mC=vshrq_n_u64(mul[12],32);
	mul[13]=vaddq_u64(mul[14],mC);
	mul[14]=vshrq_n_u64(mul[13],32);
	
	
	
	a=vld1_u32(&A2[22]);
	mul[0]=vandq_u64(mul[0],l);
	mul[1]=vandq_u64(mul[1],l);
	mul[2]=vandq_u64(mul[2],l);
	mul[3]=vandq_u64(mul[3],l);
	mul[4]=vandq_u64(mul[4],l);
	mul[5]=vandq_u64(mul[5],l);
	mul[6]=vandq_u64(mul[6],l);
	mul[7]=vandq_u64(mul[7],l);
	mul[8]=vandq_u64(mul[8],l);
	mul[9]=vandq_u64(mul[9],l);
	mul[10]=vandq_u64(mul[10],l);
	mul[11]=vandq_u64(mul[11],l);
	mul[12]=vandq_u64(mul[12],l);
	mul[13]=vandq_u64(mul[13],l);
	
	mul[0]=vmlal_u32(mul[0],a,b[0]);
	mC=vshrq_n_u64(mul[0],32);
	mul[1]=vmlal_u32(mul[1],a,b[1]);
	mul[1]=vaddq_u64(mul[1],mC);
	mC=vshrq_n_u64(mul[1],32);
	mul[2]=vmlal_u32(mul[2],a,b[2]);
	mul[2]=vaddq_u64(mul[2],mC);
	mC=vshrq_n_u64(mul[2],32);
	mul[3]=vmlal_u32(mul[3],a,b[3]);
	mul[3]=vaddq_u64(mul[3],mC);
	mC=vshrq_n_u64(mul[3],32);
	mul[4]=vmlal_u32(mul[4],a,b[4]);
	mul[4]=vaddq_u64(mul[4],mC);
	mC=vshrq_n_u64(mul[4],32);
	mul[5]=vmlal_u32(mul[5],a,b[5]);
	mul[5]=vaddq_u64(mul[5],mC);
	mC=vshrq_n_u64(mul[5],32);
	mul[6]=vmlal_u32(mul[6],a,b[6]);
	mul[6]=vaddq_u64(mul[6],mC);
	mC=vshrq_n_u64(mul[6],32);
	mul[7]=vmlal_u32(mul[7],a,b[7]);
	mul[7]=vaddq_u64(mul[7],mC);
	mC=vshrq_n_u64(mul[7],32);
	mul[8]=vmlal_u32(mul[8],a,b[8]);
	mul[8]=vaddq_u64(mul[8],mC);
	mC=vshrq_n_u64(mul[8],32);
	mul[9]=vmlal_u32(mul[9],a,b[9]);
	mul[9]=vaddq_u64(mul[9],mC);
	mC=vshrq_n_u64(mul[9],32);
	mul[10]=vmlal_u32(mul[10],a,b[10]);
	mul[10]=vaddq_u64(mul[10],mC);
	mC=vshrq_n_u64(mul[10],32);
	mul[11]=vmlal_u32(mul[11],a,b[11]);
	mul[11]=vaddq_u64(mul[11],mC);
	mC=vshrq_n_u64(mul[11],32);
	mul[12]=vmlal_u32(mul[12],a,b[12]);
	mul[12]=vaddq_u64(mul[12],mC);
	mC=vshrq_n_u64(mul[12],32);
	mul[13]=vmlal_u32(mul[13],a,b[13]);
	mul[13]=vaddq_u64(mul[13],mC);
	mC=vshrq_n_u64(mul[13],32);
	mul[14]=vaddq_u64(mul[14],mC);
	mul[0]=vandq_u64(mul[0],l);
	mul[1]=vandq_u64(mul[1],l);
	mul[2]=vandq_u64(mul[2],l);
	mul[3]=vandq_u64(mul[3],l);
	mul[4]=vandq_u64(mul[4],l);
	mul[5]=vandq_u64(mul[5],l);
	mul[6]=vandq_u64(mul[6],l);
	mul[7]=vandq_u64(mul[7],l);
	mul[8]=vandq_u64(mul[8],l);
	mul[9]=vandq_u64(mul[9],l);
	mul[10]=vandq_u64(mul[10],l);
	mul[11]=vandq_u64(mul[11],l);
	mul[12]=vandq_u64(mul[12],l);
	mul[13]=vandq_u64(mul[13],l);
	t0=vmovn_u64(mul[0]);
	mC=vmull_n_u32(t0,pi[0]);
	a=vmovn_u64(mC);
	mC=vmlal_n_u32(mul[0],a,p[0]);
	mC=vshrq_n_u64(mC,32);
	mul[0]=vmlal_n_u32(mul[1],a,p[1]);
	mul[0]=vaddq_u64(mul[0],mC);
	mC=vshrq_n_u64(mul[0],32);
	mul[1]=vmlal_n_u32(mul[2],a,p[2]);
	mul[1]=vaddq_u64(mul[1],mC);
	mC=vshrq_n_u64(mul[1],32);
	mul[2]=vmlal_n_u32(mul[3],a,p[3]);
	mul[2]=vaddq_u64(mul[2],mC);
	mC=vshrq_n_u64(mul[2],32);
	mul[3]=vmlal_n_u32(mul[4],a,p[4]);
	mul[3]=vaddq_u64(mul[3],mC);
	mC=vshrq_n_u64(mul[3],32);
	mul[4]=vmlal_n_u32(mul[5],a,p[5]);
	mul[4]=vaddq_u64(mul[4],mC);
	mC=vshrq_n_u64(mul[4],32);
	mul[5]=vmlal_n_u32(mul[6],a,p[6]);
	mul[5]=vaddq_u64(mul[5],mC);
	mC=vshrq_n_u64(mul[5],32);
	mul[6]=vmlal_n_u32(mul[7],a,p[7]);
	mul[6]=vaddq_u64(mul[6],mC);
	mC=vshrq_n_u64(mul[6],32);
	mul[7]=vmlal_n_u32(mul[8],a,p[8]);
	mul[7]=vaddq_u64(mul[7],mC);
	mC=vshrq_n_u64(mul[7],32);
	mul[8]=vmlal_n_u32(mul[9],a,p[9]);
	mul[8]=vaddq_u64(mul[8],mC);
	mC=vshrq_n_u64(mul[8],32);
	mul[9]=vmlal_n_u32(mul[10],a,p[10]);
	mul[9]=vaddq_u64(mul[9],mC);
	mC=vshrq_n_u64(mul[9],32);
	mul[10]=vmlal_n_u32(mul[11],a,p[11]);
	mul[10]=vaddq_u64(mul[10],mC);
	mC=vshrq_n_u64(mul[10],32);
	mul[11]=vmlal_n_u32(mul[12],a,p[12]);
	mul[11]=vaddq_u64(mul[11],mC);
	mC=vshrq_n_u64(mul[11],32);
	mul[12]=vmlal_n_u32(mul[13],a,p[13]);
	mul[12]=vaddq_u64(mul[12],mC);
	mC=vshrq_n_u64(mul[12],32);
	mul[13]=vaddq_u64(mul[14],mC);
	mul[14]=vshrq_n_u64(mul[13],32);
	
	
	a=vld1_u32(&A2[24]);
	mul[0]=vandq_u64(mul[0],l);
	mul[1]=vandq_u64(mul[1],l);
	mul[2]=vandq_u64(mul[2],l);
	mul[3]=vandq_u64(mul[3],l);
	mul[4]=vandq_u64(mul[4],l);
	mul[5]=vandq_u64(mul[5],l);
	mul[6]=vandq_u64(mul[6],l);
	mul[7]=vandq_u64(mul[7],l);
	mul[8]=vandq_u64(mul[8],l);
	mul[9]=vandq_u64(mul[9],l);
	mul[10]=vandq_u64(mul[10],l);
	mul[11]=vandq_u64(mul[11],l);
	mul[12]=vandq_u64(mul[12],l);
	mul[13]=vandq_u64(mul[13],l);
	
	mul[0]=vmlal_u32(mul[0],a,b[0]);
	mC=vshrq_n_u64(mul[0],32);
	mul[1]=vmlal_u32(mul[1],a,b[1]);
	mul[1]=vaddq_u64(mul[1],mC);
	mC=vshrq_n_u64(mul[1],32);
	mul[2]=vmlal_u32(mul[2],a,b[2]);
	mul[2]=vaddq_u64(mul[2],mC);
	mC=vshrq_n_u64(mul[2],32);
	mul[3]=vmlal_u32(mul[3],a,b[3]);
	mul[3]=vaddq_u64(mul[3],mC);
	mC=vshrq_n_u64(mul[3],32);
	mul[4]=vmlal_u32(mul[4],a,b[4]);
	mul[4]=vaddq_u64(mul[4],mC);
	mC=vshrq_n_u64(mul[4],32);
	mul[5]=vmlal_u32(mul[5],a,b[5]);
	mul[5]=vaddq_u64(mul[5],mC);
	mC=vshrq_n_u64(mul[5],32);
	mul[6]=vmlal_u32(mul[6],a,b[6]);
	mul[6]=vaddq_u64(mul[6],mC);
	mC=vshrq_n_u64(mul[6],32);
	mul[7]=vmlal_u32(mul[7],a,b[7]);
	mul[7]=vaddq_u64(mul[7],mC);
	mC=vshrq_n_u64(mul[7],32);
	mul[8]=vmlal_u32(mul[8],a,b[8]);
	mul[8]=vaddq_u64(mul[8],mC);
	mC=vshrq_n_u64(mul[8],32);
	mul[9]=vmlal_u32(mul[9],a,b[9]);
	mul[9]=vaddq_u64(mul[9],mC);
	mC=vshrq_n_u64(mul[9],32);
	mul[10]=vmlal_u32(mul[10],a,b[10]);
	mul[10]=vaddq_u64(mul[10],mC);
	mC=vshrq_n_u64(mul[10],32);
	mul[11]=vmlal_u32(mul[11],a,b[11]);
	mul[11]=vaddq_u64(mul[11],mC);
	mC=vshrq_n_u64(mul[11],32);
	mul[12]=vmlal_u32(mul[12],a,b[12]);
	mul[12]=vaddq_u64(mul[12],mC);
	mC=vshrq_n_u64(mul[12],32);
	mul[13]=vmlal_u32(mul[13],a,b[13]);
	mul[13]=vaddq_u64(mul[13],mC);
	mC=vshrq_n_u64(mul[13],32);
	mul[14]=vaddq_u64(mul[14],mC);
	mul[0]=vandq_u64(mul[0],l);
	mul[1]=vandq_u64(mul[1],l);
	mul[2]=vandq_u64(mul[2],l);
	mul[3]=vandq_u64(mul[3],l);
	mul[4]=vandq_u64(mul[4],l);
	mul[5]=vandq_u64(mul[5],l);
	mul[6]=vandq_u64(mul[6],l);
	mul[7]=vandq_u64(mul[7],l);
	mul[8]=vandq_u64(mul[8],l);
	mul[9]=vandq_u64(mul[9],l);
	mul[10]=vandq_u64(mul[10],l);
	mul[11]=vandq_u64(mul[11],l);
	mul[12]=vandq_u64(mul[12],l);
	mul[13]=vandq_u64(mul[13],l);
	t0=vmovn_u64(mul[0]);
	mC=vmull_n_u32(t0,pi[0]);
	a=vmovn_u64(mC);
	mC=vmlal_n_u32(mul[0],a,p[0]);
	mC=vshrq_n_u64(mC,32);
	mul[0]=vmlal_n_u32(mul[1],a,p[1]);
	mul[0]=vaddq_u64(mul[0],mC);
	mC=vshrq_n_u64(mul[0],32);
	mul[1]=vmlal_n_u32(mul[2],a,p[2]);
	mul[1]=vaddq_u64(mul[1],mC);
	mC=vshrq_n_u64(mul[1],32);
	mul[2]=vmlal_n_u32(mul[3],a,p[3]);
	mul[2]=vaddq_u64(mul[2],mC);
	mC=vshrq_n_u64(mul[2],32);
	mul[3]=vmlal_n_u32(mul[4],a,p[4]);
	mul[3]=vaddq_u64(mul[3],mC);
	mC=vshrq_n_u64(mul[3],32);
	mul[4]=vmlal_n_u32(mul[5],a,p[5]);
	mul[4]=vaddq_u64(mul[4],mC);
	mC=vshrq_n_u64(mul[4],32);
	mul[5]=vmlal_n_u32(mul[6],a,p[6]);
	mul[5]=vaddq_u64(mul[5],mC);
	mC=vshrq_n_u64(mul[5],32);
	mul[6]=vmlal_n_u32(mul[7],a,p[7]);
	mul[6]=vaddq_u64(mul[6],mC);
	mC=vshrq_n_u64(mul[6],32);
	mul[7]=vmlal_n_u32(mul[8],a,p[8]);
	mul[7]=vaddq_u64(mul[7],mC);
	mC=vshrq_n_u64(mul[7],32);
	mul[8]=vmlal_n_u32(mul[9],a,p[9]);
	mul[8]=vaddq_u64(mul[8],mC);
	mC=vshrq_n_u64(mul[8],32);
	mul[9]=vmlal_n_u32(mul[10],a,p[10]);
	mul[9]=vaddq_u64(mul[9],mC);
	mC=vshrq_n_u64(mul[9],32);
	mul[10]=vmlal_n_u32(mul[11],a,p[11]);
	mul[10]=vaddq_u64(mul[10],mC);
	mC=vshrq_n_u64(mul[10],32);
	mul[11]=vmlal_n_u32(mul[12],a,p[12]);
	mul[11]=vaddq_u64(mul[11],mC);
	mC=vshrq_n_u64(mul[11],32);
	mul[12]=vmlal_n_u32(mul[13],a,p[13]);
	mul[12]=vaddq_u64(mul[12],mC);
	mC=vshrq_n_u64(mul[12],32);
	mul[13]=vaddq_u64(mul[14],mC);
	mul[14]=vshrq_n_u64(mul[13],32);
	
	
	
	a=vld1_u32(&A2[26]);
	mul[0]=vandq_u64(mul[0],l);
	mul[1]=vandq_u64(mul[1],l);
	mul[2]=vandq_u64(mul[2],l);
	mul[3]=vandq_u64(mul[3],l);
	mul[4]=vandq_u64(mul[4],l);
	mul[5]=vandq_u64(mul[5],l);
	mul[6]=vandq_u64(mul[6],l);
	mul[7]=vandq_u64(mul[7],l);
	mul[8]=vandq_u64(mul[8],l);
	mul[9]=vandq_u64(mul[9],l);
	mul[10]=vandq_u64(mul[10],l);
	mul[11]=vandq_u64(mul[11],l);
	mul[12]=vandq_u64(mul[12],l);
	mul[13]=vandq_u64(mul[13],l);
	
	mul[0]=vmlal_u32(mul[0],a,b[0]);
	mC=vshrq_n_u64(mul[0],32);
	mul[1]=vmlal_u32(mul[1],a,b[1]);
	mul[1]=vaddq_u64(mul[1],mC);
	mC=vshrq_n_u64(mul[1],32);
	mul[2]=vmlal_u32(mul[2],a,b[2]);
	mul[2]=vaddq_u64(mul[2],mC);
	mC=vshrq_n_u64(mul[2],32);
	mul[3]=vmlal_u32(mul[3],a,b[3]);
	mul[3]=vaddq_u64(mul[3],mC);
	mC=vshrq_n_u64(mul[3],32);
	mul[4]=vmlal_u32(mul[4],a,b[4]);
	mul[4]=vaddq_u64(mul[4],mC);
	mC=vshrq_n_u64(mul[4],32);
	mul[5]=vmlal_u32(mul[5],a,b[5]);
	mul[5]=vaddq_u64(mul[5],mC);
	mC=vshrq_n_u64(mul[5],32);
	mul[6]=vmlal_u32(mul[6],a,b[6]);
	mul[6]=vaddq_u64(mul[6],mC);
	mC=vshrq_n_u64(mul[6],32);
	mul[7]=vmlal_u32(mul[7],a,b[7]);
	mul[7]=vaddq_u64(mul[7],mC);
	mC=vshrq_n_u64(mul[7],32);
	mul[8]=vmlal_u32(mul[8],a,b[8]);
	mul[8]=vaddq_u64(mul[8],mC);
	mC=vshrq_n_u64(mul[8],32);
	mul[9]=vmlal_u32(mul[9],a,b[9]);
	mul[9]=vaddq_u64(mul[9],mC);
	mC=vshrq_n_u64(mul[9],32);
	mul[10]=vmlal_u32(mul[10],a,b[10]);
	mul[10]=vaddq_u64(mul[10],mC);
	mC=vshrq_n_u64(mul[10],32);
	mul[11]=vmlal_u32(mul[11],a,b[11]);
	mul[11]=vaddq_u64(mul[11],mC);
	mC=vshrq_n_u64(mul[11],32);
	mul[12]=vmlal_u32(mul[12],a,b[12]);
	mul[12]=vaddq_u64(mul[12],mC);
	mC=vshrq_n_u64(mul[12],32);
	mul[13]=vmlal_u32(mul[13],a,b[13]);
	mul[13]=vaddq_u64(mul[13],mC);
	mC=vshrq_n_u64(mul[13],32);
	mul[14]=vaddq_u64(mul[14],mC);
	mul[0]=vandq_u64(mul[0],l);
	mul[1]=vandq_u64(mul[1],l);
	mul[2]=vandq_u64(mul[2],l);
	mul[3]=vandq_u64(mul[3],l);
	mul[4]=vandq_u64(mul[4],l);
	mul[5]=vandq_u64(mul[5],l);
	mul[6]=vandq_u64(mul[6],l);
	mul[7]=vandq_u64(mul[7],l);
	mul[8]=vandq_u64(mul[8],l);
	mul[9]=vandq_u64(mul[9],l);
	mul[10]=vandq_u64(mul[10],l);
	mul[11]=vandq_u64(mul[11],l);
	mul[12]=vandq_u64(mul[12],l);
	mul[13]=vandq_u64(mul[13],l);
	t0=vmovn_u64(mul[0]);
	mC=vmull_n_u32(t0,pi[0]);
	a=vmovn_u64(mC);
	mC=vmlal_n_u32(mul[0],a,p[0]);
	mC=vshrq_n_u64(mC,32);
	mul[0]=vmlal_n_u32(mul[1],a,p[1]);
	mul[0]=vaddq_u64(mul[0],mC);
	mC=vshrq_n_u64(mul[0],32);
	mul[1]=vmlal_n_u32(mul[2],a,p[2]);
	mul[1]=vaddq_u64(mul[1],mC);
	mC=vshrq_n_u64(mul[1],32);
	mul[2]=vmlal_n_u32(mul[3],a,p[3]);
	mul[2]=vaddq_u64(mul[2],mC);
	mC=vshrq_n_u64(mul[2],32);
	mul[3]=vmlal_n_u32(mul[4],a,p[4]);
	mul[3]=vaddq_u64(mul[3],mC);
	mC=vshrq_n_u64(mul[3],32);
	mul[4]=vmlal_n_u32(mul[5],a,p[5]);
	mul[4]=vaddq_u64(mul[4],mC);
	mC=vshrq_n_u64(mul[4],32);
	mul[5]=vmlal_n_u32(mul[6],a,p[6]);
	mul[5]=vaddq_u64(mul[5],mC);
	mC=vshrq_n_u64(mul[5],32);
	mul[6]=vmlal_n_u32(mul[7],a,p[7]);
	mul[6]=vaddq_u64(mul[6],mC);
	mC=vshrq_n_u64(mul[6],32);
	mul[7]=vmlal_n_u32(mul[8],a,p[8]);
	mul[7]=vaddq_u64(mul[7],mC);
	mC=vshrq_n_u64(mul[7],32);
	mul[8]=vmlal_n_u32(mul[9],a,p[9]);
	mul[8]=vaddq_u64(mul[8],mC);
	mC=vshrq_n_u64(mul[8],32);
	mul[9]=vmlal_n_u32(mul[10],a,p[10]);
	mul[9]=vaddq_u64(mul[9],mC);
	mC=vshrq_n_u64(mul[9],32);
	mul[10]=vmlal_n_u32(mul[11],a,p[11]);
	mul[10]=vaddq_u64(mul[10],mC);
	mC=vshrq_n_u64(mul[10],32);
	mul[11]=vmlal_n_u32(mul[12],a,p[12]);
	mul[11]=vaddq_u64(mul[11],mC);
	mC=vshrq_n_u64(mul[11],32);
	mul[12]=vmlal_n_u32(mul[13],a,p[13]);
	mul[12]=vaddq_u64(mul[12],mC);
	mC=vshrq_n_u64(mul[12],32);
	mul[13]=vaddq_u64(mul[14],mC);
	mul[14]=vshrq_n_u64(mul[13],32);
	
	
	
	b[0]=vmovn_u64(mul[0]);
	b[1]=vmovn_u64(mul[1]);
	b[2]=vmovn_u64(mul[2]);
	b[3]=vmovn_u64(mul[3]);
	b[4]=vmovn_u64(mul[4]);
	b[5]=vmovn_u64(mul[5]);
	b[6]=vmovn_u64(mul[6]);
	b[7]=vmovn_u64(mul[7]);
	b[8]=vmovn_u64(mul[8]);
	b[9]=vmovn_u64(mul[9]);
	b[10]=vmovn_u64(mul[10]);
	b[11]=vmovn_u64(mul[11]);
	b[12]=vmovn_u64(mul[12]);
	b[13]=vmovn_u64(mul[13]);
	
	
	T[0]=vget_lane_u32(b[0],0);
	T2[0]=vget_lane_u32(b[0],1);
	T[1]=vget_lane_u32(b[1],0);
	T2[1]=vget_lane_u32(b[1],1);
	T[2]=vget_lane_u32(b[2],0);
	T2[2]=vget_lane_u32(b[2],1);
	T[3]=vget_lane_u32(b[3],0);
	T2[3]=vget_lane_u32(b[3],1);
	T[4]=vget_lane_u32(b[4],0);
	T2[4]=vget_lane_u32(b[4],1);
	T[5]=vget_lane_u32(b[5],0);
	T2[5]=vget_lane_u32(b[5],1);
	T[6]=vget_lane_u32(b[6],0);
	T2[6]=vget_lane_u32(b[6],1);
	T[7]=vget_lane_u32(b[7],0);
	T2[7]=vget_lane_u32(b[7],1);
	T[8]=vget_lane_u32(b[8],0);
	T2[8]=vget_lane_u32(b[8],1);
	T[9]=vget_lane_u32(b[9],0);
	T2[9]=vget_lane_u32(b[9],1);
	T[10]=vget_lane_u32(b[10],0);
	T2[10]=vget_lane_u32(b[10],1);
	T[11]=vget_lane_u32(b[11],0);
	T2[11]=vget_lane_u32(b[11],1);
	T[12]=vget_lane_u32(b[12],0);
	T2[12]=vget_lane_u32(b[12],1);
	T[13]=vget_lane_u32(b[13],0);
	T2[13]=vget_lane_u32(b[13],1);

	while(T[13]>p[13]){
		dig_t carry;
		carry = mpn_sub_n(T, T, p, FP_DIGS);
	}
	while(T2[13]>p[13]){
		dig_t carry;
		carry = mpn_sub_n(T2, T2, p, FP_DIGS);
	}
}


void fp2_mul_dual(fp2_t C1, fp2_t C2, fp2_t A1, fp2_t B1, fp2_t A2, fp2_t B2)
{
	fp_t S1, T1, S2, T2;
	dv_t d0, d1,d2,e0,e1,e2;

	fp_add(S1, A1[0], A1[1]);
	fp_add(T1, B1[0], B1[1]);
	fp_add(S2, A2[0], A2[1]);
	fp_add(T2, B2[0], B2[1]);
	
	fp_mul_446_dual_NEON_nr(d0,e0,S1,T1,S2,T2);
	fp_mul_446_dual_NEON_nr(d1,d2,A1[0], B1[0], A1[1], B1[1]);
	fp_mul_446_dual_NEON_nr(e1,e2,A2[0], B2[0], A2[1], B2[1]);

	fp_subd(d0, d0, d1);
	fp_subd(d0, d0, d2);
	fp_subd(d1,d1, d2);

	fp_subd(e0, e0, e1);
	fp_subd(e0, e0, e2);
	fp_subd(e1 ,e1, e2);

	fp2_rdc_NEON(C1[1], C1[0], d0, d1);
	fp2_rdc_NEON(C2[1], C2[0], e0, e1);
}
void fp2_mul_dual_nr(dv2_t C1, dv2_t C2, fp2_t A1, fp2_t B1, fp2_t A2, fp2_t B2)
{
	fp_t S1, T1, S2, T2;
	dv_t d0, d1,d2,e0,e1,e2;

	fp_add(S1, A1[0], A1[1]);
	fp_add(T1, B1[0], B1[1]);
	fp_add(S2, A2[0], A2[1]);
	fp_add(T2, B2[0], B2[1]);
	
	fp_mul_446_dual_NEON_nr(d0,e0,S1,T1,S2,T2);
	fp_mul_446_dual_NEON_nr(d1,d2,A1[0], B1[0], A1[1], B1[1]);
	fp_mul_446_dual_NEON_nr(e1,e2,A2[0], B2[0], A2[1], B2[1]);

	fp_subd(d0, d0, d1);
	fp_subd(d0, d0, d2);
	fp_subd(d1,d1, d2);

	fp_subd(e0, e0, e1);
	fp_subd(e0, e0, e2);
	fp_subd(e1 ,e1, e2);
}


void fp2_const_init() {
#if ALLOC == STATIC
	fp2_new(const_frb);
#endif
}

void fp2_const_clean() {
#if ALLOC == STATIC
	fp2_free(const_frb);
#endif
}

/*
void fp2_const_calc() {
	bn_t e;
	fp2_t t;

	bn_null(e);
	fp2_null(t);

	TRY {
		bn_new(e);
		fp2_new(t);
		fp2_zero(t);
		fp_set_dig(t[0], 1);
		fp2_mul_nor(t, t);
		e->used = FP_DIGS;
		dv_copy(e->dp, fp_prime_get(), FP_DIGS);
		bn_sub_dig(e, e, 1);
		bn_div_dig(e, e, 6);
		fp2_exp(t, t, e);
		fp_copy(const_frb[0], t[0]);
		fp_copy(const_frb[1], t[1]);
	} CATCH_ANY {
		THROW(ERR_CAUGHT);
	} FINALLY {
		bn_free(e);
		fp2_free(t);
	}
}
*/
void fp2_const_calc() {
        bn_t e;
	fp2_t t, t1;

	bn_null(e);
	fp2_null(t);
	fp2_null(t1);

	TRY {
	        bn_new(e);
		fp2_new(t);
		fp2_new(t1);
		fp2_zero(t);
		fp_set_dig(t[0], 1);
		fp2_mul_nor(t, t);
		fp2_copy(t1, t);
		fp_neg(t1[1], t1[1]);
		fp2_mul(t1, t1, t);
		
		e->used = FP_DIGS;
		dv_copy(e->dp, fp_prime_get(), FP_DIGS);
		bn_sub_dig(e, e, 1);
		bn_div_dig(e, e, 6);
		
		fp2_exp(t, t, e);
		fp2_exp(t1, t1, e);
		fp_copy(const_frb[0][0], t[0]);
		fp_copy(const_frb[0][1], t[1]);
		fp2_sqr(const_frb[1], const_frb[0]);
		fp2_mul(const_frb[2], const_frb[1], const_frb[0]);
		fp2_sqr(const_frb[3], const_frb[1]);
		fp2_mul(const_frb[4], const_frb[3], const_frb[0]);
		
		fp_copy(const_sqr[0], t1[0]);
		fp_sqr(const_sqr[1], const_sqr[0]);
		fp_mul(const_sqr[2], const_sqr[1], const_sqr[0]);

		//for m-type twist
		if(fp_param_get() != BNN_256){
		fp2_zero(t);
		fp_set_dig(t[0], 1);
		fp2_mul_nor(t, t);

		fp2_mul_nor(const_frb[5], const_frb[3]);
		fp2_mul_nor(const_frb[6], const_frb[2]);
		fp2_inv(const_frb[7], t);}
	} CATCH_ANY {
	        THROW(ERR_CAUGHT);
	} FINALLY {
	        bn_free(e);
		fp2_free(t);
		fp2_free(t1);
	}
}

void fp2_mul_frb(fp2_t c, fp2_t a, int i) {
        fp2_mul(c, a, const_frb[i-1]);
}

void fp2_mul_frb_sqr(fp2_t c, fp2_t a, int i) {
        //fp_mul(c[0], a[0], const_sqr[i-1]);
        //fp_mul(c[1], a[1], const_sqr[i-1]);
		fp_mul_446_dual_NEON(c[0], c[1], a[0], const_sqr[i-1], a[1], const_sqr[i-1]);
}

void fp2_dbl(fp2_t c, fp2_t a) {
	/* 2 * (a0 + a1 * u) = 2 * a0 + 2 * a1 * u. */
	/*fp_dbl(c[0], a[0]);
	fp_dbl(c[1], a[1]);*/
	fp2_add(c,a,a);
}


/*void fp2_mul(fp2_t c, fp2_t a, fp2_t b) {
// Full quadratic extension field multiplication using Karatsuba

	fp_t t0, t1, t2;

	fp_null(t0);
	fp_null(t1);
	fp_null(t2);

	TRY {

		fp_new(t0);
		fp_new(t1);
		fp_new(t2);

		// Karatsuba algorithm. //
		fp_mul(t0, a[0], b[0]);
		fp_print(t0);
		fp_mul(t1, a[1], b[1]);
		fp_print(t1);
		fp_add(t2, a[0], a[1]);
		fp_print(t2);
		fp_add(c[1], b[0], b[1]);
		fp_print(c[1]);
		fp_mul(c[1], c[1], t2);
		fp_print(c[1]);
		fp_sub(c[1], c[1], t0);
		fp_print(c[1]);
		fp_sub(c[1], c[1], t1);
		fp_print(c[1]);
		fp_sub(c[0], t0, t1);
		fp_print(c[0]);

		if (fp_prime_get_qnr() == -2) {
			fp_sub(c[0], c[0], t1);
		}
	}
	CATCH_ANY {
		THROW(ERR_CAUGHT);
	}
	FINALLY {
		fp_free(t0);
		fp_free(t1);
		fp_free(t2);
	}
}*/


void fp2_mul(fp2_t c, fp2_t a, fp2_t b) {
// Full quadratic extension field multiplication using Karatsuba and lazy reduction
        fp_t t0, t1;
        dv_t D0, D1, D2;

        fp_null(t0);
        fp_null(t1);
        dv_null(D0);
        dv_null(D1);
        dv_null(D2);

        TRY {
                fp_new(t0);
                fp_new(t1);
                dv_new(D0);
                dv_new(D1);
                dv_new(D2);

                // Karatsuba algorithm. //
                //fp_muln_low(D0, a[0], b[0]);            // D0 = a0*b0
                //fp_muln_low(D1, a[1], b[1]);            // D1 = a1*b1
                fp_mul_446_dual_NEON_nr(D0, D1, a[0], b[0], a[1], b[1]);
				
				fp_subd(D2, D0, D1);                    // D2 = a0*b0 - a1*b1
                if (fp_prime_get_qnr() == -2) {
                       fp_subd(D2, D2, D1);             // D2 = a0*b0 + beta*a1*b1
                }
                fp_add(t0, a[0], a[1]);                 // t0 = a0+a1
                fp_add(t1, b[0], b[1]);                 // t1 = b0+b1
                fp_rdc(c[0],D2);                        // c0
                fp_muln_low(D2, t0, t1);                // D[1] = (a0+a1)(b0+b1)
                fp_subd(D2, D2, D0);                    // D[1] = (a0+a1)(b0+b1) - a0*b0
                fp_subd(D2, D2, D1);                    // D[1] = (a0+a1)(b0+b1) - a0*b0 - a1*b1
                fp_rdc(c[1],D2);                        // c1
        }
        CATCH_ANY {
                THROW(ERR_CAUGHT);
        }
        FINALLY {
                fp_free(t0);
                fp_free(t1);
                dv_free(D0);
                dv_free(D1);
                dv_free(D2);
        }
}

void fp2_mul_nr(dv2_t D, fp2_t a, fp2_t b) {
// Quadratic extension field multiplication w/o reduction using Karatsuba and lazy reduction
        fp_t t0, t1;
        dv_t D0, D1;

        fp_null(t0);
        fp_null(t1);
        dv_null(D0);
        dv_null(D1);

        TRY {
                fp_new(t0);
                fp_new(t1);
                dv_new(D0);
                dv_new(D1);

                // Karatsuba algorithm. //
                //fp_muln_low(D0, a[0], b[0]);            // D0 = a0*b0
                //fp_muln_low(D1, a[1], b[1]);            // D1 = a1*b1
				fp_mul_446_dual_NEON_nr(D0, D1, a[0], b[0], a[1], b[1]);
                fp_subd(D[0], D0, D1);                  // D[0] = a0*b0 - a1*b1
                if (fp_prime_get_qnr() == -2) {
                       fp_subd(D[0], D[0], D1);         // D[0] = a0*b0 + beta*a1*b1
                }
                fp_add(t0, a[0], a[1]);                 // t0 = a0+a1
                fp_add(t1, b[0], b[1]);                 // c1 = b0+b1
                //fp_rdc(c[0],D[0]);                    // c0
                fp_muln_low(D[1], t0, t1);              // D[1] = (a0+a1)(b0+b1)
                fp_subd(D[1], D[1], D0);                // D[1] = (a0+a1)(b0+b1) - a0*b0
                fp_subd(D[1], D[1], D1);                // D[1] = (a0+a1)(b0+b1) - a0*b0 - a1*b1
                //fp_rdc(c[1],D2);                      // c1
         }
         CATCH_ANY {
                THROW(ERR_CAUGHT);
         }
         FINALLY {
                fp_free(t0);
                fp_free(t1);
                dv_free(D0);
                dv_free(D1);
         }
}

/*
static inline void fp2_mulkar_nr(dv2_t D, fp2_t a, fp2_t b) {
// Quadratic extension field multiplication using Karatsuba and lazy reduction
        fp_t t0, t1;
        dv_t D0, D1;

	fp_null(t0);
	fp_null(t1);
	dv_null(D0);
	dv_null(D1);

        TRY {
	        fp_new(t0);
                fp_new(t1);
		dv_new(D0);
                dv_new(D1);

		// Karatsuba algorithm. //
	        fp_muln_low(D0, a[0], b[0]);            // D0 = a0*b0
	        fp_muln_low(D1, a[1], b[1]);            // D1 = a1*b1
                fp_subd(D[0], D0, D1);                  // D[0] = a0*b0 - a1*b1
                if (fp_prime_get_qnr() == -2) {
		       fp_subd(D[0], D[0], D1);         // D[0] = a0*b0 + beta*a1*b1
                }
	        fp_add(t0, a[0], a[1]);                 // t0 = a0+a1
	        fp_add(t1, b[0], b[1]);                 // c1 = b0+b1
                //fp_rdc(c[0],D[0]);                    // c0
	        fp_muln_low(D[1], t0, t1);              // D[1] = (a0+a1)(b0+b1)
	        fp_subd(D[1], D[1], D0);                // D[1] = (a0+a1)(b0+b1) - a0*b0
	        fp_subd(D[1], D[1], D1);                // D[1] = (a0+a1)(b0+b1) - a0*b0 - a1*b1
		//fp_rdc(c[1],D2);                      // c1
        }
        CATCH_ANY {
	  THROW(ERR_CAUGHT);
        }
        FINALLY {
	  fp_free(t0);
          fp_free(t1);
          dv_free(D0);
          dv_free(D1);
        }
}

void fp2_mul(fp2_t c, fp2_t a, fp2_t b) {
// Full quadratic extension field multiplication
        dv2_t C;

        dv2_null(C);

        TRY {
                 dv2_new(C);

                 fp2_mulkar_nr(C, a, b);      // Karatsuba multiplication            
                 fp2_rdc(c,C);
        }
        CATCH_ANY {
                 THROW(ERR_CAUGHT);
        }
        FINALLY {
                 dv2_free(C);
        }
}

void fp2_mul_nr(dv2_t C, fp2_t a, fp2_t b) {
// Quadratic extension field multiplication w/o reduction           
        TRY {
                 fp2_mulkar_nr(C, a, b);      // Karatsuba multiplication
        }
        CATCH_ANY {
                 THROW(ERR_CAUGHT);
        }
        FINALLY {
        }
}
*/

void fp2_mul_art(fp2_t c, fp2_t a) {
	fp_t t0;

	fp_null(t0);

	TRY {
		fp_new(t0);

		/* (a0 + a1 * u) * u = a1 * u^2 + a0 * u. */
		fp_copy(t0, a[0]);
		fp_neg(c[0], a[1]);
		if (fp_prime_get_qnr() == -2) {
			fp_dbl(c[0], c[0]);
		}
		fp_copy(c[1], t0);
	}
	CATCH_ANY {
		THROW(ERR_CAUGHT);
	}
	FINALLY {
		fp_free(t0);
	}
}

void fp2_mul_artd(dv2_t C, dv2_t A) {
        dv_t T0;

        dv_null(T0);

        TRY {
                dv_new(T0);

                /* (a0 + a1 * u) * u = a1 * u^2 + a0 * u. */
                dv_copy(T0, A[0], 2*FP_DIGS);
                fp_negd(C[0], A[1]);
                if (fp_prime_get_qnr() == -2) {
		        fp_addd(C[0], C[0], C[0]);
                }
                dv_copy(C[1], T0, 2*FP_DIGS);
        }
        CATCH_ANY {
                THROW(ERR_CAUGHT);
        }
        FINALLY {
                dv_free(T0);
        }
}

void fp2_mul_nor(fp2_t c, fp2_t a) {
	fp2_t t0;

	fp2_null(t0);

	TRY {
		fp2_new(t0);

		switch (fp_prime_get_mod8()) {
			case 5:
				/* If p = 5 mod 8, x^2 - sqrt(sqrt(-2)) is irreducible. */
				fp2_mul_art(c, a);
				break;
			case 3:
				/* If p = 3 mod 8, x^2 - sqrt(1 + sqrt(-1)) is irreducible. */
				//fp2_mul_art(t0, a);
				//fp2_add(c, t0, a);
				fp_sub(t0[0], a[0], a[1]);
				fp_add(c[1], a[0], a[1]);
				fp_copy(c[0], t0[0]);
				break;
			case 7:
				/* If p = 7 mod 8 and p = 2,3 mod 5, x^2 - sqrt(2 + sqrt(-1)) is
				 * irreducible. */
				//fp2_mul_art(t0, a);
				//fp2_add(c, t0, a);
				fp_dbl(t0[0], a[1]);
				fp_sub(t0[0], a[0], t0[0]);
				fp_add(c[1], a[1], a[0]);
				fp_copy(c[0], t0[0]);
				break;
			default:
				THROW(ERR_INVALID);
		}
	}
	CATCH_ANY {
		THROW(ERR_CAUGHT);
	}
	FINALLY {
		fp2_free(t0);
	}
}

void fp2_mul_nord(dv2_t C, dv2_t A) {
        dv2_t T0;

        dv2_null(T0);

        TRY {
                dv2_new(T0);

                switch (fp_prime_get_mod8()) {
                case 5:
                /* If p = 5 mod 8, x^2 - sqrt(sqrt(-2)) is irreducible. */
                       fp2_mul_artd(C, A);
                break;
                case 3:
                /* If p = 3 mod 8, x^2 - sqrt(1 + sqrt(-1)) is irreducible. */
                      // fp2_mul_artd(T0, A);
                       //fp2_addd(C, T0, A);
			fp_subd(T0[0], A[0], A[1]);
			fp_addd(C[1], A[0], A[1]);
			dv_copy(C[0], T0[0], 2*FP_DIGS);
                break;
                case 7:
                /* If p = 7 mod 8 and p = 2,3 mod 5, x^2 - sqrt(2 + sqrt(-1)) is
                 * irreducible. */
                       //fp2_mul_artd(T0, A);
                       //fp2_addd(T1, T0, A);

			fp_addd(T0[0], A[1], A[1]);
			fp_subd(T0[0], A[0], T0[0]);
			fp_addd(C[1], A[1], A[0]);
			dv_copy(C[0], T0[0], 2*FP_DIGS);

                break;
                default:
                       THROW(ERR_INVALID);
                }
        }
        CATCH_ANY {
                THROW(ERR_CAUGHT);
        }
        FINALLY {
                dv2_free(T0);
        }
}

void fp2_sqr(fp2_t c, fp2_t a) {
	fp_t t0, t1;

	fp_null(t0);
	fp_null(t1);

	TRY {
		fp_new(t0);
		fp_new(t1);

		fp_add(t0, a[0], a[1]);          // t0 = (a0 + a1)
		fp_sub(t1, a[0], a[1]);          // t1 = (a0 - a1)			
		if (fp_prime_get_qnr() == -2) {
		        fp_sub(t1, t1, a[1]);    // t1 = (a0 - 2 * a1)
		}
#if USE_NEON_FP2
		fp_mul_446_dual_NEON(c[1], c[0], a[0], a[1], t0, t1 );
#else
		fp_mul(c[1], a[0], a[1]);        // c_1 = a0 * a1

		// c_0 = a0^2 + b_0^2 * u^2
		fp_mul(c[0], t0, t1);
#endif
		
		if (fp_prime_get_qnr() == -2) {
			fp_add(c[0], c[0], c[1]);
		}
		fp_dbl(c[1], c[1]);              // c_1 = 2 * a0 * a1
	}
	CATCH_ANY {
		THROW(ERR_CAUGHT);
	}
	FINALLY {
		fp_free(t0);
		fp_free(t1);
	}
}

void fp2_sqr_nr(dv2_t C, fp2_t a) {
        fp_t t0, t1;

	fp_null(t0);
	fp_null(t1);

	TRY {
	        fp_new(t0);
		fp_new(t1);

		fp_add(t0, a[0], a[1]);                // t0 = (a0 + a1)
		fp_sub(t1, a[0], a[1]);                // t1 = (a0 - a1)
		if (fp_prime_get_qnr() == -2) {
		         fp_sub(t1, t1, a[1]);         // t1 = (a0 - 2 * a1)		     
		}
#if USE_NEON_FP2
		fp_mul_446_dual_NEON_nr(C[1], C[0], a[0], a[1], t0, t1);
#else
		fp_muln_low(C[1], a[0], a[1]);         // C_1 = a0 * a1
		fp_muln_low(C[0], t0, t1);   	       // C_0 = a0^2 + b_0^2 * u^2
#endif
		if (fp_prime_get_qnr() == -2) {
                         fp_addd(C[0], C[0], C[1]);
		}
		fp_addd(C[1], C[1], C[1]);             // C_1 = 2 * a0 * a1
	}
	CATCH_ANY {
	        THROW(ERR_CAUGHT);
	}
	FINALLY {
	        fp_free(t0);
		fp_free(t1);
	}
}

void fp2_inv(fp2_t c, fp2_t a) {
	fp_t t, t0;
	dv_t T0, T1;

	fp_null(t);
	fp_null(t0);
	dv_null(T0);
	dv_null(T1);

	TRY {
		fp_new(t);
		fp_new(t0);
		dv_new(T0);
		dv_new(T1);
#if USE_NEON_FP2
		fp_mul_446_dual_NEON_nr(T0, T1, a[0], a[0], a[1], a[1]);
#else
		/* t0 = a0^2, t1 = a1^2. */
		fp_muln_low(T0, a[0], a[0]);
		fp_muln_low(T1, a[1], a[1]);
#endif
		if (fp_prime_get_qnr() == -2) {
			fp_addd(T1, T1, T1);
		}

		/* t1 = 1/(a0^2 + a1^2). */
		fp_addd(T0, T0, T1);
		fp_rdc(t0, T0);
		fp_inv(t, t0);
#if USE_NEON_FP2		
		fp_mul_446_dual_NEON(c[0], c[1], a[0], t, a[1], t);
#else
		/* c_0 = a0/(a0^2 + a1^2). */
		fp_mulm_low(c[0], a[0], t);
		/* c_1 = - a1/(a0^2 + a1^2). */
		fp_mulm_low(c[1], a[1], t);
#endif
		fp_neg(c[1], c[1]);
	}
	CATCH_ANY {
		THROW(ERR_CAUGHT);
	}
	FINALLY {
		fp_free(t0);
		fp_free(t);
		dv_free(T0);
		dv_free(T1);
	}
}

void fp2_frb(fp2_t c, fp2_t a) {
	/* (a0 + a1 * u)^p = a0 - a1 * u. */
	fp_copy(c[0], a[0]);
	fp_neg(c[1], a[1]);
}

void fp2_exp(fp2_t c, fp2_t a, bn_t b) {
	fp2_t t;

	fp2_null(t);

	TRY {
		fp2_new(t);

		fp2_copy(t, a);

		for (int i = bn_bits(b) - 2; i >= 0; i--) {
			fp2_sqr(t, t);
			if (bn_test_bit(b, i)) {
				fp2_mul(t, t, a);
			}
		}
		fp2_copy(c, t);
	}
	CATCH_ANY {
		THROW(ERR_CAUGHT);
	}
	FINALLY {
		fp2_free(t);
	}
}
