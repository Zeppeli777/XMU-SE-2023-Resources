#include <GL/glut.h>
#include <GL/GL.h>
#include <cmath>
#include <ctime>

#define PI 3.1415926535
#define LINE_NUM 40 //经线纬线的数量
#define N 100 // 设置圆的顶点数

/* 物体绘制 */
GLfloat theta = 0, fai = 0;
GLfloat radius = 2;

/* 物体旋转 */
GLfloat beta = 0;
GLfloat delta = 1.0; // 旋转速度

/* 键盘交互 */
bool enableMove = true; //是否启用相机移动
GLfloat moveSpeed = 0.1;
GLfloat cameraPosX = 0.0f, cameraPosY = 0.0f, cameraPosZ = 5.0f;
GLfloat lookAtX = 0.0f, lookAtY = 0.0f, lookAtZ = -10.0f;

/* 鼠标交互 */
GLfloat latestX = 0, latestY = 0, latestZ = 0;
GLint w = 1000, h = 1000;

GLfloat aspectRatio = 1.0;// 保存窗口的宽高比

void init()
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glColor3f(1.0, 0.0, 0.0);

	glShadeModel(GL_FLAT);
	glEnable(GL_DEPTH_TEST); //开启深度测试

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(80, 1, 0.1, 60); // 选择透视视景体
	glMatrixMode(GL_MODELVIEW);

	latestX = w / 2;
	latestY = h / 2;
	glutWarpPointer(w / 2, h / 2);
}

GLfloat* getRandomColor()
{
	srand((unsigned)time(NULL));

	GLfloat* colors = new GLfloat[3];

	colors[0] = (double)rand() / (RAND_MAX);
	colors[1] = (double)rand() / (RAND_MAX);
	colors[2] = (double)rand() / (RAND_MAX);

	return colors;
}


void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
	gluLookAt(cameraPosX, cameraPosY, cameraPosZ,
		lookAtX, lookAtY, lookAtZ,
		0, 1, 0); // 控制相机的旋转

	glRotatef(beta, 1.0, -1.0, -1.0); //旋转

	if (aspectRatio < 1.0) 
		glScalef(aspectRatio, aspectRatio, aspectRatio); //缩放

	GLfloat x, y, z;
	//绘制线框球体
	for (int i = 0; i <= LINE_NUM; i++)
	{
		glColor3fv(getRandomColor());//随机颜色

		// 绘制经线
		theta = 2 * PI * i / LINE_NUM;//根据i设置方向角theta
		glBegin(GL_LINE_STRIP);
		for (int j = 0; j <= N; j++)//绘制当前方向角theta的经线
		{
			fai = PI * j / N;							//不断更新极角求经线上顶点的坐标
			x = radius * sin(fai) * cos(theta);//根据球的参数方程计算
			y = radius * sin(fai) * sin(theta);//根据球半径、极角fai和方向角theta计算顶点的坐标
			z = radius * cos(fai);
			glVertex3f(x, y, z);
		}
		glEnd();
		// 绘制纬线
		fai = PI * i / LINE_NUM;//根据i设置极角fai
		glBegin(GL_LINE_STRIP);
		for (int j = 0; j <= N; j++)//绘制当前极角fai的纬线
		{
			theta = 2 * PI * j / N;					//不断更新方向角求经线上顶点的坐标
			x = radius * sin(fai) * cos(theta);
			y = radius * sin(fai) * sin(theta);
			z = radius * cos(fai);
			glVertex3f(x, y, z);
		}
		glEnd();
	}
	
	glutSwapBuffers();
}

void reshape(int w, int h)
{
	glViewport(0, 0, w, h);//修改视口宽高
	aspectRatio = (GLfloat)w / (GLfloat)h;

	// 调整投影矩阵
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(80.0, aspectRatio, 1.0, 60.0);//根据窗口宽高比调整视景体的宽高比
	glMatrixMode(GL_MODELVIEW);
}

void idlefunc()
{
	//改变动画参数
	beta += delta;
	if (beta > 360) beta -= 360;
	glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y)//实现相机互动，如task2相同
{
	GLfloat move = enableMove ? moveSpeed : 0;
	switch (key) {
	case 27: exit(0); break;
	case 's': cameraPosZ += move; lookAtZ += move; break;
	case 'w': cameraPosZ -= move; lookAtZ -= move; break;
	case 'd': cameraPosX += move; lookAtX += move; break;
	case 'a': cameraPosX -= move; lookAtX -= move; break;
	case 'q': cameraPosY += move; lookAtY += move; break;
	case 'e': cameraPosY -= move; lookAtY -= move; break;
	case 'l': enableMove = !enableMove; break; //禁用相机移动
	}
}

void mouse(int x, int y)
{
	if (enableMove)
	{
		lookAtX += (x - latestX) / 25;
		lookAtY += (latestY - y) / 25;
	}
	latestX = x;
	latestY = y;
}


int main(int argc, char** argv)
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(w, h);
	glutInitWindowPosition(100, 100);
	glutCreateWindow(argv[0]);
	init();
	glutPassiveMotionFunc(mouse);
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutIdleFunc(idlefunc);
	glutKeyboardFunc(keyboard);
	glutMainLoop();
	return 0;
}
