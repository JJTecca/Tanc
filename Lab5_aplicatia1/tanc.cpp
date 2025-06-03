#include "glos.h"
#include "gl.h"
#include "glu.h"
#include "glaux.h"
#include "bitmap.h"
#include <stdio.h>
#include <cmath>

static GLfloat x = 0;
static GLfloat y = 0;
static GLfloat alpha = 0;
static GLfloat sizeX = 1.55;  
static GLfloat sizeY = 1.5;
static GLfloat sizeZ = 4.25;
static int rotate = 0;
GLUquadricObj* wheelsFront[8];
GLUquadricObj* wheelsBack[8];
GLUquadricObj* diskwheelsFront[8];
GLUquadricObj* diskwheelsBack[8];

GLuint textureId1;
GLuint textureId2;
GLuint textureId3;

GLfloat ctrlpoints[4][4][3] = {
    {{-0.5, -3.0, -1.0}, {-0.5, -4.0, 0.5}, {0.5, -4.0, 0.5}, {1.5, -3.0, 0.0}},
    {{-0.5, -2.0, 0.5}, {-0.5, -2.0, 1.5}, {0.5, -2.0, 1.5}, {1.5, -2.0, 0.5}},
    {{-0.5, 2.0, 0.5}, {-0.5, 2.0, 2.0}, {0.5, 2.0, 2.0}, {1.5, 2.0, 0.5}},
    {{-0.5, 5.0, -1.0}, {-0.5, 6.0, 0.5}, {0.5, 6.0, 0.5}, {1.5, 5.0, 0.0}}
};

GLfloat ctrlpoints1[4][4][3] = {
    {{-0.5, -3.0, -0.0}, {-0.5, -4.0, 0.5}, {0.5, -4.0, 0.5}, {1.5, -3.0, 0.0}},
    {{-0.5, -2.0, 0.5}, {-0.5, -2.0, 1.5}, {0.5, -2.0, 1.5}, {1.5, -2.0, 0.5}},
    {{-0.5, 2.0, 0.5}, {-0.5, 2.0, 2.0}, {0.5, 2.0, 2.0}, {1.5, 2.0, 0.5}},
    {{-0.5, 4.0, 0.0}, {-0.5, 5.0, 0.5}, {0.5, 5.0, 0.5}, {1.5, 4.0, 0.0}}
};

GLfloat ctrlpoints2[4][4][3] = {
    {{-2.0, 4.0, 0.5}, {-0.7, 3.0, 0.0}, {0.7, 4.0, 0.5}, {2.0, 4.0, 0.5}},
    {{-2.0, 1.5, 0.0}, {-0.7, 0.5, -1.0}, {0.7, 3.5, -0.0}, {2.0, 3.5, 0.0}},
    {{-2.0, -1.5, 0.0}, {-0.7, -1.5, -1.0}, {0.7, -1.5, -1.0}, {2.0, -1.5, 0.0}},
    {{-2.0, -2.0, 0.0}, {-0.7, -3.0, 0.0}, {0.7, -3.0, 0.7}, {2.0, -4.5, 0.3}} 
};

typedef struct {
    char caracter;
    GLubyte* letter;
}BitmapLetter;

BitmapLetter* Letters = new BitmapLetter[14]{
    { 'M', litera_M },
    { 'A', litera_A },
    { 'I', litera_I },
    { 'O', litera_O },
    { 'R', litera_R },
    { 'C', litera_C },
    { 'S', litera_S },
    { 'T', litera_T },
    { 'N', litera_N },
    { 'L', litera_L },
    { 'E', litera_E },
    { 'X', litera_X },
    { 'D', litera_D },
    { 'U', litera_U }
};

GLuint incarcaTextura(const char* s)
{
    GLuint textureId = 0;
    AUX_RGBImageRec* pImagineTextura = auxDIBImageLoad(s);

    if (pImagineTextura != NULL)
    {
        glGenTextures(1, &textureId);
        glBindTexture(GL_TEXTURE_2D, textureId);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, 3, pImagineTextura->sizeX, pImagineTextura->sizeY,
            0, GL_RGB, GL_UNSIGNED_BYTE, pImagineTextura->data);
    }
    if (pImagineTextura)
    {
        if (pImagineTextura->data) {
            free(pImagineTextura->data);
        }
        free(pImagineTextura);
    }
    return textureId;
}



void CALLBACK RotireTunDreapta(void) { rotate = (rotate - 3) % 360; }
void CALLBACK RotireTunStanga(void) { rotate = (rotate + 3) % 360; }
void CALLBACK MutaStanga(void) { x -= 10; }
void CALLBACK MutaDreapta(void) { x += 10; }
void CALLBACK rotateLeft(void) { alpha -= 10; }
void CALLBACK rotateRight(void) { alpha += 10; }
void CALLBACK rotateObj1(void) {}
void CALLBACK rotateObj2(void) { alpha -= 35; }

void designWheel(GLUquadricObj* obj, int position, int radius, bool front) {
    obj = gluNewQuadric();
    glBindTexture(GL_TEXTURE_2D, textureId2);
    gluQuadricOrientation(obj, GLU_OUTSIDE);
    {
        gluQuadricDrawStyle(obj, GLU_FILL);
        glPushMatrix();
        if (front) {
            if (position < 6)
                glTranslatef(0, 30, 20 * position);
            else
                glTranslatef(5, 30, 20 * position + 10);
        }
        else {
            if (position < 6)
                glTranslatef(65, 0, 20 * position);
            else
                glTranslatef(70, 0, 20 * position + 10);
        }
        glRotatef(-90, 1, 0, 0);
        gluCylinder(obj, radius, radius, 35.0, 50, 70);
        glPopMatrix();
    }
}


void drawBezier(GLfloat ctrlpoints[4][4][3]) {
    glScalef(20.0f, 20.0f, 20.0f);
    glRotatef(120, 1, 1, 1);

    glEnable(GL_MAP2_VERTEX_3);
    glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 4,
        0, 1, 12, 4, &ctrlpoints[0][0][0]);

    glMapGrid2f(20, 0.0, 1.0, 20, 0.0, 1.0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glBindTexture(GL_TEXTURE_2D, textureId1);

    for (int j = 0; j < 20; j++) {
        glBegin(GL_TRIANGLE_STRIP);
        for (int i = 0; i <= 20; i++) {
            float u = (float)i / 20.0f;
            float v1 = (float)(j + 1) / 20.0f;
            float v2 = (float)j / 20.0f;

            glTexCoord2f(u, v1);
            glEvalCoord2f(u, v1);

            glTexCoord2f(u, v2);
            glEvalCoord2f(u, v2);
        }
        glEnd();
    }

    glDisable(GL_MAP2_VERTEX_3);
}

void Body() {
    glBegin(GL_QUADS);

    glNormal3f(0.0f, 0.0f, 1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-sizeX, -sizeY, sizeZ);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(sizeX, -sizeY, sizeZ);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(sizeX, sizeY, sizeZ);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-sizeX, sizeY, sizeZ);

    glNormal3f(0.0f, 0.0f, -1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-sizeX, -sizeY, -sizeZ);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(sizeX, -sizeY, -sizeZ);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(sizeX, sizeY, -sizeZ);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-sizeX, sizeY, -sizeZ); 

    glNormal3f(-1.0f, 0.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-sizeX, -sizeY, -sizeZ);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-sizeX, -sizeY, sizeZ);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-sizeX, sizeY, sizeZ);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-sizeX, sizeY, -sizeZ); 

    glNormal3f(1.0f, 0.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(sizeX, -sizeY, -sizeZ);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(sizeX, -sizeY, sizeZ);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(sizeX, sizeY, sizeZ);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(sizeX, sizeY, -sizeZ); 

    glNormal3f(0.0f, 1.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-sizeX, sizeY, -sizeZ);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(sizeX, sizeY, -sizeZ);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(sizeX, sizeY, sizeZ);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-sizeX, sizeY, sizeZ);

    glNormal3f(0.0f, -1.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-sizeX, -sizeY, -sizeZ);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(sizeX, -sizeY, -sizeZ);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(sizeX, -sizeY, sizeZ);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-sizeX, -sizeY, sizeZ); 

    glEnd();
}


void BodyFront(int sign) {
    GLfloat baseY = 0.0f;
    GLfloat topY = 60.0f;

    GLfloat frontZ = sign * 40.0f;
    GLfloat midZ = sign * 30.0f;

    GLfloat A[] = { -40.0f, baseY, 0.0f };
    GLfloat B[] = { 60.0f, baseY, 0.0f };
    GLfloat C[] = { 40.0f, baseY, frontZ };
    GLfloat D[] = { -20.0f, baseY, frontZ };

    GLfloat E[] = { -20.0f, topY, frontZ };
    GLfloat F[] = { 40.0f, topY, frontZ };
    GLfloat G[] = { 50.0f, topY, 0.0f };
    GLfloat H[] = { -30.0f, topY, 0.0f };

    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3fv(D);
    glTexCoord2f(1.0f, 0.0f); glVertex3fv(C);
    glTexCoord2f(1.0f, 1.0f); glVertex3fv(F);
    glTexCoord2f(0.0f, 1.0f); glVertex3fv(E); 

    glTexCoord2f(0.0f, 0.0f); glVertex3fv(A);
    glTexCoord2f(1.0f, 0.0f); glVertex3fv(D);
    glTexCoord2f(1.0f, 1.0f); glVertex3fv(E);
    glTexCoord2f(0.0f, 1.0f); glVertex3fv(H);

    glTexCoord2f(0.0f, 0.0f); glVertex3fv(C);
    glTexCoord2f(1.0f, 0.0f); glVertex3fv(B);
    glTexCoord2f(1.0f, 1.0f); glVertex3fv(G);
    glTexCoord2f(0.0f, 1.0f); glVertex3fv(F); 

    glTexCoord2f(0.0f, 0.0f); glVertex3fv(E);
    glTexCoord2f(1.0f, 0.0f); glVertex3fv(F);
    glTexCoord2f(1.0f, 1.0f); glVertex3fv(G);
    glTexCoord2f(0.0f, 1.0f); glVertex3fv(H); 

    glEnd();
}

void setupLighting(GLfloat* light_ambient, GLfloat* light_diffuse, GLfloat* light_specular, GLfloat* light_position,
    GLfloat* mat_ambient, GLfloat* mat_diffuse, GLfloat* mat_specular, GLfloat* mat_shininess) {


    glEnable(GL_LIGHTING);

    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glEnable(GL_LIGHT0);
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
}
void myinit(void) {

    glClearColor(0.5, 0.5, 0.5, 1.0);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
   
    textureId1 = incarcaTextura("tracks.bmp");
    textureId2 = incarcaTextura("parchet.bmp");
    textureId3 = incarcaTextura("camo.bmp");

    glEnable(GL_TEXTURE_2D);
    glShadeModel(GL_FLAT);

    glEnable(GL_NORMALIZE);
    glEnable(GL_AUTO_NORMAL);


}

void CALLBACK display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity(); 
    glTranslatef(-70 + x, -30 + y, -200);
    glRotatef(145 + alpha, 1, 1, 1);
    gluLookAt(0.0, 0.0, 3.0,
        0.0, 0.0, 0.0,
        0.0, 1.0, 0.0);


    GLfloat** position_lightnings = (GLfloat**)malloc(5 * sizeof(GLfloat*));
    GLfloat** ambient_lightnings = (GLfloat**)malloc(5 * sizeof(GLfloat*));
    GLfloat** diffuse_lightnings = (GLfloat**)malloc(5 * sizeof(GLfloat*));
    GLfloat** specular_lightnings = (GLfloat**)malloc(5 * sizeof(GLfloat*));

    GLfloat light_positions[][4] = {
        { 0.0f, 100.0f, 150.0f, 1.0f },
        { 60.0f, 80.0f, 100.0f, 1.0f },
        { 100.0f, -60.0f, 80.0f, 1.0f },
        { 0.0f, 140.0f, 40.0f, 1.0f },
        { 0.0f, 160.0f, 120.0f, 1.0f }
    };

    GLfloat light_ambient[][4] = {
        { 0.7f, 0.7f, 0.7f, 1.0f },
        { 0.6f, 0.6f, 0.8f, 1.0f },
        { 0.8f, 0.6f, 0.6f, 1.0f },
        { 0.3f, 0.3f, 0.3f, 1.0f },
        { 0.7f, 0.7f, 0.2f, 1.0f }
    };

    GLfloat light_diffuse[][4] = {
        { 1.4f, 1.4f, 1.4f, 1.0f },
        { 1.0f, 1.0f, 1.4f, 1.0f },
        { 1.4f, 1.0f, 1.0f, 1.0f },
        { 1.6f, 1.6f, 1.6f, 1.0f },
        { 1.4f, 1.4f, 1.0f, 1.0f }
    };

    GLfloat light_specular[][4] = {
        { 1.0f, 1.0f, 1.0f, 1.0f },
        { 0.9f, 0.9f, 1.0f, 1.0f },
        { 1.0f, 0.9f, 0.9f, 1.0f },
        { 1.2f, 1.2f, 1.2f, 1.0f },
        { 1.0f, 1.0f, 0.9f, 1.0f }
    };

    for (int columns = 0; columns < 5; columns++) {
        *(position_lightnings + columns) = (GLfloat*)(malloc(4 * sizeof(GLfloat)));
        *(ambient_lightnings + columns) = (GLfloat*)(malloc(4 * sizeof(GLfloat)));
        *(diffuse_lightnings + columns) = (GLfloat*)(malloc(4 * sizeof(GLfloat)));
        *(specular_lightnings + columns) = (GLfloat*)(malloc(4 * sizeof(GLfloat)));
    }
    for (int rows = 0; rows < 5; rows++) {
        for (int columns = 0; columns < 4; columns++) {
            position_lightnings[rows][columns] = light_positions[rows][columns];
            ambient_lightnings[rows][columns] = light_ambient[rows][columns];
            diffuse_lightnings[rows][columns] = light_diffuse[rows][columns];
            specular_lightnings[rows][columns] = light_specular[rows][columns];
        }
    }


    GLfloat mat_ambient[] = { 0.5f, 0.5f, 0.5f, 1.0f };
    GLfloat mat_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat mat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat mat_shininess[] = { 90.0f };

    for (int i = 0; i < 6; i++) {
        setupLighting(ambient_lightnings[0], diffuse_lightnings[0], specular_lightnings[0], position_lightnings[0],
            mat_ambient, mat_diffuse, mat_specular, mat_shininess);
        designWheel(wheelsFront[i], i, 10, true);
        designWheel(diskwheelsFront[i], i, 4, true);
        designWheel(wheelsBack[i], i, 10, false);
        designWheel(diskwheelsBack[i], i, 4, false);
    }
    setupLighting(ambient_lightnings[0], diffuse_lightnings[0], specular_lightnings[0], position_lightnings[0],
        mat_ambient, mat_diffuse, mat_specular, mat_shininess);
    designWheel(wheelsFront[7], 6, 10, true);
    designWheel(diskwheelsFront[7], 6, 4, true);
    designWheel(wheelsBack[7], 6, 10, false);
    designWheel(diskwheelsBack[7], 6, 4, false);

    glPushMatrix(); glTranslatef(70.0f, 0.0f, 45.0f);
    setupLighting(ambient_lightnings[1], diffuse_lightnings[1], specular_lightnings[1], position_lightnings[1],
        mat_ambient, mat_diffuse, mat_specular, mat_shininess);
    drawBezier(ctrlpoints); glPopMatrix();

    glPushMatrix(); glTranslatef(10.0f, 40.0f, 55.0f);
    setupLighting(ambient_lightnings[2], diffuse_lightnings[2], specular_lightnings[2], position_lightnings[2],
        mat_ambient, mat_diffuse, mat_specular, mat_shininess);
    drawBezier(ctrlpoints1); glPopMatrix();

    glPushMatrix(); glTranslatef(-13.0f, 40.0f, 65.0f);
    setupLighting(ambient_lightnings[3], diffuse_lightnings[3], specular_lightnings[3], position_lightnings[3],
        mat_ambient, mat_diffuse, mat_specular, mat_shininess);
    drawBezier(ctrlpoints2); glPopMatrix();

    glPushMatrix(); glTranslatef(55.0f, 30.0f, 60.0f);
    setupLighting(ambient_lightnings[4], diffuse_lightnings[4], specular_lightnings[4], position_lightnings[4],
        mat_ambient, mat_diffuse, mat_specular, mat_shininess);
    drawBezier(ctrlpoints2); glPopMatrix();

    glPushMatrix();
    glTranslatef(55.0f, 30.0f, 65.0f);
    glScalef(20.0f, 20.0f, 20.0f);
    glBindTexture(GL_TEXTURE_2D, textureId3);
    setupLighting(ambient_lightnings[0], diffuse_lightnings[0], specular_lightnings[0], position_lightnings[0],
        mat_ambient, mat_diffuse, mat_specular, mat_shininess);
    Body();
    glPopMatrix();

    glPushMatrix(); glTranslatef(30.0f, 0.0f, 150.0f);
    setupLighting(ambient_lightnings[1], diffuse_lightnings[1], specular_lightnings[1], position_lightnings[1],
        mat_ambient, mat_diffuse, mat_specular, mat_shininess);
    BodyFront(1); glPopMatrix();

    glPushMatrix(); glTranslatef(30.0f, 0.0f, -20.0f);
    setupLighting(ambient_lightnings[2], diffuse_lightnings[2], specular_lightnings[2], position_lightnings[2],
        mat_ambient, mat_diffuse, mat_specular, mat_shininess);
    BodyFront(-1); glPopMatrix();

    glPushMatrix();
    GLUquadricObj* obj = gluNewQuadric();
    gluQuadricOrientation(obj, GLU_OUTSIDE);
    gluQuadricDrawStyle(obj, GLU_FILL);
    glTranslatef(80.0f, 25.0f, 65.0f);
    glRotatef(90, 0, 1, 0);
    glScalef(5.0f, 2.0f, 1.0f);
    setupLighting(ambient_lightnings[3], diffuse_lightnings[3], specular_lightnings[3], position_lightnings[3],
        mat_ambient, mat_diffuse, mat_specular, mat_shininess);
    gluQuadricTexture(obj, GL_TRUE);
    glBindTexture(GL_TEXTURE_2D, textureId3);
    gluCylinder(obj, 10, 10, 35.0, 50, 70);
    glPopMatrix();

    glPushMatrix();
    GLUquadricObj* obj1 = gluNewQuadric();
    gluQuadricOrientation(obj1, GLU_OUTSIDE);
    gluQuadricDrawStyle(obj1, GLU_FILL);
    glTranslatef(105.0f, 25.0f, 80.0f);
    glRotatef(rotate, 1.0f, 0.0f, 0.0f);
    glScalef(1.0f, 1.0f, 4.0f);
    setupLighting(ambient_lightnings[4], diffuse_lightnings[4], specular_lightnings[4], position_lightnings[4],
        mat_ambient, mat_diffuse, mat_specular, mat_shininess);
    gluCylinder(obj1, 10, 10, 35.0, 50, 70);
    glPopMatrix();

    for (int i = 0; i < 5; i++) {
        free(position_lightnings[i]);
        free(ambient_lightnings[i]);
        free(diffuse_lightnings[i]);
        free(specular_lightnings[i]);
    }
    free(position_lightnings);
    free(ambient_lightnings);
    free(diffuse_lightnings);
    free(specular_lightnings);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, 800, 600, 0);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos2f(20.0f, 20.0f);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for (int nume = 0; nume < 5; nume++) {
        glBitmap(10, 12, 0.0, 0.0, 20.0, 0.0, Letters[nume].letter);
    }

    glBitmap(0, 0, 0.0, 0.0, 20.0, 0.0, nullptr);
    glBitmap(10, 12, 0.0, 0.0, 20.0, 0.0, Letters[5].letter); 
    glBitmap(10, 12, 0.0, 0.0, 20.0, 0.0, Letters[4].letter);  
    glBitmap(10, 12, 0.0, 0.0, 20.0, 0.0, Letters[2].letter); 
    glBitmap(10, 12, 0.0, 0.0, 20.0, 0.0, Letters[6].letter);
    glBitmap(10, 12, 0.0, 0.0, 20.0, 0.0, Letters[7].letter); 
    glBitmap(10, 12, 0.0, 0.0, 20.0, 0.0, Letters[2].letter); 
    glBitmap(10, 12, 0.0, 0.0, 20.0, 0.0, Letters[1].letter); 
    glBitmap(10, 12, 0.0, 0.0, 20.0, 0.0, Letters[8].letter);  

    glBitmap(0, 0, 0.0, 0.0, 20.0, 0.0, nullptr);
    glBitmap(10, 12, 0.0, 0.0, 20.0, 0.0, Letters[1].letter); 
    glBitmap(10, 12, 0.0, 0.0, 20.0, 0.0, Letters[9].letter);
    glBitmap(10, 12, 0.0, 0.0, 20.0, 0.0, Letters[10].letter); 
    glBitmap(10, 12, 0.0, 0.0, 20.0, 0.0, Letters[11].letter);
    glBitmap(10, 12, 0.0, 0.0, 20.0, 0.0, Letters[1].letter); 
    glBitmap(10, 12, 0.0, 0.0, 20.0, 0.0, Letters[8].letter); 
    glBitmap(10, 12, 0.0, 0.0, 20.0, 0.0, Letters[12].letter);
    glBitmap(10, 12, 0.0, 0.0, 20.0, 0.0, Letters[4].letter); 
    glBitmap(10, 12, 0.0, 0.0, 20.0, 0.0, Letters[13].letter);

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    auxSwapBuffers();
}

void CALLBACK myReshape(GLsizei w, GLsizei h) {
    if (!h) return;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (w <= h)
        glOrtho(-200.0, 200.0, -200.0 * (GLfloat)h / (GLfloat)w, 200.0 * (GLfloat)h / (GLfloat)w, -250.0, 250.0);
    else
        glOrtho(-200.0 * (GLfloat)w / (GLfloat)h, 200.0 * (GLfloat)w / (GLfloat)h, -200.0, 200.0, -250.0, 250.0);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    glEnable(GL_DEPTH_TEST);
    auxInitDisplayMode(AUX_DOUBLE | AUX_RGB);
    auxInitPosition(200, 100, 800, 600);
    auxInitWindow("Proiect tank");
    myinit();
    auxKeyFunc(AUX_LEFT, RotireTunStanga);
    auxKeyFunc(AUX_RIGHT, RotireTunDreapta);    
    //auxKeyFunc(AUX_UP, MutaSus);
    //auxKeyFunc(AUX_DOWN, MutaJos);
    auxKeyFunc(AUX_w, rotateLeft);
    auxKeyFunc(AUX_s, rotateRight);
    auxKeyFunc(AUX_a, MutaStanga);
    auxKeyFunc(AUX_d, MutaDreapta);
    auxReshapeFunc(myReshape);
    auxMainLoop(display);
    return 0;
}