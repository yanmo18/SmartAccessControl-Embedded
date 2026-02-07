from fastapi import FastAPI
from fastapi.middleware.cors import CORSMiddleware
from contextlib import asynccontextmanager

from app.api import auth, users, devices, records, alarms, permissions
from app.utils.database import engine, Base
from app.utils.logger import setup_logger

# 初始化数据库
Base.metadata.create_all(bind=engine)

# 设置日志
logger = setup_logger()

@asynccontextmanager
async def lifespan(app: FastAPI):
    # 启动时
    logger.info("Starting up Smart Access Control API...")
    yield
    # 关闭时
    logger.info("Shutting down Smart Access Control API...")

# 创建FastAPI应用
app = FastAPI(
    title="Smart Access Control API",
    description="API for Smart Access Control System",
    version="1.0.0",
    lifespan=lifespan
)

# 配置CORS
app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"],
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)

# 注册路由
app.include_router(auth.router, prefix="/api/auth", tags=["auth"])
app.include_router(users.router, prefix="/api/users", tags=["users"])
app.include_router(devices.router, prefix="/api/devices", tags=["devices"])
app.include_router(records.router, prefix="/api/records", tags=["records"])
app.include_router(alarms.router, prefix="/api/alarms", tags=["alarms"])
app.include_router(permissions.router, prefix="/api/permissions", tags=["permissions"])

# 根路径
@app.get("/")
def read_root():
    return {
        "message": "Smart Access Control API",
        "version": "1.0.0",
        "status": "running"
    }

# 健康检查
@app.get("/health")
def health_check():
    return {
        "status": "healthy",
        "timestamp": "2024-01-01T00:00:00Z"
    }

# 系统信息
@app.get("/api/system/info")
def system_info():
    return {
        "app_name": "Smart Access Control",
        "version": "1.0.0",
        "api_version": "v1",
        "features": [
            "User Authentication",
            "Device Management",
            "Access Control",
            "Alarm Management",
            "Real-time Monitoring"
        ]
    }

if __name__ == "__main__":
    import uvicorn
    uvicorn.run(
        "main:app",
        host="0.0.0.0",
        port=8000,
        reload=True
    )
