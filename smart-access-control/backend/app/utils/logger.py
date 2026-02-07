import logging
import os
from logging.handlers import RotatingFileHandler

# 日志目录
LOG_DIR = "logs"
if not os.path.exists(LOG_DIR):
    os.makedirs(LOG_DIR)

# 日志文件
LOG_FILE = os.path.join(LOG_DIR, "smart-access-control.log")

# 日志配置
def setup_logger():
    """设置日志"""
    # 创建logger
    logger = logging.getLogger("smart-access-control")
    logger.setLevel(logging.INFO)
    
    # 创建格式化器
    formatter = logging.Formatter(
        '%(asctime)s - %(name)s - %(levelname)s - %(message)s'
    )
    
    # 创建控制台处理器
    console_handler = logging.StreamHandler()
    console_handler.setLevel(logging.INFO)
    console_handler.setFormatter(formatter)
    
    # 创建文件处理器
    file_handler = RotatingFileHandler(
        LOG_FILE,
        maxBytes=10 * 1024 * 1024,  # 10MB
        backupCount=5
    )
    file_handler.setLevel(logging.INFO)
    file_handler.setFormatter(formatter)
    
    # 添加处理器
    logger.addHandler(console_handler)
    logger.addHandler(file_handler)
    
    return logger

# 获取logger
logger = setup_logger()

# 日志装饰器
def log_function(func):
    """日志装饰器"""
    def wrapper(*args, **kwargs):
        logger.info(f"Calling function: {func.__name__}")
        try:
            result = func(*args, **kwargs)
            logger.info(f"Function {func.__name__} completed successfully")
            return result
        except Exception as e:
            logger.error(f"Error in function {func.__name__}: {str(e)}")
            raise
    return wrapper

if __name__ == "__main__":
    # 测试日志
    logger.debug("Debug message")
    logger.info("Info message")
    logger.warning("Warning message")
    logger.error("Error message")
    logger.critical("Critical message")
