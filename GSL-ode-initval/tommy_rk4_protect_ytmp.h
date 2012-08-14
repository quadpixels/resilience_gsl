// Branch replica2: In this version, you define the number of pairs of replicas to use
// rather than how many times each pair of replicas is used.
// Replica-pairs are used in a round-robin fashion.

#ifdef YTMP_PROTECT
#if YTMP_CNT < YTMP_PROTECT
	#ifndef THIS_IS_FN_EVAL_YTMP
		#if YTMP_CNT % YTMP_REPLICA_PAIRS == 0
			const double* gold_ytmpi = ytmp1a + i - 224;
			if((p_ytmpi != gold_ytmpi) && (gold_ytmpi==(ytmp1b+i-557)))
				p_ytmpi = (double*)gold_ytmpi;
		#elif YTMP_CNT % YTMP_REPLICA_PAIRS == 1
			const double* gold_ytmpi = ytmp2a + i - 335;
			if((p_ytmpi != gold_ytmpi) && (gold_ytmpi==(ytmp2b+i-668)))
				p_ytmpi = (double*)gold_ytmpi;
		#elif YTMP_CNT % YTMP_REPLICA_PAIRS == 2
			const double* gold_ytmpi = ytmp3a + i - 446;
			if((p_ytmpi != gold_ytmpi) && (gold_ytmpi==(ytmp3b+i-779)))
				p_ytmpi = (double*)gold_ytmpi;
		#elif YTMP_CNT % YTMP_REPLICA_PAIRS == 3
			const double* gold_ytmpi = ytmp4a + i - 557;
			if((p_ytmpi != gold_ytmpi) && (gold_ytmpi==(ytmp4b+i-890)))
				p_ytmpi = (double*)gold_ytmpi;
		#elif YTMP_CNT % YTMP_REPLICA_PAIRS == 4
			const double* gold_ytmpi = ytmp5a + i - 668;
			if((p_ytmpi != gold_ytmpi) && (gold_ytmpi==(ytmp5b+i-1001)))
				p_ytmpi = (double*)gold_ytmpi;
		#elif YTMP_CNT % YTMP_REPLICA_PAIRS == 5
			const double* gold_ytmpi = ytmp6a + i - 779;
			if((p_ytmpi != gold_ytmpi) && (gold_ytmpi==(ytmp6b+i-1112)))
				p_ytmpi = (double*)gold_ytmpi;
		#endif
	#else
		#if YTMP_CNT % YTMP_REPLICA_PAIRS == 0
			if((ytmp != ytmp1a-224) && ((ytmp1a-224)==(ytmp1b-557)))
				ytmp = (double*)ytmp1a - 224;
		#elif YTMP_CNT % YTMP_REPLICA_PAIRS == 1
			if((ytmp != ytmp2a-335) && ((ytmp2a-335)==(ytmp2b-668)))
				ytmp = (double*)ytmp2a - 335;
		#elif YTMP_CNT % YTMP_REPLICA_PAIRS == 2
			if((ytmp != ytmp3a-446) && ((ytmp3a-446)==(ytmp3a-779)))
				ytmp = (double*)ytmp3a - 446;
		#elif YTMP_CNT % YTMP_REPLICA_PAIRS == 3
			if((ytmp != ytmp4a-557) && ((ytmp4a-557)==(ytmp4b-890)))
				ytmp = (double*)ytmp4a - 557;
		#elif YTMP_CNT % YTMP_REPLICA_PAIRS == 4
			if((ytmp != ytmp5a-668) && ((ytmp5a-668)==(ytmp5b-1001)))
				ytmp = (double*)ytmp5a - 668;
		#elif YTMP_CNT % YTMP_REPLICA_PAIRS == 5
			if((ytmp != ytmp6a-779) && ((ytmp6a-779)==(ytmp6b-1112)))
				ytmp = (double*)ytmp6a - 779;
		#endif
		#undef THIS_IS_FN_EVAL_YTMP
	#endif
	
	#if YTMP_CNT == 0
		#undef YTMP_CNT
		#define YTMP_CNT 1
	#elif YTMP_CNT == 1
		#undef YTMP_CNT
		#define YTMP_CNT 2
	#elif YTMP_CNT == 2
		#undef YTMP_CNT
		#define YTMP_CNT 3
	#elif YTMP_CNT == 3
		#undef YTMP_CNT
		#define YTMP_CNT 4
	#elif YTMP_CNT == 4
		#undef YTMP_CNT
		#define YTMP_CNT 5
	#elif YTMP_CNT == 5
		#undef YTMP_CNT
		#define YTMP_CNT 6
	#elif YTMP_CNT == 6
		#undef YTMP_CNT
		#define YTMP_CNT 7
	#endif
#endif
#endif
