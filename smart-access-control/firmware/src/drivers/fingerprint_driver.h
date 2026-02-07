#ifndef FINGERPRINT_DRIVER_H
#define FINGERPRINT_DRIVER_H

#include <Arduino.h>

/**
 * 指纹模块初始化
 */
void fingerprint_init();

/**
 * 检查指纹
 * @return 是否检测到指纹
 */
bool fingerprint_check();

/**
 * 识别指纹
 * @return 指纹ID，-1表示未识别
 */
int fingerprint_identify();

/**
 * 添加指纹
 * @param id 指纹ID
 * @return 是否成功
 */
bool fingerprint_add(uint8_t id);

/**
 * 删除指纹
 * @param id 指纹ID
 * @return 是否成功
 */
bool fingerprint_delete(uint8_t id);

/**
 * 清空所有指纹
 * @return 是否成功
 */
bool fingerprint_clear_all();

/**
 * 获取指纹模板数量
 * @return 模板数量
 */
int fingerprint_get_template_count();

/**
 * 检查指纹模块状态
 * @return 是否初始化成功
 */
bool fingerprint_is_initialized();

/**
 * 测试指纹模块
 */
void fingerprint_test();

#endif
