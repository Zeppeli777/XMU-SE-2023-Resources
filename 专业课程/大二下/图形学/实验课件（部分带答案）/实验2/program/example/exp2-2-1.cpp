#include <GL/glut.h>
#include<math.h>

#define DEG_TO_RAD 0.017453
const GLfloat RR = 2.0; //camera rad
GLfloat beta = 0;

//新增全局变量
float camX = 4.0f, camY = 2.0f, camZ = 4.0f;        // 摄像机位置
float yaw = -135.0f, pitch = -20.0f;                // 摄像机旋转角
float lastX = 250, lastY = 250;                     // 鼠标上次位置
bool firstMouse = true;
bool cameraLocked = false;

float cameraSpeed = 0.1f; // 视角移动速度


void init(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glColor3f(1.0, 0.0, 0.0);

	//以下语句二选一，看看效果
	glShadeModel(GL_SMOOTH);
	//glShadeModel(GL_FLAT);

	glEnable(GL_DEPTH_TEST);
}

void display(void)
{
	// glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	/*gluLookAt(RR * sin(beta * DEG_TO_RAD), RR * cos(beta * DEG_TO_RAD) * cos(45.0), RR * cos(beta * DEG_TO_RAD) * sin(45),
		0.0, 0.0, 0.0,
		0.0, 1.0, 0.0);*/
	
	// 计算方向向量
	float frontX = cos(pitch * DEG_TO_RAD) * cos(yaw * DEG_TO_RAD);
	float frontY = sin(pitch * DEG_TO_RAD);
	float frontZ = cos(pitch * DEG_TO_RAD) * sin(yaw * DEG_TO_RAD);

	gluLookAt(camX, camY, camZ,
		camX + frontX, camY + frontY, camZ + frontZ,
		0.0f, 1.0f, 0.0f);

	//物体旋转
	//glRotatef(beta, 0.0, 1.0, 0.0);

	glBegin(GL_TRIANGLES);
	// face A, part 1
	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(-1, 1, -1);
	glVertex3f(-1, 1, 1);
	glVertex3f(1, 1, -1);
	glEnd();

	// face A, part 2
	glBegin(GL_TRIANGLES);
	glColor3f(0.5, 0, 0);
	glVertex3f(-1, 1, 1);
	glVertex3f(1, 1, -1);
	glVertex3f(1, 1, 1);
	glEnd();
	// face B, part 1
	glBegin(GL_TRIANGLES);
	glColor3f(1, 1, 0);
	glVertex3f(1, 1, -1);
	glVertex3f(1, 1, 1);
	glVertex3f(1, -1, -1);
	glEnd();
	// face B, part 2
	glBegin(GL_TRIANGLES);
	glColor3f(1, 1, 0.5);
	glVertex3f(1, 1, 1);
	glVertex3f(1, -1, -1);
	glVertex3f(1, -1, 1);
	glEnd();
	// face C
	glBegin(GL_TRIANGLES);
	glColor3f(0, 1, 0);
	glVertex3f(-1, 1, 1);
	glVertex3f(1, 1, 1);
	glVertex3f(-1, -1, 1);
	glEnd();
	glBegin(GL_TRIANGLES);
	glColor3f(0, 0.5, 0);
	glVertex3f(-1, -1, 1);
	glVertex3f(1, 1, 1);
	glVertex3f(1, -1, 1);
	glEnd();
	// face D
	glBegin(GL_TRIANGLES);
	glColor3f(0, 0, 1);
	glVertex3f(-1, 1, 1);
	glVertex3f(-1, -1, -1);
	glVertex3f(-1, 1, -1);
	glEnd();
	glBegin(GL_TRIANGLES);
	glColor3f(0, 0, 0.5);
	glVertex3f(-1, 1, 1);
	glVertex3f(-1, -1, -1);
	glVertex3f(-1, -1, 1);
	glEnd();
	// face E
	glBegin(GL_TRIANGLES);
	glColor3f(1, 0, 1);
	glVertex3f(-1, 1, -1);
	glVertex3f(1, -1, -1);
	glVertex3f(-1, -1, -1);
	glEnd();
	glBegin(GL_TRIANGLES);
	glColor3f(0.5, 0, 0.5);
	glVertex3f(-1, 1, -1);
	glVertex3f(1, -1, -1);
	glVertex3f(1, 1, -1);
	glEnd();
	//face F
	glBegin(GL_TRIANGLES);
	glColor3f(1, 1, 1);
	glVertex3f(-1, -1, 1);
	glVertex3f(1, -1, 1);
	glVertex3f(1, -1, -1);
	glEnd();
	glBegin(GL_TRIANGLES);
	glColor3f(0.5, 0.5, 0.5);
	glVertex3f(-1, -1, 1);
	glVertex3f(-1, -1, -1);
	glVertex3f(1, -1, -1);
	glEnd();

	//glFlush(); /* clear buffers */
	glutSwapBuffers();
}


void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);

	//以下语句设置相机投影参数
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//以下两句二选一，看看效果
	//glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
	glOrtho(-4.0, 4.0, -4.0, 4.0, -4.0, 4.0);
	//glFrustum(-1.0, 1.0, -1.0, 1.0, 0.3, 10.0);


	//以下语句设置相机和物体的相对方位
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	//gluLookAt(0.0, 0.0, 0.5, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}


void idlefunc()
{
	//改变动画参数
	beta += 0.05;
	if (beta > 360) beta -= 360;
	glutPostRedisplay();
}

//键盘控制
void keyboard(unsigned char key, int x, int y)
{
	if (cameraLocked) return;

	float radYaw = yaw * DEG_TO_RAD;
	float radPitch = pitch * DEG_TO_RAD;

	// 摄像机方向向量
	float dirX = cos(radPitch) * cos(radYaw);
	float dirY = sin(radPitch);
	float dirZ = cos(radPitch) * sin(radYaw);

	// 摄像机右方向（水平）
	float rightX = -sin(radYaw);
	float rightZ = cos(radYaw);

	switch (key) {
	case 'w':
		camX += cameraSpeed * dirX;
		camY += cameraSpeed * dirY;
		camZ += cameraSpeed * dirZ;
		break;
	case 's':
		camX -= cameraSpeed * dirX;
		camY -= cameraSpeed * dirY;
		camZ -= cameraSpeed * dirZ;
		break;
	case 'a':
		camX -= cameraSpeed * rightX;
		camZ -= cameraSpeed * rightZ;
		break;
	case 'd':
		camX += cameraSpeed * rightX;
		camZ += cameraSpeed * rightZ;
		break;
	case 'q':
		camY += cameraSpeed;
		break;
	case 'e':
		camY -= cameraSpeed;
		break;
	case 'l': case 'L':
		cameraLocked = !cameraLocked;
		break;
	case 27:
		exit(0);
	}
}
//鼠标控制
void motion(int x, int y)
{
	if (cameraLocked) return;

	if (firstMouse) {
		lastX = x;
		lastY = y;
		firstMouse = false;
	}

	float xoffset = x - lastX;
	float yoffset = lastY - y; 

	lastX = x;
	lastY = y;

	float sensitivity = 0.1f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f) pitch = 89.0f;
	if (pitch < -89.0f) pitch = -89.0f;
}


int main(int argc, char** argv)
{
	/*
		1. draw cube:
			draw each face with 2 triangles
		2. adjust view point:
			modify params in gluLookAt
		3. try to move camera:
			3.1 open depth
				glEnable(GL_DEPTH_TEST);
				glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			3.2 change viewpoint dynamicly
				3.2.1 load camera in display
					glMatrixMode(GL_MODELVIEW);
					glLoadIdentity();
					gluLookAt();
				3.2.2 change angle in idlefunc
					note pre-define constant and angle2rad
			3.3 use double buffer to smooth
				glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
				glutSwapBuffers();
	*/
	glutInit(&argc, argv);
	//glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	//glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow(argv[0]);
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutIdleFunc(idlefunc);
	glutKeyboardFunc(keyboard);
	glutPassiveMotionFunc(motion);

	glutMainLoop();
	return 0;
}
