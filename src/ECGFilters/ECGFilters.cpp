#include <ECGFilters.h>

#include <stdlib.h>
#include <math.h>

const int16_t bandpass_filter::a[3] = {-3, 3, -1};
const int16_t bandpass_filter::b[49] = {1, 2, 0, -2, -1, 0, -2, -4, 0, 4, 2, 0, 1, 2, 0, -2,
				    -33, -32, 64, 64, -32, -32, 64, 64, -128, -128, 64,
				    64, -32, -32, 64, 64, -33, -34, 0, 2, 1, 0, 2, 4, 0,
				    -4, -2, 0, -1, -2, 0, 2, 1};




inline float logistic(int32_t value, float scale)
{
  float scaled_value = value / scale;
  return (scaled_value / sqrt(1 + scaled_value * scaled_value) + 1.0) / 2.0;
}



qrs_filter::qrs_filter()
{
  filter_1 = bandpass_filter();
  
  for(int16_t i=0; i < 31; i++) { x_history_1[i] = 0; }
  for(int16_t i=0; i < 20; i++) { x_history_2[i] = 0; }
  for(int16_t i=0; i < 20; i++) { x_history_3[i] = 0; }
  
  filter_2 = ma_filter(31, x_history_1);
  resp_phase_mean_filter = ma_filter(20, x_history_2);
  mean_abs_filter = ma_filter(20, x_history_3);
  
  max_value = 0;
  state = 0;
}

return_value qrs_filter::step(int16_t x)
{
  int32_t bandpass_value = filter_1.step(x) / 200;
  int32_t square_value = bandpass_value * bandpass_value;
  int32_t averaged_value = filter_2.step(square_value) / 10000;
  
  int32_t respiration_phase = -1;
  int32_t mean_respiration_phase = -1;
  int32_t relative_respiration_phase = -1;
  int32_t abs_relative_respiration_phase = -1;
  int32_t mean_abs_relative_respiration_phase = -1;
  float squashed_respiration_phase_float = -1.0;
  int32_t squashed_respiration_phase = -1;
  if (state >= 50)
  {
    respiration_phase = max_value;
    mean_respiration_phase = resp_phase_mean_filter.step(respiration_phase);
    relative_respiration_phase = respiration_phase - mean_respiration_phase;
    abs_relative_respiration_phase = abs(relative_respiration_phase);
    mean_abs_relative_respiration_phase = mean_abs_filter.step(abs_relative_respiration_phase);
    
    relative_respiration_phase = relative_respiration_phase / (mean_abs_relative_respiration_phase / 100);
    squashed_respiration_phase_float = logistic(relative_respiration_phase, 100.0);
    squashed_respiration_phase = squashed_respiration_phase_float * 255.0;
    
    max_value = 0;
    state = 0;
  }
  else if (state > 0)
  {
    if (averaged_value > max_value)
    {
      max_value = averaged_value;
    }
    state = state + 1;
  }
  else if (averaged_value >= 3000)
  {
    state = 1;
  }
  
  return_value ret = {squashed_respiration_phase, averaged_value};
  return ret;
}

	
bandpass_filter::bandpass_filter()
{
  for(int16_t i=0; i < b_length; i++)
    {
      x_history[i] = 0;
    }
  
  for(int16_t i=0; i < a_length; i++)
    {
      y_history[i] = 0;
    }
}


int32_t bandpass_filter::step(int16_t x)
{
  for(int16_t i=b_length-1; i > 0; i--)
    {
      x_history[i] = x_history[i - 1];
    }
  
  x_history[0] = x;
  
  int32_t y = 0;
  for(int16_t i=0; i < b_length; i++)
    {
      y = y + b[i] * x_history[i];
    }
  
  for(int16_t i=0; i < a_length; i++)
    {
      y = y - a[i] * y_history[i];
    }
  
  for(int16_t i=a_length-1; i > 0; i--)
    {
      y_history[i] = y_history[i - 1];
    }
  
  y_history[0] = y;
  
  return y;
}

ma_filter::ma_filter()
{
}

ma_filter::ma_filter(int16_t history_length, int32_t *history)
{
  x_history_length = history_length;
  x_history = history;
  ma_value = 0;
}

int32_t ma_filter::step(int32_t x)
{
  for(int16_t i = x_history_length - 1; i > 0; i--)
    {
      x_history[i] = x_history[i -1];
    }
  
  x_history[0] = x / x_history_length;
  
  ma_value = ma_value + x_history[0] - x_history[x_history_length - 1];
  return ma_value;
}


inline void fill_with_zeros(int16_t *array, int16_t array_length)
{
  for(int16_t i=0; i < array_length; i++)
    {
    array[i] = 0;
    }
}


