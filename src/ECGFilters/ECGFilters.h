#ifndef ECGFilters_h
#define ECGFilters_h

#include <stdint.h>

struct return_value {
   int32_t respiration_phase;
   int32_t signal_value;
};


class bandpass_filter
{
 public:
  bandpass_filter();
  int32_t step(int16_t);
 private:
  static const int16_t b[49];
  static const int16_t a[3];
  const static int16_t b_length = 49;
  const static int16_t a_length = 3;
  const static int16_t gain = 30 * 30 * 8;
  int16_t x_history[49];
  int32_t y_history[3];
};

class ma_filter
{
 public:
  ma_filter();
  ma_filter(int16_t history_length, int32_t *history);
  int32_t step(int32_t);
 private:
  int32_t *x_history;
  int16_t x_history_length;
  int32_t ma_value;
};


class qrs_filter
{
 public:
  qrs_filter();
  return_value step(int16_t);
 private:
  bandpass_filter filter_1;
  ma_filter filter_2;
  ma_filter resp_phase_mean_filter;
  ma_filter mean_abs_filter;
  int32_t max_value;
  int32_t state;
  int32_t x_history_1[31];
  int32_t x_history_2[20];
  int32_t x_history_3[20];
};

#endif
