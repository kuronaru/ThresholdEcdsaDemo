from flask import Flask, render_template, jsonify, request
from flask_socketio import SocketIO, join_room, leave_room, send, emit

app = Flask(__name__)
app.config['SECRET_KEY'] = 'secret!'

socketio = SocketIO(app)

# 在线客户端计数
connected_clients = 0


@app.route('/')
def index():
    return render_template('index.html')


@app.route('/status', methods=['GET'])
def get_status():
    """Returns the number of currently connected clients."""
    return jsonify({"connected_clients": connected_clients})


@app.route('/create_clients', methods=['POST'])
def create_clients():
    """Returns dummy clients creation request for front-end to handle."""
    data = request.json
    num_clients = data.get("num_clients", 0)
    return jsonify({"status": "ok", "message": f"{num_clients} dummy clients requested"})


# 处理连接事件
@socketio.on('connect')
def handle_connect():
    global connected_clients
    connected_clients += 1
    print("客户端已连接", connected_clients)


# 处理断开事件
@socketio.on('disconnect')
def handle_disconnect():
    global connected_clients
    connected_clients -= 1
    print("客户端已断开连接", connected_clients)


# 加入房间
@socketio.on('join_room')
def handle_join_room(data):
    room = data['room']
    join_room(room)
    emit('message', f"{data['username']} 已加入房间 {room}", to=room)


# 离开房间
@socketio.on('leave_room')
def handle_leave_room(data):
    room = data['room']
    leave_room(room)
    emit('message', f"{data['username']} 已离开房间 {room}", to=room)


# 接收消息并广播到房间
@socketio.on('send_message')
def handle_message(data):
    room = data['room']
    msg = f"{data['username']}: {data['message']}"
    send(msg, to=room)


if __name__ == '__main__':
    socketio.run(app, debug=True)