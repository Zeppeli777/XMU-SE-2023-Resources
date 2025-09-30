#include <GL/glut.h>
#include <cmath>
#include <iostream>
const float M_PI = 3.1415926;

// 球体参数
float sphereRadius = 1.0f;
int slices = 30, stacks = 30;
float sphereRotation = 0.0f;

// 相机参数
float camX = 0.0f, camY = 0.0f, camZ = 5.0f;
float camYaw = 0.0f, camPitch = 0.0f;
bool lockCamera = false;
bool keys[256] = { false };

// 鼠标初始
int lastMouseX = -1, lastMouseY = -1;

// 绘制线框球体（手动）
void drawWireSphere(float radius, int slices, int stacks) {
    for (int i = 0; i <= stacks; ++i) {
        float lat0 = M_PI * (-0.5 + (float)(i - 1) / stacks);
        float lat1 = M_PI * (-0.5 + (float)i / stacks);
        float z0 = radius * sin(lat0);
        float z1 = radius * sin(lat1);
        float zr0 = radius * cos(lat0);
        float zr1 = radius * cos(lat1);

        glBegin(GL_LINE_STRIP);
        for (int j = 0; j <= slices; ++j) {
            float lng = 2 * M_PI * (float)(j - 1) / slices;
            float x = cos(lng);
            float y = sin(lng);
            glVertex3f(x * zr0, y * zr0, z0);
            glVertex3f(x * zr1, y * zr1, z1);
        }
        glEnd();
    }
}

// 相机移动
void updateCamera() {
    float speed = 0.05f;
    float radYaw = camYaw * M_PI / 180.0f;
    float radPitch = camPitch * M_PI / 180.0f;

    float dx = sin(radYaw);
    float dz = -cos(radYaw);
    float dy = sin(radPitch);

    if (!lockCamera) {
        if (keys['w']) { camX += dx * speed; camZ += dz * speed; }
        if (keys['s']) { camX -= dx * speed; camZ -= dz * speed; }
        if (keys['a']) { camX += dz * speed; camZ -= dx * speed; }
        if (keys['d']) { camX -= dz * speed; camZ += dx * speed; }
        if (keys['q']) { camY += speed; }
        if (keys['e']) { camY -= speed; }
    }
}

// 显示回调
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // 设置相机方向
    float radYaw = camYaw * M_PI / 180.0f;
    float radPitch = camPitch * M_PI / 180.0f;
    float lx = cos(radPitch) * sin(radYaw);
    float ly = sin(radPitch);
    float lz = -cos(radPitch) * cos(radYaw);
    gluLookAt(camX, camY, camZ, camX + lx, camY + ly, camZ + lz, 0.0f, 1.0f, 0.0f);

    // 球体旋转
    glPushMatrix();
    glRotatef(sphereRotation, 0.0f, 1.0f, 0.0f);
    drawWireSphere(sphereRadius, slices, stacks);
    glPopMatrix();

    glutSwapBuffers();
}

// 空闲回调：更新动画和相机
void idle() {
    sphereRotation += 0.2f;
    if (sphereRotation > 360.0f) sphereRotation -= 360.0f;

    updateCamera();
    glutPostRedisplay();
}

// 窗口缩放时保持比例
void reshape(int w, int h) {
    if (h == 0) h = 1;
    float ratio = (float)w / h;
    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, ratio, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

// 键盘按下
void keyboard(unsigned char key, int x, int y) {
    if (key == 27) exit(0); // ESC退出
    if (key == 'l' || key == 'L') lockCamera = !lockCamera;
    keys[key] = true;
}

// 键盘松开
void keyboardUp(unsigned char key, int x, int y) {
    keys[key] = false;
}

// 鼠标移动
void mouseMotion(int x, int y) {
    if (lockCamera) return;

    if (lastMouseX == -1) {
        lastMouseX = x;
        lastMouseY = y;
        return;
    }

    int dx = x - lastMouseX;
    int dy = y - lastMouseY;
    lastMouseX = x;
    lastMouseY = y;

    float sensitivity = 0.2f;
    camYaw += dx * sensitivity;
    camPitch -= dy * sensitivity;

    // 限制 pitch 角度
    if (camPitch > 89.0f) camPitch = 89.0f;
    if (camPitch < -89.0f) camPitch = -89.0f;
}

// 初始化
void init() {
    glEnable(GL_DEPTH_TEST);
    glClearColor(0, 0, 0, 1);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Manual Wireframe Sphere");

    init();

    glutDisplayFunc(display);
    glutIdleFunc(idle);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutKeyboardUpFunc(keyboardUp);
    glutPassiveMotionFunc(mouseMotion);

    glutMainLoop();
    return 0;
}
