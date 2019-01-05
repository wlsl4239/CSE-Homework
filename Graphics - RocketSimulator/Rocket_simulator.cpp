// Include standard headers
#include <stdio.h>
#include <stdlib.h>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace glm;

#include <common/shader.hpp>
#include <common/texture.hpp>
#include <common/controls.hpp>

//빛의 위치
glm::vec3 lightPos(0.0f, 10.0f, 2.5f);
GLfloat *makeCircleVertexData(GLfloat x, GLfloat y, GLfloat z, GLfloat h, GLfloat radius, GLint numberOfSides);

int main( void )
{
	// Initialise GLFW
	if( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow( 1024, 768, "12131469 - Rocket Simulator", NULL, NULL);
	if( window == NULL ){
		fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}
	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	// sky blue background
	glClearColor(0.6f, 0.8f, 0.9f, 0.0f);



	// depth 테스트 활성화
	glEnable(GL_DEPTH_TEST);
	// 그리려는 fragment가 이전에 그린 fragment보다 앞에있으면 허용
	glDepthFunc(GL_LESS);

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Create and compile our GLSL program from the shaders
	GLuint programID = LoadShaders( "TransformVertexShader.vertexshader", "ColorFragmentShader.fragmentshader" );

	// Get a handle for our "MVP" uniform
	GLuint MatrixID = glGetUniformLocation(programID, "MVP");




	// Our vertices. Tree consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
	// A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices


	static const GLfloat ground[] = {
		100.0f,-1.5f,100.0f, 100.0f,-1.5f,-100.0f, -100.0f,-1.5f,100.0f,
		-100.0f,-1.5f,-100.0f, 100.0f,-1.5f,-100.0f, -100.0f,-1.5f,100.0f
	};

	static const GLfloat rocket_body[] = {
		0.1f,-1.5f,0.1f, 0.1f,-1.5f,-0.1f, -0.1f,-1.5f,0.1f,
		-0.1f,-1.5f,-0.1f, 0.1f,-1.5f,-0.1f, -0.1f,-1.5f,0.1f,

		0.1f,-0.5f,0.1f, 0.1f,-0.5f,-0.1f, -0.1f,-0.5f,0.1f,
		-0.1f,-0.5f,-0.1f, 0.1f,-0.5f,-0.1f, -0.1f,-0.5f,0.1f,

		-0.1f,-1.5f,0.1f, 0.1f,-1.5f,0.1f, -0.1f,-0.5f,0.1f,
		-0.1f,-0.5f,0.1f, 0.1f,-0.5f,0.1f, 0.1f,-1.5f,0.1f,

		0.1f,-0.5f,0.1f, 0.1f,-0.5f,-0.1f, 0.1f,-1.5f,0.1f,
		0.1f,-1.5f,-0.1f, 0.1f,-1.5f,0.1f, 0.1f,-0.5f,-0.1f,

		-0.1f,-0.5f,-0.1f, 0.1f,-0.5f,-0.1f, 0.1f,-1.5f,-0.1f,
		-0.1f,-0.5f,-0.1f, -0.1f,-1.5f,-0.1f, 0.1f,-1.5f,-0.1f,

		-0.1f,-0.5f,0.1f, -0.1f,-0.5f,-0.1f, -0.1f,-1.5f,-0.1f,
		-0.1f,-0.5f,0.1f, -0.1f,-1.5f,0.1f, -0.1f,-1.5f,-0.1f
	};

	static const GLfloat rocket_leftwing[] = {
		-0.1f, -1.5f, 0.02f, -0.2f, -1.5f, 0.0f, -0.1f, -1.1f, 0.0f,
		-0.1f, -1.5f, -0.02f, -0.2f, -1.5f, 0.0f, -0.1f, -1.1f, 0.0f,
		-0.1f, -1.5f, 0.02f, -0.1f, -1.5f, -0.01f, -0.2f, -1.5f, 0.0f
	};

	static const GLfloat rocket_rightwing[] = {
		0.1f, -1.5f, 0.02f, 0.2f, -1.5f, 0.0f, 0.1f, -1.1f, 0.0f,
		0.1f, -1.5f, -0.02f, 0.2f, -1.5f, 0.0f, 0.1f, -1.1f, 0.0f,
		0.1f, -1.5f, 0.02f,0.1f, -1.5f, -0.01f,0.2f, -1.5f, 0.0f
	};

	static const GLfloat color_ground[] = {
		0.9f,0.65f,0.1f,0.9f,0.65f,0.1f,0.9f,0.65f,0.1f,
		0.9f,0.65f,0.1f,0.9f,0.65f,0.1f,0.9f,0.65f,0.1f
	};

	static const GLfloat color_rocket_head[] = {
		1.0f,-0.5f,1.0f
	};

	static const GLfloat color_rocket_body[] = {
		0.95f,0.95f,0.95f,0.95f,0.95f,0.95f,0.95f,0.95f,0.95f,
		0.95f,0.95f,0.95f,0.95f,0.95f,0.95f,0.95f,0.95f,0.95f,
		0.95f,0.95f,0.95f,0.95f,0.95f,0.95f,0.95f,0.95f,0.95f,
		0.95f,0.95f,0.95f,0.95f,0.95f,0.95f,0.95f,0.95f,0.95f,
		0.95f,0.95f,0.95f,0.95f,0.95f,0.95f,0.95f,0.95f,0.95f,
		0.95f,0.95f,0.95f,0.95f,0.95f,0.95f,0.95f,0.95f,0.95f,
		0.95f,0.95f,0.95f,0.95f,0.95f,0.95f,0.95f,0.95f,0.95f,
		0.95f,0.95f,0.95f,0.95f,0.95f,0.95f,0.95f,0.95f,0.95f,
		0.95f,0.95f,0.95f,0.95f,0.95f,0.95f,0.95f,0.95f,0.95f,
		0.95f,0.95f,0.95f,0.95f,0.95f,0.95f,0.95f,0.95f,0.95f,
		0.95f,0.95f,0.95f,0.95f,0.95f,0.95f,0.95f,0.95f,0.95f,
		0.95f,0.95f,0.95f,0.95f,0.95f,0.95f,0.95f,0.95f,0.95f

	};

	static const GLfloat color_rocket_leftwing[] = {
		1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f
	};

	static const GLfloat color_rocket_rightwing[] = {
		1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f
	};

	static const GLfloat parachute[] = {
		0.0f, 1.7f, 0.0f, 0.0f, 1.0f, 0.0f, 2.0f, 1.0f, 0.0f,
		0.0f, 1.7f, 0.0f, 0.0f, 1.0f, 0.0f, sqrt(2), 1.0f, sqrt(2) ,
		0.0f, 1.7f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 2.0f,
		0.0f, 1.7f, 0.0f, 0.0f, 1.0f, 0.0f, -sqrt(2) , 1.0f, sqrt(2) ,
		0.0f, 1.7f, 0.0f, 0.0f, 1.0f, 0.0f, -2.0f, 1.0f, 0.0f,
		0.0f, 1.7f, 0.0f, 0.0f, 1.0f, 0.0f, -sqrt(2) , 1.0f, -sqrt(2) ,
		0.0f, 1.7f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, -2.0f,
		0.0f, 1.7f, 0.0f, 0.0f, 1.0f, 0.0f, sqrt(2) , 1.0f, -sqrt(2) ,

		0.0f, 1.7f, 0.0f,2.0f, 1.0f, 0.0f,sqrt(2) , 1.0f, sqrt(2) ,
		0.0f, 1.7f, 0.0f,sqrt(2) , 1.0f, sqrt(2) ,0.0f, 1.0f, 2.0f,
		0.0f, 1.7f, 0.0f,0.0f, 1.0f, 2.0f,-sqrt(2) , 1.0f, sqrt(2) ,
		0.0f, 1.7f, 0.0f,-sqrt(2) , 1.0f, sqrt(2) ,-2.0f, 1.0f, 0.0f,
		0.0f, 1.7f, 0.0f,-2.0f, 1.0f, 0.0f,-sqrt(2) , 1.0f, -sqrt(2) ,
		0.0f, 1.7f, 0.0f,-sqrt(2) , 1.0f, -sqrt(2) ,0.0f, 1.0f, -2.0f,
		0.0f, 1.7f, 0.0f,0.0f, 1.0f, -2.0f,sqrt(2) , 1.0f, -sqrt(2) ,
		0.0f, 1.7f, 0.0f,sqrt(2) , 1.0f, -sqrt(2) ,2.0f, 1.0f, 0.0f,


		0.0f, -0.5f, 0.0f, 2.01f, 1.0f, 0.0f , 2.0f, 1.0f, 0.0f,
		0.0f, -0.5f, 0.0f, 0.01+sqrt(2) , 1.0f, sqrt(2)  , sqrt(2) , 1.0f, sqrt(2) ,
		0.0f, -0.5f, 0.0f, 0.01f, 1.0f, 2.0f , 0.0f, 1.0f, 2.0f,
		0.0f, -0.5f, 0.0f, -0.01 -sqrt(2) , 1.0f, sqrt(2)  , -sqrt(2) , 1.0f, sqrt(2) ,
		0.0f, -0.5f, 0.0f, -2.01f, 1.0f, 0.0f , -2.0f, 1.0f, 0.0f,
		0.0f, -0.5f, 0.0f, -0.01-sqrt(2) , 1.0f, -sqrt(2)  , -sqrt(2) , 1.0f, -sqrt(2) ,
		0.0f, -0.5f, 0.0f, 0.01f, 1.0f, -2.0f , 0.0f, 1.0f, -2.0f,
		0.0f, -0.5f, 0.0f, 0.01 + sqrt(2) , 1.0f, -sqrt(2)  , sqrt(2) , 1.0f, -sqrt(2) 

	};

	static const GLfloat color_parachute[] = {
		0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f,
		0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f,
		0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f,
		0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f,
		0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f,
		0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f,
		0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f,
		0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f,
		0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f,
		0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f,
		0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f,
		0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f,
		0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f,
		0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f,
		0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f,
		0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f,
		0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f,
		0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f,
		0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f,
		0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f,
		0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f,
		0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f,
		0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f,
		0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f,
		0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f

	};

	static const GLfloat g_color_buffer_data[] = {
		1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f
	};

	GLuint groundbuffer;
	glGenBuffers(1, &groundbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, groundbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(ground), ground, GL_STATIC_DRAW);
	GLuint colorgroundbuffer;
	glGenBuffers(1, &colorgroundbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorgroundbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(color_ground), color_ground, GL_STATIC_DRAW);


	GLuint bodybuffer;
	glGenBuffers(1, &bodybuffer);
	glBindBuffer(GL_ARRAY_BUFFER, bodybuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rocket_body), rocket_body, GL_STATIC_DRAW);
	GLuint colorbodybuffer;
	glGenBuffers(1, &colorbodybuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorbodybuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(color_rocket_body), color_rocket_body, GL_STATIC_DRAW);

	GLuint leftwingbuffer;
	glGenBuffers(1, &leftwingbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, leftwingbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rocket_leftwing), rocket_leftwing, GL_STATIC_DRAW);
	GLuint colorleftwingbuffer;
	glGenBuffers(1, &colorleftwingbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorleftwingbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(color_rocket_leftwing), color_rocket_leftwing, GL_STATIC_DRAW);


	GLuint rightwingbuffer;
	glGenBuffers(1, &rightwingbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, rightwingbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rocket_rightwing), rocket_rightwing, GL_STATIC_DRAW);
	GLuint colorrightwingbuffer;
	glGenBuffers(1, &colorrightwingbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorrightwingbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(color_rocket_rightwing), color_rocket_rightwing, GL_STATIC_DRAW);


	GLfloat *g_circle_buffer_data = makeCircleVertexData(0, -0.5, 0, 0.5, 0.17, 36);

	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, 38 * 3 * sizeof(GLfloat), g_circle_buffer_data, GL_STATIC_DRAW);

	GLuint colorbuffer;
	glGenBuffers(1, &colorbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);


	GLuint parachutebuffer;
	glGenBuffers(1, &parachutebuffer);
	glBindBuffer(GL_ARRAY_BUFFER, parachutebuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(parachute), parachute, GL_STATIC_DRAW);
	GLuint colorparachutebuffer;
	glGenBuffers(1, &colorparachutebuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorparachutebuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(color_parachute), color_parachute, GL_STATIC_DRAW);

	glm::mat4 groundMatrix = glm::mat4(1.0);
	glm::mat4 ModelMatrix = glm::mat4(1.0);

	bool isRising = true;
	bool flag_parachute = false;
	bool flag_start = false;
//	bool isStop = false;
//	bool initial = true;
	do{
		
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		static double lastTime = glfwGetTime();
		double currentTime = glfwGetTime();
		float deltaTime = float(currentTime - lastTime);

		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) 
			flag_parachute = true;

		if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS) 
			flag_start = true;

		glm::vec3 g;

		if (flag_start) {
			if (!flag_parachute)
				g = vec3(0, (10 - (0.980665) * deltaTime)*deltaTime, 0);
			else
				g = vec3(0, (7 - (0.490665) * deltaTime)*deltaTime, 0);

			if ((10 - (0.980665) * deltaTime)*deltaTime <= 0)
				isRising = false;
		}


		// Use our shader
		glUseProgram(programID);
		// Compute the MVP matrix from keyboard and mouse input
		computeMatricesFromInputs();
		glm::mat4 ProjectionMatrix = getProjectionMatrix();
		glm::mat4 ViewMatrix;
	/*	if(initial){
			ViewMatrix = glm::lookAt(glm::vec3(1, 1, 4), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
			
		}
		else*/
			ViewMatrix = getViewMatrix();

		ModelMatrix += translate(mat4(),g);
		glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
		glm::mat4 groundMVP = ProjectionMatrix * ViewMatrix * groundMatrix;



		// Send our transformation to the currently bound shader, 
		// in the "groundMVP" uniform
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &groundMVP[0][0]);


		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, groundbuffer);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		// 2nd attribute buffer : colors
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, colorgroundbuffer);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glDrawArrays(GL_TRIANGLES, 0, 3 * 2);
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, bodybuffer);
		glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,(void*)0);

		// 2nd attribute buffer : colors
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, colorbodybuffer);
		glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,0,(void*)0);

		glDrawArrays(GL_TRIANGLES, 0, 3*12);
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, rightwingbuffer);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		// 2nd attribute buffer : colors
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, colorrightwingbuffer);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glDrawArrays(GL_TRIANGLES, 0, 3*3);
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, leftwingbuffer);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		// 2nd attribute buffer : colors
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, colorleftwingbuffer);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glDrawArrays(GL_TRIANGLES, 0, 3*3);
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		// 2nd attribute buffer : colors
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glDrawArrays(GL_TRIANGLE_FAN, 0, 3 * 38);
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

		if (!isRising && flag_parachute)
		{
			glEnableVertexAttribArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, parachutebuffer);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

			glEnableVertexAttribArray(1);
			glBindBuffer(GL_ARRAY_BUFFER, colorparachutebuffer);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

			glDrawArrays(GL_TRIANGLES, 0, 3 * 72*3);
			glDisableVertexAttribArray(0);
			glDisableVertexAttribArray(1);
		}


		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();


	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0 );

	// Cleanup VBO and shader
	glDeleteBuffers(1, &rightwingbuffer);
	glDeleteBuffers(1, &colorrightwingbuffer);
	glDeleteProgram(programID);
	glDeleteVertexArrays(1, &VertexArrayID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}

GLfloat *makeCircleVertexData(GLfloat x, GLfloat y, GLfloat z, GLfloat h, GLfloat radius, GLint numberOfSides)
{
	GLint numberOfVertices = numberOfSides + 1;

	GLfloat doublePi = 2.0f * 3.141592f;

	GLfloat *circleVerticesX = new GLfloat[numberOfVertices];
	GLfloat *circleVerticesY = new GLfloat[numberOfVertices];
	GLfloat *circleVerticesZ = new GLfloat[numberOfVertices];

	for (int i = 0; i < numberOfVertices; i++)
	{
		circleVerticesX[i] = x + (radius * cos(i * doublePi / numberOfSides));
		circleVerticesY[i] = y;
		circleVerticesZ[i] = z + (radius * sin(i * doublePi / numberOfSides));
	}
	GLfloat *allCircleVertices = new GLfloat[numberOfVertices * 3 + 3];

	allCircleVertices[0] = x;
	allCircleVertices[1] = y + h;
	allCircleVertices[2] = z;

	for (int i = 1; i <= numberOfVertices; i++)
	{
		allCircleVertices[i * 3] = circleVerticesX[i-1];
		allCircleVertices[(i * 3) + 1] = circleVerticesY[i-1];
		allCircleVertices[(i * 3) + 2] = circleVerticesZ[i-1];
	}

	return allCircleVertices;
}