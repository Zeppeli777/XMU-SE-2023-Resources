import random

def estimate_set_size(X, trials=1000):
    total_k = 0
    n_estimates = []
    for _ in range(trials):
        seen = set()
        k = 0
        while True:
            elem = random.choice(list(X))
            if elem in seen:
                total_k += (k + 1)  # 首次重复在 k+1 次
                break
            seen.add(elem)
            k += 1
    avg_k = total_k / trials
    return (2 / 3.1415927) * avg_k ** 2  # π 近似 3.1416

# 测试：假设 X 是含 n 个元素的集合
X = set(range(1000))  # 实际中 X 是未知集合，这里模拟
print(estimate_set_size(X))
