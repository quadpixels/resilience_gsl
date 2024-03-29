// Branch replica2: In this version, you define the number of pairs of replicas to use
// rather than how many times each pair of replicas is used.
// Replica-pairs are used in a round-robin fashion.

#ifdef Y0_PROTECT
#if Y0_CNT < Y0_PROTECT
	#if Y0_CNT % Y0_REPLICA_PAIRS == 0
		const double* gold_y0i = y01a + i - 133;
		if((p_y0i != gold_y0i) && (gold_y0i==(y01b+i-466)))
			p_y0i = (double*)gold_y0i;
	#elif Y0_CNT % Y0_REPLICA_PAIRS == 1
		const double* gold_y0i = y02a + i - 244;
		if((p_y0i != gold_y0i) && (gold_y0i==(y02b+i-577)))
			p_y0i = (double*)gold_y0i;
	#elif Y0_CNT % Y0_REPLICA_PAIRS == 2
		const double* gold_y0i = y03a + i - 355;
		if((p_y0i != gold_y0i) && (gold_y0i==(y03b+i-688)))
			p_y0i = (double*)gold_y0i;
	#endif
	
	#if Y0_CNT == 0
		#undef Y0_CNT
		#define Y0_CNT 1
	#elif Y0_CNT == 1
		#undef Y0_CNT
		#define Y0_CNT 2
	#elif Y0_CNT == 2
		#undef Y0_CNT
		#define Y0_CNT 3
	#endif
#endif
#endif
