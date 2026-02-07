#include <Arduino.h>

// 传感器引脚定义
#define DOOR_SENSOR_PIN    34
#define TAMPER_SENSOR_PIN  35

// 传感器状态
bool sensorInitialized = false;
bool doorOpen = false;
bool tamperDetected = false;

// 状态变化时间
unsigned long doorChangeTime = 0;
unsigned long tamperChangeTime = 0;

// 消抖时间
#define DEBOUNCE_TIME  50

/**
 * 传感器初始化
 */
void sensor_init() {
  // 初始化门磁传感器
  pinMode(DOOR_SENSOR_PIN, INPUT_PULLUP);
  
  // 初始化防拆传感器
  pinMode(TAMPER_SENSOR_PIN, INPUT_PULLUP);
  
  // 读取初始状态
  doorOpen = !digitalRead(DOOR_SENSOR_PIN);
  tamperDetected = !digitalRead(TAMPER_SENSOR_PIN);
  
  sensorInitialized = true;
  Serial.println("传感器驱动初始化完成");
  Serial.printf("门状态: %s\n", doorOpen ? "打开" : "关闭");
  Serial.printf("防拆状态: %s\n", tamperDetected ? "触发" : "正常");
}

/**
 * 检查门状态
 * @return 是否打开
 */
bool sensor_check_door_status() {
  if (!sensorInitialized) {
    return doorOpen;
  }
  
  bool currentState = !digitalRead(DOOR_SENSOR_PIN);
  unsigned long currentTime = millis();
  
  // 消抖
  if (currentState != doorOpen) {
    if (currentTime - doorChangeTime > DEBOUNCE_TIME) {
      doorOpen = currentState;
      doorChangeTime = currentTime;
      
      Serial.printf("门状态变化: %s\n", doorOpen ? "打开" : "关闭");
    }
  }
  
  return doorOpen;
}

/**
 * 检查防拆状态
 * @return 是否触发
 */
bool sensor_check_tamper_status() {
  if (!sensorInitialized) {
    return tamperDetected;
  }
  
  bool currentState = !digitalRead(TAMPER_SENSOR_PIN);
  unsigned long currentTime = millis();
  
  // 消抖
  if (currentState != tamperDetected) {
    if (currentTime - tamperChangeTime > DEBOUNCE_TIME) {
      tamperDetected = currentState;
      tamperChangeTime = currentTime;
      
      Serial.printf("防拆状态变化: %s\n", tamperDetected ? "触发" : "正常");
    }
  }
  
  return tamperDetected;
}

/**
 * 获取门状态
 * @return 门状态
 */
bool sensor_get_door_status() {
  return doorOpen;
}

/**
 * 获取防拆状态
 * @return 防拆状态
 */
bool sensor_get_tamper_status() {
  return tamperDetected;
}

/**
 * 检查报警状态
 * @return 是否有报警
 */
bool sensor_check_alarm_status() {
  if (!sensorInitialized) {
    return false;
  }
  
  // 检查门状态
  sensor_check_door_status();
  
  // 检查防拆状态
  sensor_check_tamper_status();
  
  // 返回报警状态
  return tamperDetected;
}

/**
 * 检查传感器状态
 * @return 是否初始化成功
 */
bool sensor_is_initialized() {
  return sensorInitialized;
}

/**
 * 测试传感器
 */
void sensor_test() {
  if (!sensorInitialized) {
    Serial.println("传感器未初始化");
    return;
  }
  
  Serial.println("传感器测试开始...");
  
  // 测试门磁传感器
  Serial.println("测试门磁传感器...");
  for (int i = 0; i < 5; i++) {
    bool doorState = sensor_check_door_status();
    Serial.printf("门状态: %s\n", doorState ? "打开" : "关闭");
    delay(1000);
  }
  
  // 测试防拆传感器
  Serial.println("测试防拆传感器...");
  for (int i = 0; i < 5; i++) {
    bool tamperState = sensor_check_tamper_status();
    Serial.printf("防拆状态: %s\n", tamperState ? "触发" : "正常");
    delay(1000);
  }
  
  Serial.println("传感器测试完成");
}
