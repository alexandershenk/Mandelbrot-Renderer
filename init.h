#ifndef INIT_H
#define INIT_H
 
 
// Window dimensions
const GLuint WIDTH = 720, HEIGHT = 480;

extern GLFWwindow* window;


// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
//
static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);

int init();

#endif
