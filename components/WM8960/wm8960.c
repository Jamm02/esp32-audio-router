#include "wm8960.h"


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


R0_LEFT_INPUT_VOLUME_t dummy = {.LINVOL = 0,
                                 .LINMUTE= 10};
