#ifndef CAMERA_DRIVER_H
#define CAMERA_DRIVER_H

#include <Arduino.h>
#include "esp_camera.h"

/**
 * 摄像头初始化
 */
void camera_init();

/**
 * 拍摄照片
 * @param fb 帧缓冲区
 * @return 是否成功
 */
bool camera_capture(camera_fb_t **fb);

/**
 * 释放帧缓冲区
 * @param fb 帧缓冲区
 */
void camera_release_fb(camera_fb_t *fb);

/**
 * 设置摄像头参数
 * @param frameSize 帧大小
 * @param quality 质量
 */
void camera_set_params(framesize_t frameSize, int quality);

/**
 * 获取摄像头状态
 * @return 是否初始化成功
 */
bool camera_is_initialized();

/**
 * 测试摄像头
 */
void camera_test();

#endif
