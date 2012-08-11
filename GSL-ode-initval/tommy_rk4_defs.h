#define noinline __attribute__((noinline))

const double* trick_me(const double*, int) noinline;
const double* trick_me(const double* in, int delta) {
	const double* ret = in + delta; return ret;
}

const int trick_me_int(const int, int) noinline;
const int trick_me_int(const int in, int delta) {
	return in + delta;
}


