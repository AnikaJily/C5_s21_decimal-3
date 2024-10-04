#include "s21_decimal.h"

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int status = 0;
  if (result) {
    int equal_signs = 0;
    zero_s21_decimal(result);

    big_decimal bvalue_1 = {0};
    init_big(value_1, &bvalue_1);
    big_decimal bvalue_2 = {0};
    init_big(value_2, &bvalue_2);
    big_decimal result_b = {0};

    s21_normalize_big(&bvalue_1, &bvalue_2);

    if (s21_get_sign(value_1) == s21_get_sign(value_2)) equal_signs = 1;

    if (equal_signs) {
      sum_mantissa(&bvalue_1, &bvalue_2, &result_b);
      result_b.negative = bvalue_1.negative;
    } else {
      int compare = s21_compare_mantis_big(&bvalue_1, &bvalue_2);
      if (compare > 0) {
        sub_mantis_big(bvalue_1, bvalue_2, &result_b);
        result_b.negative = bvalue_1.negative;
      } else if (compare < 0) {
        sub_mantis_big(bvalue_2, bvalue_1, &result_b);
        result_b.negative = bvalue_2.negative;
      }
    }

    result_b.exponenta = bvalue_1.exponenta;
    status = s21_big_to_s21decimal(result, &result_b);

  } else {
    status = 1;
  }
  return status;
}

int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int status = 0;
  if (result) {
    int equal_signs = 0;
    zero_s21_decimal(result);

    change_znak_s21(&value_2);

    big_decimal bvalue_1 = {0};
    init_big(value_1, &bvalue_1);
    big_decimal bvalue_2 = {0};
    init_big(value_2, &bvalue_2);
    big_decimal result_b = {0};

    s21_normalize_big(&bvalue_1, &bvalue_2);

    if (s21_get_sign(value_1) == s21_get_sign(value_2)) equal_signs = 1;

    if (equal_signs) {
      sum_mantissa(&bvalue_1, &bvalue_2, &result_b);
      result_b.negative = bvalue_1.negative;
    } else {
      int compare = s21_compare_mantis_big(&bvalue_1, &bvalue_2);
      if (compare > 0) {
        sub_mantis_big(bvalue_1, bvalue_2, &result_b);
        result_b.negative = bvalue_1.negative;
      } else if (compare < 0) {
        sub_mantis_big(bvalue_2, bvalue_1, &result_b);
        result_b.negative = bvalue_2.negative;
      }
    }

    result_b.exponenta = bvalue_1.exponenta;
    status = s21_big_to_s21decimal(result, &result_b);

  } else {
    status = 1;
  }
  return status;
}

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int status = 0;
  if (result) {
    int equal_signs = 0;

    zero_s21_decimal(result);

    big_decimal bvalue_1 = {0};
    init_big(value_1, &bvalue_1);
    big_decimal bvalue_2 = {0};
    init_big(value_2, &bvalue_2);
    big_decimal result_b = {0};

    if (s21_get_sign(value_1) == s21_get_sign(value_2)) equal_signs = 1;

    if (s21_multiply_mantis_big(bvalue_1, &bvalue_2, &result_b)) status = 1;

    if (equal_signs) {
      result_b.negative = 0;
    } else {
      result_b.negative = 1;
    }

    zeroes_left_big(&result_b);
    if (result_b.one_position_left == -1) {
      result_b.negative = 0;
      result_b.exponenta = 0;
    }

    if (!status) status = s21_big_to_s21decimal(result, &result_b);

  } else {
    status = 1;
  }
  return status;
}

int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int status = 0;
  int zero1 = 0;
  if (!is_zero_s21_decimal(value_1)) zero1 = 1;

  if (result) {
    zero_s21_decimal(result);

    if (is_zero_s21_decimal(value_2)) {
      big_decimal bvalue1 = {0}, bvalue2 = {0}, b_result = {0};
      init_big(value_1, &bvalue1);
      init_big(value_2, &bvalue2);

      s21_division(bvalue1, bvalue2, &b_result);

      if (s21_get_sign(value_1) != s21_get_sign(value_2)) b_result.negative = 1;
      if (!status) status = s21_big_to_s21decimal(result, &b_result);

    } else {
      status = 3;
    }
    if (status || zero1) zero_s21_decimal(result);
  } else {
    status = 1;
  }
  return status;
}
