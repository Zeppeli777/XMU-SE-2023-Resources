#include <GL/freeglut.h>
#include <Magick++.h>
#include <vector>
#include <iostream>
#include <cmath>

using namespace std;
using namespace Magick;

struct Points {
    float x, y;
};

vector<Points> controlPoints = {
    {100, 100}, {200, 300}, {400, 100}, {400, 200}, {150, 200}, {250, 300}, {500,350},{550,250}
}; // 默认3个控制点，可增至8

vector<Points> bezierCurve;
vector<Image> gifFrames;
bool animating = true;
int selectedPoint = -1;
int frameIndex = 0;
const int totalFrames = 100;

// 工具函数
Points lerp(const Points& a, const Points& b, float t) {
    return { (1 - t) * a.x + t * b.x, (1 - t) * a.y + t * b.y };
}

Points deCasteljau(const vector<Points>& pts, float t) {
    vector<Points> temp = pts;
    int n = temp.size();
    for (int r = 1; r < n; ++r)
        for (int i = 0; i < n - r; ++i)
            temp[i] = lerp(temp[i], temp[i + 1], t);
    return temp[0];
}

void drawPoint(const Points& p, float size = 5.0f) {
    glPointSize(size);
    glBegin(GL_POINTS);
    glVertex2f(p.x, p.y);
    glEnd();
}

void drawLine(const Points& a, const Points& b) {
    glBegin(GL_LINES);
    glVertex2f(a.x, a.y);
    glVertex2f(b.x, b.y);
    glEnd();
}

//添加动画细节
void drawDeCasteljauLines(const vector<Points>& points, float t) {
    vector<Points> current = points;
    int n = current.size();

    glColor3f(1.0f, 1.0f, 0.0f); // 黄色线条

    for (int r = 1; r < n; ++r) {
        vector<Points> next;
        for (int i = 0; i < current.size() - 1; ++i) {
            Points p = lerp(current[i], current[i + 1], t);
            drawLine(current[i], current[i + 1]); // draw line of this level
            next.push_back(p);
        }
        current = next;
    }
}


//渲染函数 
void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1, 1, 1);

    // Draw control polygon
    glColor3f(0.3f, 0.3f, 1);
    for (size_t i = 0; i < controlPoints.size() - 1; ++i)
        drawLine(controlPoints[i], controlPoints[i + 1]);

    // Draw control Points
    glColor3f(1, 0, 0);
    for (const auto& p : controlPoints)
        drawPoint(p, 8);

    // draw bezier lines using de Casteljau interpolation
    float t = frameIndex / float(totalFrames);
    drawDeCasteljauLines(controlPoints, t);

    // Draw bezier curve up to current frame
    glColor3f(0, 1, 0);
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i <= frameIndex; ++i) {
        float t = i / float(totalFrames);
        Points p = deCasteljau(controlPoints, t);
        glVertex2f(p.x, p.y);
    }
    glEnd();

    glutSwapBuffers();
}

// 帧更新生成gif
void timer(int value) {
    if (animating && frameIndex < totalFrames) {
        frameIndex++;

        // 保存当前帧为图片（ImageMagick）
        unsigned char* pixels = new unsigned char[3 * 800 * 600];
        glReadPixels(0, 0, 800, 600, GL_RGB, GL_UNSIGNED_BYTE, pixels);
        Image frame(Geometry(800, 600), "white");
        frame.read(800, 600, "RGB", CharPixel, pixels);
        frame.flip(); // OpenGL的Y轴向上需flip
        gifFrames.push_back(frame);
        delete[] pixels;

        glutPostRedisplay();
        glutTimerFunc(30, timer, 0);
    }
    else if (frameIndex >= totalFrames) {
        animating = false;
        cout << "导出GIF..." << endl;
        writeImages(gifFrames.begin(), gifFrames.end(), "bezier_animation.gif");
        cout << "导出完成。" << endl;
    }
}

//控制点拖动 
void mouseClick(int button, int state, int x, int y) {
    float fx = x;
    float fy = 600 - y;
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        for (int i = 0; i < controlPoints.size(); ++i) {
            if (hypot(controlPoints[i].x - fx, controlPoints[i].y - fy) < 10) {
                selectedPoint = i;
                break;
            }
        }
    }
    else if (state == GLUT_UP) {
        selectedPoint = -1;
    }
}

void mouseMotion(int x, int y) {
    if (selectedPoint != -1) {
        controlPoints[selectedPoint].x = x;
        controlPoints[selectedPoint].y = 600 - y;
        glutPostRedisplay();
    }
}

void init() {
    glClearColor(0.05f, 0.05f, 0.05f, 1);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 800, 0, 600);
    glMatrixMode(GL_MODELVIEW);
}


int main(int argc, char** argv) {
    InitializeMagick(*argv);
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Bezier Curve - de Casteljau");

    init();
    glutDisplayFunc(display);
    glutMouseFunc(mouseClick);
    glutMotionFunc(mouseMotion);
    glutTimerFunc(100, timer, 0);
    glutMainLoop();
    return 0;
}