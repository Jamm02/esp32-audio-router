#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <esp_log.h>
#include <driver/gpio.h>
#include <esp_err.h>
#include <driver/i2c.h>
typedef struct R7_AUDIO_INTERFACE{
    uint8_t FORMAT       :2; // bit 1:0
    uint8_t WL           :2; // bit 1:0
    uint8_t LRP          :1; // bit 4
    uint8_t DLRSWAP      :1; // bit 5
    uint8_t MS           :1; // bit 6
    uint8_t BCLKINV      :1; // bit 7
    uint8_t ALRSWAP      :1; // bit 8
};

typedef struct R8_CLOCKING_2{
    uint8_t BCLKDIV      :4; // bit 3:0
    uint8_t DCLKDIV      :3; // bit 2:0
};

typedef struct R9_AUDIO_INTERFACE{
    uint8_t LOOPBACK     :1; // bit 0
    uint8_t ADCCOMP      :2; // bits 1:0
    uint8_t DACCOMP      :2; // bits 1:0
    uint8_t WL8          :1; // bit 5
    uint8_t ALRCGPIO     :1; // bit 6
};

typedef struct R10_LEFT_DAC_VOLUME{
    uint8_t LDACVOL      :8; // bit 7:0
    uint8_t DACVU        :1; // bit 8
};

typedef struct R11_RIGHT_DAC_VOLUME{
    uint8_t RDACVOL      :8; // bit 7:0
    uint8_t DACVU        :1; // bit 8
};

// typedef struct R15_RESET{};

typedef struct R16_3D_CONTROL{
    uint8_t D3EN         :1; // bit 0 
    uint8_t D3DEPTH      :4; // bits 3:0
    uint8_t D3LC         :1; // bit 5
    uint8_t D3UC         :1; // bit 6
};

typedef struct R17_ALC1{
    uint8_t ALCL         :4; // bits 3:0
    uint8_t MAXGAIN      :3; // bits 2:0
    uint8_t ALCSEL       :2; // bits 1:0
};

typedef struct R18_ALC2{
    uint8_t HLD          :4; // bits 3:0
    uint8_t MINGAIN      :3; // bits 2:0
};

typedef struct R19_ALC3{
    uint8_t ATK          :4; // bits 3:0
    uint8_t DCY          :4; // bits 3:0
};

typedef struct R20_NOISE_GATE{
    uint8_t NGAT         :1; // bit 0
    uint8_t NGTH         :5; // bit 4:0
};

typedef struct R21_LEFT_ADC_VOLUME{
    uint8_t LADCVOL      :8; // bits 7:0
    uint8_t ADCVU        :1; // bit  8
};

typedef struct R22_RIGHT_ADC_VOLUME{
    uint8_t RADCVOL      :8; // bits 7:0
    uint8_t ADCVU        :1; // bit  8
};

typedef struct R23_ADDITIONAL_CONTROL_1{
    uint8_t TOEN         :1; // bit 1
    uint8_t TOCLKSEL     :1; // bit  2
    uint8_t DATSEL       :2; // bits 1:0
    uint8_t DMONOMIX     :1; // bit 4
    uint8_t VSEL         :2; // bits 1:0
    uint8_t TSDEN        :1; // bit 8
};

typedef struct R24_ADDITIONAL_CONTROL_2{
    uint8_t LRCM         :1; // bit 2
    uint8_t TRIS         :1; // bit 3
    uint8_t HPSWPOL      :1; // bit 5
    uint8_t HPSWEN       :1; // bit 6
};

typedef struct R25_PWR_MGMT_1{
    uint8_t DIGENB       :1; // bit 0
    uint8_t MICB         :1; // bit 1
    uint8_t ADCR         :1; // bit 2
    uint8_t ADCL         :1; // bit 3
    uint8_t AINR         :1; // bit 4
    uint8_t AINL         :1; // bit 5
    uint8_t VREF         :1; // bit 6
    uint8_t VMIDSEL      :2; // bit 1:0
};

typedef struct R26_PWR_MGMT_2{
    uint8_t PLL_EN       :1; // bit 0
    uint8_t OUT3         :1; // bit 1
    uint8_t SPKR         :1; // bit 3
    uint8_t SPKL         :1; // bit 4
    uint8_t ROUT1        :1; // bit 5
    uint8_t LOUT1        :1; // bit 6
    uint8_t DACR         :1; // bit 7
    uint8_t DACL         :1; // bit 8
};

typedef struct R27_ADDITIONAL_CONTROL_3{
    uint8_t ADC_ALC_SR   :3; // bits 2:0
    uint8_t OUT3CAP      :1; // bit 3
    uint8_t VROI         :1; // bit 6
};

typedef struct R28_ANTI_POP_1{
    uint8_t HPSTBY       :1; // bit 0
    uint8_t SOFT_ST      :1; // bit 2
    uint8_t BUFIOEN      :1; // bit 3
    uint8_t BUFDCOPEN    :1; // bit 4
    uint8_t POBCTRL      :1; // bit 7
};

typedef struct R29_ANTI_POP_2{
    uint8_t DRES         :2; // bits 1:0
    uint8_t DISOP        :1; // bit 5
};

typedef struct R32_ADCL_SIGNAL_PATH{
    uint8_t LMIC2B       :1; // bit 3
    uint8_t LMICBOOST    :2; // bits 1:0
    uint8_t LMP2         :1; // bit 6
    uint8_t LMP3         :1; // bit 7
    uint8_t LMN1         :1; // bit 8
};

typedef struct R33_ADCR_SIGNAL_PATH{
    uint8_t RMIC2B       :1; // bit 3
    uint8_t RMICBOOST    :2; // bits 1:0
    uint8_t RMP2         :1; // bit 6
    uint8_t RMP3         :1; // bit 7
    uint8_t RMN1         :1; // bit 8
};

typedef struct R34_LEFT_OUT_MIX_1{
    uint8_t LI2LOVOL     :3; // bits 2:0
    uint8_t LI2LO        :1; // bit 7
    uint8_t LD2LO        :1; // bit 8
};

typedef struct R37_RIGHT_OUT_MIX_2{
    uint8_t RI2ROVOL     :3; // bits 2:0
    uint8_t RI2RO        :1; // bit 7
    uint8_t RD2RO        :1; // bit 8
};

typedef struct R38_MONO_OUT_MIX_1{
    uint8_t L2MO         :1; // bit 7
};

typedef struct R39_MONO_OUT_MIX_2{
    uint8_t R2MO         :1; // bit 7
};

typedef struct R40_LOUT2_VOLUME{
    uint8_t SPKLVOL      :7; // bits 6:0
    uint8_t SPKLZC       :1; // bit 7
    uint8_t SPKVU        :1; // bit 8
};

typedef struct R41_ROUT2_VOLUME{
    uint8_t SPKRVOL      :7; // bits 6:0
    uint8_t SPKLZC       :1; // bit 7
    uint8_t SPKVU        :1; // bit 8
};

typedef struct R42_MONOOUT_VOLUME{
    uint8_t MOUTVOL      :1; //bit 6
};

typedef struct R43_INPUT_BOOST_MIXER_1{
    uint8_t LIN2BOOST    :3; // bits 2:0
    uint8_t LIN3BOOST    :3; // bits 2:0
};

typedef struct R44_INPUT_BOOST_MIXER_2{
    uint8_t RIN2BOOST    :3; // bits 2:0
    uint8_t RIN3BOOST    :3; // bits 2:0
};

typedef struct R45_BYPASS_1{
    uint8_t LB2LOVOL     :3; // bits 2:0
    uint8_t LB2LO        :1; // bit 5
};

typedef struct R46_BYPASS_2{
    uint8_t RB2ROVOL     :3; // bits 2:0
    uint8_t RB2RO        :1; // bit 5
};

typedef struct R47_PWR_MGMT_3{
    uint8_t ROMIX        :1; // bit 2
    uint8_t LOMIX        :1; // bit 3
    uint8_t RMIC         :1; // bit 4
    uint8_t LMIC         :1; // bit 5
};

typedef struct R48_ADDITIONAL_CONTROL_4{
    uint8_t MBSEL        :1; // bit 0
    uint8_t TSENSEN      :1; // bit 1
    uint8_t HPSEL        :2; // bits 1:0
    uint8_t GPIOSEL      :3; // bits 2:0
    uint8_t GPIOPOL      :1; // bit 7
};

typedef struct R49_CLASS_D_CONTROL_1{
    uint8_t SPK_OP_EN    :2; // bits 1:0
};

typedef struct R51_CLASS_D_CONTROL_3{
    uint8_t ACGAIN       :3; // bits 2:0
    uint8_t DCGAIN       :3; // bits 2:0
};

typedef struct R52_PLL_N{
    uint8_t PLLN         :4; // bits 3:0
    uint8_t PLLRESCALE   :1; // bit 4
    uint8_t SDM          :1; // bit 5
    uint8_t OPCLKDIV     :3; // bits 2:0
};

typedef struct R53_PLL_K_1{
    uint8_t PLLK         :8; // bits 23:16 
};

typedef struct R54_PLL_K_2{
    uint8_t PLLK         :8; // bits 15:8 
};

typedef struct R55_PLL_K_3{
    uint8_t PLLK         :8; // bits 7:0 
};
