#include <Arduino.h>
#include <SoftwareSerial.h>
#include <Adafruit_Fingerprint.h>

// 指纹模块引脚定义
#define FINGERPRINT_TX_PIN  16
#define FINGERPRINT_RX_PIN  17

// 串口实例
SoftwareSerial fingerprintSerial(FINGERPRINT_TX_PIN, FINGERPRINT_RX_PIN);

// 指纹模块实例
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&fingerprintSerial);

// 指纹模块状态
bool fingerprintInitialized = false;

/**
 * 指纹模块初始化
 */
void fingerprint_init() {
  // 初始化串口
  fingerprintSerial.begin(57600);
  
  // 初始化指纹模块
  finger.begin(57600);
  
  // 检查模块连接
  if (finger.verifyPassword()) {
    fingerprintInitialized = true;
    Serial.println("指纹模块初始化完成");
    
    // 获取模块信息
    uint16_t version = finger.getTemplateCount();
    Serial.printf("指纹模板数量: %d\n", version);
  } else {
    Serial.println("指纹模块初始化失败");
  }
}

/**
 * 检查指纹
 * @return 是否检测到指纹
 */
bool fingerprint_check() {
  if (!fingerprintInitialized) {
    return false;
  }
  
  // 检查是否有指纹
  int p = finger.getImage();
  if (p != FINGERPRINT_OK) {
    return false;
  }
  
  return true;
}

/**
 * 识别指纹
 * @return 指纹ID，-1表示未识别
 */
int fingerprint_identify() {
  if (!fingerprintInitialized) {
    return -1;
  }
  
  // 获取指纹图像
  int p = finger.getImage();
  if (p != FINGERPRINT_OK) {
    return -1;
  }
  
  // 转换图像
  p = finger.image2Tz();
  if (p != FINGERPRINT_OK) {
    return -1;
  }
  
  // 搜索指纹
  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK) {
    return -1;
  }
  
  // 返回指纹ID
  return finger.fingerID;
}

/**
 * 添加指纹
 * @param id 指纹ID
 * @return 是否成功
 */
bool fingerprint_add(uint8_t id) {
  if (!fingerprintInitialized) {
    return false;
  }
  
  // 获取指纹图像
  Serial.println("请放置指纹...");
  int p = 0;
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    delay(100);
  }
  
  // 转换图像
  p = finger.image2Tz(1);
  if (p != FINGERPRINT_OK) {
    return false;
  }
  
  Serial.println("请抬起手指...");
  delay(2000);
  
  Serial.println("请再次放置指纹...");
  p = 0;
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    delay(100);
  }
  
  // 转换图像
  p = finger.image2Tz(2);
  if (p != FINGERPRINT_OK) {
    return false;
  }
  
  // 创建模板
  p = finger.createTemplate();
  if (p != FINGERPRINT_OK) {
    return false;
  }
  
  // 存储模板
  p = finger.storeTemplate(id, 1);
  if (p != FINGERPRINT_OK) {
    return false;
  }
  
  return true;
}

/**
 * 删除指纹
 * @param id 指纹ID
 * @return 是否成功
 */
bool fingerprint_delete(uint8_t id) {
  if (!fingerprintInitialized) {
    return false;
  }
  
  int p = finger.deleteTemplate(id);
  return (p == FINGERPRINT_OK);
}

/**
 * 清空所有指纹
 * @return 是否成功
 */
bool fingerprint_clear_all() {
  if (!fingerprintInitialized) {
    return false;
  }
  
  int p = finger.emptyDatabase();
  return (p == FINGERPRINT_OK);
}

/**
 * 获取指纹模板数量
 * @return 模板数量
 */
int fingerprint_get_template_count() {
  if (!fingerprintInitialized) {
    return 0;
  }
  
  return finger.getTemplateCount();
}

/**
 * 检查指纹模块状态
 * @return 是否初始化成功
 */
bool fingerprint_is_initialized() {
  return fingerprintInitialized;
}

/**
 * 测试指纹模块
 */
void fingerprint_test() {
  if (!fingerprintInitialized) {
    Serial.println("指纹模块未初始化");
    return;
  }
  
  Serial.println("指纹模块测试开始...");
  
  // 检查模块连接
  if (finger.verifyPassword()) {
    Serial.println("指纹模块连接正常");
    
    // 获取模板数量
    int count = fingerprint_get_template_count();
    Serial.printf("当前模板数量: %d\n", count);
    
    // 获取模块地址
    uint32_t address = finger.getAddress();
    Serial.printf("模块地址: 0x%08X\n", address);
  } else {
    Serial.println("指纹模块连接失败");
  }
  
  Serial.println("指纹模块测试完成");
}
