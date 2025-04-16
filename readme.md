# Shadow System Explanation

In this project, the Shadow system simulates the AWS IoT Device Shadow, managing the state synchronization between a central control interface (frontend), a local gateway (edge device), and the physical device.

## 中文版說明
- https://github.com/JW-Albert/IOT_Shadow_Display_UnixFinal/tree/Shadow/readme_zh-tw.md
---

## What is a Shadow?

A shadow is a virtual representation of a device's state in the system. It stores the desired state, the reported state, and the delta (difference).

---

## JSON Structure Example

```json
{
  "state": {
    "desired": {
      "status": 1,
      "permission": 1
    },
    "reported": {
      "status": 0,
      "permission": 1
    }
  },
  "delta": {
    "status": 1
  }
}
```

- `desired`: what the central control wants the device to do
- `reported`: what the device is currently doing
- `delta`: the difference between `desired` and `reported` (only present when needed)

---

## Workflow

1. Central control sends desired state via API.
2. Shadow calculates delta (difference between desired and reported).
3. Local gateway fetches delta, performs hardware action.
4. Device reports actual state, which updates the `reported` field.
5. Shadow recalculates delta. If synchronized, delta becomes empty.

---

## Emergency Local Control

If central control is offline, the local gateway can enter emergency mode and override permission settings for safety reasons.

---

## File Format

Each device has a shadow stored in a JSON file:

- `shadow_device001.json`
- `shadow_device002.json`
- ...

These files are managed via the Flask backend.
