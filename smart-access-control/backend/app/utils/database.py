from sqlalchemy import create_engine
from sqlalchemy.ext.declarative import declarative_base
from sqlalchemy.orm import sessionmaker
from sqlalchemy.orm import Session
import os
from dotenv import load_dotenv

# 加载环境变量
load_dotenv()

# 数据库连接URL
DATABASE_URL = os.getenv("DATABASE_URL", "postgresql://admin:password@localhost:5432/example_db")

# 创建数据库引擎
engine = create_engine(DATABASE_URL)

# 创建会话工厂
SessionLocal = sessionmaker(autocommit=False, autoflush=False, bind=engine)

# 创建基类
Base = declarative_base()

# 依赖项
def get_db():
    """获取数据库会话"""
    db = SessionLocal()
    try:
        yield db
    finally:
        db.close()

# 数据库工具类
class DatabaseManager:
    """数据库管理类"""
    
    @staticmethod
    def init_db():
        """初始化数据库"""
        Base.metadata.create_all(bind=engine)
    
    @staticmethod
    def drop_db():
        """删除数据库表"""
        Base.metadata.drop_all(bind=engine)
    
    @staticmethod
    def get_session() -> Session:
        """获取数据库会话"""
        return SessionLocal()

# 初始化数据库
if __name__ == "__main__":
    DatabaseManager.init_db()
    print("Database initialized successfully!")
