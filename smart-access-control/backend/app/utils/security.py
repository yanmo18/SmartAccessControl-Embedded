from datetime import datetime, timedelta
from typing import Optional, Union
from jose import JWTError, jwt
from passlib.context import CryptContext
import os
from dotenv import load_dotenv

# 加载环境变量
load_dotenv()

# 密钥配置
SECRET_KEY = os.getenv("SECRET_KEY", "your-secret-key-here")
ALGORITHM = os.getenv("ALGORITHM", "HS256")
ACCESS_TOKEN_EXPIRE_MINUTES = int(os.getenv("ACCESS_TOKEN_EXPIRE_MINUTES", "30"))

# 密码加密上下文
pwd_context = CryptContext(schemes=["bcrypt"], deprecated="auto")

# 密码工具类
class PasswordManager:
    """密码管理类"""
    
    @staticmethod
    def verify_password(plain_password: str, hashed_password: str) -> bool:
        """验证密码"""
        return pwd_context.verify(plain_password, hashed_password)
    
    @staticmethod
    def get_password_hash(password: str) -> str:
        """获取密码哈希值"""
        return pwd_context.hash(password)

# JWT工具类
class JWTManager:
    """JWT管理类"""
    
    @staticmethod
    def create_access_token(data: dict, expires_delta: Optional[timedelta] = None) -> str:
        """创建访问令牌"""
        to_encode = data.copy()
        if expires_delta:
            expire = datetime.utcnow() + expires_delta
        else:
            expire = datetime.utcnow() + timedelta(minutes=ACCESS_TOKEN_EXPIRE_MINUTES)
        
        to_encode.update({"exp": expire})
        encoded_jwt = jwt.encode(to_encode, SECRET_KEY, algorithm=ALGORITHM)
        return encoded_jwt
    
    @staticmethod
    def decode_token(token: str) -> Optional[dict]:
        """解码令牌"""
        try:
            payload = jwt.decode(token, SECRET_KEY, algorithms=[ALGORITHM])
            return payload
        except JWTError:
            return None
    
    @staticmethod
    def get_current_user_id(token: str) -> Optional[int]:
        """从令牌中获取用户ID"""
        payload = JWTManager.decode_token(token)
        if payload:
            return payload.get("sub")
        return None

# 权限检查
class PermissionChecker:
    """权限检查类"""
    
    @staticmethod
    def has_permission(user_role: str, required_role: str) -> bool:
        """检查用户权限"""
        roles = {
            "admin": 3,
            "manager": 2,
            "user": 1,
            "guest": 0
        }
        
        user_level = roles.get(user_role, 0)
        required_level = roles.get(required_role, 0)
        
        return user_level >= required_level

if __name__ == "__main__":
    # 测试密码加密
    password = "test123"
    hashed = PasswordManager.get_password_hash(password)
    print(f"Password: {password}")
    print(f"Hashed: {hashed}")
    print(f"Verified: {PasswordManager.verify_password(password, hashed)}")
    
    # 测试JWT
    token = JWTManager.create_access_token({"sub": "1", "name": "Admin", "role": "admin"})
    print(f"Token: {token}")
    payload = JWTManager.decode_token(token)
    print(f"Payload: {payload}")
    
    # 测试权限检查
    print(f"Admin has admin permission: {PermissionChecker.has_permission('admin', 'admin')}")
    print(f"User has admin permission: {PermissionChecker.has_permission('user', 'admin')}")
