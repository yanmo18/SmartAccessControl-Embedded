#include <Arduino.h>
#include <SPI.h>
#include <SD.h>

// 存储模块状态
bool storageInitialized = false;

// SD卡引脚
#define SD_CS_PIN  5

// 文件路径
#define LOG_FILE   "/logs.txt"
#define CONFIG_FILE "/config.json"
#define USERS_FILE  "/users.json"

/**
 * 存储模块初始化
 */
void storage_init() {
  // 初始化SD卡
  if (!SD.begin(SD_CS_PIN)) {
    Serial.println("SD卡初始化失败");
    return;
  }
  
  // 检查文件系统
  uint8_t cardType = SD.cardType();
  if (cardType == CARD_NONE) {
    Serial.println("未检测到SD卡");
    return;
  }
  
  // 打印SD卡信息
  Serial.printf("SD卡类型: %s\n", getCardTypeString(cardType));
  
  uint64_t cardSize = SD.cardSize() / (1024 * 1024);
  Serial.printf("SD卡容量: %llu MB\n", cardSize);
  
  uint64_t freeSpace = SD.totalBytes() / (1024 * 1024);
  Serial.printf("可用空间: %llu MB\n", freeSpace);
  
  // 创建必要的目录
  if (!SD.exists("/logs")) {
    SD.mkdir("/logs");
  }
  
  storageInitialized = true;
  Serial.println("存储模块初始化完成");
}

/**
 * 获取SD卡类型字符串
 * @param cardType 卡类型
 * @return 类型字符串
 */
const char* getCardTypeString(uint8_t cardType) {
  switch (cardType) {
    case CARD_MMC:
      return "MMC";
    case CARD_SD:
      return "SDSC";
    case CARD_SDHC:
      return "SDHC";
    default:
      return "Unknown";
  }
}

/**
 * 写入日志
 * @param message 日志消息
 * @return 是否成功
 */
bool storage_write_log(const char *message) {
  if (!storageInitialized) {
    return false;
  }
  
  // 打开日志文件
  File file = SD.open(LOG_FILE, FILE_APPEND);
  if (!file) {
    return false;
  }
  
  // 写入时间戳
  char timestamp[20];
  getTimestamp(timestamp, sizeof(timestamp));
  file.print(timestamp);
  file.print(": ");
  
  // 写入消息
  file.println(message);
  
  // 关闭文件
  file.close();
  
  return true;
}

/**
 * 读取配置
 * @param key 配置键
 * @param value 配置值
 * @param maxLength 最大长度
 * @return 是否成功
 */
bool storage_read_config(const char *key, char *value, int maxLength) {
  if (!storageInitialized) {
    return false;
  }
  
  // 打开配置文件
  File file = SD.open(CONFIG_FILE, FILE_READ);
  if (!file) {
    return false;
  }
  
  // 读取文件内容
  size_t size = file.size();
  char *buffer = (char *)malloc(size + 1);
  if (!buffer) {
    file.close();
    return false;
  }
  
  file.readBytes(buffer, size);
  buffer[size] = '\0';
  
  // 解析JSON
  // 简化实现，实际应使用JSON库
  
  file.close();
  free(buffer);
  
  return true;
}

/**
 * 写入配置
 * @param key 配置键
 * @param value 配置值
 * @return 是否成功
 */
bool storage_write_config(const char *key, const char *value) {
  if (!storageInitialized) {
    return false;
  }
  
  // 打开配置文件
  File file = SD.open(CONFIG_FILE, FILE_WRITE);
  if (!file) {
    return false;
  }
  
  // 写入配置
  // 简化实现，实际应使用JSON库
  
  file.close();
  
  return true;
}

/**
 * 读取用户数据
 * @param userId 用户ID
 * @param userData 用户数据
 * @param maxLength 最大长度
 * @return 是否成功
 */
bool storage_read_user(int userId, char *userData, int maxLength) {
  if (!storageInitialized) {
    return false;
  }
  
  // 打开用户文件
  File file = SD.open(USERS_FILE, FILE_READ);
  if (!file) {
    return false;
  }
  
  // 读取文件内容
  size_t size = file.size();
  char *buffer = (char *)malloc(size + 1);
  if (!buffer) {
    file.close();
    return false;
  }
  
  file.readBytes(buffer, size);
  buffer[size] = '\0';
  
  // 解析JSON
  // 简化实现，实际应使用JSON库
  
  file.close();
  free(buffer);
  
  return true;
}

/**
 * 写入用户数据
 * @param userId 用户ID
 * @param userData 用户数据
 * @return 是否成功
 */
bool storage_write_user(int userId, const char *userData) {
  if (!storageInitialized) {
    return false;
  }
  
  // 打开用户文件
  File file = SD.open(USERS_FILE, FILE_WRITE);
  if (!file) {
    return false;
  }
  
  // 写入用户数据
  // 简化实现，实际应使用JSON库
  
  file.close();
  
  return true;
}

/**
 * 同步数据
 */
void storage_sync() {
  if (!storageInitialized) {
    return;
  }
  
  // 同步文件系统
  // SD卡会自动同步
  
  Serial.println("数据同步完成");
}

/**
 * 获取时间戳
 * @param buffer 缓冲区
 * @param size 缓冲区大小
 */
void getTimestamp(char *buffer, int size) {
  // 简化实现，实际应使用RTC
  sprintf(buffer, "%lu", millis());
}

/**
 * 获取存储状态
 * @param status 状态缓冲区
 * @param maxLength 最大长度
 * @return 状态信息长度
 */
int storage_get_status(char *status, int maxLength) {
  if (!storageInitialized) {
    snprintf(status, maxLength, "未初始化");
    return strlen(status);
  }
  
  uint64_t cardSize = SD.cardSize() / (1024 * 1024);
  uint64_t freeSpace = SD.totalBytes() / (1024 * 1024);
  
  int len = snprintf(status, maxLength, "SD卡容量: %llu MB, 可用空间: %llu MB", 
                     cardSize, freeSpace);
  
  return len;
}

/**
 * 检查存储状态
 * @return 是否初始化成功
 */
bool storage_is_initialized() {
  return storageInitialized;
}

/**
 * 测试存储模块
 */
void storage_test() {
  if (!storageInitialized) {
    Serial.println("存储模块未初始化");
    return;
  }
  
  Serial.println("存储模块测试开始...");
  
  // 获取存储状态
  char status[100];
  storage_get_status(status, sizeof(status));
  Serial.printf("存储状态: %s\n", status);
  
  // 测试写入日志
  Serial.println("测试写入日志...");
  bool success = storage_write_log("存储模块测试日志");
  Serial.printf("写入日志: %s\n", success ? "成功" : "失败");
  
  // 测试读取文件
  Serial.println("测试读取日志文件...");
  File file = SD.open(LOG_FILE, FILE_READ);
  if (file) {
    Serial.println("日志文件内容:");
    while (file.available()) {
      Serial.write(file.read());
    }
    file.close();
  } else {
    Serial.println("无法打开日志文件");
  }
  
  Serial.println("存储模块测试完成");
}
