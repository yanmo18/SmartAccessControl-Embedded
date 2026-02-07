#ifndef LOCK_DRIVER_H
#define LOCK_DRIVER_H

#include <Arduino.h>

/**
 * 锁驱动初始化
 */
void lock_init();

/**
 * 开锁
 * @param duration 开锁时间(ms)
 * @return 是否成功
 */
bool lock_unlock(unsigned long duration);

/**
 * 关锁
 * @return 是否成功
 */
bool lock_lock();

/**
 * 获取锁状态
 * @return 锁状态
 */
int lock_get_state();

/**
 * 蜂鸣器报警
 * @param duration 报警时间(ms)
 * @param frequency 频率(Hz)
 */
void lock_buzzer_alarm(unsigned long duration, unsigned int frequency);

/**
 * 检查锁驱动状态
 * @return 是否初始化成功
 */
bool lock_is_initialized();

/**
 * 测试锁驱动
 */
void lock_test();

#endif
