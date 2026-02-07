#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

// 通信模块状态
bool communicationInitialized = false;

// WiFi配置
const char* ssid = "your-ssid";
const char* password = "your-password";

// MQTT配置
const char* mqttServer = "localhost";
const int mqttPort = 1883;
const char* mqttUser = "admin";
const char* mqttPassword = "password";

// 主题定义
#define MQTT_TOPIC_STATUS         "access-control/status"
#define MQTT_TOPIC_ACCESS_RECORD  "access-control/record"
#define MQTT_TOPIC_ALARM          "access-control/alarm"
#define MQTT_TOPIC_COMMAND        "access-control/command"
#define MQTT_TOPIC_SENSOR_DATA    "access-control/sensor"

/**
 * 通信模块初始化
 * @param client MQTT客户端
 */
void communication_init(PubSubClient *client) {
  // 设置回调函数
  client->setCallback(mqtt_callback);
  
  communicationInitialized = true;
  Serial.println("通信模块初始化完成");
}

/**
 * 连接WiFi
 * @return 是否成功
 */
bool communication_connect_wifi() {
  Serial.printf("正在连接WiFi: %s\n", ssid);
  
  WiFi.begin(ssid, password);
  
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(500);
    Serial.print(".");
    attempts++;
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("
WiFi连接成功");
    Serial.printf("IP地址: %s\n", WiFi.localIP().toString().c_str());
    return true;
  } else {
    Serial.println("
WiFi连接失败");
    return false;
  }
}

/**
 * 连接MQTT
 * @param client MQTT客户端
 * @param deviceId 设备ID
 * @return 是否成功
 */
bool communication_connect_mqtt(PubSubClient *client, const char *deviceId) {
  Serial.printf("正在连接MQTT服务器: %s\n", mqttServer);
  
  client->setServer(mqttServer, mqttPort);
  
  // 连接MQTT
  if (client->connect(deviceId, mqttUser, mqttPassword)) {
    Serial.println("MQTT连接成功");
    
    // 订阅命令主题
    client->subscribe(MQTT_TOPIC_COMMAND);
    Serial.printf("已订阅主题: %s\n", MQTT_TOPIC_COMMAND);
    
    // 发布上线状态
    communication_publish_status(client, deviceId, "online");
    
    return true;
  } else {
    Serial.println("MQTT连接失败");
    return false;
  }
}

/**
 * MQTT回调函数
 * @param topic 主题
 * @param payload 负载
 * @param length 长度
 */
void mqtt_callback(char* topic, byte* payload, unsigned int length) {
  Serial.printf("收到MQTT消息: %s\n", topic);
  
  // 解析消息
  DynamicJsonDocument doc(1024);
  DeserializationError error = deserializeJson(doc, payload, length);
  
  if (error) {
    Serial.printf("JSON解析错误: %s\n", error.c_str());
    return;
  }
  
  // 处理命令
  if (strcmp(topic, MQTT_TOPIC_COMMAND) == 0) {
    const char* command = doc["command"];
    const char* deviceId = doc["device_id"];
    
    Serial.printf("收到命令: %s, 设备ID: %s\n", command, deviceId);
    
    // 处理远程开门命令
    if (strcmp(command, "open_door") == 0) {
      // 调用远程开门函数
      extern bool access_control_remote_open();
      access_control_remote_open();
    }
    
    // 处理状态查询命令
    if (strcmp(command, "get_status") == 0) {
      // 发布设备状态
      PubSubClient client(WiFiClient());
      communication_publish_device_status(&client, deviceId);
    }
  }
}

/**
 * 发布状态
 * @param client MQTT客户端
 * @param deviceId 设备ID
 * @param status 状态
 */
void communication_publish_status(PubSubClient *client, const char *deviceId, const char *status) {
  if (!client->connected()) {
    return;
  }
  
  DynamicJsonDocument doc(256);
  doc["device_id"] = deviceId;
  doc["status"] = status;
  doc["timestamp"] = millis();
  
  char payload[256];
  serializeJson(doc, payload);
  
  client->publish(MQTT_TOPIC_STATUS, payload);
  Serial.printf("发布状态: %s\n", status);
}

/**
 * 发布设备状态
 * @param client MQTT客户端
 * @param deviceId 设备ID
 */
void communication_publish_device_status(PubSubClient *client, const char *deviceId) {
  if (!client->connected()) {
    return;
  }
  
  extern int lock_get_state();
  extern bool sensor_get_door_status();
  extern bool sensor_get_tamper_status();
  
  DynamicJsonDocument doc(256);
  doc["device_id"] = deviceId;
  doc["lock_state"] = lock_get_state() == 1 ? "unlocked" : "locked";
  doc["door_state"] = sensor_get_door_status() ? "open" : "closed";
  doc["tamper_state"] = sensor_get_tamper_status() ? "triggered" : "normal";
  doc["wifi_rssi"] = WiFi.RSSI();
  doc["timestamp"] = millis();
  
  char payload[256];
  serializeJson(doc, payload);
  
  client->publish(MQTT_TOPIC_STATUS, payload);
  Serial.println("发布设备状态");
}

/**
 * 发布门禁记录
 * @param userId 用户ID
 * @param method 识别方式
 * @param result 结果
 */
void communication_publish_access_record(int userId, const char *method, const char *result) {
  // 获取MQTT客户端
  extern PubSubClient mqttClient;
  if (!mqttClient.connected()) {
    return;
  }
  
  DynamicJsonDocument doc(256);
  doc["user_id"] = userId;
  doc["method"] = method;
  doc["result"] = result;
  doc["timestamp"] = millis();
  
  char payload[256];
  serializeJson(doc, payload);
  
  mqttClient.publish(MQTT_TOPIC_ACCESS_RECORD, payload);
  Serial.printf("发布门禁记录: 用户=%d, 方式=%s, 结果=%s\n", userId, method, result);
}

/**
 * 发布报警信息
 * @param type 报警类型
 * @param message 报警信息
 */
void communication_publish_alarm(const char *type, const char *message) {
  // 获取MQTT客户端
  extern PubSubClient mqttClient;
  if (!mqttClient.connected()) {
    return;
  }
  
  DynamicJsonDocument doc(256);
  doc["type"] = type;
  doc["message"] = message;
  doc["timestamp"] = millis();
  
  char payload[256];
  serializeJson(doc, payload);
  
  mqttClient.publish(MQTT_TOPIC_ALARM, payload);
  Serial.printf("发布报警信息: 类型=%s, 信息=%s\n", type, message);
}

/**
 * 发布传感器数据
 * @param client MQTT客户端
 * @param deviceId 设备ID
 */
void communication_publish_sensor_data(PubSubClient *client, const char *deviceId) {
  if (!client->connected()) {
    return;
  }
  
  extern bool sensor_get_door_status();
  extern bool sensor_get_tamper_status();
  
  DynamicJsonDocument doc(256);
  doc["device_id"] = deviceId;
  doc["door_open"] = sensor_get_door_status();
  doc["tamper_triggered"] = sensor_get_tamper_status();
  doc["wifi_rssi"] = WiFi.RSSI();
  doc["timestamp"] = millis();
  
  char payload[256];
  serializeJson(doc, payload);
  
  client->publish(MQTT_TOPIC_SENSOR_DATA, payload);
}

/**
 * 发布事件
 * @param client MQTT客户端
 * @param deviceId 设备ID
 * @param eventType 事件类型
 * @param message 事件消息
 */
void communication_publish_event(PubSubClient *client, const char *deviceId, const char *eventType, const char *message) {
  if (!client->connected()) {
    return;
  }
  
  DynamicJsonDocument doc(256);
  doc["device_id"] = deviceId;
  doc["event_type"] = eventType;
  doc["message"] = message;
  doc["timestamp"] = millis();
  
  char payload[256];
  serializeJson(doc, payload);
  
  client->publish("access-control/event", payload);
  Serial.printf("发布事件: 类型=%s, 消息=%s\n", eventType, message);
}

/**
 * 检查通信状态
 * @return 是否初始化成功
 */
bool communication_is_initialized() {
  return communicationInitialized;
}

/**
 * 测试通信模块
 * @param client MQTT客户端
 */
void communication_test(PubSubClient *client) {
  if (!communicationInitialized) {
    Serial.println("通信模块未初始化");
    return;
  }
  
  Serial.println("通信模块测试开始...");
  
  // 测试WiFi连接
  Serial.println("测试WiFi连接...");
  Serial.printf("WiFi状态: %s\n", WiFi.status() == WL_CONNECTED ? "已连接" : "未连接");
  if (WiFi.status() == WL_CONNECTED) {
    Serial.printf("IP地址: %s\n", WiFi.localIP().toString().c_str());
    Serial.printf("RSSI: %d dBm\n", WiFi.RSSI());
  }
  
  // 测试MQTT连接
  Serial.println("测试MQTT连接...");
  Serial.printf("MQTT状态: %s\n", client->connected() ? "已连接" : "未连接");
  
  // 测试发布消息
  if (client->connected()) {
    Serial.println("测试发布消息...");
    communication_publish_event(client, "test-device", "test", "通信模块测试消息");
  }
  
  Serial.println("通信模块测试完成");
}
