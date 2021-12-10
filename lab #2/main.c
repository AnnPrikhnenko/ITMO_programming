#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#define MAX_NUM_OF_DIGIT 310
#define NUM_OF_INT 32

typedef struct {
	unsigned int bytesx4[NUM_OF_INT];     // 5 = [5][][]....[] - 32 
} uint1024_t;

// unsigned int == 4 bytes [0 ... 2**32 - 1]
uint1024_t from_uint(unsigned int x) {
	uint1024_t result = { 0 };
	result.bytesx4[0] = x;
	return result;
}

void zero_uint(uint1024_t *x) {
	int i;
	for (i = 0; i < NUM_OF_INT; i++) {
		x->bytesx4[i] = 0;
	}
}

uint1024_t add_op(uint1024_t x, uint1024_t y) {
	uint64_t sum = 0;
	uint1024_t result = { 0 };
	int i;
	int p = 0;                                           // bool - true if perepolnenie

	for (i = 0; i < NUM_OF_INT; i++) {
		sum = (uint64_t)x.bytesx4[i] + y.bytesx4[i] + p;

		if (sum <= UINT32_MAX) {
			p = 0;
			result.bytesx4[i] = sum;
		} else {
			p = 1;
			result.bytesx4[i] = sum & UINT32_MAX;       // only last 32 bits
		}
	}

	return result;
}

// result = x - y
uint1024_t subtr_op(uint1024_t x, uint1024_t y) {
	int64_t sub = 0;
	uint1024_t result = { 0 };
	int i;
	int n = 0;                                          // = 1 if previous need dop 

	for (i = 0; i < NUM_OF_INT; i++) {
		sub = (int64_t)x.bytesx4[i] - y.bytesx4[i] - n;

		if (sub >= 0) {
			n = 0;
			result.bytesx4[i] = sub;
		} else if (i != 31) {
			n = 1;
			result.bytesx4[i] = UINT32_MAX + 1 + sub;
		} else {                                       // i = 31, x < y 
			printf("Unsigned int 1024 can not be < 0");
		}
	}

	return result;
}

uint1024_t mult_uint_and_int(uint1024_t x, int y) {
	uint1024_t result = { 0 };
	int i;
	for (i = 0; i < y; i++) {
		result = add_op(result, x);
	}
	return result;
}

void move_to_right(uint1024_t* x) {
	int i;
	int temp;
	bool c = 0;
	for (i = 0; i < NUM_OF_INT; i++) {
		temp = x->bytesx4[i];
		x->bytesx4[i] = (x->bytesx4[i] << 1) + c;
		c = temp & (1 << 31); 
	}
}

// result = x * y
uint1024_t mult_op(uint1024_t x, uint1024_t y) {
	uint1024_t result = { 0 };
	int i, j;
	int k;
	int current_int_of_x;
	int bits_number;
	bool cur_bit_of_x;

	for (i = 0; i < NUM_OF_INT; i++) {
		current_int_of_x = x.bytesx4[i];

		for (j = 0; j < NUM_OF_INT; j++) {
			cur_bit_of_x = current_int_of_x & 1;

			if (!cur_bit_of_x) {
				current_int_of_x >>= 1;        // delete 1 bit from the end
				continue;
			}
			bits_number = 32 * i + j;

			uint1024_t buffer = y;
			for (k = 0; k < bits_number; k++) {
				move_to_right(&buffer);
			}
			result = add_op(result, buffer);

			current_int_of_x >>= 1;            // delete 1 bit from the end
		}
	}
	return result;
}

void printf_value(uint1024_t x) {
	int i;
	bool was_dig = false;

	for (i = NUM_OF_INT - 1; i >= 0; i--) {
		if (x.bytesx4[i] == 0 && !was_dig) {
			continue;
		}
		was_dig = true;
		printf("%x", x.bytesx4[i]);
	}
}

void scanf_value(uint1024_t* x) {
	char console_string[MAX_NUM_OF_DIGIT];
	int len;
	int i;

	memset(console_string, 0, MAX_NUM_OF_DIGIT);
	scanf("%s", &console_string);
	// gets(console_string);
	len = strlen(console_string);

	if (len > MAX_NUM_OF_DIGIT) {
		printf("You can not input so big number");
	} else {
		zero_uint(x);
		for (i = 0; i < len; i++) {
			char one_element = console_string[i];
			*x = add_op(from_uint(one_element - '0'), mult_uint_and_int(*x, 10));
		}
	}
}
 
// UINSIGNED INT - 1024 bits
int main() {
	uint1024_t x, y, z, w;

	x = from_uint(100);
	printf("x = ");
	printf_value(x);
	printf("\n");

	y = from_uint(1);
	printf("y = ");
	printf_value(y);
	printf("\n");

	z = subtr_op(x, y);
	printf("x - y = ");
	printf_value(z);
	printf("\n");

	w = from_uint(0);
	w = subtr_op(z, x);
	printf("x + y - x = y = ");
	printf_value(w);
	printf("\n");

	uint1024_t a1, a2, a3;
	a1 = from_uint(1000000000);
	a2 = from_uint(8);
	a3 = mult_op(a1, a2);
	printf("a3 = ");
	printf_value(a3);
	printf("\n");
	/*
	uint1024_t s;
	scanf_value(&s);
	printf("\n You have input: ");
	printf_value(s);
	printf("\n");
	*/
	return 0;
}
