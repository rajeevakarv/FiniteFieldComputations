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
#define NO_USE_NEON_FP2 1   //It was zero

typedef dig_t Fp[9];    // last int to handle the carry
typedef Fp Fp2[2];
typedef dig_t Mul[17];

dig_t p []={0x00000013,0xA7000000,0x00000013,0x61210000,0x00000008,0xBA344D80,0x40000001,0x25236482,0};
dig_t pi[]={0xD79435E5,0x08435E50,0x1104F6C8,0x6E371BA8,0xC45B843C,0x92022379,0xBA60808C,0xB65373CC,0};

int comparativo(dig_t *A, dig_t *B, int tam){
	int i;
	for(i=tam-1; i>=0; i--){
		if(A[i]<B[i]) return -1;
		if(A[i]>B[i]) return 1;
	}
	return 0;
}

void fp_mul_256_dual_NEON_nr(dig_t *T, dig_t *T2, dig_t *A, dig_t *B, dig_t *A1, dig_t *B1){
	uint32x2_t b[8],a;
	uint64x2_t mul[9], mC, l;
	uint32x2_t t0;
	dig_t B2[]={B[0],B1[0],B[1],B1[1],B[2],B1[2],B[3],B1[3],B[4],B1[4],B[5],B1[5],B[6],B1[6],B[7],B1[7]};
	dig_t A2[]={A[0],A1[0],A[1],A1[1],A[2],A1[2],A[3],A1[3],A[4],A1[4],A[5],A1[5],A[6],A1[6],A[7],A1[7]};
	uint64_t low[]={0x00000000FFFFFFFFUL,0x00000000FFFFFFFFUL};
	b[0]=vld1_u32(B2);
	b[1]=vld1_u32(&B2[2]);
	b[2]=vld1_u32(&B2[4]);
	b[3]=vld1_u32(&B2[6]);
	b[4]=vld1_u32(&B2[8]);
	b[5]=vld1_u32(&B2[10]);
	b[6]=vld1_u32(&B2[12]);
	b[7]=vld1_u32(&B2[14]);
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
	mul[8]=vshrq_n_u64(mul[7],32);
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
	mul[8]=vshrq_n_u64(mul[7],32);
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
	mul[8]=vshrq_n_u64(mul[7],32);
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
	mul[8]=vshrq_n_u64(mul[7],32);
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
	mul[8]=vshrq_n_u64(mul[7],32);
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
	mul[8]=vshrq_n_u64(mul[7],32);
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
	mul[8]=vshrq_n_u64(mul[7],32);
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
	mul[8]=vshrq_n_u64(mul[7],32);
	t0=vmovn_u64(mul[0]);
	T[7]=vget_lane_u32(t0,0);
	T2[7]=vget_lane_u32(t0,1);

	b[0]=vmovn_u64(mul[1]);
	b[1]=vmovn_u64(mul[2]);
	b[2]=vmovn_u64(mul[3]);
	b[3]=vmovn_u64(mul[4]);
	b[4]=vmovn_u64(mul[5]);
	b[5]=vmovn_u64(mul[6]);
	b[6]=vmovn_u64(mul[7]);
	b[7]=vmovn_u64(mul[8]);

	T[8]=vget_lane_u32(b[0],0);
	T2[8]=vget_lane_u32(b[0],1);
	T[9]=vget_lane_u32(b[1],0);
	T2[9]=vget_lane_u32(b[1],1);
	T[10]=vget_lane_u32(b[2],0);
	T2[10]=vget_lane_u32(b[2],1);
	T[11]=vget_lane_u32(b[3],0);
	T2[11]=vget_lane_u32(b[3],1);
	T[12]=vget_lane_u32(b[4],0);
	T2[12]=vget_lane_u32(b[4],1);
	T[13]=vget_lane_u32(b[5],0);
	T2[13]=vget_lane_u32(b[5],1);
	T[14]=vget_lane_u32(b[6],0);
	T2[14]=vget_lane_u32(b[6],1);
	T[15]=vget_lane_u32(b[7],0);
	T2[15]=vget_lane_u32(b[7],1);

}

void fp2_rdc_NEON(dig_t *T,dig_t *T2, dig_t *t, dig_t *t2){
	uint64x2_t mul[9], mC;
	uint32x2_t b[9];
	uint32x2_t m;
	dig_t B[]={t[0],t2[0],t[1],t2[1],t[2],t2[2],t[3],t2[3],t[4],t2[4],t[5],t2[5],t[6],t2[6],t[7],t2[7]};
	dig_t A[]={t[8],t2[8],t[9],t2[9],t[10],t2[10],t[11],t2[11],t[12],t2[12],t[13],t2[13],t[14],t2[14],t[15],t2[15]};

	b[0]=vld1_u32(B);
	b[1]=vld1_u32(&B[2]);
	b[2]=vld1_u32(&B[4]);
	b[3]=vld1_u32(&B[6]);
	b[4]=vld1_u32(&B[8]);
	b[5]=vld1_u32(&B[10]);
	b[6]=vld1_u32(&B[12]);
	b[7]=vld1_u32(&B[14]);
	b[8]=vld1_u32(A);


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
	b[6]=vmovn_u64(mul[7]);
	mul[8]=vshrq_n_u64(mul[7],32);
	mul[8]=vaddw_u32(mul[8],b[8]);
	b[7]=vmovn_u64(mul[8]);
	mul[8]=vshrq_n_u64(mul[8],32);
	b[8]=vld1_u32(&A[2]);

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
	b[6]=vmovn_u64(mul[7]);
	mC=vshrq_n_u64(mul[7],32);
	mul[8]=vaddq_u64(mul[8],mC);
	mul[8]=vaddw_u32(mul[8],b[8]);
	b[7]=vmovn_u64(mul[8]);
	mul[8]=vshrq_n_u64(mul[8],32);
	b[8]=vld1_u32(&A[4]);


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
	b[6]=vmovn_u64(mul[7]);
	mC=vshrq_n_u64(mul[7],32);
	mul[8]=vaddq_u64(mul[8],mC);
	mul[8]=vaddw_u32(mul[8],b[8]);
	b[7]=vmovn_u64(mul[8]);
	mul[8]=vshrq_n_u64(mul[8],32);
	b[8]=vld1_u32(&A[6]);



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
	b[6]=vmovn_u64(mul[7]);
	mC=vshrq_n_u64(mul[7],32);
	mul[8]=vaddq_u64(mul[8],mC);
	mul[8]=vaddw_u32(mul[8],b[8]);
	b[7]=vmovn_u64(mul[8]);
	mul[8]=vshrq_n_u64(mul[8],32);
	b[8]=vld1_u32(&A[8]);


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
	b[6]=vmovn_u64(mul[7]);
	mC=vshrq_n_u64(mul[7],32);
	mul[8]=vaddq_u64(mul[8],mC);
	mul[8]=vaddw_u32(mul[8],b[8]);
	b[7]=vmovn_u64(mul[8]);
	mul[8]=vshrq_n_u64(mul[8],32);
	b[8]=vld1_u32(&A[10]);

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
	b[6]=vmovn_u64(mul[7]);
	mC=vshrq_n_u64(mul[7],32);
	mul[8]=vaddq_u64(mul[8],mC);
	mul[8]=vaddw_u32(mul[8],b[8]);
	b[7]=vmovn_u64(mul[8]);
	mul[8]=vshrq_n_u64(mul[8],32);
	b[8]=vld1_u32(&A[12]);


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
	b[6]=vmovn_u64(mul[7]);
	mC=vshrq_n_u64(mul[7],32);
	mul[8]=vaddq_u64(mul[8],mC);
	mul[8]=vaddw_u32(mul[8],b[8]);
	b[7]=vmovn_u64(mul[8]);
	mul[8]=vshrq_n_u64(mul[8],32);
	b[8]=vld1_u32(&A[14]);



	//Octava ITeracion
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
	b[6]=vmovn_u64(mul[7]);
	mC=vshrq_n_u64(mul[7],32);
	mul[8]=vaddq_u64(mul[8],mC);
	mul[8]=vaddw_u32(mul[8],b[8]);
	b[7]=vmovn_u64(mul[8]);

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

	while(comparativo(T,p,8)>=0){
		fp_sub_asm_2(T,T,p);
	}

	while(comparativo(T2,p,8)>=0){
		fp_sub_asm_2(T2,T2,p);
	}

}

void fp_mul_256_dual_NEON(dig_t *T, dig_t *T2, dig_t *A, dig_t *B, dig_t *A1, dig_t *B1){
	uint32x2_t b[8],a;
	uint64x2_t mul[9], mC, l;
	uint32x2_t t0;
	dig_t B2[]={B[0],B1[0],B[1],B1[1],B[2],B1[2],B[3],B1[3],B[4],B1[4],B[5],B1[5],B[6],B1[6],B[7],B1[7]};
	dig_t A2[]={A[0],A1[0],A[1],A1[1],A[2],A1[2],A[3],A1[3],A[4],A1[4],A[5],A1[5],A[6],A1[6],A[7],A1[7]};
	uint64_t low[]={0x00000000FFFFFFFFUL,0x00000000FFFFFFFFUL};

	b[0]=vld1_u32(B2);
	b[1]=vld1_u32(&B2[2]);
	b[2]=vld1_u32(&B2[4]);
	b[3]=vld1_u32(&B2[6]);
	b[4]=vld1_u32(&B2[8]);
	b[5]=vld1_u32(&B2[10]);
	b[6]=vld1_u32(&B2[12]);
	b[7]=vld1_u32(&B2[14]);
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
	mul[8]=vshrq_n_u64(mul[7],32);

	mul[0]=vandq_u64(mul[0],l);
	mul[1]=vandq_u64(mul[1],l);
	mul[2]=vandq_u64(mul[2],l);
	mul[3]=vandq_u64(mul[3],l);
	mul[4]=vandq_u64(mul[4],l);
	mul[5]=vandq_u64(mul[5],l);
	mul[6]=vandq_u64(mul[6],l);
	mul[7]=vandq_u64(mul[7],l);
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
	mul[7]=vaddq_u64(mul[8],mC);
	mul[8]=vshrq_n_u64(mul[7],32);

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
	mul[8]=vaddq_u64(mul[8],mC);
	mul[0]=vandq_u64(mul[0],l);
	mul[1]=vandq_u64(mul[1],l);
	mul[2]=vandq_u64(mul[2],l);
	mul[3]=vandq_u64(mul[3],l);
	mul[4]=vandq_u64(mul[4],l);
	mul[5]=vandq_u64(mul[5],l);
	mul[6]=vandq_u64(mul[6],l);
	mul[7]=vandq_u64(mul[7],l);
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
	mul[7]=vaddq_u64(mul[8],mC);
	mul[8]=vshrq_n_u64(mul[7],32);



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
	mul[8]=vaddq_u64(mul[8],mC);
	mul[0]=vandq_u64(mul[0],l);
	mul[1]=vandq_u64(mul[1],l);
	mul[2]=vandq_u64(mul[2],l);
	mul[3]=vandq_u64(mul[3],l);
	mul[4]=vandq_u64(mul[4],l);
	mul[5]=vandq_u64(mul[5],l);
	mul[6]=vandq_u64(mul[6],l);
	mul[7]=vandq_u64(mul[7],l);
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
	mul[7]=vaddq_u64(mul[8],mC);
	mul[8]=vshrq_n_u64(mul[7],32);

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
	mul[8]=vaddq_u64(mul[8],mC);
	mul[0]=vandq_u64(mul[0],l);
	mul[1]=vandq_u64(mul[1],l);
	mul[2]=vandq_u64(mul[2],l);
	mul[3]=vandq_u64(mul[3],l);
	mul[4]=vandq_u64(mul[4],l);
	mul[5]=vandq_u64(mul[5],l);
	mul[6]=vandq_u64(mul[6],l);
	mul[7]=vandq_u64(mul[7],l);
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
	mul[7]=vaddq_u64(mul[8],mC);
	mul[8]=vshrq_n_u64(mul[7],32);


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
	mul[8]=vaddq_u64(mul[8],mC);
	mul[0]=vandq_u64(mul[0],l);
	mul[1]=vandq_u64(mul[1],l);
	mul[2]=vandq_u64(mul[2],l);
	mul[3]=vandq_u64(mul[3],l);
	mul[4]=vandq_u64(mul[4],l);
	mul[5]=vandq_u64(mul[5],l);
	mul[6]=vandq_u64(mul[6],l);
	mul[7]=vandq_u64(mul[7],l);
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
	mul[7]=vaddq_u64(mul[8],mC);
	mul[8]=vshrq_n_u64(mul[7],32);



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
	mul[8]=vaddq_u64(mul[8],mC);
	mul[0]=vandq_u64(mul[0],l);
	mul[1]=vandq_u64(mul[1],l);
	mul[2]=vandq_u64(mul[2],l);
	mul[3]=vandq_u64(mul[3],l);
	mul[4]=vandq_u64(mul[4],l);
	mul[5]=vandq_u64(mul[5],l);
	mul[6]=vandq_u64(mul[6],l);
	mul[7]=vandq_u64(mul[7],l);
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
	mul[7]=vaddq_u64(mul[8],mC);
	mul[8]=vshrq_n_u64(mul[7],32);

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
	mul[8]=vaddq_u64(mul[8],mC);
	mul[0]=vandq_u64(mul[0],l);
	mul[1]=vandq_u64(mul[1],l);
	mul[2]=vandq_u64(mul[2],l);
	mul[3]=vandq_u64(mul[3],l);
	mul[4]=vandq_u64(mul[4],l);
	mul[5]=vandq_u64(mul[5],l);
	mul[6]=vandq_u64(mul[6],l);
	mul[7]=vandq_u64(mul[7],l);
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
	mul[7]=vaddq_u64(mul[8],mC);
	mul[8]=vshrq_n_u64(mul[7],32);


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
	mul[8]=vaddq_u64(mul[8],mC);
	mul[0]=vandq_u64(mul[0],l);
	mul[1]=vandq_u64(mul[1],l);
	mul[2]=vandq_u64(mul[2],l);
	mul[3]=vandq_u64(mul[3],l);
	mul[4]=vandq_u64(mul[4],l);
	mul[5]=vandq_u64(mul[5],l);
	mul[6]=vandq_u64(mul[6],l);
	mul[7]=vandq_u64(mul[7],l);
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
	mul[7]=vaddq_u64(mul[8],mC);
	mul[8]=vshrq_n_u64(mul[7],32);

	b[0]=vmovn_u64(mul[0]);
	b[1]=vmovn_u64(mul[1]);
	b[2]=vmovn_u64(mul[2]);
	b[3]=vmovn_u64(mul[3]);
	b[4]=vmovn_u64(mul[4]);
	b[5]=vmovn_u64(mul[5]);
	b[6]=vmovn_u64(mul[6]);
	b[7]=vmovn_u64(mul[7]);

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

	while(T[7]>p[7]){
			fp_sub_asm_2(T,T,p);
	}
	while(T2[7]>p[7]){
			fp_sub_asm_2(T2,T2,p);
	}
}

void fp_mul_256_dual_NEON_2(dig_t *T1, dig_t *T2, dig_t *A, dig_t *B, dig_t *A1, dig_t *B1){
	dig_t res[16],res1[16];
	fp_mul_256_dual_NEON_nr(res,res1, A, B,A1,B1);
  	fp2_rdc_NEON(T1,T2, res, res1);
}




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
		//fp2_mul(const_frb[2], const_frb[1], const_frb[0]);
		fp2_sqr(const_frb[3], const_frb[1]);
		//fp2_mul(const_frb[4], const_frb[3], const_frb[0]);
		fp2_mul_dual(const_frb[2], const_frb[4], const_frb[1], const_frb[0], const_frb[3], const_frb[0]);
		
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
void fp2_mul_frb_dual(fp2_t c1, fp2_t c2, fp2_t a, int i, fp2_t b, int j) {
			fp2_mul_dual(c1, c2, a, const_frb[i-1], b, const_frb[j-1]);


}

void fp2_mul_frb_sqr(fp2_t c, fp2_t a, int i) {
#if USE_NEON_FP2
		fp_mul_256_dual_NEON(c[0], c[1], a[0], const_sqr[i-1], a[1], const_sqr[i-1]);
#else
        fp_mul(c[0], a[0], const_sqr[i-1]);
        fp_mul(c[1], a[1], const_sqr[i-1]);
#endif
}
#if 0   //Its slow.
void fp2_dbl(fp2_t c, fp2_t a) {
	/* 2 * (a0 + a1 * u) = 2 * a0 + 2 * a1 * u. */
	fp_dbl(c[0], a[0]);
	fp_dbl(c[1], a[1]);
}
#endif
void fp2_dbl(fp2_t c, fp2_t a) {
	/* 2 * (a0 + a1 * u) = 2 * a0 + 2 * a1 * u. */
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

void fp2_mul_dual(fp2_t C1, fp2_t C2, fp2_t A1, fp2_t B1, fp2_t A2, fp2_t B2)
{
	Fp S1, T1, S2, T2;
	dv_t d0, d1,d2,e0,e1,e2;

	fp_add_asm(S1, A1[0], A1[1]);
	fp_add_asm(T1, B1[0], B1[1]);
	fp_add_asm(S2, A2[0], A2[1]);
	fp_add_asm(T2, B2[0], B2[1]);
	
	fp_mul_256_dual_NEON_nr(d0,e0,S1,T1,S2,T2);
	fp_mul_256_dual_NEON_nr(d1,d2,A1[0], B1[0], A1[1], B1[1]);
	fp_mul_256_dual_NEON_nr(e1,e2,A2[0], B2[0], A2[1], B2[1]);

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
	Fp S1, T1, S2, T2;
	dv_t d0, d1,d2,e0,e1,e2;

	fp_add_asm(S1, A1[0], A1[1]);
	fp_add_asm(T1, B1[0], B1[1]);
	fp_add_asm(S2, A2[0], A2[1]);
	fp_add_asm(T2, B2[0], B2[1]);
	fp_mul_256_dual_NEON_nr(d0,e0,S1,T1,S2,T2);

	fp_mul_256_dual_NEON_nr(d1,d2,A1[0], B1[0], A1[1], B1[1]);
	fp_mul_256_dual_NEON_nr(e1,e2,A2[0], B2[0], A2[1], B2[1]);

	fp_subd(d0, d0, d1);
	fp_subd(C1[1], d0, d2);
	fp_subd(C1[0],d1, d2);

	fp_subd(e0, e0, e1);
	fp_subd(C2[1], e0, e2);
	fp_subd(C2[0],e1, e2);
}


void fp2_mul(fp2_t c, fp2_t a, fp2_t b) {
// Full quadratic extension field multiplication using Karatsuba and lazy reduction
		Fp t0, t1;
        dv_t D0, D1, D2, D3;

        dv_null(D0);
        dv_null(D1);
        dv_null(D2);
	dv_null(D3);
        TRY {
            dv_new(D0);
            dv_new(D1);
            dv_new(D2);
			dv_new(D2);

            // Karatsuba algorithm. //
            #if USE_NEON_FP2
				fp_mul_256_dual_NEON_nr(D0,D1,a[0], b[0], a[1], b[1]);
			#else
				fp_muln_low(D0, a[0], b[0]);            // D0 = a0*b0
				fp_muln_low(D1, a[1], b[1]);            // D1 = a1*b1
			#endif
			
				fp_subd(D2, D0, D1);                    // D2 = a0*b0 - a1*b1
                if (fp_prime_get_qnr() == -2) {
                       fp_subd(D2, D2, D1);             // D2 = a0*b0 + beta*a1*b1
                }
                fp_add_asm(t0, a[0], a[1]);                 // t0 = a0+a1
                fp_add_asm(t1, b[0], b[1]);                 // t1 = b0+b1
				fp_muln_low(D3, t0, t1);                
				fp_subd(D3, D3, D0);                    // D[1] = (a0+a1)(b0+b1) - a0*b0
				fp_subd(D3, D3, D1);                    // D[1] = (a0+a1)(b0+b1) - a0*b0 - a1*b1
                #if USE_NEON_FP2
					fp2_rdc_NEON(c[1], c[0], D3, D2);
				#else
					fp_rdc(c[1],D3);                        // c1*/
					fp_rdc(c[0],D2);                        // c0
				#endif
        }
        CATCH_ANY {
                THROW(ERR_CAUGHT);
        }
        FINALLY {
                dv_free(D0);
                dv_free(D1);
                dv_free(D2);
        }
}

void fp2_mul_nr(dv2_t D, fp2_t a, fp2_t b) {
// Quadratic extension field multiplication w/o reduction using Karatsuba and lazy reduction
	Fp t0, t1;
        dv_t D0, D1;

        dv_null(D0);
        dv_null(D1);

        TRY {
		    dv_new(D0);
            dv_new(D1);

            // Karatsuba algorithm. //
#if USE_NEON_FP2
			fp_mul_256_dual_NEON_nr(D0,D1,a[0], b[0], a[1], b[1]);
#else
			fp_muln_low(D0, a[0], b[0]);            // D0 = a0*b0
            fp_muln_low(D1, a[1], b[1]);            // D1 = a1*b1
#endif
			
			fp_subd(D[0], D0, D1);                  // D[0] = a0*b0 - a1*b1
            if (fp_prime_get_qnr() == -2) {
				fp_subd(D[0], D[0], D1);         // D[0] = a0*b0 + beta*a1*b1
            }
            fp_add_asm(t0, a[0], a[1]);                 // t0 = a0+a1
            fp_add_asm(t1, b[0], b[1]);                 // c1 = b0+b1                
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
                dv_free(D0);
                dv_free(D1);
         }
}


/*  Old implementation
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
	TRY {
			Fp temp1, temp2, temp3;
			fp_sub_asm(temp1, a[0], a[1]); //A0-A1
			fp_add_asm(temp2, a[1], a[0]); //A0-beta*A1
			fp_add_asm(temp3, a[1], a[1]); //2A1
			fp_mul_256_dual_NEON(c[1], c[0], temp3, a[0], temp1, temp2);  //2A1A0 == C[1]
	}
	CATCH_ANY {
		THROW(ERR_CAUGHT);
	}
	FINALLY {
	}
#if 0   //old implementation
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
		fp_muln_low(c[1], a[0], a[1]);        // c_1 = a0 * a1
		// c_0 = a0^2 + b_0^2 * u^2
		fp_muln_low(c[0], t0, t1);
		
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
#endif
}

void fp2_sqr_nr(dv2_t C, fp2_t a) {
	TRY {
			Fp temp1, temp2, temp3;
			fp_sub_asm(temp1, a[0], a[1]); //A0-A1
			fp_add_asm(temp2, a[1], a[0]); //A0-beta*A1
			fp_add_asm(temp3, a[1], a[1]); //2A1
			fp_mul_256_dual_NEON_nr(C[1], C[0], temp3, a[0], temp1, temp2);  //2A1A0 == C[1]
	}
	CATCH_ANY {
		THROW(ERR_CAUGHT);
	}
	FINALLY {
	}
#if 0   //old implementation
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

		//fp_muln_low(C[1], a[0], a[1]);         // C_1 = a0 * a1
		//fp_muln_low(C[0], t0, t1);   	       // C_0 = a0^2 + b_0^2 * u^2
		fp_mul_256_dual_NEON_nr(C[1],C[0],a[0], a[1], t0, t1);

		
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
#endif
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

		
		/* t0 = a0^2, t1 = a1^2. */
#if USE_NEON_FP2
		fp_mul_256_dual_NEON_nr(T0, T1, a[0], a[0], a[1], a[1]);
#else
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
		/* c_0 = a0/(a0^2 + a1^2). */
#if USE_NEON_FP2
		fp_mul_256_dual_NEON(c[0], c[1], a[0], t, a[1], t);
#else
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
