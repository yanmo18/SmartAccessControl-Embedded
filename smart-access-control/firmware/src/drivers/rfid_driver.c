#include <Arduino.h>
#include <SPI.h>
#include <MFRC522.h>

// RFID引脚定义
#define RFID_SS_PIN    5
#define RFID_RST_PIN   4

// MFRC522实例
MFRC522 mfrc522(RFID_SS_PIN, RFID_RST_PIN);

// RFID状态
bool rfidInitialized = false;

/**
 * RFID初始化
 */
void rfid_init() {
  // 初始化SPI
  SPI.begin();
  
  // 初始化MFRC522
  mfrc522.PCD_Init();
  
  // 天线增益设置
  mfrc522.PCD_SetAntennaGain(MFRC522::PCD_RxGain::RxGain_38dB);
  
  rfidInitialized = true;
  Serial.println("RFID驱动初始化完成");
}

/**
 * 检查RFID卡
 * @return 是否检测到卡
 */
bool rfid_check_card() {
  if (!rfidInitialized) {
    return false;
  }
  
  // 检查是否有新卡
  if (!mfrc522.PICC_IsNewCardPresent()) {
    return false;
  }
  
  // 选择卡
  if (!mfrc522.PICC_ReadCardSerial()) {
    return false;
  }
  
  return true;
}

/**
 * 获取卡号
 * @param cardId 卡号缓冲区
 * @param len 缓冲区长度
 * @return 卡号长度
 */
int rfid_get_card_id(byte *cardId, int len) {
  if (!rfidInitialized || !mfrc522.uid.uidByte) {
    return 0;
  }
  
  // 复制卡号
  int uidLen = mfrc522.uid.size;
  if (uidLen > len) {
    uidLen = len;
  }
  
  for (int i = 0; i < uidLen; i++) {
    cardId[i] = mfrc522.uid.uidByte[i];
  }
  
  return uidLen;
}

/**
 * 获取卡号字符串
 * @param buffer 缓冲区
 * @param bufferSize 缓冲区大小
 * @return 卡号字符串
 */
char* rfid_get_card_id_string(char *buffer, size_t bufferSize) {
  if (!rfidInitialized || !mfrc522.uid.uidByte) {
    return NULL;
  }
  
  // 格式化卡号
  int uidLen = mfrc522.uid.size;
  int pos = 0;
  
  for (int i = 0; i < uidLen; i++) {
    if (pos + 2 >= bufferSize) {
      break;
    }
    
    sprintf(buffer + pos, "%02X", mfrc522.uid.uidByte[i]);
    pos += 2;
  }
  
  buffer[pos] = '\0';
  return buffer;
}

/**
 * 获取卡类型
 * @return 卡类型
 */
const char* rfid_get_card_type() {
  if (!rfidInitialized || !mfrc522.uid.uidByte) {
    return "Unknown";
  }
  
  // 获取卡类型
  MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
  
  switch (piccType) {
    case MFRC522::PICC_TYPE_MIFARE_MINI:
      return "MIFARE Mini";
    case MFRC522::PICC_TYPE_MIFARE_1K:
      return "MIFARE 1K";
    case MFRC522::PICC_TYPE_MIFARE_4K:
      return "MIFARE 4K";
    case MFRC522::PICC_TYPE_MIFARE_UL:
      return "MIFARE Ultralight";
    case MFRC522::PICC_TYPE_MIFARE_PLUS:
      return "MIFARE Plus";
    case MFRC522::PICC_TYPE_MIFARE_DESFIRE:
      return "MIFARE DESFire";
    case MFRC522::PICC_TYPE_TNP3XXX:
      return "TNP3XXX";
    default:
      return "Unknown";
  }
}

/**
 * 休眠卡
 */
void rfid_halt_card() {
  if (rfidInitialized && mfrc522.uid.uidByte) {
    mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCrypto1();
  }
}

/**
 * 检查RFID状态
 * @return 是否初始化成功
 */
bool rfid_is_initialized() {
  return rfidInitialized;
}

/**
 * 获取RFID版本
 * @return 版本号
 */
byte rfid_get_version() {
  if (!rfidInitialized) {
    return 0;
  }
  
  return mfrc522.PCD_ReadRegister(MFRC522::VersionReg);
}

/**
 * 测试RFID
 */
void rfid_test() {
  if (!rfidInitialized) {
    Serial.println("RFID未初始化");
    return;
  }
  
  Serial.println("RFID测试开始...");
  
  // 打印版本
  byte version = rfid_get_version();
  Serial.printf("RFID版本: 0x%02X\n", version);
  
  // 测试天线
  mfrc522.PCD_Test天线(0);
  
  Serial.println("RFID测试完成");
}
