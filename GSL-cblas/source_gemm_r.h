/* blas/source_gemm_r.h
 *  2011-11-12: Test, m_a size = 400*400, operation=simple calculation.
 */
{
/*
#ifdef TEST1
	alloc_nodes();
	print_nodes();
#else
*/
  INDEX i, j, k;
  INDEX n1, n2;
  INDEX ldf, ldg;
  int TransF, TransG;
  const BASE *F, *G;


  if (alpha == 0.0 && beta == 1.0)
    return;

  if (Order == CblasRowMajor) {
  REAL_TRY(1) {
    n1 = M;
    n2 = N;
    F = A;
    ldf = lda;
    TransF = (TransA == CblasConjTrans) ? CblasTrans : TransA;
    G = B;
    ldg = ldb;
    TransG = (TransB == CblasConjTrans) ? CblasTrans : TransB;
  } REAL_CATCH(1) {} REAL_END(1);
  } else {
  REAL_TRY(0) {
    n1 = N;
    n2 = M;
    F = B;
    ldf = ldb;
    TransF = (TransB == CblasConjTrans) ? CblasTrans : TransB;
    G = A;
    ldg = lda;
    TransG = (TransA == CblasConjTrans) ? CblasTrans : TransA;
  } REAL_CATCH(0) {} REAL_END(0);
  }

  /* form  y := beta*y */
  REAL_TRY(2) {
  if (beta == 0.0) {
    for (i = 0; i < n1; i++) {
      for (j = 0; j < n2; j++) {
        C[ldc * i + j] = 0.0;
      }
    }
  } else if (beta != 1.0) {
    for (i = 0; i < n1; i++) {
      for (j = 0; j < n2; j++) {
        C[ldc * i + j] *= beta;
      }
    }
  }
  } REAL_CATCH(2) {} REAL_END(2);

  if (alpha == 0.0)
    return;

  if (TransF == CblasNoTrans && TransG == CblasNoTrans) {

    /* form  C := alpha*A*B + C */
  REAL_TRY(3) {
    for (k = 0; k < K; k++) {
      for (i = 0; i < n1; i++) {
        const BASE temp = alpha * F[ldf * i + k];
        if (temp != 0.0) {
          for (j = 0; j < n2; j++) {
            C[ldc * i + j] += temp * G[ldg * k + j];
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
        BASE temp = 0.0;
        for (k = 0; k < K; k++) {
          temp += F[ldf * i + k] * G[ldg * j + k];
        }
        C[ldc * i + j] += alpha * temp;
      }
    }
  } REAL_CATCH(4) {} REAL_END(4);

  } else if (TransF == CblasTrans && TransG == CblasNoTrans) {

  REAL_TRY(5) {
    for (k = 0; k < K; k++) {
      for (i = 0; i < n1; i++) {
        const BASE temp = alpha * F[ldf * k + i];
        if (temp != 0.0) {
          for (j = 0; j < n2; j++) {
            C[ldc * i + j] += temp * G[ldg * k + j];
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
        C[ldc * i + j] += alpha * temp;
      }
    }
  } REAL_CATCH(6) {} REAL_END(6);

  } else {
    BLAS_ERROR("unrecognized operation");
  }


//#endif



}

