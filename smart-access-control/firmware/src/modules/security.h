#ifndef SECURITY_H
#define SECURITY_H

#include <Arduino.h>

/**
 * 安全模块初始化
 */
void security_init();

/**
 * 安全检查
 */
void security_check();

/**
 * 处理识别失败
 */
void security_handle_failed_attempt();

/**
 * 处理识别成功
 */
void security_handle_success();

/**
 * 锁定系统
 */
void security_lockout();

/**
 * 处理防拆
 */
void security_handle_tamper();

/**
 * 处理防拆解除
 */
void security_handle_tamper_clear();

/**
 * 加密数据
 * @param data 数据
 * @param length 长度
 * @param key 密钥
 * @return 加密后的数据
 */
byte* security_encrypt(byte *data, int length, byte *key);

/**
 * 解密数据
 * @param data 数据
 * @param length 长度
 * @param key 密钥
 * @return 解密后的数据
 */
byte* security_decrypt(byte *data, int length, byte *key);

/**
 * 获取安全状态
 * @param status 状态缓冲区
 * @param length 缓冲区长度
 * @return 状态信息长度
 */
int security_get_status(char *status, int length);

/**
 * 检查是否锁定
 * @return 是否锁定
 */
bool security_is_locked_out();

/**
 * 检查安全模块状态
 * @return 是否初始化成功
 */
bool security_is_initialized();

/**
 * 测试安全模块
 */
void security_test();

#endif
