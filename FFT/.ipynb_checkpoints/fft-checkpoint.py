import numpy as np
import matplotlib.pyplot as plt

def fft(a, inverse=False):

    x = np.asarray(x, dtype=float)
    N = x.shape[0]
    if (!inverse):
        return np.fft(x, N)
    else:
        return np.ifft(x, N)
'''
    if np.log2(N) % 1 > 0:
        raise ValueError("size of x must be a power of 2")

    # N_min here is equivalent to the stopping condition above,
    # and should be a power of 2
    N_min = min(N, 32)
    
    # Perform an O[N^2] DFT on all length-N_min sub-problems at once
    n = np.arange(N_min)
    k = n[:, None]
    M = np.exp(-2j * np.pi * n * k / N_min)
    X = np.dot(M, x.reshape((N_min, -1)))

    # build-up each level of the recursive calculation all at once
    while X.shape[0] < N:
        X_even = X[:, :X.shape[1] / 2]
        X_odd = X[:, X.shape[1] / 2:]
        factor = np.exp(-1j * np.pi * np.arange(X.shape[0])
                        / X.shape[0])[:, None]
        X = np.vstack([X_even + factor * X_odd,
                       X_even - factor * X_odd]) 

    return X.ravel() '''

def discretization(f, To, Step):
    A = np.arange(0, To, Step)
    return f(A)

def test(f, coefficients_deleted=0):
    #и тут код
    xs = np.linspace(0, 10, 100);
    plt.plot(xs, f(xs));
#     print(np.sin(xs))
#     print(xs)
    plt.grid(True)
    plt.show()

'''тут новая ячейка, начинаем тестить
   тестим примерно так:
   f = lambda x: x
   test(f, 0)
   для каждой функции делаем по 3+ графиков и какой-то вывод про D после (см. задание)
   текстовые вставки делаем отдельными ячейками, выбирая Markdown вместо Code!!!'''

f = lambda x: x
test(f)
g = fft(discretization(f, 100, 2.5))
e = fft(g, True)
test(e)
# Change part of dots

