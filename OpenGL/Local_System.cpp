#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "path/shader/shader.hpp"
#include "path/vertices.hpp"

unsigned int VAO, VBO, texture1, texture2;
const unsigned int SCR_WIDTH  = 1280;
const unsigned int SCR_HEIGHT = 720;

GLFWwindow * FwWindowInit(void);
void WindowClean(void);
void VerticesInit(void);
void TexCoordInit(std::string path, unsigned int & texture);
void framebuffer_size_callback(GLFWwindow * window, int width, int height);
void processInput(GLFWwindow * window);
GLenum glCheckError_(const char * File, int Line);

int main()
{
    GLFWwindow * window = FwWindowInit();
    if(NULL == window)
    {
        std::cout << "FwWindow Init Failed" << std::endl;
        return -1;
    }
	Shader ourShader("./path/shader/vertexShader/shader.vs", "./path/shader/fragmentShader/shader.fs");
    
    VerticesInit();
    TexCoordInit("/home/ubuntu/test/pic/4444.png", texture1);

    while(!glfwWindowShouldClose(window))
    {
        processInput(window);
        WindowClean();
        glEnable(GL_DEPTH_TEST);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);

        ourShader.use();
        /* 设置摄像机位置 */
        glm::vec3 cameraPos         = glm::vec3(0.0f, 0.0f, 3.0f);
        /* 摄像机方向，交换相减的顺序，获得一个指向摄像机正Z轴方向的向量 */
        glm::vec3 cameraTarget      = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 cameraDirection   = glm::normalize(cameraPos - cameraTarget);
        /* 设置一个右向量，将Y轴和Z轴叉乘，获得一个垂直于两个向量的向量，即摄像机空间中的X轴正方向 */
        glm::vec3 up                = glm::vec3(0.0f, 1.0f, 0.0f);
        glm::vec3 cameraRight       = glm::normalize(glm::cross(up, cameraDirection));
        /* 将X轴和Z轴叉乘，得到一个正Y轴向量 */
        glm::vec3 cameraUp          = glm::cross(cameraDirection, cameraRight);

        // /* 创建一个LookAt矩阵 */
        // float radius = 10.0f;
        // float camX = sin(glfwGetTime()) * radius;
        // float camZ = cos(glfwGetTime()) * radius;
        // glm::mat4 view;
        // view = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f), 
        //     glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));    

        glm::mat4 view          = glm::mat4(1.0f);
        glm::mat4 model         = glm::mat4(1.0f);
        glm::mat4 projection    = glm::mat4(1.0f);
        /**
         * 参数1 目标矩阵
         * 参数2 平移的位置矩阵
         */
        view  = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
        /**
         * 参数1 目标矩阵
         * 参数2 旋转的度数，迭代
         * 参数3 旋转轴
         */
        model = glm::rotate(model, fabsf(sin(glfwGetTime())), glm::vec3(1.0f, 0.0f, 0.0f));
        /**
         * 参数1定义了fov, 表示视野，并且设置了观察空间的大小
         * 参数2设置宽高比，由视口的宽/高所得
         * 参数3 4设置平截头体的近平面和远平面
         */
        projection = glm::perspective(glm::radians(65.0f), 
            (float)SCR_WIDTH/(float)(SCR_HEIGHT), 0.1f, 100.0f);
        unsigned int viewLoc  = glGetUniformLocation(ourShader.ID, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("model", model);
        glBindVertexArray(VAO);
        
        for(int i = 0; i < 10; i++)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * (i + 1);
            /**
             * 参数1 目标矩阵D
             * 参数2 旋转的度数，迭代
             * 参数3 旋转轴
             */
            if(i % 3 == 0)
                model = glm::rotate(model, fabsf(sin(glfwGetTime())), glm::vec3(1.0f, 0.3f, 0.5f));
            ourShader.setMat4("model", model);

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
    /* 圖像色彩轉換 */
    cv::cvtColor(img, img, CV_RGBA2BGRA);
    /* 在圖像上添加字符 */
    cv::putText(img, "-------->", cv::Point(img.rows/2, img.cols/2), cv::FONT_HERSHEY_COMPLEX, 1.0f, cv::Scalar(255, 255, 255), 5, cv::LINE_AA, false);
    /* 綁定圖片紋理單元 */
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

void processInput(GLFWwindow * window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}

void framebuffer_size_callback(GLFWwindow * window, int width, int height)
{
    glViewport(128, 64, width, height);
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