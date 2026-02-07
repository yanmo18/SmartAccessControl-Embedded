#ifndef SENSOR_DRIVER_H
#define SENSOR_DRIVER_H

#include <Arduino.h>

/**
 * 传感器初始化
 */
void sensor_init();

/**
 * 检查门状态
 * @return 是否打开
 */
bool sensor_check_door_status();

/**
 * 检查防拆状态
 * @return 是否触发
 */
bool sensor_check_tamper_status();

/**
 * 获取门状态
 * @return 门状态
 */
bool sensor_get_door_status();

/**
 * 获取防拆状态
 * @return 防拆状态
 */
bool sensor_get_tamper_status();

/**
 * 检查报警状态
 * @return 是否有报警
 */
bool sensor_check_alarm_status();

/**
 * 检查传感器状态
 * @return 是否初始化成功
 */
bool sensor_is_initialized();

/**
 * 测试传感器
 */
void sensor_test();

#endif
