#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "path/camera/camera.hpp"
#include "path/shader/shader.hpp"
#include "path/vertices.hpp"

unsigned int VAO, VBO, texture1, texture2;
#define SCR_WIDTH  1280
#define SCR_HEIGHT 720
glm::vec3 cameraPos(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp(0.0f, 1.0f, 0.0f);
float yaw = -89.0f, pitch = 0.0f, fov = 45.0f;
float lastX = 640.0f, lastY = 360.0f;
static bool firstMouse = true;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));

GLFWwindow * FwWindowInit(void);
void WindowClean(void);
void VerticesInit(void);
void TexCoordInit(std::string path, unsigned int & texture);
void framebuffer_size_callback(GLFWwindow * window, int width, int height);
void processInput(GLFWwindow * window, float deltaTime);
void mouse_callback(GLFWwindow * window, double xpos, double ypos);
void scroll_callback(GLFWwindow * window, double xoffset, double yoffset);
GLenum glCheckError_(const char * File, int Line);

int main()
{
    float lastTime = 0.0f;
    float timeCurrent = 0.0f;
    GLFWwindow * window = FwWindowInit();
    if(NULL == window)
    {
        std::cout << "FwWindow Init Failed" << std::endl;
        return -1;
    }
    glm::mat4 view = glm::mat4(1.0f);  
	Shader ourShader("./path/shader/vertexShader/shader.vs", "./path/shader/fragmentShader/shader.fs");
    /* 初始化頂點 */
    VerticesInit();
    /* 加載紋理數據，以2D，平鋪方式拉伸 */
    TexCoordInit("/home/ubuntu/test/pic/4444.png", texture1);
    /* 啓用着色器 */
    ourShader.use();
    /* 將紋理數據傳遞給着色器管線 */
    ourShader.setInt("texture1");

    while(!glfwWindowShouldClose(window))
    {
        timeCurrent = glfwGetTime();
        processInput(window, timeCurrent - lastTime);
        lastTime = timeCurrent;
        WindowClean();
        /* 開啓深度測試 */
        glEnable(GL_DEPTH_TEST);
        /* 清除顏色緩衝區 */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        /* 綁定紋理 */
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        ourShader.use();
        /* 創建平截頭體 */
        glm::mat4 projection = glm::perspective(glm::radians(camera.GetFOV()), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        /* 傳遞數據給頂點着色器 */
        ourShader.setMat4("projection", projection);
        // float radius = 10.0f;
        // float camX = (sin(glfwGetTime()) * radius);
        // float camY = (cos(glfwGetTime()) * radius);

        // view = glm::lookAt(
        //        glm::vec3(0.0f, 0.0f, 3.0f) //攝像機位置
        //      , glm::vec3(0.0f, 0.0f, 0.0f) //攝像機看向的位置，指向坐標
        //      , glm::vec3(0.0f, 1.0f, 0.0f)); //定義豎直向上的方向
        // view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        view = camera.GetViewMatrix();
        ourShader.setMat4("view", view);
    
        glBindVertexArray(VAO);

        for(int i = 0; i < 10; i++)
        {
            ourShader.use();
            glm::mat4 model = glm::mat4(1.0f);  
            /* 加載10個頂點數據 */
            model = glm::translate(model, cubePositions[i]);
            /* i 迭代次數，作用在單個model中，使每個model都不相同 */
            float angle = 30.0f * (i + 1);
            model = glm::rotate(model,  glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            /* 使用uniform，將外部數據傳遞給着色器 */
            ourShader.setMat4("model", model);
            /* 繪制36個頂點 */
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();    
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}

GLFWwindow * FwWindowInit(void)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow * window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if(window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return window;
    }

    glfwMakeContextCurrent(window);
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initalize GLAD" << std::endl;
        return window;
    }
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    /* 隱藏鼠標 */
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    return window;
}

void WindowClean(void)
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void VerticesInit(void)
{
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), (void *)(3 * sizeof(GL_FLOAT)));
    glEnableVertexAttribArray(1);
}

static void LoadImage(std::string path)
{
    cv::Mat img = cv::imread(path, CV_LOAD_IMAGE_UNCHANGED);
    if(img.empty())
    {
        std::cout << "Failed read image" << std::endl;
        return ;
    }
    cv::cvtColor(img, img, CV_RGBA2BGRA);
    cv::putText(img, "-------->", cv::Point(img.rows/2, img.cols/2), cv::FONT_HERSHEY_COMPLEX, 1.0f, cv::Scalar(255, 0, 255), 5, cv::LINE_AA, false);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img.cols, img.rows, 0, GL_RGBA, GL_UNSIGNED_BYTE, img.data);
    glGenerateMipmap(GL_TEXTURE_2D);
}

void TexCoordInit(std::string path, unsigned int & texture)
{
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    LoadImage(path);
}

void processInput(GLFWwindow * window, float deltaTime)
{
    /* set speed */
    /* 這裏使用OpenGL封裝的庫函數獲取按鍵，內置有消抖，鎖定等方向 */
    float cameraSpeed = 2.5f * deltaTime;
    /* 獲取W按鍵信號，將攝像機向前移動 */
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, cameraSpeed);
        // cameraPos += cameraSpeed * cameraFront;
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, cameraSpeed);
        // cameraPos -= cameraSpeed * cameraFront;
    /* 獲取A按鍵信號，獲得向右單位方向向量，將攝像機向右移動 */
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, cameraSpeed);
        // cameraPos -= cameraSpeed * (glm::normalize(glm::cross(cameraFront, cameraUp)));
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, cameraSpeed);
        // cameraPos += cameraSpeed * (glm::normalize(glm::cross(cameraFront, cameraUp)));

    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}

void framebuffer_size_callback(GLFWwindow * window, int width, int height)
{
    glViewport(128, 64, width, height);
}

void mouse_callback(GLFWwindow * window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);
    if(firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; //屏幕的Y坐標是從上往下依次增大的
    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
    // float sensitivity = 0.0005f; //設置事件的靈敏度
    // xoffset *= sensitivity;
    // yoffset *= sensitivity;

    // /* 將偏移量加到俯仰角和偏航角上 */
    // yaw    -= xoffset;
    // pitch  -= yoffset;
    
    // /* 設定範圍爲正負89 */
    // if(yaw >  89.0f)
    //     yaw = 89.0f;
    // if(yaw < -89.0f)
    //     yaw = -89.0f;
    // /* 設定範圍爲正負89 */
    // if(pitch >  89.0f)
    //     pitch = 89.0f;
    // if(pitch < -89.0f)
    //     pitch = -89.0f;
    
    // std::cout << "yaw: " << yaw << " pitch: " << pitch << std::endl;
    // /* 計算偏航角，獲得方向向量 */
    // glm::vec3 front;
    // /* radians, 將角度轉化爲弧度制 */
    // front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
    // front.y = sin(glm::radians(pitch));
    // front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
    // /* 歸一化，將計算完成的數據傳遞出去 */
    // cameraFront = glm::normalize(front);
}

void scroll_callback(GLFWwindow * window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll((float)xoffset, (float)yoffset);
    // /* 設置視場角 */
    // if(fov >= 1.0f && fov <= 55.0f)
    //     fov -= yoffset;
    // if(fov <= 1.0f)
    //     fov = 1.0f;
    // if(fov >= 55.0f)
    //     fov = 55.0f;
}

GLenum glCheckError_(const char * File, int Line)
{
    GLenum errorCode;
    if((errorCode = glGetError()) != GL_NO_ERROR)
    {
        std::string error;
		switch(errorCode)
		{
			case GL_INVALID_ENUM: 					error = "GL_INVALID_ENUM"; 					break;
			case GL_INVALID_VALUE: 					error = "GL_INVALID_VALUE"; 				break;
			case GL_INVALID_OPERATION: 				error = "GL_INVALID_OPERATION"; 			break;
			case GL_STACK_OVERFLOW: 				error = "GL_STACK_OVERFLOW"; 				break;
			case GL_STACK_UNDERFLOW: 				error = "GL_STACK_UNDERFLOW"; 				break;
			case GL_OUT_OF_MEMORY: 					error = "GL_OUT_OF_MEMORY"; 				break;
			case GL_INVALID_FRAMEBUFFER_OPERATION:	error = "GL_INVALID_FRAMEBUFFER_OPERATION"; break;
			default:																			break;
		}
		std::cout << error << ": [" << File << "]: [" << Line << "]" << std::endl;
	}
	return errorCode;
}