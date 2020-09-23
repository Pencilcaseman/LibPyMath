__all__ = ["fibo", "pi"]

def fibo(n):
	return n if n < 2 else fibo(n - 1) + fibo(n - 2)

def pi(its):
	sum = 0
	for i in range(its):
		sum += 1 / (i + 1) ** 2
	return (6 * sum) ** 0.5
