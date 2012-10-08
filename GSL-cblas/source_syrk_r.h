#include "../real.h"
/* blas/source_syrk_r.h
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
  INDEX i, j, k;
  int uplo, trans;
  BASE* curr_c = (BASE*)malloc(sizeof(BASE) * N);
  
  struct sigaction sa;
  sigemptyset(&sa.sa_mask);
  sa.sa_sigaction = syrk_handler;
  if(sigaction(SIGSEGV, &sa, NULL) == -1) exit(EXIT_FAILURE);

  if (alpha == 0.0 && beta == 1.0)
    return;

  if (Order == CblasRowMajor) {
    uplo = Uplo;
    trans = (Trans == CblasConjTrans) ? CblasTrans : Trans;
  } else {
    uplo = (Uplo == CblasUpper) ? CblasLower : CblasUpper;

    if (Trans == CblasTrans || Trans == CblasConjTrans) {
      trans = CblasNoTrans;
    } else {
      trans = CblasTrans;
    }
  }

  /* form  y := beta*y */
  if (beta == 0.0) {
REAL_TRY(0) {
	INDEX i, j;
    if (uplo == CblasUpper) {
      for (i = 0; i < N; i++) {
        for (j = i; j < N; j++) {
          C[ldc * i + j] = 0.0;
        }
      }
    } else {
      for (i = 0; i < N; i++) {
        for (j = 0; j <= i; j++) {
          C[ldc * i + j] = 0.0;
        }
      }
    }
} REAL_CATCH(0) {} REAL_END(0);
  } else if (beta != 1.0) {
REAL_TRY(1) {
	INDEX i, j;
    if (uplo == CblasUpper) {
      for (i = 0; i < N; i++) {
        for (j = i; j < N; j++) {
          C[ldc * i + j] *= beta;
        }
      }
    } else {
      for (i = 0; i < N; i++) {
        for (j = 0; j <= i; j++) {
          C[ldc * i + j] *= beta;
        }
      }
    }
} REAL_CATCH(1) {} REAL_END(1);
  }

  if (alpha == 0.0)
    return;

  if (uplo == CblasUpper && trans == CblasNoTrans) {
REAL_TRY(2) {
	INDEX j, k;
	if(sigsetjmp(buf_rk, 1) == 0) {
		i = 0; j = i;
		memset(curr_c, 0x00, sizeof(BASE)*N);
	} else {
		memcpy(&C[i*ldc], curr_c, sizeof(BASE)*N); 
	}
	
    for (/*i = 0*/; i < N; i++) {
      for (j = i; j < N; j++) {
        BASE temp = 0.0;
        for (k = 0; k < K; k++) {
          temp += A[i * lda + k] * A[j * lda + k];
        }
        C[i * ldc + j] += alpha * temp;
      }
      memcpy(curr_c, &C[i*ldc], sizeof(BASE)*N);
      j = (i+1);
    }
} REAL_CATCH(2) {} REAL_END(2);
  } else if (uplo == CblasUpper && trans == CblasTrans) {
REAL_TRY(3) {
	INDEX k, j;
	if(sigsetjmp(buf_rk, 1) == 0) {
		i = 0; j = i;
	} else {
		memcpy(&C[i*ldc], curr_c, sizeof(BASE)*N);
	}
	
    for (/*i = 0*/; i < N; i++) {
      for (j = i; j < N; j++) {
        BASE temp = 0.0;
        for (k = 0; k < K; k++) {
          temp += A[k * lda + i] * A[k * lda + j];
        }
        C[i * ldc + j] += alpha * temp;
      }
      memcpy(curr_c, &C[i*ldc], sizeof(BASE)*N);
      j = (i+1);
    }
} REAL_CATCH(3) {} REAL_END(3);
  } else if (uplo == CblasLower && trans == CblasNoTrans) {
REAL_TRY(4) {
	INDEX k, j;
	if(sigsetjmp(buf_rk, 1) == 0) {
		i = 0; j=0;
	} else {
		memcpy(&C[i*ldc], curr_c, sizeof(BASE)*N);
	}
    for (/*i = 0*/; i < N; i++) {
      for (j = 0; j <= i; j++) {
        BASE temp = 0.0;
        for (k = 0; k < K; k++) {
          temp += A[i * lda + k] * A[j * lda + k];
        }
        C[i * ldc + j] += alpha * temp;
      }
      memcpy(curr_c, &C[i*ldc], sizeof(BASE)*N);
      j = 0;
    }
} REAL_CATCH(4) {} REAL_END(4);
  } else if (uplo == CblasLower && trans == CblasTrans) {
REAL_TRY(5) {
	INDEX k, j;
	if(sigsetjmp(buf_rk, 1) == 0) {
		i = 0; j = 0;
	} else {
		memcpy(&C[i*ldc], curr_c, sizeof(BASE)*N);
	}
    for (/*i = 0*/; i < N; i++) {
      for (j = 0; j <= i; j++) {
        BASE temp = 0.0;
        for (k = 0; k < K; k++) {
          temp += A[k * lda + i] * A[k * lda + j];
        }
        C[i * ldc + j] += alpha * temp;
      }
      memcpy(curr_c, &C[i*ldc], sizeof(BASE)*N);
      j = 0;
    }
} REAL_CATCH(5) {} REAL_END(5);
  } else {
    BLAS_ERROR("unrecognized operation");
  }
  
  
  free(curr_c);
}
