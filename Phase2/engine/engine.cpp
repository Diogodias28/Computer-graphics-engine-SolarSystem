#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define _USE_MATH_DEFINES
#include <math.h>
#include <vector>
#include "../data_structs/Settings.hpp"

// Variáveis da câmara
int height, width;
float alphaCam, betaCam, radius;
float camx, camy, camz;
float lookAtx, lookAty, lookAtz;
float upx, upy, upz;
float fov, nearPlane, farPlane;
float camX, camY, camZ;

// fps
int timebase = 0;
int frame = 0;
float fps = 0.0f;

Settings set;
Group group;

GLenum drawmode = GL_LINE; // Modo de Desenho
bool showAxes = true;

void updateWindowTitle() {
    char coords[100];
    sprintf(coords, "Camera Position: x=%.2f, y=%.2f, z=%.2f | FPS: %.2f", camX, camY, camZ, fps);

    // Atualiza o título da janela
    glutSetWindowTitle(coords);
}

void updateCamera(){

    camX = radius*cosf(betaCam)*sinf(alphaCam);
    camY = radius*sinf(betaCam);
    camZ = radius*cosf(betaCam)*cosf(alphaCam);

}

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
    gluPerspective(fov, ratio, nearPlane, farPlane);

    // return to the model view matrix mode
    glMatrixMode(GL_MODELVIEW);
}

void updateFPS() {
    int time = glutGet(GLUT_ELAPSED_TIME);
    frame++;

    if (time - timebase > 100) { // Atualiza FPS a cada segundo
        fps = frame * 1000.0f / (time - timebase);
        timebase = time;
        frame = 0;
    }

    updateWindowTitle();

    glutPostRedisplay();
}

void drawFigures(Group g) {

    std::vector<Transform> transformations = getTransformations(g);
    std::vector<Model> models = getModels(g);
    std::vector<Group> subgroups = getSubgroup(g);

    glPushMatrix();

    for (int i = 0; i < transformations.size(); i++) {
        char type = getType(transformations[i]);
        Point tv = getTransVal(transformations[i]);

        if(type == 'R'){
            float angle = getAngle(transformations[i]);
            glRotatef(angle, getX(tv), getY(tv), getZ(tv));
        } else if (type == 'T'){
            glTranslatef(getX(tv), getY(tv), getZ(tv));
        } else if (type == 'S'){
            glScalef(getX(tv), getY(tv), getZ(tv));
        }
    }   

    glBegin(GL_TRIANGLES);
    
    for (int i = 0; i < models.size(); i++) {
        std::vector<Point> m_points = getPoints(models[i]);
        float r,g,b;
        getColor(models[i], r, g, b);

        glColor3f(r,g,b);

        for (int j = 0; j<m_points.size(); j++){
            glVertex3f(getX(m_points[j]),getY(m_points[j]), getZ(m_points[j]));
        }
    }

    glEnd();


    for(int i = 0; i < subgroups.size(); i++){
        drawFigures(subgroups[i]);
    }
    glPopMatrix();
}

void renderScene(void) {
    // clear buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // set the camera
    glLoadIdentity();
    // primeiro triplo: onde a camera esta
    // segundo triplo: para onde a camera esta a olhar
    // (r, alphaCam, betaCam) -> (x, y, z)
	gluLookAt(camX, camY, camZ,
		      lookAtx, lookAty, lookAtz,
			  upx,upy,upz); 

    if (showAxes) {
        glBegin(GL_LINES);
        // Eixo X em vermelho
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex3f(-100.0f, 0.0f, 0.0f);
        glVertex3f(100.0f, 0.0f, 0.0f);

        // Eixo Y em verde
        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex3f(0.0f, -100.0f, 0.0f);
        glVertex3f(0.0f, 100.0f, 0.0f);

        // Eixo Z em azul
        glColor3f(0.0f, 0.0f, 1.0f);
        glVertex3f(0.0f, 0.0f, -100.0f);
        glVertex3f(0.0f, 0.0f, 100.0f);
        glEnd();
    }
        
          
    glPolygonMode(GL_FRONT_AND_BACK, drawmode);
    
    drawFigures(group); 

    // End of frame
    glutSwapBuffers();
}

void processKeys(unsigned char c, int xx, int yy) {

    // put code to process regular keys in here
    switch (c) {
        case 'w':
            if (betaCam <= (M_PI / 2)) betaCam += 0.5f;
            break;
        case 's':
            if (betaCam >= (-M_PI / 2)) betaCam -= 0.5;
            break;
        case 'd':
            alphaCam -= 0.2f;
            break;
        case 'a':
            alphaCam += 0.2f;
            break;
        case '+': // Zoom In
            radius -= 0.2f;
            break;
        case '-': // Zoom out
            radius += 0.2f;
            break;
        case 'm': // Altera os modos de desenho
            if (drawmode == GL_FILL) drawmode = GL_LINE;
            else if (drawmode == GL_LINE) drawmode = GL_POINT;
            else drawmode = GL_FILL;
            break;
        case 'p': // Altera a visibilidade dos eixos
            showAxes = !showAxes;
            break;
    }
    updateCamera();
    glutPostRedisplay();
}


int main(int argc, char **argv) {

    if(argc<2){
        fprintf(stderr, "Usage: %s <path_to_xml>\n", argv[0]);
        return 1;
    }

    set = xmlToSettings(argv[1]);

    height  = getHeight(set);
    width   = getWidth(set);
    camx    = getXPosCam(set);
    camy    = getYPosCam(set);
    camz    = getZPosCam(set);
    radius  = sqrt(camx*camx + camy*camy + camz*camz);
    lookAtx = getXLookAt(set);
    lookAty = getYLookAt(set);
    lookAtz = getZLookAt(set);
    alphaCam = atan2(camx, camz);
    betaCam  = asin(camy/radius);
    upx     = getXUp(set);
    upy     = getYUp(set);
    upz     = getZUp(set);
    fov     = getFov(set);
    nearPlane = getNear(set);
    farPlane = getFar(set);
    group = getGroup(set);

    camX = radius*cosf(betaCam)*sinf(alphaCam);
    camY = radius*sinf(betaCam);
    camZ = radius*cosf(betaCam)*cosf(alphaCam);

    // init GLUT and the window
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
    glutInitWindowPosition(100,100);
    glutInitWindowSize(width,height);
    glutCreateWindow("Projeto CG");

    // Inicializar o tempo base para FPS
    timebase = glutGet(GLUT_ELAPSED_TIME);
        
    // Required callback registry
    glutDisplayFunc(renderScene);
    glutReshapeFunc(changeSize);

    // Callback registration for keyboard processing
    glutKeyboardFunc(processKeys);

    glutIdleFunc(updateFPS);

    // OpenGL settings
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    
    // enter GLUT's main cycle
    glutMainLoop();
    
    return 1;
}
