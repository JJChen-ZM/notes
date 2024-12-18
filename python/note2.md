## 输入网址先进入index.html, 但没有检测到用户登录会跳转到login.html

为实现用户访问根路径（如 `http://127.0.0.1:8080/`）时，默认先加载 `index.html`，但如果未检测到用户已登录，则自动跳转到 `login.html`，可以通过以下逻辑实现：

1. **利用会话管理**：
   使用 CherryPy 的内置会话工具 (`cherrypy.session`) 来记录用户登录状态。

2. **条件判断**：
   在访问 `index.html` 时，检查用户是否已经登录（例如检查 `session` 中是否有用户名）。如果未登录，则重定向到登录页面。

---

### 修改代码

#### **更新后的 `app.py`**

```python
import cherrypy
import json
import os
from datetime import datetime

# 用户数据文件和日志文件路径
USERS_FILE = "logs/users.json"
LOG_FILE = "logs/operations.log"

# 确保日志目录存在
os.makedirs("logs", exist_ok=True)

# 如果用户文件不存在，则初始化
if not os.path.exists(USERS_FILE):
    with open(USERS_FILE, "w") as f:
        json.dump({}, f)


def log_operation(username, operation):
    """记录用户操作到日志文件"""
    timestamp = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
    with open(LOG_FILE, "a") as f:
        f.write(f"{timestamp} - {username}: {operation}\n")


class LoginApp:
    @cherrypy.expose
    def index(self):
        """主页，检查登录状态"""
        if "username" in cherrypy.session:
            # 用户已登录，加载主页
            return open("public/index.html").read()
        else:
            # 用户未登录，跳转到登录页面
            raise cherrypy.HTTPRedirect("/login_page")

    @cherrypy.expose
    def login_page(self):
        """提供静态 HTML 登录页面"""
        return open("public/login.html").read()

    @cherrypy.expose
    @cherrypy.tools.json_in()
    @cherrypy.tools.json_out()
    def login(self):
        """处理用户登录请求"""
        input_data = cherrypy.request.json
        username = input_data.get("username")
        password = input_data.get("password")

        with open(USERS_FILE, "r") as f:
            users = json.load(f)

        if username in users and users[username] == password:
            # 登录成功，保存到会话
            cherrypy.session["username"] = username
            log_operation(username, "User logged in")
            return {"status": "success", "message": "Login successful", "redirect": "/index"}
        else:
            return {"status": "error", "message": "Invalid username or password"}

    @cherrypy.expose
    @cherrypy.tools.json_in()
    @cherrypy.tools.json_out()
    def register(self):
        """处理用户注册请求"""
        input_data = cherrypy.request.json
        username = input_data.get("username")
        password = input_data.get("password")

        with open(USERS_FILE, "r") as f:
            users = json.load(f)

        if username in users:
            return {"status": "error", "message": "Username already exists"}
        else:
            users[username] = password
            with open(USERS_FILE, "w") as f:
                json.dump(users, f)
            log_operation(username, "User registered")
            return {"status": "success", "message": "Registration successful"}

    @cherrypy.expose
    def logout(self):
        """处理用户注销请求"""
        username = cherrypy.session.pop("username", None)
        if username:
            log_operation(username, "User logged out")
        raise cherrypy.HTTPRedirect("/login_page")


if __name__ == "__main__":
    # 配置 CherryPy 静态文件路径和会话工具
    config = {
        "/": {
            "tools.staticdir.on": True,
            "tools.staticdir.dir": os.path.abspath("public"),
        },
        "/": {
            "tools.sessions.on": True,  # 启用会话管理
            "tools.sessions.storage_type": "ram",
            "tools.sessions.timeout": 60,  # 会话超时时间（分钟）
        },
    }

    # 启动应用
    cherrypy.quickstart(LoginApp(), "/", config)
```

---

### 改动说明

1. **启用会话**：
   - 配置 `tools.sessions.on` 来启用会话管理。
   - 使用 `cherrypy.session` 存储登录状态。

2. **登录状态检查**：
   - 在 `index` 方法中，检查 `cherrypy.session` 是否包含 `username`。
   - 如果未登录，使用 `raise cherrypy.HTTPRedirect("/login_page")` 跳转到登录页面。

3. **登录逻辑**：
   - 成功登录后，将用户名存储到会话中（`cherrypy.session["username"] = username`）。

4. **注销功能**：
   - 提供 `logout` 方法，清除会话并重定向到登录页面。

---

### 测试步骤

1. 启动应用：
   ```bash
   python app.py
   ```

2. 访问 `http://127.0.0.1:8080/`：
   - 如果未登录，会自动跳转到登录页面。
   - 登录成功后会显示 `index.html`。

3. 测试注销：
   - 登录后访问 `http://127.0.0.1:8080/logout`，会返回到登录页面。

---

### 效果

- 默认访问 `/` 时先加载 `index.html`。
- 如果用户未登录，会自动跳转到 `login.html`。
- 登录成功后可以正常访问主页，支持用户注销功能。


## WebSocket 和 CherryPy 的公用端口号，并且透过js的websocket将log传递到python并打印

为了实现 WebSocket 和 HTTP 使用同一端口号，并通过 JavaScript 的 WebSocket 将日志传递给 Python 后端，可以将 CherryPy 和 WebSocket 协议结合在一个服务中。以下是完整的实现：

---

### **1. CherryPy 服务端代码**

```python
import cherrypy
from cherrypy.process import servers
from ws4py.server.cherrypyserver import WebSocketPlugin, WebSocketTool
from ws4py.websocket import WebSocket

# WebSocket Handler
class LogWebSocketHandler(WebSocket):
    def received_message(self, message):
        print(f"Received log: {message}")

    def closed(self, code, reason=None):
        print(f"WebSocket closed. Code: {code}, Reason: {reason}")


# CherryPy Application
class LogServer:
    @cherrypy.expose
    def index(self):
        return open("index.html").read()

    @cherrypy.expose
    def ws(self):
        # WebSocket endpoint
        pass


if __name__ == "__main__":
    # Enable WebSocket Plugin and Tool
    WebSocketPlugin(cherrypy.engine).subscribe()
    cherrypy.tools.websocket = WebSocketTool()

    # CherryPy Configuration
    config = {
        '/': {
            'tools.staticdir.on': True,
            'tools.staticdir.dir': '.',
            'tools.staticdir.index': 'index.html',
        },
        '/ws': {
            'tools.websocket.on': True,
            'tools.websocket.handler_cls': LogWebSocketHandler,
        }
    }

    # Start CherryPy server
    cherrypy.quickstart(LogServer(), '/', config)
```

---

### **2. 前端 HTML 文件 (`index.html`)**

```html
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>WebSocket Logger</title>
</head>
<body>
    <h1>WebSocket Logger</h1>
    <button id="send-log">Send Log</button>
    <div id="status"></div>
    <script>
        // Connect to WebSocket
        const ws = new WebSocket("ws://" + window.location.host + "/ws");

        // WebSocket Event Handlers
        ws.onopen = () => {
            document.getElementById("status").textContent = "WebSocket connection established.";
        };

        ws.onmessage = (event) => {
            console.log("Message from server:", event.data);
        };

        ws.onclose = () => {
            document.getElementById("status").textContent = "WebSocket connection closed.";
        };

        ws.onerror = (error) => {
            console.error("WebSocket error:", error);
        };

        // Send log to WebSocket
        document.getElementById("send-log").addEventListener("click", () => {
            if (ws.readyState === WebSocket.OPEN) {
                ws.send("This is a log message from JS!");
            } else {
                console.error("WebSocket is not open.");
            }
        });
    </script>
</body>
</html>
```

---

### **3. 功能实现说明**

1. **WebSocket 与 HTTP 共用端口：**
   - CherryPy 启动一个 HTTP 服务器，同时通过 `ws4py` 插件支持 WebSocket。
   - WebSocket 的路径 `/ws` 通过 CherryPy 的工具 `WebSocketTool` 处理。

2. **日志传递：**
   - 前端 JavaScript 通过 WebSocket 向 `/ws` 发送日志。
   - 后端的 `LogWebSocketHandler` 接收消息，并在服务器端打印日志。

3. **静态文件服务：**
   - CherryPy 静态文件工具 (`tools.staticdir`) 提供前端页面。

---

### **4. 测试步骤**

1. **启动服务器：**
   ```bash
   python server.py
   ```

2. **访问页面：**
   打开浏览器访问 `http://localhost:8080`。

3. **发送日志：**
   - 点击页面上的 "Send Log" 按钮。
   - 查看服务器终端输出，应该能看到日志内容。

4. **调试连接：**
   - 在浏览器控制台中输入以下代码，手动发送日志：
     ```javascript
     ws.send("Manual log from browser console!");
     ```

---

### **5. 示例运行效果**

#### **服务器输出：**
```plaintext
WebSocket connection established.
Received log: This is a log message from JS!
Received log: Manual log from browser console!
```

#### **浏览器控制台：**
```plaintext
WebSocket connection established.
Message from server: Acknowledged log: This is a log message from JS!
```

---

### **6. 依赖安装**

- `ws4py` 是 WebSocket 支持所需的库，可通过 pip 安装：
  ```bash
  pip install ws4py
  ```

---

通过这种方式，WebSocket 和 HTTP 共用端口，并可以将前端日志无缝传递到后端打印或存储。



## 将 /ws 路径的配置写入server.conf 中
要将 `/ws` 路径的配置写入 CherryPy 的外部 `server.conf` 文件，您可以通过以下步骤实现。

---

### **1. 配置 `server.conf`**

创建一个 `server.conf` 文件，并在其中为 `/` 和 `/ws` 路径分别配置。内容如下：

```ini
[/]
tools.staticdir.on = True
tools.staticdir.dir = .
tools.staticdir.index = index.html

[/ws]
tools.websocket.on = True
tools.websocket.handler_cls = __main__.LogWebSocketHandler
```

## 将 setup_logger() 变成 LogWebSocketHandler 类的成员函数可以吗？
将 `setup_logger()` 变成 `LogWebSocketHandler` 类的成员函数是可行的，但需要确保以下几点：

1. **每个 `LogWebSocketHandler` 实例共享同一个日志记录器**  
   日志记录器是全局的工具，通常不需要为每个类实例重新创建。因此，我们需要避免在每次实例化 `LogWebSocketHandler` 时重复初始化日志记录器。

2. **处理器不会重复添加**  
   如果不小心每次都为日志记录器添加处理器，会导致日志重复打印。

---

### **改造方法：将 `setup_logger` 合并到类中**

可以将 `setup_logger` 方法作为 `LogWebSocketHandler` 的类方法 (`@classmethod`) 或静态方法 (`@staticmethod`) 实现，从而确保日志记录器只被初始化一次。

#### **实现代码**

```python
import logging
from logging.handlers import TimedRotatingFileHandler
import os
from threading import Lock
from ws4py.websocket import WebSocket

# 全局锁
logger_lock = Lock()

class LogWebSocketHandler(WebSocket):
    logger = None  # 静态成员变量，所有实例共享
    logger_initialized = False

    @classmethod
    def setup_logger(cls):
        """初始化日志记录器，仅调用一次"""
        with logger_lock:  # 加锁避免多线程冲突
            if cls.logger_initialized:
                return cls.logger  # 如果已经初始化，则直接返回

            log_dir = "logs"
            os.makedirs(log_dir, exist_ok=True)  # 确保日志目录存在

            log_file = os.path.join(log_dir, "zm_radar.log")
            cls.logger = logging.getLogger("ZMRadarLogger")
            cls.logger.setLevel(logging.INFO)

            # 确保不重复添加处理器
            if not cls.logger.handlers:
                handler = TimedRotatingFileHandler(
                    filename=log_file,
                    when="midnight",   # 每天午夜分割日志
                    interval=1,
                    backupCount=7,
                    encoding="utf-8"
                )
                formatter = logging.Formatter('%(asctime)s - %(levelname)s - %(message)s')
                handler.setFormatter(formatter)
                cls.logger.addHandler(handler)

            cls.logger_initialized = True
            return cls.logger

    def opened(self):
        # 确保日志记录器已设置
        self.logger = self.setup_logger()

        session_id = self.environ.get('HTTP_COOKIE', '').split('=')[-1]
        self.username = session_data.get(session_id, "Anonymous")

        self.logger.info(f"WebSocket opened by user: {self.username}")
        print(f"WebSocket opened by user: {self.username}")

    def received_message(self, message):
        self.logger.info(f"Received log: {message} from User: {self.username}")
        print(f"Received log: {message} from User: {self.username}")

    def closed(self, code, reason=None):
        self.logger.info(f"WebSocket closed. Code: {code}, Reason: {reason}")
        print(f"WebSocket closed. Code: {code}, Reason: {reason}")
```

---

### **说明**

1. **类方法 `setup_logger`**:
   - 确保日志记录器是类级别的，只会初始化一次。
   - 通过 `cls.logger_initialized` 标志变量避免重复初始化。
   - 使用 `logger_lock` 确保多线程下的安全性。

2. **实例方法访问共享日志记录器**:
   - `self.logger` 被初始化为类级别的 `logger`。
   - 每个 `LogWebSocketHandler` 实例共享同一个日志记录器。

---

### **调用示例**

使用 `LogWebSocketHandler` 时无需额外操作，日志记录器会自动初始化并用于所有实例。

---

### **验证**

1. **避免重复日志**:
   - 确保日志文件中没有重复的打印内容。
   - 检查 `logger.handlers` 长度，确保处理器没有重复添加。

2. **并发安全**:
   - 如果多个 `WebSocket` 连接同时打开，日志记录器的初始化仍然是线程安全的。

---

### **优点**

- 避免重复日志问题。
- 日志记录器的管理集中在类内，减少外部依赖。
- 实例共享日志记录器，降低内存开销。
