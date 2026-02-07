# 智能门禁控制平台

## 项目简介

智能门禁控制平台是一个集成多种识别方式（卡、指纹、人脸、密码）的智能门禁管理系统，支持远程管理、实时监控、数据分析等功能。

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

## 项目结构

```
smart-access-control/
├── .env.example        # 环境变量示例
├── .gitignore          # Git忽略文件
├── docker-compose.yml  # Docker配置
├── README.md           # 项目说明
├── hardware/           # 硬件设计
│   ├── schematics/     # 电路原理图
│   ├── pcb/            # PCB设计
│   └── components.csv  # 组件清单
├── firmware/           # 嵌入式固件
│   ├── src/            # 源代码
│   │   ├── drivers/    # 设备驱动
│   │   ├── modules/    # 功能模块
│   │   ├── protocols/  # 通信协议
│   │   └── main.c      # 主程序
│   ├── include/        # 头文件
│   ├── lib/            # 第三方库
│   ├── platformio.ini  # PlatformIO配置
│   └── build.sh        # 构建脚本
├── backend/            # 后端服务
│   ├── app/            # 应用代码
│   │   ├── api/        # API接口
│   │   ├── services/   # 业务逻辑
│   │   ├── models/     # 数据模型
│   │   ├── schemas/    # 数据校验
│   │   └── utils/      # 工具函数
│   ├── config/         # 配置文件
│   ├── main.py         # 主入口
│   ├── requirements.txt # 依赖管理
│   └── Dockerfile      # Dockerfile
├── frontend/           # 前端界面
│   ├── public/         # 静态文件
│   ├── src/            # 源代码
│   │   ├── components/ # 组件
│   │   ├── pages/      # 页面
│   │   ├── services/   # API服务
│   │   ├── utils/      # 工具函数
│   │   └── main.js     # 主入口
│   ├── index.html      # 主页
│   ├── style.css       # 样式文件
│   └── script.js       # 脚本文件
└── docs/               # 文档
    ├── api/            # API文档
    ├── hardware/       # 硬件文档
    └── user-guide/     # 用户指南
```

## 快速开始

### 1. 环境准备

#### 硬件环境
- ESP32开发板
- RFID模块
- 指纹模块
- 摄像头模块
- 其他外设组件

#### 软件环境
- Python 3.9+
- PostgreSQL
- Redis (可选)
- Docker (可选)
- PlatformIO (嵌入式开发)

### 2. 安装依赖

#### 后端依赖
```bash
cd backend
pip install -r requirements.txt
```

### 3. 配置环境变量

复制 `.env.example` 文件为 `.env` 并修改配置：

```bash
cp .env.example .env
```

### 4. 启动服务

#### 后端服务
```bash
cd backend
python main.py
```

#### 前端界面
直接打开 `frontend/index.html` 文件。

### 5. 硬件固件

使用PlatformIO编译并上传固件：

```bash
cd firmware
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

## 项目文档

- **API文档**：`docs/api/`
- **硬件文档**：`docs/hardware/`
- **用户指南**：`docs/user-guide/`

## 联系方式

- **项目维护**：智能门禁开发团队
- **邮箱**：contact@smart-access-control.com
- **版本**：v1.0.0
