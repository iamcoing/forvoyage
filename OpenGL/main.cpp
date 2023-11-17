#include <iostream>
#include <opencv2/opencv.hpp>
#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <math.h>
#include <unistd.h>

#include "path/shader/shader.hpp"

#define VERTEX_AND_COLOR 1
#define UNIFORM_INSERT 0
#define EBO_TEST 0
#define TEXCOORD 0
#define TRANSFROM_SPACE 1

/*创建顶点数组包括颜色和位置*/
#if VERTEX_AND_COLOR
#if (TEXCOORD)
static GLfloat vertices[] = {
	//位置                  //颜色				//纹理
	 0.8f,  0.8f, 0.0f,   1.0f, 0.0f, 0.0f,   2.0f, 2.0f, // top right
	 0.8f, -0.8f, 0.0f,   0.0f, 1.0f, 0.0f,   2.0f, 0.0f, // bottom right
	-0.8f, -0.8f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
	-0.8f,  0.8f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 2.0f  // top left 
};
#else
static GLfloat vertices[] = {
	//位置                  //颜色
	 0.5f, -0.5f, 0.0f,    1.0f, 0.0f, 0.0f, //右下
	-0.5f, -0.5f, 0.0f,    0.0f, 1.0f, 0.0f, //左下
	 0.0f,  0.5f, 0.0f,    0.0f, 0.0f, 1.0f, //顶部
};
#endif
#endif
/*创建一个顶点数组*/
GLfloat vertices_0[] = {
	-1.0f, -1.0f, 0.0f,
	-0.5f,  1.0f, 0.0f,
	 0.0f, -1.0f, 0.0f,
};
GLfloat vertices_1[] = {
	0.0f, -1.0f, 0.0f,
   	0.5f,  1.0f, 0.0f,
	1.0f, -1.0f, 0.0f,
};
GLfloat vertices_2[] = {
	-0.8f, -0.8f, 0.0f,
	-0.8f,  0.8f, 0.0f,
	 0.8f,  0.8f, 0.0f,
	 0.8f, -0.8f, 0.0f,
};

unsigned int index_EBO[]  = {
	0, 1, 3, // first triangle
	1, 2, 3  // second triangle
};

/*定义一个顶点着色器*/
#if (UNIFORM_INSERT)
static const char * vert_shader_test = 
"#version 330 core									\n"
"layout(location = 0) in vec3 Pos;					\n"
"out vec4 vertexColor;								\n"
"void main()										\n"
"{													\n"
// "	gl_Position = vec4(Pos.x, Pos.y, Pos.z, 1.0);	\n"
"	gl_Position = vec4(Pos, 1.0);					\n"
"	vertexColor = vec4(0.5, 0.0, 0.0, 1.0);			\n"
"}													\n";
#elif (VERTEX_AND_COLOR)
static const char * vert_shader_test = 
"#version 330 core									\n"
"layout(location = 0) in vec3 Pos;					\n"
"layout(location = 1) in vec3 Color;				\n"
"out vec3 ourColor;									\n"
"void main()										\n"
"{													\n"
"	gl_Position = vec4(Pos.x, -Pos.y, Pos.z, 1.0);	\n"
"	ourColor	= Color;							\n"
"}													\n";

#endif

/*定义一个片段着色器*/
#if (!UNIFORM_INSERT)
#if VERTEX_AND_COLOR
static const char * frag_shader_test = 
"#version 330 core									\n"
"in vec3 ourColor;									\n"
"out vec4 FragColor;								\n"
"void main()										\n"
"{													\n"
"	FragColor = vec4(ourColor, 1.0f);				\n"
"}													\n";
#else													
static const char * frag_shader_test = 
"#version 330 core									\n"
"out vec4 FragColor;								\n"
//"in vec4 vertexColor;								\n"
//"out vec4 FragColor;								\n"
"void main()										\n"
"{													\n"
// "	FragColor = vertexColor;						\n"
"	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);		\n"
"}													\n";
#endif
#else
static const char * frag_shader_test = 
"#version 330 core									\n"
"out vec4 FragColor;								\n"
"uniform vec4 ourColor;								\n"
"void main()										\n"
"{													\n"
"	FragColor = ourColor;							\n"
"}													\n";

#endif

/*定义一个片段着色器*/
static const char * frag_shader_yellow = 
"#version 330 core									\n"
"out vec4 FragColor;								\n"
"void main()										\n"
"{													\n"
"	FragColor = vec4(0.0f, 0.0f, 1.0f, 1.0f);		\n"
"}													\n";

#define glCheckError() glCheckError_(__FILE__, __LINE__)

unsigned int vertexShader[2], fragmentShader[2], shaderProgram[2];
unsigned int VAO[2], VBO[2], EBO;
cv::Mat img = cv::imread("/home/ubuntu/test/pic/4444.png", CV_LOAD_IMAGE_UNCHANGED);
  

using namespace cv;
using namespace std;

GLFWwindow * FwWindowInit(void);
void VerticesInit(void);
void TexCoordInit(void);
void ShaderInit(void);
void ElementInit(void);
glm::mat4 GetTransform(void);
void processInput(GLFWwindow * window);
void framebuffer_size_callback(GLFWwindow * window, int width, int height);
GLenum glCheckError_(const char * File, int Line);

int main(int argc, const char * argv[])
{
	GLFWwindow * window = FwWindowInit();
	if(NULL == window)
	{
		cout << "FwWindowInit failed" << endl;
		return -1;
	}
	Shader ourShader("./path/shader/vertexShader/shader.vs", "./path/shader/fragmentShader/shader.fs");

	/*初始化shader和VAO，VBO*/
	VerticesInit();
#if EBO_TEST
	ElementInit();
#endif
#if TEXCOORD
	TexCoordInit();
#else
	ShaderInit();
#endif
	// unsigned int transformLoc = glGetUniformLocation(ourShader.ID, "transform");
	// glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(GetTransform()));
	/* 循环打开窗口，直到得到推出条件 */
	/*双缓冲(Double Buffer)
	 *应用程序使用单缓冲绘图时可能会存在图像闪烁的问题。 这是因为生成的图像不是一下子被绘制出来的，而是按照从左到右，由上而下逐像素地绘制而成的。最终图像不是在瞬间显示给用户，而是通过一步一步生成的，这会导致渲染的结果很不真实。为了规避这些问题，我们应用双缓冲渲染窗口应用程序。前缓冲保存着最终输出的图像，它会在屏幕上显示；而所有的的渲染指令都会在后缓冲上绘制。当所有的渲染指令执行完毕后，我们交换(Swap)前缓冲和后缓冲，这样图像就立即呈显出来，之前提到的不真实感就消除了。 */
	while( !glfwWindowShouldClose(window) )
	{
		/* 检测用户是否按下按键 */
		processInput(window);

		/* 清除窗口 */
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		/*画一个三角形*/
#if !TEXCOORD
		glUseProgram(shaderProgram[0]);
#endif
#if UNIFORM_INSERT
		/* 找到着色器中uniform属性的索引，位置值 */
		/* 查询函数提供着色器程序，检测uniform名字 */
		int vertexColorLocation = glGetUniformLocation(shaderProgram[0], "ourColor");
		if( -1 == vertexColorLocation )
		{
			cout << "ERROR::GL get uniform location::FAILED" << endl;
		}
		else
		{
			/* 设置查询到的uniform值 */
			/* glUnifrom*
			4f:4个float分量，i:int分量，ui:unsigned int分量，fv：float类型的向量或数组 */
			glUniform4f(vertexColorLocation, 0.0f, fabsf(sin(glfwGetTime())), 0.0f, 1.0f);
		}
#endif
#if EBO_TEST
		ourShader.use();
	#if TRANSFROM_SPACE
		/* 创建一个单位4X4的矩阵 */
		glm::mat4 trans = glm::mat4(1.0f);
		/* 进行平移 */
        trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
		/* 进行旋转 */
        trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
		ourShader.use();
		/* 获取变量信息，传入变换后的矩阵 */
		unsigned int transformLoc = glGetUniformLocation(ourShader.ID, "transform");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
	#endif
		glBindVertexArray(VAO[0]);
	#if !TEXCOORD
	
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	#endif
		/*使用EBO的情况，声明需要绘制的点数是6个一组*/
		// glm::mat4 trans;
		// trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
		// trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
#endif
#if !TEXCOORD
		glBindVertexArray(VAO[0]);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glUseProgram(shaderProgram[1]);
		glBindVertexArray(VAO[1]);
		glDrawArrays(GL_TRIANGLES, 0, 3);
#endif
		/* 交换颜色缓冲，将输出结果显示在屏幕上 */
		glfwSwapBuffers(window);
		/* 检查有没有事件触发 */
		glfwPollEvents();
	}
	glDeleteBuffers(1, &EBO);
	glDeleteBuffers(1, &VBO[0]);
	glDeleteVertexArrays(1, &VAO[0]);
	for(int i = 0; i < 2; i++)
		glDeleteProgram(shaderProgram[i]);

	/* 关闭窗口 */
	glfwTerminate();
	return 0;
}

GLFWwindow * FwWindowInit(void)
{
	/* 初始化窗口，设置OpenGL版本号,使用核心模式 */
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	/* 创建一个窗口对象，存放了所有和窗口有关的数据 */
	GLFWwindow * window = glfwCreateWindow(960, 640, "LearnOpenGL", NULL, NULL);
	if(window == NULL){
		cout << "Failed to create GLFW window" << endl;
		glfwTerminate();
		return window;
	}
	glfwMakeContextCurrent(window);
	/* 初始化GLAD以管理OpenGL的函数指针 */
	if( !gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) ){
		cout << "Failed to initialize GLAD" << endl;
		return window;
	}

	/* 设置窗口的回调函数 */
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	return window;
}

/*创建一个顶点缓冲对象*/
void VerticesInit(void)
{
	/*生成一个VAO对象，并分配一个ID*/
	glGenVertexArrays(1, &VAO[0]);
	glBindVertexArray(VAO[0]);
	/*生成一个VBO对象，并分配一个缓冲ID*/
	glGenBuffers(1, &VBO[0]);
	/*把新创建的缓冲绑定到GL_ARRAY_BUFFER上，任何调用GL_ARRAY_BUFFER都会用来配置当前绑定的缓冲*/
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
#if (VERTEX_AND_COLOR)
#if (!TEXCOORD)
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void * )0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void * )(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
#else
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void * )0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void * )(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void * )(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
#endif
#else
	/*将之前定义的顶点数据复制到缓冲的内存中*/
	/**
		GL_STATIC_DRAW ：数据不会或几乎不会改变。
		GL_DYNAMIC_DRAW：数据会被改变很多。
		GL_STREAM_DRAW ：数据每次绘制时都会改变。
	*/
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_0), vertices_0, GL_STATIC_DRAW);
	/*设置顶点属性指针*/
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void *)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(VAO[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_1), vertices_1, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void *)0);
	glEnableVertexAttribArray(0);
#endif
}

void TexCoordInit(void)
{
    /* 创建纹理 */
    unsigned int texture;
    /* 生成一个纹理句柄 */
    glGenTextures(1, &texture);
    /* 绑定在2D纹理中 */
    glBindTexture(GL_TEXTURE_2D, texture);
    /* 对单独的坐标轴设置纹理
    GL_TEXTURE_2D 指定了纹理目标
    GL_TEXTURE_WRAP_S 配置的是WRAP选项，并指定S和T轴
    GL_CLAMP_TO_BORDER 设置纹理环绕方式，自动拉伸至边缘 */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // /* 传递一个float数组作为边缘的颜色值 */
    // float borderColor[] = {1.0f, 1.0f, 0.0, 1.0f};
    // /* 设置纹理填充方式，边缘填充，使用颜色 */
    // glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    /* 设置纹理过滤，邻近过滤或者线性过滤 */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    /* 设置为多级渐远纹理过滤，主要使用在纹理被缩小的位情况下，无法对放大纹理设置多级渐远 */
    /**
    GL_NEAREST_MIPMAP_NEAREST 	使用最邻近的多级渐远纹理来匹配像素大小，并使用邻近插值进行纹理采样
    GL_LINEAR_MIPMAP_NEAREST 	使用最邻近的多级渐远纹理级别，并使用线性插值进行采样
    GL_NEAREST_MIPMAP_LINEAR 	在两个最匹配像素大小的多级渐远纹理之间进行线性插值，使用邻近插值进行采样
    GL_LINEAR_MIPMAP_LINEAR 	在两个邻近的多级渐远纹理之间使用线性插值，并使用线性插值进行采样 
    */
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	cv::cvtColor(img, img, CV_RGBA2RGB);
    // std::cout << img.channels() << std::endl;
    if (!img.empty())
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img.cols, img.rows, 0, GL_RGB, 
		GL_UNSIGNED_BYTE, img.data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
}

void ElementInit(void)
{
#if !EBO_TEST
	return;
#endif
	/*生成一个元素缓冲*/
	/*EBO会调用VBO中的数据，他和VAO相关联，一个VAO中包含一个EBO*/
	glGenBuffers(1, &EBO);
	/*将索引数据复制到缓冲里*/
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index_EBO), index_EBO, GL_STATIC_DRAW);
}

void ShaderInit(void)
{
	int success;
	char infoLog[512];
	/*创建一个着色器对象并储存*/
	vertexShader[0] = glCreateShader(GL_VERTEX_SHADER);
	/*编译着色器*/
	glShaderSource(vertexShader[0], 1, &vert_shader_test, NULL);
	glCompileShader(vertexShader[0]);
	/*检测编译着色器是否成功，失败则返回错误信息*/
	glGetShaderiv(vertexShader[0], GL_COMPILE_STATUS, &success);
	if(!success)
	{
		glGetShaderInfoLog(vertexShader[0], 512, NULL, infoLog);
		cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED: " << infoLog << endl;
	}
	fragmentShader[0] = glCreateShader(GL_FRAGMENT_SHADER);
	/* 绑定片段着色器 */
	glShaderSource(fragmentShader[0], 1, &frag_shader_test, NULL);
	glCompileShader(fragmentShader[0]);
	glGetShaderiv(fragmentShader[0], GL_COMPILE_STATUS, &success);
	if(!success)
	{
		glGetShaderInfoLog(fragmentShader[0], 512, NULL, infoLog);
		cout << "ERROR::SHADER::FRAGMENT0::COMPILATION_FAILED: " << infoLog << endl;
	}
	fragmentShader[1] = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader[1], 1, &frag_shader_yellow, NULL);
	glCompileShader(fragmentShader[1]);
	glGetShaderiv(fragmentShader[1], GL_COMPILE_STATUS, &success);
	if(!success)
	{
		glGetShaderInfoLog(fragmentShader[1], 512, NULL, infoLog);
		cout << "ERROR::SHADER::FRAGMENT1::COMPILATION_FAILED: " << infoLog << endl;
	}

	/*链接着色器*/
	/*创建一个程序对象*/
	shaderProgram[0] = glCreateProgram();
	/*将之前编译的着色器附加到程序对象上*/
	glAttachShader(shaderProgram[0], vertexShader[0]);
	glAttachShader(shaderProgram[0], fragmentShader[0]);
	glLinkProgram(shaderProgram[0]);
	glGetShaderiv(shaderProgram[0], GL_COMPILE_STATUS, &success);
	if(!success)
	{
		glGetShaderInfoLog(shaderProgram[0], 512, NULL, infoLog);
		cout << "ERROR::SHADER::shaderProgram0::COMPILATION_FAILED: " << infoLog << endl;
	}
	shaderProgram[1] = glCreateProgram();
	glAttachShader(shaderProgram[1], vertexShader[0]);
	glAttachShader(shaderProgram[1], fragmentShader[1]);
	glLinkProgram(shaderProgram[1]);
	/* 查询链接状态 */
	glGetShaderiv(shaderProgram[1], GL_COMPILE_STATUS, &success);
	if(!success)
	{
		glGetShaderInfoLog(shaderProgram[1], 512, NULL, infoLog);
		cout << "ERROR::SHADER::shaderProgram1::COMPILATION_FAILED: " << infoLog << endl;
	}

	/*调用glUseProgram，创建程序对象*/
	// glUseProgram(shaderProgram);
	/*删除着色器对象*/
	glDeleteShader(vertexShader[0]);
	glDeleteShader(fragmentShader[0]);
}

/* 获取变换矩阵 */
glm::mat4 GetTransform(void)
{
	/* 将一个向量位移（1, 1, 0）个单位，齐次坐标设定为1.0 */
	glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
	glm::mat4 trans;

	/* 先进行位移 */
	trans = glm::translate(trans, glm::vec3(1.0f, 1.0f, 0.0f));
	vec = trans * vec;

	/* 进行旋转 */
	trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
	/* 进行缩放 */
	trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));

	return trans;
}

/* 设置按键按下的回调函数 */
void processInput(GLFWwindow * window)
{
	/* 获取按键值和状态 */
	if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
		/* 关闭窗口 */
		glfwSetWindowShouldClose(window, true);
	}
}

void framebuffer_size_callback(GLFWwindow * window, int width, int height)
{
	/* 设置窗口的维度 */
	glViewport(0, 0, width, height);	
}	

/*纠错函数*/
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
		std::cout << error << ": [" << File << "]: [" << Line << "]" << endl;
	}
	return errorCode;
}
