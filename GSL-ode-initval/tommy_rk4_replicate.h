// Branch replica2: In this version, you define the number of pairs of replicas to use
// rather than how many times each pair of replicas is used.
// Replica-pairs are used in a round-robin fashion.

// Defines replicas of pointers
// Replicate 'k'
#ifdef K_PROTECT
#ifdef K_REPLICA_PAIRS
#if K_REPLICA_PAIRS > 0 && K_PROTECT > 0
	#define K_CNT 0
	#if K_REPLICA_PAIRS >= 1
		const double* k1a = trick_me(k, 123);
		const double* k1b = trick_me(k, 456);
	#endif
	#if K_REPLICA_PAIRS >= 2
		const double* k2a = trick_me(k, 234);
		const double* k2b = trick_me(k, 567);
	#endif
	#if K_REPLICA_PAIRS >= 3
		const double* k3a = trick_me(k, 345);
		const double* k3b = trick_me(k, 678);
	#endif
	#if K_REPLICA_PAIRS >= 4
		const double* k4a = trick_me(k, 456);
		const double* k4b = trick_me(k, 789);
	#endif
	#if K_REPLICA_PAIRS >= 5
		const double* k5a = trick_me(k, 567);
		const double* k5b = trick_me(k, 900);
	#endif
	#if K_REPLICA_PAIRS >= 6
		const double* k6a = trick_me(k, 678);
		const double* k6b = trick_me(k, 1011);
	#endif
	#if K_REPLICA_PAIRS >= 7
		const double* k7a = trick_me(k, 789);
		const double* k7b = trick_me(k, 1122);
	#endif
#endif
#endif
#endif

// Replicate 'y'
#ifdef Y_PROTECT
#ifdef Y_REPLICA_PAIRS
#if Y_PROTECT > 0 && Y_REPLICA_PAIRS > 0
	#define Y_CNT 0
	#if Y_REPLICA_PAIRS >= 1
		const double* y1a = trick_me(y, 132);
		const double* y1b = trick_me(y, 465);
	#endif
	#if Y_REPLICA_PAIRS >= 2
		const double* y2a = trick_me(y, 243);
		const double* y2b = trick_me(y, 576);
	#endif
	#if Y_REPLICA_PAIRS >= 3
		const double* y3a = trick_me(y, 354);
		const double* y3b = trick_me(y, 687);
	#endif
	#if Y_REPLICA_PAIRS >= 4
		const double* y4a = trick_me(y, 465);
		const double* y4b = trick_me(y, 798);
	#endif
#endif
#endif
#endif

// Replicate 'y0'
#ifdef Y0_PROTECT
#ifdef Y0_REPLICA_PAIRS
#if Y0_PROTECT > 0 && Y0_REPLICA_PAIRS > 0
	#define Y0_CNT 0
	#if Y0_REPLICA_PAIRS >= 1
		const double* y01a = trick_me(y0, 133);
		const double* y01b = trick_me(y0, 466);
	#endif
	#if Y0_REPLICA_PAIRS >= 2
		const double* y02a = trick_me(y0, 244);
		const double* y02b = trick_me(y0, 577);
	#endif
	#if Y0_REPLICA_PAIRS >= 3 
		const double* y03a = trick_me(y0, 355);
		const double* y03b = trick_me(y0, 688);
	#endif
	#if Y0_REPLICA_PAIRS >= 4
		const double* y04a = trick_me(y0, 466);
		const double* y04b = trick_me(y0, 799);
	#endif
#endif
#endif
#endif

// Replicate 'ytmp'
#ifdef YTMP_PROTECT
#ifdef YTMP_REPLICA_PAIRS
#if YTMP_PROTECT > 0 && YTMP_REPLICA_PAIRS > 0
	#define YTMP_CNT 0
	#if YTMP_REPLICA_PAIRS >= 1
		const double* ytmp1a = trick_me(ytmp, 224);
		const double* ytmp1b = trick_me(ytmp, 557);
	#endif
	#if YTMP_REPLICA_PAIRS >= 2
		const double* ytmp2a = trick_me(ytmp, 335);
		const double* ytmp2b = trick_me(ytmp, 668);
	#endif
	#if YTMP_REPLICA_PAIRS >= 3
		const double* ytmp3a = trick_me(ytmp, 446);
		const double* ytmp3b = trick_me(ytmp, 779);
	#endif
	#if YTMP_REPLICA_PAIRS >= 4
		const double* ytmp4a = trick_me(ytmp, 557);
		const double* ytmp4b = trick_me(ytmp, 890);
	#endif
	#if YTMP_REPLICA_PAIRS >= 5
		const double* ytmp5a = trick_me(ytmp, 668);
		const double* ytmp5b = trick_me(ytmp, 1001);
	#endif
	#if YTMP_REPLICA_PAIRS >= 6
		const double* ytmp6a = trick_me(ytmp, 779);
		const double* ytmp6b = trick_me(ytmp, 1112);
	#endif
#endif
#endif
#endif
