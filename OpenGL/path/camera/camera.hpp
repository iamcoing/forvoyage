#ifndef __CAMERA_HPP__
#define __CAMERA_HPP__

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

typedef enum
{
    FORWARD     = 0,
    BACKWARD    = 1,
    LEFT        = 2,
    RIGHT       = 3,

    DIRECTION_NUM
}enDIRECTION;

class Camera
{
private:
    void updateCameraVectors();
protected:
    glm::vec3 cameraPos;
    glm::vec3 cameraFront;
    glm::vec3 cameraUp;
    glm::vec3 cameraRight;
    glm::vec3 worldUp;
    float sensitivity;
    float yaw;
    float pitch;
    float fov;
    bool firstMouse;

public:
    Camera(glm::vec3 Position, glm::vec3 FrontVector, glm::vec3 UpVector);

    void ProcessMouseMovement(float xoffset, float yoffset);
    void ProcessMouseScroll(float xoffset, float yoffset);
    void ProcessKeyboard(enDIRECTION DIR,  float deltaTime);

    glm::mat4 GetViewMatrix();
    void SetSensitivity(float value);
    float GetSensitivity();
    void SetFOV(float value);
    float GetFOV();
};


#endif