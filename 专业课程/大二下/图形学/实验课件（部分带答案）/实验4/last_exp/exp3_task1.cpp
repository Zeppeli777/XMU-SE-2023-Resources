#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<GL/glut.h>
#include<math.h>
#define WIDTH 500
#define HEIGHT 500

using namespace std;

typedef struct Coordinate
{
	float x;
	float y;
	float z;
	float nx;
	float ny;
	float nz;
}Coordinate;

typedef struct Face
{
	int vertex1;
	int vertex2;
	int vertex3;
}Face;

Coordinate* coordinates;
Face* face;
int vertexNum, faceNum;
float center_x = 0, center_y = 0, center_z = 0;

void init()
{
	glClearColor(0, 0, 0, 0);
	glEnable(GL_DEPTH_TEST);
}

void LoadFile()
{
	//读取ply文件，并将其显示出来
	FILE* fp = fopen("D:\\lizhenxiout.ply", "r");

	char s[100];
	for (int i = 1; i <= 3; i++)
	{
		fgets(s, 100, fp);
		printf("%s", s);
	}

	//读取有多少个顶点
	fscanf(fp, "element vertex %d\n", &vertexNum);
	for (int i = 1; i <= 6; i++)
	{
		fgets(s, 100, fp);
	}
	//读取有多少个面
	fscanf(fp, "element face %d\n", &faceNum);
	for (int i = 1; i <= 2; ++i)
	{
		fgets(s, 100, fp);
	}

	//读取每个顶点的信息
	coordinates = new Coordinate[vertexNum];
	for (int i = 0; i < vertexNum; i++)
	{
		fscanf(fp, "%f %f %f %f %f %f\n", &coordinates[i].x, &coordinates[i].y, &coordinates[i].z,
			&coordinates[i].nx, &coordinates[i].ny, &coordinates[i].nz);
		center_x += coordinates[i].x;
		center_y += coordinates[i].y;
		center_z += coordinates[i].z;
	}

	center_x /= vertexNum;
	center_y /= vertexNum;
	center_z /= vertexNum;

	//读取每个面的信息
	face = new Face[faceNum];
	for (int i = 0; i < faceNum; ++i)
	{
		fscanf(fp, "3 %d %d %d\n", &face[i].vertex1, &face[i].vertex2, &face[i].vertex3);
	}

	fclose(fp);
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//绘制小人
	glColor3f(1, 0, 0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(center_x, center_y, center_z + 1500, center_x, center_y, center_z, 0, 1, 0);

	for (int i = 0; i < faceNum; i++)
	{
		glBegin(GL_TRIANGLES);
		glVertex3f(coordinates[face[i].vertex1].x, coordinates[face[i].vertex1].y, coordinates[face[i].vertex1].z);
		glVertex3f(coordinates[face[i].vertex2].x, coordinates[face[i].vertex2].y, coordinates[face[i].vertex2].z);
		glVertex3f(coordinates[face[i].vertex3].x, coordinates[face[i].vertex3].y, coordinates[face[i].vertex3].z);
		glEnd();
	}

	glutSwapBuffers();
}

void reshape(int w, int h)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90, 1, 0.1, 2000);
}
void CalculateAndPrintModelInfo() {//计算模型的空间信息
	if (vertexNum == 0) {
		printf("错误：没有顶点数据！\n");
		return;
	}

	// 初始化包围盒极值
	float min_x = coordinates[0].x;
	float max_x = coordinates[0].x;
	float min_y = coordinates[0].y;
	float max_y = coordinates[0].y;
	float min_z = coordinates[0].z;
	float max_z = coordinates[0].z;

	// 遍历所有顶点计算极值
	for (int i = 1; i < vertexNum; ++i) {
		const Coordinate& c = coordinates[i];
		min_x = std::min(min_x, c.x);
		max_x = std::max(max_x, c.x);
		min_y = std::min(min_y, c.y);
		max_y = std::max(max_y, c.y);
		min_z = std::min(min_z, c.z);
		max_z = std::max(max_z, c.z);
	}

	// 计算包围盒尺寸
	const float bbox_length = max_x - min_x;
	const float bbox_width = max_y - min_y;
	const float bbox_height = max_z - min_z;

	// 计算包围盒中心
	const float bbox_center_x = (min_x + max_x) / 2;
	const float bbox_center_y = (min_y + max_y) / 2;
	const float bbox_center_z = (min_z + max_z) / 2;

	// 打印结果
	printf("\n==== 模型空间信息 ====\n");
	printf("包围盒范围：\n");
	printf("X Axis  [%-8.2f ~ %-8.2f]  长度: %-8.2f\n", min_x, max_x, bbox_length);
	printf("Y Axis  [%-8.2f ~ %-8.2f]  宽度: %-8.2f\n", min_y, max_y, bbox_width);
	printf("Z Axis  [%-8.2f ~ %-8.2f]  深度: %-8.2f\n", min_z, max_z, bbox_height);
	printf("\n中心点信息：\n");
	printf("Vertex Avg Center (%-8.2f, %-8.2f, %-8.2f)\n", center_x, center_y, center_z);
	printf("BBox Center (%-8.2f, %-8.2f, %-8.2f)\n", bbox_center_x, bbox_center_y, bbox_center_z);
	printf("==========================\n\n");
}
int main(int argc, char** argv)
{
	SetConsoleOutputCP(CP_UTF8);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(WIDTH, HEIGHT);
	LoadFile();
	CalculateAndPrintModelInfo();
	glutCreateWindow("Exp3 Task1");
	glutDisplayFunc(display);
	init();
	glutReshapeFunc(reshape);
	glutMainLoop();
}