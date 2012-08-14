; ModuleID = 'rk4.bc'
target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128"
target triple = "x86_64-unknown-linux-gnu"
	%struct..0__pthread_mutex_s = type { i32, i32, i32, i32, i32, i32, %struct.__pthread_list_t }
	%struct.MyPtr = type { i64, i64, i64, i64, i8* }
	%struct.__pthread_list_t = type { %struct.__pthread_list_t*, %struct.__pthread_list_t* }
	%struct.gsl_odeiv_step_type = type { i8*, i32, i32, i8* (i64)*, i32 (i8*, i64, double, double, double*, double*, double*, double*, %struct.gsl_odeiv_system*)*, i32 (i8*, i64)*, i32 (i8*)*, void (i8*)* }
	%struct.gsl_odeiv_system = type { i32 (double, %struct.MyPtr*, %struct.MyPtr*, i8*)*, i32 (double, %struct.MyPtr*, %struct.MyPtr*, %struct.MyPtr*, i8*)*, i64, i8* }
	%struct.pthread_attr_t = type { i64, [48 x i8] }
	%struct.pthread_mutex_t = type { %struct..0__pthread_mutex_s }
	%struct.pthread_mutexattr_t = type { i32 }
	%struct.rk4_state_t = type { double*, double*, double*, double*, double* }
	%"struct.std::ios_base::Init" = type <{ i8 }>
@cnt_diff_idx_i = global i32 0		; <i32*> [#uses=2]
@cnt_diff_ptr_k = global i32 0		; <i32*> [#uses=2]
@g_dim = global i64 0		; <i64*> [#uses=5]
@cnt_oob_ptr_k = global i32 0		; <i32*> [#uses=2]
@cnt_diff_ptr_y = global i32 0		; <i32*> [#uses=2]
@cnt_oob_ptr_y = global i32 0		; <i32*> [#uses=2]
@cnt_diff_ptr_y0 = global i32 0		; <i32*> [#uses=2]
@cnt_oob_ptr_y0 = global i32 0		; <i32*> [#uses=2]
@cnt_diff_ptr_ytmp = global i32 0		; <i32*> [#uses=2]
@cnt_oob_ptr_ytmp = global i32 0		; <i32*> [#uses=2]
@.str = internal constant [8 x i8] c"Unnamed\00"		; <[8 x i8]*> [#uses=1]
@dummy = external global i32		; <i32*> [#uses=2]
@.str1 = internal constant [39 x i8] c"failed to allocate space for rk4_state\00"		; <[39 x i8]*> [#uses=1]
@.str2 = internal constant [6 x i8] c"rk4.c\00"		; <[6 x i8]*> [#uses=1]
@.str3 = internal constant [31 x i8] c"failed to allocate space for k\00"		; <[31 x i8]*> [#uses=1]
@.str4 = internal constant [32 x i8] c"failed to allocate space for k1\00"		; <[32 x i8]*> [#uses=1]
@.str5 = internal constant [32 x i8] c"failed to allocate space for y0\00"		; <[32 x i8]*> [#uses=1]
@.str6 = internal constant [34 x i8] c"failed to allocate space for ytmp\00"		; <[34 x i8]*> [#uses=1]
@_ZStL8__ioinit = internal global %"struct.std::ios_base::Init" zeroinitializer		; <%"struct.std::ios_base::Init"*> [#uses=2]
@__dso_handle = external global i8*		; <i8**> [#uses=1]
@gsl_odeiv_step_rk4 = global %struct.gsl_odeiv_step_type* @_ZL8rk4_type		; <%struct.gsl_odeiv_step_type**> [#uses=0]
@_ZL8rk4_type = internal constant %struct.gsl_odeiv_step_type { i8* getelementptr ([4 x i8]* @.str7, i32 0, i32 0), i32 1, i32 1, i8* (i64)* @_ZL9rk4_allocm, i32 (i8*, i64, double, double, double*, double*, double*, double*, %struct.gsl_odeiv_system*)* @_ZL9rk4_applyPvmddPdS0_PKdS0_PK16gsl_odeiv_system, i32 (i8*, i64)* @_ZL9rk4_resetPvm, i32 (i8*)* @_ZL9rk4_orderPv, void (i8*)* @_ZL8rk4_freePv }, align 32		; <%struct.gsl_odeiv_step_type*> [#uses=1]
@.str7 = internal constant [4 x i8] c"rk4\00"		; <[4 x i8]*> [#uses=1]
@llvm.global_ctors = appending global [1 x { i32, void ()* }] [ { i32, void ()* } { i32 65535, void ()* @_GLOBAL__I_cnt_diff_idx_i } ]		; <[1 x { i32, void ()* }]*> [#uses=0]

@_ZL20__gthrw_pthread_oncePiPFvvE = alias weak i32 (i32*, void ()*)* @pthread_once		; <i32 (i32*, void ()*)*> [#uses=0]
@_ZL27__gthrw_pthread_getspecificj = alias weak i8* (i32)* @pthread_getspecific		; <i8* (i32)*> [#uses=0]
@_ZL27__gthrw_pthread_setspecificjPKv = alias weak i32 (i32, i8*)* @pthread_setspecific		; <i32 (i32, i8*)*> [#uses=0]
@_ZL22__gthrw_pthread_createPmPK14pthread_attr_tPFPvS3_ES3_ = alias weak i32 (i64*, %struct.pthread_attr_t*, i8* (i8*)*, i8*)* @pthread_create		; <i32 (i64*, %struct.pthread_attr_t*, i8* (i8*)*, i8*)*> [#uses=0]
@_ZL22__gthrw_pthread_cancelm = alias weak i32 (i64)* @pthread_cancel		; <i32 (i64)*> [#uses=0]
@_ZL26__gthrw_pthread_mutex_lockP15pthread_mutex_t = alias weak i32 (%struct.pthread_mutex_t*)* @pthread_mutex_lock		; <i32 (%struct.pthread_mutex_t*)*> [#uses=0]
@_ZL29__gthrw_pthread_mutex_trylockP15pthread_mutex_t = alias weak i32 (%struct.pthread_mutex_t*)* @pthread_mutex_trylock		; <i32 (%struct.pthread_mutex_t*)*> [#uses=0]
@_ZL28__gthrw_pthread_mutex_unlockP15pthread_mutex_t = alias weak i32 (%struct.pthread_mutex_t*)* @pthread_mutex_unlock		; <i32 (%struct.pthread_mutex_t*)*> [#uses=0]
@_ZL26__gthrw_pthread_mutex_initP15pthread_mutex_tPK19pthread_mutexattr_t = alias weak i32 (%struct.pthread_mutex_t*, %struct.pthread_mutexattr_t*)* @pthread_mutex_init		; <i32 (%struct.pthread_mutex_t*, %struct.pthread_mutexattr_t*)*> [#uses=0]
@_ZL26__gthrw_pthread_key_createPjPFvPvE = alias weak i32 (i32*, void (i8*)*)* @pthread_key_create		; <i32 (i32*, void (i8*)*)*> [#uses=0]
@_ZL26__gthrw_pthread_key_deletej = alias weak i32 (i32)* @pthread_key_delete		; <i32 (i32)*> [#uses=0]
@_ZL30__gthrw_pthread_mutexattr_initP19pthread_mutexattr_t = alias weak i32 (%struct.pthread_mutexattr_t*)* @pthread_mutexattr_init		; <i32 (%struct.pthread_mutexattr_t*)*> [#uses=0]
@_ZL33__gthrw_pthread_mutexattr_settypeP19pthread_mutexattr_ti = alias weak i32 (%struct.pthread_mutexattr_t*, i32)* @pthread_mutexattr_settype		; <i32 (%struct.pthread_mutexattr_t*, i32)*> [#uses=0]
@_ZL33__gthrw_pthread_mutexattr_destroyP19pthread_mutexattr_t = alias weak i32 (%struct.pthread_mutexattr_t*)* @pthread_mutexattr_destroy		; <i32 (%struct.pthread_mutexattr_t*)*> [#uses=0]

define internal void @_GLOBAL__I_cnt_diff_idx_i() {
entry:
	call void @_Z41__static_initialization_and_destruction_0ii(i32 1, i32 65535)
	br label %return

return:		; preds = %entry
	ret void
}

define void @_Z11check_idx_immm(i64 %i, i64 %i1, i64 %i2) nounwind noinline {
entry:
	%i_addr = alloca i64		; <i64*> [#uses=3]
	%i1_addr = alloca i64		; <i64*> [#uses=2]
	%i2_addr = alloca i64		; <i64*> [#uses=2]
	%"alloca point" = bitcast i32 0 to i32		; <i32> [#uses=0]
	store i64 %i, i64* %i_addr
	store i64 %i1, i64* %i1_addr
	store i64 %i2, i64* %i2_addr
	%0 = load i64* %i_addr, align 8		; <i64> [#uses=1]
	%1 = load i64* %i1_addr, align 8		; <i64> [#uses=1]
	%2 = icmp ne i64 %0, %1		; <i1> [#uses=1]
	br i1 %2, label %bb1, label %bb

bb:		; preds = %entry
	%3 = load i64* %i_addr, align 8		; <i64> [#uses=1]
	%4 = load i64* %i2_addr, align 8		; <i64> [#uses=1]
	%5 = icmp ne i64 %3, %4		; <i1> [#uses=1]
	br i1 %5, label %bb1, label %bb2

bb1:		; preds = %bb, %entry
	%6 = load i32* @cnt_diff_idx_i, align 4		; <i32> [#uses=1]
	%7 = add i32 %6, 1		; <i32> [#uses=1]
	store i32 %7, i32* @cnt_diff_idx_i, align 4
	br label %bb2

bb2:		; preds = %bb1, %bb
	br label %return

return:		; preds = %bb2
	ret void
}

define void @_Z11check_ptr_kPKdS0_S0_i(double* %k, double* %k1, double* %k2, i32 %idx) nounwind noinline {
entry:
	%k_addr = alloca double*		; <double**> [#uses=2]
	%k1_addr = alloca double*		; <double**> [#uses=2]
	%k2_addr = alloca double*		; <double**> [#uses=2]
	%idx_addr = alloca i32		; <i32*> [#uses=4]
	%kk = alloca double*		; <double**> [#uses=3]
	%"alloca point" = bitcast i32 0 to i32		; <i32> [#uses=0]
	store double* %k, double** %k_addr
	store double* %k1, double** %k1_addr
	store double* %k2, double** %k2_addr
	store i32 %idx, i32* %idx_addr
	%0 = load i32* %idx_addr, align 4		; <i32> [#uses=1]
	%1 = sext i32 %0 to i64		; <i64> [#uses=1]
	%2 = sub i64 0, %1		; <i64> [#uses=1]
	%3 = load double** %k_addr, align 8		; <double*> [#uses=1]
	%4 = getelementptr double* %3, i64 %2		; <double*> [#uses=1]
	store double* %4, double** %kk, align 8
	%5 = load double** %k1_addr, align 8		; <double*> [#uses=1]
	%6 = getelementptr double* %5, i64 -1		; <double*> [#uses=1]
	%7 = load double** %kk, align 8		; <double*> [#uses=1]
	%8 = icmp ne double* %6, %7		; <i1> [#uses=1]
	br i1 %8, label %bb1, label %bb

bb:		; preds = %entry
	%9 = load double** %k2_addr, align 8		; <double*> [#uses=1]
	%10 = getelementptr double* %9, i64 -2		; <double*> [#uses=1]
	%11 = load double** %kk, align 8		; <double*> [#uses=1]
	%12 = icmp ne double* %10, %11		; <i1> [#uses=1]
	br i1 %12, label %bb1, label %bb2

bb1:		; preds = %bb, %entry
	%13 = load i32* @cnt_diff_ptr_k, align 4		; <i32> [#uses=1]
	%14 = add i32 %13, 1		; <i32> [#uses=1]
	store i32 %14, i32* @cnt_diff_ptr_k, align 4
	br label %bb2

bb2:		; preds = %bb1, %bb
	%15 = load i32* %idx_addr, align 4		; <i32> [#uses=1]
	%16 = icmp slt i32 %15, 0		; <i1> [#uses=1]
	br i1 %16, label %bb4, label %bb3

bb3:		; preds = %bb2
	%17 = load i32* %idx_addr, align 4		; <i32> [#uses=1]
	%18 = sext i32 %17 to i64		; <i64> [#uses=1]
	%19 = load i64* @g_dim, align 8		; <i64> [#uses=1]
	%20 = icmp ugt i64 %18, %19		; <i1> [#uses=1]
	br i1 %20, label %bb4, label %bb5

bb4:		; preds = %bb3, %bb2
	%21 = load i32* @cnt_oob_ptr_k, align 4		; <i32> [#uses=1]
	%22 = add i32 %21, 1		; <i32> [#uses=1]
	store i32 %22, i32* @cnt_oob_ptr_k, align 4
	br label %bb5

bb5:		; preds = %bb4, %bb3
	br label %return

return:		; preds = %bb5
	ret void
}

define void @_Z11check_ptr_yPKdS0_S0_i(double* %y, double* %y1, double* %y2, i32 %idx) nounwind noinline {
entry:
	%y_addr = alloca double*		; <double**> [#uses=2]
	%y1_addr = alloca double*		; <double**> [#uses=2]
	%y2_addr = alloca double*		; <double**> [#uses=2]
	%idx_addr = alloca i32		; <i32*> [#uses=4]
	%yy = alloca double*		; <double**> [#uses=3]
	%"alloca point" = bitcast i32 0 to i32		; <i32> [#uses=0]
	store double* %y, double** %y_addr
	store double* %y1, double** %y1_addr
	store double* %y2, double** %y2_addr
	store i32 %idx, i32* %idx_addr
	%0 = load i32* %idx_addr, align 4		; <i32> [#uses=1]
	%1 = sext i32 %0 to i64		; <i64> [#uses=1]
	%2 = sub i64 0, %1		; <i64> [#uses=1]
	%3 = load double** %y_addr, align 8		; <double*> [#uses=1]
	%4 = getelementptr double* %3, i64 %2		; <double*> [#uses=1]
	store double* %4, double** %yy, align 8
	%5 = load double** %y1_addr, align 8		; <double*> [#uses=1]
	%6 = getelementptr double* %5, i64 -1		; <double*> [#uses=1]
	%7 = load double** %yy, align 8		; <double*> [#uses=1]
	%8 = icmp ne double* %6, %7		; <i1> [#uses=1]
	br i1 %8, label %bb1, label %bb

bb:		; preds = %entry
	%9 = load double** %y2_addr, align 8		; <double*> [#uses=1]
	%10 = getelementptr double* %9, i64 -2		; <double*> [#uses=1]
	%11 = load double** %yy, align 8		; <double*> [#uses=1]
	%12 = icmp ne double* %10, %11		; <i1> [#uses=1]
	br i1 %12, label %bb1, label %bb2

bb1:		; preds = %bb, %entry
	%13 = load i32* @cnt_diff_ptr_y, align 4		; <i32> [#uses=1]
	%14 = add i32 %13, 1		; <i32> [#uses=1]
	store i32 %14, i32* @cnt_diff_ptr_y, align 4
	br label %bb2

bb2:		; preds = %bb1, %bb
	%15 = load i32* %idx_addr, align 4		; <i32> [#uses=1]
	%16 = icmp slt i32 %15, 0		; <i1> [#uses=1]
	br i1 %16, label %bb4, label %bb3

bb3:		; preds = %bb2
	%17 = load i32* %idx_addr, align 4		; <i32> [#uses=1]
	%18 = sext i32 %17 to i64		; <i64> [#uses=1]
	%19 = load i64* @g_dim, align 8		; <i64> [#uses=1]
	%20 = icmp ugt i64 %18, %19		; <i1> [#uses=1]
	br i1 %20, label %bb4, label %bb5

bb4:		; preds = %bb3, %bb2
	%21 = load i32* @cnt_oob_ptr_y, align 4		; <i32> [#uses=1]
	%22 = add i32 %21, 1		; <i32> [#uses=1]
	store i32 %22, i32* @cnt_oob_ptr_y, align 4
	br label %bb5

bb5:		; preds = %bb4, %bb3
	br label %return

return:		; preds = %bb5
	ret void
}

define void @_Z12check_ptr_y0PKdS0_S0_i(double* %y0, double* %y0_1, double* %y0_2, i32 %idx) nounwind noinline {
entry:
	%y0_addr = alloca double*		; <double**> [#uses=2]
	%y0_1_addr = alloca double*		; <double**> [#uses=2]
	%y0_2_addr = alloca double*		; <double**> [#uses=2]
	%idx_addr = alloca i32		; <i32*> [#uses=4]
	%yy = alloca double*		; <double**> [#uses=3]
	%"alloca point" = bitcast i32 0 to i32		; <i32> [#uses=0]
	store double* %y0, double** %y0_addr
	store double* %y0_1, double** %y0_1_addr
	store double* %y0_2, double** %y0_2_addr
	store i32 %idx, i32* %idx_addr
	%0 = load i32* %idx_addr, align 4		; <i32> [#uses=1]
	%1 = sext i32 %0 to i64		; <i64> [#uses=1]
	%2 = sub i64 0, %1		; <i64> [#uses=1]
	%3 = load double** %y0_addr, align 8		; <double*> [#uses=1]
	%4 = getelementptr double* %3, i64 %2		; <double*> [#uses=1]
	store double* %4, double** %yy, align 8
	%5 = load double** %y0_1_addr, align 8		; <double*> [#uses=1]
	%6 = getelementptr double* %5, i64 -1		; <double*> [#uses=1]
	%7 = load double** %yy, align 8		; <double*> [#uses=1]
	%8 = icmp ne double* %6, %7		; <i1> [#uses=1]
	br i1 %8, label %bb1, label %bb

bb:		; preds = %entry
	%9 = load double** %y0_2_addr, align 8		; <double*> [#uses=1]
	%10 = getelementptr double* %9, i64 -2		; <double*> [#uses=1]
	%11 = load double** %yy, align 8		; <double*> [#uses=1]
	%12 = icmp ne double* %10, %11		; <i1> [#uses=1]
	br i1 %12, label %bb1, label %bb2

bb1:		; preds = %bb, %entry
	%13 = load i32* @cnt_diff_ptr_y0, align 4		; <i32> [#uses=1]
	%14 = add i32 %13, 1		; <i32> [#uses=1]
	store i32 %14, i32* @cnt_diff_ptr_y0, align 4
	br label %bb2

bb2:		; preds = %bb1, %bb
	%15 = load i32* %idx_addr, align 4		; <i32> [#uses=1]
	%16 = icmp slt i32 %15, 0		; <i1> [#uses=1]
	br i1 %16, label %bb4, label %bb3

bb3:		; preds = %bb2
	%17 = load i32* %idx_addr, align 4		; <i32> [#uses=1]
	%18 = sext i32 %17 to i64		; <i64> [#uses=1]
	%19 = load i64* @g_dim, align 8		; <i64> [#uses=1]
	%20 = icmp ugt i64 %18, %19		; <i1> [#uses=1]
	br i1 %20, label %bb4, label %bb5

bb4:		; preds = %bb3, %bb2
	%21 = load i32* @cnt_oob_ptr_y0, align 4		; <i32> [#uses=1]
	%22 = add i32 %21, 1		; <i32> [#uses=1]
	store i32 %22, i32* @cnt_oob_ptr_y0, align 4
	br label %bb5

bb5:		; preds = %bb4, %bb3
	br label %return

return:		; preds = %bb5
	ret void
}

define void @_Z14check_ptr_ytmpPKdS0_S0_i(double* %ytmp, double* %ytmp_1, double* %ytmp_2, i32 %idx) nounwind noinline {
entry:
	%ytmp_addr = alloca double*		; <double**> [#uses=2]
	%ytmp_1_addr = alloca double*		; <double**> [#uses=2]
	%ytmp_2_addr = alloca double*		; <double**> [#uses=2]
	%idx_addr = alloca i32		; <i32*> [#uses=4]
	%yy = alloca double*		; <double**> [#uses=3]
	%"alloca point" = bitcast i32 0 to i32		; <i32> [#uses=0]
	store double* %ytmp, double** %ytmp_addr
	store double* %ytmp_1, double** %ytmp_1_addr
	store double* %ytmp_2, double** %ytmp_2_addr
	store i32 %idx, i32* %idx_addr
	%0 = load i32* %idx_addr, align 4		; <i32> [#uses=1]
	%1 = sext i32 %0 to i64		; <i64> [#uses=1]
	%2 = sub i64 0, %1		; <i64> [#uses=1]
	%3 = load double** %ytmp_addr, align 8		; <double*> [#uses=1]
	%4 = getelementptr double* %3, i64 %2		; <double*> [#uses=1]
	store double* %4, double** %yy, align 8
	%5 = load double** %ytmp_1_addr, align 8		; <double*> [#uses=1]
	%6 = getelementptr double* %5, i64 -1		; <double*> [#uses=1]
	%7 = load double** %yy, align 8		; <double*> [#uses=1]
	%8 = icmp ne double* %6, %7		; <i1> [#uses=1]
	br i1 %8, label %bb1, label %bb

bb:		; preds = %entry
	%9 = load double** %ytmp_2_addr, align 8		; <double*> [#uses=1]
	%10 = getelementptr double* %9, i64 -2		; <double*> [#uses=1]
	%11 = load double** %yy, align 8		; <double*> [#uses=1]
	%12 = icmp ne double* %10, %11		; <i1> [#uses=1]
	br i1 %12, label %bb1, label %bb2

bb1:		; preds = %bb, %entry
	%13 = load i32* @cnt_diff_ptr_ytmp, align 4		; <i32> [#uses=1]
	%14 = add i32 %13, 1		; <i32> [#uses=1]
	store i32 %14, i32* @cnt_diff_ptr_ytmp, align 4
	br label %bb2

bb2:		; preds = %bb1, %bb
	%15 = load i32* %idx_addr, align 4		; <i32> [#uses=1]
	%16 = icmp slt i32 %15, 0		; <i1> [#uses=1]
	br i1 %16, label %bb4, label %bb3

bb3:		; preds = %bb2
	%17 = load i32* %idx_addr, align 4		; <i32> [#uses=1]
	%18 = sext i32 %17 to i64		; <i64> [#uses=1]
	%19 = load i64* @g_dim, align 8		; <i64> [#uses=1]
	%20 = icmp ugt i64 %18, %19		; <i1> [#uses=1]
	br i1 %20, label %bb4, label %bb5

bb4:		; preds = %bb3, %bb2
	%21 = load i32* @cnt_oob_ptr_ytmp, align 4		; <i32> [#uses=1]
	%22 = add i32 %21, 1		; <i32> [#uses=1]
	store i32 %22, i32* @cnt_oob_ptr_ytmp, align 4
	br label %bb5

bb5:		; preds = %bb4, %bb3
	br label %return

return:		; preds = %bb5
	ret void
}

define internal i32 @_ZL9rk4_orderPv(i8* %vstate) nounwind {
entry:
	%vstate_addr = alloca i8*		; <i8**> [#uses=2]
	%retval = alloca i32		; <i32*> [#uses=2]
	%state = alloca %struct.rk4_state_t*		; <%struct.rk4_state_t**> [#uses=2]
	%0 = alloca i32		; <i32*> [#uses=2]
	%"alloca point" = bitcast i32 0 to i32		; <i32> [#uses=0]
	store i8* %vstate, i8** %vstate_addr
	%1 = load i8** %vstate_addr, align 8		; <i8*> [#uses=1]
	%2 = bitcast i8* %1 to %struct.rk4_state_t*		; <%struct.rk4_state_t*> [#uses=1]
	store %struct.rk4_state_t* %2, %struct.rk4_state_t** %state, align 8
	store %struct.rk4_state_t* null, %struct.rk4_state_t** %state, align 8
	store i32 4, i32* %0, align 4
	%3 = load i32* %0, align 4		; <i32> [#uses=1]
	store i32 %3, i32* %retval, align 4
	br label %return

return:		; preds = %entry
	%retval1 = load i32* %retval		; <i32> [#uses=1]
	ret i32 %retval1
}

define internal void @_ZL8rk4_freePv(i8* %vstate) nounwind {
entry:
	%vstate_addr = alloca i8*		; <i8**> [#uses=2]
	%state = alloca %struct.rk4_state_t*		; <%struct.rk4_state_t**> [#uses=7]
	%"alloca point" = bitcast i32 0 to i32		; <i32> [#uses=0]
	store i8* %vstate, i8** %vstate_addr
	%0 = load i8** %vstate_addr, align 8		; <i8*> [#uses=1]
	%1 = bitcast i8* %0 to %struct.rk4_state_t*		; <%struct.rk4_state_t*> [#uses=1]
	store %struct.rk4_state_t* %1, %struct.rk4_state_t** %state, align 8
	%2 = load %struct.rk4_state_t** %state, align 8		; <%struct.rk4_state_t*> [#uses=1]
	%3 = getelementptr %struct.rk4_state_t* %2, i32 0, i32 0		; <double**> [#uses=1]
	%4 = load double** %3, align 8		; <double*> [#uses=1]
	%5 = bitcast double* %4 to i8*		; <i8*> [#uses=1]
	call void @free(i8* %5) nounwind
	%6 = load %struct.rk4_state_t** %state, align 8		; <%struct.rk4_state_t*> [#uses=1]
	%7 = getelementptr %struct.rk4_state_t* %6, i32 0, i32 1		; <double**> [#uses=1]
	%8 = load double** %7, align 8		; <double*> [#uses=1]
	%9 = bitcast double* %8 to i8*		; <i8*> [#uses=1]
	call void @free(i8* %9) nounwind
	%10 = load %struct.rk4_state_t** %state, align 8		; <%struct.rk4_state_t*> [#uses=1]
	%11 = getelementptr %struct.rk4_state_t* %10, i32 0, i32 2		; <double**> [#uses=1]
	%12 = load double** %11, align 8		; <double*> [#uses=1]
	%13 = bitcast double* %12 to i8*		; <i8*> [#uses=1]
	call void @free(i8* %13) nounwind
	%14 = load %struct.rk4_state_t** %state, align 8		; <%struct.rk4_state_t*> [#uses=1]
	%15 = getelementptr %struct.rk4_state_t* %14, i32 0, i32 3		; <double**> [#uses=1]
	%16 = load double** %15, align 8		; <double*> [#uses=1]
	%17 = bitcast double* %16 to i8*		; <i8*> [#uses=1]
	call void @free(i8* %17) nounwind
	%18 = load %struct.rk4_state_t** %state, align 8		; <%struct.rk4_state_t*> [#uses=1]
	%19 = getelementptr %struct.rk4_state_t* %18, i32 0, i32 4		; <double**> [#uses=1]
	%20 = load double** %19, align 8		; <double*> [#uses=1]
	%21 = bitcast double* %20 to i8*		; <i8*> [#uses=1]
	call void @free(i8* %21) nounwind
	%22 = load %struct.rk4_state_t** %state, align 8		; <%struct.rk4_state_t*> [#uses=1]
	%23 = bitcast %struct.rk4_state_t* %22 to i8*		; <i8*> [#uses=1]
	call void @free(i8* %23) nounwind
	br label %return

return:		; preds = %entry
	ret void
}

declare void @free(i8*) nounwind

define internal i32 @_ZL9rk4_resetPvm(i8* %vstate, i64 %dim) nounwind {
entry:
	%vstate_addr = alloca i8*		; <i8**> [#uses=2]
	%dim_addr = alloca i64		; <i64*> [#uses=6]
	%retval = alloca i32		; <i32*> [#uses=2]
	%state = alloca %struct.rk4_state_t*		; <%struct.rk4_state_t**> [#uses=6]
	%0 = alloca i32		; <i32*> [#uses=2]
	%"alloca point" = bitcast i32 0 to i32		; <i32> [#uses=0]
	store i8* %vstate, i8** %vstate_addr
	store i64 %dim, i64* %dim_addr
	%1 = load i8** %vstate_addr, align 8		; <i8*> [#uses=1]
	%2 = bitcast i8* %1 to %struct.rk4_state_t*		; <%struct.rk4_state_t*> [#uses=1]
	store %struct.rk4_state_t* %2, %struct.rk4_state_t** %state, align 8
	%3 = load i64* %dim_addr, align 8		; <i64> [#uses=1]
	%4 = mul i64 %3, 8		; <i64> [#uses=1]
	%5 = load %struct.rk4_state_t** %state, align 8		; <%struct.rk4_state_t*> [#uses=1]
	%6 = getelementptr %struct.rk4_state_t* %5, i32 0, i32 0		; <double**> [#uses=1]
	%7 = load double** %6, align 8		; <double*> [#uses=1]
	%8 = bitcast double* %7 to i8*		; <i8*> [#uses=1]
	call void @llvm.memset.i64(i8* %8, i8 0, i64 %4, i32 1)
	%9 = load i64* %dim_addr, align 8		; <i64> [#uses=1]
	%10 = mul i64 %9, 8		; <i64> [#uses=1]
	%11 = load %struct.rk4_state_t** %state, align 8		; <%struct.rk4_state_t*> [#uses=1]
	%12 = getelementptr %struct.rk4_state_t* %11, i32 0, i32 1		; <double**> [#uses=1]
	%13 = load double** %12, align 8		; <double*> [#uses=1]
	%14 = bitcast double* %13 to i8*		; <i8*> [#uses=1]
	call void @llvm.memset.i64(i8* %14, i8 0, i64 %10, i32 1)
	%15 = load i64* %dim_addr, align 8		; <i64> [#uses=1]
	%16 = mul i64 %15, 8		; <i64> [#uses=1]
	%17 = load %struct.rk4_state_t** %state, align 8		; <%struct.rk4_state_t*> [#uses=1]
	%18 = getelementptr %struct.rk4_state_t* %17, i32 0, i32 2		; <double**> [#uses=1]
	%19 = load double** %18, align 8		; <double*> [#uses=1]
	%20 = bitcast double* %19 to i8*		; <i8*> [#uses=1]
	call void @llvm.memset.i64(i8* %20, i8 0, i64 %16, i32 1)
	%21 = load i64* %dim_addr, align 8		; <i64> [#uses=1]
	%22 = mul i64 %21, 8		; <i64> [#uses=1]
	%23 = load %struct.rk4_state_t** %state, align 8		; <%struct.rk4_state_t*> [#uses=1]
	%24 = getelementptr %struct.rk4_state_t* %23, i32 0, i32 3		; <double**> [#uses=1]
	%25 = load double** %24, align 8		; <double*> [#uses=1]
	%26 = bitcast double* %25 to i8*		; <i8*> [#uses=1]
	call void @llvm.memset.i64(i8* %26, i8 0, i64 %22, i32 1)
	%27 = load i64* %dim_addr, align 8		; <i64> [#uses=1]
	%28 = mul i64 %27, 8		; <i64> [#uses=1]
	%29 = load %struct.rk4_state_t** %state, align 8		; <%struct.rk4_state_t*> [#uses=1]
	%30 = getelementptr %struct.rk4_state_t* %29, i32 0, i32 4		; <double**> [#uses=1]
	%31 = load double** %30, align 8		; <double*> [#uses=1]
	%32 = bitcast double* %31 to i8*		; <i8*> [#uses=1]
	call void @llvm.memset.i64(i8* %32, i8 0, i64 %28, i32 1)
	store i32 0, i32* %0, align 4
	%33 = load i32* %0, align 4		; <i32> [#uses=1]
	store i32 %33, i32* %retval, align 4
	br label %return

return:		; preds = %entry
	%retval1 = load i32* %retval		; <i32> [#uses=1]
	ret i32 %retval1
}

declare void @llvm.memset.i64(i8* nocapture, i8, i64, i32) nounwind

define linkonce void @_ZN5MyPtrC1EPKv(%struct.MyPtr* %this, i8* %ptr) noinline {
entry:
	%this_addr = alloca %struct.MyPtr*		; <%struct.MyPtr**> [#uses=2]
	%ptr_addr = alloca i8*		; <i8**> [#uses=2]
	%"alloca point" = bitcast i32 0 to i32		; <i32> [#uses=0]
	store %struct.MyPtr* %this, %struct.MyPtr** %this_addr
	store i8* %ptr, i8** %ptr_addr
	%0 = load %struct.MyPtr** %this_addr, align 8		; <%struct.MyPtr*> [#uses=1]
	%1 = load i8** %ptr_addr, align 8		; <i8*> [#uses=1]
	call void @_ZN5MyPtr4initEPKvPKcm(%struct.MyPtr* %0, i8* %1, i8* getelementptr ([8 x i8]* @.str, i32 0, i64 0), i64 -1)
	br label %return

return:		; preds = %entry
	ret void
}

declare void @_ZN5MyPtr4initEPKvPKcm(%struct.MyPtr*, i8*, i8*, i64)

define internal i32 @_ZL8rk4_stepPdPK11rk4_state_tddmPK16gsl_odeiv_system(double* %y, %struct.rk4_state_t* %state, double %h, double %t, i64 %dim, %struct.gsl_odeiv_system* %sys) {
entry:
	%y_addr = alloca double*		; <double**> [#uses=10]
	%state_addr = alloca %struct.rk4_state_t*		; <%struct.rk4_state_t**> [#uses=4]
	%h_addr = alloca double		; <double*> [#uses=11]
	%t_addr = alloca double		; <double*> [#uses=4]
	%dim_addr = alloca i64		; <i64*> [#uses=6]
	%sys_addr = alloca %struct.gsl_odeiv_system*		; <%struct.gsl_odeiv_system**> [#uses=7]
	%retval = alloca i32		; <i32*> [#uses=2]
	%s = alloca i32		; <i32*> [#uses=3]
	%s1 = alloca i32		; <i32*> [#uses=3]
	%s2 = alloca i32		; <i32*> [#uses=3]
	%p_ytmp_i = alloca double*		; <double**> [#uses=3]
	%y0_i = alloca double		; <double*> [#uses=2]
	%p_y0_i = alloca double*		; <double**> [#uses=3]
	%hki = alloca double		; <double*> [#uses=2]
	%p_y_i = alloca double*		; <double**> [#uses=4]
	%h_k_i = alloca double		; <double*> [#uses=2]
	%k_i = alloca double		; <double*> [#uses=3]
	%p_k_i = alloca double*		; <double**> [#uses=5]
	%i2 = alloca i64		; <i64*> [#uses=4]
	%i1 = alloca i64		; <i64*> [#uses=4]
	%i = alloca i64		; <i64*> [#uses=42]
	%k2 = alloca double*		; <double**> [#uses=3]
	%k1 = alloca double*		; <double**> [#uses=5]
	%k = alloca double*		; <double**> [#uses=12]
	%ytmp_2 = alloca double*		; <double**> [#uses=2]
	%ytmp_1 = alloca double*		; <double**> [#uses=2]
	%ytmp = alloca double*		; <double**> [#uses=9]
	%y0_2 = alloca double*		; <double**> [#uses=2]
	%y0_1 = alloca double*		; <double**> [#uses=2]
	%y0 = alloca double*		; <double**> [#uses=6]
	%y2 = alloca double*		; <double**> [#uses=2]
	%y1 = alloca double*		; <double**> [#uses=2]
	%0 = alloca %struct.MyPtr		; <%struct.MyPtr*> [#uses=2]
	%1 = alloca %struct.MyPtr		; <%struct.MyPtr*> [#uses=2]
	%2 = alloca %struct.MyPtr		; <%struct.MyPtr*> [#uses=2]
	%3 = alloca %struct.MyPtr		; <%struct.MyPtr*> [#uses=2]
	%4 = alloca i32		; <i32*> [#uses=5]
	%5 = alloca %struct.MyPtr		; <%struct.MyPtr*> [#uses=2]
	%6 = alloca %struct.MyPtr		; <%struct.MyPtr*> [#uses=2]
	%"alloca point" = bitcast i32 0 to i32		; <i32> [#uses=0]
	store double* %y, double** %y_addr
	store %struct.rk4_state_t* %state, %struct.rk4_state_t** %state_addr
	store double %h, double* %h_addr
	store double %t, double* %t_addr
	store i64 %dim, i64* %dim_addr
	store %struct.gsl_odeiv_system* %sys, %struct.gsl_odeiv_system** %sys_addr
	%7 = load double** %y_addr, align 8		; <double*> [#uses=1]
	%8 = getelementptr double* %7, i64 1		; <double*> [#uses=1]
	store double* %8, double** %y1, align 8
	%9 = load double** %y_addr, align 8		; <double*> [#uses=1]
	%10 = getelementptr double* %9, i64 2		; <double*> [#uses=1]
	store double* %10, double** %y2, align 8
	%11 = load i64* %dim_addr, align 8		; <i64> [#uses=1]
	store i64 %11, i64* @g_dim, align 8
	%12 = load %struct.rk4_state_t** %state_addr, align 8		; <%struct.rk4_state_t*> [#uses=1]
	%13 = getelementptr %struct.rk4_state_t* %12, i32 0, i32 2		; <double**> [#uses=1]
	%14 = load double** %13, align 8		; <double*> [#uses=1]
	store double* %14, double** %y0, align 8
	%15 = load double** %y0, align 8		; <double*> [#uses=1]
	%16 = getelementptr double* %15, i64 1		; <double*> [#uses=1]
	store double* %16, double** %y0_1, align 8
	%17 = load double** %y0, align 8		; <double*> [#uses=1]
	%18 = getelementptr double* %17, i64 2		; <double*> [#uses=1]
	store double* %18, double** %y0_2, align 8
	%19 = load %struct.rk4_state_t** %state_addr, align 8		; <%struct.rk4_state_t*> [#uses=1]
	%20 = getelementptr %struct.rk4_state_t* %19, i32 0, i32 3		; <double**> [#uses=1]
	%21 = load double** %20, align 8		; <double*> [#uses=1]
	store double* %21, double** %ytmp, align 8
	%22 = load double** %ytmp, align 8		; <double*> [#uses=1]
	%23 = getelementptr double* %22, i64 1		; <double*> [#uses=1]
	store double* %23, double** %ytmp_1, align 8
	%24 = load double** %ytmp, align 8		; <double*> [#uses=1]
	%25 = getelementptr double* %24, i64 2		; <double*> [#uses=1]
	store double* %25, double** %ytmp_2, align 8
	%26 = load %struct.rk4_state_t** %state_addr, align 8		; <%struct.rk4_state_t*> [#uses=1]
	%27 = getelementptr %struct.rk4_state_t* %26, i32 0, i32 0		; <double**> [#uses=1]
	%28 = load double** %27, align 8		; <double*> [#uses=1]
	store double* %28, double** %k, align 8
	%29 = load double** %k, align 8		; <double*> [#uses=1]
	%30 = getelementptr double* %29, i64 123		; <double*> [#uses=1]
	store double* %30, double** %k1, align 8
	%31 = load double** %k, align 8		; <double*> [#uses=1]
	%32 = getelementptr double* %31, i64 456		; <double*> [#uses=1]
	store double* %32, double** %k2, align 8
	call void @llvm_real_split(i32 0)
	%33 = load i32* @dummy, align 4		; <i32> [#uses=1]
	%34 = icmp ne i32 %33, 0		; <i1> [#uses=1]
	br i1 %34, label %jump0, label %bb

bb:		; preds = %entry
	br label %real_catch0

jump0:		; preds = %entry
	call void @llvm_real_split(i32 0)
	%35 = load i32* @dummy, align 4		; <i32> [#uses=1]
	%36 = icmp ne i32 %35, 0		; <i1> [#uses=1]
	br i1 %36, label %real_try0, label %bb3

bb3:		; preds = %jump0
	br label %real_end0

real_try0:		; preds = %jump0
	call void @llvm_real_try(i32 0)
	store i64 0, i64* %i, align 8
	store i64 0, i64* %i1, align 8
	store i64 0, i64* %i2, align 8
	br label %bb8

bb4:		; preds = %bb8
	%37 = load double** %k, align 8		; <double*> [#uses=1]
	%38 = load i64* %i, align 8		; <i64> [#uses=1]
	%39 = getelementptr double* %37, i64 %38		; <double*> [#uses=1]
	store double* %39, double** %p_k_i, align 8
	%40 = load i64* %i, align 8		; <i64> [#uses=1]
	%41 = trunc i64 %40 to i32		; <i32> [#uses=1]
	%42 = load double** %p_k_i, align 8		; <double*> [#uses=1]
	%43 = load double** %k1, align 8		; <double*> [#uses=1]
	%44 = load double** %k2, align 8		; <double*> [#uses=1]
	call void @_Z11check_ptr_kPKdS0_S0_i(double* %42, double* %43, double* %44, i32 %41) nounwind
	%45 = load double** %k1, align 8		; <double*> [#uses=1]
	%46 = load i64* %i, align 8		; <i64> [#uses=1]
	%47 = getelementptr double* %45, i64 %46		; <double*> [#uses=1]
	%48 = getelementptr double* %47, i64 -123		; <double*> [#uses=1]
	%49 = load double** %p_k_i, align 8		; <double*> [#uses=1]
	%50 = icmp ne double* %48, %49		; <i1> [#uses=1]
	br i1 %50, label %bb5, label %bb7

bb5:		; preds = %bb4
	%51 = load double** %k2, align 8		; <double*> [#uses=1]
	%52 = getelementptr double* %51, i64 -333		; <double*> [#uses=1]
	%53 = load double** %k1, align 8		; <double*> [#uses=1]
	%54 = icmp eq double* %52, %53		; <i1> [#uses=1]
	br i1 %54, label %bb6, label %bb7

bb6:		; preds = %bb5
	%55 = load double** %k1, align 8		; <double*> [#uses=1]
	%56 = load i64* %i, align 8		; <i64> [#uses=1]
	%57 = getelementptr double* %55, i64 %56		; <double*> [#uses=1]
	%58 = getelementptr double* %57, i64 -1		; <double*> [#uses=1]
	store double* %58, double** %p_k_i, align 8
	br label %bb7

bb7:		; preds = %bb6, %bb5, %bb4
	%59 = load double** %p_k_i, align 8		; <double*> [#uses=1]
	%60 = load double* %59, align 8		; <double> [#uses=1]
	store double %60, double* %k_i, align 8
	%61 = load double* %k_i, align 8		; <double> [#uses=1]
	%62 = load double* %h_addr, align 8		; <double> [#uses=1]
	%63 = mul double %61, %62		; <double> [#uses=1]
	%64 = fdiv double %63, 6.000000e+00		; <double> [#uses=1]
	store double %64, double* %h_k_i, align 8
	%65 = load double** %y_addr, align 8		; <double*> [#uses=1]
	%66 = load i64* %i, align 8		; <i64> [#uses=1]
	%67 = getelementptr double* %65, i64 %66		; <double*> [#uses=1]
	store double* %67, double** %p_y_i, align 8
	%68 = load i64* %i, align 8		; <i64> [#uses=1]
	%69 = trunc i64 %68 to i32		; <i32> [#uses=1]
	%70 = load double** %p_y_i, align 8		; <double*> [#uses=1]
	%71 = load double** %y1, align 8		; <double*> [#uses=1]
	%72 = load double** %y2, align 8		; <double*> [#uses=1]
	call void @_Z11check_ptr_yPKdS0_S0_i(double* %70, double* %71, double* %72, i32 %69) nounwind
	%73 = load double** %p_y_i, align 8		; <double*> [#uses=1]
	%74 = load double* %73, align 8		; <double> [#uses=1]
	%75 = load double* %h_k_i, align 8		; <double> [#uses=1]
	%76 = add double %74, %75		; <double> [#uses=1]
	%77 = load double** %p_y_i, align 8		; <double*> [#uses=1]
	store double %76, double* %77, align 8
	%78 = load double* %h_addr, align 8		; <double> [#uses=1]
	%79 = mul double %78, 5.000000e-01		; <double> [#uses=1]
	%80 = load double* %k_i, align 8		; <double> [#uses=1]
	%81 = mul double %79, %80		; <double> [#uses=1]
	store double %81, double* %hki, align 8
	%82 = load double** %y0, align 8		; <double*> [#uses=1]
	%83 = load i64* %i, align 8		; <i64> [#uses=1]
	%84 = getelementptr double* %82, i64 %83		; <double*> [#uses=1]
	store double* %84, double** %p_y0_i, align 8
	%85 = load i64* %i, align 8		; <i64> [#uses=1]
	%86 = trunc i64 %85 to i32		; <i32> [#uses=1]
	%87 = load double** %p_y0_i, align 8		; <double*> [#uses=1]
	%88 = load double** %y0_1, align 8		; <double*> [#uses=1]
	%89 = load double** %y0_2, align 8		; <double*> [#uses=1]
	call void @_Z12check_ptr_y0PKdS0_S0_i(double* %87, double* %88, double* %89, i32 %86) nounwind
	%90 = load double** %p_y0_i, align 8		; <double*> [#uses=1]
	%91 = load double* %90, align 8		; <double> [#uses=1]
	store double %91, double* %y0_i, align 8
	%92 = load double** %ytmp, align 8		; <double*> [#uses=1]
	%93 = load i64* %i, align 8		; <i64> [#uses=1]
	%94 = getelementptr double* %92, i64 %93		; <double*> [#uses=1]
	store double* %94, double** %p_ytmp_i, align 8
	%95 = load i64* %i, align 8		; <i64> [#uses=1]
	%96 = trunc i64 %95 to i32		; <i32> [#uses=1]
	%97 = load double** %p_ytmp_i, align 8		; <double*> [#uses=1]
	%98 = load double** %ytmp_1, align 8		; <double*> [#uses=1]
	%99 = load double** %ytmp_2, align 8		; <double*> [#uses=1]
	call void @_Z14check_ptr_ytmpPKdS0_S0_i(double* %97, double* %98, double* %99, i32 %96) nounwind
	%100 = load double* %y0_i, align 8		; <double> [#uses=1]
	%101 = load double* %hki, align 8		; <double> [#uses=1]
	%102 = add double %100, %101		; <double> [#uses=1]
	%103 = load double** %p_ytmp_i, align 8		; <double*> [#uses=1]
	store double %102, double* %103, align 8
	%104 = load i64* %i, align 8		; <i64> [#uses=1]
	%105 = add i64 %104, 1		; <i64> [#uses=1]
	store i64 %105, i64* %i, align 8
	%106 = load i64* %i1, align 8		; <i64> [#uses=1]
	%107 = add i64 %106, 1		; <i64> [#uses=1]
	store i64 %107, i64* %i1, align 8
	%108 = load i64* %i2, align 8		; <i64> [#uses=1]
	%109 = add i64 %108, 1		; <i64> [#uses=1]
	store i64 %109, i64* %i2, align 8
	%110 = load i64* %i, align 8		; <i64> [#uses=1]
	%111 = load i64* %i1, align 8		; <i64> [#uses=1]
	%112 = load i64* %i2, align 8		; <i64> [#uses=1]
	call void @_Z11check_idx_immm(i64 %110, i64 %111, i64 %112) nounwind
	br label %bb8

bb8:		; preds = %bb7, %real_try0
	%113 = load i64* %i, align 8		; <i64> [#uses=1]
	%114 = load i64* %dim_addr, align 8		; <i64> [#uses=1]
	%115 = icmp ult i64 %113, %114		; <i1> [#uses=1]
	br i1 %115, label %bb4, label %bb9

bb9:		; preds = %bb8
	br label %real_end0

real_catch0:		; preds = %bb
	call void @llvm_real_catch(i32 0)
	br label %real_end0

real_end0:		; preds = %real_catch0, %bb9, %bb3
	call void @llvm_real_end(i32 0)
	%116 = load %struct.gsl_odeiv_system** %sys_addr, align 8		; <%struct.gsl_odeiv_system*> [#uses=1]
	%117 = getelementptr %struct.gsl_odeiv_system* %116, i32 0, i32 0		; <i32 (double, %struct.MyPtr*, %struct.MyPtr*, i8*)**> [#uses=1]
	%118 = load i32 (double, %struct.MyPtr*, %struct.MyPtr*, i8*)** %117, align 8		; <i32 (double, %struct.MyPtr*, %struct.MyPtr*, i8*)*> [#uses=1]
	%119 = load %struct.gsl_odeiv_system** %sys_addr, align 8		; <%struct.gsl_odeiv_system*> [#uses=1]
	%120 = getelementptr %struct.gsl_odeiv_system* %119, i32 0, i32 3		; <i8**> [#uses=1]
	%121 = load i8** %120, align 8		; <i8*> [#uses=1]
	%122 = load double** %k, align 8		; <double*> [#uses=1]
	%123 = bitcast double* %122 to i8*		; <i8*> [#uses=1]
	call void @_ZN5MyPtrC1EPKv(%struct.MyPtr* %6, i8* %123) noinline
	%124 = load double** %ytmp, align 8		; <double*> [#uses=1]
	%125 = bitcast double* %124 to i8*		; <i8*> [#uses=1]
	call void @_ZN5MyPtrC1EPKv(%struct.MyPtr* %5, i8* %125) noinline
	%126 = load double* %h_addr, align 8		; <double> [#uses=1]
	%127 = mul double %126, 5.000000e-01		; <double> [#uses=1]
	%128 = load double* %t_addr, align 8		; <double> [#uses=1]
	%129 = add double %127, %128		; <double> [#uses=1]
	%130 = call i32 %118(double %129, %struct.MyPtr* byval %5, %struct.MyPtr* byval %6, i8* %121)		; <i32> [#uses=1]
	store i32 %130, i32* %s2, align 4
	%131 = load i32* %s2, align 4		; <i32> [#uses=1]
	%132 = icmp ne i32 %131, 0		; <i1> [#uses=1]
	br i1 %132, label %bb10, label %bb11

bb10:		; preds = %real_end0
	%133 = load i32* %s2, align 4		; <i32> [#uses=1]
	store i32 %133, i32* %4, align 4
	br label %bb25

bb11:		; preds = %real_end0
	store i64 0, i64* %i, align 8
	br label %bb13

bb12:		; preds = %bb13
	%134 = load double** %y_addr, align 8		; <double*> [#uses=1]
	%135 = load i64* %i, align 8		; <i64> [#uses=1]
	%136 = getelementptr double* %134, i64 %135		; <double*> [#uses=1]
	%137 = load double* %136, align 1		; <double> [#uses=1]
	%138 = load double* %h_addr, align 8		; <double> [#uses=1]
	%139 = fdiv double %138, 3.000000e+00		; <double> [#uses=1]
	%140 = load double** %k, align 8		; <double*> [#uses=1]
	%141 = load i64* %i, align 8		; <i64> [#uses=1]
	%142 = getelementptr double* %140, i64 %141		; <double*> [#uses=1]
	%143 = load double* %142, align 1		; <double> [#uses=1]
	%144 = mul double %139, %143		; <double> [#uses=1]
	%145 = add double %137, %144		; <double> [#uses=1]
	%146 = load double** %y_addr, align 8		; <double*> [#uses=1]
	%147 = load i64* %i, align 8		; <i64> [#uses=1]
	%148 = getelementptr double* %146, i64 %147		; <double*> [#uses=1]
	store double %145, double* %148, align 1
	%149 = load double** %y0, align 8		; <double*> [#uses=1]
	%150 = load i64* %i, align 8		; <i64> [#uses=1]
	%151 = getelementptr double* %149, i64 %150		; <double*> [#uses=1]
	%152 = load double* %151, align 1		; <double> [#uses=1]
	%153 = load double* %h_addr, align 8		; <double> [#uses=1]
	%154 = mul double %153, 5.000000e-01		; <double> [#uses=1]
	%155 = load double** %k, align 8		; <double*> [#uses=1]
	%156 = load i64* %i, align 8		; <i64> [#uses=1]
	%157 = getelementptr double* %155, i64 %156		; <double*> [#uses=1]
	%158 = load double* %157, align 1		; <double> [#uses=1]
	%159 = mul double %154, %158		; <double> [#uses=1]
	%160 = add double %152, %159		; <double> [#uses=1]
	%161 = load double** %ytmp, align 8		; <double*> [#uses=1]
	%162 = load i64* %i, align 8		; <i64> [#uses=1]
	%163 = getelementptr double* %161, i64 %162		; <double*> [#uses=1]
	store double %160, double* %163, align 1
	%164 = load i64* %i, align 8		; <i64> [#uses=1]
	%165 = add i64 %164, 1		; <i64> [#uses=1]
	store i64 %165, i64* %i, align 8
	br label %bb13

bb13:		; preds = %bb12, %bb11
	%166 = load i64* %i, align 8		; <i64> [#uses=1]
	%167 = load i64* %dim_addr, align 8		; <i64> [#uses=1]
	%168 = icmp ult i64 %166, %167		; <i1> [#uses=1]
	br i1 %168, label %bb12, label %bb14

bb14:		; preds = %bb13
	%169 = load %struct.gsl_odeiv_system** %sys_addr, align 8		; <%struct.gsl_odeiv_system*> [#uses=1]
	%170 = getelementptr %struct.gsl_odeiv_system* %169, i32 0, i32 0		; <i32 (double, %struct.MyPtr*, %struct.MyPtr*, i8*)**> [#uses=1]
	%171 = load i32 (double, %struct.MyPtr*, %struct.MyPtr*, i8*)** %170, align 8		; <i32 (double, %struct.MyPtr*, %struct.MyPtr*, i8*)*> [#uses=1]
	%172 = load %struct.gsl_odeiv_system** %sys_addr, align 8		; <%struct.gsl_odeiv_system*> [#uses=1]
	%173 = getelementptr %struct.gsl_odeiv_system* %172, i32 0, i32 3		; <i8**> [#uses=1]
	%174 = load i8** %173, align 8		; <i8*> [#uses=1]
	%175 = load double** %k, align 8		; <double*> [#uses=1]
	%176 = bitcast double* %175 to i8*		; <i8*> [#uses=1]
	call void @_ZN5MyPtrC1EPKv(%struct.MyPtr* %3, i8* %176) noinline
	%177 = load double** %ytmp, align 8		; <double*> [#uses=1]
	%178 = bitcast double* %177 to i8*		; <i8*> [#uses=1]
	call void @_ZN5MyPtrC1EPKv(%struct.MyPtr* %2, i8* %178) noinline
	%179 = load double* %h_addr, align 8		; <double> [#uses=1]
	%180 = mul double %179, 5.000000e-01		; <double> [#uses=1]
	%181 = load double* %t_addr, align 8		; <double> [#uses=1]
	%182 = add double %180, %181		; <double> [#uses=1]
	%183 = call i32 %171(double %182, %struct.MyPtr* byval %2, %struct.MyPtr* byval %3, i8* %174)		; <i32> [#uses=1]
	store i32 %183, i32* %s1, align 4
	%184 = load i32* %s1, align 4		; <i32> [#uses=1]
	%185 = icmp ne i32 %184, 0		; <i1> [#uses=1]
	br i1 %185, label %bb15, label %bb16

bb15:		; preds = %bb14
	%186 = load i32* %s1, align 4		; <i32> [#uses=1]
	store i32 %186, i32* %4, align 4
	br label %bb25

bb16:		; preds = %bb14
	store i64 0, i64* %i, align 8
	br label %bb18

bb17:		; preds = %bb18
	%187 = load double** %y_addr, align 8		; <double*> [#uses=1]
	%188 = load i64* %i, align 8		; <i64> [#uses=1]
	%189 = getelementptr double* %187, i64 %188		; <double*> [#uses=1]
	%190 = load double* %189, align 1		; <double> [#uses=1]
	%191 = load double* %h_addr, align 8		; <double> [#uses=1]
	%192 = fdiv double %191, 3.000000e+00		; <double> [#uses=1]
	%193 = load double** %k, align 8		; <double*> [#uses=1]
	%194 = load i64* %i, align 8		; <i64> [#uses=1]
	%195 = getelementptr double* %193, i64 %194		; <double*> [#uses=1]
	%196 = load double* %195, align 1		; <double> [#uses=1]
	%197 = mul double %192, %196		; <double> [#uses=1]
	%198 = add double %190, %197		; <double> [#uses=1]
	%199 = load double** %y_addr, align 8		; <double*> [#uses=1]
	%200 = load i64* %i, align 8		; <i64> [#uses=1]
	%201 = getelementptr double* %199, i64 %200		; <double*> [#uses=1]
	store double %198, double* %201, align 1
	%202 = load double** %y0, align 8		; <double*> [#uses=1]
	%203 = load i64* %i, align 8		; <i64> [#uses=1]
	%204 = getelementptr double* %202, i64 %203		; <double*> [#uses=1]
	%205 = load double* %204, align 1		; <double> [#uses=1]
	%206 = load double** %k, align 8		; <double*> [#uses=1]
	%207 = load i64* %i, align 8		; <i64> [#uses=1]
	%208 = getelementptr double* %206, i64 %207		; <double*> [#uses=1]
	%209 = load double* %208, align 1		; <double> [#uses=1]
	%210 = load double* %h_addr, align 8		; <double> [#uses=1]
	%211 = mul double %209, %210		; <double> [#uses=1]
	%212 = add double %205, %211		; <double> [#uses=1]
	%213 = load double** %ytmp, align 8		; <double*> [#uses=1]
	%214 = load i64* %i, align 8		; <i64> [#uses=1]
	%215 = getelementptr double* %213, i64 %214		; <double*> [#uses=1]
	store double %212, double* %215, align 1
	%216 = load i64* %i, align 8		; <i64> [#uses=1]
	%217 = add i64 %216, 1		; <i64> [#uses=1]
	store i64 %217, i64* %i, align 8
	br label %bb18

bb18:		; preds = %bb17, %bb16
	%218 = load i64* %i, align 8		; <i64> [#uses=1]
	%219 = load i64* %dim_addr, align 8		; <i64> [#uses=1]
	%220 = icmp ult i64 %218, %219		; <i1> [#uses=1]
	br i1 %220, label %bb17, label %bb19

bb19:		; preds = %bb18
	%221 = load %struct.gsl_odeiv_system** %sys_addr, align 8		; <%struct.gsl_odeiv_system*> [#uses=1]
	%222 = getelementptr %struct.gsl_odeiv_system* %221, i32 0, i32 0		; <i32 (double, %struct.MyPtr*, %struct.MyPtr*, i8*)**> [#uses=1]
	%223 = load i32 (double, %struct.MyPtr*, %struct.MyPtr*, i8*)** %222, align 8		; <i32 (double, %struct.MyPtr*, %struct.MyPtr*, i8*)*> [#uses=1]
	%224 = load %struct.gsl_odeiv_system** %sys_addr, align 8		; <%struct.gsl_odeiv_system*> [#uses=1]
	%225 = getelementptr %struct.gsl_odeiv_system* %224, i32 0, i32 3		; <i8**> [#uses=1]
	%226 = load i8** %225, align 8		; <i8*> [#uses=1]
	%227 = load double** %k, align 8		; <double*> [#uses=1]
	%228 = bitcast double* %227 to i8*		; <i8*> [#uses=1]
	call void @_ZN5MyPtrC1EPKv(%struct.MyPtr* %1, i8* %228) noinline
	%229 = load double** %ytmp, align 8		; <double*> [#uses=1]
	%230 = bitcast double* %229 to i8*		; <i8*> [#uses=1]
	call void @_ZN5MyPtrC1EPKv(%struct.MyPtr* %0, i8* %230) noinline
	%231 = load double* %t_addr, align 8		; <double> [#uses=1]
	%232 = load double* %h_addr, align 8		; <double> [#uses=1]
	%233 = add double %231, %232		; <double> [#uses=1]
	%234 = call i32 %223(double %233, %struct.MyPtr* byval %0, %struct.MyPtr* byval %1, i8* %226)		; <i32> [#uses=1]
	store i32 %234, i32* %s, align 4
	%235 = load i32* %s, align 4		; <i32> [#uses=1]
	%236 = icmp ne i32 %235, 0		; <i1> [#uses=1]
	br i1 %236, label %bb20, label %bb21

bb20:		; preds = %bb19
	%237 = load i32* %s, align 4		; <i32> [#uses=1]
	store i32 %237, i32* %4, align 4
	br label %bb25

bb21:		; preds = %bb19
	store i64 0, i64* %i, align 8
	br label %bb23

bb22:		; preds = %bb23
	%238 = load double** %y_addr, align 8		; <double*> [#uses=1]
	%239 = load i64* %i, align 8		; <i64> [#uses=1]
	%240 = getelementptr double* %238, i64 %239		; <double*> [#uses=1]
	%241 = load double* %240, align 1		; <double> [#uses=1]
	%242 = load double* %h_addr, align 8		; <double> [#uses=1]
	%243 = fdiv double %242, 6.000000e+00		; <double> [#uses=1]
	%244 = load double** %k, align 8		; <double*> [#uses=1]
	%245 = load i64* %i, align 8		; <i64> [#uses=1]
	%246 = getelementptr double* %244, i64 %245		; <double*> [#uses=1]
	%247 = load double* %246, align 1		; <double> [#uses=1]
	%248 = mul double %243, %247		; <double> [#uses=1]
	%249 = add double %241, %248		; <double> [#uses=1]
	%250 = load double** %y_addr, align 8		; <double*> [#uses=1]
	%251 = load i64* %i, align 8		; <i64> [#uses=1]
	%252 = getelementptr double* %250, i64 %251		; <double*> [#uses=1]
	store double %249, double* %252, align 1
	%253 = load i64* %i, align 8		; <i64> [#uses=1]
	%254 = add i64 %253, 1		; <i64> [#uses=1]
	store i64 %254, i64* %i, align 8
	br label %bb23

bb23:		; preds = %bb22, %bb21
	%255 = load i64* %i, align 8		; <i64> [#uses=1]
	%256 = load i64* %dim_addr, align 8		; <i64> [#uses=1]
	%257 = icmp ult i64 %255, %256		; <i1> [#uses=1]
	br i1 %257, label %bb22, label %bb24

bb24:		; preds = %bb23
	store i32 0, i32* %4, align 4
	br label %bb25

bb25:		; preds = %bb24, %bb20, %bb15, %bb10
	%258 = load i32* %4, align 4		; <i32> [#uses=1]
	store i32 %258, i32* %retval, align 4
	br label %return

return:		; preds = %bb25
	%retval26 = load i32* %retval		; <i32> [#uses=1]
	ret i32 %retval26
}

declare void @llvm_real_split(i32)

declare void @llvm_real_try(i32)

declare void @llvm_real_catch(i32)

declare void @llvm_real_end(i32)

define internal i32 @_ZL9rk4_applyPvmddPdS0_PKdS0_PK16gsl_odeiv_system(i8* %vstate, i64 %dim, double %t, double %h, double* %y, double* %yerr, double* %dydt_in, double* %dydt_out, %struct.gsl_odeiv_system* %sys) {
entry:
	%vstate_addr = alloca i8*		; <i8**> [#uses=2]
	%dim_addr = alloca i64		; <i64*> [#uses=16]
	%t_addr = alloca double		; <double*> [#uses=7]
	%h_addr = alloca double		; <double*> [#uses=7]
	%y_addr = alloca double*		; <double**> [#uses=13]
	%yerr_addr = alloca double*		; <double**> [#uses=2]
	%dydt_in_addr = alloca double*		; <double**> [#uses=3]
	%dydt_out_addr = alloca double*		; <double**> [#uses=3]
	%sys_addr = alloca %struct.gsl_odeiv_system*		; <%struct.gsl_odeiv_system**> [#uses=10]
	%retval = alloca i32		; <i32*> [#uses=2]
	%s = alloca i32		; <i32*> [#uses=3]
	%s1 = alloca i32		; <i32*> [#uses=3]
	%s2 = alloca i32		; <i32*> [#uses=3]
	%s3 = alloca i32		; <i32*> [#uses=3]
	%s4 = alloca i32		; <i32*> [#uses=3]
	%s5 = alloca i32		; <i32*> [#uses=3]
	%y_onestep = alloca double*		; <double**> [#uses=4]
	%y0 = alloca double*		; <double**> [#uses=7]
	%k1 = alloca double*		; <double**> [#uses=6]
	%k = alloca double*		; <double**> [#uses=6]
	%i = alloca i64		; <i64*> [#uses=7]
	%state = alloca %struct.rk4_state_t*		; <%struct.rk4_state_t**> [#uses=8]
	%0 = alloca %struct.MyPtr		; <%struct.MyPtr*> [#uses=2]
	%1 = alloca %struct.MyPtr		; <%struct.MyPtr*> [#uses=2]
	%2 = alloca %struct.MyPtr		; <%struct.MyPtr*> [#uses=2]
	%3 = alloca %struct.MyPtr		; <%struct.MyPtr*> [#uses=2]
	%4 = alloca i32		; <i32*> [#uses=8]
	%5 = alloca %struct.MyPtr		; <%struct.MyPtr*> [#uses=2]
	%6 = alloca %struct.MyPtr		; <%struct.MyPtr*> [#uses=2]
	%"alloca point" = bitcast i32 0 to i32		; <i32> [#uses=0]
	store i8* %vstate, i8** %vstate_addr
	store i64 %dim, i64* %dim_addr
	store double %t, double* %t_addr
	store double %h, double* %h_addr
	store double* %y, double** %y_addr
	store double* %yerr, double** %yerr_addr
	store double* %dydt_in, double** %dydt_in_addr
	store double* %dydt_out, double** %dydt_out_addr
	store %struct.gsl_odeiv_system* %sys, %struct.gsl_odeiv_system** %sys_addr
	%7 = load i8** %vstate_addr, align 8		; <i8*> [#uses=1]
	%8 = bitcast i8* %7 to %struct.rk4_state_t*		; <%struct.rk4_state_t*> [#uses=1]
	store %struct.rk4_state_t* %8, %struct.rk4_state_t** %state, align 8
	%9 = load %struct.rk4_state_t** %state, align 8		; <%struct.rk4_state_t*> [#uses=1]
	%10 = getelementptr %struct.rk4_state_t* %9, i32 0, i32 0		; <double**> [#uses=1]
	%11 = load double** %10, align 8		; <double*> [#uses=1]
	store double* %11, double** %k, align 8
	%12 = load %struct.rk4_state_t** %state, align 8		; <%struct.rk4_state_t*> [#uses=1]
	%13 = getelementptr %struct.rk4_state_t* %12, i32 0, i32 1		; <double**> [#uses=1]
	%14 = load double** %13, align 8		; <double*> [#uses=1]
	store double* %14, double** %k1, align 8
	%15 = load %struct.rk4_state_t** %state, align 8		; <%struct.rk4_state_t*> [#uses=1]
	%16 = getelementptr %struct.rk4_state_t* %15, i32 0, i32 2		; <double**> [#uses=1]
	%17 = load double** %16, align 8		; <double*> [#uses=1]
	store double* %17, double** %y0, align 8
	%18 = load %struct.rk4_state_t** %state, align 8		; <%struct.rk4_state_t*> [#uses=1]
	%19 = getelementptr %struct.rk4_state_t* %18, i32 0, i32 4		; <double**> [#uses=1]
	%20 = load double** %19, align 8		; <double*> [#uses=1]
	store double* %20, double** %y_onestep, align 8
	%21 = load i64* %dim_addr, align 8		; <i64> [#uses=1]
	%22 = mul i64 %21, 8		; <i64> [#uses=1]
	%23 = load double** %y0, align 8		; <double*> [#uses=1]
	%24 = load double** %y_addr, align 8		; <double*> [#uses=1]
	%25 = bitcast double* %23 to i8*		; <i8*> [#uses=1]
	%26 = bitcast double* %24 to i8*		; <i8*> [#uses=1]
	call void @llvm.memcpy.i64(i8* %25, i8* %26, i64 %22, i32 1)
	%27 = load double** %dydt_in_addr, align 8		; <double*> [#uses=1]
	%28 = icmp ne double* %27, null		; <i1> [#uses=1]
	br i1 %28, label %bb, label %bb6

bb:		; preds = %entry
	%29 = load i64* %dim_addr, align 8		; <i64> [#uses=1]
	%30 = mul i64 %29, 8		; <i64> [#uses=1]
	%31 = load double** %k, align 8		; <double*> [#uses=1]
	%32 = load double** %dydt_in_addr, align 8		; <double*> [#uses=1]
	%33 = bitcast double* %31 to i8*		; <i8*> [#uses=1]
	%34 = bitcast double* %32 to i8*		; <i8*> [#uses=1]
	call void @llvm.memcpy.i64(i8* %33, i8* %34, i64 %30, i32 1)
	br label %bb8

bb6:		; preds = %entry
	%35 = load %struct.gsl_odeiv_system** %sys_addr, align 8		; <%struct.gsl_odeiv_system*> [#uses=1]
	%36 = getelementptr %struct.gsl_odeiv_system* %35, i32 0, i32 0		; <i32 (double, %struct.MyPtr*, %struct.MyPtr*, i8*)**> [#uses=1]
	%37 = load i32 (double, %struct.MyPtr*, %struct.MyPtr*, i8*)** %36, align 8		; <i32 (double, %struct.MyPtr*, %struct.MyPtr*, i8*)*> [#uses=1]
	%38 = load %struct.gsl_odeiv_system** %sys_addr, align 8		; <%struct.gsl_odeiv_system*> [#uses=1]
	%39 = getelementptr %struct.gsl_odeiv_system* %38, i32 0, i32 3		; <i8**> [#uses=1]
	%40 = load i8** %39, align 8		; <i8*> [#uses=1]
	%41 = load double** %k, align 8		; <double*> [#uses=1]
	%42 = bitcast double* %41 to i8*		; <i8*> [#uses=1]
	call void @_ZN5MyPtrC1EPKv(%struct.MyPtr* %6, i8* %42) noinline
	%43 = load double** %y0, align 8		; <double*> [#uses=1]
	%44 = bitcast double* %43 to i8*		; <i8*> [#uses=1]
	call void @_ZN5MyPtrC1EPKv(%struct.MyPtr* %5, i8* %44) noinline
	%45 = load double* %t_addr, align 8		; <double> [#uses=1]
	%46 = call i32 %37(double %45, %struct.MyPtr* byval %5, %struct.MyPtr* byval %6, i8* %40)		; <i32> [#uses=1]
	store i32 %46, i32* %s5, align 4
	%47 = load i32* %s5, align 4		; <i32> [#uses=1]
	%48 = icmp ne i32 %47, 0		; <i1> [#uses=1]
	br i1 %48, label %bb7, label %bb8

bb7:		; preds = %bb6
	%49 = load i32* %s5, align 4		; <i32> [#uses=1]
	store i32 %49, i32* %4, align 4
	br label %bb23

bb8:		; preds = %bb6, %bb
	%50 = load i64* %dim_addr, align 8		; <i64> [#uses=1]
	%51 = mul i64 %50, 8		; <i64> [#uses=1]
	%52 = load double** %k1, align 8		; <double*> [#uses=1]
	%53 = load double** %k, align 8		; <double*> [#uses=1]
	%54 = bitcast double* %52 to i8*		; <i8*> [#uses=1]
	%55 = bitcast double* %53 to i8*		; <i8*> [#uses=1]
	call void @llvm.memcpy.i64(i8* %54, i8* %55, i64 %51, i32 1)
	%56 = load i64* %dim_addr, align 8		; <i64> [#uses=1]
	%57 = mul i64 %56, 8		; <i64> [#uses=1]
	%58 = load double** %y_onestep, align 8		; <double*> [#uses=1]
	%59 = load double** %y_addr, align 8		; <double*> [#uses=1]
	%60 = bitcast double* %58 to i8*		; <i8*> [#uses=1]
	%61 = bitcast double* %59 to i8*		; <i8*> [#uses=1]
	call void @llvm.memcpy.i64(i8* %60, i8* %61, i64 %57, i32 1)
	%62 = load double** %y_onestep, align 8		; <double*> [#uses=1]
	%63 = load %struct.rk4_state_t** %state, align 8		; <%struct.rk4_state_t*> [#uses=1]
	%64 = load double* %h_addr, align 8		; <double> [#uses=1]
	%65 = load double* %t_addr, align 8		; <double> [#uses=1]
	%66 = load i64* %dim_addr, align 8		; <i64> [#uses=1]
	%67 = load %struct.gsl_odeiv_system** %sys_addr, align 8		; <%struct.gsl_odeiv_system*> [#uses=1]
	%68 = call i32 @_ZL8rk4_stepPdPK11rk4_state_tddmPK16gsl_odeiv_system(double* %62, %struct.rk4_state_t* %63, double %64, double %65, i64 %66, %struct.gsl_odeiv_system* %67)		; <i32> [#uses=1]
	store i32 %68, i32* %s4, align 4
	%69 = load i32* %s4, align 4		; <i32> [#uses=1]
	%70 = icmp ne i32 %69, 0		; <i1> [#uses=1]
	br i1 %70, label %bb9, label %bb10

bb9:		; preds = %bb8
	%71 = load i32* %s4, align 4		; <i32> [#uses=1]
	store i32 %71, i32* %4, align 4
	br label %bb23

bb10:		; preds = %bb8
	%72 = load i64* %dim_addr, align 8		; <i64> [#uses=1]
	%73 = mul i64 %72, 8		; <i64> [#uses=1]
	%74 = load double** %k, align 8		; <double*> [#uses=1]
	%75 = load double** %k1, align 8		; <double*> [#uses=1]
	%76 = bitcast double* %74 to i8*		; <i8*> [#uses=1]
	%77 = bitcast double* %75 to i8*		; <i8*> [#uses=1]
	call void @llvm.memcpy.i64(i8* %76, i8* %77, i64 %73, i32 1)
	%78 = load double* %h_addr, align 8		; <double> [#uses=1]
	%79 = fdiv double %78, 2.000000e+00		; <double> [#uses=1]
	%80 = load double** %y_addr, align 8		; <double*> [#uses=1]
	%81 = load %struct.rk4_state_t** %state, align 8		; <%struct.rk4_state_t*> [#uses=1]
	%82 = load double* %t_addr, align 8		; <double> [#uses=1]
	%83 = load i64* %dim_addr, align 8		; <i64> [#uses=1]
	%84 = load %struct.gsl_odeiv_system** %sys_addr, align 8		; <%struct.gsl_odeiv_system*> [#uses=1]
	%85 = call i32 @_ZL8rk4_stepPdPK11rk4_state_tddmPK16gsl_odeiv_system(double* %80, %struct.rk4_state_t* %81, double %79, double %82, i64 %83, %struct.gsl_odeiv_system* %84)		; <i32> [#uses=1]
	store i32 %85, i32* %s3, align 4
	%86 = load i32* %s3, align 4		; <i32> [#uses=1]
	%87 = icmp ne i32 %86, 0		; <i1> [#uses=1]
	br i1 %87, label %bb11, label %bb12

bb11:		; preds = %bb10
	%88 = load i64* %dim_addr, align 8		; <i64> [#uses=1]
	%89 = mul i64 %88, 8		; <i64> [#uses=1]
	%90 = load double** %y_addr, align 8		; <double*> [#uses=1]
	%91 = load double** %y0, align 8		; <double*> [#uses=1]
	%92 = bitcast double* %90 to i8*		; <i8*> [#uses=1]
	%93 = bitcast double* %91 to i8*		; <i8*> [#uses=1]
	call void @llvm.memcpy.i64(i8* %92, i8* %93, i64 %89, i32 1)
	%94 = load i32* %s3, align 4		; <i32> [#uses=1]
	store i32 %94, i32* %4, align 4
	br label %bb23

bb12:		; preds = %bb10
	%95 = load %struct.gsl_odeiv_system** %sys_addr, align 8		; <%struct.gsl_odeiv_system*> [#uses=1]
	%96 = getelementptr %struct.gsl_odeiv_system* %95, i32 0, i32 0		; <i32 (double, %struct.MyPtr*, %struct.MyPtr*, i8*)**> [#uses=1]
	%97 = load i32 (double, %struct.MyPtr*, %struct.MyPtr*, i8*)** %96, align 8		; <i32 (double, %struct.MyPtr*, %struct.MyPtr*, i8*)*> [#uses=1]
	%98 = load %struct.gsl_odeiv_system** %sys_addr, align 8		; <%struct.gsl_odeiv_system*> [#uses=1]
	%99 = getelementptr %struct.gsl_odeiv_system* %98, i32 0, i32 3		; <i8**> [#uses=1]
	%100 = load i8** %99, align 8		; <i8*> [#uses=1]
	%101 = load double** %k, align 8		; <double*> [#uses=1]
	%102 = bitcast double* %101 to i8*		; <i8*> [#uses=1]
	call void @_ZN5MyPtrC1EPKv(%struct.MyPtr* %3, i8* %102) noinline
	%103 = load double** %y_addr, align 8		; <double*> [#uses=1]
	%104 = bitcast double* %103 to i8*		; <i8*> [#uses=1]
	call void @_ZN5MyPtrC1EPKv(%struct.MyPtr* %2, i8* %104) noinline
	%105 = load double* %h_addr, align 8		; <double> [#uses=1]
	%106 = fdiv double %105, 2.000000e+00		; <double> [#uses=1]
	%107 = load double* %t_addr, align 8		; <double> [#uses=1]
	%108 = add double %106, %107		; <double> [#uses=1]
	%109 = call i32 %97(double %108, %struct.MyPtr* byval %2, %struct.MyPtr* byval %3, i8* %100)		; <i32> [#uses=1]
	store i32 %109, i32* %s2, align 4
	%110 = load i32* %s2, align 4		; <i32> [#uses=1]
	%111 = icmp ne i32 %110, 0		; <i1> [#uses=1]
	br i1 %111, label %bb13, label %bb14

bb13:		; preds = %bb12
	%112 = load i64* %dim_addr, align 8		; <i64> [#uses=1]
	%113 = mul i64 %112, 8		; <i64> [#uses=1]
	%114 = load double** %y_addr, align 8		; <double*> [#uses=1]
	%115 = load double** %y0, align 8		; <double*> [#uses=1]
	%116 = bitcast double* %114 to i8*		; <i8*> [#uses=1]
	%117 = bitcast double* %115 to i8*		; <i8*> [#uses=1]
	call void @llvm.memcpy.i64(i8* %116, i8* %117, i64 %113, i32 1)
	%118 = load i32* %s2, align 4		; <i32> [#uses=1]
	store i32 %118, i32* %4, align 4
	br label %bb23

bb14:		; preds = %bb12
	%119 = load i64* %dim_addr, align 8		; <i64> [#uses=1]
	%120 = mul i64 %119, 8		; <i64> [#uses=1]
	%121 = load double** %k1, align 8		; <double*> [#uses=1]
	%122 = load double** %y0, align 8		; <double*> [#uses=1]
	%123 = bitcast double* %121 to i8*		; <i8*> [#uses=1]
	%124 = bitcast double* %122 to i8*		; <i8*> [#uses=1]
	call void @llvm.memcpy.i64(i8* %123, i8* %124, i64 %120, i32 1)
	%125 = load i64* %dim_addr, align 8		; <i64> [#uses=1]
	%126 = mul i64 %125, 8		; <i64> [#uses=1]
	%127 = load double** %y0, align 8		; <double*> [#uses=1]
	%128 = load double** %y_addr, align 8		; <double*> [#uses=1]
	%129 = bitcast double* %127 to i8*		; <i8*> [#uses=1]
	%130 = bitcast double* %128 to i8*		; <i8*> [#uses=1]
	call void @llvm.memcpy.i64(i8* %129, i8* %130, i64 %126, i32 1)
	%131 = load double* %h_addr, align 8		; <double> [#uses=1]
	%132 = fdiv double %131, 2.000000e+00		; <double> [#uses=1]
	%133 = load double* %t_addr, align 8		; <double> [#uses=1]
	%134 = add double %132, %133		; <double> [#uses=1]
	%135 = load double* %h_addr, align 8		; <double> [#uses=1]
	%136 = fdiv double %135, 2.000000e+00		; <double> [#uses=1]
	%137 = load double** %y_addr, align 8		; <double*> [#uses=1]
	%138 = load %struct.rk4_state_t** %state, align 8		; <%struct.rk4_state_t*> [#uses=1]
	%139 = load i64* %dim_addr, align 8		; <i64> [#uses=1]
	%140 = load %struct.gsl_odeiv_system** %sys_addr, align 8		; <%struct.gsl_odeiv_system*> [#uses=1]
	%141 = call i32 @_ZL8rk4_stepPdPK11rk4_state_tddmPK16gsl_odeiv_system(double* %137, %struct.rk4_state_t* %138, double %136, double %134, i64 %139, %struct.gsl_odeiv_system* %140)		; <i32> [#uses=1]
	store i32 %141, i32* %s1, align 4
	%142 = load i32* %s1, align 4		; <i32> [#uses=1]
	%143 = icmp ne i32 %142, 0		; <i1> [#uses=1]
	br i1 %143, label %bb15, label %bb16

bb15:		; preds = %bb14
	%144 = load i64* %dim_addr, align 8		; <i64> [#uses=1]
	%145 = mul i64 %144, 8		; <i64> [#uses=1]
	%146 = load double** %y_addr, align 8		; <double*> [#uses=1]
	%147 = load double** %k1, align 8		; <double*> [#uses=1]
	%148 = bitcast double* %146 to i8*		; <i8*> [#uses=1]
	%149 = bitcast double* %147 to i8*		; <i8*> [#uses=1]
	call void @llvm.memcpy.i64(i8* %148, i8* %149, i64 %145, i32 1)
	%150 = load i32* %s1, align 4		; <i32> [#uses=1]
	store i32 %150, i32* %4, align 4
	br label %bb23

bb16:		; preds = %bb14
	%151 = load double** %dydt_out_addr, align 8		; <double*> [#uses=1]
	%152 = icmp ne double* %151, null		; <i1> [#uses=1]
	br i1 %152, label %bb17, label %bb19

bb17:		; preds = %bb16
	%153 = load %struct.gsl_odeiv_system** %sys_addr, align 8		; <%struct.gsl_odeiv_system*> [#uses=1]
	%154 = getelementptr %struct.gsl_odeiv_system* %153, i32 0, i32 0		; <i32 (double, %struct.MyPtr*, %struct.MyPtr*, i8*)**> [#uses=1]
	%155 = load i32 (double, %struct.MyPtr*, %struct.MyPtr*, i8*)** %154, align 8		; <i32 (double, %struct.MyPtr*, %struct.MyPtr*, i8*)*> [#uses=1]
	%156 = load %struct.gsl_odeiv_system** %sys_addr, align 8		; <%struct.gsl_odeiv_system*> [#uses=1]
	%157 = getelementptr %struct.gsl_odeiv_system* %156, i32 0, i32 3		; <i8**> [#uses=1]
	%158 = load i8** %157, align 8		; <i8*> [#uses=1]
	%159 = load double** %dydt_out_addr, align 8		; <double*> [#uses=1]
	%160 = bitcast double* %159 to i8*		; <i8*> [#uses=1]
	call void @_ZN5MyPtrC1EPKv(%struct.MyPtr* %1, i8* %160) noinline
	%161 = load double** %y_addr, align 8		; <double*> [#uses=1]
	%162 = bitcast double* %161 to i8*		; <i8*> [#uses=1]
	call void @_ZN5MyPtrC1EPKv(%struct.MyPtr* %0, i8* %162) noinline
	%163 = load double* %t_addr, align 8		; <double> [#uses=1]
	%164 = load double* %h_addr, align 8		; <double> [#uses=1]
	%165 = add double %163, %164		; <double> [#uses=1]
	%166 = call i32 %155(double %165, %struct.MyPtr* byval %0, %struct.MyPtr* byval %1, i8* %158)		; <i32> [#uses=1]
	store i32 %166, i32* %s, align 4
	%167 = load i32* %s, align 4		; <i32> [#uses=1]
	%168 = icmp ne i32 %167, 0		; <i1> [#uses=1]
	br i1 %168, label %bb18, label %bb19

bb18:		; preds = %bb17
	%169 = load i64* %dim_addr, align 8		; <i64> [#uses=1]
	%170 = mul i64 %169, 8		; <i64> [#uses=1]
	%171 = load double** %y_addr, align 8		; <double*> [#uses=1]
	%172 = load double** %k1, align 8		; <double*> [#uses=1]
	%173 = bitcast double* %171 to i8*		; <i8*> [#uses=1]
	%174 = bitcast double* %172 to i8*		; <i8*> [#uses=1]
	call void @llvm.memcpy.i64(i8* %173, i8* %174, i64 %170, i32 1)
	%175 = load i32* %s, align 4		; <i32> [#uses=1]
	store i32 %175, i32* %4, align 4
	br label %bb23

bb19:		; preds = %bb17, %bb16
	store i64 0, i64* %i, align 8
	br label %bb21

bb20:		; preds = %bb21
	%176 = load double** %y_addr, align 8		; <double*> [#uses=1]
	%177 = load i64* %i, align 8		; <i64> [#uses=1]
	%178 = getelementptr double* %176, i64 %177		; <double*> [#uses=1]
	%179 = load double* %178, align 1		; <double> [#uses=1]
	%180 = load double** %y_onestep, align 8		; <double*> [#uses=1]
	%181 = load i64* %i, align 8		; <i64> [#uses=1]
	%182 = getelementptr double* %180, i64 %181		; <double*> [#uses=1]
	%183 = load double* %182, align 1		; <double> [#uses=1]
	%184 = sub double %179, %183		; <double> [#uses=1]
	%185 = mul double %184, 4.000000e+00		; <double> [#uses=1]
	%186 = fdiv double %185, 1.500000e+01		; <double> [#uses=1]
	%187 = load double** %yerr_addr, align 8		; <double*> [#uses=1]
	%188 = load i64* %i, align 8		; <i64> [#uses=1]
	%189 = getelementptr double* %187, i64 %188		; <double*> [#uses=1]
	store double %186, double* %189, align 1
	%190 = load i64* %i, align 8		; <i64> [#uses=1]
	%191 = add i64 %190, 1		; <i64> [#uses=1]
	store i64 %191, i64* %i, align 8
	br label %bb21

bb21:		; preds = %bb20, %bb19
	%192 = load i64* %i, align 8		; <i64> [#uses=1]
	%193 = load i64* %dim_addr, align 8		; <i64> [#uses=1]
	%194 = icmp ult i64 %192, %193		; <i1> [#uses=1]
	br i1 %194, label %bb20, label %bb22

bb22:		; preds = %bb21
	store i32 0, i32* %4, align 4
	br label %bb23

bb23:		; preds = %bb22, %bb18, %bb15, %bb13, %bb11, %bb9, %bb7
	%195 = load i32* %4, align 4		; <i32> [#uses=1]
	store i32 %195, i32* %retval, align 4
	br label %return

return:		; preds = %bb23
	%retval24 = load i32* %retval		; <i32> [#uses=1]
	ret i32 %retval24
}

declare void @llvm.memcpy.i64(i8* nocapture, i8* nocapture, i64, i32) nounwind

define internal i8* @_ZL9rk4_allocm(i64 %dim) {
entry:
	%dim_addr = alloca i64		; <i64*> [#uses=6]
	%retval = alloca i8*		; <i8**> [#uses=2]
	%state = alloca %struct.rk4_state_t*		; <%struct.rk4_state_t**> [#uses=28]
	%0 = alloca i8*		; <i8**> [#uses=8]
	%"alloca point" = bitcast i32 0 to i32		; <i32> [#uses=0]
	store i64 %dim, i64* %dim_addr
	%1 = call i8* @malloc(i64 40) nounwind		; <i8*> [#uses=1]
	%2 = bitcast i8* %1 to %struct.rk4_state_t*		; <%struct.rk4_state_t*> [#uses=1]
	store %struct.rk4_state_t* %2, %struct.rk4_state_t** %state, align 8
	%3 = load %struct.rk4_state_t** %state, align 8		; <%struct.rk4_state_t*> [#uses=1]
	%4 = icmp eq %struct.rk4_state_t* %3, null		; <i1> [#uses=1]
	br i1 %4, label %bb, label %bb1

bb:		; preds = %entry
	call void @gsl_error(i8* getelementptr ([39 x i8]* @.str1, i32 0, i64 0), i8* getelementptr ([6 x i8]* @.str2, i32 0, i64 0), i32 64, i32 8)
	store i8* null, i8** %0, align 8
	br label %bb12

bb1:		; preds = %entry
	%5 = load i64* %dim_addr, align 8		; <i64> [#uses=1]
	%6 = mul i64 %5, 8		; <i64> [#uses=1]
	%7 = call i8* @malloc(i64 %6) nounwind		; <i8*> [#uses=1]
	%8 = bitcast i8* %7 to double*		; <double*> [#uses=1]
	%9 = load %struct.rk4_state_t** %state, align 8		; <%struct.rk4_state_t*> [#uses=1]
	%10 = getelementptr %struct.rk4_state_t* %9, i32 0, i32 0		; <double**> [#uses=1]
	store double* %8, double** %10, align 8
	%11 = load %struct.rk4_state_t** %state, align 8		; <%struct.rk4_state_t*> [#uses=1]
	%12 = getelementptr %struct.rk4_state_t* %11, i32 0, i32 0		; <double**> [#uses=1]
	%13 = load double** %12, align 8		; <double*> [#uses=1]
	%14 = icmp eq double* %13, null		; <i1> [#uses=1]
	br i1 %14, label %bb2, label %bb3

bb2:		; preds = %bb1
	%15 = load %struct.rk4_state_t** %state, align 8		; <%struct.rk4_state_t*> [#uses=1]
	%16 = bitcast %struct.rk4_state_t* %15 to i8*		; <i8*> [#uses=1]
	call void @free(i8* %16) nounwind
	call void @gsl_error(i8* getelementptr ([31 x i8]* @.str3, i32 0, i64 0), i8* getelementptr ([6 x i8]* @.str2, i32 0, i64 0), i32 72, i32 8)
	store i8* null, i8** %0, align 8
	br label %bb12

bb3:		; preds = %bb1
	%17 = load i64* %dim_addr, align 8		; <i64> [#uses=1]
	%18 = mul i64 %17, 8		; <i64> [#uses=1]
	%19 = call i8* @malloc(i64 %18) nounwind		; <i8*> [#uses=1]
	%20 = bitcast i8* %19 to double*		; <double*> [#uses=1]
	%21 = load %struct.rk4_state_t** %state, align 8		; <%struct.rk4_state_t*> [#uses=1]
	%22 = getelementptr %struct.rk4_state_t* %21, i32 0, i32 1		; <double**> [#uses=1]
	store double* %20, double** %22, align 8
	%23 = load %struct.rk4_state_t** %state, align 8		; <%struct.rk4_state_t*> [#uses=1]
	%24 = getelementptr %struct.rk4_state_t* %23, i32 0, i32 1		; <double**> [#uses=1]
	%25 = load double** %24, align 8		; <double*> [#uses=1]
	%26 = icmp eq double* %25, null		; <i1> [#uses=1]
	br i1 %26, label %bb4, label %bb5

bb4:		; preds = %bb3
	%27 = load %struct.rk4_state_t** %state, align 8		; <%struct.rk4_state_t*> [#uses=1]
	%28 = getelementptr %struct.rk4_state_t* %27, i32 0, i32 0		; <double**> [#uses=1]
	%29 = load double** %28, align 8		; <double*> [#uses=1]
	%30 = bitcast double* %29 to i8*		; <i8*> [#uses=1]
	call void @free(i8* %30) nounwind
	%31 = load %struct.rk4_state_t** %state, align 8		; <%struct.rk4_state_t*> [#uses=1]
	%32 = bitcast %struct.rk4_state_t* %31 to i8*		; <i8*> [#uses=1]
	call void @free(i8* %32) nounwind
	call void @gsl_error(i8* getelementptr ([32 x i8]* @.str4, i32 0, i64 0), i8* getelementptr ([6 x i8]* @.str2, i32 0, i64 0), i32 81, i32 8)
	store i8* null, i8** %0, align 8
	br label %bb12

bb5:		; preds = %bb3
	%33 = load i64* %dim_addr, align 8		; <i64> [#uses=1]
	%34 = mul i64 %33, 8		; <i64> [#uses=1]
	%35 = call i8* @malloc(i64 %34) nounwind		; <i8*> [#uses=1]
	%36 = bitcast i8* %35 to double*		; <double*> [#uses=1]
	%37 = load %struct.rk4_state_t** %state, align 8		; <%struct.rk4_state_t*> [#uses=1]
	%38 = getelementptr %struct.rk4_state_t* %37, i32 0, i32 2		; <double**> [#uses=1]
	store double* %36, double** %38, align 8
	%39 = load %struct.rk4_state_t** %state, align 8		; <%struct.rk4_state_t*> [#uses=1]
	%40 = getelementptr %struct.rk4_state_t* %39, i32 0, i32 2		; <double**> [#uses=1]
	%41 = load double** %40, align 8		; <double*> [#uses=1]
	%42 = icmp eq double* %41, null		; <i1> [#uses=1]
	br i1 %42, label %bb6, label %bb7

bb6:		; preds = %bb5
	%43 = load %struct.rk4_state_t** %state, align 8		; <%struct.rk4_state_t*> [#uses=1]
	%44 = getelementptr %struct.rk4_state_t* %43, i32 0, i32 0		; <double**> [#uses=1]
	%45 = load double** %44, align 8		; <double*> [#uses=1]
	%46 = bitcast double* %45 to i8*		; <i8*> [#uses=1]
	call void @free(i8* %46) nounwind
	%47 = load %struct.rk4_state_t** %state, align 8		; <%struct.rk4_state_t*> [#uses=1]
	%48 = getelementptr %struct.rk4_state_t* %47, i32 0, i32 1		; <double**> [#uses=1]
	%49 = load double** %48, align 8		; <double*> [#uses=1]
	%50 = bitcast double* %49 to i8*		; <i8*> [#uses=1]
	call void @free(i8* %50) nounwind
	%51 = load %struct.rk4_state_t** %state, align 8		; <%struct.rk4_state_t*> [#uses=1]
	%52 = bitcast %struct.rk4_state_t* %51 to i8*		; <i8*> [#uses=1]
	call void @free(i8* %52) nounwind
	call void @gsl_error(i8* getelementptr ([32 x i8]* @.str5, i32 0, i64 0), i8* getelementptr ([6 x i8]* @.str2, i32 0, i64 0), i32 91, i32 8)
	store i8* null, i8** %0, align 8
	br label %bb12

bb7:		; preds = %bb5
	%53 = load i64* %dim_addr, align 8		; <i64> [#uses=1]
	%54 = mul i64 %53, 8		; <i64> [#uses=1]
	%55 = call i8* @malloc(i64 %54) nounwind		; <i8*> [#uses=1]
	%56 = bitcast i8* %55 to double*		; <double*> [#uses=1]
	%57 = load %struct.rk4_state_t** %state, align 8		; <%struct.rk4_state_t*> [#uses=1]
	%58 = getelementptr %struct.rk4_state_t* %57, i32 0, i32 3		; <double**> [#uses=1]
	store double* %56, double** %58, align 8
	%59 = load %struct.rk4_state_t** %state, align 8		; <%struct.rk4_state_t*> [#uses=1]
	%60 = getelementptr %struct.rk4_state_t* %59, i32 0, i32 3		; <double**> [#uses=1]
	%61 = load double** %60, align 8		; <double*> [#uses=1]
	%62 = icmp eq double* %61, null		; <i1> [#uses=1]
	br i1 %62, label %bb8, label %bb9

bb8:		; preds = %bb7
	%63 = load %struct.rk4_state_t** %state, align 8		; <%struct.rk4_state_t*> [#uses=1]
	%64 = getelementptr %struct.rk4_state_t* %63, i32 0, i32 2		; <double**> [#uses=1]
	%65 = load double** %64, align 8		; <double*> [#uses=1]
	%66 = bitcast double* %65 to i8*		; <i8*> [#uses=1]
	call void @free(i8* %66) nounwind
	%67 = load %struct.rk4_state_t** %state, align 8		; <%struct.rk4_state_t*> [#uses=1]
	%68 = getelementptr %struct.rk4_state_t* %67, i32 0, i32 0		; <double**> [#uses=1]
	%69 = load double** %68, align 8		; <double*> [#uses=1]
	%70 = bitcast double* %69 to i8*		; <i8*> [#uses=1]
	call void @free(i8* %70) nounwind
	%71 = load %struct.rk4_state_t** %state, align 8		; <%struct.rk4_state_t*> [#uses=1]
	%72 = getelementptr %struct.rk4_state_t* %71, i32 0, i32 1		; <double**> [#uses=1]
	%73 = load double** %72, align 8		; <double*> [#uses=1]
	%74 = bitcast double* %73 to i8*		; <i8*> [#uses=1]
	call void @free(i8* %74) nounwind
	%75 = load %struct.rk4_state_t** %state, align 8		; <%struct.rk4_state_t*> [#uses=1]
	%76 = bitcast %struct.rk4_state_t* %75 to i8*		; <i8*> [#uses=1]
	call void @free(i8* %76) nounwind
	call void @gsl_error(i8* getelementptr ([34 x i8]* @.str6, i32 0, i64 0), i8* getelementptr ([6 x i8]* @.str2, i32 0, i64 0), i32 102, i32 8)
	store i8* null, i8** %0, align 8
	br label %bb12

bb9:		; preds = %bb7
	%77 = load i64* %dim_addr, align 8		; <i64> [#uses=1]
	%78 = mul i64 %77, 8		; <i64> [#uses=1]
	%79 = call i8* @malloc(i64 %78) nounwind		; <i8*> [#uses=1]
	%80 = bitcast i8* %79 to double*		; <double*> [#uses=1]
	%81 = load %struct.rk4_state_t** %state, align 8		; <%struct.rk4_state_t*> [#uses=1]
	%82 = getelementptr %struct.rk4_state_t* %81, i32 0, i32 4		; <double**> [#uses=1]
	store double* %80, double** %82, align 8
	%83 = load %struct.rk4_state_t** %state, align 8		; <%struct.rk4_state_t*> [#uses=1]
	%84 = getelementptr %struct.rk4_state_t* %83, i32 0, i32 4		; <double**> [#uses=1]
	%85 = load double** %84, align 8		; <double*> [#uses=1]
	%86 = icmp eq double* %85, null		; <i1> [#uses=1]
	br i1 %86, label %bb10, label %bb11

bb10:		; preds = %bb9
	%87 = load %struct.rk4_state_t** %state, align 8		; <%struct.rk4_state_t*> [#uses=1]
	%88 = getelementptr %struct.rk4_state_t* %87, i32 0, i32 3		; <double**> [#uses=1]
	%89 = load double** %88, align 8		; <double*> [#uses=1]
	%90 = bitcast double* %89 to i8*		; <i8*> [#uses=1]
	call void @free(i8* %90) nounwind
	%91 = load %struct.rk4_state_t** %state, align 8		; <%struct.rk4_state_t*> [#uses=1]
	%92 = getelementptr %struct.rk4_state_t* %91, i32 0, i32 2		; <double**> [#uses=1]
	%93 = load double** %92, align 8		; <double*> [#uses=1]
	%94 = bitcast double* %93 to i8*		; <i8*> [#uses=1]
	call void @free(i8* %94) nounwind
	%95 = load %struct.rk4_state_t** %state, align 8		; <%struct.rk4_state_t*> [#uses=1]
	%96 = getelementptr %struct.rk4_state_t* %95, i32 0, i32 0		; <double**> [#uses=1]
	%97 = load double** %96, align 8		; <double*> [#uses=1]
	%98 = bitcast double* %97 to i8*		; <i8*> [#uses=1]
	call void @free(i8* %98) nounwind
	%99 = load %struct.rk4_state_t** %state, align 8		; <%struct.rk4_state_t*> [#uses=1]
	%100 = getelementptr %struct.rk4_state_t* %99, i32 0, i32 1		; <double**> [#uses=1]
	%101 = load double** %100, align 8		; <double*> [#uses=1]
	%102 = bitcast double* %101 to i8*		; <i8*> [#uses=1]
	call void @free(i8* %102) nounwind
	%103 = load %struct.rk4_state_t** %state, align 8		; <%struct.rk4_state_t*> [#uses=1]
	%104 = bitcast %struct.rk4_state_t* %103 to i8*		; <i8*> [#uses=1]
	call void @free(i8* %104) nounwind
	call void @gsl_error(i8* getelementptr ([34 x i8]* @.str6, i32 0, i64 0), i8* getelementptr ([6 x i8]* @.str2, i32 0, i64 0), i32 114, i32 8)
	store i8* null, i8** %0, align 8
	br label %bb12

bb11:		; preds = %bb9
	%105 = load %struct.rk4_state_t** %state, align 8		; <%struct.rk4_state_t*> [#uses=1]
	%106 = bitcast %struct.rk4_state_t* %105 to i8*		; <i8*> [#uses=1]
	store i8* %106, i8** %0, align 8
	br label %bb12

bb12:		; preds = %bb11, %bb10, %bb8, %bb6, %bb4, %bb2, %bb
	%107 = load i8** %0, align 8		; <i8*> [#uses=1]
	store i8* %107, i8** %retval, align 8
	br label %return

return:		; preds = %bb12
	%retval13 = load i8** %retval		; <i8*> [#uses=1]
	ret i8* %retval13
}

declare i8* @malloc(i64) nounwind

declare void @gsl_error(i8*, i8*, i32, i32)

define internal void @_Z41__static_initialization_and_destruction_0ii(i32 %__initialize_p, i32 %__priority) {
entry:
	%__initialize_p_addr = alloca i32		; <i32*> [#uses=2]
	%__priority_addr = alloca i32		; <i32*> [#uses=2]
	%"alloca point" = bitcast i32 0 to i32		; <i32> [#uses=0]
	store i32 %__initialize_p, i32* %__initialize_p_addr
	store i32 %__priority, i32* %__priority_addr
	%0 = load i32* %__initialize_p_addr, align 4		; <i32> [#uses=1]
	%1 = icmp eq i32 %0, 1		; <i1> [#uses=1]
	br i1 %1, label %bb, label %bb2

bb:		; preds = %entry
	%2 = load i32* %__priority_addr, align 4		; <i32> [#uses=1]
	%3 = icmp eq i32 %2, 65535		; <i1> [#uses=1]
	br i1 %3, label %bb1, label %bb2

bb1:		; preds = %bb
	call void @_ZNSt8ios_base4InitC1Ev(%"struct.std::ios_base::Init"* @_ZStL8__ioinit)
	%4 = call i32 @__cxa_atexit(void (i8*)* @__tcf_0, i8* null, i8* bitcast (i8** @__dso_handle to i8*)) nounwind		; <i32> [#uses=0]
	br label %bb2

bb2:		; preds = %bb1, %bb, %entry
	br label %return

return:		; preds = %bb2
	ret void
}

declare void @_ZNSt8ios_base4InitC1Ev(%"struct.std::ios_base::Init"*)

declare i32 @__cxa_atexit(void (i8*)*, i8*, i8*) nounwind

define internal void @__tcf_0(i8* %unnamed_arg) {
entry:
	%unnamed_arg_addr = alloca i8*		; <i8**> [#uses=1]
	%"alloca point" = bitcast i32 0 to i32		; <i32> [#uses=0]
	store i8* %unnamed_arg, i8** %unnamed_arg_addr
	call void @_ZNSt8ios_base4InitD1Ev(%"struct.std::ios_base::Init"* @_ZStL8__ioinit)
	br label %return

return:		; preds = %entry
	ret void
}

declare void @_ZNSt8ios_base4InitD1Ev(%"struct.std::ios_base::Init"*)

declare extern_weak i32 @pthread_once(i32*, void ()*)

declare extern_weak i8* @pthread_getspecific(i32)

declare extern_weak i32 @pthread_setspecific(i32, i8*)

declare extern_weak i32 @pthread_create(i64*, %struct.pthread_attr_t*, i8* (i8*)*, i8*)

declare extern_weak i32 @pthread_cancel(i64)

declare extern_weak i32 @pthread_mutex_lock(%struct.pthread_mutex_t*)

declare extern_weak i32 @pthread_mutex_trylock(%struct.pthread_mutex_t*)

declare extern_weak i32 @pthread_mutex_unlock(%struct.pthread_mutex_t*)

declare extern_weak i32 @pthread_mutex_init(%struct.pthread_mutex_t*, %struct.pthread_mutexattr_t*)

declare extern_weak i32 @pthread_key_create(i32*, void (i8*)*)

declare extern_weak i32 @pthread_key_delete(i32)

declare extern_weak i32 @pthread_mutexattr_init(%struct.pthread_mutexattr_t*)

declare extern_weak i32 @pthread_mutexattr_settype(%struct.pthread_mutexattr_t*, i32)

declare extern_weak i32 @pthread_mutexattr_destroy(%struct.pthread_mutexattr_t*)
