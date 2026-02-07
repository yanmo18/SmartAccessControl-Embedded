#ifndef IDENTITY_H
#define IDENTITY_H

#include <Arduino.h>

/**
 * 身份识别初始化
 */
void identity_init();

/**
 * 检查识别请求
 */
void identity_check();

/**
 * 检查卡片识别
 */
void identity_check_card();

/**
 * 检查指纹识别
 */
void identity_check_fingerprint();

/**
 * 检查密码识别
 */
void identity_check_password();

/**
 * 检查人脸识别
 */
void identity_check_face();

/**
 * 根据卡号查找用户
 * @param cardId 卡号
 * @return 用户ID，0表示未找到
 */
int identity_find_user_by_card(const char *cardId);

/**
 * 根据指纹ID查找用户
 * @param fingerprintId 指纹ID
 * @return 用户ID，0表示未找到
 */
int identity_find_user_by_fingerprint(int fingerprintId);

/**
 * 根据密码查找用户
 * @param password 密码
 * @return 用户ID，0表示未找到
 */
int identity_find_user_by_password(const char *password);

/**
 * 启用/禁用用户
 * @param userId 用户ID
 * @param enabled 是否启用
 * @return 是否成功
 */
bool identity_set_user_enabled(int userId, bool enabled);

/**
 * 检查身份识别状态
 * @return 是否初始化成功
 */
bool identity_is_initialized();

/**
 * 测试身份识别
 */
void identity_test();

#endif
