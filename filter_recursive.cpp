#include "filter_recursive.h"
#include "math.h"
#include "stdint.h"

FR_err_t FR_LowPass_SinglePole_Init(FR_LowPass_SinglePole_t* filter) {
  float cut_freq = filter->cut_freq_hz;

  float b1 = powf(FR_E, -2.0f * FR_PI * cut_freq);
  float a0 = 1.0 - b1;

  filter->a0 = a0;
  filter->b1 = b1;

  filter->yn1 = 0.0f;

  return FR_OK;
}

FR_err_t FR_LowPass_SinglePole_Run(FR_LowPass_SinglePole_t* filter, float input, float* output) {
  float yn = (filter->a0) * input + (filter->b1) * (filter->yn1);
  filter->yn1 = yn;
  *output = yn;

  return FR_OK;
}

FR_err_t FR_HighPass_SinglePole_Init(FR_HighPass_SinglePole_t* filter) {
  float cut_freq = filter->cut_freq_hz;

  float x = powf(FR_E, -2.0f * FR_PI * cut_freq);

  filter->a0 = (1.0f + x) / 2.0f;
  filter->a1 = (filter->a0) * (-1.0f);
  filter->b1 = x;

  filter->xn1 = 0.0f;
  filter->yn1 = 0.0f;
  return FR_OK;
}

FR_err_t FR_HighPass_SinglePole_Run(FR_HighPass_SinglePole_t* filter, float input, float* output) {
  float yn = (filter->a0) * input + (filter->a1) * (filter->xn1) + (filter->b1) * (filter->yn1);
  filter->xn1 = input;
  filter->yn1 = yn;
  *output = yn;
  return FR_OK;
}

FR_err_t FR_LowPass_SinglePole_FourStage_Init(FR_LowPass_SinglePole_FourStage_t* filter) {
  float cut_freq = filter->cut_freq_hz;

  float x = powf(FR_E, -2.0f * FR_PI * cut_freq);
  float x2 = x * x;
  float x3 = x2 * x;
  float x4 = x3 * x;

  filter->a0 = (1.0 - x) * (1.0 - x) * (1.0 - x) * (1.0 - x);
  filter->b1 = 4.0f * x;
  filter->b2 = -6.0f * x2;
  filter->b3 = 4.0f * x3;
  filter->b4 = -1.0f * x4;

  filter->yn1 = 0.0f;
  filter->yn2 = 0.0f;
  filter->yn3 = 0.0f;
  filter->yn4 = 0.0f;

  return FR_OK;
}

FR_err_t FR_LowPass_SinglePole_FourStage_Run(FR_LowPass_SinglePole_FourStage_t* filter, float input, float* output) {
  float yn = (filter->a0) * input + (filter->b1) * (filter->yn1) + (filter->b2) * (filter->yn2) + (filter->b3) * (filter->yn3) + (filter->b4) * (filter->yn4);
  filter->yn4 = filter->yn3;
  filter->yn3 = filter->yn2;
  filter->yn2 = filter->yn1;
  filter->yn1 = yn;
  *output = yn;

  return FR_OK;
}

FR_err_t FR_BandPass_Init(FR_NarrowBand_t* filter) {
  float fc = filter->center_freq_hz;
  float bw = filter->bandwidth_hz;
  float sample_freq = filter->sample_freq_hz;

  float fcn = fc / sample_freq;
  float bwn = bw / sample_freq;

  bool fcn_in_range = ((fcn > 0.0f) && (fcn < 0.5f));
  bool bwn_in_range = ((bwn > 0.0f) && (bwn < 0.5f));

  if (!fcn_in_range)
    return FR_FCN_NOT_IN_RANGE;
  if (!bwn_in_range)
    return FR_BWN_NOT_IN_RANGE;

  float cos2pifcn = cosf(2.0f * FR_PI * fcn);
  float R = 1.0f - 3.0f * bwn;
  float K = (1.0f - 2.0f * R * cos2pifcn + R * R) / (2.0f - 2.0f * cos2pifcn);

  filter->a0 = 1.0f - K;
  filter->a1 = 2.0f * (K - R) * cos2pifcn;
  filter->a2 = R * R - K;
  filter->b1 = 2.0f * R * cos2pifcn;
  filter->b2 = -1.0f * R * R;

  filter->xn1 = 0.0f;
  filter->xn2 = 0.0f;
  filter->yn1 = 0.0f;
  filter->yn2 = 0.0f;

  return FR_OK;
}

FR_err_t FR_BandPass_Run(FR_NarrowBand_t* filter, float input, float* output) {
  float yn = (filter->a0) * input + (filter->a1) * (filter->xn1) + (filter->a2) * (filter->xn2) + (filter->b1) * (filter->yn1) + (filter->b2) * (filter->yn2);
  filter->yn2 = filter->yn1;
  filter->yn1 = yn;
  filter->xn2 = filter->xn1;
  filter->xn1 = input;

  *output = yn * 0.5f;  // to investigate why "*0.5f"

  return FR_OK;
}

FR_err_t FR_BandReject_Init(FR_NarrowBand_t* filter) {
  float fc = filter->center_freq_hz;
  float bw = filter->bandwidth_hz;
  float sample_freq = filter->sample_freq_hz;

  float fcn = fc / sample_freq;
  float bwn = bw / sample_freq;

  bool fcn_in_range = ((fcn > 0.0f) && (fcn < 0.5f));
  bool bwn_in_range = ((bwn > 0.0f) && (bwn < 0.5f));

  if (!fcn_in_range)
    return FR_FCN_NOT_IN_RANGE;
  if (!bwn_in_range)
    return FR_BWN_NOT_IN_RANGE;

  float cos2pifcn = cosf(2.0f * FR_PI * fcn);
  float R = 1.0f - 3.0f * bwn;
  float K = (1.0f - 2.0f * R * cos2pifcn + R * R) / (2.0f - 2.0f * cos2pifcn);

  filter->a0 = K;
  filter->a1 = -2.0f * K * cos2pifcn;
  filter->a2 = K;
  filter->b1 = 2.0f * R * cos2pifcn;
  filter->b2 = -1.0f * R * R;

  filter->xn1 = 0.0f;
  filter->xn2 = 0.0f;
  filter->yn1 = 0.0f;
  filter->yn2 = 0.0f;

  return FR_OK;
}

FR_err_t FR_BandReject_Run(FR_NarrowBand_t* filter, float input, float* output) {
  float yn = (filter->a0) * input + (filter->a1) * (filter->xn1) + (filter->a2) * (filter->xn2) + (filter->b1) * (filter->yn1) + (filter->b2) * (filter->yn2);
  filter->yn2 = filter->yn1;
  filter->yn1 = yn;
  filter->xn2 = filter->xn1;
  filter->xn1 = input;

  *output = yn * 0.5f;  // to investigate why "*0.5f"

  return FR_OK;
}

FR_err_t FR_DebounceSwitch_Run(FR_DebounceState_t input, FR_DebounceState_t* output)
{
  static uint8_t y_old = 0, flag = 0;
  uint8_t temp;
  //digital filter part  y_old = x_new*0.25 + y_old*0.75
  temp = (y_old >> 2);                                //this gives y_old/4
  y_old = y_old - temp;                               //do (y_old*0.75) by subtraction
                                                      //if button is pushed, add 0.25 (3F) of new value (1.0)
  if (input == FR_PRESSED) { y_old = y_old + 0x3F; }  //

  //software schmitt trigger
  if ((y_old > 0xF0) && (flag == 0)) {
    flag = 1;
    *output = FR_PRESSED;  //output = 1;
  }
  if ((y_old < 0x0F) && (flag == 1)) {
    flag = 0;
    *output = FR_NOT_PRESSED;  //output = 0;
  }

  return FR_OK;
}
