#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

// 头文件包含
#include "drivers/rfid_driver.h"
#include "drivers/fingerprint_driver.h"
#include "drivers/camera_driver.h"
#include "drivers/keypad_driver.h"
#include "drivers/lock_driver.h"
#include "drivers/sensor_driver.h"

#include "modules/access_control.h"
#include "modules/identity.h"
#include "modules/communication.h"
#include "modules/security.h"
#include "modules/storage.h"

// 全局变量
WiFiClient espClient;
PubSubClient mqttClient(espClient);

// 设备信息
char deviceId[] = "ESP32-ACCESS-CONTROL-001";
char deviceName[] = "智能门禁控制器";

// 系统状态
bool systemReady = false;
bool isOnline = false;

// 任务句柄
TaskHandle_t accessControlTaskHandle;
TaskHandle_t communicationTaskHandle;
TaskHandle_t securityTaskHandle;

// 初始化函数
void setup() {
  // 初始化串口
  Serial.begin(115200);
  Serial.println("\n智能门禁控制器初始化...");

  // 初始化存储
  storage_init();
  Serial.println("✓ 存储初始化完成");

  // 初始化传感器
  sensor_init();
  Serial.println("✓ 传感器初始化完成");

  // 初始化驱动
  rfid_init();
  fingerprint_init();
  camera_init();
  keypad_init();
  lock_init();
  Serial.println("✓ 驱动初始化完成");

  // 初始化模块
  access_control_init();
  identity_init();
  communication_init(&mqttClient);
  security_init();
  Serial.println("✓ 模块初始化完成");

  // 连接网络
  communication_connect_wifi();
  Serial.println("✓ 网络初始化完成");

  // 连接MQTT
  communication_connect_mqtt(&mqttClient, deviceId);
  Serial.println("✓ MQTT初始化完成");

  // 创建任务
  xTaskCreatePinnedToCore(
    access_control_task,
    "AccessControlTask",
    4096,
    NULL,
    5,
    &accessControlTaskHandle,
    0
  );

  xTaskCreatePinnedToCore(
    communication_task,
    "CommunicationTask",
    4096,
    &mqttClient,
    4,
    &communicationTaskHandle,
    1
  );

  xTaskCreatePinnedToCore(
    security_task,
    "SecurityTask",
    4096,
    NULL,
    3,
    &securityTaskHandle,
    1
  );

  systemReady = true;
  Serial.println("\n✅ 智能门禁控制器初始化完成！");
  Serial.printf("设备ID: %s\n", deviceId);
  Serial.printf("设备名称: %s\n", deviceName);
}

// 主循环
void loop() {
  // 系统状态监控
  if (systemReady) {
    // 检查网络连接
    if (WiFi.status() == WL_CONNECTED && !isOnline) {
      isOnline = true;
      Serial.println("📶 网络已连接");
      communication_publish_status(&mqttClient, deviceId, "online");
    } else if (WiFi.status() != WL_CONNECTED && isOnline) {
      isOnline = false;
      Serial.println("📶 网络已断开");
      communication_publish_status(&mqttClient, deviceId, "offline");
    }

    // MQTT重连
    if (!mqttClient.connected()) {
      communication_connect_mqtt(&mqttClient, deviceId);
    }

    // MQTT循环
    mqttClient.loop();
  }

  // 延迟
  delay(1000);
}

// 访问控制任务
void access_control_task(void *pvParameters) {
  while (1) {
    if (systemReady) {
      // 检查识别请求
      identity_check();

      // 检查门状态
      sensor_check_door_status();

      // 检查报警状态
      sensor_check_alarm_status();
    }

    // 任务延迟
    vTaskDelay(pdMS_TO_TICKS(100));
  }
}

// 通信任务
void communication_task(void *pvParameters) {
  PubSubClient *client = (PubSubClient *)pvParameters;

  while (1) {
    if (systemReady && client->connected()) {
      // 发布设备状态
      communication_publish_device_status(client, deviceId);

      // 发布传感器数据
      communication_publish_sensor_data(client, deviceId);
    }

    // 任务延迟
    vTaskDelay(pdMS_TO_TICKS(5000));
  }
}

// 安全任务
void security_task(void *pvParameters) {
  while (1) {
    if (systemReady) {
      // 安全检查
      security_check();

      // 数据同步
      storage_sync();
    }

    // 任务延迟
    vTaskDelay(pdMS_TO_TICKS(10000));
  }
}

// MQTT回调函数
void mqtt_callback(char *topic, byte *payload, unsigned int length) {
  Serial.printf("收到消息: %s\n", topic);

  // 解析消息
  DynamicJsonDocument doc(1024);
  DeserializationError error = deserializeJson(doc, payload, length);

  if (error) {
    Serial.printf("JSON解析错误: %s\n", error.c_str());
    return;
  }

  // 处理远程开门命令
  if (strcmp(topic, "access-control/commands/open") == 0) {
    const char *targetDevice = doc["device_id"];
    if (strcmp(targetDevice, deviceId) == 0) {
      Serial.println("🔓 收到远程开门命令");
      access_control_open_door();
      communication_publish_event(&mqttClient, deviceId, "remote_open", "远程开门成功");
    }
  }

  // 处理配置更新命令
  if (strcmp(topic, "access-control/commands/config") == 0) {
    const char *targetDevice = doc["device_id"];
    if (strcmp(targetDevice, deviceId) == 0) {
      Serial.println("⚙️ 收到配置更新命令");
      // 处理配置更新
    }
  }
}
