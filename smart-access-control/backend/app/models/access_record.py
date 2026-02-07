from sqlalchemy import Column, Integer, String, ForeignKey, DateTime
from sqlalchemy.sql import func
from app.utils.database import Base

class AccessRecord(Base):
    """门禁记录模型"""
    __tablename__ = "access_records"
    
    id = Column(Integer, primary_key=True, index=True)
    user_id = Column(Integer, ForeignKey("users.id"), index=True)
    device_id = Column(Integer, ForeignKey("devices.id"), nullable=False, index=True)
    access_time = Column(DateTime(timezone=True), nullable=False, server_default=func.now())
    access_method = Column(String(20), nullable=False)
    access_result = Column(String(20), nullable=False)
    access_message = Column(String(255))
    ip_address = Column(String(50))
    created_at = Column(DateTime(timezone=True), server_default=func.now())
    
    def __repr__(self):
        return f"<AccessRecord(id={self.id}, user_id={self.user_id}, device_id={self.device_id}, result='{self.access_result}')>"
    
    def to_dict(self):
        """转换为字典"""
        return {
            "id": self.id,
            "user_id": self.user_id,
            "device_id": self.device_id,
            "access_time": self.access_time.isoformat() if self.access_time else None,
            "access_method": self.access_method,
            "access_result": self.access_result,
            "access_message": self.access_message,
            "ip_address": self.ip_address,
            "created_at": self.created_at.isoformat() if self.created_at else None
        }
