#include BME688.h
#include <stdio.h>
#include "driver/i2c_master.h"

//handles for the code
static i2c_master_bus_handle_t i2c_bus;
static i2c_master_dev_handle_t bme_i2c_dev;

static struct bme68x_dev bme_dev;
static struct bme68x_conf bme_conf;
static struct bme68x_heatr_conf bme_heatr;


//lower level functions for bosch
static BME68X_INTF_RET_TYPE bme68x_i2c_read(uint8_t reg_addr, uint8_t *reg data, uint32_t len, void *intf_ptr);
static BME68X_INTF_RET_TYPE bme68x_i2c_write(uint8_t reg_addr, const uint8_t *reg_data, uint32_t len, void *intf_ptr);
static void bme68x_delay_us(uint32_t period, void *intf_ptr);



//I2C Master bus initialization
esp_err_t sensor_setup(void){
    esp_err_t ESP

//i2c config
 i2c_master_bus_config_t bus_config = {
        .i2c_port = I2C_NUM_PORT,
        .sda_io_num = I2C_SDA_PIN,
        .scl_io_num = I2C_SCL_PIN,
        .clk_source = I2C_CLK_SPEED_HZ,
        .glitch_ignore_cnt = I2C_IGNORE,
        .flags.enable_internal_pullup = true,
    };

i2c_device_config_t bme_config = {
    .dev_addr_length = I2C_ADDR_BIT_LEN_7,
    .device_address = BME688_ADDR.
    .scl_speed_hz = I2C_SCL_SPEED_HZ,
};

//i2c initialization
ESP = i2c_new_master_bus(&bus_config,&i2c_bus)
if (ESP != ESP_OK){
    printf("Error: Fail I2C bus initialization")
    return(err);
};

//i2c add the bme
ESP = i2c_master_bus_add_device(i2c_bus, &bme_config, &bme_dev);
if (ESP != ESP_OK){
    printf("Error: Failed to initialize BME688 as a device")
    return(err);
};

printf("I2C Master Bus Initialiazation was Successlful");
return(ESP_OK);

}

//BME688  Settings Config
esp_err_t bme_set(void){

    in8_t result;


    //use the bosch structure with pointers
    bme_dev.read = bme68x_i2c_read;
    bme_dev.write = bme68x_i2c_write;
    bme_dev.delay = bme68X_delay_us;
    bme_dev.intf = BME68X_I2C_INTF;
    bme_dev.intf_ptr = bme_i2c_dev;
    bme_dev.amb_temp _ 25;

    //Check that it is connected to the correct chip id
    result = bme68x_init(&bme_dev)
    if (result != BME68X_OK){
        printf("Error: did not connect to BME688");
        return ESP_FAIL;
    }

    //configure over sampling and filtering
    bme_conf.os_hum = BME68X_OS_2X;
    bme_conf.os_pres = BME68X_OS_4X;
    bme_conf.os_temp = BME68X_OS_8X;
    bme_conf.filter = BME68X_FILTER_SIZE_3;

    result = bme68x_set_conf(&bme_conf,&bme_dev);
    if (result != BME68X_OK){
        printf("Configuration has failed");
        return ESP_FAIL;
    }

    // Configure gas sensor heater profile (300°C for 100 ms)
    bme_heatr_conf.enable = BME68X_ENABLE;
    bme_heatr_conf.heatr_temp = 300;
    bme_heatr_conf.heatr_dur = 100;

    rslt = bme68x_set_heatr_conf(BME68X_FORCED_MODE, &bme_heatr_conf, &bme_dev);
    if (rslt != BME68X_OK) {
        ESP_LOGE(TAG, "Failed to set BME688 heater configuration: code %d", rslt);
        return ESP_FAIL;
    }

    ESP_LOGI(TAG, "BME688 configured successfully.");
    return ESP_OK;
}

esperr_t bme688_read_data(bme688_sensor_data_t *data){
    
}

uint8_t current_mode = 0;

// Reads the mode from the sensor's internal register
int8_t rslt = bme68x_get_op_mode(&current_mode, &bme_dev);

if (rslt == BME68X_OK) {
    switch (current_mode) {
        case BME68X_SLEEP_MODE:
            printf("Sensor Mode: SLEEP MODE\n");
            break;
        case BME68X_FORCED_MODE:
            printf("Sensor Mode: FORCED MODE (Measuring...)\n");
            break;
        case BME68X_PARALLEL_MODE:
            printf("Sensor Mode: PARALLEL MODE (Scanning...)\n");
            break;
        default:
            printf("Unknown Mode: 0x%02X\n", current_mode);
            break;
    }
}