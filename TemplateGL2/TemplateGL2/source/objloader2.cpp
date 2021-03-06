#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <windows.h>

#include <learnopengl/shader_m.h>
#include <learnopengl/camera.h>
#include <learnopengl/model.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <cstdlib>
#include <vector>
#include <cstdio>

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader_utils.h"

using namespace std;

Shader* shader;
Shader *shader1;
Model* model1;
Model* model;
// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;

Camera camera(glm::vec3(10.0f, 10.0f, 10.0f));

bool firstMouse = true;

void mouse_callback(int button, int state, int x, int y) {
	if (state == GLUT_DOWN) {
		if (firstMouse) {
			lastX = x;
			lastY = y;
			//firstMouse = false;
		}


		//glutPostRedisplay();
	}
}

void motion_callback(int x, int y) {
	float xoffset = x - lastX;
	float yoffset = lastY - y;

	lastX = x;
	lastY = y;
	camera.ProcessMouseMovement(xoffset, yoffset);
	glutPostRedisplay();
}

void init_resources() {
	shader = new Shader("1.model_loading.vs", "1.model_loading.fs");
	model = new Model("cs_assault.obj");
	shader1 = new Shader("1.model_loading.vs", "1.model_loading.fs");
	model1 = new Model("deagle.obj");

}

//glm::vec3 camPosition(5.0f, 0.0f, 0.0f);
glm::vec3 camPosition(0.0f, 3.45f, 0.0f);
//glm::vec3 pistolPosition(5.0f, -10.0f, -48.0f);
glm::vec3 pistolPosition(0.0f, 3.45f, 0.0f);
GLfloat pitch = 0.0f;
GLfloat yaw = -90.0f;
glm::vec3 camUp(0.0f, 1.0f, 0.0f);
glm::vec3 camRight;
glm::vec3 front;

void onDisplay() {
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// don't forget to enable shader before setting uniforms
	shader->use();
	shader1->use();

	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw))* cos(glm::radians(pitch));
	front = glm::normalize(front);

	camRight = glm::normalize(glm::cross(front, glm::vec3(0.0f, 1.0f, 0.0f)));
	camUp = glm::normalize(glm::cross(camRight, front));
	// view/projection transformations
	glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 200.0f);
	//glm::mat4 view = camera.GetViewMatrix();C:\Users\Joe\Documents\20182\Gr�ficos\ta\avanceToAru\TemplateGL2\TemplateGL2\Debug
	glm::mat4 view = glm::lookAt(camPosition, camPosition + front, camUp);
	shader->setMat4("projection", projection);
	shader->setMat4("view", view);
	shader1->setMat4("projection", projection);
	shader1->setMat4("view", view);

	//render the loaded model
	glm::mat4 mod = glm::scale(glm::mat4(1.0f), glm::vec3(0.002f, 0.002f, 0.002f)) *
		glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
		//* glm::rotate(glm::mat4(1.0f), 3.14f, glm::vec3(1.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
	;	// it's a bit too big for our scene, so scale it down
	shader->setMat4("model", mod);
	model->Draw(*shader);

 //glm::vec3(0.0f, -10.0f, 35.0f)

	glm::mat4 mod1 = glm::translate(glm::mat4(1.0f), glm::vec3(camPosition.x + 0.05f, camPosition.y - 0.05f, camPosition.z))
		*glm::rotate(glm::mat4(1.0f), glm::radians(-180.0f), glm::vec3(0.0f,0.0f,1.0f))
		* glm::scale(glm::mat4(1.0f), glm::vec3(0.015f, 0.015f, 0.015f));	// it's a bit too big for our scene, so scale it down
	shader1->setMat4("model", mod1);
	model1->Draw(*shader1);

	glutSwapBuffers();
}

float rotacion = 3.5f;
float avance = 0.03f;

void keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 'w':
		camPosition += front * avance;
		pistolPosition += front * avance;
		break;
	case 's':
		camPosition -= front * avance;
		pistolPosition -= front * avance;
		break;
	case 'r':
		camPosition += front * avance*2.0f;
		pistolPosition += front * avance*2.0f;
		break;
	case 'd':
		yaw = (yaw >= 180.0f) ? -180.0f + rotacion : yaw + rotacion;
		break;
	case 'a':
		yaw = (yaw <= -180.0f) ? 180.0f - rotacion : yaw - rotacion;
		break;
	default: break;
	}

	glutPostRedisplay();
}

/*void mouse(int state,int x, int y, int z) {
	if (y > 400) yaw = (yaw <= -180.0f) ? 180.0f - rotacion : yaw - rotacion;
	glutPostRedisplay();
}*/

void mouse(int button, int state, int x, int y)
{
		if (button == GLUT_RIGHT_BUTTON)
		{
				if (state == GLUT_DOWN)
					glScalef(2.0, 2.0, 2.0);
				else
					cout << "right button lifted at (" << x << "," << y << ")" << endl;
		}
		else if (button == GLUT_LEFT_BUTTON)
		{
				if (state == GLUT_DOWN)
					cout << "Disparo realizado" << endl;
		}
}


int main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitContextVersion(2, 0);
	glutInitDisplayMode(GLUT_RGBA | GLUT_ALPHA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(SCR_WIDTH, SCR_HEIGHT);
	glutCreateWindow("Nuevo OpenGL");

	GLenum glew_status = glewInit();
	if (glew_status != GLEW_OK) {
		cout << "Error inicializando GLEW!" << endl;
		exit(EXIT_FAILURE);
	}

	if (!GLEW_VERSION_2_0) {
		cout << "Su tajeta gr�fica no soporta OpenGL 2.0" << endl;
		exit(EXIT_FAILURE);
	}

	init_resources();

	glutDisplayFunc(onDisplay);
	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glutMouseFunc(mouse_callback);
	glutMotionFunc(motion_callback);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	//glutMouseWheelFunc(mouse);
	glutMainLoop();


}