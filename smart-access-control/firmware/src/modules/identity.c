#include <Arduino.h>

// 头文件包含
#include "drivers/rfid_driver.h"
#include "drivers/fingerprint_driver.h"
#include "drivers/camera_driver.h"
#include "drivers/keypad_driver.h"
#include "modules/access_control.h"

// 身份识别状态
bool identityInitialized = false;

// 识别方式定义
#define ID_METHOD_CARD      "card"
#define ID_METHOD_FINGER    "finger"
#define ID_METHOD_FACE      "face"
#define ID_METHOD_PASSWORD  "password"

// 用户数据结构
typedef struct {
  int id;
  char name[50];
  char cardId[20];
  int fingerprintId;
  char password[20];
  bool enabled;
} User;

// 模拟用户数据
User users[] = {
  {1, "管理员", "12345678", 1, "123456", true},
  {2, "张三", "87654321", 2, "654321", true},
  {3, "李四", "11223344", 3, "111111", true},
  {4, "王五", "44332211", 4, "222222", true},
  {5, "赵六", "55667788", 5, "333333", true}
};

#define USER_COUNT (sizeof(users) / sizeof(User))

/**
 * 身份识别初始化
 */
void identity_init() {
  identityInitialized = true;
  Serial.println("身份识别模块初始化完成");
  Serial.printf("当前用户数量: %d\n", USER_COUNT);
}

/**
 * 检查识别请求
 */
void identity_check() {
  if (!identityInitialized) {
    return;
  }
  
  // 检查RFID卡
  if (rfid_check_card()) {
    identity_check_card();
  }
  
  // 检查指纹
  if (fingerprint_check()) {
    identity_check_fingerprint();
  }
  
  // 检查密码
  char key = keypad_scan();
  if (key) {
    identity_check_password();
  }
  
  // 检查人脸（暂未实现）
  // identity_check_face();
}

/**
 * 检查卡片识别
 */
void identity_check_card() {
  char cardId[20];
  if (rfid_get_card_id_string(cardId, sizeof(cardId))) {
    Serial.printf("检测到卡片: %s\n", cardId);
    
    // 查找用户
    int userId = identity_find_user_by_card(cardId);
    if (userId > 0) {
      // 验证通过
      access_control_open_door(userId, ID_METHOD_CARD);
    } else {
      // 验证失败
      access_control_deny_access(0, ID_METHOD_CARD);
    }
    
    // 休眠卡
    rfid_halt_card();
  }
}

/**
 * 检查指纹识别
 */
void identity_check_fingerprint() {
  int fingerprintId = fingerprint_identify();
  if (fingerprintId >= 0) {
    Serial.printf("检测到指纹: ID=%d\n", fingerprintId);
    
    // 查找用户
    int userId = identity_find_user_by_fingerprint(fingerprintId);
    if (userId > 0) {
      // 验证通过
      access_control_open_door(userId, ID_METHOD_FINGER);
    } else {
      // 验证失败
      access_control_deny_access(0, ID_METHOD_FINGER);
    }
  }
}

/**
 * 检查密码识别
 */
void identity_check_password() {
  Serial.println("请输入密码，按#结束");
  
  char password[20];
  int length = keypad_get_password(password, sizeof(password), 30000);
  
  if (length > 0) {
    Serial.printf("输入密码: %s\n", password);
    
    // 查找用户
    int userId = identity_find_user_by_password(password);
    if (userId > 0) {
      // 验证通过
      access_control_open_door(userId, ID_METHOD_PASSWORD);
    } else {
      // 验证失败
      access_control_deny_access(0, ID_METHOD_PASSWORD);
    }
  }
}

/**
 * 检查人脸识别（暂未实现）
 */
void identity_check_face() {
  // 暂未实现
  // 后续可集成OpenCV或其他人脸识别库
}

/**
 * 根据卡号查找用户
 * @param cardId 卡号
 * @return 用户ID，0表示未找到
 */
int identity_find_user_by_card(const char *cardId) {
  for (int i = 0; i < USER_COUNT; i++) {
    if (users[i].enabled && strcmp(users[i].cardId, cardId) == 0) {
      return users[i].id;
    }
  }
  return 0;
}

/**
 * 根据指纹ID查找用户
 * @param fingerprintId 指纹ID
 * @return 用户ID，0表示未找到
 */
int identity_find_user_by_fingerprint(int fingerprintId) {
  for (int i = 0; i < USER_COUNT; i++) {
    if (users[i].enabled && users[i].fingerprintId == fingerprintId) {
      return users[i].id;
    }
  }
  return 0;
}

/**
 * 根据密码查找用户
 * @param password 密码
 * @return 用户ID，0表示未找到
 */
int identity_find_user_by_password(const char *password) {
  for (int i = 0; i < USER_COUNT; i++) {
    if (users[i].enabled && strcmp(users[i].password, password) == 0) {
      return users[i].id;
    }
  }
  return 0;
}

/**
 * 获取用户信息
 * @param userId 用户ID
 * @param user 用户信息
 * @return 是否找到
 */
bool identity_get_user(int userId, User *user) {
  for (int i = 0; i < USER_COUNT; i++) {
    if (users[i].id == userId) {
      *user = users[i];
      return true;
    }
  }
  return false;
}

/**
 * 添加用户
 * @param user 用户信息
 * @return 是否成功
 */
bool identity_add_user(User *user) {
  // 暂未实现
  // 后续可实现用户管理功能
  return false;
}

/**
 * 删除用户
 * @param userId 用户ID
 * @return 是否成功
 */
bool identity_delete_user(int userId) {
  // 暂未实现
  // 后续可实现用户管理功能
  return false;
}

/**
 * 启用/禁用用户
 * @param userId 用户ID
 * @param enabled 是否启用
 * @return 是否成功
 */
bool identity_set_user_enabled(int userId, bool enabled) {
  for (int i = 0; i < USER_COUNT; i++) {
    if (users[i].id == userId) {
      users[i].enabled = enabled;
      return true;
    }
  }
  return false;
}

/**
 * 检查身份识别状态
 * @return 是否初始化成功
 */
bool identity_is_initialized() {
  return identityInitialized;
}

/**
 * 测试身份识别
 */
void identity_test() {
  if (!identityInitialized) {
    Serial.println("身份识别模块未初始化");
    return;
  }
  
  Serial.println("身份识别模块测试开始...");
  
  // 打印用户信息
  Serial.println("当前用户列表:");
  for (int i = 0; i < USER_COUNT; i++) {
    Serial.printf("ID: %d, 姓名: %s, 卡号: %s, 指纹ID: %d, 状态: %s\n", 
                 users[i].id, users[i].name, users[i].cardId, 
                 users[i].fingerprintId, users[i].enabled ? "启用" : "禁用");
  }
  
  Serial.println("身份识别模块测试完成");
}
