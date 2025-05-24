from flask import Flask, request, jsonify, make_response
import time

app = Flask(__name__)

# 模擬帳號密碼資料庫
USERS = {
    "admin": {"password": "1234", "role": "admin", "token": "admin-token-123"},
    "user": {"password": "5678", "role": "user", "token": "user-token-456"}
}

@app.route("/login", methods=["POST"])
def login():
    data = request.get_json()
    username = data.get("username")
    password = data.get("password")

    user = USERS.get(username)
    if user and user["password"] == password:
        resp = make_response(jsonify({"status": "ok", "username": username, "role": user["role"]}))
        resp.set_cookie("token", user["token"], httponly=True, samesite="Strict", path="/")
        return resp
    return jsonify({"error": "invalid credentials"}), 401

@app.route("/whoami", methods=["GET"])
def whoami():
    token = request.cookies.get("token")
    for username, info in USERS.items():
        if info["token"] == token:
            return jsonify({"username": username, "role": info["role"]})
    return jsonify({"error": "unauthorized"}), 401

@app.route("/servertime")
def get_server_time():
    return jsonify(unixtime=int(time.time()))

if __name__ == "__main__":
    app.run(host="0.0.0.0", port=6000)
