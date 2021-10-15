#ifdef WM_8960
#define WM_8960
include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/spi_master.h"
#include "driver/gpio.h"
#include "esp_err.h"
#include "esp_log.h"

#define CHECK(x) do { esp_err_t __; if ((__ = x) != ESP_OK) return __; } while (0)
#define CHECK_ARG(VAL) do { if (!(VAL)) return ESP_ERR_INVALID_ARG; } while (0)

#define R0_LEFT_INPUT_VOLUME 0x00
#define R1_RIGHT_INPUT_VOLUME 0x01
#define R2_LOUT1_VOLUME 0x02
#define R3_ROUT1_VOLUME 0x03
#define R4_CLOCKING_1 0x04
#define R5_ADC_DAC_CONTROL_CTR1 0x05
#define R6_ADC_DAC_CONTROL_CTR2 0x06
#define R7_AUDIO_INTERFACE_1 0x07
#define R8_CLOCKING_2 0x08
#define R10_LEFT_DAC_VOLUME 0x09
#define R11_RIGHT_DAC_VOLUME 0x0A
#define R15_RESET 0x0F
#define R16_3D_CONTROL 0x10
#define R17_ALC1 0x11
#define R18_ALC2 0x12
#define R19_ALC3 0x13
#define R20_NOISE_GATE 0x14
#define R21_LEFT_ADC_VOLUME 0x15
#define R22_RIGHT_ADC_VOLUME 0x16
#define R23_ADDITIONAL_CONTROL_1 0x17
#define R24_ADDITIONAL_CONTROL_2 0x18
#define R25_PWR_MGMT_1 0x19
#define R26_PWR_MGMT_2 0x1A
#define R27_ADDITIONAL_CONTROL_3 0x1B
#define R28_ADDITIONAL_CONTROL_2 0x1C





#endif