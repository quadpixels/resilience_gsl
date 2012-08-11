// Branch replica2: In this version, you define the number of pairs of replicas to use
// rather than how many times each pair of replicas is used.
// Replica-pairs are used in a round-robin fashion.

#ifdef K_PROTECT
#if K_CNT < K_PROTECT
	#ifndef THIS_IS_FN_EVAL_K
	/* Different contexts. Refer to rk4.c */
		#if K_CNT % K_REPLICA_PAIRS == 0
			// Use the first replica.
			const double* gold_ki = k1a + i - 123;
			if((p_ki != gold_ki) && (gold_ki==(k1b+i-456)))
				p_ki = (double*)gold_ki;
		#elif K_CNT % K_REPLICA_PAIRS == 1
			// Use the second replica.
			const double* gold_ki = k2a + i - 234;
			if((p_ki != gold_ki) && (gold_ki==(k2b+i-567)))
				p_ki = (double*)gold_ki;
		#elif K_CNT % K_REPLICA_PAIRS == 2
			// Use the third replica.
			const double* gold_ki = k3a + i - 345;
			if((p_ki != gold_ki) && (gold_ki==(k3b+i-678)))
				p_ki = (double*)gold_ki;
		#elif K_CNT % K_REPLICA_PAIRS == 3
			// Use the fourth replica.
			const double* gold_ki = k4a + i - 456;
			if((p_ki != gold_ki) && (gold_ki==(k4b+i-789)))
				p_ki = (double*)gold_ki;
		#elif K_CNT % K_REPLICA_PAIRS == 4
			// Use the fifth replica.
			const double* gold_ki = k5a + i - 567;
			if((p_ki != gold_ki) && (gold_ki==(k5b+i-900)))
				p_ki = (double*)gold_ki;
		#elif K_CNT % K_REPLICA_PAIRS == 5
			// Use the sixth replica.
			const double* gold_ki = k6a + i - 678;
			if((p_ki != gold_ki) && (gold_ki==(k6b+i-1011)))
				p_ki = (double*)gold_ki;
		#elif K_CNT % K_REPLICA_PAIRS == 6
			// Use the seventh replica.
			const double* gold_ki = k7a + i - 789;
			if((p_ki != gold_ki) && (gold_ki==(k7b+i-1122)))
				p_ki = (double*)gold_ki;
		#endif
	#else
		#if K_CNT % K_REPLICA_PAIRS == 0
			if((k!=k1a-123) && ((k1a-123)==(k1b-456)))
				k=(double*)(k1a)-123;
		#elif K_CNT % K_REPLICA_PAIRS == 1
			if((k!=k2a-234) && ((k2a-234)==(k2b-567)))
				k=(double*)(k2a)-234;
		#elif K_CNT % K_REPLICA_PAIRS == 2
			if((k!=k3a-345) && ((k3a-345)==(k3b-678)))
				k=(double*)(k3a)-345;
		#elif K_CNT % K_REPLICA_PAIRS == 3
			if((k!=k4a-456) && ((k4a-456)==(k4b-789)))
				k=(double*)(k4a)-456;
		#elif K_CNT % K_REPLICA_PAIRS == 4
			if((k!=k5a-567) && ((k5a-567)==(k5b-900)))
				k=(double*)(k5a)-567;
		#elif K_CNT % K_REPLICA_PAIRS == 5
			if((k!=k6a-678) && ((k6a-678)==(k6b-1011)))
				k=(double*)(k6a)-678;
		#elif K_CNT % K_REPLICA_PAIRS == 6
			if((k!=k7a-789) && ((k7a-789)==(k7b-1122)))
				k=(double*)(k7a)-789;
		#endif
		#undef THIS_IS_FN_EVAL_K
	#endif

	#if K_CNT == 0
		#undef K_CNT
		#define K_CNT 1
	#elif K_CNT == 1
		#undef K_CNT
		#define K_CNT 2
	#elif K_CNT == 2
		#undef K_CNT
		#define K_CNT 3
	#elif K_CNT == 3
		#undef K_CNT
		#define K_CNT 4
	#elif K_CNT == 4
		#undef K_CNT
		#define K_CNT 5
	#elif K_CNT == 5
		#undef K_CNT
		#define K_CNT 6
	#elif K_CNT == 6
		#undef K_CNT
		#define K_CNT 7
	#endif
		
#endif
#endif
