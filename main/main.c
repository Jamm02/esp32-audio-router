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
#include "esp_err.h"
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

static const char *TAG = "WM8960";

static uint8_t volume;
#define CODEC_ADDR  0x34
#define SDA_PIN  21
#define SCL_PIN  22
#define I2C_BUS_NO 0
#define ACK_CHECK_EN  0
#define ACK_CHECK_DIS 0
#define ACK_VAL       0
#define NACK_VAL      1
const uint16_t wm8960_reg_defaults[] = {
  0x0, 0x0117,
  0x1, 0x0117,
  0x2, 0x01fd,
  0x3, 0x01fd,
  0x4, 0x0000,
  0x5, 0x0000,
  0x6, 0x0000,
  0x7, 0x000A,
  0x8, 0x01c0,
  0x9, 0x0000,
  0xa, 0x01ff,
  0xb, 0x01ff,

 0x10, 0x0000,
 0x11, 0x007b,
 0x12, 0x0100,
 0x13, 0x0032,
 0x14, 0x0000,
 0x15, 0x00c3,
 0x16, 0x00c3,
 0x17, 0x01c0,
 0x18, 0x0000,
 0x19, 0x0000,
 0x1a, 0x01e0,
 0x1b, 0x0000,
 0x1c, 0x0000,
 0x1d, 0x0000,

 0x20, 0x0100,
 0x21, 0x0100,
 0x22, 0x0150,

 0x25, 0x0150,
 0x26, 0x0000,
 0x27, 0x0000,
 0x28, 0x0000,
 0x29, 0x0000,
 0x2a, 0x0040,
 0x2b, 0x0000,
 0x2c, 0x0000,
 0x2d, 0x0050,
 0x2e, 0x0050,
 0x2f, 0x0000,
 0x30, 0x0002,
 0x31, 0x0037,

 0x33, 0x0080,
 0x34, 0x0008,
 0x35, 0x0031,
 0x36, 0x0026,
 0x37, 0x00e9,

};


static void wm8960_i2c_init()
{
    esp_err_t rc;
    i2c_config_t i2c_conf;
    memset(&i2c_conf, 0x00, sizeof(i2c_conf));
    i2c_conf.mode = I2C_MODE_MASTER;
    i2c_conf.sda_io_num = SDA_PIN;
    i2c_conf.scl_io_num = SCL_PIN;
    i2c_conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
    i2c_conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
    i2c_conf.master.clk_speed = 100 * 1000;
    ESP_LOGI(TAG, "Configuring I2C");
    rc = i2c_param_config(I2C_BUS_NO, &i2c_conf);
    // ESP_LOGD(TAG, "I2C Param Config: %s", esp_err_to_name(rc));
    rc = i2c_driver_install(I2C_BUS_NO, I2C_MODE_MASTER, 0, 0, 0);
    // ESP_LOGD(TAG, "I2C Driver Install; %s", esp_err_to_name(rc));
}

static esp_err_t i2c_trans(i2c_port_t i2c_num, uint8_t addr, void *txdata, uint8_t txlen)
{
    esp_err_t rc;
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    // ESP_LOGV(TAG, "CMD Handle: %s", esp_err_to_name(rc));
    rc = i2c_master_start(cmd);
    // ESP_LOGV(TAG, "[W] Master_start: %s", esp_err_to_name(rc));
    rc = i2c_master_write_byte(cmd, (addr << 1) | I2C_MASTER_WRITE, ACK_CHECK_EN);
    // ESP_LOGV(TAG, "[W] Master_write_byte: %s", esp_err_to_name(rc));
    rc = i2c_master_write(cmd, txdata, txlen, ACK_CHECK_EN);
    // ESP_LOGV(TAG, "[W] Master_write: %s", esp_err_to_name(rc));
    rc = i2c_master_stop(cmd);
    // ESP_LOGV(TAG, "[W] Master stop: %s", esp_err_to_name(rc));
    rc = i2c_master_cmd_begin(I2C_BUS_NO, cmd, 10);
    // ESP_LOGI(TAG, "[W] CMD begin: %s", esp_err_to_name(rc));
    i2c_cmd_link_delete(cmd);
    return rc;
}

static esp_err_t i2c_recv(i2c_port_t i2c_num, uint8_t addr, void *rxdata, uint8_t rxlen)
{
    esp_err_t rc;
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (CODEC_ADDR << 1) | I2C_MASTER_READ, ACK_CHECK_DIS);
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (addr << 1), ACK_CHECK_EN);
    if (rxlen > 1) {
        i2c_master_read(cmd, rxdata, rxlen - 1, ACK_VAL);
    }
    i2c_master_read_byte(cmd, rxdata + rxlen - 1, NACK_VAL);
    i2c_master_stop(cmd);
    rc = i2c_master_cmd_begin(i2c_num, cmd, 10);
    printf("Recv: %d\n", rc);
    i2c_cmd_link_delete(cmd);
    return rc;
}

static esp_err_t write_register_i2c(uint8_t slave_id, uint32_t reg_addr, uint32_t reg_val)
{
    esp_err_t rc;
    uint8_t buff[3];
    buff[0] = (reg_addr << 1) | ((reg_val >> 8) & 0x0f);
    buff[1] = reg_val & 0xff;
    //printf("%x %x \t %x %x\n", reg_addr, reg_val, buff[0], buff[1]);
    rc = i2c_trans(I2C_BUS_NO, slave_id, buff, 2);
    return rc;
}

static esp_err_t read_register_i2c(uint8_t slave_id, uint8_t reg_addr)
{
    esp_err_t rc;
    uint8_t data = 0;
    rc = i2c_recv(I2C_BUS_NO, reg_addr, &data, 1);
    printf("Read Value: %x\n", data);
    return rc;

    esp_err_t res;
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();

    res  = i2c_master_start(cmd);
    res |= i2c_master_write_byte(cmd, (slave_id << 1) | I2C_MASTER_WRITE, 1 /*ACK_CHECK_EN*/);
    res |= i2c_master_write_byte(cmd, (reg_addr << 1), 1 /*ACK_CHECK_EN*/);
    res |= i2c_master_stop(cmd);
    res |= i2c_master_cmd_begin(0, cmd, 1000 / portTICK_RATE_MS);
    printf("%d\n", res);
    i2c_cmd_link_delete(cmd);

}

esp_err_t wm8960_init()
{
    esp_err_t ret = 0;

    wm8960_i2c_init();
    for (int i = 0; i < sizeof(wm8960_reg_defaults)/sizeof(uint16_t); i += 2 ) {
        printf("[%d] %x %x\n", i, wm8960_reg_defaults[i], wm8960_reg_defaults[i+1]);
        ret = write_register_i2c(CODEC_ADDR, wm8960_reg_defaults[i], wm8960_reg_defaults[i+1]);
        if (ret != 0) {
            ESP_LOGE(TAG, "Error: %d", ret);
            return ret;
        }
    }
     if (ret == ESP_OK) {
        ESP_LOGI(TAG, "WM8960 is up");
    } else {
        ESP_LOGE(TAG, "WM8960 setup error");
    }
    return ret;
}
esp_err_t wm8960_set_vol(int vol)
{
    esp_err_t ret = 0;
    int vol_to_set = 0;
    if (vol == 0) {
        vol_to_set = 0;
    } else {
        volume = vol;
        vol_to_set = (vol / 10) * 5 + 200;
    }
    ret |= write_register_i2c(CODEC_ADDR, 0xc, 0x100 | vol_to_set);
    ret |= write_register_i2c(CODEC_ADDR, 0xb, 0x100 | vol_to_set);

    return ret;
}

esp_err_t wm8960_set_mute(bool mute)
{
    esp_err_t ret = 0;
    if (mute) {
        ret |= wm8960_set_vol(0);
    } else {
        ret |= wm8960_set_vol(volume);
    }
    return ret;
}

esp_err_t wm8960_get_volume(uint8_t* vol)
{
    *vol = volume;
    return ESP_OK;
}




void app_main(void)
{

    wm8960_init();
    wm8960_set_vol(50);


   
      /* Initialize NVS — it is used to store PHY calibration data */
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    





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

