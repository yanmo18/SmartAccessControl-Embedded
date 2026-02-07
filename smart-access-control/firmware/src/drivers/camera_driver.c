#include <Arduino.h>
#include "esp_camera.h"

// 摄像头引脚定义
#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27

#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5

#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

// 摄像头状态
bool cameraInitialized = false;

/**
 * 摄像头初始化
 */
void camera_init() {
  // 摄像头配置
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;
  config.frame_size = FRAMESIZE_QVGA;
  config.jpeg_quality = 12;
  config.fb_count = 2;

  // 初始化摄像头
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("摄像头初始化失败: %d\n", err);
    return;
  }

  cameraInitialized = true;
  Serial.println("摄像头初始化完成");
}

/**
 * 拍摄照片
 * @param fb 帧缓冲区
 * @return 是否成功
 */
bool camera_capture(camera_fb_t **fb) {
  if (!cameraInitialized) {
    return false;
  }

  // 拍摄照片
  *fb = esp_camera_fb_get();
  if (!*fb) {
    return false;
  }

  return true;
}

/**
 * 释放帧缓冲区
 * @param fb 帧缓冲区
 */
void camera_release_fb(camera_fb_t *fb) {
  if (fb) {
    esp_camera_fb_return(fb);
  }
}

/**
 * 设置摄像头参数
 * @param frameSize 帧大小
 * @param quality 质量
 */
void camera_set_params(framesize_t frameSize, int quality) {
  if (!cameraInitialized) {
    return;
  }

  sensor_t *s = esp_camera_sensor_get();
  if (s) {
    s->set_framesize(s, frameSize);
    s->set_quality(s, quality);
  }
}

/**
 * 获取摄像头状态
 * @return 是否初始化成功
 */
bool camera_is_initialized() {
  return cameraInitialized;
}

/**
 * 测试摄像头
 */
void camera_test() {
  if (!cameraInitialized) {
    Serial.println("摄像头未初始化");
    return;
  }

  Serial.println("摄像头测试开始...");

  // 拍摄测试照片
  camera_fb_t *fb = NULL;
  if (camera_capture(&fb)) {
    Serial.printf("照片尺寸: %dx%d\n", fb->width, fb->height);
    Serial.printf("数据大小: %d bytes\n", fb->len);
    Serial.printf("格式: %s\n", fb->format == PIXFORMAT_JPEG ? "JPEG" : "其他");
    camera_release_fb(fb);
    Serial.println("摄像头测试成功");
  } else {
    Serial.println("摄像头测试失败");
  }

  Serial.println("摄像头测试完成");
}
