﻿#define _CRT_SECURE_NO_WARNINGS
#include <GL/glut.h>
#include <cstdio>
#include <cmath>

typedef struct Vertex_3D//顶点结构体
{
    float x;
    float y;
    float z;
} VERTEX3D;

//定义一个三角形结构体
typedef struct _Triangle
{
    int A, B, C;//三个顶点的索引
} TRIANGLE;

VERTEX3D* vet;
VERTEX3D* norm;
VERTEX3D* color;


TRIANGLE* triPatch;

float rotatex = 0.0f, rotatey = 0.0f, rotatez = 0.0f;
float modelPos[3] = { 0, 0, -15.0f };
float scale = 5.0f;

const float lightPos[] = { 1.2f, 1.2f, 1.2f, 0.2f };

char strModelName[512] = "lizhenxiout.ply";//ply文件路径

int nVerticesNumber;//顶点数
int nFacesNumber;//面数

//归一化
void Normalize(VERTEX3D& v)
{
    double length = sqrt(v.y * v.y + v.z * v.z + v.x * v.x);

    if (length != 0)
    {
        v.y = v.y / length;
        v.z = v.z / length;
        v.x = v.x / length;
    }
}

//获取最小最大值
void GetMinMax(VERTEX3D& vMin, VERTEX3D& vMax, VERTEX3D v)
{

    if (v.y < vMin.y) vMin.y = v.y;
    if (v.z < vMin.z) vMin.z = v.z;
    if (v.x < vMin.x) vMin.x = v.x;
    if (v.y > vMax.y) vMax.y = v.y;
    if (v.z > vMax.z) vMax.z = v.z;
    if (v.x > vMax.x) vMax.x = v.x;
}

//格式化几何
void FormatGeometry(VERTEX3D& v, VERTEX3D vMin, VERTEX3D vMax, float scale)
{
    v.y = scale * (v.y - 0.5 * (vMin.y + vMax.y));
    v.z = scale * (v.z - 0.5 * (vMin.z + vMax.z));
    v.x = scale * (v.x - 0.5 * (vMin.x + vMax.x));
}

//加载PLY文件
void LoadMeshPLY(char* FileName)
{
    int i, j;
    VERTEX3D vMin, vMax;
    char strBuff[512];
    char type[512] = "";
    bool bStartPropertyCount = true;//用于判断开始统计属性的位置
    float alpha;

    vMin.x = vMin.y = vMin.z = 99888.0;
    vMax.x = vMax.y = vMax.z = -99888.0;

    FILE* fp;
    errno_t err = fopen_s(&fp, FileName, "r");

    if (fp == NULL)//文件不存在的情况
    {
        printf("ERROR: unable to open model [%s]!\n", FileName);
        exit(0);
    }
    //读取文件
    fgets(strBuff, 256, fp);
    fgets(strBuff, 256, fp);
    fgets(strBuff, 256, fp);
    fscanf(fp, "element vertex %d\n", &nVerticesNumber);
    j = 0;
    while (strcmp(type, "element") != 0)
    {
        fgets(strBuff, 256.9, fp);
        i = 0;
        while (strBuff[i] != '\0' && strBuff[i] != ' ' && strBuff[i] != '\n')
        {
            type[i] = strBuff[i];
            i++;
        }
        if (bStartPropertyCount && strcmp(type, "property") == 0)
            j++;
        else
            bStartPropertyCount = false;
        type[i] = '\0';
    }
    sscanf(strBuff, "element face %d\n", &nFacesNumber);
    printf("Number of Triangles: %d\n", nFacesNumber);

    fgets(strBuff, 256, fp);
    fgets(strBuff, 256, fp);

    //分配内存空间
    vet = new VERTEX3D[nVerticesNumber];
    norm = new VERTEX3D[nVerticesNumber];
    color = new VERTEX3D[nVerticesNumber];

    triPatch = new TRIANGLE[nFacesNumber];

    //读取顶点数据
    for (i = 0; i < nVerticesNumber; i++)
    {
        if (j == 3)
            fscanf(fp, "%f %f %f", &vet[i].x, &vet[i].y, &vet[i].z);
        else if (j == 6)
            fscanf(fp, "%f %f %f %f %f %f", &vet[i].x, &vet[i].y, &vet[i].z,
                &norm[i].x, &norm[i].y, &norm[i].z);
        else if (j == 7)
            fscanf(fp, "%f %f %f %f %f %f %f", &vet[i].x, &vet[i].y, &vet[i].z,
                &color[i].x, &color[i].y, &color[i].z,
                &alpha);
        else if (j == 10)
            fscanf(fp, "%f %f %f %f %f %f %f %f %f %f", &vet[i].x, &vet[i].y, &vet[i].z,
                &norm[i].x, &norm[i].y, &norm[i].z,
                &color[i].x, &color[i].y, &color[i].z, &alpha);
        else
        {
            printf("Warning: the viewer cann't read the colors of models\n");
            exit(1);
        }
        GetMinMax(vMin, vMax, vet[i]);
        Normalize(norm[i]);
    }

    int tmp;
    for (i = 0; i < nFacesNumber; i++)
    {
        fscanf(fp, "%d %d %d %d", &tmp, &triPatch[i].A, &triPatch[i].B, &triPatch[i].C);
    }

    if ((vMax.x - vMin.x) > (vMax.y - vMin.y))
    {
        if ((vMax.x - vMin.x) > (vMax.z - vMin.z))
            scale = scale / (vMax.x - vMin.x);
        else
            scale = scale / (vMax.z - vMin.z);
    }
    else
    {
        if ((vMax.y - vMin.y) > (vMax.z - vMin.z))
            scale = scale / (vMax.y - vMin.y);
        else
            scale = scale / (vMax.z - vMin.z);
    }

    for (i = 0; i < nVerticesNumber; i++)
        FormatGeometry(vet[i], vMin, vMax, scale);

    fclose(fp);
}

//绘制模型
void DrawVertices()
{
    glPointSize(1.0);
    glBegin(GL_TRIANGLES);
    for (int i = 0; i < nFacesNumber; i++)
    {
        glBegin(GL_TRIANGLES);

        // 设置小人的颜色
        glColor3f(0.0f, 1.0f, 0.0f);

        // 绘制小人的三角形
        glNormal3f(norm[triPatch[i].A].x, norm[triPatch[i].A].y, norm[triPatch[i].A].z);
        glVertex3f(vet[triPatch[i].A].x, vet[triPatch[i].A].y, vet[triPatch[i].A].z);

        glNormal3f(norm[triPatch[i].B].x, norm[triPatch[i].B].y, norm[triPatch[i].B].z);
        glVertex3f(vet[triPatch[i].B].x, vet[triPatch[i].B].y, vet[triPatch[i].B].z);

        glNormal3f(norm[triPatch[i].C].x, norm[triPatch[i].C].y, norm[triPatch[i].C].z);
        glVertex3f(vet[triPatch[i].C].x, vet[triPatch[i].C].y, vet[triPatch[i].C].z);

        glEnd();
    }
    glEnd();
}

static void resize(int width, int height)
{
    const float ar = (float)width / (float)height;

    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, ar, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

static void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();

    glTranslatef(modelPos[0], modelPos[1], modelPos[2]);

    DrawVertices();

    glutSwapBuffers();
}

static void idle(void)
{
    glutPostRedisplay();
}

int main(int argc, char* argv[])
{
    glutInit(&argc, argv);//初始化GLUT
    glutInitWindowPosition(0, 0);//设置窗口位置
    glutInitWindowSize(1010, 1010);//设置窗口大小
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow("PLY Model");//创建窗口("窗口标题")
    glutReshapeFunc(resize);//设置应用程序的窗口大小的变化
    glutDisplayFunc(display);
    glutIdleFunc(idle);
    glClearColor(0.0, 0.0, 0.0, 1); // 设置背景色为黑色
    glDisable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glShadeModel(GL_SMOOTH);
    LoadMeshPLY(strModelName);
    glutMainLoop();//使用GLUT事件处理循环

    return EXIT_SUCCESS;//返回EXIT_SUCCESS
}