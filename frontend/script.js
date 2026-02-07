// 全局变量
let currentUser = null;
let token = null;

// DOM元素
const loginPage = document.getElementById('login-page');
const dashboardPage = document.getElementById('dashboard-page');
const loginForm = document.getElementById('login-form');
const loginMessage = document.getElementById('login-message');
const logoutBtn = document.getElementById('logout-btn');
const currentUserElement = document.getElementById('current-user');

// 侧边栏菜单
const menuItems = document.querySelectorAll('.menu-item');
const sections = document.querySelectorAll('.section');

// 仪表盘数据元素
const todayAccessElement = document.getElementById('today-access');
const onlineDevicesElement = document.getElementById('online-devices');
const alarmEventsElement = document.getElementById('alarm-events');
const registeredUsersElement = document.getElementById('registered-users');
const activityListElement = document.getElementById('activity-list');

// 初始化
function init() {
    // 绑定登录表单提交事件
    if (loginForm) {
        loginForm.addEventListener('submit', handleLogin);
    }
    
    // 绑定退出登录事件
    if (logoutBtn) {
        logoutBtn.addEventListener('click', handleLogout);
    }
    
    // 绑定侧边栏菜单点击事件
    menuItems.forEach(item => {
        item.addEventListener('click', function(e) {
            e.preventDefault();
            const target = this.getAttribute('data-target');
            switchSection(target);
            
            // 更新菜单激活状态
            menuItems.forEach(menuItem => menuItem.classList.remove('active'));
            this.classList.add('active');
        });
    });
    
    // 检查本地存储中的登录状态
    checkLoginStatus();
}

// 检查登录状态
function checkLoginStatus() {
    const storedToken = localStorage.getItem('token');
    const storedUser = localStorage.getItem('user');
    
    if (storedToken && storedUser) {
        token = storedToken;
        currentUser = JSON.parse(storedUser);
        showDashboard();
        loadDashboardData();
    }
}

// 处理登录
function handleLogin(e) {
    e.preventDefault();
    
    const username = document.getElementById('username').value;
    const password = document.getElementById('password').value;
    
    // 模拟登录验证
    if (username === 'admin' && password === 'admin123') {
        currentUser = { id: 1, username: 'admin', role: 'admin' };
        token = 'mock-token-' + Date.now();
        
        // 存储到本地存储
        localStorage.setItem('token', token);
        localStorage.setItem('user', JSON.stringify(currentUser));
        
        showDashboard();
        loadDashboardData();
    } else {
        showMessage('用户名或密码错误', 'error');
    }
}

// 处理退出登录
function handleLogout() {
    // 清除本地存储
    localStorage.removeItem('token');
    localStorage.removeItem('user');
    
    currentUser = null;
    token = null;
    
    showLoginPage();
}

// 显示登录页面
function showLoginPage() {
    loginPage.classList.add('active');
    dashboardPage.classList.remove('active');
}

// 显示仪表盘
function showDashboard() {
    loginPage.classList.remove('active');
    dashboardPage.classList.add('active');
    
    if (currentUserElement && currentUser) {
        currentUserElement.textContent = currentUser.username;
    }
}

// 切换section
function switchSection(targetId) {
    sections.forEach(section => {
        section.classList.remove('active');
    });
    
    const targetSection = document.getElementById(targetId);
    if (targetSection) {
        targetSection.classList.add('active');
    }
}

// 显示消息
function showMessage(text, type) {
    if (loginMessage) {
        loginMessage.textContent = text;
        loginMessage.className = 'message ' + type;
        
        // 3秒后清除消息
        setTimeout(() => {
            loginMessage.textContent = '';
            loginMessage.className = 'message';
        }, 3000);
    }
}

// 加载仪表盘数据
function loadDashboardData() {
    // 模拟数据
    const dashboardData = {
        todayAccess: 128,
        onlineDevices: 8,
        alarmEvents: 3,
        registeredUsers: 156
    };
    
    // 更新统计数据
    if (todayAccessElement) todayAccessElement.textContent = dashboardData.todayAccess;
    if (onlineDevicesElement) onlineDevicesElement.textContent = dashboardData.onlineDevices;
    if (alarmEventsElement) alarmEventsElement.textContent = dashboardData.alarmEvents;
    if (registeredUsersElement) registeredUsersElement.textContent = dashboardData.registeredUsers;
    
    // 加载最近活动
    loadRecentActivity();
    
    // 加载设备列表
    loadDevices();
    
    // 加载用户列表
    loadUsers();
    
    // 加载访问记录
    loadRecords();
    
    // 加载系统设置
    loadSettings();
}

// 加载最近活动
function loadRecentActivity() {
    const activities = [
        { id: 1, user: '张三', action: '刷卡开门', time: '2024-01-01 14:30:25' },
        { id: 2, user: '李四', action: '指纹开门', time: '2024-01-01 14:25:18' },
        { id: 3, user: '王五', action: '密码开门', time: '2024-01-01 14:20:45' },
        { id: 4, user: '赵六', action: '人脸识别开门', time: '2024-01-01 14:15:30' },
        { id: 5, user: '管理员', action: '远程开门', time: '2024-01-01 14:10:12' }
    ];
    
    if (activityListElement) {
        activityListElement.innerHTML = '';
        
        activities.forEach(activity => {
            const activityItem = document.createElement('div');
            activityItem.className = 'activity-item';
            activityItem.innerHTML = `
                <div class="activity-info">
                    <h4>${activity.user}</h4>
                    <p>${activity.action}</p>
                </div>
                <div class="activity-time">${activity.time}</div>
            `;
            activityListElement.appendChild(activityItem);
        });
    }
}

// 加载设备列表
function loadDevices() {
    const devices = [
        { id: 1, name: '前门门禁', status: 'online', location: '公司前台', lastActive: '2024-01-01 14:30:25' },
        { id: 2, name: '后门门禁', status: 'online', location: '公司后门', lastActive: '2024-01-01 14:25:18' },
        { id: 3, name: '仓库门禁', status: 'offline', location: '仓库', lastActive: '2024-01-01 14:20:45' },
        { id: 4, name: '机房门禁', status: 'online', location: '机房', lastActive: '2024-01-01 14:15:30' }
    ];
    
    const devicesListElement = document.getElementById('devices-list');
    if (devicesListElement) {
        devicesListElement.innerHTML = '';
        
        devices.forEach(device => {
            const deviceItem = document.createElement('div');
            deviceItem.className = 'device-item';
            deviceItem.innerHTML = `
                <div class="device-info">
                    <h3>${device.name}</h3>
                    <p>位置: ${device.location}</p>
                    <p>最后活跃: ${device.lastActive}</p>
                </div>
                <div class="device-actions">
                    <span class="device-status status-${device.status}">
                        ${device.status === 'online' ? '在线' : '离线'}
                    </span>
                    <button class="btn-secondary">控制</button>
                </div>
            `;
            devicesListElement.appendChild(deviceItem);
        });
    }
}

// 加载用户列表
function loadUsers() {
    const users = [
        { id: 1, username: 'admin', name: '管理员', role: 'admin', status: 'active' },
        { id: 2, username: 'zhangsan', name: '张三', role: 'user', status: 'active' },
        { id: 3, username: 'lisi', name: '李四', role: 'user', status: 'active' },
        { id: 4, username: 'wangwu', name: '王五', role: 'user', status: 'inactive' }
    ];
    
    const usersListElement = document.getElementById('users-list');
    if (usersListElement) {
        usersListElement.innerHTML = '';
        
        users.forEach(user => {
            const userItem = document.createElement('div');
            userItem.className = 'user-item';
            userItem.innerHTML = `
                <div class="user-info">
                    <h3>${user.name}</h3>
                    <p>用户名: ${user.username}</p>
                    <p class="user-role">角色: ${user.role === 'admin' ? '管理员' : '普通用户'}</p>
                </div>
                <div class="user-actions">
                    <span class="device-status status-${user.status}">
                        ${user.status === 'active' ? '活跃' : '禁用'}
                    </span>
                    <button class="btn-secondary">编辑</button>
                </div>
            `;
            usersListElement.appendChild(userItem);
        });
    }
}

// 加载访问记录
function loadRecords() {
    const records = [
        { id: 1, user: '张三', device: '前门门禁', method: '刷卡', time: '2024-01-01 14:30:25', result: 'success' },
        { id: 2, user: '李四', device: '后门门禁', method: '指纹', time: '2024-01-01 14:25:18', result: 'success' },
        { id: 3, user: '陌生人', device: '仓库门禁', method: '密码', time: '2024-01-01 14:20:45', result: 'failed' },
        { id: 4, user: '赵六', device: '机房门禁', method: '人脸识别', time: '2024-01-01 14:15:30', result: 'success' },
        { id: 5, user: '管理员', device: '前门门禁', method: '远程', time: '2024-01-01 14:10:12', result: 'success' }
    ];
    
    const recordsListElement = document.getElementById('records-list');
    if (recordsListElement) {
        recordsListElement.innerHTML = '';
        
        records.forEach(record => {
            const recordItem = document.createElement('div');
            recordItem.className = 'record-item';
            recordItem.innerHTML = `
                <div class="record-info">
                    <h3>${record.user}</h3>
                    <p>${record.device} - ${record.method}</p>
                    <p class="record-time">${record.time}</p>
                </div>
                <div class="record-result result-${record.result}">
                    ${record.result === 'success' ? '成功' : '失败'}
                </div>
            `;
            recordsListElement.appendChild(recordItem);
        });
    }
}

// 加载系统设置
function loadSettings() {
    const settingsFormElement = document.querySelector('.settings-form');
    if (settingsFormElement) {
        settingsFormElement.innerHTML = `
            <div class="settings-section">
                <h3>系统配置</h3>
                <div class="form-group">
                    <label for="system-name">系统名称</label>
                    <input type="text" id="system-name" value="智能门禁管理控制平台">
                </div>
                <div class="form-group">
                    <label for="system-timezone">时区设置</label>
                    <select id="system-timezone">
                        <option value="Asia/Shanghai" selected>Asia/Shanghai (UTC+8)</option>
                        <option value="America/New_York">America/New_York (UTC-5)</option>
                        <option value="Europe/London">Europe/London (UTC+0)</option>
                    </select>
                </div>
                <div class="form-group">
                    <label for="system-language">语言设置</label>
                    <select id="system-language">
                        <option value="zh-CN" selected>中文</option>
                        <option value="en-US">English</option>
                    </select>
                </div>
            </div>
            
            <div class="settings-section">
                <h3>安全设置</h3>
                <div class="form-group">
                    <label for="password-policy">密码策略</label>
                    <select id="password-policy">
                        <option value="low">低 (6位以上)</option>
                        <option value="medium" selected>中 (8位以上，包含字母数字)</option>
                        <option value="high">高 (10位以上，包含字母数字特殊字符)</option>
                    </select>
                </div>
                <div class="form-group">
                    <label for="session-timeout">会话超时</label>
                    <input type="number" id="session-timeout" value="30" min="5" max="120"> 分钟
                </div>
                <div class="form-group">
                    <label>
                        <input type="checkbox" id="enable-2fa" checked> 启用双因素认证
                    </label>
                </div>
            </div>
            
            <div class="settings-section">
                <h3>通知设置</h3>
                <div class="form-group">
                    <label>
                        <input type="checkbox" id="enable-email-notification" checked> 启用邮件通知
                    </label>
                </div>
                <div class="form-group">
                    <label for="email-server">邮件服务器</label>
                    <input type="text" id="email-server" value="smtp.example.com">
                </div>
                <div class="form-group">
                    <label>
                        <input type="checkbox" id="enable-sms-notification"> 启用短信通知
                    </label>
                </div>
            </div>
            
            <div class="settings-section">
                <h3>数据管理</h3>
                <div class="form-group">
                    <label for="data-retention">数据保留期</label>
                    <select id="data-retention">
                        <option value="30">30天</option>
                        <option value="90" selected>90天</option>
                        <option value="180">180天</option>
                        <option value="365">365天</option>
                    </select>
                </div>
                <div class="form-group">
                    <button class="btn-secondary">导出数据</button>
                    <button class="btn-secondary">导入数据</button>
                </div>
            </div>
            
            <div class="settings-actions">
                <button class="btn-primary">保存设置</button>
                <button class="btn-secondary">恢复默认</button>
            </div>
        `;
    }
}

// 显示消息
function showMessage(text, type) {
    if (loginMessage) {
        loginMessage.textContent = text;
        loginMessage.className = 'message ' + type;
        
        // 3秒后清除消息
        setTimeout(() => {
            loginMessage.textContent = '';
            loginMessage.className = 'message';
        }, 3000);
    }
}

// 模拟API请求
function fetchAPI(endpoint, method = 'GET', data = null) {
    return new Promise((resolve, reject) => {
        // 模拟网络延迟
        setTimeout(() => {
            // 模拟API响应
            const mockResponses = {
                '/api/auth/login': {
                    success: true,
                    data: {
                        token: 'mock-token-' + Date.now(),
                        user: { id: 1, username: 'admin', role: 'admin' }
                    }
                },
                '/api/dashboard/stats': {
                    success: true,
                    data: {
                        todayAccess: 128,
                        onlineDevices: 8,
                        alarmEvents: 3,
                        registeredUsers: 156
                    }
                }
            };
            
            if (mockResponses[endpoint]) {
                resolve(mockResponses[endpoint]);
            } else {
                reject({ error: 'Endpoint not found' });
            }
        }, 500);
    });
}

// 页面加载完成后初始化
if (document.readyState === 'loading') {
    document.addEventListener('DOMContentLoaded', init);
} else {
    init();
}
