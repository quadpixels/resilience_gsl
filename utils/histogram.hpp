// 2012-08-14: Fixed an evil little cute BUG!
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>

class Histogram {
private:
	double* breaks;
	unsigned int* counts;
	int num_brk_zeroone; /* Num of breaks between zero and one */
	int num_brk_onetwo;  /* Num of breaks between one and two  */
	int num_breaks;
	static const int num_brk_beyondtwo = 10;
public:
	Histogram(int n_brk_01=49, int n_brk_12=47) {
		/* Breaks include zero, inf, -inf, NaN and +/- between 0 and 1, 1 and 2 and beyond 2 */
		int num_breaks = 4 + 2*(n_brk_01 + 1 + n_brk_12 + num_brk_beyondtwo);
		this->num_breaks = num_breaks;
		this->num_brk_zeroone = n_brk_01;
		this->num_brk_onetwo  = n_brk_12;

		int i, j;
		breaks = new double[num_breaks];
		counts = new unsigned int[num_breaks];
		for(i=0; i<num_breaks; i++) counts[i]=0;

		/* 1. First of all, ones */
		breaks[num_brk_beyondtwo + num_brk_onetwo] = -1;
		breaks[num_breaks - 1 - num_brk_beyondtwo - num_brk_onetwo] = 1;

		/* 1. Generate values between zero and one and between one and two. */
		int n = num_brk_zeroone + num_brk_onetwo + num_brk_beyondtwo + 1;
		double k = 0.75;
		int max_num_brk = (num_brk_zeroone > num_brk_onetwo) ? num_brk_zeroone : num_brk_onetwo;
		for(i=0; i<max_num_brk; i++) {
			if(i < num_brk_zeroone) {
				double k1 = 1 - k;
				breaks[num_breaks - n + i] = k1;
				breaks[n - 1 - i] = -k1;
			}
			if(i < num_brk_onetwo) {
				double k1 = 1 + k;
				breaks[num_breaks - num_brk_beyondtwo - 1 - i] = k1;
				breaks[num_brk_beyondtwo + i] = -k1;
			}
			k = k/2;
		}
		for(i=0; i<num_brk_beyondtwo; i++) {
			double k1 = pow(2, (pow(2, (i+1)) - 1)) + 1;
			breaks[num_breaks - num_brk_beyondtwo + i] = k1;
			breaks[num_brk_beyondtwo - 1 - i] = -k1;
		}
		
		/* Last, fill zero and inf and nan */
		breaks[n] = INFINITY;
		breaks[n+1] = 0;
		breaks[n+2] = -NAN;
		breaks[n+3] = NAN;
	}

	void PrintBreaks() {
		int i;
		/* Print */
		for(i=0; i<num_breaks; i++) {
			printf("%d\t%.20g\t%d\n", i, breaks[i], counts[i]);
		}	
	}

	void Print() {
		int i; for(i=0; i<num_breaks; i++) { 
			printf("%d", counts[i]);
			if(i < num_breaks-1) printf(",");
		}
		printf("\n");
	}

	void AddEntry(double gold, double coal) {
		if(isnan(gold) || isnan(coal)) { AddEntry(NAN); return; }
		if(isinf(gold) || isinf(coal)) { AddEntry(INFINITY); return; }
		if(gold == 0 && coal == 0) { AddEntry(1); return; }
		if(gold == 0 && coal != 0) { AddEntry(INFINITY); return; }
		AddEntry((coal-gold) / gold);
	}

	void AddEntry(double x) {
		const int n = num_brk_zeroone + num_brk_onetwo + num_brk_beyondtwo + 1;
		if(isnan(x)) { counts[n+3]++; return; }
		else if(isinf(x)) { counts[n+2]++; return; }
		else if(x == 0) { counts[n+1]++; return; }
		else if(x > 0) {
			int search_begin = (x>1) ? num_breaks - num_brk_beyondtwo - num_brk_onetwo - 1
			                         : num_breaks - num_brk_beyondtwo - num_brk_onetwo - 1 - num_brk_zeroone;
			int flag = 1, old_flag = 1, p;
			for(p = search_begin; p < num_breaks; p++, flag = ((x > breaks[p] || isnan(breaks[p])) ? 1 : 0)) {
				if(flag == 0 && old_flag == 1) {
					counts[p-1]++;
//					printf("%g belongs to bucket %.13e - %.13e\n.", x, breaks[p-1], breaks[p]);
					return;
				}
				old_flag = flag;
			}
			breaks[num_breaks-1]++;
//			printf("%g belongs to the last bucket.\n", x);
			return;
		} else if(x < 0) {
			int search_begin = (x<-1) ? 0 : num_brk_beyondtwo;
			int flag, old_flag = -999, p;
			flag = (x > breaks[search_begin]) ? 1: 0;
			for(p = search_begin; p < n; p++, flag = ((x > breaks[p]) ? 1 : 0)) {
				if(flag == 0 && old_flag == 1) {
					counts[p-1]++;
//					printf("%g belongs to bucket %.13e - %.13e\n.", x, breaks[p-1], breaks[p]);
					return;
				}
				old_flag = flag;
			}
			breaks[n-1]++;
//			printf("%g belongs to the first bucket.\n", x);
			return;
		}
		assert(0);
	}

	int GetNumberOfElements() {
		int ret = 0, i; for(i=0; i<num_breaks; i++) ret += counts[i];
		return ret;
	}
};
