#include "s21_decimal.h"

int s21_is_less(s21_decimal x1, s21_decimal x2) {
  int res = s21_comparing(&x2, &x1);
  // если 1, то x2 > x1 (x1 < x2)
  if (res == -1) res = 0;
  return res;
}

int s21_is_less_or_equal(s21_decimal x1, s21_decimal x2) {
  int res = s21_comparing(&x2, &x1);
  if (res == 0)
    res = 1;
  else if (res == -1)
    res = 0;
  return res;
}

int s21_is_greater(s21_decimal x1, s21_decimal x2) {
  int res = s21_comparing(&x1, &x2);
  if (res == -1) res = 0;
  return res;
}

int s21_is_greater_or_equal(s21_decimal x1, s21_decimal x2) {
  int res = s21_comparing(&x1, &x2);
  if (res == 0)
    res = 1;
  else if (res == -1)
    res = 0;
  return res;
}

int s21_is_equal(s21_decimal x1, s21_decimal x2) {
  return (s21_comparing(&x1, &x2)) ? 0 : 1;
}

int s21_is_not_equal(s21_decimal x1, s21_decimal x2) {
  return (s21_comparing(&x1, &x2)) ? 1 : 0;
}
