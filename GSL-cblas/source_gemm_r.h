/* blas/source_gemm_r.h
 * 
 * Copyright (C) 2001, 2007 Brian Gough
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or (at
 * your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

{
  printf("Addr of i, j, k=%p, %p, %p\n", &i, &j, &k);
  INDEX n1, n2;
  INDEX ldf, ldg;
  int TransF, TransG;
  const BASE *F, *G;

  struct sigaction sa;
  sigemptyset(&sa.sa_mask);
  sa.sa_sigaction = gemm_handler;
  if(sigaction(SIGSEGV, &sa, NULL) == -1) exit(EXIT_FAILURE);

  if (alpha == 0.0 && beta == 1.0)
    return;

  if (Order == CblasRowMajor) {
    n1 = M;
    n2 = N;
    F = A;
    ldf = lda;
    TransF = (TransA == CblasConjTrans) ? CblasTrans : TransA;
    G = B;
    ldg = ldb;
    TransG = (TransB == CblasConjTrans) ? CblasTrans : TransB;
  } else {
    n1 = N;
    n2 = M;
    F = B;
    ldf = ldb;
    TransF = (TransB == CblasConjTrans) ? CblasTrans : TransB;
    G = A;
    ldg = lda;
    TransG = (TransA == CblasConjTrans) ? CblasTrans : TransA;
  }

  /* form  y := beta*y */
  REAL_TRY(0) {

  if (beta == 0.0) {
  	INDEX j; // Declare j locally, non-volatile.

    if(sigsetjmp(buf_mm, 1) == 0) {
		i=0; j=0; // KB123332
    } else {
		i = 0; j = 0;
    }

    for (; i < n1; i++) {
      for (; j < n2; j++) {
        C[ldc * i + j] = 0.0;
      }
	  j = 0;
    }
  } else if (beta != 1.0) {

	if(sigsetjmp(buf_mm, 1) == 0) {
		i=0; j=0; // KB123332
	} else {
		printf("[MM y:=beta*y case 2] i=%d j=%d\n", i, j);
	}

    for (; i < n1; i++) {
      for (; j < n2; j++) {
        C[ldc * i + j] *= beta;
      }
	  j = 0;
    }
  }
  } REAL_CATCH(0) {} REAL_END(0);

  if (alpha == 0.0)
    return;

  if (TransF == CblasNoTrans && TransG == CblasNoTrans) {

    /* form  C := alpha*A*B + C */
  REAL_TRY(1) {
  	INDEX i, j;
  	BASE* curr_c = (BASE*)malloc(sizeof(BASE) * n1 * n2);
  	memset(curr_c, 0x00, sizeof(BASE) * n1 * n2);
    if(sigsetjmp(buf_mm, 1) == 0) {
		i=0; j=0; k=0; // ?????? KB123332
	} else {
		printf("[MM Branch 1] k=%d\n",
			k);
		long ii; for(ii=0; ii<n1*n2; ii++) {
			C[ii] = curr_c[ii]; // Roll back to the start of this K loop
		}
	}

    for (; k < K; k++) {
      for (i=0; i < n1; i++) {
        const BASE temp = alpha * F[ldf * i + k];
        if (temp != 0.0) {
          for (j=0; j < n2; j++) {
            C[ldc * i + j] += temp * G[ldg * k + j];
          }
          j = 0;
        }
      }
      i = 0;
      // Back-up
      long ii; for(ii=0; ii<n1*n2; ii++) {
      	  curr_c[ii] = C[ii];
      }
    }
    free(curr_c);
  } REAL_CATCH(1) {} REAL_END(1);

  } else if (TransF == CblasNoTrans && TransG == CblasTrans) {

    /* form  C := alpha*A*B' + C */

  REAL_TRY(2) {
  	INDEX k;
	if(sigsetjmp(buf_mm, 1) == 0) {
		i=0; j=0; // KB123332
	} else {
		printf("[MM branch 2] i=%d j=%d\n",
			i, j);
	}

    for (; i < n1; i++) {
      for (; j < n2; j++) {
        BASE temp = 0.0;
        for (k = 0; k < K; k++) {
          temp += F[ldf * i + k] * G[ldg * j + k];
        }
        C[ldc * i + j] += alpha * temp;
      }
	  j = 0;
    }
  } REAL_CATCH(2) {} REAL_END(2);

  } else if (TransF == CblasTrans && TransG == CblasNoTrans) {

  REAL_TRY(3) {
  	INDEX i, j;
  	BASE* curr_c = (BASE*)malloc(sizeof(BASE) * n1 * n2);
	memset(curr_c, 0x00, sizeof(BASE) * n1 * n2);
	if(sigsetjmp(buf_mm, 1) == 0) {
		i=0; j=0; k=0; // KB123332
	} else {
		printf("[MM branch 3] i=%d k=%d\n",
			i, k);
		long ii; for(ii=0; ii<n1*n2; ii++) {
			C[ii] = curr_c[ii]; // roll back to the start of this K loop
		}
	}

    for (; k < K; k++) {
      for (i=0; i < n1; i++) {
        const BASE temp = alpha * F[ldf * k + i];
        if (temp != 0.0) {
          for (j=0; j < n2; j++) {
            C[ldc * i + j] += temp * G[ldg * k + j];
          }
        }
      }
	  // Back-up
	  long ii; for(ii=0; ii<n1*n2; ii++) {
	  	  curr_c[ii] = C[ii];
	  }
    }
    k=0;
  } REAL_CATCH(3) {} REAL_END(3);

  } else if (TransF == CblasTrans && TransG == CblasTrans) {

  REAL_TRY(4) {
  	INDEX k;
	if(sigsetjmp(buf_mm, 1) == 0) {
		i=0; j=0; // KB123332
	} else {
		printf("[MM branch 4] i=%d j=%d\n",
			i, j);
	}
    for (; i < n1; i++) {
      for (; j < n2; j++) {
        BASE temp = 0.0;
        for (k = 0; k < K; k++) {
          temp += F[ldf * k + i] * G[ldg * j + k];
        }
        C[ldc * i + j] += alpha * temp;
      }
	  j = 0;
    }
  } REAL_CATCH(4) {} REAL_END(4);

  } else {
    BLAS_ERROR("unrecognized operation");
  }
}
