#ifndef BME688.h
#define BME688.h

#include "esp_err.h"
#include "bme68x.h"

//Pin configuration + Address
#define I2C_SDA_PIN              23 //Where SDA pin is on stratus
#define I2C_SCL_PIN              18 //Where SCL pin is on STRATUS
#define I2C_SCL_SPEED_HZ         100000 //standard, cand go to 400khz if needed, should be fine with size of traces, but will cuase more noise
#define I2C_IGNORE               7 // standard
#define I2C_NUM_PORT             0
#define SLEEP_MS                 1000 //1 second

#define BME688_ADDR              BME68X_I2C_ADDR_LOW // 0x76

//data structure for the sensor data
typedef struct {
    float temperature_c;
    float humidity_percent;
    float pressure_hpa;
    float gas_resistance_ohm;
} bme688_sensor_data_t;

//Bme688 Functions
//initializes i2c master bus
esp_err_t sensors_setup(void);

//config the oversampling
esp_err_t bme688_setup(void);

// reads data from sensor and puts it into the data structure created
esp_err_t bme688_read_data(bme688_sensor_data_t *data);

#endif 