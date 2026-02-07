#ifndef KEYPAD_DRIVER_H
#define KEYPAD_DRIVER_H

#include <Arduino.h>

/**
 * 键盘初始化
 */
void keypad_init();

/**
 * 扫描键盘
 * @return 按键值，0表示无按键
 */
char keypad_scan();

/**
 * 获取密码输入
 * @param password 密码缓冲区
 * @param maxLength 最大长度
 * @param timeout 超时时间(ms)
 * @return 输入长度
 */
int keypad_get_password(char *password, int maxLength, unsigned long timeout);

/**
 * 检查键盘状态
 * @return 是否初始化成功
 */
bool keypad_is_initialized();

/**
 * 测试键盘
 */
void keypad_test();

#endif
