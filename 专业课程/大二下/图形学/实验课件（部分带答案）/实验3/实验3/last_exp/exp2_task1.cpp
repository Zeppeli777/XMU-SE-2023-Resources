#include <GL/glut.h>
#include<cmath>

GLfloat PI = 3.1415;
GLfloat r = 1.0f, g = 0.0f, b = 1.0f;
GLfloat radius = 1.0;
GLfloat angle = 0;
bool flag = true;

void init(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glColor3f(1.0, 0.0, 0.0);
}

void sierpinski(float x1, float y1, float x2, float y2, float x3, float y3, int currentDepth) {
	if (currentDepth == 0) {
		 r = rand() % 100 / 100.0;
		 g = rand() % 100 / 100.0;
		 b = rand() % 100 / 100.0;
		glColor3f(r, g, b);//随机颜色
		glBegin(GL_TRIANGLES);//绘制
		glVertex2f(x1, y1);
		glVertex2f(x2, y2);
		glVertex2f(x3, y3);
		glEnd();
	}
	else {
		// 计算中点
		float mid1x = (x1 + x2) / 2, mid1y = (y1 + y2) / 2;
		float mid2x = (x2 + x3) / 2, mid2y = (y2 + y3) / 2;
		float mid3x = (x3 + x1) / 2, mid3y = (y3 + y1) / 2;

		// 递归绘制三个子三角形
		sierpinski(x1, y1, mid1x, mid1y, mid3x, mid3y, currentDepth - 1);
		sierpinski(mid1x, mid1y, x2, y2, mid2x, mid2y, currentDepth - 1);
		sierpinski(mid3x, mid3y, mid2x, mid2y, x3, y3, currentDepth - 1);
	}
}

void display(void)
{

	/* 实现旋转和缩放方法一：在初始三角形顶点坐标控制每次旋转与缩放 */
	GLfloat vertices[3][2] = { {radius * cos(angle + 5.0 / 4 * PI),radius * sin(angle + 5.0 / 4 * PI)}
					,{radius * cos(angle + 1.0 / 2 * PI),radius * sin(angle + 1.0 / 2 * PI)}
					,{radius * cos(angle + 7.0 / 4 * PI),radius * sin(angle + 7.0 / 4 * PI)} };//三角形三个顶点坐标
	/*实现旋转和缩放方法二：通过旋转函数和缩放函数*/
	//GLfloat vertices[3][2] = { {1,-1},{-1,-1},{0,0.5} };//三角形三个顶点坐标
	//glLoadIdentity();
	//glRotatef(angle,0,0,-1 );
	//glScalef(radius, radius, radius);
	int j, k;
	GLfloat p[2] = { 0.0,0.0 };  /* 初始点内部点 */
	glClear(GL_COLOR_BUFFER_BIT);  /* clear the window */
	//随机点法
	glBegin(GL_POINTS);
	r = rand() % 100 / 100.0;
	g = rand() % 100 / 100.0;
	b = rand() % 100 / 100.0;
	float color1[3] = { r,g,b };
	float color2[3] = { g,b,r };
	float color3[3] = { b,r,g };
	

	/* compute and plots 50000 new points */
	
	
	
	sierpinski(vertices[0][0], vertices[0][1], vertices[1][0], vertices[1][1], vertices[2][0], vertices[2][1], 5);
	// 配置下次更新的角度三角形大小
	angle += 0.05;
	if (flag)
		radius -= 0.01;
	else
		radius += 0.01;
	if (radius > 1.2 || radius < 0.2)
		flag = !flag;
	
	glFlush(); /* clear buffers */
}

void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);

}


void idlefunc()
{
	Sleep(30);
	glutPostRedisplay();
}


void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 27:
		exit(0);
		break;
	}
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Sierpinski Gasket");
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutIdleFunc(idlefunc);
	glutKeyboardFunc(keyboard);
	glutMainLoop();
	return 0;
}