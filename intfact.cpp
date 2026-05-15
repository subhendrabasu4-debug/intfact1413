#include <iostream>
#include <math.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm>
#include <boost/lexical_cast.hpp>
#include <gmp.h>
#include <acb.h>
#include <acb_dirichlet.h>
#include <flint/fmpz.h>
#include <vector>
#define PREC 4096
#define OFFSET 2
using namespace std;
using namespace boost;

char* get_zero(int zero_index, int prec=PREC) {
	acb_t zeros;
	acb_init(zeros);
	fmpz_t n;
	fmpz_init_set_ui(n, zero_index);
	struct timeval start, end;
	gettimeofday(&start, NULL);
	acb_dirichlet_zeta_zeros(zeros, n, 1, prec);
	gettimeofday(&end, NULL);
	double time_taken = (end.tv_sec-start.tv_sec) + (end.tv_usec-start.tv_usec) / 1e6;
	//	printf("Zero generation in %f seconds\n", time_taken);
	arb_t im;
	arb_init(im);
	acb_get_imag(im, zeros);
	char* zero = strdup(arb_get_str((const arb_struct*)im, prec, 0));
	char* ptr = strstr(zero, "+/-");
	*ptr = '\0';
	ptr = strchr(zero,'.');
	zero = ptr + 1;
	acb_clear(zeros);
	fmpz_clear(n);
	arb_clear(im);
	return zero;
}

char* exp(int base, int exp) {
	mpz_t res;
	mpz_init(res);
	mpz_ui_pow_ui(res, base, exp);
	char* ret = strdup(mpz_get_str(0, 10, res));
	return ret;
}

char* subtract(char* x, char* y) {
	mpz_t a;
	mpz_init(a);
	mpz_set_str(a, x, 10);
	mpz_t b;
	mpz_init(b);
	mpz_set_str(b, y, 10);
	mpz_t c;
	mpz_init(c);
	mpz_sub(c, a, b);
	char* ret = strdup(mpz_get_str(0, 10, c));
	return ret;
}

int main(int argc, char* argv[]) {
	char* num = strdup(argv[1]);
	unsigned long long int l = strlen(num);
	unsigned long long int x = 0;
	unsigned long int zero_index = 1;
	while (x < l) {
		unsigned long long int y = 0;
		char* zero = get_zero(zero_index, PREC);
		unsigned long int ctr = 0;
		while (1) {
			char zz = zero[ctr];
			char nn = num[ctr % l];
			if (nn == zz) {
				y = ctr % l;
				break;
			}
			++ctr;
		}
		cout << "Hit @Pos1: " << y << endl;
		if (x == y) {
			++x;
			cout << "\t\tHit @Pos1: " << y << endl;
		} else {
			cout << "Hit @Pos1: " << y << endl;
		}
		++zero_index;
	}
	char* pow_num = exp(10, l);
	cout << pow_num << endl;
	char* residue = subtract(pow_num, num);
	cout << residue << endl;
	x = l - 1;
	zero_index = 1;
	unsigned long int ntimes = 0;
	while (ntimes < l) {
		unsigned long long int y = 0;
		char* zero = get_zero(zero_index, PREC);
		unsigned long int ctr = 0;
		while (1) {
			char zz = zero[ctr];
			char nn = residue[ctr % l];
			if (nn == zz) {
				y = ctr % l;
				break;
			}
			++ctr;
		}
		if (x == y) {
			--x;
			++ntimes;
			cout << "\t\tHit @Pos2: " << y << endl;
		} else {
			cout << "Hit @Pos2: " << y << endl;
		}

		++zero_index;
	}
	return 0;
}
