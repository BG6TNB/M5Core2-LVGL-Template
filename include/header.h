#include <Arduino.h>

// PMIC Used in M5Stack Core 2
#include <AXP192.h>

// Screen drivers and touch screen drivers
#include <TFT_eSPI.h>
#include <FT6336U.h>

/*Using LVGL with Arduino requires some extra steps:
 *Be sure to read the docs here: https://docs.lvgl.io/master/get-started/platforms/arduino.html  */
#include <lvgl.h>

/*To use the built-in examples and demos of LVGL uncomment the includes below respectively.
 *You also need to copy `lvgl/examples` to `lvgl/src/examples`. Similarly for the demos `lvgl/demos` to `lvgl/src/demos`.
 *Note that the `lv_examples` library is for LVGL v7 and you shouldn't install it for this version (since LVGL v8)
 *as the examples and demos are now part of the main LVGL library. */
#include <examples/lv_examples.h>
#include <demos/lv_demos.h>

// FT6336U Pin Definition
#define I2C_SDA 21
#define I2C_SCL 22
#define INT_N_PIN 39
#define RST_N_PIN -1
