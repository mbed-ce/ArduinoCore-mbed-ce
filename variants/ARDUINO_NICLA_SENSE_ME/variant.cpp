#include "Arduino.h"
#include "pinDefinitions.h"

/* wiring_analog variables definition */
/* Flag to indicate whether the ADC config has been changed from the default one */
bool isAdcConfigChanged = false;

#ifdef ANALOG_CONFIG
/* 
 * Configuration used for all the active ADC channels, it is initialized with the mbed default values
 * When it is changed, all the ADC channels are reconfigured accordingly 
 */
analogin_config_t adcCurrentConfig = {
    .resistor_p = NRF_SAADC_RESISTOR_DISABLED,
    .resistor_n = NRF_SAADC_RESISTOR_DISABLED,
    .gain       = NRF_SAADC_GAIN1_4,
    .reference  = NRF_SAADC_REFERENCE_VDD4,
    .acq_time   = NRF_SAADC_ACQTIME_10US,
    .mode       = NRF_SAADC_MODE_SINGLE_ENDED,
    .burst      = NRF_SAADC_BURST_DISABLED,
    .pin_p      = NRF_SAADC_INPUT_DISABLED,
    .pin_n      = NRF_SAADC_INPUT_DISABLED
};

void analogReference(uint8_t mode)
{
  nrf_saadc_reference_t reference = NRF_SAADC_REFERENCE_VDD4;
  nrf_saadc_gain_t gain = NRF_SAADC_GAIN1_4;
  if (mode == AR_VDD) {
    reference = NRF_SAADC_REFERENCE_VDD4;
    gain = NRF_SAADC_GAIN1_4;
  } else if (mode == AR_INTERNAL) {
    reference = NRF_SAADC_REFERENCE_INTERNAL;
    gain = NRF_SAADC_GAIN1;
  } else if (mode == AR_INTERNAL1V2) {
    reference = NRF_SAADC_REFERENCE_INTERNAL;
    gain = NRF_SAADC_GAIN1_2;
  } else if (mode == AR_INTERNAL2V4) {
    reference = NRF_SAADC_REFERENCE_INTERNAL;
    gain = NRF_SAADC_GAIN1_4;
  }
  adcCurrentConfig.reference = reference;
  adcCurrentConfig.gain = gain;
  analogUpdate();
}

void analogAcquisitionTime(uint8_t time)
{
  nrf_saadc_acqtime_t acqTime = NRF_SAADC_ACQTIME_10US;
  if (time == AT_3_US) {
    acqTime = NRF_SAADC_ACQTIME_3US;
  } else if (time == AT_5_US) {
    acqTime = NRF_SAADC_ACQTIME_5US;
  } else if (time == AT_10_US) {
    acqTime = NRF_SAADC_ACQTIME_10US;
  } else if (time == AT_15_US) {
    acqTime = NRF_SAADC_ACQTIME_15US;
  } else if (time == AT_20_US) {
    acqTime = NRF_SAADC_ACQTIME_20US;
  } else if (time == AT_40_US) {
    acqTime = NRF_SAADC_ACQTIME_40US;
  }
  adcCurrentConfig.acq_time = acqTime;
  analogUpdate();
}
#endif

AnalogPinDescription g_AAnalogPinDescription[] = {
    // A0 - A7
  { P0_2,  NULL },    // A0
  { P0_30,  NULL }    // A1
};

PinDescription g_APinDescription[] = {

  { P0_10, NULL, NULL, NULL },    // 0: GPIO3
  { P0_9, NULL, NULL, NULL },     // 1: GPIO2/RX
  { P0_20, NULL, NULL, NULL },    // 2: GPIO1/TX
  { P0_23, NULL, NULL, NULL },    // 3: SCL1
  { P0_22, NULL, NULL, NULL },    // 4: SDA1


  { P0_24, NULL, NULL, NULL },    // 5: GPIO0
  { P0_29, NULL, NULL, NULL },    // 6: CS
  { P0_28, NULL, NULL, NULL },    // 7: CIPO
  { P0_27, NULL, NULL, NULL },    // 8: COPI
  { P0_11, NULL, NULL, NULL },    // 9: SCLK

  { P0_2, NULL, NULL, NULL },     // 10: A0
  { P0_30, NULL, NULL, NULL },    // 11: A1

  { P0_19, NULL, NULL, NULL },    // 12: INT ESLOV
  { P0_18, NULL, NULL, NULL },    // 13: Reset BHI260
  { P0_14, NULL, NULL, NULL },    // 14: INT BHI260
  { P0_25, NULL, NULL, NULL },    // 15: BQ25120 CD
  { P0_26, NULL, NULL, NULL },    // 16: CS FLASH
  { P0_31, NULL, NULL, NULL },    // 17: CS BHI260  
};

extern "C" {
  unsigned int PINCOUNT_fn() {
    return (sizeof(g_APinDescription) / sizeof(g_APinDescription[0]));
  }
}

mbed::InterruptIn button(BUTTON1);

void resetFunc()
{
  NVIC_SystemReset();
}

#include "nrf_rtc.h"
#include "nrf_uarte.h"
#include "nrf_uart.h"

void initVariant() {

  CoreDebug->DEMCR = 0;
  NRF_CLOCK->TRACECONFIG = 0;

  NRF_PWM_Type* PWM[] = {
    NRF_PWM0, NRF_PWM1, NRF_PWM2
#ifdef NRF_PWM3
    ,NRF_PWM3
#endif
  };

  for (unsigned int i = 0; i < (sizeof(PWM)/sizeof(PWM[0])); i++) {
    PWM[i]->ENABLE = 0;
    PWM[i]->PSEL.OUT[0] = 0xFFFFFFFFUL;
  } 

  button.fall(&resetFunc);
}
