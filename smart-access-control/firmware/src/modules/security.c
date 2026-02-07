#include <Arduino.h>
#include <WiFi.h>

// 安全模块状态
bool securityInitialized = false;

// 安全配置
#define MAX_FAILED_ATTEMPTS 5
#define LOCKOUT_DURATION 60000 // 1分钟

// 安全状态
int failedAttempts = 0;
ulong lockoutStartTime = 0;
bool isLockedOut = false;
bool tamperDetected = false;

/**
 * 安全模块初始化
 */
void security_init() {
  securityInitialized = true;
  Serial.println("安全模块初始化完成");
}

/**
 * 安全检查
 */
void security_check() {
  if (!securityInitialized) {
    return;
  }
  
  // 检查锁定状态
  security_check_lockout();
  
  // 检查防拆状态
  extern bool sensor_check_tamper_status();
  bool currentTamper = sensor_check_tamper_status();
  
  if (currentTamper && !tamperDetected) {
    tamperDetected = true;
    security_handle_tamper();
  } else if (!currentTamper && tamperDetected) {
    tamperDetected = false;
    security_handle_tamper_clear();
  }
  
  // 检查网络安全
  security_check_network();
}

/**
 * 检查锁定状态
 */
void security_check_lockout() {
  if (isLockedOut) {
    if (millis() - lockoutStartTime >= LOCKOUT_DURATION) {
      // 解除锁定
      isLockedOut = false;
      failedAttempts = 0;
      Serial.println("锁定状态已解除");
    }
  }
}

/**
 * 处理识别失败
 */
void security_handle_failed_attempt() {
  if (!securityInitialized) {
    return;
  }
  
  failedAttempts++;
  Serial.printf("识别失败，当前失败次数: %d\n", failedAttempts);
  
  // 检查是否达到最大失败次数
  if (failedAttempts >= MAX_FAILED_ATTEMPTS) {
    security_lockout();
  }
}

/**
 * 处理识别成功
 */
void security_handle_success() {
  if (!securityInitialized) {
    return;
  }
  
  // 重置失败次数
  failedAttempts = 0;
  isLockedOut = false;
  Serial.println("识别成功，安全状态已重置");
}

/**
 * 锁定系统
 */
void security_lockout() {
  isLockedOut = true;
  lockoutStartTime = millis();
  
  Serial.printf("系统已锁定，持续时间: %d秒\n", LOCKOUT_DURATION / 1000);
  
  // 蜂鸣器报警
  extern void lock_buzzer_alarm(unsigned long, unsigned int);
  lock_buzzer_alarm(2000, 1000);
  
  // 发送报警信息
  extern void communication_publish_alarm(const char*, const char*);
  communication_publish_alarm("lockout", "识别失败次数过多，系统已锁定");
}

/**
 * 处理防拆
 */
void security_handle_tamper() {
  if (!securityInitialized) {
    return;
  }
  
  Serial.println("检测到防拆触发");
  
  // 蜂鸣器报警
  extern void lock_buzzer_alarm(unsigned long, unsigned int);
  lock_buzzer_alarm(3000, 800);
  
  // 发送报警信息
  extern void communication_publish_alarm(const char*, const char*);
  communication_publish_alarm("tamper", "设备被拆卸，可能遭受攻击");
}

/**
 * 处理防拆解除
 */
void security_handle_tamper_clear() {
  if (!securityInitialized) {
    return;
  }
  
  Serial.println("防拆状态已解除");
  
  // 发送恢复信息
  extern void communication_publish_event(PubSubClient*, const char*, const char*, const char*);
  extern PubSubClient mqttClient;
  communication_publish_event(&mqttClient, "ESP32-ACCESS-CONTROL-001", "tamper_clear", "防拆状态已恢复正常");
}

/**
 * 检查网络安全
 */
void security_check_network() {
  if (WiFi.status() == WL_CONNECTED) {
    // 检查RSSI
    int rssi = WiFi.RSSI();
    if (rssi < -80) {
      Serial.printf("网络信号弱，RSSI: %d dBm\n", rssi);
    }
    
    // 检查IP地址
    IPAddress ip = WiFi.localIP();
    if (ip == IPAddress(0, 0, 0, 0)) {
      Serial.println("网络IP地址异常");
    }
  }
}

/**
 * 加密数据
 * @param data 数据
 * @param length 长度
 * @param key 密钥
 * @return 加密后的数据
 */
byte* security_encrypt(byte *data, int length, byte *key) {
  // 简单的XOR加密
  static byte encrypted[256];
  
  for (int i = 0; i < length; i++) {
    encrypted[i] = data[i] ^ key[i % 8];
  }
  
  return encrypted;
}

/**
 * 解密数据
 * @param data 数据
 * @param length 长度
 * @param key 密钥
 * @return 解密后的数据
 */
byte* security_decrypt(byte *data, int length, byte *key) {
  // 简单的XOR解密
  static byte decrypted[256];
  
  for (int i = 0; i < length; i++) {
    decrypted[i] = data[i] ^ key[i % 8];
  }
  
  return decrypted;
}

/**
 * 获取安全状态
 * @param status 状态缓冲区
 * @param length 缓冲区长度
 * @return 状态信息长度
 */
int security_get_status(char *status, int length) {
  if (!securityInitialized) {
    snprintf(status, length, "未初始化");
    return strlen(status);
  }
  
  int len = snprintf(status, length, "失败次数: %d, 锁定状态: %s, 防拆状态: %s", 
                     failedAttempts, 
                     isLockedOut ? "锁定" : "正常", 
                     tamperDetected ? "触发" : "正常");
  
  return len;
}

/**
 * 检查是否锁定
 * @return 是否锁定
 */
bool security_is_locked_out() {
  return isLockedOut;
}

/**
 * 检查安全模块状态
 * @return 是否初始化成功
 */
bool security_is_initialized() {
  return securityInitialized;
}

/**
 * 测试安全模块
 */
void security_test() {
  if (!securityInitialized) {
    Serial.println("安全模块未初始化");
    return;
  }
  
  Serial.println("安全模块测试开始...");
  
  // 打印安全状态
  char status[100];
  security_get_status(status, sizeof(status));
  Serial.printf("当前安全状态: %s\n", status);
  
  // 测试失败尝试
  Serial.println("测试失败尝试...");
  for (int i = 0; i < 3; i++) {
    security_handle_failed_attempt();
    delay(500);
  }
  
  // 测试成功
  Serial.println("测试成功尝试...");
  security_handle_success();
  
  // 再次打印安全状态
  security_get_status(status, sizeof(status));
  Serial.printf("测试后安全状态: %s\n", status);
  
  Serial.println("安全模块测试完成");
}
