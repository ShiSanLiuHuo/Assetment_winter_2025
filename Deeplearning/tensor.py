import torch

# 1. 创建一个5行3列的随机张量X
X = torch.rand(5, 3)
print("随机张量X:\n", X)

# 2. 用pytorch内置函数检测X的shape，dtype，device
print("X的Shape:", X.shape)
print("X的Dtype:", X.dtype)
print("X的Device:", X.device)

# 3. 直接创建一个[[1,2,3,4,5],[6,7,8,9,10],[11,12,13,14,15]]的张量Y
Y = torch.tensor([[1,2,3,4,5],[6,7,8,9,10],[11,12,13,14,15]])
print("原始张量Y:\n", Y)

# 4. 用任意方法将Y的维度转变为5行3列
Y = Y.view(5, 3)
print("调整后张量Y:\n", Y)

# 5. 实现X和Y的加减乘除
# 加法
addition = torch.add(X, Y)
print("X + Y:\n", addition)

# 减法
subtraction = torch.sub(X, Y)
print("X - Y:\n", subtraction)

# 乘法
multiplication = torch.mul(X, Y)
print("X * Y:\n", multiplication)

# 除法
division = torch.div(X, Y)
print("X / Y:\n", division)

# abs()，sqrt()，neg()，mean()的作用
print("Y的绝对值:\n", torch.abs(Y))
print("Y的平方根:\n", torch.sqrt(Y.float())) # 将Y转换为float类型以计算平方根
print("Y的负数形式:\n", torch.neg(Y))
print("Y的平均值:\n", torch.mean(Y.float()))

# 7. 将张量X转为Numpy格式，再将其转回来。
X_np = X.numpy()
print("转换为Numpy数组的X:\n", X_np)
X_back_to_tensor = torch.from_numpy(X_np)
print("从Numpy转换回的张量X:\n", X_back_to_tensor)

# 8. 将张量X放到cuda上。
if torch.cuda.is_available():
    X_cuda = X.cuda()
    print("放在CUDA上的X:\n", X_cuda)
else:
    print("CUDA不可用")