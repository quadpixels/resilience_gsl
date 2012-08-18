#include <stdio.h>

__attribute__((noinline)) void foo() {
	int i=0, a;
	for(i=0; i<10000000; i++)
	a=i*i;
}

__attribute__((noinline)) void bar() {
	int i=0, a;
	for(i=0; i<40000000; i++) a=i*i;
}

int main(int argc, char** argv) {
	foo();
	bar();
}
