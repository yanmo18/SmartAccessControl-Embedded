#include <Arduino.h>

// 键盘引脚定义
#define ROW1_PIN 25
#define ROW2_PIN 26
#define ROW3_PIN 27
#define ROW4_PIN 32

#define COL1_PIN 33
#define COL2_PIN 2
#define COL3_PIN 0
#define COL4_PIN 4

// 键盘状态
bool keypadInitialized = false;

// 键盘映射
char keymap[4][4] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

// 行引脚
int rowPins[] = {ROW1_PIN, ROW2_PIN, ROW3_PIN, ROW4_PIN};

// 列引脚
int colPins[] = {COL1_PIN, COL2_PIN, COL3_PIN, COL4_PIN};

/**
 * 键盘初始化
 */
void keypad_init() {
  // 初始化行引脚为输出
  for (int i = 0; i < 4; i++) {
    pinMode(rowPins[i], OUTPUT);
    digitalWrite(rowPins[i], HIGH);
  }
  
  // 初始化列引脚为输入，上拉
  for (int i = 0; i < 4; i++) {
    pinMode(colPins[i], INPUT_PULLUP);
  }
  
  keypadInitialized = true;
  Serial.println("键盘驱动初始化完成");
}

/**
 * 扫描键盘
 * @return 按键值，0表示无按键
 */
char keypad_scan() {
  if (!keypadInitialized) {
    return 0;
  }
  
  // 扫描每一行
  for (int row = 0; row < 4; row++) {
    // 设置当前行为低电平
    digitalWrite(rowPins[row], LOW);
    
    // 扫描每一列
    for (int col = 0; col < 4; col++) {
      if (digitalRead(colPins[col]) == LOW) {
        // 按键被按下
        delay(50); // 消抖
        
        // 等待按键释放
        while (digitalRead(colPins[col]) == LOW) {
          delay(10);
        }
        
        // 恢复行状态
        digitalWrite(rowPins[row], HIGH);
        
        return keymap[row][col];
      }
    }
    
    // 恢复行状态
    digitalWrite(rowPins[row], HIGH);
  }
  
  return 0;
}

/**
 * 获取密码输入
 * @param password 密码缓冲区
 * @param maxLength 最大长度
 * @param timeout 超时时间(ms)
 * @return 输入长度
 */
int keypad_get_password(char *password, int maxLength, unsigned long timeout) {
  if (!keypadInitialized) {
    return 0;
  }
  
  int length = 0;
  unsigned long startTime = millis();
  
  while (length < maxLength && (millis() - startTime) < timeout) {
    char key = keypad_scan();
    if (key) {
      if (key == '#') {
        // 结束输入
        password[length] = '\0';
        return length;
      } else if (key == '*') {
        // 删除
        if (length > 0) {
          length--;
          Serial.print("\b \");
        }
      } else {
        // 数字或字母
        password[length] = key;
        length++;
        Serial.print('*');
      }
    }
    delay(10);
  }
  
  password[length] = '\0';
  return length;
}

/**
 * 检查键盘状态
 * @return 是否初始化成功
 */
bool keypad_is_initialized() {
  return keypadInitialized;
}

/**
 * 测试键盘
 */
void keypad_test() {
  if (!keypadInitialized) {
    Serial.println("键盘未初始化");
    return;
  }
  
  Serial.println("键盘测试开始...");
  Serial.println("请按键，按#结束");
  
  char key;
  while ((key = keypad_scan()) != '#') {
    if (key) {
      Serial.printf("按键: %c\n", key);
    }
    delay(10);
  }
  
  Serial.println("键盘测试完成");
}
