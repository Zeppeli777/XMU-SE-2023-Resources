#include <math.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else

#include <GL/freeglut.h>
#endif

//定义旋转角度
static GLfloat spin = 0.0;

// 定义圆心位置和半径
float centerX = 0.0f;
float centerY = 0.0f;
float radius = 0.5f;
// 定义用于近似圆形的顶点数量
const int numVertices = 1000;

// 定义图像在屏幕中的偏移量
float offsetX = 0.3f;
float offsetY = 0.3f;

void display(void)
{
    // 清除颜色缓冲区
    glClear(GL_COLOR_BUFFER_BIT);

    // 改变图像在屏幕中的位置
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    //glTranslatef(offsetX, offsetY, 0.0f);

    // 开始绘制多边形来近似圆形
    
    for (int j = 1; j <= 8; j++) {
        glBegin(GL_POLYGON);
        for (int i = (numVertices / 8) * (j-1); i < (numVertices / 8) * j; ++i) {
            // 计算当前顶点的角度
            float angle = spin + 2.0f * 3.1415926f * static_cast<float>(i) / static_cast<float>(numVertices);
            // 计算当前顶点的 x 和 y 坐标
            float x = centerX + radius * cosf(angle);
            float y = centerY + radius * sinf(angle);

            // 指定顶点的颜色
           
            glColor3f(0.125f * static_cast<float>(j) , 1.0f - 0.125f * static_cast<float>(j) , 1.0f/ static_cast<float>(j));

            // 指定当前顶点的坐标
            glVertex2f(x, y);
        }
        glVertex2f(centerX, centerY);
        glEnd();
        
    }
   
    // 刷新 OpenGL 缓冲区
    glutSwapBuffers();
 
}

void spinDisplay(void) {
    spin = spin + 2.0;
    if (spin > 360.0)
        spin = spin - 360.0;
    glutPostRedisplay();
}

void mouse(int button, int state, int x, int y) {
    switch (button) {
        case GLUT_LEFT_BUTTON:
            if (state == GLUT_DOWN)
                glutIdleFunc(spinDisplay);
            break;
        case GLUT_MIDDLE_BUTTON:
            if (state == GLUT_DOWN)
                glutIdleFunc(NULL);
            break;
        default:
            break;
    }
}

void init()
{
    // 设置清除颜色为黑色
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    // 设置投影矩阵
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // 设置正交投影，范围为 -1 到 1
    glOrtho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
}

int main(int argc, char** argv)
{
    // 初始化 GLUT
    glutInit(&argc, argv);
    // 设置显示模式为单缓冲和 RGB 颜色模式
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    // 设置窗口大小
    glutInitWindowSize(500, 500);
    // 设置窗口位置
    glutInitWindowPosition(0, 0);
    // 创建窗口，窗口标题为 "simple"
    glutCreateWindow("simple");

   
    init();

    
    glutDisplayFunc(display);
    glutMouseFunc(mouse);

    
    glutMainLoop();

    return 0;
}