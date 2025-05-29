## 中文版說明
- https://github.com/JW-Albert/IOT_Shadow_Display_UnixFinal/blob/auth/readme_zh-tw.md
- 
# Authentication Server for IoT Shadow System

This is a Flask-based authentication server that provides user authentication and time synchronization services for the IoT Shadow System. It handles user login, session management, and server time synchronization.

## Features
- User authentication with role-based access control
- Secure session management using HTTP-only cookies
- Server time synchronization
- Simple in-memory user database
- RESTful API endpoints

## API Endpoints

### Authentication
- **POST /login**
  - Authenticates users and creates a session
  - Request body:
    ```json
    {
      "username": "string",
      "password": "string"
    }
    ```
  - Response:
    ```json
    {
      "status": "ok",
      "username": "string",
      "role": "string"
    }
    ```
  - Sets HTTP-only cookie with user token

### User Information
- **GET /whoami**
  - Returns current user information based on session token
  - Response:
    ```json
    {
      "username": "string",
      "role": "string"
    }
    ```
  - Requires valid session cookie

### Time Synchronization
- **GET /servertime**
  - Returns current server time in Unix timestamp
  - Response:
    ```json
    {
      "unixtime": integer
    }
    ```

## User Roles
- **Admin**
  - Full system access
  - Can manage device permissions
  - Token: "admin-token-123"
- **Regular User**
  - Basic device control
  - Limited permissions
  - Token: "user-token-456"

## Security Features
- HTTP-only cookies for session management
- SameSite cookie policy
- Role-based access control
- Secure password handling
- Token-based authentication

## Configuration
Default configuration:
- Host: 0.0.0.0
- Port: 6000
- Debug mode: Enabled in development

## Dependencies
- Flask
- Python 3.x

## Getting Started

1. Install dependencies:
   ```bash
   pip install flask
   ```

2. Run the server:
   ```bash
   python src/main.py
   ```

3. The server will start on port 6000

## Development Notes
- Current implementation uses in-memory user storage
- For production, implement proper database storage
- Add proper password hashing
- Implement token expiration
- Add rate limiting
- Enable HTTPS

## License
This project is provided for educational purposes only. Free to modify and use in academic projects.

---

*For production use, ensure proper security measures are implemented.*
