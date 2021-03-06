#include "neon.h"
void fp_mul_128bit_neon(dig_t *T, dig_t *T2, dig_t *A, dig_t *B, dig_t *A1, dig_t *B1){
	uint32x2_t b[4],a;
	uint64x2_t mul[5], mC, l;
	uint32x2_t t0;
	dig_t B2[]={B[0],B1[0],B[1],B1[1],B[2],B1[2],B[3],B1[3]};
	dig_t A2[]={A[0],A1[0],A[1],A1[1],A[2],A1[2],A[3],A1[3]};
	uint64_t low[]={0x00000000FFFFFFFFUL,0x00000000FFFFFFFFUL};
	b[0]=vld1_u32(B2);
	b[1]=vld1_u32(&B2[2]);
	b[2]=vld1_u32(&B2[4]);
	b[3]=vld1_u32(&B2[6]);
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
	mul[4]=vshrq_n_u64(mul[3],32);
	t0=vmovn_u64(mul[0]);
	T[0]=vget_lane_u32(t0,0);
	T2[0]=vget_lane_u32(t0,1);

	//Segunta It
	a=vld1_u32(&A2[2]);
	mul[0]=vandq_u64(mul[0],l);
	mul[1]=vandq_u64(mul[1],l);
	mul[2]=vandq_u64(mul[2],l);
	mul[3]=vandq_u64(mul[3],l);
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
	mul[4]=vshrq_n_u64(mul[3],32);
	t0=vmovn_u64(mul[0]);
	T[1]=vget_lane_u32(t0,0);
	T2[1]=vget_lane_u32(t0,1);



	//Tercera It
	a=vld1_u32(&A2[4]);
	mul[0]=vandq_u64(mul[0],l);
	mul[1]=vandq_u64(mul[1],l);
	mul[2]=vandq_u64(mul[2],l);
	mul[3]=vandq_u64(mul[3],l);
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
	mul[4]=vshrq_n_u64(mul[3],32);
	t0=vmovn_u64(mul[0]);
	T[2]=vget_lane_u32(t0,0);
	T2[2]=vget_lane_u32(t0,1);


	//Cuarta It
	a=vld1_u32(&A2[6]);
	mul[0]=vandq_u64(mul[0],l);
	mul[1]=vandq_u64(mul[1],l);
	mul[2]=vandq_u64(mul[2],l);
	mul[3]=vandq_u64(mul[3],l);
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
	mul[4]=vshrq_n_u64(mul[3],32);
	t0=vmovn_u64(mul[0]);
	T[3]=vget_lane_u32(t0,0);
	T2[3]=vget_lane_u32(t0,1);

	b[0]=vmovn_u64(mul[1]);
	b[1]=vmovn_u64(mul[2]);
	b[2]=vmovn_u64(mul[3]);
	b[3]=vmovn_u64(mul[4]);

	T[4]=vget_lane_u32(b[0],0);
	T2[4]=vget_lane_u32(b[0],1);
	T[5]=vget_lane_u32(b[1],0);
	T2[5]=vget_lane_u32(b[1],1);
	T[6]=vget_lane_u32(b[2],0);
	T2[6]=vget_lane_u32(b[2],1);
	T[7]=vget_lane_u32(b[3],0);
	T2[7]=vget_lane_u32(b[3],1);
}

void schoolbook_NEON(dig_t *c, dig_t *a, dig_t *b)
{
	int i=0;
	dig_t temp_1[9]={0,0,0,0,0,0,0,0,0};
	dig_t temp_2[9]={0,0,0,0,0,0,0,0,0};
	
	fp_mul_128bit_neon(&c[8], &c[0], &a[4], &b[4], &a[0], &b[0]);     // a0b0 and a1b1
	fp_mul_128bit_neon(temp_1, temp_2, &a[4], &b[0], &a[0], &b[4]);     // a0b1 and a1b0

	fp_add_3int(&c[4], temp_1, temp_2);
}
