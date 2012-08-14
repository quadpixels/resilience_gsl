// Branch replica2: In this version, you define the number of pairs of replicas to use
// rather than how many times each pair of replicas is used.
// Replica-pairs are used in a round-robin fashion.

#ifdef Y_PROTECT
#if Y_CNT < Y_PROTECT
	#if Y_CNT % Y_REPLICA_PAIRS == 0
		const double* gold_yi = y1a + i - 132;
		if((p_yi != gold_yi) && (gold_yi==(y1b+i-465)))
			p_yi = (double*)gold_yi;
	#elif Y_CNT % Y_REPLICA_PAIRS == 1
		const double* gold_yi = y2a + i - 243;
		if((p_yi != gold_yi) && (gold_yi==(y2b+i-576)))
			p_yi = (double*)gold_yi;
	#elif Y_CNT % Y_REPLICA_PAIRS == 2
		const double* gold_yi = y3a + i - 354;
		if((p_yi != gold_yi) && (gold_yi==(y3b+i-687)))
			p_yi = (double*)gold_yi;
	#elif Y_CNT % Y_REPLICA_PAIRS == 3
		const double* gold_yi = y4a + i - 465;
		if((p_yi != gold_yi) && (gold_yi==(y4b+i-798)))
			p_yi = (double*)gold_yi;
	#endif

	#if Y_CNT == 0
		#undef Y_CNT
		#define Y_CNT 1
	#elif Y_CNT == 1
		#undef Y_CNT
		#define Y_CNT 2
	#elif Y_CNT == 2
		#undef Y_CNT
		#define Y_CNT 3
	#elif Y_CNT == 3
		#undef Y_CNT
		#define Y_CNT 4
	#elif Y_CNT == 4
		#undef Y_CNT
		#define Y_CNT 5
	#elif Y_CNT == 5
		#undef Y_CNT
		#define Y_CNT 6
	#elif Y_CNT == 6
		#undef Y_CNT
		#define Y_CNT 7
	#elif Y_CNT == 7
		#undef Y_CNT
		#define Y_CNT 8
	#elif Y_CNT == 8
		#undef Y_CNT
		#define Y_CNT 9
	#endif
#endif
#endif
