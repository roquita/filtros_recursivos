#ifndef FILTER_RECURSIVE
#define FILTER_RECURSIVE

#define FR_PI (3.14159265f)  // pi
#define FR_E (2.718281f)     // epsilon

typedef enum {
  FR_U8,
  FR_I8,
  FR_U32,
  FR_I32,
  FR_FLOAT,
  FR_DOUBLE,
} FR_Datatype_t;

typedef struct {
  float cut_freq_hz;
  float a0;
  float b1;
  float yn1;
} FR_LowPass_SinglePole_t;

typedef struct {
  float cut_freq_hz;
  float a0;
  float a1;
  float b1;
  float xn1;
  float yn1;
} FR_HighPass_SinglePole_t;

typedef struct {
  float cut_freq_hz;
  float a0;
  float b1;
  float b2;
  float b3;
  float b4;
  float yn1;
  float yn2;
  float yn3;
  float yn4;
} FR_LowPass_SinglePole_FourStage_t;

typedef struct {
  float sample_freq_hz;
  float center_freq_hz;
  float bandwidth_hz;
  float a0;
  float a1;
  float a2;
  float b1;
  float b2;
  float xn1;
  float xn2;
  float yn1;
  float yn2;
} FR_NarrowBand_t;

typedef enum{
  FR_PRESSED,
  FR_NOT_PRESSED,
}FR_DebounceState_t;

typedef enum {
  FR_OK,
  FR_INVALID_DATATYPE,
  FR_FCN_NOT_IN_RANGE,
  FR_BWN_NOT_IN_RANGE,
} FR_err_t;

// LOW PASS FOR SMOOTING
FR_err_t FR_LowPass_SinglePole_Init(FR_LowPass_SinglePole_t* filter);
FR_err_t FR_LowPass_SinglePole_Run(FR_LowPass_SinglePole_t* filter, float input, float* output);

// HIGH PASS
FR_err_t FR_HighPass_SinglePole_Init(FR_HighPass_SinglePole_t* filter);
FR_err_t FR_HighPass_SinglePole_Run(FR_HighPass_SinglePole_t* filter, float input, float* output);

// LOW PASS IMPROVED
FR_err_t FR_LowPass_SinglePole_FourStage_Init(FR_LowPass_SinglePole_FourStage_t* filter);
FR_err_t FR_LowPass_SinglePole_FourStage_Run(FR_LowPass_SinglePole_FourStage_t* filter, float input, float* output);

// BAND PASS
FR_err_t FR_BandPass_Init(FR_NarrowBand_t* filter);
FR_err_t FR_BandPass_Run(FR_NarrowBand_t* filter, float input, float* output);

// BAND REJECT
FR_err_t FR_BandReject_Init(FR_NarrowBand_t* filter);
FR_err_t FR_BandReject_Run(FR_NarrowBand_t* filter, float input, float* output);

// DEBOUNCE SWITCH
FR_err_t FR_DebounceSwitch_Run(FR_DebounceState_t input, FR_DebounceState_t* output);

#endif