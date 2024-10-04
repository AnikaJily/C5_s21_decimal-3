#include "s21_decimal.h"

// узнать значение конкретного бита
int s21_get_decimal_bit(s21_decimal num, int cur_bit) {
  int bit;
  if ((num.bits[cur_bit / 32] & (1 << cur_bit % 32)) == 0) {
    bit = 0;
  } else {
    bit = 1;
  }
  return bit;
}

// узнать значение конкретного бита для big decimal
int s21_get_decimal_bit_big(big_decimal *num, int cur_bit) {
  int bit;
  if ((num->bits[cur_bit / 32] & (1 << cur_bit % 32)) == 0) {
    bit = 0;
  } else {
    bit = 1;
  }
  return bit;
}

// установить значение конкретного бита
void s21_set_decimal_bit(s21_decimal *num, int bit, int result) {
  if (result == 1) {
    num->bits[bit / 32] = num->bits[bit / 32] | (1 << bit % 32);
  } else if (result == 0) {
    num->bits[bit / 32] = num->bits[bit / 32] & ~(1 << bit % 32);
  }
}

// установить значение конкретного бита для big_decimal
void s21_set_decimal_bit_big(big_decimal *num, int bit, int result) {
  if (result == 1) {
    num->bits[bit / 32] = num->bits[bit / 32] | (1 << bit % 32);
  } else if (result == 0) {
    num->bits[bit / 32] = num->bits[bit / 32] & ~(1 << bit % 32);
  }
}

// устанавливает минус
void s21_set_minus(s21_decimal *num, int sign) {
  s21_set_decimal_bit(num, 127, sign);
}

// // получить знак
int s21_get_sign(s21_decimal num) { return s21_get_decimal_bit(num, 127); }

// узнать значение scale коэфициента масштабирования
int s21_get_decimal_scale(s21_decimal num) {
  int mask = 0b11111111;
  mask <<= 16;
  mask = num.bits[3] & mask;
  mask >>= 16;
  return mask;
}

// установить значение scale коэфициента масштабирования
int s21_set_decimal_scale(s21_decimal *num, int scale) {
  int result = 0;
  if (scale < 256 && scale >= 0) {
    for (int i = 0; i < 8; i++) {
      s21_set_decimal_bit(num, i + 16 + 96, scale % 2);
      scale >>= 1;
    }
  } else {
    result = 1;
  }
  return result;
}

void init_big(s21_decimal value, big_decimal *big) {
  big->exponenta = s21_get_decimal_scale(value);
  big->negative = s21_get_sign(value);
  big->bits[0] = value.bits[0];
  big->bits[1] = value.bits[1];
  big->bits[2] = value.bits[2];
  zeroes_left_big(big);
}

void change_znak_s21(s21_decimal *value) {
  s21_set_minus(value, !s21_get_sign(*value));
}

void full_decimal(s21_decimal *num) {
  for (int i = 0; i < 96; i++) {
    s21_set_decimal_bit(num, i, 1);
  }
}

int s21_big_to_s21decimal(s21_decimal *result, big_decimal *result_big) {
  int status = 0;
  zeroes_left_big(result_big);
  big_decimal ten = {0};
  ten.bits[0] = 10;
  int diff = 0;
  big_decimal copy_result_big_x = *result_big;
  while (result_big->exponenta > 28) {
    if (s21_compare_mantis_big(result_big, &ten) >= 0) {
      division_with_rest_for10(*result_big, result_big);
      diff++;
    } else {
      status = 2;
      zero_s21_decimal(result);
      break;
    }
  }
  if (!status) {
    if (diff > 0) s21_bank_round(copy_result_big_x, result_big, diff);

    diff = 0;
    if (go_beyond_big_decimal_s21(result_big)) {
      if (result_big->exponenta < 1) {
        status = 1;
      } else {
        copy_result_big_x = *result_big;
        while (go_beyond_big_decimal_s21(result_big) &&
               (result_big->exponenta > 0)) {
          division_with_rest_for10(*result_big, result_big);
          diff++;
        }
        if (diff > 0) s21_bank_round(copy_result_big_x, result_big, diff);
      }
    }

    if (go_beyond_big_decimal_s21(result_big)) status = 1;

    if ((status == 1) && result_big->negative) status = 2;

    if (!status) {
      if (result_big->negative) s21_set_minus(result, 1);
      s21_big_to_s21decimal_95(result_big, result);
      s21_set_decimal_scale(result, result_big->exponenta);
    }
  }
  return status;
}

void s21_bank_round(big_decimal copy_result_big_x, big_decimal *result_big,
                    int exp_diff) {
  big_decimal one = {0};
  one.bits[0] = 1;
  big_decimal ostatok_xy = {0};
  big_decimal half = {0};
  half.bits[0] = 5;
  half.exponenta = 1;

  if (exp_diff > 0) exp_diff--;
  big_decimal copy_result_big_y = *result_big;
  s21_normalize_big(&copy_result_big_x, &copy_result_big_y);

  sub_mantis_big(copy_result_big_x, copy_result_big_y, &ostatok_xy);
  ostatok_xy.negative = 0;
  multiply_10_mantis_big(&half, exp_diff);

  int compare_res = s21_compare_mantis_big(&ostatok_xy, &half);
  if (compare_res == 1) {
    sum_mantissa(result_big, &one, result_big);
  } else if (compare_res == 0) {
    if (s21_get_decimal_bit_big(result_big, 0) == 1)
      sum_mantissa(result_big, &one, result_big);
  }
}

unsigned int division_with_rest_for10(big_decimal val1, big_decimal *res) {
  big_decimal val2 = {0};
  val2.bits[0] = 10;
  zeroes_left_big(&val2);
  int q = 0;
  big_decimal part = {0};
  big_decimal part_next = {0};
  big_decimal sum = {0};
  big_decimal before_sum = {0};
  while (is_zero_big_decimal(val1) &&
         is_greater_or_equal_big_decimal(val1, val2)) {
    q = 0;
    zero_big_decimal(&part);
    zero_big_decimal(&before_sum);
    part_next = val2;
    part = val2;
    zeroes_left_big(&val1);
    while (is_greater_or_equal_big_decimal(val1, part_next)) {
      if (q == 0) {
        int difference_elder_bit =
            val1.one_position_left - val2.one_position_left;
        if (difference_elder_bit > 2) {
          q = difference_elder_bit - 1;
          shift_left_big(&part_next, q);
        }
      }
      part = part_next;
      shift_left_big(&part_next, 1);
      q++;
    }
    q--;
    s21_set_decimal_bit_big(&before_sum, q, 1);
    sum_mantissa(&sum, &before_sum, &sum);
    if (is_greater_or_equal_big_decimal(val1, part))
      sub_mantis_big(val1, part, &val1);
  }

  sum.exponenta = val1.exponenta - 1;
  sum.negative = val1.negative;
  *res = sum;
  return val1.bits[0];
}

int go_beyond_big_decimal_s21(big_decimal *big) {
  int result = 0;
  for (int i = 3; i < BITS_B; i++) {
    if (big->bits[i] != 0) {
      result = 1;
      break;
    }
  }

  return result;
}

void zero_s21_decimal(s21_decimal *value) {
  value->bits[0] = value->bits[1] = value->bits[2] = value->bits[3] = 0;
}

int is_greater_or_equal_big_decimal(big_decimal value_1, big_decimal value_2) {
  int result = 1;
  for (int i = 7; i >= 0; i--) {
    if (value_1.bits[i] > value_2.bits[i]) {
      result = 1;
      break;
    } else if (value_1.bits[i] < value_2.bits[i]) {
      result = 0;
      break;
    }
  }
  return result;
}

int is_greater_big_decimal(big_decimal value_1, big_decimal value_2) {
  int result = 0, out = 0;
  for (int i = 7; i >= 0 && !result && !out; i--) {
    if (value_1.bits[i] || value_2.bits[i]) {
      if (value_1.bits[i] > value_2.bits[i]) {
        result = 1;
      }
      if (value_1.bits[i] != value_2.bits[i]) out = 1;
    }
  }
  return result;
}

int is_zero_big_decimal(big_decimal big) {
  int result = 0;
  for (int i = 7; i >= 0; i--) {
    if (big.bits[i] != 0) {
      result = 1;
      break;
    }
  }
  return result;
}

int is_zero_s21_decimal(s21_decimal value) {
  int res = 0;
  for (int i = 2; i >= 0; i--) {
    if (value.bits[i] != 0) {
      res = 1;
      break;
    }
  }
  return res;
}

void sub_mantis_big(big_decimal value_1, big_decimal value_2,
                    big_decimal *result) {
  int tmp = 0, res = 0;
  for (int i = 0; i <= BITS_BIG; i++) {
    res = s21_get_decimal_bit_big(&value_1, i) -
          s21_get_decimal_bit_big(&value_2, i) - tmp;
    tmp = res < 0;
    res = abs(res);
    s21_set_decimal_bit_big(result, i, res % 2);
  }
}

int multiply_10_mantis_big(big_decimal *bvalue, int def) {
  int status = 0;

  s21_decimal ten_s = {0};
  s21_from_int_to_decimal(10, &ten_s);
  big_decimal ten = {0};
  init_big(ten_s, &ten);

  for (int i = 1; i <= def; i++) {
    if (s21_multiply_mantis_big(*bvalue, &ten, bvalue)) status = 1;
  }
  bvalue->exponenta += def;
  return status;
}

int s21_compare_mantis_big(big_decimal *bvalue1, big_decimal *bvalue2) {
  int result = 0;
  for (int i = BITS_BIG; i >= 0; i--) {
    int rvalue1 = 0, rvalue2 = 0;
    rvalue1 = s21_get_decimal_bit_big(bvalue1, i);
    rvalue2 = s21_get_decimal_bit_big(bvalue2, i);
    if (rvalue1 != rvalue2) {
      result = rvalue1 - rvalue2;
      break;
    }
  }
  return result;
}

void s21_big_to_s21decimal_95(big_decimal *result_big, s21_decimal *result) {
  for (int i = 0; i < 3; i++) {
    result->bits[i] = result_big->bits[i];
  }
}

int sum_mantissa(big_decimal *bvalue_1, big_decimal *bvalue_2,
                 big_decimal *result) {
  int status = 0;
  int tmp = 0;
  int var = 0;

  for (int i = 0; i <= BITS_BIG; i++) {
    var = (s21_get_decimal_bit_big(bvalue_1, i) +
           s21_get_decimal_bit_big(bvalue_2, i) + tmp);
    if (var == 3) {
      tmp = 1;
      s21_set_decimal_bit_big(result, i, 1);
    } else if (var == 2) {
      tmp = 1;
      s21_set_decimal_bit_big(result, i, 0);
    } else if (var == 1) {
      s21_set_decimal_bit_big(result, i, 1);
      tmp = 0;
    } else if (var == 0) {
      s21_set_decimal_bit_big(result, i, 0);
      tmp = 0;
    }
  }
  if (tmp == 1) status = 1;
  return status;
}

void s21_normalize_big(big_decimal *bvalue_1, big_decimal *bvalue_2) {
  int def = bvalue_1->exponenta - bvalue_2->exponenta;
  if (def > 0) {
    multiply_10_mantis_big(bvalue_2, def);
    zeroes_left_big(bvalue_2);
  } else if (def < 0) {
    multiply_10_mantis_big(bvalue_1, -def);
    zeroes_left_big(bvalue_1);
  }
}

int s21_multiply_mantis_big(big_decimal bvalue_1, big_decimal *bvalue_2,
                            big_decimal *result) {
  int status = 0;

  zero_big_decimal(result);

  zeroes_left_big(bvalue_2);
  zeroes_left_big(&bvalue_1);

  for (int i = 0; i <= bvalue_2->one_position_left; i++) {
    if (i != 0)
      if (shift_left_big(&bvalue_1, 1) == 1) {
        status = 1;
        break;
      }
    if (s21_get_decimal_bit_big(bvalue_2, i))
      if (sum_mantissa(result, &bvalue_1, result)) status = 1;
  }

  int equal_znak = (bvalue_1.negative == bvalue_2->negative);
  if (!equal_znak) result->negative = 1;

  result->exponenta = bvalue_1.exponenta + bvalue_2->exponenta;

  return status;
}

void s21_zero_mantisa_big(big_decimal *result) {
  for (int i = 0; i < BITS_B; i++) {
    result->bits[i] = 0;
  }
}

void zero_big_decimal(big_decimal *result) {
  s21_zero_mantisa_big(result);
  result->exponenta = 0;
  result->negative = 0;
  result->one_position_left = 0;
  result->one_right = 0;
  result->zero_left = 0;
}

int shift_left_big(big_decimal *bvalue, int def) {
  int status = 0;

  zeroes_left_big(bvalue);

  if ((BITS_BIG - bvalue->one_position_left) < def) status = 1;

  for (int i = bvalue->one_position_left; i >= 0; i--) {
    if ((i + def) <= BITS_BIG) {
      s21_set_decimal_bit_big(bvalue, i + def,
                              s21_get_decimal_bit_big(bvalue, i));
    }
  }
  for (int i = 0; i < def; i++) {
    s21_set_decimal_bit_big(bvalue, i, 0);
  }
  zeroes_left_big(bvalue);

  return status;
}

void zeroes_left_big(big_decimal *bvalue) {
  int i = BITS_BIG;
  while (1) {
    if (s21_get_decimal_bit_big(bvalue, i) != 0) {
      bvalue->zero_left = BITS_BIG - i;
      bvalue->one_position_left = i;
      break;
    }
    i--;
    if (!(i >= 0)) break;
  }
  if (i == -1) {
    bvalue->zero_left = BITS_BIG;
    bvalue->one_position_left = -1;
  }
  for (int j = 0; j <= BITS_BIG; j++) {
    if (s21_get_decimal_bit_big(bvalue, j) != 0) {
      bvalue->one_right = j;
      break;
    }
  }
}

void s21_division(big_decimal val1, big_decimal val2, big_decimal *res) {
  int scale_dif = (val1.exponenta - val2.exponenta);
  int q = 0;
  big_decimal part = {0};
  big_decimal part_next = {0};
  big_decimal sum = {0};
  big_decimal before_sum = {0};
  while (is_zero_big_decimal(val1) && sum.exponenta < 31) {
    while (is_greater_big_decimal(val2, val1)) {
      multiply_10_mantis_big(&val1, 1);
      multiply_10_mantis_big(&sum, 1);
    }
    q = 0;
    zero_big_decimal(&part);
    zero_big_decimal(&before_sum);
    part_next = val2;
    part = val2;
    zeroes_left_big(&val1);
    while (is_greater_or_equal_big_decimal(val1, part_next)) {
      if (q == 0) {
        int difference_elder_bit =
            val1.one_position_left - val2.one_position_left;
        if (difference_elder_bit > 2) {
          q = difference_elder_bit - 2;
          shift_left_big(&part_next, q);
        }
      }
      part = part_next;
      shift_left_big(&part_next, 1);
      q++;
    }
    q--;
    s21_set_decimal_bit_big(&before_sum, q, 1);
    sum_mantissa(&sum, &before_sum, &sum);
    if (is_greater_or_equal_big_decimal(val1, part))
      sub_mantis_big(val1, part, &val1);
  }
  sum.exponenta += scale_dif;
  if (scale_dif < 0) {
    multiply_10_mantis_big(&sum, -scale_dif);
  }
  *res = sum;
}

int s21_comparing(s21_decimal *x1, s21_decimal *x2) {
  int res = 0;
  int sign1 = s21_get_sign(*x1), sign2 = s21_get_sign(*x2);
  int both_negative = 0;

  if (sign1 == 1 && sign2 == 0 &&
      (is_zero_s21_decimal(*x1) || is_zero_s21_decimal(*x2)))
    res = -1;
  else if (sign1 == 0 && sign2 == 1 &&
           (is_zero_s21_decimal(*x1) || is_zero_s21_decimal(*x2)))
    res = 1;
  else if (sign1 == 1 && sign2 == 1)
    both_negative = 1;

  if (!res) {
    // приведение к одной степени
    big_decimal big1 = {0}, big2 = {0};
    init_big(*x1, &big1);
    init_big(*x2, &big2);
    s21_normalize_big(&big1, &big2);
    int difference_position = big1.one_position_left - big2.one_position_left;
    if (difference_position > 0) {
      res = 1;
    } else if (difference_position < 0) {
      res = -1;
    } else {
      // проверка мантисс
      int difference_bit = 0;
      for (int i = big1.one_position_left; i >= 0; i--) {
        difference_bit = s21_get_decimal_bit_big(&big1, i) -
                         s21_get_decimal_bit_big(&big2, i);
        if (difference_bit > 0) {
          res = 1;
          break;
        } else if (difference_bit < 0) {
          res = -1;
          break;
        }
      }
    }
    if (res) {
      if (both_negative) res = -res;
    }
  }

  return res;
}

void s21_print_big_decimal(big_decimal *num) {
  zeroes_left_big(num);
  printf("\nЭксонента: %d | Отрицательное: %d |", num->exponenta,
         num->negative);
  printf(
      " Позиция старшего бита: %d | Колич нулей слев: %d | Первая единичка "
      "справа %d",
      num->one_position_left, num->zero_left, num->one_right);
  printf("\n");
  for (int i = 7; i >= 0; i--) {
    for (int j = 31; j >= 0; j--) {
      printf("%d", s21_get_decimal_bit_big(num, i * 32 + j));
    }
  }
}
