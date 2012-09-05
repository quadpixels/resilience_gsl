// Continue to go home in a TANK
// Tank version 2012-09-05
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
  #define PROTECT_IDX_I  \
  ((i!=i1-1) && (i1-1==i2-2) && (i=i1-1)), \
  ((i1-1!=i2-2) && (i2-2==i) && (i1=i+1)), \
  ((i2-2!=i) && (i==i1-1) && (i2=i+2))	
	
  #define PROTECT_IDX_J \
  ((j!=j1-1) && (j1-1==j2-2) && (j=j1-1)), \
  ((j1-1!=j2-2) && (j2-2==j) && (j1=j+1)), \
  ((j2-2!=j) && (j==j1-1) && (j2=j+2))
  
  #define PROTECT_IDX_K  \
  ((k!=k1-1) && (k1-1==k2-2) && (k=k1-1)), \
  ((k1-1!=k2-2) && (k2-2==k) && (k1=k+1)), \
  ((k2-2!=k) && (k==k1-1) && (k2=k+2))

  #define TRI_BASE(in, b0, b1, b2) {\
  	b0 = (BASE*)trick_me_ptr((unsigned long)in, sizeof(BASE)*0); \
	b1 = (BASE*)trick_me_ptr((unsigned long)in, sizeof(BASE)*123); \
	b2 = (BASE*)trick_me_ptr((unsigned long)in, sizeof(BASE)*456); \
  }
  
  #define TR_R_B_MSG DBG(printf("[TRI_RECOVER_BASE] trsv corrected 1 element\n"));
  #define TR_R_I_MSG DBG(printf("[TRI_RECOVER_INDEX] trsv corrected 1 element\n"));
  
  #define TRI_RECOVER(b0, b1, b2) {\
  	if((b0 != b1-1) && (b1-1 == b2-2)) {b0=b1-1;}\
	else if((b1-1 != b2-2) && (b2-2==b0)) {b1=b0+1;}\
	else if((b2-2 != b0) && (b0 == b1-1)) {b2=b0+2;}\
  }
  
  #define TRI_RECOVER_BASE(b0, b1, b2) {\
  	if((b0 != b1-123) && (b1-123 == b2-456)) {b0=b1-123; TR_R_B_MSG; }\
	else if((b1-123 != b2-456) && (b2-456==b0)) {b1=b0+123; TR_R_B_MSG; }\
	else if((b2-456 != b0) && (b0 == b1-123)) {b2=b0+456; TR_R_B_MSG; }\
  }
  
  INDEX ix1, ix2, jx1, jx2;
  INDEX i1, i2, j1, j2;
  
  BASE *X0, *X1, *X2;
  TRI_BASE(X, X0, X1, X2);
  BASE *A0, *A1, *A2;
  TRI_BASE(A, A0, A1, A2);
  
  const int nonunit = (Diag == CblasNonUnit);
  INDEX ix, jx;
  INDEX i, j;
  const int Trans = (TransA != CblasConjTrans) ? TransA : CblasTrans;

  if (N == 0)
    return;

  /* form  x := inv( A )*x */

  if ((order == CblasRowMajor && Trans == CblasNoTrans && Uplo == CblasUpper)
      || (order == CblasColMajor && Trans == CblasTrans && Uplo == CblasLower)) {
  REAL_TRY(0){
    /* backsubstitution */
    ix = OFFSET(N, incX) + incX * (N - 1);
    if (nonunit) {
      X[ix] = X[ix] / A[lda * (N - 1) + (N - 1)];
    }
    ix -= incX;
    ix1 = ix+1;
    ix2 = ix+2;
    for (i = N - 2, i1=i+1, i2=i+2; i>=0; i--, i1--, i2--,
    	PROTECT_IDX_I
    	) {
//      BASE tmp = X[ix];
#define TMP_ASSIGN_X_IX \
	  TRI_RECOVER(ix, ix1, ix2); \
	  TRI_RECOVER_BASE(X0, X1, X2); \
	  BASE* p_tmp = X0 + ix; \
	  if((p_tmp != X1+ix-123) && (X1+ix-123 == X2+ix-456)) p_tmp = X1+ix-123; \
	  BASE tmp = *p_tmp;
	  
	  TMP_ASSIGN_X_IX;

      jx = ix + incX;
      jx1= jx + 1;
      jx2= jx + 1;
      trick_blackhole(jx1); trick_blackhole(jx2);
      for (j = i + 1, j1=j+1, j2=j+2; j < N; j++, j1++, j2++,
      	  PROTECT_IDX_J
      	  ) {
        const BASE Aij = A[lda * i + j];
//      tmp -= Aij * X[jx]        
#define X_JX_ASSIGN_X_JX \
        TRI_RECOVER_BASE(X0, X1, X2); \
        TRI_RECOVER(jx, jx1, jx2); \
        BASE* p_x = X0+jx; \
        if((p_x != X1+jx-123) && (X1+jx-123==X2+jx-456)) p_x = X1+jx-123; \
        BASE x_jx = *p_x;
        
        X_JX_ASSIGN_X_JX;
        tmp -= Aij * x_jx;
        
        jx += incX;
        jx1 = jx+1;
        jx2 = jx+2;
      }
      if (nonunit) {
//      X[ix] = tmp / A[lda * i + i];
#define X_IX_TMP_ASSIGN_A_LDA_I_I \
      	TRI_RECOVER_BASE(A0, A1, A2); \
      	INDEX idx = lda*i+i; \
      	BASE* p_a = A0+idx; \
      	if(p_a != A1+idx-123 && A1+idx-123==A2+idx-456) p_a=A1+idx-123; \
      	BASE a_idx = *p_a; \
      	TRI_RECOVER_BASE(X0, X1, X2); \
      	TRI_RECOVER(ix, ix1, ix2); \
      	BASE* x_ix = X0+ix; \
      	if(x_ix != X1+ix-123 && X1+ix-123==X2+ix-456) x_ix=X1+ix-123; \
      	*x_ix = tmp/a_idx; 
      	
      	X_IX_TMP_ASSIGN_A_LDA_I_I;
      } else {
      	TRI_RECOVER_BASE(X0, X1, X2);
      	TRI_RECOVER(ix, ix1, ix2);
      	BASE* x_ix = X0+ix;
      	if(x_ix != X1+ix-123 && X1+ix-123==X2+ix-456) x_ix=X1+ix-123;
        *x_ix = tmp;
      }
      ix -= incX;
      ix1 = ix+1;
      ix2 = ix+2;
    }
  } REAL_CATCH(0) {} REAL_END(0);
  } else if ((order == CblasRowMajor && Trans == CblasNoTrans && Uplo == CblasLower)
             || (order == CblasColMajor && Trans == CblasTrans && Uplo == CblasUpper)) {
  REAL_TRY(1) {
    /* forward substitution */
    ix = OFFSET(N, incX);
    if (nonunit) {
      X[ix] = X[ix] / A[lda * 0 + 0];
    }
    ix += incX;
    for (i=1, i1=2, i2=3; i < N; i++, i1++, i2++,
    	PROTECT_IDX_I
    	) {
//    BASE tmp = X[ix];
      TRI_RECOVER(ix, ix1, ix2);
      TRI_RECOVER_BASE(X0, X1, X2);
      BASE* p_xix = X0 + ix;
      if(p_xix != X1+ix-123 && X1+ix-123==X2+ix-456) p_xix=X1+ix-123;
      BASE tmp = *p_xix;
      
      jx = OFFSET(N, incX);
      jx1 = jx+1;
      jx2 = jx+2;
      for (j=0, j1=1, j2=2; j < i; j++, j1++, j2++,
      	  PROTECT_IDX_J
      	  ) {
//      tmp -= Aij * X[jx];
        const BASE Aij = A[lda * i + j];
        TRI_RECOVER(jx, jx1, jx2);
        TRI_RECOVER_BASE(X0, X1, X2);
        BASE* p_xjx = X0 + jx;
        if(p_xjx != X1+jx-123 && X1+jx-123==X2+jx-456) p_xjx = X1+jx-123; 
        tmp -= Aij * (*p_xjx);

        jx += incX;
        jx1 = jx+1;
        jx2 = jx+2;
      }
      if (nonunit) {
//      X[ix] = tmp / A[lda * i + i];
        X_IX_TMP_ASSIGN_A_LDA_I_I;
      } else {
        X[ix] = tmp;
      }
      ix += incX;
      ix1 = ix+1;
      ix2 = ix+2;
    }
  } REAL_CATCH(1) {} REAL_END(1);
  } else if ((order == CblasRowMajor && Trans == CblasTrans && Uplo == CblasUpper)
             || (order == CblasColMajor && Trans == CblasNoTrans && Uplo == CblasLower)) {
  REAL_TRY(2) {
    /* form  x := inv( A' )*x */

    /* forward substitution */
    ix = OFFSET(N, incX);
    if (nonunit) {
      X[ix] = X[ix] / A[lda * 0 + 0];
    }
    ix += incX;
    for (i=1, i1=2, i2=3; i < N; i++, i1++, i2++, 
    	PROTECT_IDX_I
    	) {
//    BASE tmp = X[ix];
	  TMP_ASSIGN_X_IX;

      jx = OFFSET(N, incX);
      for (j=0, j1=1, j2=2; j < i; j++, j1++, j2++,
      	  PROTECT_IDX_J
      	  ) {
        const BASE Aji = A[lda * j + i];
        
        X_JX_ASSIGN_X_JX;
        tmp -= Aji * x_jx;
        
        jx += incX;
        jx1 = jx+1;
        jx2 = jx+2;
      }
      if (nonunit) {
//      X[ix] = tmp / A[lda * i + i];
	    X_IX_TMP_ASSIGN_A_LDA_I_I;
      } else {
        X[ix] = tmp;
      }
      ix += incX;
    }
  } REAL_CATCH(2) {} REAL_END(2);
  } else if ((order == CblasRowMajor && Trans == CblasTrans && Uplo == CblasLower)
             || (order == CblasColMajor && Trans == CblasNoTrans && Uplo == CblasUpper)) {
  REAL_TRY(3) {
    /* backsubstitution */
    ix = OFFSET(N, incX) + (N - 1) * incX;
    if (nonunit) {
      X[ix] = X[ix] / A[lda * (N - 1) + (N - 1)];
    }
    ix -= incX;
    ix1 = ix + 1;
    ix2 = ix + 2;
    for (i=N-2, i1=i+1, i2=i+2; i >= 0; i--, i1--, i2--,
    	PROTECT_IDX_I
    	) {
//    BASE tmp = X[ix];
	  TMP_ASSIGN_X_IX;
	  
      jx = ix + incX;
      jx1 = jx+1;
      jx2 = jx+2;
      for (j = i + 1; j < N; j++) {
        const BASE Aji = A[lda * j + i];
//        tmp -= Aji * X[jx];
		X_JX_ASSIGN_X_JX;
        tmp -= Aji * x_jx;

        jx += incX;
      }
      if (nonunit) {
//        X[ix] = tmp / A[lda * i + i];
		X_IX_TMP_ASSIGN_A_LDA_I_I;
      } else {
        X[ix] = tmp;
      }
      ix -= incX;
      ix1 = ix+1;
      ix2 = ix+2;
    }
  } REAL_CATCH(3) {} REAL_END(3);
  } else {
    BLAS_ERROR("unrecognized operation");
  }
  #undef X_IX_TMP_ASSIGN_A_LDA_I_I
  #undef PROTECT_IDX_I
  #undef PROTECT_IDX_J
  #undef PROTECT_IDX_K
}
