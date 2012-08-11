LIBTARGET	:= libGSLODE
#SOURCES		:= bsimp.c  control.c  cscal.c  cstd.c  evolve.c  gear1.c  gear2.c  rk2.c  rk2imp.c  rk2simp.c  rk4_tommy.cpp  rk4imp.c  rk8pd.c  rkck.c  rkf45.c  step.c

# Testing accounting and correction for ODE solver

SOURCES		:= bsimp.c  control.c  cscal.c  cstd.c  evolve.c  gear1.c  gear2.c  rk2.c  rk2imp.c  rk2simp.c  rk4.c  rk4imp.c  rk8pd.c  rkck.c  rkf45.c  step.c


CFLAGS		:= -DADD_ -I../GSL-include $(HARPIA_FLAG)
CXXFLAGS	:= -DADD_ -I../GSL-include $(HARPIA_FLAG)

include ../Makefile.common
