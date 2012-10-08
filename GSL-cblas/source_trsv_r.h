#include "../real.h"
/* blas/source_trsv_r.h
 * 
 * Copyright (C) 1996, 1997, 1998, 1999, 2000 Gerard Jungman
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
  const int nonunit = (Diag == CblasNonUnit);
//  INDEX ix, jx;
//  INDEX i, j;
  const int Trans = (TransA != CblasConjTrans) ? TransA : CblasTrans;

  struct sigaction sa;
  sigemptyset(&sa.sa_mask);
  sa.sa_sigaction = trsv_handler;
  if(sigaction(SIGSEGV, &sa, NULL) == -1) exit(EXIT_FAILURE);
  
  if (N == 0)
    return;

  /* form  x := inv( A )*x */

  if ((order == CblasRowMajor && Trans == CblasNoTrans && Uplo == CblasUpper)
      || (order == CblasColMajor && Trans == CblasTrans && Uplo == CblasLower)) {
  REAL_TRY(0){
  	INDEX j; // j should not be volatile
    /* backsubstitution */
    ix = OFFSET(N, incX) + incX * (N - 1);
    
    if (nonunit) {
      X[ix] = X[ix] / A[lda * (N - 1) + (N - 1)];
    }
    ix -= incX;
    
    if(sigsetjmp(buf_sv, 1) == 0) {
    	i = N-1;
    	printf("[SV: case 1, back.subst.]\n");
    } else {
    }
    
    for (/*i = N - 1*/; i > 0 && i--;) {
      BASE tmp = X[ix];
      jx = ix + incX;
      for (j = i + 1; j < N; j++) {
        const BASE Aij = A[lda * i + j];
        tmp -= Aij * X[jx];
        jx += incX;
      }
      if (nonunit) {
        X[ix] = tmp / A[lda * i + i];
      } else {
        X[ix] = tmp;
      }
      ix -= incX;
    }
  } REAL_CATCH(0) {} REAL_END(0);
  } else if ((order == CblasRowMajor && Trans == CblasNoTrans && Uplo == CblasLower)
             || (order == CblasColMajor && Trans == CblasTrans && Uplo == CblasUpper)) {
  REAL_TRY(1) {
  	INDEX j; // j should not be volatile
    /* forward substitution */
    ix = OFFSET(N, incX);
    if (nonunit) {
      X[ix] = X[ix] / A[lda * 0 + 0];
    }
    ix += incX;
    
    if(sigsetjmp(buf_sv, 1) == 0) {
    	i = 1;
    	printf("[SV: case 2, fwd. subst.]\n");
    } else {
    }
    
    for (; i < N; i++) {
      BASE tmp = X[ix];
      jx = OFFSET(N, incX);
      for (j = 0; j < i; j++) {
        const BASE Aij = A[lda * i + j];
        tmp -= Aij * X[jx];
        jx += incX;
      }
      if (nonunit) {
        X[ix] = tmp / A[lda * i + i];
      } else {
        X[ix] = tmp;
      }
      ix += incX;
    }
  } REAL_CATCH(1) {} REAL_END(1);
  } else if ((order == CblasRowMajor && Trans == CblasTrans && Uplo == CblasUpper)
             || (order == CblasColMajor && Trans == CblasNoTrans && Uplo == CblasLower)) {
  REAL_TRY(2) {
  	INDEX j; // j should not be volatile
    /* form  x := inv( A' )*x */

    /* forward substitution */
    ix = OFFSET(N, incX);
    if (nonunit) {
      X[ix] = X[ix] / A[lda * 0 + 0];
    }
    ix += incX;
    
    if(sigsetjmp(buf_sv, 1) == 0) {
    	i = 1;
    	printf("[SV: case 3, fwd. subst.]\n");
    } else {
    }
    
    for (; i < N; i++) {
      BASE tmp = X[ix];
      jx = OFFSET(N, incX);
      for (j = 0; j < i; j++) {
        const BASE Aji = A[lda * j + i];
        tmp -= Aji * X[jx];
        jx += incX;
      }
      if (nonunit) {
        X[ix] = tmp / A[lda * i + i];
      } else {
        X[ix] = tmp;
      }
      ix += incX;
    }
  } REAL_CATCH(2) {} REAL_END(2);
  } else if ((order == CblasRowMajor && Trans == CblasTrans && Uplo == CblasLower)
             || (order == CblasColMajor && Trans == CblasNoTrans && Uplo == CblasUpper)) {
  REAL_TRY(3) {
  	INDEX j;
    /* backsubstitution */
    ix = OFFSET(N, incX) + (N - 1) * incX;
    if (nonunit) {
      X[ix] = X[ix] / A[lda * (N - 1) + (N - 1)];
    }
    ix -= incX;
    
    if(sigsetjmp(buf_sv, 1) == 0) {
    	printf("[SV: case 4, back subst.]\n");
    	i = N-1;
    } else {
    }
    
    for (; i > 0 && i--;) {
      BASE tmp = X[ix];
      jx = ix + incX;
      for (j = i + 1; j < N; j++) {
        const BASE Aji = A[lda * j + i];
        tmp -= Aji * X[jx];
        jx += incX;
      }
      if (nonunit) {
        X[ix] = tmp / A[lda * i + i];
      } else {
        X[ix] = tmp;
      }
      ix -= incX;
    }
  } REAL_CATCH(3) {} REAL_END(3);
  } else {
    BLAS_ERROR("unrecognized operation");
  }

}
