#include "camera.hpp"

Camera::Camera(glm::vec3 Position, glm::vec3 FrontVector, glm::vec3 UpVector)
{
    this->fov   = 45.0f;
    this->pitch = 0.0f;
    this->yaw   = -89.0f;
    sensitivity = 0.005f;
    cameraPos   = Position;
    cameraFront = FrontVector;
    cameraUp    = UpVector;
    worldUp     = UpVector;
    updateCameraVectors();
}

/* 更新攝像機的方向向量 */
void Camera::updateCameraVectors()
{
    /* 計算歐拉角，獲得方向向量 */
    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    /* 將計算完成歐拉角作用在Front上 */
    this->cameraFront = glm::normalize(front);
    /* 計算獲得右和上向量 */
    this->cameraRight = glm::normalize(glm::cross(this->cameraFront, this->worldUp));
    this->cameraUp    = glm::normalize(glm::cross(this->cameraRight, this->cameraFront));
}

/* 獲取鼠標移動事件 */
void Camera::ProcessMouseMovement(float xoffset, float yoffset)
{
    /* 設置鼠標左右移動的靈敏度 */
    xoffset *= this->sensitivity;
    yoffset *= this->sensitivity;

    /* 將偏移量加到俯仰角和偏航角上 */
    this->yaw    += xoffset;
    this->pitch  += yoffset;
    /* 設定歐拉角的範圍 */
    if(yaw > 89.0f)
        yaw   = 89.0f;
    if(yaw < -89.0f)
        yaw   = -89.0f;
    if(pitch > 89.0f)
        pitch = 89.0f;
    if(pitch < -89.0f)
        pitch = -89.0f;
    updateCameraVectors();
}
/* 獲取鼠標滾輪事件 */
void Camera::ProcessMouseScroll(float xoffset, float yoffset)
{
    /* 鼠標滾輪事件，對應視場角 */
    if(fov >= 1.0f  && fov <= 55.0f)
        fov -= yoffset;
    /* 設置fov的範圍 */
    if(fov <= 1.0f)
        fov = 1.0f;
    if(fov >= 55.0f)
        fov = 55.0f;
}
/* 獲取鍵盤按鍵事件 */
void Camera::ProcessKeyboard(enDIRECTION DIR,  float deltaTime)
{
    float cameraSpeed = 2.5f * deltaTime;
    /* 移動相機位置，與前後（X軸），左右（Z軸）的向量作加減運算 */
    switch(DIR)
    {
        case FORWARD:
            this->cameraPos += cameraSpeed * this->cameraFront;
            break;
        case BACKWARD:
            this->cameraPos -= cameraSpeed * this->cameraFront;
            break;
        case LEFT:
            this->cameraPos -= cameraSpeed * this->cameraRight;
            break;
        case RIGHT:
            this->cameraPos += cameraSpeed * this->cameraRight;
            break;
        default:
            break;
    }
}
/* 獲取觀察矩陣 */
glm::mat4 Camera::GetViewMatrix()
{
    return glm::lookAt(this->cameraPos, this->cameraPos + this->cameraFront, this->cameraUp);
}

/* 設置靈敏度 */
void Camera::SetSensitivity(float value)
{
    this->sensitivity = value;
}
/* 獲取靈敏度 */
float Camera::GetSensitivity()
{
    return this->sensitivity;
}
/* 設置FOV */
void Camera::SetFOV(float value)
{
    this->fov = value;
}
/* 獲得FOV */
float Camera::GetFOV()
{
    return this->fov;
}