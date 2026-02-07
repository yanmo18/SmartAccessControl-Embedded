from sqlalchemy import Column, Integer, String, ForeignKey, DateTime
from sqlalchemy.sql import func
from app.utils.database import Base

class AccessMethod(Base):
    """访问方式模型"""
    __tablename__ = "access_methods"
    
    id = Column(Integer, primary_key=True, index=True)
    user_id = Column(Integer, ForeignKey("users.id"), nullable=False, index=True)
    method_type = Column(String(20), nullable=False)  # card, fingerprint, face, password
    method_value = Column(String(255), nullable=False)  # 卡号, 指纹ID, 人脸特征, 密码哈希
    status = Column(String(20), nullable=False, default="active")
    created_at = Column(DateTime(timezone=True), server_default=func.now())
    updated_at = Column(DateTime(timezone=True), server_default=func.now(), onupdate=func.now())
    
    def __repr__(self):
        return f"<AccessMethod(id={self.id}, user_id={self.user_id}, type='{self.method_type}')>"
    
    def to_dict(self):
        """转换为字典"""
        return {
            "id": self.id,
            "user_id": self.user_id,
            "method_type": self.method_type,
            "method_value": self.method_value,
            "status": self.status,
            "created_at": self.created_at.isoformat() if self.created_at else None,
            "updated_at": self.updated_at.isoformat() if self.updated_at else None
        }
