#ifndef ACCESS_CONTROL_H
#define ACCESS_CONTROL_H

#include <Arduino.h>

/**
 * 门禁控制初始化
 */
void access_control_init();

/**
 * 开锁
 * @param userId 用户ID
 * @param method 识别方式
 * @return 是否成功
 */
bool access_control_open_door(int userId, const char *method);

/**
 * 拒绝访问
 * @param userId 用户ID
 * @param method 识别方式
 */
void access_control_deny_access(int userId, const char *method);

/**
 * 远程开门
 * @return 是否成功
 */
bool access_control_remote_open();

/**
 * 检查门禁状态
 * @return 门禁状态
 */
bool access_control_check_status();

/**
 * 处理门禁事件
 * @param eventType 事件类型
 * @param eventData 事件数据
 */
void access_control_handle_event(const char *eventType, const char *eventData);

/**
 * 测试门禁控制
 */
void access_control_test();

#endif
