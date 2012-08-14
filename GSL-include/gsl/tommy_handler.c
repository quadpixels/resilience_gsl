#include "tommy.h"

// Used for sigmentation fault handler and long jmp.

#ifdef __cplusplus
extern "C" {
#endif

jmp_buf buf;
int _count = 0;
int _countmax = 20;

void MY_SET_SIGSEGV_HANDLER() {
	printf(" >> My SIGSEGV Handler Set\n");
	struct sigaction sa;
	memset(&sa, 0, sizeof(struct sigaction));
	sa.sa_handler = my_action;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGSEGV, &sa, NULL);
	sigaction(SIGBUS,  &sa, NULL);
}

void MY_REMOVE_SIGSEGV_HANDLER() {
	printf(" >> Unsetted SIGSEGV handler.\n");
	struct sigaction sa;
	memset(&sa, 0, sizeof(struct sigaction));
	sa.sa_handler = SIG_DFL;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGSEGV, &sa, NULL);
	sigaction(SIGBUS,  &sa, NULL);
	printf(" >> Unsetted.\n");
}

void my_action(int sig) {
	my_stopwatch_checkpoint(5);
	_count = _count + 1;
	printf(" >> Caught SIGSEGV signal (%d out of %d allowed)\n", _count, NUM_OF_SIGSEGV);
	
	/* Backtrace stuff */
	void* array[10];
	char** strings;
	size_t size;
	size = backtrace(array, 10);
	
	printf(" >> Stack contents (level=%d):\n", size);
	// Either use backtrace_symbols_fd or backtrace_symbols
	backtrace_symbols_fd(array, size, 2);
#ifdef UNWIND
	printf(" >> Stack contents by libunwind\n");
	do_backtrace();
	printf("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n");
#endif
	if(_count >= NUM_OF_SIGSEGV) { abort(); }
	else { 
		my_stopwatch_stop(5);
		siglongjmp(buf, 1); 
	}
	printf(" >> Execution not expected here.\n");
	abort();
}

#ifdef UNWIND
#include "libunwind.h"
void do_backtrace() {
	unw_cursor_t cursor;
	unw_context_t context;
	int ret;
	ret = unw_getcontext(&context);
	if(ret != 0) { printf("[do_backtrace] unw_getcontext returned %d.\n", ret); }
	ret = unw_init_local(&cursor, &context);
	if(ret != 0) { printf("[do_backtrace] unw_init_local returned %d.\n", ret); }

	do {
		unw_word_t offset, pc;
		char fname[64];
		unw_get_reg(&cursor, UNW_REG_IP, &pc);

		fname[0]='\0';
		(void) unw_get_proc_name(&cursor, fname, sizeof(fname), &offset);
		printf("%p : (%s+0x%x) [%p]\n", pc, fname, offset, pc);
	}
	while(unw_step(&cursor) > 0);
}
#endif

void MY_SIGSEGV_HANDLER_SUMMARY() {
	printf("[MySigsegvHandler] # of faults handled: %d\n", _count);
}

#ifdef __cplusplus
}
#endif

