export function createClient(clientCount, clientsContainer, io, logOutput) {
    const clientId = `client_${clientCount}`;

    // 创建客户端DOM元素
    const clientBox = document.createElement('div');
    clientBox.className = 'client-box';
    clientBox.id = clientId;
    clientBox.innerHTML = `
        <div style="position: relative;">
            <button id="${clientId}_close" 
                    style="
                        position: absolute; 
                        top: 0px; 
                        right: 0px; 
                        background: none; 
                        border: none; 
                        color: #787878; 
                        font-size: 25px; 
                        cursor: pointer;"
                    title="关闭">×</button>
        </div>
        <h4 style="display: inline;">客户端：</h4>
        <h4 style="display: inline;"
            contenteditable="true"
            id="${clientId}_username"
            ondblclick="this.focus()">${clientId}</h4>
        <label>
            <input type="checkbox" id="${clientId}_ecdsa"> 启用 ECDSA
        </label>
        <br>
        <input type="text"
               id="${clientId}_message"
               placeholder="输入消息..."
               class="form-control">
        <button id="${clientId}_send" class="form-control">发送</button>
    `;
    clientsContainer.appendChild(clientBox);

    // 设置模拟客户端的WebSocket连接
    const fakeSocket = io();

    // 发送按钮功能
    document.getElementById(`${clientId}_send`).addEventListener('click', () => {
        const username = document.getElementById(`${clientId}_username`).value.trim();
        const message = document.getElementById(`${clientId}_message`).value.trim();
        const ecdsaEnabled = document.getElementById(`${clientId}_ecdsa`).checked; // 是否启用ECDSA

        if (!username || !message) {
            alert("请输入用户名和消息！");
            return;
        }

        if (ecdsaEnabled) {
            console.log(`[${clientId}] 启用了ECDSA`);
            // 假装在这里对消息进行签名的逻辑
            const messageWithECDSA = `ECDSA签名消息: ${message}`;
            fakeSocket.emit('send_message', { username, room: "default", message: messageWithECDSA });
        } else {
            fakeSocket.emit('send_message', { username, room: "default", message });
        }

        document.getElementById(`${clientId}_message`).value = ''; // 清空消息框
    });

    // 自动模拟该客户端加入默认房间
    fakeSocket.emit('join_room', { username: `User_${clientCount}`, room: "default" });

    // 接收服务器消息，更新到日志和当前客户端的输出
    fakeSocket.on('message', (msg) => {
        const log = document.createElement('div');
        log.innerText = msg;
        logOutput.appendChild(log);
        logOutput.scrollTop = logOutput.scrollHeight; // 滚动到底部
    });

    // 事件监听：标题修改后执行逻辑（可选）
    document.getElementById(`${clientId}_username`).addEventListener('blur', (e) => {
        const newTitle = e.target.innerText.trim();
        console.log(`客户端 ${clientId} 标题已修改为: ${newTitle}`);
    });

    // 关闭按钮功能
    document.getElementById(`${clientId}_close`).addEventListener('click', () => {
        // 移除当前客户端的 DOM
        const clientElement = document.getElementById(clientId);
        if (clientElement) {
            clientsContainer.removeChild(clientElement);
        }

        // 断开 Socket.IO 连接
        fakeSocket.disconnect();

        // 控制台日志
        console.log(`客户端 ${clientId} 已关闭并断开连接`);
    });

    return clientId;
}