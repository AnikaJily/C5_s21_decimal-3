#include "s21_decimal.h"

int s21_from_int_to_decimal(int src, s21_decimal *dst) {
  int status = S21_OK;

  if (dst) {
    zero_s21_decimal(dst);
    dst->bits[0] = abs(src);
    if (src < 0) {
      s21_set_minus(dst, 1);
    }
  } else {
    status = S21_ERROR;
  }

  return status;
}

int s21_from_decimal_to_int(s21_decimal src, int *dst) {
  int res = S21_OK;
  if (dst) {
    *dst = 0;
    //  - 0 - OK
    //  - 1 - ошибка конвертации
    int exp = s21_get_decimal_scale(src);
    big_decimal full = {0};
    int special_case = 0;
    init_big(src, &full);
    if (exp) {
      for (int i = 0; i < exp; i++) {
        division_with_rest_for10(full, &full);  // делим мантиссу на 10^exp
      }
    }
    zeroes_left_big(&full);

    if (full.one_position_left >
        30)  // так как 1 бит в int зарезервирован под знак
      res = S21_ERROR;  // decimal > int
    if (full.one_position_left == 31 && full.negative) {
      int all_zeros = 1;
      for (int i = 30; i >= 0; i--) {
        if (s21_get_decimal_bit_big(&full, i)) {
          all_zeros = 0;
          break;
        }
      }
      if (all_zeros) {
        res = S21_OK;
        special_case = 1;
        *dst = -2147483648;
      }
    }
    // перевод последнего bits в int
    int bit = 0;
    for (int i = 30; i >= 0; i--) {
      bit = s21_get_decimal_bit_big(&full, i);
      *dst = *dst | bit << i;
    }
    if (full.negative) {
      if (!special_case) *dst = ~(*dst) + 1;
    }
    if (exp > 28) res = S21_ERROR;
  } else
    res = S21_ERROR;
  return res;
}

int s21_from_float_to_decimal(float src, s21_decimal *dst) {
  int status = S21_OK;

  if (dst) {
    zero_s21_decimal(dst);
  } else {
    status = S21_ERROR;
  }

  if (status || (0 < fabsf(src) && fabsf(src) < 1e-28) || isnan(src) ||
      isinf(src) || 79228162514264337593543950335.f < src) {
    status = S21_ERROR;
  } else {
    char part[128] = {0};
    int scale = 0;
    int decimal_part = 0;
    int length = 0;

    sprintf(part, "%.7g", src);
    s21_get_float_part(part, &scale, &length, &decimal_part);
    if (scale < 0 && 28 < abs(scale) + length) {
      memset(part, 0, 128);

      sprintf(part, "%.*g", 29 - abs(scale), src);
      s21_get_float_part(part, &scale, &length, &decimal_part);
    }

    // считываем целую часть
    int number = abs(atoi(part));
    // выставляем место для дробной части
    for (int i = 0; i < length; ++i) {
      number *= 10;
    }
    // прибавляем дробную часть
    number += decimal_part;

    // устанавливаем минус
    if (src < 0) {
      s21_set_minus(dst, 1);
    }

    // устанавливаем экспоненту
    if (scale < 0) {
      s21_set_decimal_scale(dst, abs(scale) + length);
    } else if (!scale) {
      s21_set_decimal_scale(dst, length);
    }

    // побитово записываем в мантиссу
    for (int i = 0; number; ++i, number /= 2) {
      if (number % 2) {
        s21_set_decimal_bit(dst, i, 1);
      }
    }

    // домножаем мантиссу на 10 до тех пор пока не выведем из дробной части
    if (0 < scale - length) {
      s21_decimal s21_mul_by_10 = {{10, 0, 0, 0}};
      for (int i = 0; !status && i < scale - length; ++i) {
        status = s21_mul(*dst, s21_mul_by_10, dst);
      }
    }
  }

  return status;
}

void s21_get_float_part(char *part, int *scale, int *length,
                        int *decimal_part) {
  char *dot = strchr(part, '.');

  char *exponenta = strchr(part, 'e');

  *scale = 0;
  *decimal_part = 0;
  *length = 0;
  // записываем значени экспоненты
  if (exponenta) {
    *exponenta++ = '\0';
    *scale = atoi(exponenta);
  }

  // записываем значение после точки
  if (dot) {
    *dot++ = '\0';
    *length = strlen(dot);
    *decimal_part = atoi(dot);
  }
}

int s21_from_decimal_to_float(s21_decimal src, float *dst) {
  int status = S21_OK;
  unsigned int scale = s21_get_decimal_scale(src);

  if (!dst || 28 < scale) {
    status = S21_ERROR;
  } else {
    *dst = 0.f;

    for (int i = 0; i < 96; ++i) {
      if (s21_get_decimal_bit(src, i)) {
        *dst += powf(2, i);
      }
    }

    *dst /= powf(10, scale);
    if (s21_get_sign(src)) {
      *dst *= -1;
    }
  }

  return status;
}
