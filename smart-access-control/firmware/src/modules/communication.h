#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <Arduino.h>
#include <PubSubClient.h>

/**
 * 通信模块初始化
 * @param client MQTT客户端
 */
void communication_init(PubSubClient *client);

/**
 * 连接WiFi
 * @return 是否成功
 */
bool communication_connect_wifi();

/**
 * 连接MQTT
 * @param client MQTT客户端
 * @param deviceId 设备ID
 * @return 是否成功
 */
bool communication_connect_mqtt(PubSubClient *client, const char *deviceId);

/**
 * 发布状态
 * @param client MQTT客户端
 * @param deviceId 设备ID
 * @param status 状态
 */
void communication_publish_status(PubSubClient *client, const char *deviceId, const char *status);

/**
 * 发布设备状态
 * @param client MQTT客户端
 * @param deviceId 设备ID
 */
void communication_publish_device_status(PubSubClient *client, const char *deviceId);

/**
 * 发布门禁记录
 * @param userId 用户ID
 * @param method 识别方式
 * @param result 结果
 */
void communication_publish_access_record(int userId, const char *method, const char *result);

/**
 * 发布报警信息
 * @param type 报警类型
 * @param message 报警信息
 */
void communication_publish_alarm(const char *type, const char *message);

/**
 * 发布传感器数据
 * @param client MQTT客户端
 * @param deviceId 设备ID
 */
void communication_publish_sensor_data(PubSubClient *client, const char *deviceId);

/**
 * 发布事件
 * @param client MQTT客户端
 * @param deviceId 设备ID
 * @param eventType 事件类型
 * @param message 事件消息
 */
void communication_publish_event(PubSubClient *client, const char *deviceId, const char *eventType, const char *message);

/**
 * 检查通信状态
 * @return 是否初始化成功
 */
bool communication_is_initialized();

/**
 * 测试通信模块
 * @param client MQTT客户端
 */
void communication_test(PubSubClient *client);

#endif
