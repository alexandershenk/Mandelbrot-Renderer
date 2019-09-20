#include <iostream>
#include <math.h>

#include <glad/glad.h>



// GLFW
#include <GLFW/glfw3.h>
#include "init.h"


// Function prototypes
float zoom = 1.0f;
float xpos, ypos;
float offsetX=0;
float offsetY=0;
int direction;


// Shaders
const GLchar* vertexShaderSource = "#version 329 core\n"
    "layout (location = 0) in vec3 position;\n"
	"precision highp float;\n"
	"out vec2 coord;\n"
    "void main()\n"
    "{\n"
    "gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
    "coord = position.xy;\n"
    "}\0";
const GLchar* fragmentShaderSource = "#version 330 core\n"
	"precision highp float;\n"
	"in vec2 coord;\n"
    "out vec4 color;\n"
    "uniform vec2 windowSize;\n"
    "uniform vec2 mouseCoords;\n"
    "uniform vec2 offset;\n"
	"uniform float scale;\n"
	"void main(){\n"
		"vec2 c = vec2((((gl_FragCoord.x/(windowSize.x/3))-2)/scale)+offset.x,(((gl_FragCoord.y/(480/2))-1 )/scale)+offset.y);\n"
		"//vec2 c = vec2((coord.x*3)-2.0,(coord.y*3)-1.5);\n"
		"color = vec4(0.0f, 0.0f, 0.0f, 1.0f);\n"
		"vec2 z = vec2(0.0f,0.0f);\n"
		"float i;\n"
		"for(i = 0.0f; i <100.0f; i++){\n"
			"float zx = z.x;\n"
			"z.x = (z.x*z.x)-((z.y*z.y)) +c.x;\n"
			"z.y = 2*(z.y*zx) +c.y;\n"
			"if(dot(z.x*z.x,z.y*z.y) > 4){\n"
				"float whole  = i/100.0f;\n"
				"color = vec4(whole,0,0,0.333);\n"
				"color = vec4(0,whole,0,0.333);\n"
				"break;\n"
			"}\n"
	"}\n"
    "}\0";
	//
// The MAIN function, from here we start the application and run the game loop
int main()
{



    // Set the required callback functions
	//

	init();
	 glEnable (GL_BLEND); glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 

    glfwSetKeyCallback(window, key_callback);

	//glfwSetCursorPosCallback(window, cursor_position_callback);
    // Build and compile our shader program
	//
    // Vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // Check for compile time errors
	//
    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // Fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // Check for compile time errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // Link shaders
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);


	int uniform_WindowSize = glGetUniformLocation(shaderProgram,"windowSize");
	int uniform_zoomScale = glGetUniformLocation(shaderProgram,"scale");
	//int uniform_mouseCoords = glGetUniformLocation(shaderProgram,"mouseCoords");
	int uniform_offset = glGetUniformLocation(shaderProgram,"offset");
    // Check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);


    // Set up vertex data (and buffer(s)) and attribute pointers
    //GLfloat vertices[] = {
    //  // First triangle
    //   0.5f,  0.5f,  // Top Right
    //   0.5f, -0.5f,  // Bottom Right
    //  -0.5f,  0.5f,  // Top Left 
    //  // Second triangle
    //   0.5f, -0.5f,  // Bottom Right
    //  -0.5f, -0.5f,  // Bottom Left
    //  -0.5f,  0.5f   // Top Left
    //}; 
    GLfloat vertices[] = {
         1.0f,  1.0f, 0.0f,  // Top Right
         1.0f, -1.0f, 0.0f,  // Bottom Right
        -1.0f, -1.0f, 0.0f,  // Bottom Left
        -1.0f,  1.0f, 0.0f   // Top Left 
    };
    GLuint indices[] = {  // Note that we start from 0!
        0, 1, 3,  // First Triangle
        1, 2, 3   // Second Triangle
    };
    GLuint VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind

    glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs), remember: do NOT unbind the EBO, keep it bound to this VAO


    // Uncommenting this call will result in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // Game loop
    while (!glfwWindowShouldClose(window))
    {
        // Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
        glfwPollEvents();

        // Render
        // Clear the colorbuffer
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Draw our first triangle
        glUseProgram(shaderProgram);
		glUniform2f(uniform_WindowSize, (float) WIDTH, (float) HEIGHT);
		glUniform1f(uniform_zoomScale , zoom);

		//glUniform2f(uniform_mouseCoords, (float) xpos, (float) ypos);
		glUniform2f(uniform_offset, offsetX, offsetY);


        glBindVertexArray(VAO);
        //glDrawArrays(GL_TRIANGLES, 0, 6);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // Swap the screen buffers
        glfwSwapBuffers(window);
    }
    // Properly de-allocate all resources once they've outlived their purpose
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    // Terminate GLFW, clearing any resources allocated by GLFW.
    glfwTerminate();
    return 0;
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    std::cout << key << std::endl;
	if(key ==87){
		std::cout << "zoom in" << std::endl;

		zoom = zoom/.75;
	}
	else if(key ==83){
		std::cout << "zoom out" << std::endl;

		zoom *= .75;
	}

	 if(key ==265){
		std::cout << "up" << std::endl;
		offsetY+=.05/zoom;

	}

	 if(key ==264){
		std::cout << "down" << std::endl;
		offsetY-=.05/zoom;
	}

	 if(key ==263){
		std::cout << "left" << std::endl;
		offsetX-=.05/zoom;
	}

	 if(key ==262){
		std::cout << "right" << std::endl;

		offsetX+=.05/zoom;
	}
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}
static void cursor_position_callback(GLFWwindow* window, double xPos, double yPos)
{
	xpos = xPos;

	ypos = yPos;
	std::cout << xpos << ypos << std::endl;
}
