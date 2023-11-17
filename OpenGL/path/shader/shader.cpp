#include "shader.hpp"

Shader::Shader(const char*vertexPath, const char * fragmentPath)
{
    /* 1.从文件路径中获取顶点/片段着色器 */
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    /* 保证ifstream对象可以抛出异常 */
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        /* 打开文件 */
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;
        /* 读取文件的缓冲内容到数据流中 */
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        /* 关闭文件处理器 */
        vShaderFile.close();
        fShaderFile.close();
        /* 转换数据流到string */
        vertexCode   = vShaderStream.str();
        fragmentCode = fShaderStream.str();

    }
    catch(const std::ifstream::failure e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
    }
    const char * vShaderCode = vertexCode.c_str();
    const char * fShaderCode = fragmentCode.c_str();

    /* 2.编译着色器 */
    unsigned int vertex, fragment;
    int success;
    char infoLog[512];

    /* 顶点着色器 */
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    /* 片段着色器 */
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    glGetProgramiv(ID,  GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(ID, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    /* 删除着色器 */
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Shader::use()
{
    glUseProgram(this->ID);
}

void Shader::setBool(const std::string &name, bool value) const
{
    glUniform1i(glGetUniformLocation(this->ID, name.c_str()), (int)value);
}

void Shader::setFloat(const std::string &name, cv::Point3f value) const
{
    glUniform3f(glGetUniformLocation(this->ID, name.c_str()), value.x, value.y, value.z);
}

void Shader::setInt(const std::string &name, cv::Point3i value) const
{
    /**
     * 参数1 着色器属性位置
     * 参数2 3 4 需要修改的数值
     */
    glUniform3i(glGetUniformLocation(this->ID, name.c_str()), value.x, value.y, value.z);
}

void Shader::setMat4(const std::string &name, glm::mat4 & value) const
{
    /**
     * 参数1 着色器属性位置
     * 参数2 传入矩阵个数
     * 参数3 是否将矩阵转秩
     * 参数4 将矩阵转化成数组
     */
    glUniformMatrix4fv(glGetUniformLocation(this->ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

unsigned int Shader::GetShaderID()
{
    return this->ID;
}