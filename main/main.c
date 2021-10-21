#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "nvs.h"
#include "nvs_flash.h"
#include "esp_system.h"
#include "esp_log.h"

#include "esp_bt.h"
#include "bt_app_core.h"
#include "bt_app_av.h"
#include "esp_bt_main.h"
#include "esp_bt_device.h"
#include "esp_gap_bt_api.h"
#include "esp_a2dp_api.h"
#include "esp_avrc_api.h"
#include "driver/i2s.h"

#include "tuning_http_server.h"

#include "driver/i2c.h"

#define I2C_MASTER_SCL_IO           22                         /*!< GPIO number used for I2C master clock */
#define I2C_MASTER_SDA_IO           21                         /*!< GPIO number used for I2C master data  */
#define I2C_MASTER_NUM              0                          /*!< I2C master i2c port number, the number of i2c peripheral interfaces available will depend on the chip */
#define I2C_MASTER_FREQ_HZ          400000                     /*!< I2C master clock frequency */
#define I2C_MASTER_TX_BUF_DISABLE   0                          /*!< I2C master doesn't need buffer */
#define I2C_MASTER_RX_BUF_DISABLE   0                          /*!< I2C master doesn't need buffer */
#define I2C_MASTER_TIMEOUT_MS       1000

#define WM8960_AUDIO_BOARD_ADDR                 0x34       /*!< Slave address of WM8960 Audio Board */
#define R0   0x01
#define R1   0x03
#define R2   0x05
#define R3   0x07
#define R5   0x0A
#define R26  0x35
#define R34  0x45
#define R37  0x4B
#define R0_DATA  0x17
#define R1_DATA  0x17
#define R2_DATA  0xFD
#define R3_DATA  0xFD
#define R5_DATA  0x00
#define R26_DATA 0xE0
#define R34_DATA 0x50
#define R37_DATA 0x50
static const char *TAG = "i2c-WM8960-CONFIG";


static esp_err_t WM8960_AUDIO_BOARD_register_write_byte(uint8_t reg_addr, uint8_t data)
{
    int ret;
    uint8_t write_buf[2] = {reg_addr, data};

    ret = i2c_master_write_to_device(I2C_MASTER_NUM, WM8960_AUDIO_BOARD_ADDR  , write_buf, sizeof(write_buf), I2C_MASTER_TIMEOUT_MS / portTICK_RATE_MS);

    return ret;
}

static esp_err_t i2c_master_init(void)
{
    int i2c_master_port = I2C_MASTER_NUM;

    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = I2C_MASTER_SDA_IO,
        .scl_io_num = I2C_MASTER_SCL_IO,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = I2C_MASTER_FREQ_HZ,
    };

    i2c_param_config(i2c_master_port, &conf);

    return i2c_driver_install(i2c_master_port, conf.mode, I2C_MASTER_RX_BUF_DISABLE, I2C_MASTER_TX_BUF_DISABLE, 0);
}



void app_main(void)
{
    ESP_ERROR_CHECK(i2c_master_init());
    ESP_LOGI(TAG, "I2C initialized successfully");

      /* Initialize NVS — it is used to store PHY calibration data */
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

     /* Demonstrate writing by reseting the WM8960_AUDIO_BOARD */
    ESP_ERROR_CHECK(WM8960_AUDIO_BOARD_register_write_byte(R0 , R0_DATA));
    
    ESP_ERROR_CHECK(WM8960_AUDIO_BOARD_register_write_byte(R1 , R1_DATA));
  
    ESP_ERROR_CHECK(WM8960_AUDIO_BOARD_register_write_byte(R2 , R2_DATA));

    ESP_ERROR_CHECK(WM8960_AUDIO_BOARD_register_write_byte(R3 , R3_DATA));

    ESP_ERROR_CHECK(WM8960_AUDIO_BOARD_register_write_byte(R5 , R5_DATA));
 
    ESP_ERROR_CHECK(WM8960_AUDIO_BOARD_register_write_byte(R26 , R26_DATA));
 
    ESP_ERROR_CHECK(WM8960_AUDIO_BOARD_register_write_byte(R34 , R34_DATA));
    //vTaskDelay(50 / portTICK_PERIOD_MS);
    ESP_ERROR_CHECK(WM8960_AUDIO_BOARD_register_write_byte(R37 , R37_DATA));


    i2s_config_t i2s_config = {
#ifdef CONFIG_EXAMPLE_A2DP_SINK_OUTPUT_INTERNAL_DAC
        .mode = I2S_MODE_MASTER | I2S_MODE_TX | I2S_MODE_DAC_BUILT_IN,
#else
        .mode = I2S_MODE_MASTER | I2S_MODE_TX,                                  // Only TX
#endif
        .sample_rate = 44100,
        .bits_per_sample = 16,
        .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,                           //2-channels
        .communication_format = I2S_COMM_FORMAT_STAND_MSB,
        .dma_buf_count = 6,
        .dma_buf_len = 60,
        .intr_alloc_flags = 0,                                                  //Default interrupt priority
        .tx_desc_auto_clear = true                                              //Auto clear tx descriptor on underflow
    };


    i2s_driver_install(0, &i2s_config, 0, NULL);
#ifdef CONFIG_EXAMPLE_A2DP_SINK_OUTPUT_INTERNAL_DAC
    i2s_set_dac_mode(I2S_DAC_CHANNEL_BOTH_EN);
    i2s_set_pin(0, NULL);
#else
    i2s_pin_config_t pin_config = {
        .bck_io_num = CONFIG_EXAMPLE_I2S_BCK_PIN,
        .ws_io_num = CONFIG_EXAMPLE_I2S_LRCK_PIN,
        .data_out_num = CONFIG_EXAMPLE_I2S_DATA_PIN,
        .data_in_num = -1                                                       //Not used
    };

    i2s_set_pin(0, &pin_config);
#endif


    ESP_ERROR_CHECK(esp_bt_controller_mem_release(ESP_BT_MODE_BLE));

    esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
    if ((err = esp_bt_controller_init(&bt_cfg)) != ESP_OK) {
        ESP_LOGE(BT_AV_TAG, "%s initialize controller failed: %s\n", __func__, esp_err_to_name(err));
        return;
    }

    if ((err = esp_bt_controller_enable(ESP_BT_MODE_CLASSIC_BT)) != ESP_OK) {
        ESP_LOGE(BT_AV_TAG, "%s enable controller failed: %s\n", __func__, esp_err_to_name(err));
        return;
    }

    if ((err = esp_bluedroid_init()) != ESP_OK) {
        ESP_LOGE(BT_AV_TAG, "%s initialize bluedroid failed: %s\n", __func__, esp_err_to_name(err));
        return;
    }

    if ((err = esp_bluedroid_enable()) != ESP_OK) {
        ESP_LOGE(BT_AV_TAG, "%s enable bluedroid failed: %s\n", __func__, esp_err_to_name(err));
        return;
    }
    start_tuning_http_server();
}