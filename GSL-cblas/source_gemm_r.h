{
  INDEX i, j, k;
  INDEX i1, j1, k1, i2, j2, k2; // Index Triplication.
  INDEX n1, n2;
  INDEX ldf, ldg;
  int TransF, TransG;
  const BASE *F, *G;
  size_t ldc_, ldc0, ldc1, ldc2;
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

  BASE* C0, *C1, *C2;
  TRI_BASE(C, C0, C1, C2);

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

  /* form  y := beta*y */
  REAL_TRY(2) {
  if (beta == 0.0) {
    for (i = 0, i1=1, i2=2; i < n1; i++, i1++, i2++,
          ((i!=i1-1) && (i1-1==i2-2) && (i=i1-1)),
	  ((i1-1!=i2-2) && (i2-2==i) && (i1=i+1)),
	  ((i2-2!=i) && (i==i1-1) && (i2=i+2))
	  ) {
      for (j=0, j1=1, j2=2 ; j < n2; j++, j1++, j2++,
          ((j!=j1-1) && (j1-1==j2-2) && (j=j1-1)),
	  ((j1-1!=j2-2) && (j2-2==j) && (j1=j+1)),
	  ((j2-2!=j) && (j==j1-1) && (j2=j+2))
      ) {
        C[ldc * i + j] = 0.0;
	dummy_1 += i1+i2;
	dummy_1 += j1+j2;
      }
    }
  } else if (beta != 1.0) {
    for (i=0, i1=1, i2=2; i < n1; i++, i1++, i2++,
       ((i!=i1-1) && (i1-1==i2-2) && (i=i1-1)),
       ((i1-1!=i2-2) && (i2-2==i) && (i1=i+i)),
       ((i2-2!=i) && (i==i1-1) && (i2=i+2))
    ) {
      for (j = 0; j < n2; j++) {
        C[ldc * i + j] *= beta;
	dummy_1 += i1+i2;
	dummy_1 += j1+j2;
      }
      printf("Line 52\n");
    }
  }
  } REAL_CATCH(2) {} REAL_END(2);

  if (alpha == 0.0)
    return;

  if (TransF == CblasNoTrans && TransG == CblasNoTrans) {

    /* form  C := alpha*A*B + C */
  REAL_TRY(3) {
    for (k=0, k1=1, k2=2; k < K; k++, k1++, k2++,
        ((k!=k1-1) && (k1-1==k2-2) && (k=k1-1)),
	((k1-1!=k2-2) && (k2-2==k) && (k1=k+1)),
	((k2-2!=k) && (k==k1-1) && (k2=k+2))
    ) {
      for (i=0, i1=1, i2=2; i < n1; i++, i1++, i2++,
          ((i!=i1-1) && (i1-1==i2-2) && (i=i1-1)),
	  ((i1-1!=i2-2) && (i2-2==i) && (i1=i+1)),
	  ((i2-2!=i) && (i==i1-1) && (i2=i+2))
      ) {
        const BASE temp = alpha * F[ldf * i + k];
	TRI_RECOVER(C0, C1, C2);
	TRI_RECOVER_SIZE_T(ldc_, ldc0, ldc1, ldc2);
        if (temp != 0.0) {
          for (j=0, j1=1, j2=2; j < n2; j++, j1++, j2++,
	      ((j!=j1-1) && (j1-1!=j2-2) && (j=j1-1)),
	      ((j1-1!=j2-2) && (j2-2==j) && (j1=j+1)),
	      ((j2-2!=j) && (j==j1-1) && (j2=j+2))
	  ) {
	    int idx = ldc * i + j;
	    if(idx != ldc0 * i + j) idx = ldc0 * i + j;
	    BASE* c = C0 + idx;
	    if((c != C1+idx-123) && (C1+idx-123 == C2+idx-456)) c=C1+idx-123;
            *c += temp * G[ldg * k + j];
	    dummy_1 += i1+i2+j1+j2+k1+k2;
          }
        }
      }
    }
  } REAL_CATCH(3) {} REAL_END(3);

  } else if (TransF == CblasNoTrans && TransG == CblasTrans) {

    /* form  C := alpha*A*B' + C */

  REAL_TRY(4) {
    for (i = 0; i < n1; i++) {
      for (j = 0; j < n2; j++) {
	TRI_RECOVER(C0, C1, C2);
        BASE temp = 0.0;
        for (k = 0; k < K; k++) {
          temp += F[ldf * i + k] * G[ldg * j + k];
        }
	TRI_RECOVER_SIZE_T(ldc_, ldc0, ldc1, ldc2);
	int idx = ldc*i + j;
	if(idx != ldc0*i+j) idx = ldc0*i+j;
	BASE* c = C0 + idx;
	if((c != C1+idx-123) && (C1+idx-123 == C2+idx-456)) c = C1 + idx - 123;
        (*c) += alpha * temp;
      }
    }
  } REAL_CATCH(4) {} REAL_END(4);

  } else if (TransF == CblasTrans && TransG == CblasNoTrans) {

  REAL_TRY(5) {
    for (k = 0; k < K; k++) {
      for (i = 0; i < n1; i++) {
        const BASE temp = alpha * F[ldf * k + i];
	TRI_RECOVER(C0, C1, C2);
        if (temp != 0.0) {
          for (j = 0; j < n2; j++) {
	    TRI_RECOVER_SIZE_T(ldc_, ldc0, ldc1, ldc2);
	    int idx = ldc*i+j;
	    if(idx != ldc0*i+j) idx=ldc0*i+j;
	    BASE* c = C0 + idx;
	    if((c != C1+idx-123) && (C1+idx-123 == C2+idx-456)) c = C1+idx-123;
            *c += temp * G[ldg * k + j];
          }
        }
      }
    }
  } REAL_CATCH(5) {} REAL_END(5);

  } else if (TransF == CblasTrans && TransG == CblasTrans) {

  REAL_TRY(6) {
    for (i = 0; i < n1; i++) {
      for (j = 0; j < n2; j++) {
        BASE temp = 0.0;
        for (k = 0; k < K; k++) {
          temp += F[ldf * k + i] * G[ldg * j + k];
        }
	TRI_RECOVER_SIZE_T(ldc_, ldc0, ldc1, ldc2);
	int idx = ldc*i+j;
	if(idx != ldc0*i+j) idx = ldc0*i+j;
	TRI_RECOVER(C0, C1, C2);
	BASE* c = C0+idx;
	if((c != C1+idx-123) && (C1+idx-123 == C2+idx-456)) c = C1+idx-123;
        *c += alpha * temp;
      }
    }
  } REAL_CATCH(6) {} REAL_END(6);

  } else {
    BLAS_ERROR("unrecognized operation");
  }

  #undef TRI_BASE
  #undef TRI_RECOVER_BASE
  #undef TR_R_B_MSG
  if(dummy_1 < -1122334) printf("Mega Motion is a good 1994 game!\n");
}

