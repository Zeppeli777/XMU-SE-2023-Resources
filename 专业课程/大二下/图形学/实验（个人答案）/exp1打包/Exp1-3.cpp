#include <math.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif
const int WIDTH = 800;
const int HEIGHT = 600;
const float PI = 3.1415927f;

const int segments = 100;
const float lineWidth = 5.0f;

// 五环参数结构体
struct OlympicRing {
    float centerX, centerY;
    float radius;
    float color[3];
    int zOrder; // 绘制顺序
};

// 官方标准五环参数
OlympicRing rings[5] = {
    {-0.32f,  0.1f, 0.15f, {0.0f, 0.47f, 0.75f}, 3}, // 蓝环 (上层左)
    { 0.0f,   0.1f, 0.15f, {0.0f, 0.0f, 0.0f},  4}, // 黑环 (上层中)
    { 0.32f,  0.1f, 0.15f, {0.87f, 0.13f, 0.23f},2}, // 红环 (上层右)
    {-0.16f, -0.1f, 0.15f, {1.0f, 0.81f, 0.0f}, 0}, // 黄环 (下层左)
    { 0.16f, -0.1f, 0.15f, {0.0f, 0.65f, 0.31f},1}  // 绿环 (下层右)
};

void drawRing(const OlympicRing& ring) {
    

    glColor3fv(ring.color);
    glLineWidth(lineWidth);

    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < segments; ++i) {
        float angle = 2 * PI * i / segments;
        float x = ring.centerX + ring.radius * cosf(angle);
        float y = ring.centerY + ring.radius * sinf(angle);
        glVertex2f(x, y);
    }
    glEnd();
    
}

void RingRecover() {
    
    //禁用深度测试，画在最表面
    GLboolean depthState;
    glGetBooleanv(GL_DEPTH_TEST, &depthState);
    glDisable(GL_DEPTH_TEST);

    // 设置优先绘制参数
    
    glDepthFunc(GL_ALWAYS); // 强制通过深度测试
    
    glColor3fv(rings[0].color);
    glLineWidth(lineWidth);

    glBegin(GL_LINE_LOOP);
    for (int i = 0; i <= segments / 8 ; ++i) {
        float angle = 2 * PI * ((float)i / (float)segments + 7.0/8.0);
        float x = rings[0].centerX + rings[0].radius * cosf(angle);
        float y = rings[0].centerY + rings[0].radius * sinf(angle);
        glVertex2f(x, y);
    }
    glEnd();
    //修补黑环
    glColor3fv(rings[1].color);
    glLineWidth(lineWidth);

    glBegin(GL_LINE_LOOP);
    for (int i = 0; i <= segments / 4; ++i) {
        float angle = 2 * PI * ((float)i / (float)segments/2 + 7.0/8.0);
        float x = rings[1].centerX + rings[1].radius * cosf(angle);
        float y = rings[1].centerY + rings[1].radius * sinf(angle);
        glVertex2f(x, y);
    }
    glEnd();

    glBegin(GL_LINE_LOOP);
    for (int i = 0; i <= segments / 8 ; ++i) {
        float angle = 2 * PI * ((float)i / (float)segments + 5.0/8.0);
        float x = rings[1].centerX + rings[1].radius * cosf(angle);
        float y = rings[1].centerY + rings[1].radius * sinf(angle);
        glVertex2f(x, y);
    }
    glEnd();
    //修补红环
    glColor3fv(rings[2].color);
    glLineWidth(lineWidth);

    glBegin(GL_LINE_LOOP);
    for (int i = 0; i <= segments / 8 ; ++i) {
        float angle = 2 * PI * ((float)i / (float)segments+ 5.0/8.0);
        float x = rings[2].centerX + rings[2].radius * cosf(angle);
        float y = rings[2].centerY + rings[2].radius * sinf(angle);
        glVertex2f(x, y);
    }
    glEnd();
}

// 处理环的交叉覆盖
void drawOlympicRings() {
    // 按z-order从后到前绘制
    for (int z = 4; z >= 0; --z) {
        for (int i = 0; i < 5; ++i) {
            if (rings[i].zOrder == z) {
                drawRing(rings[i]);
            }
        }
    }
}



void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    /*
    //移除以下投影矩阵设置代码（只在 reshape 中处理）
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
    */

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // 启用抗锯齿
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    drawOlympicRings();

    RingRecover();

    glutSwapBuffers();
}

void reshape(int width, int height) {
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    float aspect = (float)width / (float)height;
    if (width <= height) {
        glOrtho(-1.0, 1.0, -1.0 / aspect, 1.0 / aspect, -1.0, 1.0);
    }
    else {
        glOrtho(-1.0 * aspect, 1.0 * aspect, -1.0, 1.0, -1.0, 1.0);
    }
}

void init() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("Olympic Rings - Line Art");

    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);

    glutMainLoop();
    return 0;
}