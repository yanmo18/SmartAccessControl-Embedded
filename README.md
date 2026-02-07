# 嵌入式智能门禁管理控制平台

## 项目简介

嵌入式智能门禁管理控制平台是一个完整的智能门禁系统解决方案，集成了多种识别方式（卡、指纹、人脸、密码），支持远程管理、实时监控、数据分析等功能。

本项目包含硬件设计、嵌入式固件、后端服务和前端界面四个主要部分，形成了一个从硬件到软件的完整系统。

## 项目结构

```
嵌入式智能门禁管理控制平台/
├── .venv/                # Python虚拟环境
├── .vscode/              # VS Code配置文件
├── documents/            # 项目文档
│   └── 智能门禁管理控制平台实现计划.md
├── frontend/             # 前端界面
│   ├── index.html        # 主页面
│   ├── script.js         # JavaScript逻辑
│   └── style.css         # 样式文件
├── smart-access-control/ # 核心功能代码
│   ├── .env.example      # 环境变量示例
│   ├── .gitignore        # Git忽略文件
│   ├── docker-compose.yml # Docker配置
│   ├── README.md         # 核心功能说明
│   ├── hardware/         # 硬件设计
│   ├── firmware/         # 嵌入式固件
│   └── backend/          # 后端服务
```

## 各部分功能说明

### 1. .venv/
- **功能**：Python虚拟环境目录
- **含义**：用于隔离项目依赖，包含Python解释器和安装的第三方包

### 2. .vscode/
- **功能**：Visual Studio Code编辑器配置目录
- **含义**：存储项目的IDE配置文件，优化开发环境
- **内容**：
  - `c_cpp_properties.json`：C/C++语言配置，用于固件开发
  - `launch.json`：调试配置
  - `settings.json`：VS Code全局设置

### 3. documents/
- **功能**：项目文档目录
- **含义**：存储项目相关文档，如实现计划、需求分析等
- **内容**：
  - `智能门禁管理控制平台实现计划.md`：项目实现计划文档

### 4. frontend/
- **功能**：前端界面代码
- **含义**：实现用户界面，提供门禁系统的可视化操作界面
- **内容**：
  - `index.html`：主页面HTML结构
  - `script.js`：前端JavaScript逻辑
  - `style.css`：页面样式

### 5. smart-access-control/
- **功能**：核心功能代码目录，包含整个门禁系统的主要实现
- **含义**：项目的核心部分，包含后端、固件和硬件设计

#### 5.1 hardware/
- **功能**：硬件设计文件
- **含义**：包含门禁系统的硬件设计文档
- **内容**：
  - `schematics/`：原理图
    - `SCHEMATIC.md`：原理图文档
  - `components.csv`：组件清单

#### 5.2 firmware/
- **功能**：嵌入式固件代码
- **含义**：使用C语言实现的嵌入式设备固件，运行在门禁控制器上
- **内容**：
  - `src/`：源代码
    - `drivers/`：硬件驱动
      - `camera_driver.c/h`：摄像头驱动
      - `fingerprint_driver.c/h`：指纹识别驱动
      - `keypad_driver.c/h`：键盘驱动
      - `lock_driver.c/h`：锁控制驱动
      - `rfid_driver.c/h`：RFID卡驱动
      - `sensor_driver.c/h`：传感器驱动
    - `modules/`：功能模块
      - `access_control.c/h`：访问控制逻辑
      - `communication.c/h`：通信模块
      - `identity.c/h`：身份识别模块
      - `security.c/h`：安全模块
      - `storage.c/h`：存储模块
  - `main.c`：固件主入口
  - `platformio.ini`：PlatformIO配置文件

#### 5.3 backend/
- **功能**：后端服务器代码
- **含义**：使用Python实现的服务器端逻辑，处理数据管理、用户认证等
- **内容**：
  - `app/`：应用代码
    - `models/`：数据模型
      - `access_method.py`：访问方式模型
      - `access_record.py`：访问记录模型
      - `device.py`：设备模型
      - `user.py`：用户模型
    - `utils/`：工具函数
      - `database.py`：数据库操作
      - `logger.py`：日志管理
      - `security.py`：安全相关功能
  - `main.py`：后端主入口
  - `requirements.txt`：Python依赖包

## 技术栈

### 硬件技术
- **主控芯片**：ESP32-WROOM-32
- **开发环境**：PlatformIO、Arduino IDE
- **实时操作系统**：FreeRTOS
- **通信协议**：MQTT、HTTP、ModBus RTU

### 后端技术
- **语言**：Python 3.9+
- **Web框架**：FastAPI
- **数据库**：PostgreSQL
- **缓存**：Redis
- **认证**：JWT
- **ORM**：SQLAlchemy
- **数据验证**：Pydantic
- **部署**：Docker、Nginx

### 前端技术
- **语言**：HTML5、CSS3、JavaScript
- **图表库**：Chart.js
- **图标库**：Font Awesome
- **HTTP客户端**：Fetch API
- **WebSocket**：原生WebSocket
- **响应式**：CSS Grid、Flexbox

## 快速开始

### 1. 环境准备

#### 硬件环境
- ESP32开发板
- RFID模块
- 指纹模块
- 摄像头模块
- 键盘模块
- 电子锁
- 传感器模块

#### 软件环境
- Python 3.9+
- PostgreSQL
- Redis (可选)
- Docker (可选)
- PlatformIO (嵌入式开发)

### 2. 安装依赖

#### 后端依赖
```bash
cd smart-access-control/backend
pip install -r requirements.txt
```

### 3. 配置环境变量

复制 `.env.example` 文件为 `.env` 并修改配置：

```bash
cd smart-access-control
cp .env.example .env
```

### 4. 启动服务

#### 后端服务
```bash
cd smart-access-control/backend
python main.py
```

#### 前端界面
直接打开 `frontend/index.html` 文件。

### 5. 硬件固件

使用PlatformIO编译并上传固件：

```bash
cd smart-access-control/firmware
platformio run --target upload
```

## 功能特性

### 1. 多种识别方式
- **卡识别**：支持MIFARE卡、CPU卡
- **指纹识别**：支持指纹采集、比对、管理
- **人脸识别**：支持人脸检测、识别、活体检测
- **密码识别**：支持密码输入、复杂度校验

### 2. 远程管理
- **Web管理界面**：支持PC和移动设备
- **实时监控**：实时查看门禁状态、设备状态
- **远程开门**：支持远程控制门禁开关
- **批量操作**：支持批量添加用户、设备

### 3. 权限管理
- **时间段权限**：支持设置用户的访问时间段
- **设备权限**：支持设置用户可访问的设备
- **角色权限**：支持管理员、普通用户、访客等角色
- **临时权限**：支持设置临时访客权限

### 4. 数据分析
- **访问统计**：统计访问次数、趋势、高峰
- **用户统计**：统计活跃用户、访问频率
- **设备统计**：统计设备使用率、故障次数
- **报警统计**：统计报警类型、处理效率

### 5. 系统安全
- **数据加密**：传输加密、存储加密
- **访问控制**：基于角色的权限控制
- **物理安全**：防拆报警、防破坏设计
- **日志审计**：完整的操作日志、系统日志

## 图片展示

### 系统架构图

![系统架构图](https://trae-api-cn.mchost.guru/api/ide/v1/text_to_image?prompt=smart%20access%20control%20system%20architecture%20diagram%20with%20hardware%2C%20firmware%2C%20backend%20and%20frontend%20components%2C%20technical%20drawing%2C%20blue%20theme&image_size=landscape_16_9)

### 硬件设计图

![硬件设计图](https://trae-api-cn.mchost.guru/api/ide/v1/text_to_image?prompt=smart%20access%20control%20hardware%20design%20with%20ESP32%2C%20fingerprint%20sensor%2C%20RFID%20reader%2C%20camera%20and%20keypad%2C%20circuit%20diagram%2C%20technical%20drawing&image_size=landscape_16_9)

### 门禁设备效果图

![门禁设备效果图](https://trae-api-cn.mchost.guru/api/ide/v1/text_to_image?prompt=smart%20access%20control%20device%20with%20fingerprint%20scanner%2C%20RFID%20reader%2C%20camera%20and%20keypad%2C%20modern%20design%2C%20wall-mounted%20device&image_size=landscape_4_3)

### 管理界面效果图

![管理界面效果图](pictureshow\00.png)

### 数据分析界面

![数据分析界面](https://trae-api-cn.mchost.guru/api/ide/v1/text_to_image?prompt=smart%20access%20control%20data%20analytics%20dashboard%20with%20charts%20and%20statistics%2C%20user%20access%20patterns%2C%20device%20status%2C%20modern%20data%20visualization&image_size=landscape_16_9)

## 项目文档

- **实现计划**：`documents/智能门禁管理控制平台实现计划.md`
- **核心功能说明**：`smart-access-control/README.md`
- **硬件原理图**：`smart-access-control/hardware/schematics/SCHEMATIC.md`

## 联系方式

- **项目维护**：智能门禁开发团队
- **版本**：v1.0.0

## 许可证

本项目采用MIT许可证。
