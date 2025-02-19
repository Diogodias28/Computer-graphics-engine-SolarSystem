#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define _USE_MATH_DEFINES
#include <math.h>

float r = 5.0f, alpha_cam = M_PI / 4, beta_cam = M_PI / 4;

GLenum drawmode = GL_LINE; // Modo de Desenho

void changeSize(int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window with zero width).
	if(h == 0)
		h = 1;

	// compute window's aspect ratio 
	float ratio = w * 1.0 / h;

	// Set the projection matrix as current
	glMatrixMode(GL_PROJECTION);
	// Load Identity Matrix
	glLoadIdentity();
	
	// Set the viewport to be the entire window
    glViewport(0, 0, w, h);

	// Set perspective
	gluPerspective(45.0f ,ratio, 1.0f ,1000.0f);

	// return to the model view matrix mode
	glMatrixMode(GL_MODELVIEW);
}


void drawFigures(float radius, float height, int slices) {

//colocar código para desenhar todas as figuras que temos

}


void renderScene(void) {

	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set the camera
	glLoadIdentity();
	// primeiro triplo: onde a camera esta
	// segundo triplo: para onde a camera esta a olhar
	// (r, alpha, beta) -> (x, y, z)
	gluLookAt(r * cos(beta_cam) * sin(alpha_cam), r * sin(beta_cam), r * cos(beta_cam) * cos(alpha_cam),
		      0.0,0.0,0.0,
			  0.0f,1.0f,0.0f); 
              // trocar segundo e terceiro triplo de coordenadas depois (?)

    glBegin(GL_LINES);
        // X axis in red
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex3f(-100.0f, 0.0f, 0.0f);
        glVertex3f( 100.0f, 0.0f, 0.0f);
        // Y Axis in Green
        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex3f(0.0f,-100.0f, 0.0f);
        glVertex3f(0.0f, 100.0f, 0.0f);
        // Z Axis in Blue
        glColor3f(0.0f, 0.0f, 1.0f);
        glVertex3f(0.0f, 0.0f,-100.0f);
        glVertex3f(0.0f, 0.0f, 100.0f);
    glEnd();
          
	glPolygonMode(GL_FRONT_AND_BACK, drawmode);
	drawFigures(1,2,10); //trocar o argumento pra alguma coisa, colocar o glbegin(GL_TRIANGLE aqui ou no drawFigures?)

	// End of frame
	glutSwapBuffers();

}


void processKeys(unsigned char c, int xx, int yy) {

// put code to process regular keys in here
	switch (c) {
		case 'w':
			if (beta_cam + 0.2f < (M_PI / 2)) beta_cam += 0.5f;
			break;
		case 's':
			if (beta_cam - 0.2f > (-M_PI / 2)) beta_cam -= 0.5;
			break;
		case 'd':
			alpha_cam -= 0.2f;
			break;
		case 'a':
			alpha_cam += 0.2f;
			break;
		case '+': // Zoom In
			r -= 0.2f;
			break;
		case '-': // Zoom out
			r += 0.2f;
			break;
		case 'm': // Altera os modos de desenho
			if (drawmode == GL_FILL) drawmode = GL_LINE;
			else if (drawmode == GL_LINE) drawmode = GL_POINT;
			else drawmode = GL_FILL;
			break;
	}
	glutPostRedisplay();
}


void processSpecialKeys(int key, int xx, int yy) {

// put code to process special keys in here

}


int main(int argc, char **argv) {

// init GLUT and the window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(800,800);
	glutCreateWindow("CG@DI-UM");
		
// Required callback registry 
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	
// Callback registration for keyboard processing
	glutKeyboardFunc(processKeys);
	glutSpecialFunc(processSpecialKeys);

//  OpenGL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	
// enter GLUT's main cycle
	glutMainLoop();
	
	return 1;
}
