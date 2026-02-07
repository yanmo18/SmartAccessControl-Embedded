#ifndef RFID_DRIVER_H
#define RFID_DRIVER_H

#include <Arduino.h>

/**
 * RFID初始化
 */
void rfid_init();

/**
 * 检查RFID卡
 * @return 是否检测到卡
 */
bool rfid_check_card();

/**
 * 获取卡号
 * @param cardId 卡号缓冲区
 * @param len 缓冲区长度
 * @return 卡号长度
 */
int rfid_get_card_id(byte *cardId, int len);

/**
 * 获取卡号字符串
 * @param buffer 缓冲区
 * @param bufferSize 缓冲区大小
 * @return 卡号字符串
 */
char* rfid_get_card_id_string(char *buffer, size_t bufferSize);

/**
 * 获取卡类型
 * @return 卡类型
 */
const char* rfid_get_card_type();

/**
 * 休眠卡
 */
void rfid_halt_card();

/**
 * 检查RFID状态
 * @return 是否初始化成功
 */
bool rfid_is_initialized();

/**
 * 获取RFID版本
 * @return 版本号
 */
byte rfid_get_version();

/**
 * 测试RFID
 */
void rfid_test();

#endif
