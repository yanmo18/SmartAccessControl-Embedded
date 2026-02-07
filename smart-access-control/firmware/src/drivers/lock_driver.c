#include <Arduino.h>

// 锁引脚定义
#define LOCK_PIN    13
#define BUZZER_PIN  12

// 状态定义
#define LOCK_STATE_LOCKED    0
#define LOCK_STATE_UNLOCKED  1

// 锁状态
int lockState = LOCK_STATE_LOCKED;
bool lockInitialized = false;

/**
 * 锁驱动初始化
 */
void lock_init() {
  // 初始化引脚
  pinMode(LOCK_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  
  // 初始状态
  digitalWrite(LOCK_PIN, LOW); // 锁定状态
  digitalWrite(BUZZER_PIN, LOW); // 蜂鸣器关闭
  
  lockInitialized = true;
  Serial.println("锁驱动初始化完成");
}

/**
 * 开锁
 * @param duration 开锁时间(ms)
 * @return 是否成功
 */
bool lock_unlock(unsigned long duration) {
  if (!lockInitialized) {
    return false;
  }
  
  // 开锁
  digitalWrite(LOCK_PIN, HIGH);
  lockState = LOCK_STATE_UNLOCKED;
  
  // 蜂鸣器提示
  digitalWrite(BUZZER_PIN, HIGH);
  delay(200);
  digitalWrite(BUZZER_PIN, LOW);
  
  Serial.println("门已开锁");
  
  // 延时后关锁
  if (duration > 0) {
    delay(duration);
    lock_lock();
  }
  
  return true;
}

/**
 * 关锁
 * @return 是否成功
 */
bool lock_lock() {
  if (!lockInitialized) {
    return false;
  }
  
  // 关锁
  digitalWrite(LOCK_PIN, LOW);
  lockState = LOCK_STATE_LOCKED;
  
  // 蜂鸣器提示
  digitalWrite(BUZZER_PIN, HIGH);
  delay(100);
  digitalWrite(BUZZER_PIN, LOW);
  
  Serial.println("门已关锁");
  
  return true;
}

/**
 * 获取锁状态
 * @return 锁状态
 */
int lock_get_state() {
  return lockState;
}

/**
 * 蜂鸣器报警
 * @param duration 报警时间(ms)
 * @param frequency 频率(Hz)
 */
void lock_buzzer_alarm(unsigned long duration, unsigned int frequency) {
  if (!lockInitialized) {
    return;
  }
  
  unsigned long startTime = millis();
  unsigned int halfPeriod = 1000000 / (frequency * 2);
  
  while (millis() - startTime < duration) {
    digitalWrite(BUZZER_PIN, HIGH);
    delayMicroseconds(halfPeriod);
    digitalWrite(BUZZER_PIN, LOW);
    delayMicroseconds(halfPeriod);
  }
  
  digitalWrite(BUZZER_PIN, LOW);
}

/**
 * 检查锁驱动状态
 * @return 是否初始化成功
 */
bool lock_is_initialized() {
  return lockInitialized;
}

/**
 * 测试锁驱动
 */
void lock_test() {
  if (!lockInitialized) {
    Serial.println("锁驱动未初始化");
    return;
  }
  
  Serial.println("锁驱动测试开始...");
  
  // 测试开锁
  Serial.println("测试开锁...");
  lock_unlock(2000);
  
  delay(1000);
  
  // 测试蜂鸣器
  Serial.println("测试蜂鸣器...");
  lock_buzzer_alarm(1000, 1000);
  
  Serial.println("锁驱动测试完成");
}
