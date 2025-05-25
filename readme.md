## 中文版說明
- https://github.com/JW-Albert/IOT_Shadow_Display_UnixFinal/tree/Shadow/readme_zh-tw.md

# Shadow System Implementation

This is a Flask-based implementation of a device shadow system, similar to AWS IoT Device Shadow. It manages device states and permissions through a RESTful API.

## Features
- Device state management (desired/reported/delta)
- Role-based access control
- Permission management
- JSON file-based storage
- RESTful API endpoints

## API Endpoints

### Shadow Management
- **GET /shadow/get**
  - Query Parameters:
    - `type`: "desired", "reported", "delta", or "full"
  - Returns the requested shadow state
  - Example Response:
    ```json
    {
      "status": 1,
      "permission": 1
    }
    ```

- **POST /shadow/update**
  - Updates shadow state
  - Request Body:
    ```json
    {
      "type": "desired",
      "data": {
        "status": 1,
        "permission": 1
      }
    }
    ```
  - Response:
    ```json
    {
      "message": "desired updated",
      "delta": {
        "status": 1
      }
    }
    ```

## Shadow Structure
```json
{
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
```

## State Management
- **Desired State**: What the system wants the device to do
- **Reported State**: What the device is actually doing
- **Delta**: Differences between desired and reported states

## Permission Control
- Only admin users can modify device permissions
- Regular users can only control device status
- Permission values:
  - 1: Allowed
  - 0: Denied

## File Storage
- Shadow states are stored in JSON files
- Default location: `../shadow/shadow.json`
- Automatic file creation if not exists

## Security
- Token-based authentication
- Role-based access control
- Request logging
- Error handling

## Dependencies
- Flask
- Python 3.x
- JSON file handling

## Getting Started

1. Install dependencies:
   ```bash
   pip install flask
   ```

2. Configure tokens in `main.py`:
   ```python
   TOKENS = {
       "admin-token-123": "admin",
       "user-token-456": "user",
       "device-token-789": "device"
   }
   ```

3. Run the server:
   ```bash
   python src/main.py
   ```

## Development Notes
- Current implementation uses file-based storage
- For production:
  - Implement database storage
  - Add proper token management
  - Enable HTTPS
  - Add rate limiting
  - Implement proper logging

## License
This project is provided for educational purposes only. Free to modify and use in academic projects.

---

*For production use, ensure proper security measures are implemented.*
