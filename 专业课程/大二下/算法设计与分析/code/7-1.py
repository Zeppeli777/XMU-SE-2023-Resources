import random

def generate_unique_random(n, m):
    result = []
    # 创建 1 到 n 的数字集合
    numbers = set(range(1, n + 1))  
    for _ in range(m):
        # 随机选一个并移除，保证不重复
        num = random.sample(numbers, 1)[0]  
        numbers.remove(num)  
        result.append(num)
    return result

# 假设n=10，m=5
n = 10
m = 5
print(generate_unique_random(n, m))
