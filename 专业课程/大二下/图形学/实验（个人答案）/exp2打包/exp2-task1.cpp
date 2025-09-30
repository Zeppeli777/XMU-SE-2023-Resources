#include <GL/freeglut.h>
#include <iostream>
#include <cmath>

// 定义三角形的三个顶点
struct Point {
    float x;
    float y;
};

// 计算两点的中点
Point midpoint(const Point& p1, const Point& p2) {
    return { (p1.x + p2.x) / 2, (p1.y + p2.y) / 2 };
}

// 递归绘制 Sierpinski 镂垫
void drawSierpinski(Point a, Point b, Point c, int depth) {
    if (depth == 0) {
        // 绘制三角形
        glBegin(GL_TRIANGLES);
        glVertex2f(a.x, a.y);
        glVertex2f(b.x, b.y);
        glVertex2f(c.x, c.y);
        glEnd();
    }
    else {
        // 计算中点
        Point ab = midpoint(a, b);
        Point bc = midpoint(b, c);
        Point ca = midpoint(c, a);

        // 递归绘制三个子三角形
        drawSierpinski(a, ab, ca, depth - 1);
        drawSierpinski(ab, b, bc, depth - 1);
        drawSierpinski(ca, bc, c, depth - 1);
    }
}

// 全局变量用于控制动画
float rotationAngle = 0.0f;
float scaleFactor = 1.0f;
float colorOffset = 0.0f;
const float colorChangeSpeed = 0.01f;
const float rotationSpeed = 0.5f;
const float scaleSpeed = 0.001f;
const float scaleRange = 0.2f;

// 显示回调函数
void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // 计算颜色
    float r = std::sin(colorOffset);
    float g = std::sin(colorOffset + 2.0f);
    float b = std::sin(colorOffset + 4.0f);
    glColor3f(r, g, b);

    // 设置变换矩阵
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, 0.0f);
    glRotatef(rotationAngle, 0.0f, 0.0f, 1.0f);
    glScalef(scaleFactor, scaleFactor, 1.0f);

    // 定义初始三角形的顶点
    Point a = { -0.8f, -0.8f };
    Point baseB = { 0.8f, -0.8f };
    Point c = { 0.0f, 0.8f };

    // 递归深度
    int depth = 5;

    // 绘制 Sierpinski 镂垫
    drawSierpinski(a, baseB, c, depth);

    glutSwapBuffers();
}

// 定时器回调函数，用于更新动画状态
void timer(int value) {
    // 更新颜色偏移
    colorOffset += colorChangeSpeed;

    // 更新旋转角度
    rotationAngle += rotationSpeed;
    if (rotationAngle >= 360.0f) {
        rotationAngle -= 360.0f;
    }

    // 更新缩放因子
    scaleFactor = 1.0f + scaleRange * std::sin(glutGet(GLUT_ELAPSED_TIME) * scaleSpeed);

    // 标记窗口需要重绘
    glutPostRedisplay();

    // 再次设置定时器
    glutTimerFunc(16, timer, 0);
}

// 初始化 OpenGL 设置
void init() {
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
}

// 主函数
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 800);
    glutCreateWindow("Animated Sierpinski Gasket");

    init();
    glutDisplayFunc(display);
    glutTimerFunc(0, timer, 0);
    glutMainLoop();
    return 0;
}