#ifndef __SHADER_H__
#define __SHADER_H__

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

class Shader
{
public:
    /* 程序ID */
    unsigned int ID;
    /* 构造函数，读取并创建着色器 */
    Shader(const char * vertexPath,  const char * fragmentPath);
    /* 使用/激活程序 */
    void use();
    /* uniform工具函数 */
    void setBool(const std::string &name, bool value) const;
    void setFloat(const std::string &name, cv::Point3f value = cv::Point3f(0.0, 0.0, 0.0)) const;
    void setInt(const std::string &name, cv::Point3i value = cv::Point3i(0, 0, 0)) const;
    void setMat4(const std::string &name, glm::mat4 &value) const;

    /* 着色器功能函数 */
    unsigned int GetShaderID(void);
};

#endif