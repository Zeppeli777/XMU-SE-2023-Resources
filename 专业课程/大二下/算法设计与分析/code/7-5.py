import random

def monte_carlo_product():
    product = 1.0
    for _ in range(25):
        # 随机选341~365的整数
        x = random.randint(341, 365)  
        product *= x
    return product / (365**25)

# 多次采样取平均（提升精度）
N = 10000
total = 0
for _ in range(N):
    total += monte_carlo_product()
result = total / N
print(f"蒙特卡洛近似结果：{result:.4g}")
