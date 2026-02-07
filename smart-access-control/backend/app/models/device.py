from sqlalchemy import Column, Integer, String, DateTime
from sqlalchemy.sql import func
from app.utils.database import Base

class Device(Base):
    """设备模型"""
    __tablename__ = "devices"
    
    id = Column(Integer, primary_key=True, index=True)
    device_id = Column(String(50), unique=True, nullable=False, index=True)
    name = Column(String(100), nullable=False)
    location = Column(String(255))
    type = Column(String(50), nullable=False)
    status = Column(String(20), nullable=False, default="offline")
    ip_address = Column(String(50))
    mac_address = Column(String(50))
    firmware_version = Column(String(20))
    last_heartbeat = Column(DateTime(timezone=True))
    created_at = Column(DateTime(timezone=True), server_default=func.now())
    updated_at = Column(DateTime(timezone=True), server_default=func.now(), onupdate=func.now())
    
    def __repr__(self):
        return f"<Device(id={self.id}, name='{self.name}', status='{self.status}')>"
    
    def to_dict(self):
        """转换为字典"""
        return {
            "id": self.id,
            "device_id": self.device_id,
            "name": self.name,
            "location": self.location,
            "type": self.type,
            "status": self.status,
            "ip_address": self.ip_address,
            "mac_address": self.mac_address,
            "firmware_version": self.firmware_version,
            "last_heartbeat": self.last_heartbeat.isoformat() if self.last_heartbeat else None,
            "created_at": self.created_at.isoformat() if self.created_at else None,
            "updated_at": self.updated_at.isoformat() if self.updated_at else None
        }
