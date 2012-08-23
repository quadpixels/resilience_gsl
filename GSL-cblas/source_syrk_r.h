// Branch "Going home in a tank!"
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

  INDEX i, j, k;
  INDEX i1, j1, k1, i2, j2, k2; // Index Triplication.
  
  BASE* A0, *A1, *A2; // Pointer Triplication.
  TRI_BASE(A, A0, A1, A2);
  BASE* C0, *C1, *C2;
  TRI_BASE(C, C0, C1, C2);

  size_t lda_, lda0, lda1, lda2;
  size_t ldc_, ldc0, ldc1, ldc2;
  lda_ = lda;
  ldc_ = ldc;
  TRIPLICATE_SIZE_T(lda, lda0, lda1, lda2);
  TRIPLICATE_SIZE_T(ldc, ldc0, ldc1, ldc2);
  INDEX dummy_1; // Trick the compiler

  int uplo, trans;

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
    if (uplo == CblasUpper) {
      for (i=0, i1=1, i2=2; i < N; i++, i1++, i2++,
           PROTECT_IDX_I
      ) {
        for (j=i, j1=i+1, j2=i+2; j < N; j++, j1++, j2++,
	     PROTECT_IDX_J
	) {
//          C[ldc * i + j] = 0.0;
	  TRI_RECOVER_SIZE_T(ldc_, ldc0, ldc1, ldc2);
	  TRI_RECOVER(C0, C1, C2);
	  int idx = ldc * i + j;
	  if(idx != ldc0*i+j) idx=ldc0*i+j;
	  BASE* c = C0 + idx;
	  if((c != C1+idx-123) && (C1+idx-123==C2+idx-456)) c=C1+idx-123;
	  *c = 0;
	  trick_blackhole(j1); trick_blackhole(j2);
        }
	trick_blackhole(i1); trick_blackhole(i2);
      }
    } else {
      for (i=0, i1=1, i2=2; i < N; i++, i1++, i2++,
           PROTECT_IDX_I
      ) {
        for (j=0, j1=1, j2=2; j <= i; j++, j1++, j2++,
	     PROTECT_IDX_J
	) {
//          C[ldc * i + j] = 0.0;
	  TRI_RECOVER_SIZE_T(ldc_, ldc0, ldc1, ldc2);
	  TRI_RECOVER(C0, C1, C2);
	  int idx = ldc * i + j;
	  if(idx != ldc0*i+j) idx=ldc0*i+j;
	  BASE* c = C0+idx;
	  if((c != C1+idx-123) && (C1+idx-123==C2+idx-456)) c=C1+idx-123;
	  *c = 0;
	  trick_blackhole(j1); trick_blackhole(j2);
	}
	trick_blackhole(i1); trick_blackhole(i2);
      }
    }
} REAL_CATCH(0) {} REAL_END(0);
  } else if (beta != 1.0) {
REAL_TRY(1) {
    if (uplo == CblasUpper) {
      for (i=0, i1=1, i2=2; i < N; i++, i1++, i2++,
           PROTECT_IDX_I
      ) {
        for (j=i, j1=i+1, j2=i+2; j < N; j++, j1++, j2++,
	     PROTECT_IDX_J
	) {
//        C[ldc * i + j] *= beta;
          TRI_RECOVER_SIZE_T(ldc_, ldc0, ldc1, ldc2);
	  TRI_RECOVER(C0, C1, C2);
	  int idx = ldc * i + j;
	  if(idx != ldc0*i+j) idx=ldc0*i+j;
	  BASE* c = C0+idx;
	  if((c != C1+idx-123) && (C1+idx-123==C2+idx-456)) c=C1+idx-123;
	  (*c) *= beta;
	  trick_blackhole(j1); trick_blackhole(j2);
        }
	trick_blackhole(i1); trick_blackhole(i2);
      }
    } else {
      for (i=0, i1=1, i2=2; i<N; i++, i1++, i2++,
           PROTECT_IDX_I
      ) {
        for (j=0, j1=1, j2=2; j <= i; j++, j1++, j2++,
	     PROTECT_IDX_J
	) {
//        C[ldc * i + j] *= beta;
          TRI_RECOVER_SIZE_T(ldc_, ldc0, ldc1, ldc2);
	  TRI_RECOVER(C0, C1, C2);
	  int idx = ldc * i + j;
	  if(idx != ldc0*i+j) idx=ldc0*i+j;
	  BASE* c = C0+idx;
	  if((c != C1+idx-123) && (C1+idx-123==C2+idx-456)) c=C1+idx-123;
	  (*c) *= beta;
	  trick_blackhole(j1); trick_blackhole(j2);
        }
	trick_blackhole(i1); trick_blackhole(i2);
      }
    }
} REAL_CATCH(1) {} REAL_END(1);
  }

  if (alpha == 0.0)
    return;

  if (uplo == CblasUpper && trans == CblasNoTrans) {
REAL_TRY(2) {
    for (i=0, i1=1, i2=2; i < N; i++, i1++, i2++,
         PROTECT_IDX_I
    ) {
      for (j=i, j1=i+1, j2=i+2; j < N; j++, j1++, j2++,
           PROTECT_IDX_J
      ) {
        BASE temp = 0.0;
        for (k=0, k1=1, k2=2; k < K; k++, k1++, k2++,
	     PROTECT_IDX_K
	) {
	  // temp += a[i * lda + k] * a[j * lda + k]
	  TRI_RECOVER(A0, A1, A2);
	  int idx = lda * i + k;
	  TRI_RECOVER_SIZE_T(lda_, lda0, lda1, lda2);
	  if(idx != lda0*i+k) idx=lda0*i+k;
	  BASE* a = A0+idx;
	  if((a != A1+idx-123) && (A1+idx-123==A2+idx-456)) a=A1+idx-123;
	  double d_a = *a;
	  
	  TRI_RECOVER(A0, A1, A2);
	  idx = lda * j + k;
	  TRI_RECOVER_SIZE_T(lda_, lda0, lda1, lda2);
	  if(idx != lda0*j+k) idx=lda0*j+k;
	  a = A0+idx;
	  if((a != A1+idx-123) && (A1+idx-123==A2+idx-456)) a=A1+idx-123;
	  double d_a2 = *a;

          temp += d_a * d_a2;
	  trick_blackhole(k1); trick_blackhole(k2);
        }

//        C[i * ldc + j] += alpha * temp;
	  TRI_RECOVER_SIZE_T(ldc_, ldc0, ldc1, ldc2);
	  TRI_RECOVER(C0, C1, C2);
	  int idx = ldc * i + j;
	  if(idx != ldc0*i+j) idx=ldc0*i+j;
	  BASE* c = C0+idx;
	  if((c != C1+idx-123) && (C1+idx-123==C2+idx-456)) c=C1+idx-123;
	  (*c) += alpha * temp;

	  trick_blackhole(j1); trick_blackhole(j2);
      }
      trick_blackhole(i1); trick_blackhole(i2);
    }
} REAL_CATCH(2) {} REAL_END(2);
  } else if (uplo == CblasUpper && trans == CblasTrans) {
REAL_TRY(3) {
    for (i=0, i1=1, i2=2; i < N; i++, i1++, i2++,
    	PROTECT_IDX_I
    ) {
      for (j=i, j1=i+1, j2=i+2; j < N; j++, j1++, j2++,
           PROTECT_IDX_J
      ) {
        BASE temp = 0.0;
        for (k=0, k1=1, k2=2; k < K; k++, k1++, k2++,
	     PROTECT_IDX_K
	) {
//        temp += A[k * lda + i] * A[k * lda + j];
	  TRI_RECOVER(A0, A1, A2);
	  int idx = lda * k + i;
	  TRI_RECOVER_SIZE_T(lda_, lda0, lda1, lda2);
	  if(idx != lda0*k+i) idx=lda0*k+i;
	  BASE* a = A0 + idx;
	  if((a != A1+idx-123) && (A1+idx-123==A2+idx-456)) a=A1+idx-123;
	  double d_a = *a;

	  TRI_RECOVER(A0, A1, A2);
	  idx = lda * k + j;
	  TRI_RECOVER_SIZE_T(lda_, lda0, lda1, lda2);
	  if(idx != lda0*k+j) idx=lda0*k+j;
	  a = A0 + idx;
	  if((a != A1+idx-123) && (A1+idx-123==A2+idx-456)) a=A1+idx-123;
	  double d_a2 = *a;

	  temp += d_a * d_a2;
	  trick_blackhole(k1); trick_blackhole(k2);
        }
//        C[i * ldc + j] += alpha * temp;
          TRI_RECOVER_SIZE_T(ldc_, ldc0, ldc1, ldc2);
	  TRI_RECOVER(C0, C1, C2);
	  int idx = ldc * i + j;
	  if(idx != ldc0*i+j) idx = ldc0*i+j;
	  BASE* c = C0+idx;
	  if((c != C1+idx-123) && (C1+idx-123==C2+idx-456)) c=C1+idx-123;
	  (*c) += alpha * temp;
	  trick_blackhole(j1); trick_blackhole(j2);
      }
      trick_blackhole(i1); trick_blackhole(i2);
    }
} REAL_CATCH(3) {} REAL_END(3);
  } else if (uplo == CblasLower && trans == CblasNoTrans) {
REAL_TRY(4) {
    for (i=0, i1=1, i2=2; i < N; i++, i1++, i2++,
         PROTECT_IDX_I
    ) {
      for (j=0, j1=1, j2=2; j <= i; j++, j1++, j2++,
           PROTECT_IDX_J
      ) {
        BASE temp = 0.0;
        for (k=0, k1=1, k2=2; k < K; k++, k1++, k2++,
	     PROTECT_IDX_K
	) {
//        temp += A[i * lda + k] * A[j * lda + k];
	  TRI_RECOVER(A0, A1, A2);
	  int idx = lda * i + k;
	  TRI_RECOVER_SIZE_T(lda_, lda0, lda1, lda2);
	  if(idx != lda0*i+k) idx = lda0*i+k;
	  BASE* a = A0+idx;
	  if((a != A1+idx-123) && (A1+idx-123==A2+idx-456)) a=A1+idx-123;
	  double d_a = *a;

	  TRI_RECOVER(A0, A1, A2);
	  idx = lda * j + k;
	  TRI_RECOVER_SIZE_T(lda_, lda0, lda1, lda2);
	  if(idx != lda0*j+k) idx = lda0*j+k;
	  a = A0+idx;
	  if((a != A1+idx-123) && (A1+idx-123==A2+idx-456)) a=A1+idx-123;
	  double d_a2 = *a;

	  temp += d_a * d_a2;
	  trick_blackhole(k1); trick_blackhole(k2);
        }
//        C[i * ldc + j] += alpha * temp;
          TRI_RECOVER_SIZE_T(ldc_, ldc0, ldc1, ldc2);
	  TRI_RECOVER(C0, C1, C2);
	  int idx = ldc * i + j;
	  if(idx != ldc0*i+j) idx = ldc0*i+j;
	  BASE* c = C0+idx;
	  if((c != C1+idx-123) && (C1+idx-123==C2+idx-456)) c=C1+idx-123;
	  (*c) += alpha * temp;
	  trick_blackhole(j1); trick_blackhole(j2);
      }
      trick_blackhole(i1); trick_blackhole(i2);
    }
} REAL_CATCH(4) {} REAL_END(4);
  } else if (uplo == CblasLower && trans == CblasTrans) {
REAL_TRY(5) {
    for (i=0, i1=1, i2=2; i < N; i++, i1++, i2++,
         PROTECT_IDX_I
    ) {
      for (j=0, j1=1, j2=2; j <= i; j++, j1++, j2++,
           PROTECT_IDX_J
      ) {
        BASE temp = 0.0;
        for (k=0, k1=1, k2=2; k < K; k++, k1++, k2++,
	     PROTECT_IDX_K
	) {
//        temp += A[k * lda + i] * A[k * lda + j];
	  TRI_RECOVER(A0, A1, A2);
	  int idx = lda * k + i;
	  TRI_RECOVER_SIZE_T(lda_, lda0, lda1, lda2);
	  if(idx != lda0*k+i) idx=lda0*k+i;
	  BASE* a = A0 + idx;
	  if((a != A1+idx-123) && (A1+idx-123==A2+idx-456)) a=A1+idx-123;
	  double d_a = *a;

	  TRI_RECOVER(A0, A1, A2);
	  idx = lda * k + j;
	  TRI_RECOVER_SIZE_T(lda_, lda0, lda1, lda2);
	  if(idx != lda0*k+j) idx=lda0*k+j;
	  a = A0 + idx;
	  if((a != A1+idx-123) && (A1+idx-123==A2+idx-456)) a=A1+idx-123;
	  double d_a2 = *a;

	  temp += d_a * d_a2;
	  trick_blackhole(k1); trick_blackhole(k2);
        }
//      C[i * ldc + j] += alpha * temp;
        TRI_RECOVER_SIZE_T(ldc_, ldc0, ldc1, ldc2);
	TRI_RECOVER(C0, C1, C2);
	int idx = ldc * i + j;
	if(idx != ldc0*i+j) idx = ldc0*i+j;
	BASE* c = C0+idx;
	if((c != C1+idx-123) && (C1+idx-123==C2+idx-456)) c=C1+idx-123;
	(*c) += alpha * temp;
	trick_blackhole(j1); trick_blackhole(j2);
      }
      trick_blackhole(i1); trick_blackhole(i2);
    }
} REAL_CATCH(5) {} REAL_END(5);
  } else {
    BLAS_ERROR("unrecognized operation");
  }
  #undef TRI_BASE
  #undef TRI_RECOVER_BASE
  #undef TR_R_B_MSG
}
