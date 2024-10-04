#include <check.h>
#include <limits.h>

#include "../s21_decimal.h"

/// Comparison tests ///

START_TEST(s21_is_equal1) {
  s21_decimal num1 = {{4, 3, 2, 0}};
  s21_decimal num2 = {{1, 3, 4, 0}};

  ck_assert_int_eq(s21_is_equal(num1, num2), 0);
}
END_TEST

START_TEST(s21_is_equal2) {
  s21_decimal num1 = {{0, 0, 1, (unsigned int)-65536}};
  s21_decimal num2 = {{0, 0, 1, 65536}};

  ck_assert_int_eq(s21_is_equal(num1, num2), 0);
}
END_TEST

START_TEST(s21_is_equal4) {
  s21_decimal num1 = {{777, 777, 777, 0}};
  s21_decimal num2 = {{777, 777, 777, 0}};
  s21_decimal num3 = {{4294967, 4294967, 4294967, 0}};
  s21_decimal num4 = {{4294967, 4294967, 4294967, 0}};

  ck_assert_int_eq(s21_is_equal(num1, num2), 1);
  ck_assert_int_eq(s21_is_equal(num1, num2), 1);
  ck_assert_int_eq(s21_is_equal(num3, num3), 1);
  ck_assert_int_eq(s21_is_equal(num3, num4), 1);
}
END_TEST

START_TEST(s21_is_less1) {
  s21_decimal num1 = {{0, 0, 0, 0}};
  s21_decimal num2 = {{0, 0, 0, 0}};

  ck_assert_int_eq(s21_is_less(num1, num2), 0);
}
END_TEST

START_TEST(s21_is_less2) {
  s21_decimal num1 = {{0, 0, 0, (unsigned int)-2147483648}};
  s21_decimal num2 = {{0, 0, 0, 0}};

  ck_assert_int_eq(s21_is_less(num1, num2), 0);
}
END_TEST

START_TEST(s21_is_less3) {
  s21_decimal num1 = {{123, 0, 1, 1}};
  s21_decimal num2 = {{123, 0, 0, 1}};

  ck_assert_int_eq(s21_is_less(num1, num2), 0);
}
END_TEST

START_TEST(s21_is_less4) {
  s21_decimal num1 = {{0, 0, 1, 0}};
  s21_decimal num2 = {{0, 0, 1, 0}};
  s21_set_decimal_bit(&num1, 0, 1);
  s21_set_decimal_bit(&num2, 2, 1);
  ck_assert_int_eq(s21_is_less(num1, num2), 1);
}
END_TEST

START_TEST(s21_is_less_or_equal1) {
  s21_decimal num1 = {{0, 0, 0, 0}};
  s21_decimal num2 = {{0, 0, 0, 0}};

  ck_assert_int_eq(s21_is_less_or_equal(num1, num2), 1);
}
END_TEST

START_TEST(s21_is_less_or_equal2) {
  s21_decimal num1 = {{1, 0, 0, 393216}};
  s21_decimal num2 = {{1, 0, 0, 196608}};

  ck_assert_int_eq(s21_is_less_or_equal(num1, num2), 1);
}
END_TEST

START_TEST(s21_is_less_or_equal3) {
  s21_decimal num1 = {{7, 0, 0, 0}};
  s21_decimal num2 = {{7, 0, 0, 0}};
  s21_decimal num3 = {{4294967, 4294967, 4294967, 0}};
  s21_decimal num4 = {{4294967, 4294967, 4294967, 0}};

  ck_assert_int_eq(s21_is_less_or_equal(num1, num2), 1);
  s21_set_decimal_bit(&num1, 127, 1);
  ck_assert_int_eq(s21_is_less_or_equal(num1, num2), 1);
  ck_assert_int_eq(s21_is_less_or_equal(num2, num2), 1);
  ck_assert_int_eq(s21_is_less_or_equal(num3, num4), 1);
  s21_set_decimal_bit(&num3, 127, 1);
  ck_assert_int_eq(s21_is_less_or_equal(num3, num4), 1);
  ck_assert_int_eq(s21_is_less_or_equal(num4, num3), 0);

  s21_decimal num5 = {{0, 0, 1, 0}};
  s21_decimal num6 = {{0, 0, 1, 0}};
  ck_assert_int_eq(s21_is_less_or_equal(num5, num6), 1);
  s21_set_decimal_bit(&num5, 0, 1);
  s21_set_decimal_bit(&num6, 0, 1);
  ck_assert_int_eq(s21_is_less_or_equal(num5, num6), 1);
}
END_TEST

START_TEST(s21_is_greater_or_equal1) {
  s21_decimal num1 = {{0, 0, 0, 0}};
  s21_decimal num2 = {{0, 0, 0, 0}};

  ck_assert_int_eq(s21_is_greater_or_equal(num1, num2), 1);
}
END_TEST

START_TEST(s21_is_greater_or_equal2) {
  s21_decimal num1 = {{1, 0, 0, 393216}};
  s21_decimal num2 = {{1, 0, 0, 196608}};

  ck_assert_int_eq(s21_is_greater_or_equal(num1, num2), 0);
}
END_TEST

START_TEST(s21_is_greater_or_equal3) {
  s21_decimal num1 = {{0, 0, 1, (unsigned int)65536}};
  s21_decimal num2 = {{0, 0, 1, 65536}};

  ck_assert_int_eq(s21_is_greater_or_equal(num1, num2), 1);
}
END_TEST

START_TEST(s21_is_greater_or_equal4) {
  s21_decimal first = {{35, 1123, 0, 0x1000}};
  s21_decimal second = {{98761, 123, 1, 0x80000}};

  int res = s21_is_greater_or_equal(first, second);

  ck_assert_int_eq(res, S21_TRUE);
}
END_TEST

START_TEST(s21_is_not_equal1) {
  s21_decimal num1 = {{0, 0, 0, 0}};
  s21_decimal num2 = {{0, 0, 0, 0}};

  ck_assert_int_eq(s21_is_not_equal(num1, num2), 0);
}
END_TEST

START_TEST(s21_is_not_equal2) {
  s21_decimal num1 = {{0, 0, 1, 393216}};
  s21_decimal num2 = {{0, 0, 1, 196608}};

  ck_assert_int_eq(s21_is_not_equal(num1, num2), 1);
}
END_TEST

START_TEST(s21_is_not_equal3) {
  s21_decimal num1 = {{0, 0, 1, (unsigned int)-65536}};
  s21_decimal num2 = {{0, 0, 1, 65536}};

  ck_assert_int_eq(s21_is_not_equal(num1, num2), 1);
}
END_TEST

START_TEST(s21_is_greater1) {
  s21_decimal num1 = {{1, 0, 0, 393216}};
  s21_decimal num2 = {{1, 0, 0, 196608}};

  ck_assert_int_eq(s21_is_greater(num1, num2), 0);
}
END_TEST

START_TEST(s21_is_greater2) {
  s21_decimal val1 = {
      {12345, 0, 0, 0b00000000000001000000000000000000}};  //  1.2345
  s21_decimal val2 = {
      {12, 0, 0, 0b10000000000000010000000000000000}};  //  -1.2;

  ck_assert_int_eq(s21_is_greater(val1, val2), 1);
}
END_TEST

START_TEST(s21_is_greater3) {
  s21_decimal num1 = {{0, 0, 1, (unsigned int)-65536}};
  s21_decimal num2 = {{0, 0, 1, 65536}};

  ck_assert_int_eq(s21_is_greater(num1, num2), 0);
}
END_TEST

/// Another tests ///

START_TEST(s21_negate1) {
  s21_decimal a = {{0, 12345, 6789, 0}}, res = {{0, 12345, 6789, INT_MAX}};
  s21_decimal negate_a = {0};

  s21_negate(a, &negate_a);
  ck_assert_int_eq(s21_is_equal(negate_a, res), 0);
}
END_TEST

START_TEST(s21_negate2) {
  unsigned int test = 1;
  s21_decimal value = {{test, 0, 0, 0x80000000}};
  s21_decimal result = {{test, 0, 0, 0x00000000}};

  s21_negate(value, &value);
  ck_assert_int_eq(value.bits[0], result.bits[0]);
  ck_assert_int_eq(value.bits[1], result.bits[1]);
  ck_assert_int_eq(value.bits[2], result.bits[2]);
  ck_assert_int_eq(value.bits[3], result.bits[3]);
}
END_TEST

START_TEST(s21_negate3) {
  s21_decimal src1 = {{0, 0, 0, 0}};
  s21_decimal *result_our = 0;
  int val = s21_negate(src1, result_our);
  ck_assert_int_eq(val, 1);
}
END_TEST

START_TEST(s21_floor1) {
  s21_decimal dec1;
  dec1.bits[0] = 0b10010011111100000001110001010010;
  dec1.bits[1] = 0b00000000000000000000000100100000;
  dec1.bits[2] = 0b00000000000000000000000000000000;
  dec1.bits[3] = 0b00000000000010100000000000000000;
  s21_decimal result;
  result.bits[0] = 0b00000000000000000000000001111011;
  result.bits[1] = 0b00000000000000000000000000000000;
  result.bits[2] = 0b00000000000000000000000000000000;
  result.bits[3] = 0b00000000000000000000000000000000;
  s21_decimal res1;
  s21_floor(dec1, &res1);
  ck_assert_float_eq(res1.bits[0], result.bits[0]);
  ck_assert_float_eq(res1.bits[1], result.bits[1]);
  ck_assert_float_eq(res1.bits[2], result.bits[2]);
  ck_assert_float_eq(res1.bits[3], result.bits[3]);
}
END_TEST

START_TEST(s21_floor2) {
  // 7151
  s21_decimal src1, origin, result = {{0, 0, 0, 0}};
  // src1 = 3.5;

  src1.bits[0] = 0b00000000000000000000000000100011;
  src1.bits[1] = 0b00000000000000000000000000000000;
  src1.bits[2] = 0b00000000000000000000000000000000;
  src1.bits[3] = 0b00000000000000010000000000000000;
  origin.bits[0] = 0b00000000000000000000000000000011;
  origin.bits[1] = 0b00000000000000000000000000000000;
  origin.bits[2] = 0b00000000000000000000000000000000;
  origin.bits[3] = 0b00000000000000000000000000000000;
  s21_floor(src1, &result);
  ck_assert_int_eq(origin.bits[3], result.bits[3]);
  ck_assert_int_eq(origin.bits[2], result.bits[2]);
  ck_assert_int_eq(origin.bits[1], result.bits[1]);
  ck_assert_int_eq(origin.bits[0], result.bits[0]);
}
END_TEST

START_TEST(s21_floor3) {
  s21_decimal src1, result_our, result_origin;
  src1.bits[0] = 23;
  src1.bits[1] = 0;
  src1.bits[2] = 0;
  src1.bits[3] = 0b00000000000000110000000000000000;
  int val = s21_floor(src1, &result_our);
  result_origin.bits[0] = 0;
  result_origin.bits[1] = 0;
  result_origin.bits[2] = 0;
  result_origin.bits[3] = 0;
  ck_assert_int_eq(result_origin.bits[0], result_our.bits[0]);
  ck_assert_int_eq(result_origin.bits[1], result_our.bits[1]);
  ck_assert_int_eq(result_origin.bits[2], result_our.bits[2]);
  ck_assert_int_eq(result_origin.bits[3], result_our.bits[3]);
  ck_assert_int_eq(val, 0);
}
END_TEST

START_TEST(s21_floor4) {
  s21_decimal src1, result_our, result_origin;
  src1.bits[0] = 0;
  src1.bits[1] = 0;
  src1.bits[2] = 0;
  src1.bits[3] = 0b00000000000000000000000000000000;
  int val = s21_floor(src1, &result_our);

  result_origin.bits[0] = 0;
  result_origin.bits[1] = 0;
  result_origin.bits[2] = 0;
  result_origin.bits[3] = 0;
  ck_assert_int_eq(result_origin.bits[0], result_our.bits[0]);
  ck_assert_int_eq(result_origin.bits[1], result_our.bits[1]);
  ck_assert_int_eq(result_origin.bits[2], result_our.bits[2]);
  ck_assert_int_eq(result_origin.bits[3], result_our.bits[3]);
  ck_assert_int_eq(val, 0);
}
END_TEST

START_TEST(s21_floor5) {
  s21_decimal value_one = {{25, 0, 0, ~(UINT_MAX / 2)}}, result = {0};
  s21_set_decimal_scale(&value_one, 1);
  ck_assert_int_eq(0, s21_floor(value_one, &result));
}
END_TEST

START_TEST(s21_floor6) {
  s21_decimal src1 = {{0, 0, 0, 0}};
  s21_decimal *result_our = NULL;
  int val = s21_floor(src1, result_our);
  ck_assert_int_eq(val, S21_ERROR);
}
END_TEST

START_TEST(s21_round1) {
  s21_decimal a = {{0, 12345, 6789, 0}}, res = {{0, 12345, 6789, 0}};
  s21_decimal round_a = {0};

  s21_round(a, &round_a);
  ck_assert_int_eq(s21_is_equal(round_a, res), 1);
}
END_TEST

START_TEST(s21_round2) {
  s21_decimal a = {{2147483, 12345, 6789, 0}},
              res = {{2147483, 12345, 6789, 0}};
  s21_decimal round_a = {0};

  s21_round(a, &round_a);
  ck_assert_int_eq(s21_is_equal(round_a, res), 1);
}
END_TEST

START_TEST(s21_round3) {
  s21_decimal value_one = {{25, 0, 0, ~(UINT_MAX / 2)}}, result = {0};
  s21_set_decimal_scale(&value_one, 1);
  ck_assert_int_eq(0, s21_round(value_one, &result));
}
END_TEST

START_TEST(s21_round4) {
  s21_decimal src1 = {{0, 0, 0, 0}};
  s21_decimal *result_our = NULL;
  int val = s21_round(src1, result_our);
  ck_assert_int_eq(val, S21_ERROR);
}
END_TEST

START_TEST(s21_round5) {
  s21_decimal src1, result_our, result_origin;
  src1.bits[0] = 53;
  src1.bits[1] = 0;
  src1.bits[2] = 0;
  src1.bits[3] = 0b00000000000000100000000000000000;
  int val = s21_round(src1, &result_our);
  result_origin.bits[0] = 1;
  result_origin.bits[1] = 0;
  result_origin.bits[2] = 0;
  result_origin.bits[3] = 0;
  ck_assert_int_eq(result_origin.bits[0], result_our.bits[0]);
  ck_assert_int_eq(result_origin.bits[1], result_our.bits[1]);
  ck_assert_int_eq(result_origin.bits[2], result_our.bits[2]);
  ck_assert_int_eq(result_origin.bits[3], result_our.bits[3]);
  ck_assert_int_eq(val, S21_OK);
}
END_TEST

START_TEST(s21_round6) {
  s21_decimal src1, result_our, result_origin;
  src1.bits[0] = 23;
  src1.bits[1] = 0;
  src1.bits[2] = 0;
  src1.bits[3] = 0b10000000000000110000000000000000;
  int val = s21_round(src1, &result_our);
  result_origin.bits[0] = 0;
  result_origin.bits[1] = 0;
  result_origin.bits[2] = 0;
  result_origin.bits[3] = 0;
  ck_assert_int_eq(result_origin.bits[0], result_our.bits[0]);
  ck_assert_int_eq(result_origin.bits[1], result_our.bits[1]);
  ck_assert_int_eq(result_origin.bits[2], result_our.bits[2]);
  ck_assert_int_eq(result_origin.bits[3], result_our.bits[3]);
  ck_assert_int_eq(val, S21_OK);
}
END_TEST

START_TEST(s21_round7) {
  s21_decimal src1, result_our, result_origin;
  src1.bits[0] = 11;
  src1.bits[1] = 0;
  src1.bits[2] = 0;
  src1.bits[3] = 0b10000000000000010000000000000000;
  int val = s21_round(src1, &result_our);
  result_origin.bits[0] = 1;
  result_origin.bits[1] = 0;
  result_origin.bits[2] = 0;
  result_origin.bits[3] = S21_MINUS;
  ck_assert_int_eq(result_origin.bits[0], result_our.bits[0]);
  ck_assert_int_eq(result_origin.bits[1], result_our.bits[1]);
  ck_assert_int_eq(result_origin.bits[2], result_our.bits[2]);
  ck_assert_int_eq(result_origin.bits[3], result_our.bits[3]);
  ck_assert_int_eq(val, S21_OK);
}
END_TEST

START_TEST(s21_truncate1) {
  s21_decimal src1, result_our, result_origin;
  src1.bits[0] = 123456;
  src1.bits[1] = 0;
  src1.bits[2] = 0;
  src1.bits[3] = 0b00000000000000110000000000000000;
  int val = s21_truncate(src1, &result_our);

  result_origin.bits[0] = 123;
  result_origin.bits[1] = 0;
  result_origin.bits[2] = 0;
  result_origin.bits[3] = 0;
  ck_assert_int_eq(result_origin.bits[0], result_our.bits[0]);
  ck_assert_int_eq(result_origin.bits[1], result_our.bits[1]);
  ck_assert_int_eq(result_origin.bits[2], result_our.bits[2]);
  ck_assert_int_eq(result_origin.bits[3], result_our.bits[3]);
  ck_assert_int_eq(val, 0);
}
END_TEST

START_TEST(s21_truncate2) {
  s21_decimal src1 = {{0, 0, 0, 0}};
  s21_decimal *result_our = NULL;
  int val = s21_truncate(src1, result_our);
  ck_assert_int_eq(val, S21_ERROR);
}
END_TEST

START_TEST(s21_truncate3) {
  s21_decimal dec1;
  dec1.bits[0] = 0b00000000000000000000000001101101;
  dec1.bits[1] = 0b00000000000011000110010101011011;
  dec1.bits[2] = 0b00000000000000000011000000111001;
  dec1.bits[3] = 0b00000000000011100000000000000000;
  s21_decimal result;
  result.bits[0] = 0b10000111101111000001011000011110;
  result.bits[1] = 0b00000000000000000000000000000000;
  result.bits[2] = 0b00000000000000000000000000000000;
  result.bits[3] = 0b00000000000000000000000000000000;
  s21_decimal res1;
  s21_truncate(dec1, &res1);
  ck_assert_float_eq(res1.bits[0], result.bits[0]);
  ck_assert_float_eq(res1.bits[1], result.bits[1]);
  ck_assert_float_eq(res1.bits[2], result.bits[2]);
  ck_assert_float_eq(res1.bits[3], result.bits[3]);
}
END_TEST

START_TEST(s21_truncate4) {
  s21_decimal value_one = {{7, 7, 7, 0}}, result = {0};
  float result_f = 0, value_two_f = (float)129127208515966861312.0;
  ck_assert_int_eq(0, s21_truncate(value_one, &result));
  s21_from_decimal_to_float(result, &result_f);
  ck_assert_float_eq(value_two_f, result_f);
}
END_TEST

START_TEST(s21_truncate5) {
  s21_decimal src1, result_our, result_origin;
  src1.bits[0] = 87654;
  src1.bits[1] = 0;
  src1.bits[2] = 0;
  src1.bits[3] = 0b10000000000001000000000000000000;
  int val = s21_truncate(src1, &result_our);
  result_origin.bits[0] = 8;
  result_origin.bits[1] = 0;
  result_origin.bits[2] = 0;
  result_origin.bits[3] = S21_MINUS;
  ck_assert_int_eq(result_origin.bits[0], result_our.bits[0]);
  ck_assert_int_eq(result_origin.bits[1], result_our.bits[1]);
  ck_assert_int_eq(result_origin.bits[2], result_our.bits[2]);
  ck_assert_int_eq(result_origin.bits[3], result_our.bits[3]);
  ck_assert_int_eq(val, S21_OK);
}
END_TEST

/// Convertor tests ///

START_TEST(s21_from_decimal_to_float1) {
  s21_decimal result = {{0, 0, 0, 0}};
  float check = 0.f;
  float test = 0.03;
  result.bits[0] = 0b01001010111100011001001011000000;
  result.bits[1] = 0b00111001010010010101100101000000;
  result.bits[2] = 0b00000000111110000010011101111000;
  result.bits[3] = 0b00000000000111000000000000000000;
  int err = s21_from_decimal_to_float(result, &check);
  ck_assert_ldouble_eq_tol(check, test, 1e-6);
  ck_assert_int_eq(err, 0);
}
END_TEST

START_TEST(s21_from_decimal_to_float2) {
  s21_decimal result = {{0, 0, 0, 0}};
  float check = 0.f;
  float test = 18.122;
  result.bits[0] = 18122;
  result.bits[1] = 0;
  result.bits[2] = 0;
  result.bits[3] = 0b00000000000000110000000000000000;
  int err = s21_from_decimal_to_float(result, &check);
  ck_assert_ldouble_eq_tol(check, test, 1e-6);
  ck_assert_int_eq(err, 0);
}
END_TEST

START_TEST(s21_from_decimal_to_float3) {
  s21_decimal result = {{0, 0, 0, 0}};
  float check = 0.f;
  result.bits[0] = 123456789;
  result.bits[1] = 0;
  result.bits[2] = 0;
  result.bits[3] = 0;
  s21_set_decimal_scale(&result, 4);
  int err = s21_from_decimal_to_float(result, &check);
  ck_assert_float_eq(check, 12345.6789);
  ck_assert_int_eq(err, 0);
}
END_TEST

START_TEST(s21_from_decimal_to_float4) {
  s21_decimal result = {{0, 0, 0, 0}};
  float check = 1.1f;
  result.bits[3] = 0b10000000000111010000000000000000;
  int err = s21_from_decimal_to_float(result, &check);

  ck_assert_int_eq(err, S21_ERROR);
}
END_TEST

START_TEST(s21_from_decimal_to_float5) {
  float *check = NULL;
  s21_decimal test = {{
      0,
      0,
      0,
      0,
  }};
  int val = s21_from_decimal_to_float(test, check);
  ck_assert_int_eq(val, S21_ERROR);
}
END_TEST

START_TEST(s21_from_decimal_to_float6) {
  s21_decimal result = {{0, 0, 0, 0}};
  float check = 0.f;
  result.bits[0] = 23450987;
  result.bits[1] = 0;
  result.bits[2] = 0;
  result.bits[3] = 2147745792;
  int err = s21_from_decimal_to_float(result, &check);
  ck_assert_float_eq_tol(check, -2345.09, 1e-1);
  ck_assert_int_eq(err, S21_OK);
}
END_TEST

START_TEST(s21_from_float_to_decimal1) {
  s21_decimal value;
  s21_from_float_to_decimal(0.03F, &value);
  ck_assert_int_eq(value.bits[0], 3);
  ck_assert_int_eq(value.bits[1], 0);
  ck_assert_int_eq(value.bits[2], 0);
  ck_assert_int_eq(value.bits[3], 0b00000000000000100000000000000000);
}
END_TEST

START_TEST(s21_from_float_to_decimal2) {
  s21_decimal value;
  s21_from_float_to_decimal(-0.03F, &value);
  ck_assert_int_eq(value.bits[0], 3);
  ck_assert_int_eq(value.bits[1], 0);
  ck_assert_int_eq(value.bits[2], 0);
  ck_assert_int_eq((unsigned)value.bits[3], 0b10000000000000100000000000000000);
}
END_TEST

START_TEST(s21_from_float_to_decimal3) {
  s21_decimal value;
  s21_from_float_to_decimal(3.0F, &value);
  ck_assert_int_eq(value.bits[0], 3);
  ck_assert_int_eq(value.bits[1], 0);
  ck_assert_int_eq(value.bits[2], 0);
  ck_assert_int_eq(value.bits[3], 0);
}
END_TEST

START_TEST(s21_from_float_to_decimal4) {
  s21_decimal value;
  s21_from_float_to_decimal(-3.0F, &value);
  ck_assert_int_eq(value.bits[0], 3);
  ck_assert_int_eq(value.bits[1], 0);
  ck_assert_int_eq(value.bits[2], 0);
  ck_assert_int_eq((unsigned)value.bits[3], 0b10000000000000000000000000000000);
}
END_TEST

START_TEST(s21_from_float_to_decimal5) {
  s21_decimal value;
  s21_from_float_to_decimal(0.0F, &value);
  ck_assert_int_eq(value.bits[0], 0);
  ck_assert_int_eq(value.bits[1], 0);
  ck_assert_int_eq(value.bits[2], 0);
  ck_assert_int_eq(value.bits[3], 0);
}
END_TEST

START_TEST(s21_from_float_to_decimal6) {
  s21_decimal value;
  s21_from_float_to_decimal(10.5F, &value);
  ck_assert_int_eq(value.bits[0], 105);
  ck_assert_int_eq(value.bits[1], 0);
  ck_assert_int_eq(value.bits[2], 0);
  ck_assert_int_eq(value.bits[3], 65536);
}
END_TEST

START_TEST(s21_from_float_to_decimal7) {
  s21_decimal value;
  int err = s21_from_float_to_decimal(1e-29F, &value);
  ck_assert_int_eq(value.bits[0], 0);
  ck_assert_int_eq(value.bits[1], 0);
  ck_assert_int_eq(value.bits[2], 0);
  ck_assert_int_eq(value.bits[3], 0);
  ck_assert_int_eq(err, 1);
}
END_TEST

START_TEST(s21_from_float_to_decimal8) {
  s21_decimal value;
  int err = s21_from_float_to_decimal(-1.0F / 0.0F, &value);
  ck_assert_int_eq(err, 1);
}
END_TEST

START_TEST(s21_from_float_to_decimal9) {
  s21_decimal value;
  int err = s21_from_float_to_decimal(1.0F / 0.0F, &value);
  ck_assert_int_eq(err, 1);
}
END_TEST

START_TEST(s21_from_float_to_decimal10) {
  s21_decimal value;
  int err = s21_from_float_to_decimal(1.234567e-28F, &value);
  ck_assert_int_eq(value.bits[0], 1);
  ck_assert_int_eq(value.bits[1], 0);
  ck_assert_int_eq(value.bits[2], 0);
  ck_assert_int_eq(value.bits[3], 1835008U);
  ck_assert_int_eq(err, 0);
}
END_TEST

START_TEST(s21_from_float_to_decimal11) {
  s21_decimal value;
  s21_from_float_to_decimal(1.02E+09F, &value);
  ck_assert_int_eq(value.bits[0], 1020000000);
  ck_assert_int_eq(value.bits[1], 0);
  ck_assert_int_eq(value.bits[2], 0);
  ck_assert_int_eq(value.bits[3], 0);
}
END_TEST

START_TEST(s21_from_float_to_decimal12) {
  s21_decimal value;
  float nan_value = NAN;
  int err = s21_from_float_to_decimal(nan_value, &value);
  ck_assert_int_eq(err, 1);
  ck_assert_int_eq(value.bits[0], 0);
  ck_assert_int_eq(value.bits[1], 0);
  ck_assert_int_eq(value.bits[2], 0);
  ck_assert_int_eq(value.bits[3], 0);
}
END_TEST

START_TEST(s21_from_float_to_decimal13) {
  s21_decimal value;
  int err = s21_from_float_to_decimal(123456789.123456789F, &value);
  ck_assert_int_eq(value.bits[0], 123456800);
  ck_assert_int_eq(value.bits[1], 0);
  ck_assert_int_eq(value.bits[2], 0);
  ck_assert_int_eq(value.bits[3], 0);
  ck_assert_int_eq(err, 0);
}
END_TEST

START_TEST(s21_from_float_to_decimal14) {
  s21_decimal value;
  s21_from_float_to_decimal(0.000000002F, &value);
  ck_assert_int_eq(value.bits[0], 2);
  ck_assert_int_eq(value.bits[1], 0);
  ck_assert_int_eq(value.bits[2], 0);
  ck_assert_int_eq(value.bits[3], 0b00000000000010010000000000000000);
}
END_TEST

START_TEST(s21_from_float_to_decimal15) {
  s21_decimal value;
  int err = s21_from_float_to_decimal(10000000000.000000001F, &value);
  ck_assert_int_eq(value.bits[0], 0b01010100000010111110010000000000);
  ck_assert_int_eq(value.bits[1], 0b00000000000000000000000000000010);
  ck_assert_int_eq(value.bits[2], 0);
  ck_assert_int_eq(value.bits[3], 0);
  ck_assert_int_eq(err, 0);
}
END_TEST

START_TEST(s21_from_float_to_decimal16) {
  s21_decimal value;
  s21_from_float_to_decimal(-5454.353F, &value);
  ck_assert_int_eq(value.bits[0], 5454353);
  ck_assert_int_eq(value.bits[1], 0);
  ck_assert_int_eq(value.bits[2], 0);
  ck_assert_int_eq((unsigned)value.bits[3], 0b10000000000000110000000000000000);
}
END_TEST

START_TEST(s21_from_float_to_decimal17) {
  s21_decimal value;
  s21_from_float_to_decimal(-5.492654F, &value);
  ck_assert_int_eq(value.bits[0], 5492654);
  ck_assert_int_eq(value.bits[1], 0);
  ck_assert_int_eq(value.bits[2], 0);
  ck_assert_int_eq((unsigned)value.bits[3], 0b10000000000001100000000000000000);
}
END_TEST

START_TEST(s21_from_float_to_decimal18) {
  s21_decimal value;
  s21_from_float_to_decimal(7.961328E+08F, &value);
  ck_assert_int_eq(value.bits[0], 796132800);
  ck_assert_int_eq(value.bits[1], 0);
  ck_assert_int_eq(value.bits[2], 0);
  ck_assert_int_eq(value.bits[3], 0);
}
END_TEST

START_TEST(s21_from_float_to_decimal19) {
  s21_decimal value;
  s21_from_float_to_decimal(-1.234568E+07F, &value);
  ck_assert_int_eq(value.bits[0], 12345680);
  ck_assert_int_eq(value.bits[1], 0);
  ck_assert_int_eq(value.bits[2], 0);
  ck_assert_int_eq((unsigned)value.bits[3], 0b10000000000000000000000000000000);
}
END_TEST

START_TEST(s21_from_float_to_decimal20) {
  float check = 0.f;
  s21_decimal *test = NULL;
  int val = s21_from_float_to_decimal(check, test);
  ck_assert_int_eq(val, 1);
}
END_TEST

START_TEST(s21_int_to_decimal1) {
  int test = 1;
  s21_decimal value = {{0, 0, 0, 0}};
  s21_decimal result = {{test, 0, 0, 0}};

  s21_from_int_to_decimal(test, &value);
  ck_assert_int_eq(value.bits[0], result.bits[0]);
  ck_assert_int_eq(value.bits[1], result.bits[1]);
  ck_assert_int_eq(value.bits[2], result.bits[2]);
  ck_assert_int_eq(value.bits[3], result.bits[3]);
}
END_TEST

START_TEST(s21_int_to_decimal2) {
  int test = -12345;
  s21_decimal value = {{0, 0, 0, 0}};
  s21_decimal result = {{-test, 0, 0, 0x80000000}};
  s21_from_int_to_decimal(test, &value);
  ck_assert_int_eq(value.bits[0], result.bits[0]);
  ck_assert_int_eq(value.bits[1], result.bits[1]);
  ck_assert_int_eq(value.bits[2], result.bits[2]);
  ck_assert_int_eq(value.bits[3], result.bits[3]);
}
END_TEST

START_TEST(s21_int_to_decimal3) {
  {
    int test_int = 0;
    s21_decimal *decimal = NULL;
    int status = s21_from_int_to_decimal(test_int, decimal);
    ck_assert_int_eq(status, 1);
  }
  {
    int test_int = 0;
    s21_decimal *decimal = malloc(sizeof(s21_decimal));
    int status = s21_from_int_to_decimal(test_int, decimal);
    ck_assert_int_eq(status, 0);
    ck_assert_int_eq(test_int, decimal->bits[0]);
    ck_assert_int_eq(0, decimal->bits[1]);
    ck_assert_int_eq(0, decimal->bits[2]);
    ck_assert_int_eq(0, s21_get_decimal_bit(*decimal, 127));
    ck_assert_int_eq(0, s21_get_decimal_bit(*decimal, 127));
    free(decimal);
  }
  {
    int test_int = INT32_MAX;
    s21_decimal *decimal = malloc(sizeof(s21_decimal));
    int status = s21_from_int_to_decimal(test_int, decimal);
    ck_assert_int_eq(status, 0);
    ck_assert_int_eq(test_int, decimal->bits[0]);
    ck_assert_int_eq(0, decimal->bits[1]);
    ck_assert_int_eq(0, decimal->bits[2]);
    ck_assert_int_eq(0, s21_get_decimal_bit(*decimal, 127));
    ck_assert_int_eq(0, s21_get_decimal_bit(*decimal, 127));
    free(decimal);
  }
}
END_TEST

START_TEST(s21_from_decimal_to_int1) {
  s21_decimal dec, big_dec = {{23536, 37375, 5647, 0}};
  int zero_i = 0, n2 = 98765321;
  int zero_res, res1, res2;
  int r1 = s21_from_int_to_decimal(zero_i, &dec);
  int r2 = s21_from_decimal_to_int(dec, &zero_res);
  ck_assert_int_eq(zero_i, zero_res);
  ck_assert_int_eq(r1, 0);
  ck_assert_int_eq(r2, 0);
  r1 = s21_from_int_to_decimal(n2, &dec);
  r2 = s21_from_decimal_to_int(dec, &res2);
  ck_assert_int_eq(n2, res2);
  ck_assert_int_eq(r1, 0);
  ck_assert_int_eq(r2, 0);
  r1 = s21_from_int_to_decimal(n2, &dec);
  r2 = s21_from_decimal_to_int(dec, &res2);
  ck_assert_int_eq(n2, res2);
  ck_assert_int_eq(r1, 0);
  ck_assert_int_eq(r2, 0);
  r1 = s21_from_decimal_to_int(big_dec, &res1);
  ck_assert_int_eq(r1, 1);
}
END_TEST

START_TEST(s21_from_decimal_to_int2) {
  s21_decimal src;
  int result = 0, number = 0;
  src.bits[0] = UINT32_MAX;
  src.bits[1] = UINT32_MAX;
  src.bits[2] = UINT32_MAX;
  src.bits[3] = UINT32_MAX;
  result = s21_from_decimal_to_int(src, &number);
  ck_assert_int_eq(result, 1);
}
END_TEST

START_TEST(s21_from_decimal_to_int3) {
  int test = 2;
  int result = 0;
  s21_decimal value = {{0, 0, 0, 0}};
  zero_s21_decimal(&value);
  value.bits[0] = 0b11111111111111111111111111110110;
  value.bits[1] = 0b00000000000000000000000000000100;
  value.bits[2] = 0b00000000000000000000000000000000;
  value.bits[3] = 0b00000000000010100000000000000000;

  s21_from_decimal_to_int(value, &result);
  ck_assert_int_eq(test, result);
}
END_TEST

START_TEST(s21_from_decimal_to_int4) {
  int test = INT_MAX;
  int result = 0;
  s21_decimal value = {{test, 0, 0, 0}};

  s21_from_decimal_to_int(value, &result);
  ck_assert_int_eq(test, result);
}
END_TEST

START_TEST(s21_from_decimal_to_int5) {
  int test = 7;
  int result = 0;
  s21_decimal value = {{0, 0, 0, 0}};
  zero_s21_decimal(&value);
  full_decimal(&value);
  value.bits[3] = 0b00000000000111000000000000000000;

  s21_from_decimal_to_int(value, &result);

  ck_assert_int_eq(test, result);
}
END_TEST

START_TEST(s21_from_decimal_to_int6) {
  int result = 0;
  s21_decimal value = {{0, 0, 0, 0}};
  zero_s21_decimal(&value);
  value.bits[0] = 0b11111111111111111111111111111111;
  value.bits[1] = 0b00000111111111111111111111111111;
  value.bits[2] = 0b00000000000000000000000000000000;
  value.bits[3] = 0b00000000000111010000000000000000;

  int status = s21_from_decimal_to_int(value, &result);
  ck_assert_int_eq(status, S21_ERROR);
}
END_TEST

START_TEST(s21_from_decimal_to_int7) {
  int test = -2147483648;
  int result = 0;
  s21_decimal value = {
      {(unsigned int)2147483648, 0, 0, 0b10000000000000000000000000000000}};
  s21_from_decimal_to_int(value, &result);
  ck_assert_int_eq(test, result);
}
END_TEST

START_TEST(s21_from_decimal_to_int8) {
  int *check = NULL;
  s21_decimal test = {{
      0,
      0,
      0,
      0,
  }};
  int val = s21_from_decimal_to_int(test, check);
  ck_assert_int_eq(val, S21_ERROR);
}
END_TEST

/// Arithmetic tests ///

START_TEST(s21_add1) {
  s21_decimal num = {{1, 0, 0, 0}};
  s21_decimal num2 = {{1, 0, 0, 0}};
  s21_decimal res = {0};
  s21_decimal answer = {{2, 0, 0, 0}};

  s21_add(num, num2, &res);
  ck_assert_int_eq(res.bits[0], answer.bits[0]);
  ck_assert_int_eq(res.bits[1], answer.bits[1]);
  ck_assert_int_eq(res.bits[2], answer.bits[2]);
  ck_assert_int_eq(res.bits[3], answer.bits[3]);
}
END_TEST

START_TEST(s21_add2) {
  s21_decimal src1 = {{0, 0, 0, 0}};
  s21_decimal src2 = {{0, 0, 0, 0}};
  s21_decimal *result_our = NULL;
  int val = s21_add(src1, src2, result_our);
  ck_assert_int_eq(val, 1);
}
END_TEST

START_TEST(s21_add3) {
  s21_decimal num1 = {
      {(unsigned int)-1, (unsigned int)-1, 56648, (unsigned int)-2147483648}};
  s21_decimal num2 = {{44, 369, 1687, (unsigned int)-2147483648}};
  s21_decimal res = {{0, 0, 0, 0}};
  int status = s21_add(num1, num2, &res);
  ck_assert_int_eq(status, 0);
  ck_assert_int_eq(res.bits[0], 43);
  ck_assert_int_eq(res.bits[1], 369);
  ck_assert_int_eq(res.bits[2], 58336);
  ck_assert_int_eq(res.bits[3], (unsigned int)-2147483648);
}
END_TEST

START_TEST(s21_add4) {
  s21_decimal num1 = {{3, 1, 3, 0}};
  s21_decimal num2 = {{5, 4, 8, (unsigned int)-2147483648}};
  s21_decimal res = {{0, 0, 0, 0}};
  int status = s21_add(num1, num2, &res);
  ck_assert_int_eq(status, 0);
  ck_assert_int_eq(res.bits[0], 2);
  ck_assert_int_eq(res.bits[1], 3);
  ck_assert_int_eq(res.bits[2], 5);
  ck_assert_int_eq(res.bits[3], (unsigned int)-2147483648);
}
END_TEST

START_TEST(s21_add5) {
  s21_decimal dec_1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF}};
  s21_decimal dec_2 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};
  s21_decimal result;
  int result_value = s21_add(dec_1, dec_2, &result);

  ck_assert_int_eq(result_value, 2);
}
END_TEST

START_TEST(s21_sub1) {
  s21_decimal num1 = {
      {(unsigned int)-1, (unsigned int)-1, (unsigned int)-1688, 0}};
  s21_decimal num2 = {{44, 369, 1687, (unsigned int)-2147483648}};
  s21_decimal res = {{0, 0, 0, 0}};

  int status = s21_sub(num1, num2, &res);

  ck_assert_int_eq(status, 1);
  ck_assert_int_eq(res.bits[0], 0);
  ck_assert_int_eq(res.bits[1], 0);
  ck_assert_int_eq(res.bits[2], 0);
  ck_assert_int_eq(res.bits[3], 0);
}
END_TEST

START_TEST(s21_sub2) {
  float tmp = pow(2, 31);
  s21_decimal dec1 = {{UINT_MAX, UINT_MAX, UINT_MAX, tmp}};
  s21_decimal dec2 = {{1, 0, 0, 0}};
  s21_decimal res1;
  int ret_s21 = s21_sub(dec1, dec2, &res1);
  ck_assert_int_eq(2, ret_s21);
}
END_TEST

START_TEST(s21_sub3) {
  s21_decimal src1 = {{0, 0, 0, 0}};
  s21_decimal src2 = {{0, 0, 0, 0}};
  s21_decimal *result_our = NULL;
  int val = s21_sub(src1, src2, result_our);
  ck_assert_int_eq(val, 1);
}
END_TEST

START_TEST(s21_sub4) {
  s21_decimal value_one = {{123, 456, 789, 0}},
              value_two = {{987, 654, 321, 0}}, result;
  s21_set_decimal_bit(&value_two, 127, 1);
  ck_assert_int_eq(0, s21_sub(value_one, value_two, &result));
}
END_TEST

START_TEST(s21_sub5) {
  s21_decimal c = {{100, 0, 0, ~(INT_MAX)}};
  s21_decimal d = {{100, 0, 0, ~(INT_MAX)}};
  s21_decimal res = {{0, 0, 0, 0}};
  s21_decimal *p_res = &res;
  int add = s21_sub(c, d, p_res);
  ck_assert_int_eq(add, 0);
}
END_TEST

START_TEST(s21_sub6) {
  s21_decimal dec_1 = {{0x1, 0x0, 0x0, 0x1D0000}};
  s21_decimal dec_2 = {{0xFFFFFFFF, 0x0, 0xFFFFFFFF, 0x0}};
  s21_decimal result;
  int result_value = s21_sub(dec_1, dec_2, &result);

  ck_assert_int_eq(result_value, 0);
}
END_TEST

START_TEST(s21_mul1) {
  s21_decimal src1 = {{0, 0, 0, 0}};
  s21_decimal src2 = {{0, 0, 0, 0}};
  s21_decimal *result_our = NULL;
  int val = s21_mul(src1, src2, result_our);
  ck_assert_int_eq(val, S21_ERROR);
}
END_TEST

START_TEST(s21_mul2) {
  s21_decimal dec_1 = {{0x0, 0x0, 0x0, 0x380000}};
  s21_decimal dec_2 = {{0xFFFFFFFF, 0x0, 0xFFFFFFFF, 0x0}};
  s21_decimal result;
  int result_value = s21_mul(dec_1, dec_2, &result);

  ck_assert_int_eq(result_value, 0);
}
END_TEST

START_TEST(s21_div1) {
  s21_decimal result1 = {{0, 0, 0, 0}};
  s21_decimal result2 = {{0, 0, 0, 0}};
  s21_decimal case_full1 = {{123, 321, -2147483648, 0}};
  s21_decimal case_full2 = {{123, 321, -2147483648, 0}};
  s21_decimal case1 = {{0, 0, 0, 0}};
  s21_decimal case_max = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
  s21_decimal case12 = {{150, 0, 0, 0x80000000}};
  s21_decimal case13 = {{2, 0, 0, 0x80000000}};
  s21_decimal d1 = {{1, 0, 0, 0}};
  s21_set_decimal_scale(&d1, 14);
  s21_set_decimal_bit(&d1, 127, 1);
  int int_res = -1;
  int_res = s21_div(case1, case_max, &result1);
  ck_assert_int_eq(int_res, 0);
  s21_div(case12, case13, &result2);
  int res_ful1 = 0;
  res_ful1 = s21_div(case_full1, case_full2, &result1);
  ck_assert_int_eq(res_ful1, 0);
}
END_TEST

START_TEST(s21_div2) {
  s21_decimal src1 = {{0, 0, 0, 0}};
  s21_decimal src2 = {{0, 0, 0, 0}};
  s21_decimal *result_our = NULL;
  int val = s21_div(src1, src2, result_our);
  ck_assert_int_eq(val, S21_ERROR);
}
END_TEST

START_TEST(s21_div3) {
  s21_decimal value_1 = {
      {UINT_MAX, UINT_MAX, UINT_MAX, 0b00000000000000000000000000000000}};
  s21_decimal value_2 = {{0, 0, 0, 0x80040000}};
  s21_decimal result = {0};
  ck_assert_int_eq((s21_div(value_1, value_2, &result)), 3);
}
END_TEST

START_TEST(s21_div4) {
  s21_decimal value_1 = {
      {UINT_MAX, UINT_MAX, UINT_MAX, 0b00000000000000000000000000000000}};
  s21_decimal value_2 = {{2, 0, 0, 0}};
  s21_decimal result = {0};
  ck_assert_int_eq((s21_mul(value_1, value_2, &result)), 1);
}
END_TEST

START_TEST(s21_div5) {
  s21_decimal num1 = {{1, 0, 0, 1835008}};
  s21_decimal num2 = {{4, 0, 0, (unsigned int)-2147483648}};
  s21_decimal res = {{0, 0, 0, 0}};
  int status = s21_div(num1, num2, &res);
  ck_assert_int_eq(status, 2);
}
END_TEST

///

int main() {
  Suite *s1 = suite_create("Core");
  TCase *tc_core = tcase_create("Core");
  SRunner *sr = srunner_create(s1);
  int nf;
  suite_add_tcase(s1, tc_core);

  /// Comparison tests ///
  tcase_add_test(tc_core, s21_is_equal1);
  tcase_add_test(tc_core, s21_is_equal2);
  tcase_add_test(tc_core, s21_is_equal4);

  tcase_add_test(tc_core, s21_is_not_equal1);
  tcase_add_test(tc_core, s21_is_not_equal2);
  tcase_add_test(tc_core, s21_is_not_equal3);

  tcase_add_test(tc_core, s21_is_less1);
  tcase_add_test(tc_core, s21_is_less2);
  tcase_add_test(tc_core, s21_is_less3);
  tcase_add_test(tc_core, s21_is_less4);

  tcase_add_test(tc_core, s21_is_less_or_equal1);
  tcase_add_test(tc_core, s21_is_less_or_equal2);
  tcase_add_test(tc_core, s21_is_less_or_equal3);

  tcase_add_test(tc_core, s21_is_greater_or_equal1);
  tcase_add_test(tc_core, s21_is_greater_or_equal2);
  tcase_add_test(tc_core, s21_is_greater_or_equal3);
  tcase_add_test(tc_core, s21_is_greater_or_equal4);

  tcase_add_test(tc_core, s21_is_greater1);
  tcase_add_test(tc_core, s21_is_greater2);
  tcase_add_test(tc_core, s21_is_greater3);

  /// Another tests ///
  tcase_add_test(tc_core, s21_floor1);
  tcase_add_test(tc_core, s21_floor2);
  tcase_add_test(tc_core, s21_floor3);
  tcase_add_test(tc_core, s21_floor4);
  tcase_add_test(tc_core, s21_floor5);
  tcase_add_test(tc_core, s21_floor6);

  tcase_add_test(tc_core, s21_negate1);
  tcase_add_test(tc_core, s21_negate2);
  tcase_add_test(tc_core, s21_negate3);

  tcase_add_test(tc_core, s21_round1);
  tcase_add_test(tc_core, s21_round2);
  tcase_add_test(tc_core, s21_round3);
  tcase_add_test(tc_core, s21_round4);
  tcase_add_test(tc_core, s21_round5);
  tcase_add_test(tc_core, s21_round6);
  tcase_add_test(tc_core, s21_round7);

  tcase_add_test(tc_core, s21_truncate1);
  tcase_add_test(tc_core, s21_truncate2);
  tcase_add_test(tc_core, s21_truncate3);
  tcase_add_test(tc_core, s21_truncate4);
  tcase_add_test(tc_core, s21_truncate5);

  /// Convertor tests ///

  tcase_add_test(tc_core, s21_from_decimal_to_float1);
  tcase_add_test(tc_core, s21_from_decimal_to_float2);
  tcase_add_test(tc_core, s21_from_decimal_to_float3);
  tcase_add_test(tc_core, s21_from_decimal_to_float4);
  tcase_add_test(tc_core, s21_from_decimal_to_float5);
  tcase_add_test(tc_core, s21_from_decimal_to_float6);

  tcase_add_test(tc_core, s21_int_to_decimal1);
  tcase_add_test(tc_core, s21_int_to_decimal2);
  tcase_add_test(tc_core, s21_int_to_decimal3);

  tcase_add_test(tc_core, s21_from_decimal_to_int1);
  tcase_add_test(tc_core, s21_from_decimal_to_int2);
  tcase_add_test(tc_core, s21_from_decimal_to_int3);
  tcase_add_test(tc_core, s21_from_decimal_to_int4);
  tcase_add_test(tc_core, s21_from_decimal_to_int5);
  tcase_add_test(tc_core, s21_from_decimal_to_int6);
  tcase_add_test(tc_core, s21_from_decimal_to_int7);
  tcase_add_test(tc_core, s21_from_decimal_to_int8);

  tcase_add_test(tc_core, s21_from_float_to_decimal1);
  tcase_add_test(tc_core, s21_from_float_to_decimal2);
  tcase_add_test(tc_core, s21_from_float_to_decimal3);
  tcase_add_test(tc_core, s21_from_float_to_decimal4);
  tcase_add_test(tc_core, s21_from_float_to_decimal5);
  tcase_add_test(tc_core, s21_from_float_to_decimal6);
  tcase_add_test(tc_core, s21_from_float_to_decimal7);
  tcase_add_test(tc_core, s21_from_float_to_decimal8);
  tcase_add_test(tc_core, s21_from_float_to_decimal9);
  tcase_add_test(tc_core, s21_from_float_to_decimal10);
  tcase_add_test(tc_core, s21_from_float_to_decimal11);
  tcase_add_test(tc_core, s21_from_float_to_decimal12);
  tcase_add_test(tc_core, s21_from_float_to_decimal13);
  tcase_add_test(tc_core, s21_from_float_to_decimal14);
  tcase_add_test(tc_core, s21_from_float_to_decimal15);
  tcase_add_test(tc_core, s21_from_float_to_decimal16);
  tcase_add_test(tc_core, s21_from_float_to_decimal17);
  tcase_add_test(tc_core, s21_from_float_to_decimal18);
  tcase_add_test(tc_core, s21_from_float_to_decimal19);
  tcase_add_test(tc_core, s21_from_float_to_decimal20);

  /// Arithmeric tests ///

  tcase_add_test(tc_core, s21_add1);
  tcase_add_test(tc_core, s21_add2);
  tcase_add_test(tc_core, s21_add3);
  tcase_add_test(tc_core, s21_add4);
  tcase_add_test(tc_core, s21_add5);

  tcase_add_test(tc_core, s21_sub1);
  tcase_add_test(tc_core, s21_sub2);
  tcase_add_test(tc_core, s21_sub3);
  tcase_add_test(tc_core, s21_sub4);
  tcase_add_test(tc_core, s21_sub5);
  tcase_add_test(tc_core, s21_sub6);

  tcase_add_test(tc_core, s21_mul1);
  tcase_add_test(tc_core, s21_mul2);

  tcase_add_test(tc_core, s21_div1);
  tcase_add_test(tc_core, s21_div2);
  tcase_add_test(tc_core, s21_div3);
  tcase_add_test(tc_core, s21_div4);
  tcase_add_test(tc_core, s21_div5);

  srunner_run_all(sr, CK_ENV);
  nf = srunner_ntests_failed(sr);
  srunner_free(sr);

  return nf ? 1 : 0;
}
