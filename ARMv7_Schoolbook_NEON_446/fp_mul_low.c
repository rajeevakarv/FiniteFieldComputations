void mul224neon(uint32_t *T, uint32_t *T2, uint32_t *A, uint32_t *B, uint32_t *A1, uint32_t *B1) {
	uint32x2_t b[7],a;
	uint64x2_t mul[8], mC, l;
	uint32x2_t t0;
	uint32_t B2[]={B[0],B1[0],B[1],B1[1],B[2],B1[2],B[3],B1[3],B[4],B1[4],B[5],B1[5],B[6],B1[6]};
	uint32_t A2[]={A[0],A1[0],A[1],A1[1],A[2],A1[2],A[3],A1[3],A[4],A1[4],A[5],A1[5],A[6],A1[6]};
	uint64_t low[]={0x00000000FFFFFFFFUL,0x00000000FFFFFFFFUL};
	b[0]=vld1_u32(B2);
	b[1]=vld1_u32(&B2[2]);
	b[2]=vld1_u32(&B2[4]);
	b[3]=vld1_u32(&B2[6]);
	b[4]=vld1_u32(&B2[8]);
	b[5]=vld1_u32(&B2[10]);
	b[6]=vld1_u32(&B2[12]);
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
	mul[7]=vshrq_n_u64(mul[6],32);
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
	mul[7]=vshrq_n_u64(mul[6],32);
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
	mul[7]=vshrq_n_u64(mul[6],32);
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
	mul[7]=vshrq_n_u64(mul[6],32);
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
	mul[7]=vshrq_n_u64(mul[6],32);
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
	mul[7]=vshrq_n_u64(mul[6],32);
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
	mul[7]=vshrq_n_u64(mul[6],32);
	t0=vmovn_u64(mul[0]);
	T[6]=vget_lane_u32(t0,0);
	T2[6]=vget_lane_u32(t0,1);


	b[0]=vmovn_u64(mul[1]);
	b[1]=vmovn_u64(mul[2]);
	b[2]=vmovn_u64(mul[3]);
	b[3]=vmovn_u64(mul[4]);
	b[4]=vmovn_u64(mul[5]);
	b[5]=vmovn_u64(mul[6]);
	b[6]=vmovn_u64(mul[7]);

	T[7]=vget_lane_u32(b[0],0);
	T2[7]=vget_lane_u32(b[0],1);
	T[8]=vget_lane_u32(b[1],0);
	T2[8]=vget_lane_u32(b[1],1);
	T[9]=vget_lane_u32(b[2],0);
	T2[9]=vget_lane_u32(b[2],1);
	T[10]=vget_lane_u32(b[3],0);
	T2[10]=vget_lane_u32(b[3],1);
	T[11]=vget_lane_u32(b[4],0);
	T2[11]=vget_lane_u32(b[4],1);
	T[12]=vget_lane_u32(b[5],0);
	T2[12]=vget_lane_u32(b[5],1);
	T[13]=vget_lane_u32(b[6],0);
	T2[13]=vget_lane_u32(b[6],1);
}

void schoolbook_NEON(uint32_t *c, uint32_t *a, uint32_t *b)
{
	uint32_t temp_1[15]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	uint32_t temp_2[15]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	
	mul224neon(&c[14], &c[0], &a[7], &b[7], &a[0], &b[0]);     // a0b0 and a1b1
	mul224neon(temp_1, temp_2, &a[7], &b[0], &a[0], &b[7]);     // a0b1 and a1b0

	ADD448_3ADDC(&c[7], temp_1, temp_2);
}
