from flask import Flask, request, jsonify
import os
import json

app = Flask(__name__)

# === 設定 ===
SHADOW_FILE = os.path.join(os.path.dirname(__file__), '../shadow/shadow.json')

# 簡易固定 token 對照表（未來可對接 authapi）
TOKENS = {
    "admin-token-123": "admin",
    "user-token-456": "user" ,
    "device-token-789": "device"
}

# === 請求前處理（授權檢查 + 請求日誌） ===
@app.before_request
def before_request():
    print(f"收到請求: {request.method} {request.path}")
    if request.method != "OPTIONS":
        token = request.cookies.get("token")
        if token not in TOKENS:
            print("Unauthorized request")
            return jsonify({"error": "Unauthorized"}), 401
        request.role = TOKENS[token]

# === 初始化 JSON 如果不存在 ===
def load_shadow():
    if not os.path.exists(SHADOW_FILE):
        os.makedirs(os.path.dirname(SHADOW_FILE), exist_ok=True)
        data = {
            "state": {
                "desired": {
                    "status": 0,
                    "permission": 1
                },
                "reported": {
                    "status": 0,
                    "permission": 1
                }
            },
            "delta": {}
        }
        save_shadow(data)
        return data
    with open(SHADOW_FILE, 'r') as f:
        return json.load(f)

def save_shadow(data):
    with open(SHADOW_FILE, 'w') as f:
        json.dump(data, f, indent=4)

# === 計算 delta 欄位 ===
def update_delta(shadow):
    desired = shadow["state"].get("desired", {})
    reported = shadow["state"].get("reported", {})
    delta = {}
    for key, val in desired.items():
        if reported.get(key) != val:
            delta[key] = val
    shadow["delta"] = delta
    return shadow

# === 更新 desired / reported ===
@app.route('/shadow/update', methods=['POST'])
def update_shadow():
    req = request.get_json()
    shadow_type = req.get("type")
    update_data = req.get("data", {})

    if shadow_type not in ["desired", "reported"]:
        return jsonify({"error": "Invalid type"}), 400

    if shadow_type == "desired" and "permission" in update_data:
        if request.role != "admin":
            return jsonify({"error": "Only admin can modify permission"}), 403

    shadow = load_shadow()
    shadow["state"][shadow_type].update(update_data)
    shadow = update_delta(shadow)
    save_shadow(shadow)

    return jsonify({
        "message": f"{shadow_type} updated",
        "delta": shadow["delta"]
    })

# === 查詢 shadow 資料 ===
@app.route('/shadow/get', methods=['GET'])
def get_shadow_data():
    shadow_type = request.args.get("type", "full")
    shadow = load_shadow()

    if shadow_type == "desired":
        return jsonify(shadow["state"]["desired"])
    elif shadow_type == "reported":
        return jsonify(shadow["state"]["reported"])
    elif shadow_type == "delta":
        return jsonify(shadow["delta"])
    elif shadow_type == "full":
        return jsonify(shadow)
    else:
        return jsonify({"error": "Invalid type"}), 400

# === 啟動 Flask 伺服器 ===
if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)
