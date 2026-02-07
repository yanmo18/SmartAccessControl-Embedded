#ifndef STORAGE_H
#define STORAGE_H

#include <Arduino.h>

/**
 * 存储模块初始化
 */
void storage_init();

/**
 * 写入日志
 * @param message 日志消息
 * @return 是否成功
 */
bool storage_write_log(const char *message);

/**
 * 读取配置
 * @param key 配置键
 * @param value 配置值
 * @param maxLength 最大长度
 * @return 是否成功
 */
bool storage_read_config(const char *key, char *value, int maxLength);

/**
 * 写入配置
 * @param key 配置键
 * @param value 配置值
 * @return 是否成功
 */
bool storage_write_config(const char *key, const char *value);

/**
 * 读取用户数据
 * @param userId 用户ID
 * @param userData 用户数据
 * @param maxLength 最大长度
 * @return 是否成功
 */
bool storage_read_user(int userId, char *userData, int maxLength);

/**
 * 写入用户数据
 * @param userId 用户ID
 * @param userData 用户数据
 * @return 是否成功
 */
bool storage_write_user(int userId, const char *userData);

/**
 * 同步数据
 */
void storage_sync();

/**
 * 获取存储状态
 * @param status 状态缓冲区
 * @param maxLength 最大长度
 * @return 状态信息长度
 */
int storage_get_status(char *status, int maxLength);

/**
 * 检查存储状态
 * @return 是否初始化成功
 */
bool storage_is_initialized();

/**
 * 测试存储模块
 */
void storage_test();

#endif
