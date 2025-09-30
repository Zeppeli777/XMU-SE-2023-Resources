#include <GL/freeglut.h>
#include <vector>
#include <cmath>
#include <iostream>

struct Point3 {
    float x, y, z;
    Point3(float _x = 0, float _y = 0, float _z = 0)
        : x(_x), y(_y), z(_z) {}
};

//全局变量
const int MAX_GRID = 5;
int gridSize = 3; // 可切换 3, 4, 5 控制点
Point3 controlPoints[MAX_GRID][MAX_GRID];

float animationProgress = 0.0f;
const float animationSpeed = 0.01f;
const int resolution = 20;

//初始化控制点（随机生成）
void initControlPoints(int size) {
    gridSize = size;
    float step = 2.0f / (gridSize - 1);
    for (int i = 0; i < gridSize; ++i) {
        for (int j = 0; j < gridSize; ++j) {
            controlPoints[i][j] = Point3(
                -1.0f + step * i,
                -1.0f + step * j,
                (rand() % 100) / 100.0f - 0.5f
            );
        }
    }
}

Point3 interpolate(const Point3& p1, const Point3& p2, float t) {
    return Point3(
        (1 - t) * p1.x + t * p2.x,
        (1 - t) * p1.y + t * p2.y,
        (1 - t) * p1.z + t * p2.z
    );
}

//de Casteljau 曲面函数
Point3 deCasteljau1D(std::vector<Point3> pts, float t) {
    int n = pts.size();
    for (int r = 1; r < n; ++r) {
        for (int i = 0; i < n - r; ++i) {
            pts[i] = interpolate(pts[i], pts[i + 1], t);
        }
    }
    return pts[0];
}

Point3 evaluateBezierSurface(float u, float v) {
    std::vector<Point3> temp;
    for (int i = 0; i < gridSize; ++i) {
        std::vector<Point3> row;
        for (int j = 0; j < gridSize; ++j) {
            row.push_back(controlPoints[i][j]);
        }
        temp.push_back(deCasteljau1D(row, v));
    }
    return deCasteljau1D(temp, u);
}

//绘制控制点
void drawControlPoints() {
    glPointSize(8.0f);
    glColor3f(1.0f, 0.2f, 0.2f);
    glBegin(GL_POINTS);
    for (int i = 0; i < gridSize; ++i) {
        for (int j = 0; j < gridSize; ++j) {
            const Point3& p = controlPoints[i][j];
            glVertex3f(p.x, p.y, p.z);
        }
    }
    glEnd();
}

//绘制曲面
void drawBezierSurface(float progress) {
    glColor3f(0.4f, 0.7f, 1.0f);
    for (int i = 0; i < resolution - 1; ++i) {
        float u1 = (float)i / (resolution - 1);
        float u2 = (float)(i + 1) / (resolution - 1);
        if (u1 > progress) break;

        glBegin(GL_QUAD_STRIP);
        for (int j = 0; j < resolution; ++j) {
            float v = (float)j / (resolution - 1);
            if (v > progress) break;

            Point3 p1 = evaluateBezierSurface(u1, v);
            Point3 p2 = evaluateBezierSurface(u2, v);

            glVertex3f(p1.x, p1.y, p1.z);
            glVertex3f(p2.x, p2.y, p2.z);
        }
        glEnd();
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPushMatrix();
    glRotatef(30, 1, 0, 0);
    glRotatef(45, 0, 1, 0);

    drawControlPoints();
    drawBezierSurface(animationProgress);

    glPopMatrix();
    glutSwapBuffers();
}


void update(int value) {
    animationProgress += animationSpeed;
    if (animationProgress > 1.0f)
        animationProgress = 1.0f;

    glutPostRedisplay();


    if (animationProgress < 1.0f) {
        glutTimerFunc(30, update, 0);
    }
}

void reshape(int w, int h) {
    float aspect = (float)w / h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, aspect, 1, 10);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(2, 2, 4, 0, 0, 0, 0, 1, 0);
}
//按r重置动画（不知道为什么按r后要最小化窗口再重新点开才会开始重新绘制），键盘3、4、5切换控制点数量
void keyboard(unsigned char key, int x, int y) {
    if (key == 'r') {
        initControlPoints(gridSize);
        animationProgress = 0.0f;
        glutTimerFunc(30, update, 0);
    }
    if (key == '3') initControlPoints(3);
    if (key == '4') initControlPoints(4);
    if (key == '5') initControlPoints(5);
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    srand((unsigned int)time(NULL));
    initControlPoints(3);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 800);
    glutCreateWindow("Bezier Surface - de Casteljau");

    glEnable(GL_DEPTH_TEST);

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(30, update, 0);

    glutMainLoop();
    return 0;
}
