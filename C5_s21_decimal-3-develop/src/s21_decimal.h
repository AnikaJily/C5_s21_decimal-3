#ifndef S21_DECIMAL
#define S21_DECIMAL

#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// количество битов большого массива
#define BITS_BIG 255
#define BITS_S21 95
#define BITS_B 8

#define S21_OK 0
#define S21_ERROR 1
#define S21_ERROR_TOO_BIG 1
#define S21_ERROR_TOO_SMALL 2
#define S21_ERROR_ZERO_DIVISION 3

#define S21_FALSE 0
#define S21_TRUE 1
#define S21_MINUS 0b10000000000000000000000000000000
#define S21_PLUS 0b00000000000000000000000000000000

typedef struct {
  unsigned int bits[4];
} s21_decimal;

typedef struct {
  unsigned int bits[BITS_B];  // мантисса
  int exponenta;              // экспонента
  int negative;               // отрицательное ли число
  int zero_left;              // количество нулей слева
  int one_position_left;      // старший бит
  int one_right;              // первая единица справа
} big_decimal;

// ARITHMETIC //

int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal* result);
int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal* result);
int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal* result);
int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal* result);

// COMPARISON //

int s21_is_less(s21_decimal val1, s21_decimal val2);
int s21_is_greater(s21_decimal val1, s21_decimal val2);
int s21_is_less_or_equal(s21_decimal val1, s21_decimal val2);
int s21_is_greater_or_equal(s21_decimal val1, s21_decimal val2);
int s21_is_equal(s21_decimal val1, s21_decimal val2);
int s21_is_not_equal(s21_decimal val1, s21_decimal val2);

// CONVERTOR //

int s21_from_int_to_decimal(int src, s21_decimal* dst);
int s21_from_float_to_decimal(float src, s21_decimal* dst);
int s21_from_decimal_to_int(s21_decimal src, int* dst);
int s21_from_decimal_to_float(s21_decimal src, float* dst);

// ANOTHER //

int s21_negate(s21_decimal value, s21_decimal* result);
int s21_truncate(s21_decimal value, s21_decimal* result);
int s21_round(s21_decimal value, s21_decimal* result);
int s21_floor(s21_decimal value, s21_decimal* result);

// HELP FUNCTIONS //

int s21_get_decimal_scale(s21_decimal num);
int s21_set_decimal_scale(s21_decimal* num, int scale);
int s21_get_decimal_bit(s21_decimal num, int cur_bit);
int s21_get_decimal_bit_big(big_decimal* num, int cur_bit);
void s21_set_decimal_bit(s21_decimal* num, int bit, int result);
void s21_set_decimal_bit_big(big_decimal* num, int bit, int result);
void s21_set_minus(s21_decimal* num, int sign);
int s21_get_sign(s21_decimal num);
void s21_normalize_big(big_decimal* bvalue_1, big_decimal* bvalue_2);
int s21_compare_mantis_big(big_decimal* bvalue1, big_decimal* bvalue2);
int s21_big_to_s21decimal(s21_decimal* result, big_decimal* result_big);
void s21_big_to_s21decimal_95(big_decimal* result_big, s21_decimal* result);
int s21_multiply_mantis_big(big_decimal bvalue_1, big_decimal* bvalue_2,
                            big_decimal* result);
void s21_zero_mantisa_big(big_decimal* result);
int s21_comparing(s21_decimal* x1, s21_decimal* x2);
void s21_bank_round(big_decimal copy_result_big_x, big_decimal* result_big,
                    int exp_diff);
void s21_get_float_part(char* part, int* scale, int* length, int* decimal_part);
void s21_print_big_decimal(big_decimal* num);
void s21_division(big_decimal val1, big_decimal val2, big_decimal* res);
unsigned int division_with_rest_for10(big_decimal val1, big_decimal* res);

void zero_big_decimal(big_decimal* result);
int multiply_10_mantis_big(big_decimal* bvalue, int def);
int sum_mantissa(big_decimal* bvalue_1, big_decimal* bvalue_2,
                 big_decimal* result);
void sub_mantis_big(big_decimal value_1, big_decimal value_2,
                    big_decimal* result);

void zeroes_left_big(big_decimal* bvalue);
int shift_left_big(big_decimal* bvalue, int def);
void init_big(s21_decimal value, big_decimal* big);
int is_greater_big_decimal(big_decimal value_1, big_decimal value_2);
int div_big(big_decimal value_1, big_decimal value_2, big_decimal* result);
void div_10_big_decimal(big_decimal* dst, int n);
int preparation_big_decimal(big_decimal* result, int scale);
int is_zero_big_decimal(big_decimal big);
int is_greater_or_equal_big_decimal(big_decimal value_1, big_decimal value_2);

int go_beyond_big_decimal_s21(big_decimal* big);
int big_to_s21decimal(s21_decimal* result, big_decimal* result_big);
int is_equal_big_decimal(big_decimal* value_1, big_decimal* value_2);

int zeroes_left(s21_decimal value);
void zero_s21_decimal(s21_decimal* value);
void full_decimal(s21_decimal* num);
int is_zero_s21_decimal(s21_decimal value);
void change_znak_s21(s21_decimal* value);

#endif  // S21_DECIMAL