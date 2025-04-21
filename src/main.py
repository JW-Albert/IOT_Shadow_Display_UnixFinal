import time
import requests

SHADOW_API_BASE = "https://unix.jw-albert.dev/api/shadow"
AUTH_COOKIE = {"token": "device-token-789"}

def get_delta():
    try:
        res = requests.get(f"{SHADOW_API_BASE}/get?type=delta", cookies=AUTH_COOKIE, timeout=5)
        return res.json()
    except Exception as e:
        print("無法取得 delta：", e)
        return {}

def report_state(state: dict):
    try:
        payload = {
            "type": "reported",
            "data": state
        }
        res = requests.post(f"{SHADOW_API_BASE}/update", json=payload, cookies=AUTH_COOKIE, timeout=5)
        print("已上報狀態：", res.json())
    except Exception as e:
        print("無法回報狀態：", e)

def main_loop():
    print("Local Gateway 啟動中...")
    while True:
        delta = get_delta()
        if delta:
            print("收到 delta：", delta)
            time.sleep(1)
            report_state(delta)
        time.sleep(2)

if __name__ == "__main__":
    main_loop()
