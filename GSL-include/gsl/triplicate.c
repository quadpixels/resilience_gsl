#include "triplicate.h"

// Use this to stop the compiler from removing sigsetjmp!

noinline void trick_me_jr(int jmpret) {
	if(jmpret == 999) printf("Jack is in the box!\n");
}
noinline
unsigned long trick_me_ptr(unsigned long ptr, int delta) {
	return ptr + delta;
}
noinline
size_t trick_me_size_t(size_t in, size_t delta) {
	return in + delta;
}
noinline
unsigned int GetMatrixChecksum(const gsl_matrix* m) {
	const char* mm = (const char*)m;
	unsigned int ret = 0;
	int i; for(i=0; i<sizeof(gsl_matrix); i++) {
		ret += *(mm + i);
	}
	return ret;
}

