#include "header.h"

/*Change to your screen resolution*/
static const uint16_t screenWidth = 320;
static const uint16_t screenHeight = 240;

static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[screenWidth * screenHeight / 10];

AXP192 Axp;

TFT_eSPI tft = TFT_eSPI(screenWidth, screenHeight); /* TFT instance */

FT6336U ft6336u(I2C_SDA, I2C_SCL, RST_N_PIN, INT_N_PIN);
FT6336U_TouchPointType tp;

#if LV_USE_LOG != 0
/* Serial debugging */
void my_print(lv_log_level_t level, const char *buf)
{
  LV_UNUSED(level);
  Serial.flush();
}
#endif

/* Display flushing */
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
  uint32_t w = (area->x2 - area->x1 + 1);
  uint32_t h = (area->y2 - area->y1 + 1);

  tft.startWrite();
  tft.setAddrWindow(area->x1, area->y1, w, h);
  tft.pushColors((uint16_t *)&color_p->full, w * h, true);
  tft.endWrite();

  lv_disp_flush_ready(disp);
}

/* Read the touchpad */
void my_touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data)
{
  tp = ft6336u.scan();

  bool touched = tp.touch_count;

  if (!touched)
  {
    data->state = LV_INDEV_STATE_REL;
  }
  else
  {
    data->state = LV_INDEV_STATE_PR;

    data->point.x = tp.tp[0].x;
    data->point.y = tp.tp[0].y;

    Serial.print("Data x ");
    Serial.println(tp.tp[0].x);

    Serial.print("Data y ");
    Serial.println(tp.tp[0].y);
  }
}

void setup()
{
  Serial.begin(115200); /* prepare for possible serial debug */

  /* Enable the PMIC to power the LCD screen. */
  Axp.begin();
  /*
   * The LCD screen backlight is controlled by the AXP192 and can be adjusted from 2.5-3.3v.
   * It is safe to set the backlight voltage at 2.8v and below.
   * Setting the voltage to 3.3v activates the maximum brightness of the LCD screen,
   * but this will cause the screen to heat up and the screen life will be reduced.
   */
  Axp.SetLcdVoltage(2700);

  tft.begin();        /* TFT init */
  tft.setRotation(1); /* Landscape orientation, flipped */

  ft6336u.begin();

  String LVGL_Arduino = "Hello Arduino! ";
  LVGL_Arduino += String('V') + lv_version_major() + "." + lv_version_minor() + "." + lv_version_patch();

  Serial.println(LVGL_Arduino);
  Serial.println("I am LVGL_Arduino");

  lv_init();

#if LV_USE_LOG != 0
  lv_log_register_print_cb(my_print); /* register print function for debugging */
#endif

  lv_disp_draw_buf_init(&draw_buf, buf, NULL, screenWidth * screenHeight / 10);

  /*Initialize the display*/
  static lv_disp_drv_t disp_drv;
  lv_disp_drv_init(&disp_drv);
  /*Change the following line to your display resolution*/
  disp_drv.hor_res = screenWidth;
  disp_drv.ver_res = screenHeight;
  disp_drv.flush_cb = my_disp_flush;
  disp_drv.draw_buf = &draw_buf;
  lv_disp_drv_register(&disp_drv);

  /*Initialize the (dummy) input device driver*/
  static lv_indev_drv_t indev_drv;
  lv_indev_drv_init(&indev_drv);
  indev_drv.type = LV_INDEV_TYPE_POINTER;
  indev_drv.read_cb = my_touchpad_read;
  lv_indev_drv_register(&indev_drv);

  /* Create simple label */
  // lv_obj_t *label = lv_label_create( lv_scr_act() );
  // lv_label_set_text( label, LVGL_Arduino.c_str() );
  // lv_obj_align( label, LV_ALIGN_CENTER, 0, 0 );

  /* Try an example. See all the examples
   * online: https://docs.lvgl.io/master/examples.html
   * source codes: https://github.com/lvgl/lvgl/tree/e7f88efa5853128bf871dde335c0ca8da9eb7731/examples */
  // lv_example_btn_1();

  /*Or try out a demo. Don't forget to enable the demos in lv_conf.h. E.g. LV_USE_DEMOS_WIDGETS*/
  lv_demo_widgets();
  // lv_demo_benchmark();
  // lv_demo_keypad_encoder();
  // lv_demo_music();
  // lv_demo_printer();
  // lv_demo_stress();

  Serial.println("Setup done");
}

void loop()
{
  lv_timer_handler(); /* let the GUI do its work */
  delay(5);
}
