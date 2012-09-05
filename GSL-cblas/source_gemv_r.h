/* blas/source_gemv_r.h
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

/* 2012-01-13 Modified by cs */
{
  INDEX i, j;
  INDEX i1, j1, i2, j2;
  INDEX lenX, lenY;
  INDEX lenX1, lenX2, lenY1, lenY2;

  #define TRI_BASE(in, b0, b1, b2) {\
  	b0 = (BASE*)trick_me_ptr((unsigned long)in, sizeof(BASE)*0); \
	b1 = (BASE*)trick_me_ptr((unsigned long)in, sizeof(BASE)*123); \
	b2 = (BASE*)trick_me_ptr((unsigned long)in, sizeof(BASE)*456); \
  }
  #define TR_R_B_MSG DBG(printf("[TRI_RECOVER_BASE] corrected 1 element\n"));
  #define TR_R_I_MSG DBG(printf("[TRI_RECOVER_INDEX] corrected 1 element\n"));
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

  BASE *Y0, *Y1, *Y2;
  BASE *X0, *X1, *X2;
  BASE *A0, *A1, *A2;
  TRI_BASE(Y, Y0, Y1, Y2);
  TRI_BASE(X, X0, X1, X2);
  TRI_BASE(A, A0, A1, A2);

  const int Trans = (TransA != CblasConjTrans) ? TransA : CblasTrans;

  if (M == 0 || N == 0)
    return;

  if (alpha == 0.0 && beta == 1.0)
    return;

  if (Trans == CblasNoTrans) {
    lenX = N;
    lenY = M;
  } else {
    lenX = M;
    lenY = N;
  }

  /* form  y := beta*y */
  REAL_TRY(0) {
  if (beta == 0.0) {
    INDEX iy = OFFSET(lenY, incY);
    INDEX iy1= iy + 1;
    INDEX iy2= iy + 2;
    for (i = 0; i < lenY; i++) {
//      Y[iy] = 0.0;
      
#define PROTECT_IDX_IY \
      if((iy != iy1-1) && (iy1-1 == iy2-2)) iy=iy1-1; \
      if((iy1-1 != iy2-2) && (iy2-2 == iy)) iy1=iy+1; \
      if((iy2-2 != iy) && (iy == iy1-1)) iy2 = iy+2;

      PROTECT_IDX_IY;

      BASE* y_iy = Y + iy;
      if((y_iy!=Y+iy1-1) && (Y+iy1-1==Y+iy2-2)) { y_iy=Y+iy1-1; TR_R_I_MSG; }
      *y_iy = 0.0;

      iy += incY; iy1 += incY; iy2 += incY;
    }
  } else if (beta != 1.0) {
    INDEX iy = OFFSET(lenY, incY);
    for (i = 0; i < lenY; i++) {
      
      Y[iy] *= beta;
      iy += incY;
    }
  }
  } REAL_CATCH(0) {} REAL_END(0);

  if (alpha == 0.0)
    return;
  if ((order == CblasRowMajor && Trans == CblasNoTrans)
      || (order == CblasColMajor && Trans == CblasTrans)) {
	REAL_TRY(1) {
    /* form  y := alpha*A*x + y */
    INDEX iy = OFFSET(lenY, incY);
    for (i = 0; i < lenY; i++) {
      BASE temp = 0.0;
      INDEX ix = OFFSET(lenX, incX);
      for (j = 0; j < lenX; j++) {
        temp += X[ix] * A[lda * i + j];
        ix += incX;
      }
      Y[iy] += alpha * temp;
      iy += incY;
    }
    } REAL_CATCH(1) {} REAL_END(1);
  } else if ((order == CblasRowMajor && Trans == CblasTrans)
             || (order == CblasColMajor && Trans == CblasNoTrans)) {
    /* form  y := alpha*A'*x + y */
	REAL_TRY(2) {
    INDEX ix = OFFSET(lenX, incX);
    for (j = 0; j < lenX; j++) {
      const BASE temp = alpha * X[ix];
      if (temp != 0.0) {
        INDEX iy = OFFSET(lenY, incY);
        for (i = 0; i < lenY; i++) {
          Y[iy] += temp * A[lda * j + i];
          iy += incY;
        }
      }
      ix += incX;
    }
	} REAL_CATCH(2) {} REAL_END(2);

  } else {
    BLAS_ERROR("unrecognized operation");
  }
}
