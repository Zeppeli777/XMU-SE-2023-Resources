#include <Eigen/Core>
#include "ppm.hpp"
#include "camera.hpp"
#include "raytracer.hpp"
#include "scene.hpp" 
#include <iostream>
#include <string>
#include <vector>
#include <utility> // 用于 std::pair
#include <cmath>   // 用于 std::abs
#include <cstdio>  // 用于 sprintf
#include <cstdlib> // 用于 rand, srand
#include <ctime>   // 用于 srand 的时间

using namespace std;
using namespace Eigen;

// 用于生成 [0, 1] 范围内的随机浮点数（后续可能用于运动模糊/软阴影）
float random_float_01() {
	return static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
}

int main()
{
	srand(static_cast<unsigned int>(time(nullptr))); // 初始化随机数种子

	int width = 640;  
	int height = 480; 
	Camera cam;

	// 静态球体（sp1 和 sp2 是动态处理的）
	Sphere sp3_static(Vector3f(-2.0, 0, -2), 0.5);
	Sphere sp4_static(Vector3f(1.0, -0.2, -1.0), 0.3);
	Sphere sp5_static(Vector3f(3.0, 0, -2.2), 0.4);


	// 材质定义
	Material mtl_sp1, mtl_sp2_ground, mtl_sp3, mtl_sp4, mtl_sp5, mtl_sp6;

	// sp1 的材质（中间黄色球体）
	mtl_sp1.SetKa(Vector3f(0.5, 0.5, 0.5));
	mtl_sp1.SetKd(Vector3f(0.8, 0.6, 0.0));
	mtl_sp1.SetKs(Vector3f(0.7, 0.8, 0.8));
	mtl_sp1.SetTransparent(false);
	mtl_sp1.SetReflective(true);
	mtl_sp1.SetShiness(50);

	// sp2 的材质（地面球体）
	mtl_sp2_ground.SetKa(Vector3f(0.5, 0.5, 0.5)); // 可见地面
	mtl_sp2_ground.SetKd(Vector3f(0.2, 0.4, 0.1)); // 暗绿色/棕色
	mtl_sp2_ground.SetKs(Vector3f(0.1, 0.1, 0.1));
	mtl_sp2_ground.SetTransparent(false);
	mtl_sp2_ground.SetReflective(false);
	mtl_sp2_ground.SetShiness(10);

	// sp3 的材质（蓝色小球）
	mtl_sp3.SetKa(Vector3f(0.5, 0.5, 0.5));
	mtl_sp3.SetKd(Vector3f(0.0, 0.3, 0.6));
	mtl_sp3.SetKs(Vector3f(0.0, 0.0, 0.0));
	mtl_sp3.SetTransparent(false);
	mtl_sp3.SetReflective(false);
	mtl_sp3.SetShiness(10);

	// sp4 的材质（右侧黑色球体 - 透明）
	mtl_sp4.SetKa(Vector3f(0.1, 0.1, 0.1));
	mtl_sp4.SetKd(Vector3f(0.0, 0.0, 0.0));
	mtl_sp4.SetKs(Vector3f(0.0, 0.0, 0.0));
	mtl_sp4.SetTransparent(true);
	mtl_sp4.SetReflective(false);
	mtl_sp4.SetRefraction(1.33);
	mtl_sp4.SetShiness(200);

	// sp5 的材质（右侧黄色球体）
	mtl_sp5.SetKa(Vector3f(0.5, 0.5, 0.5));
	mtl_sp5.SetKd(Vector3f(0.75, 0.56, 0.0));
	mtl_sp5.SetKs(Vector3f(0.75, 0.56, 0.0));
	mtl_sp5.SetTransparent(false);
	mtl_sp5.SetReflective(false);
	// mtl_sp5.SetRefraction(1.33); 
	mtl_sp5.SetShiness(100);



	// 动画和物理参数
	int total_frames = 120; // 总帧数
	float fps = 24.0f; // 帧率
	float time_per_frame = 1.0f / fps; // 每帧的时间

	// sp1（自由落体球体）的参数
	Vector3f sp1_initial_static_pos(0, 0.5, -2); // 初始位置
	float sp1_radius = 1.0f;
	Vector3f sp1_current_pos = sp1_initial_static_pos;
	sp1_current_pos.y() = 5.0f; // 从更高位置开始下落
	Vector3f sp1_velocity(0, 0, 0); // 初始速度为 0
	float gravity_y = -9.8f;       // 重力加速度
	float restitution = 0.75f;     // 弹性系数（0 到 1）

	// sp2（地面球体）的参数
	Vector3f sp2_ground_center(0, -10000.5, -2);
	float sp2_ground_radius = 10000.0f;
	float ground_contact_y = sp2_ground_center.y() + sp2_ground_radius; // 地面 y 坐标

	// 开始逐帧渲染
	for (int frame = 0; frame < total_frames; ++frame)
	{
		cout << "渲染帧 " << frame << "/" << total_frames << "..." << endl;

		// 更新 sp1 的物理状态
		sp1_velocity.y() += gravity_y * time_per_frame;
		sp1_current_pos += sp1_velocity * time_per_frame;

		// 检测与地面的碰撞
		if ((sp1_current_pos.y() - sp1_radius) < ground_contact_y)
		{
			sp1_current_pos.y() = ground_contact_y + sp1_radius; // 修正位置使其接触地面
			sp1_velocity.y() *= -restitution;                    // 反向并减少速度（反弹）

			// 如果速度很小并且几乎贴地，则停止弹跳
			if (std::abs(sp1_velocity.y()) < 0.1f && (sp1_current_pos.y() - sp1_radius - ground_contact_y) < 0.01f) {
				sp1_velocity.y() = 0;
			}
		}

		// 构建当前帧的场景
		Scene world_this_frame;
		Sphere sp1_dynamic(sp1_current_pos, sp1_radius);
		Sphere sp2_ground_dynamic(sp2_ground_center, sp2_ground_radius);

		world_this_frame.Insert(make_pair(sp1_dynamic, mtl_sp1));
		world_this_frame.Insert(make_pair(sp2_ground_dynamic, mtl_sp2_ground));
		world_this_frame.Insert(make_pair(sp3_static, mtl_sp3));
		world_this_frame.Insert(make_pair(sp4_static, mtl_sp4));
		world_this_frame.Insert(make_pair(sp5_static, mtl_sp5));
		

		// 渲染图像
		PPM outrlt(width, height);
		for (int j = height - 1; j >= 0; --j)
		{
			for (int i = 0; i < width; ++i)
			{
				float u = float(i) / float(width);
				float v = float(j) / float(height);
				Ray ray = cam.GenerateRay(u, v);
				Vector3f color = RayColor(ray, world_this_frame, 0); // 初始递归深度为 0

				outrlt.SetPixel(j, i,
					int(255.99 * color[0]), int(255.99 * color[1]), int(255.99 * color[2]));
			}
		}
		char filename_buffer[100];
		sprintf_s(filename_buffer, "frame_%04d.ppm", frame); // 生成帧文件名
		outrlt.Write2File(filename_buffer); // 保存图像到文件
	}
	cout << "渲染完成" << endl;
	return 0;
}
