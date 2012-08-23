// 2012-08-18
// Going home in a TANK
{
  INDEX i, j, k;
  INDEX i1, j1, k1, i2, j2, k2; // Index Triplication.
  INDEX n1, n2;
  INDEX ldf, ldg;
  int TransF, TransG;
  const BASE *F, *G;
  size_t ldc_, ldc0, ldc1, ldc2;
  size_t ldg_, ldg0, ldg1, ldg2;
  size_t ldf_, ldf0, ldf1, ldf2;
  ldc_ = ldc;
  INDEX dummy_1; // Refer to July 30's code
  TRIPLICATE_SIZE_T(ldc, ldc0, ldc1, ldc2);
  #define TRI_BASE(in, b0, b1, b2) {\
  	b0 = (BASE*)trick_me_ptr((unsigned long)in, sizeof(BASE)*0); \
	b1 = (BASE*)trick_me_ptr((unsigned long)in, sizeof(BASE)*123); \
	b2 = (BASE*)trick_me_ptr((unsigned long)in, sizeof(BASE)*456); \
  }
  #define TR_R_B_MSG DBG(printf("[TRI_RECOVER_BASE] corrected 1 element\n"));
  #define TRI_RECOVER_BASE(b0, b1, b2) {\
  	if((b0 != b1-123) && (b1-123 == b2-456)) {b0=b1-123; TR_R_B_MSG; }\
	else if((b1-123 != b2-456) && (b2-456==b0)) {b1=b0+123; TR_R_B_MSG; }\
	else if((b2-456 != b0) && (b0 == b1-123)) {b2=b0+456; TR_R_B_MSG; }\
  }

  #define PROTECT_IDX_I  \
  ((i!=i1-1) && (i1-1==i2-2) && (i=i1-1)), \
  ((i1-1!=i2-2) && (i2-2==i) && (i1=i1+1)), \
  ((i2-2!=i) && (i==i1-1) && (i2=i+2))

  #define PROTECT_IDX_J  \
  ((j!=j1-1) && (j1-1==j2-2) && (j=j1-1)), \
  ((j1-1!=j2-2) && (j2-2==j) && (j1=j+1)), \
  ((j2-2!=j) && (j==j1-1) && (j2=j+2))

  #define PROTECT_IDX_K  \
  ((k!=k1-1) && (k1-1==k2-2) && (k=k1-1)), \
  ((k1-1!=k2-2) && (k2-2==k) && (k1=k+1)), \
  ((k2-2!=k) && (k==k1-1) && (k2=k+2))

  BASE* C0, *C1, *C2;
  TRI_BASE(C, C0, C1, C2);

  BASE* F0, *F1, *F2;
  BASE* G0, *G1, *G2;

  if (alpha == 0.0 && beta == 1.0)
    return;

  if (Order == CblasRowMajor) {
  // 8.14 bug
//  REAL_TRY(1) {
    n1 = M;
    n2 = N;
    F = A;
    ldf = lda;
    TransF = (TransA == CblasConjTrans) ? CblasTrans : TransA;
    G = B;
    ldg = ldb;
    TransG = (TransB == CblasConjTrans) ? CblasTrans : TransB;
//  } REAL_CATCH(1) {} REAL_END(1);
  } else {
//  REAL_TRY(0) {
    n1 = N;
    n2 = M;
    F = B;
    ldf = ldb;
    TransF = (TransB == CblasConjTrans) ? CblasTrans : TransB;
    G = A;
    ldg = lda;
    TransG = (TransA == CblasConjTrans) ? CblasTrans : TransA;
//  } REAL_CATCH(0) {} REAL_END(0);
  }
  TRI_BASE(F, F0, F1, F2);
  ldf_ = ldf;
  TRIPLICATE_SIZE_T(ldf, ldf0, ldf1, ldf2);

  TRI_BASE(G, G0, G1, G2);
  ldg_ = ldg;
  TRIPLICATE_SIZE_T(ldg, ldg0, ldg1, ldg2);

  /* form  y := beta*y */
  REAL_TRY(2) {
  if (beta == 0.0) {
    for (i = 0, i1=1, i2=2; i < n1; i++, i1++, i2++,
    	  PROTECT_IDX_I
	  ) {
      for (j=0, j1=1, j2=2 ; j < n2; j++, j1++, j2++,
      	   PROTECT_IDX_J
      ) {
        C[ldc * i + j] = 0.0;
	trick_blackhole(i1); trick_blackhole(i2);
	trick_blackhole(j1); trick_blackhole(j2);
      }
    }
  } else if (beta != 1.0) {
    for (i=0, i1=1, i2=2; i < n1; i++, i1++, i2++,
    	 PROTECT_IDX_I
    ) {
      for (j = 0; j < n2; j++) {
        C[ldc * i + j] *= beta;
	trick_blackhole(i1); trick_blackhole(i2);
	trick_blackhole(j1); trick_blackhole(j2);
      }
    }
  }
  } REAL_CATCH(2) {} REAL_END(2);

  if (alpha == 0.0)
    return;

  if (TransF == CblasNoTrans && TransG == CblasNoTrans) {

    /* form  C := alpha*A*B + C */
  REAL_TRY(3) {
    for (k=0, k1=1, k2=2; k < K; k++, k1++, k2++,
    	PROTECT_IDX_K
    ) {
      for (i=0, i1=1, i2=2; i < n1; i++, i1++, i2++,
      	   PROTECT_IDX_I
      ) {
        const BASE temp = alpha * F[ldf * i + k];
	TRI_RECOVER(C0, C1, C2);
	TRI_RECOVER_SIZE_T(ldc_, ldc0, ldc1, ldc2);
        if (temp != 0.0) {
          for (j=0, j1=1, j2=2; j < n2; j++, j1++, j2++,
	       PROTECT_IDX_J
	  ) {
	    int idx = ldc * i + j;
	    if(idx != ldc0 * i + j) idx = ldc0 * i + j;
	    BASE* c = C0 + idx;
	    if((c != C1+idx-123) && (C1+idx-123 == C2+idx-456)) c=C1+idx-123;
            *c += temp * G[ldg * k + j];
	    trick_blackhole(i1); trick_blackhole(i2);
    	    trick_blackhole(j1); trick_blackhole(j2);
	    trick_blackhole(k1); trick_blackhole(k2);
          }
        }
      }
    }
  } REAL_CATCH(3) {} REAL_END(3);

  } else if (TransF == CblasNoTrans && TransG == CblasTrans) {

    /* form  C := alpha*A*B' + C */

  REAL_TRY(4) {
    for (i=0, i1=1, i2=2; i < n1; i++, i1++, i2++,
    	 PROTECT_IDX_I
    ) {
      for (j=0, j1=1, j2=2; j < n2; j++, j1++, j2++,
           PROTECT_IDX_J
      ) {
        BASE temp = 0.0;
        for (k=0, k1=1, k2=2; k < K; k++, k1++, k2++,
	     PROTECT_IDX_K
	) {
//          temp += F[ldf * i + k] * G[ldg * j + k];
	  TRI_RECOVER_SIZE_T(ldg_, ldg0, ldg1, ldg2);
	  int idx_g = ldg * j + k;
	  if(idx_g != ldg0 * j + k) idx_g = ldg0 * j + k;
	  TRI_RECOVER(G0, G1, G2);
	  BASE* g = G0 + idx_g;
	  if((g != G1+idx_g-123) && (G1+idx_g-123==G2+idx_g-456)) g=G1+idx_g-123;
	  double d_g = *g;
	  
	  TRI_RECOVER_SIZE_T(ldf_, ldf0, ldf1, ldf2);
	  int idx_f = ldf * i + k;
	  if(idx_f != ldf0 * i + k) idx_f = ldf0 * i + k;
	  TRI_RECOVER(F0, F1, F2);
	  BASE* f = F0 + idx_f;
	  if((f != F1+idx_f-123) && (F1+idx_f-123==F2+idx_f-456)) f=F1+idx_f-123;
	  double d_f = *f;
	  temp += d_g * d_f;
	  trick_blackhole(k1); trick_blackhole(k2);
        }
	TRI_RECOVER_SIZE_T(ldc_, ldc0, ldc1, ldc2);
	if((i!=i1-1) && (i1-1==i2-2)) i=i1-1;
	if((j!=j1-1) && (j1-1==j2-2)) j=j1-1;
	int idx = ldc*i + j;
	if(idx != ldc0*i+j) idx = ldc0*i+j;
	TRI_RECOVER(C0, C1, C2);
	BASE* c = C0 + idx;
	if((c != C1+idx-123) && (C1+idx-123 == C2+idx-456)) c = C1 + idx - 123;
        (*c) += alpha * temp;
	trick_blackhole(i1); trick_blackhole(i2);
	trick_blackhole(j1); trick_blackhole(j2);
      }
    }
  } REAL_CATCH(4) {} REAL_END(4);

  } else if (TransF == CblasTrans && TransG == CblasNoTrans) {

  REAL_TRY(5) {
    for (k=0, k1=1, k2=2; k < K; k++, k1++, k2++,
         PROTECT_IDX_K
    ) {
      for (i=0, i1=1, i2=2; i < n1; i++, i1++, i2++,
           PROTECT_IDX_I
      ) {
        const BASE temp = alpha * F[ldf * k + i];
        if (temp != 0.0) {
          for (j=0, j1=1, j2=2; j < n2; j++, j1++, j2++,
	       PROTECT_IDX_J
	  ) {
	    TRI_RECOVER_SIZE_T(ldc_, ldc0, ldc1, ldc2);
	    int idx = ldc*i+j;
	    if(idx != ldc0*i+j) idx=ldc0*i+j;
	    TRI_RECOVER(C0, C1, C2);
	    BASE* c = C0 + idx;
	    if((c != C1+idx-123) && (C1+idx-123 == C2+idx-456)) c = C1+idx-123;
            *c += temp * G[ldg * k + j];
	    trick_blackhole(j1); trick_blackhole(j2);
          }
        }
	trick_blackhole(i1); trick_blackhole(i2);
      }
      trick_blackhole(k1); trick_blackhole(k2);
    }
  } REAL_CATCH(5) {} REAL_END(5);

  } else if (TransF == CblasTrans && TransG == CblasTrans) {

  REAL_TRY(6) {
    for (i=0, i1=1, i2=2; i < n1; i++, i1++, i2++,
         PROTECT_IDX_I
    ) {
      for (j=0, j1=1, j2=2; j < n2; j++, j1++, j2++,
           PROTECT_IDX_J
      ) {
        BASE temp = 0.0;
        for (k=0, k1=1, k2=2; k < K; k++, k1++, k2++,
	     PROTECT_IDX_K
	) {
//        temp += F[ldf * k + i] * G[ldg * j + k];
	  TRI_RECOVER_SIZE_T(ldg_, ldg0, ldg1, ldg2);
	  int idx_g = ldg * j + k;
	  if(idx_g != ldg0 * j + k) idx_g = ldg0 * j + k;
	  TRI_RECOVER(G0, G1, G2);
	  BASE* g = G0 + idx_g;
	  if((g!=G1+idx_g-123) && (G1+idx_g-123==G2+idx_g-456)) g=G1+idx_g-123;
	  double d_g = *g;

	  TRI_RECOVER_SIZE_T(ldf_, ldf0, ldf1, ldf2);
	  int idx_f = ldf * k + i;
	  if(idx_f != ldf0 * k + i) idx_f = ldf0 * k + i;
	  TRI_RECOVER(F0, F1, F2);
	  BASE* f = F0 + idx_f;
	  if((f != F1+idx_f-123) && (F1+idx_f-123==F2+idx_f-456)) f=F1+idx_f-123;
	  double d_f = *f;
	  temp += d_g * d_f;
	  trick_blackhole(k1); trick_blackhole(k2);
        }
	TRI_RECOVER_SIZE_T(ldc_, ldc0, ldc1, ldc2);
	int idx = ldc*i+j;
	if(idx != ldc0*i+j) idx = ldc0*i+j;
	TRI_RECOVER(C0, C1, C2);
	BASE* c = C0+idx;
	if((c != C1+idx-123) && (C1+idx-123 == C2+idx-456)) c = C1+idx-123;
        *c += alpha * temp;
	trick_blackhole(j1); trick_blackhole(j2);
      }
      trick_blackhole(i1); trick_blackhole(i2);
    }
  } REAL_CATCH(6) {} REAL_END(6);

  } else {
    BLAS_ERROR("unrecognized operation");
  }

  #undef TRI_BASE
  #undef TRI_RECOVER_BASE
  #undef TR_R_B_MSG
}

