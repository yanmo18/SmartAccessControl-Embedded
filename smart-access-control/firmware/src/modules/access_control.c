#include <Arduino.h>

// 头文件包含
#include "drivers/lock_driver.h"
#include "drivers/sensor_driver.h"
#include "modules/identity.h"
#include "modules/communication.h"

// 门禁控制状态
bool accessControlInitialized = false;

// 开锁时间
#define UNLOCK_DURATION 3000

/**
 * 门禁控制初始化
 */
void access_control_init() {
  accessControlInitialized = true;
  Serial.println("门禁控制模块初始化完成");
}

/**
 * 开锁
 * @param userId 用户ID
 * @param method 识别方式
 * @return 是否成功
 */
bool access_control_open_door(int userId, const char *method) {
  if (!accessControlInitialized) {
    return false;
  }
  
  // 开锁
  bool success = lock_unlock(UNLOCK_DURATION);
  
  if (success) {
    // 记录门禁事件
    Serial.printf("用户 %d 通过 %s 方式开门\n", userId, method);
    
    // 发送门禁记录
    communication_publish_access_record(userId, method, "success");
  }
  
  return success;
}

/**
 * 拒绝访问
 * @param userId 用户ID
 * @param method 识别方式
 */
void access_control_deny_access(int userId, const char *method) {
  if (!accessControlInitialized) {
    return;
  }
  
  // 记录拒绝事件
  Serial.printf("用户 %d 通过 %s 方式访问被拒绝\n", userId, method);
  
  // 蜂鸣器报警
  lock_buzzer_alarm(500, 2000);
  
  // 发送门禁记录
  communication_publish_access_record(userId, method, "failed");
}

/**
 * 远程开门
 * @return 是否成功
 */
bool access_control_remote_open() {
  if (!accessControlInitialized) {
    return false;
  }
  
  // 开锁
  bool success = lock_unlock(UNLOCK_DURATION);
  
  if (success) {
    // 记录远程开门事件
    Serial.println("远程开门指令执行成功");
    
    // 发送门禁记录
    communication_publish_access_record(0, "remote", "success");
  }
  
  return success;
}

/**
 * 检查门禁状态
 * @return 门禁状态
 */
bool access_control_check_status() {
  if (!accessControlInitialized) {
    return false;
  }
  
  // 检查锁状态
  int lockState = lock_get_state();
  
  // 检查门状态
  bool doorOpen = sensor_check_door_status();
  
  // 检查防拆状态
  bool tamperDetected = sensor_check_tamper_status();
  
  // 打印状态
  Serial.printf("门禁状态: 锁=%s, 门=%s, 防拆=%s\n", 
               lockState == 1 ? "开锁" : "关锁",
               doorOpen ? "打开" : "关闭",
               tamperDetected ? "触发" : "正常");
  
  return true;
}

/**
 * 处理门禁事件
 * @param eventType 事件类型
 * @param eventData 事件数据
 */
void access_control_handle_event(const char *eventType, const char *eventData) {
  if (!accessControlInitialized) {
    return;
  }
  
  Serial.printf("处理门禁事件: %s, 数据: %s\n", eventType, eventData);
  
  // 处理远程开门指令
  if (strcmp(eventType, "remote_open") == 0) {
    access_control_remote_open();
  }
  
  // 处理状态查询指令
  if (strcmp(eventType, "status_query") == 0) {
    access_control_check_status();
  }
}

/**
 * 测试门禁控制
 */
void access_control_test() {
  if (!accessControlInitialized) {
    Serial.println("门禁控制模块未初始化");
    return;
  }
  
  Serial.println("门禁控制模块测试开始...");
  
  // 测试开锁
  Serial.println("测试开锁...");
  access_control_open_door(1, "test");
  
  // 测试状态检查
  Serial.println("测试状态检查...");
  access_control_check_status();
  
  Serial.println("门禁控制模块测试完成");
}
