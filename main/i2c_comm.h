#ifndef WM_8960
#define WM_8960
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/spi_master.h"
#include "driver/gpio.h"
#include "esp_err.h"
#include "esp_log.h"

#define CHECK(x) do { esp_err_t _; if ((_ = x) != ESP_OK) return __; } while (0)
#define CHECK_ARG(VAL) do { if (!(VAL)) return ESP_ERR_INVALID_ARG; } while (0)

#define R0_LEFT_INPUT_VOLUME_ADR 0x00
#define R1_RIGHT_INPUT_VOLUME_ADR 0x01
#define R2_LOUT1_VOLUME_ADR 0x02
#define R3_ROUT1_VOLUME_ADR 0x03
#define R4_CLOCKING_1_ADR 0x04
#define R5_ADC_DAC_CONTROL_CTR1_ADR 0x05
#define R6_ADC_DAC_CONTROL_CTR2_ADR 0x06
#define R7_AUDIO_INTERFACE_1_ADR 0x07
#define R8_CLOCKING_2_ADR 0x08
#define R10_LEFT_DAC_VOLUME_ADR 0x09
#define R11_RIGHT_DAC_VOLUME_ADR 0x0A
#define R15_RESET_ADR 0x0F
#define R16_3D_CONTROL_ADR 0x10
#define R17_ALC1_ADR 0x11
#define R18_ALC2_ADR 0x12
#define R19_ALC3_ADR 0x13
#define R20_NOISE_GATE_ADR 0x14
#define R21_LEFT_ADC_VOLUME_ADR 0x15
#define R22_RIGHT_ADC_VOLUME_ADR 0x16
#define R23_ADDITIONAL_CONTROL_1_ADR 0x17
#define R24_ADDITIONAL_CONTROL_2_ADR 0x18
#define R25_PWR_MGMT_1_ADR 0x19
#define R26_PWR_MGMT_2_ADR 0x1A
#define R27_ADDITIONAL_CONTROL_3_ADR 0x1B
#define R28_ANTI_POP_1_ADR 0x1C
#define R29_ANTI_POP_2_ADR 0x1D
#define R32_ADCL_SIGNAL_PATH 0x20
#define R33_ADCR_SIGNAL_PATH 0x21 
#define R34_LEFT_OUT_MIX_2 0x22 
#define R37_RIGHT_OUT_MIX_2 0x23 
#define R38_MONO_OUT_MIX_1 0x26
#define R39_MONO_OUT_MIX_2 0x27 
#define R40_LOUT2_VOLUME 0x28 
#define R41_ROUT2_VOLUME 0x29
#define R42_MONOOUT_VOLUME 0x2A
#define R43_INPUT_BOOST_MIXER_1 0x2B
#define R44_INPUT_BOOST_MIXER_2 0x2C
#define R45_BYPASS_1 0x2D
#define R46_BYPASS_2 0x2E
#define R47_PWR_MGMT_3 0x2F
#define R48_ADDITONAL_CTRL_4 0x30
#define R49_CLASS_D_CTRL_1 0x31 
#define R51_CLASS_D_CTRL_3 0x33


typedef struct R0_LEFT_INPUT_VOLUME_t
{
    uint8_t LINVOL  :6; //Bits 5:0
    uint8_t LIZC    :1; //Bits 6
    uint8_t LINMUTE :1; //Bits 7
    uint8_t IPUV    :1; //Bits 8

} _attribute_((packed, aligned(2))) R0_LEFT_INPUT_VOLUME_t;


typedef struct R1_RIGHT_INPUT_VOLUME_t
{
    uint8_t RINVOL  :6; //Bits 5:0
    uint8_t RIZC    :1; //Bits 6
    uint8_t RINMUTE :1; //Bits 7
    uint8_t IPUV    :1; //Bits 8
    
} _attribute_((packed, aligned(2))) R1_RIGHT_INPUT_VOLUME_t;


typedef struct R2_LOUT1_VOLUME_t
{
    uint8_t LOUT1VOL    :7; //Bits 6:0
    uint8_t LO1ZC       :1; //Bits 7
    uint8_t OUT1VU      :1; //Bits 8
} _attribute_((packed, aligned(2))) R2_LOUT1_VOLUME_t;

typedef struct R3_ROUT1_VOLUME_t
{
    uint8_t ROUT1VOL    :7; //Bits 6:0
    uint8_t RO1ZC       :1; //Bits 7
    uint8_t OUT1VU      :1; //Bits 8
} _attribute_((packed, aligned(2))) R3_ROUT1_VOLUME_t;

typedef struct R4_CLOCKING_1_t{

    uint8_t ADCDIV      :3; //Bits 8:6
    uint8_t DACDIV      :3; //Bits 5:3
    uint8_t SYSCLKDIV   :2; //Bits 2:1
    uint8_t CLKSEL      :1; //Bits 0
} _attribute_((packed, aligned(2))) R4_CLOCKING_1_t;

typedef struct R5_ADC_DAC_CONTROL_CTR1_t{

    uint8_t ADCHPD      :1; //Bits 0
    uint8_t DEEMPH      :2; //Bits 2:1
    uint8_t DACMU       :1; //Bits 3
    uint8_t ADCPOL      :2; //Bits 6:5
    uint8_t DACDIV2     :1; //Bits 7
}_attribute_((packed, aligned(2))) R5_ADC_DAC_CONTROL_CTR1_t ;

typedef struct R6_ADC_DAC_CONTROL_CTR2_t{

    uint8_t DACSLOPE    :1; //Bits 1
    uint8_t DACMR       :1; //Bits 2
    uint8_t DACSMM      :1; //Bits 3
    uint8_t DACPOL      :2; //Bits 6:5
}_attribute_((packed, aligned(2))) R6_ADC_DAC_CONTROL_CTR2_t;

typedef struct R7_AUDIO_INTERFACE_t{
    uint8_t FORMAT       :2; // bit 1:0
    uint8_t WL           :2; // bit 1:0
    uint8_t LRP          :1; // bit 4
    uint8_t DLRSWAP      :1; // bit 5
    uint8_t MS           :1; // bit 6
    uint8_t BCLKINV      :1; // bit 7
    uint8_t ALRSWAP      :1; // bit 8
}_attribute_((packed, aligned(2))) R7_AUDIO_INTERFACE_t;

typedef struct R8_CLOCKING_2_t{
    uint8_t BCLKDIV      :4; // bit 3:0
    uint8_t DCLKDIV      :3; // bit 2:0
}_attribute_((packed, aligned(2))) R8_CLOCKING_2_t;

typedef struct R9_AUDIO_INTERFACE_t{
    uint8_t LOOPBACK     :1; // bit 0
    uint8_t ADCCOMP      :2; // bits 1:0
    uint8_t DACCOMP      :2; // bits 1:0
    uint8_t WL8          :1; // bit 5
    uint8_t ALRCGPIO     :1; // bit 6
}_attribute_((packed, aligned(2))) R9_AUDIO_INTERFACE_t;

typedef struct R10_LEFT_DAC_VOLUME_t{
    uint8_t LDACVOL      :8; // bit 7:0
    uint8_t DACVU        :1; // bit 8
}_attribute_((packed, aligned(2))) R10_LEFT_DAC_VOLUME_t;

typedef struct R11_RIGHT_DAC_VOLUME_t{
    uint8_t RDACVOL      :8; // bit 7:0
    uint8_t DACVU        :1; // bit 8
}_attribute_((packed, aligned(2))) R11_RIGHT_DAC_VOLUME_t;

// typedef struct R15_RESET_t{};

typedef struct R16_3D_CONTROL_t{
    uint8_t D3EN         :1; // bit 0 
    uint8_t D3DEPTH      :4; // bits 3:0
    uint8_t D3LC         :1; // bit 5
    uint8_t D3UC         :1; // bit 6
}_attribute_((packed, aligned(2))) R16_3D_CONTROL_t;

typedef struct R17_ALC1_t{
    uint8_t ALCL         :4; // bits 3:0
    uint8_t MAXGAIN      :3; // bits 2:0
    uint8_t ALCSEL       :2; // bits 1:0
}_attribute_((packed, aligned(2))) R17_ALC1_t;

typedef struct R17_ALC1_t{
    uint8_t HLD          :4; // bits 3:0
    uint8_t MINGAIN      :3; // bits 2:0
}_attribute_((packed, aligned(2))) R17_ALC1_t;

typedef struct R19_ALC3_t{
    uint8_t ATK          :4; // bits 3:0
    uint8_t DCY          :4; // bits 3:0
}_attribute_((packed, aligned(2))) R19_ALC3_t;

typedef struct R20_NOISE_GATE_t{
    uint8_t NGAT         :1; // bit 0
    uint8_t NGTH         :5; // bit 4:0
}_attribute_((packed, aligned(2))) R20_NOISE_GATE_t;

typedef struct R21_LEFT_ADC_VOLUME_t{
    uint8_t LADCVOL      :8; // bits 7:0
    uint8_t ADCVU        :1; // bit  8
}_attribute_((packed, aligned(2))) R21_LEFT_ADC_VOLUME_t;

typedef struct R22_RIGHT_ADC_VOLUME_t{
    uint8_t RADCVOL      :8; // bits 7:0
    uint8_t ADCVU        :1; // bit  8
}_attribute_((packed, aligned(2))) R22_RIGHT_ADC_VOLUME_t;

typedef struct R23_ADDITIONAL_CONTROL_1_t{
    uint8_t TOEN         :1; // bit 1
    uint8_t TOCLKSEL     :1; // bit  2
    uint8_t DATSEL       :2; // bits 1:0
    uint8_t DMONOMIX     :1; // bit 4
    uint8_t VSEL         :2; // bits 1:0
    uint8_t TSDEN        :1; // bit 8
}_attribute_((packed, aligned(2))) R23_ADDITIONAL_CONTROL_1_t;

typedef struct R24_ADDITIONAL_CONTROL_2_t{
    uint8_t LRCM         :1; // bit 2
    uint8_t TRIS         :1; // bit 3
    uint8_t HPSWPOL      :1; // bit 5
    uint8_t HPSWEN       :1; // bit 6
}_attribute_((packed, aligned(2))) R24_ADDITIONAL_CONTROL_2_t;

typedef struct R25_PWR_MGMT_1_t{
    uint8_t DIGENB       :1; // bit 0
    uint8_t MICB         :1; // bit 1
    uint8_t ADCR         :1; // bit 2
    uint8_t ADCL         :1; // bit 3
    uint8_t AINR         :1; // bit 4
    uint8_t AINL         :1; // bit 5
    uint8_t VREF         :1; // bit 6
    uint8_t VMIDSEL      :2; // bit 1:0
}_attribute_((packed, aligned(2))) R25_PWR_MGMT_1_t;

typedef struct R26_PWR_MGMT_2_t{
    uint8_t PLL_EN       :1; // bit 0
    uint8_t OUT3         :1; // bit 1
    uint8_t SPKR         :1; // bit 3
    uint8_t SPKL         :1; // bit 4
    uint8_t ROUT1        :1; // bit 5
    uint8_t LOUT1        :1; // bit 6
    uint8_t DACR         :1; // bit 7
    uint8_t DACL         :1; // bit 8
}_attribute_((packed, aligned(2))) R26_PWR_MGMT_2_t;

typedef struct R27_ADDITIONAL_CONTROL_3_t{
    uint8_t ADC_ALC_SR   :3; // bits 2:0
    uint8_t OUT3CAP      :1; // bit 3
    uint8_t VROI         :1; // bit 6
}_attribute_((packed, aligned(2))) R27_ADDITIONAL_CONTROL_3_t;

typedef struct R28_ANTI_POP_1_t{
    uint8_t HPSTBY       :1; // bit 0
    uint8_t SOFT_ST      :1; // bit 2
    uint8_t BUFIOEN      :1; // bit 3
    uint8_t BUFDCOPEN    :1; // bit 4
    uint8_t POBCTRL      :1; // bit 7
}_attribute_((packed, aligned(2))) R28_ANTI_POP_1_t;

typedef struct R29_ANTI_POP_2_t{
    uint8_t DRES         :2; // bits 1:0
    uint8_t DISOP        :1; // bit 5
}_attribute_((packed, aligned(2))) R29_ANTI_POP_2_t;

typedef struct R32_ADCL_SIGNAL_PATH_t{
    uint8_t LMIC2B       :1; // bit 3
    uint8_t LMICBOOST    :2; // bits 1:0
    uint8_t LMP2         :1; // bit 6
    uint8_t LMP3         :1; // bit 7
    uint8_t LMN1         :1; // bit 8
}_attribute_((packed, aligned(2))) R32_ADCL_SIGNAL_PATH_t;

typedef struct R33_ADCR_SIGNAL_PATH_t{
    uint8_t RMIC2B       :1; // bit 3
    uint8_t RMICBOOST    :2; // bits 1:0
    uint8_t RMP2         :1; // bit 6
    uint8_t RMP3         :1; // bit 7
    uint8_t RMN1         :1; // bit 8
}_attribute_((packed, aligned(2))) R33_ADCR_SIGNAL_PATH_t;

typedef struct R34_LEFT_OUT_MIX_1_t{
    uint8_t LI2LOVOL     :3; // bits 2:0
    uint8_t LI2LO        :1; // bit 7
    uint8_t LD2LO        :1; // bit 8
}_attribute_((packed, aligned(2))) R34_LEFT_OUT_MIX_1_t;

typedef struct R37_RIGHT_OUT_MIX_2_t{
    uint8_t RI2ROVOL     :3; // bits 2:0
    uint8_t RI2RO        :1; // bit 7
    uint8_t RD2RO        :1; // bit 8
}_attribute_((packed, aligned(2))) R37_RIGHT_OUT_MIX_2_t;

typedef struct R38_MONO_OUT_MIX_1_t{
    uint8_t L2MO         :1; // bit 7
}_attribute_((packed, aligned(2))) R38_MONO_OUT_MIX_1_t;

typedef struct R39_MONO_OUT_MIX_2_t{
    uint8_t R2MO         :1; // bit 7
}_attribute_((packed, aligned(2))) R39_MONO_OUT_MIX_2_t;

typedef struct R40_LOUT2_VOLUME_t{
    uint8_t SPKLVOL      :7; // bits 6:0
    uint8_t SPKLZC       :1; // bit 7
    uint8_t SPKVU        :1; // bit 8
}_attribute_((packed, aligned(2))) R40_LOUT2_VOLUME_t;

typedef struct R41_ROUT2_VOLUME_t{
    uint8_t SPKRVOL      :7; // bits 6:0
    uint8_t SPKLZC       :1; // bit 7
    uint8_t SPKVU        :1; // bit 8
}_attribute_((packed, aligned(2))) R41_ROUT2_VOLUME_t;

typedef struct R42_MONOOUT_VOLUME_t{
    uint8_t MOUTVOL      :1; //bit 6
}_attribute_((packed, aligned(2))) R42_MONOOUT_VOLUME_t;

typedef struct R43_INPUT_BOOST_MIXER_1_t{
    uint8_t LIN2BOOST    :3; // bits 2:0
    uint8_t LIN3BOOST    :3; // bits 2:0
}_attribute_((packed, aligned(2))) R43_INPUT_BOOST_MIXER_1_t;

typedef struct R44_INPUT_BOOST_MIXER_2_t{
    uint8_t RIN2BOOST    :3; // bits 2:0
    uint8_t RIN3BOOST    :3; // bits 2:0
}_attribute_((packed, aligned(2))) R44_INPUT_BOOST_MIXER_2_t;

typedef struct R45_BYPASS_1_t{
    uint8_t LB2LOVOL     :3; // bits 2:0
    uint8_t LB2LO        :1; // bit 5
}_attribute_((packed, aligned(2))) R45_BYPASS_1_t;

typedef struct R46_BYPASS_2_t{
    uint8_t RB2ROVOL     :3; // bits 2:0
    uint8_t RB2RO        :1; // bit 5
}_attribute_((packed, aligned(2))) R46_BYPASS_2_t;

typedef struct R47_PWR_MGMT_3_t{
    uint8_t ROMIX        :1; // bit 2
    uint8_t LOMIX        :1; // bit 3
    uint8_t RMIC         :1; // bit 4
    uint8_t LMIC         :1; // bit 5
}_attribute_((packed, aligned(2))) R47_PWR_MGMT_3_t;

typedef struct R48_ADDITIONAL_CONTROL_4_t{
    uint8_t MBSEL        :1; // bit 0
    uint8_t TSENSEN      :1; // bit 1
    uint8_t HPSEL        :2; // bits 1:0
    uint8_t GPIOSEL      :3; // bits 2:0
    uint8_t GPIOPOL      :1; // bit 7
}_attribute_((packed, aligned(2))) R48_ADDITIONAL_CONTROL_4_t;

typedef struct R49_CLASS_D_CONTROL_1_t{
    uint8_t SPK_OP_EN    :2; // bits 1:0
}_attribute_((packed, aligned(2))) R49_CLASS_D_CONTROL_1_t;

typedef struct R51_CLASS_D_CONTROL_3_t{
    uint8_t ACGAIN       :3; // bits 2:0
    uint8_t DCGAIN       :3; // bits 2:0
}_attribute_((packed, aligned(2))) R51_CLASS_D_CONTROL_3_t;

typedef struct R52_PLL_N_t{
    uint8_t PLLN         :4; // bits 3:0
    uint8_t PLLRESCALE   :1; // bit 4
    uint8_t SDM          :1; // bit 5
    uint8_t OPCLKDIV     :3; // bits 2:0
}_attribute_((packed, aligned(2))) R52_PLL_N_t;

typedef struct R53_PLL_K_1_t{
    uint8_t PLLK         :8; // bits 23:16 
}_attribute_((packed, aligned(2))) R53_PLL_K_1_t;

typedef struct R54_PLL_K_2_t{
    uint8_t PLLK         :8; // bits 15:8 
}_attribute_((packed, aligned(2))) R54_PLL_K_2_t;

typedef struct R55_PLL_K_3_t{
    uint8_t PLLK         :8; // bits 7:0 
}_attribute_((packed, aligned(2))) R55_PLL_K_3_t;

#endif