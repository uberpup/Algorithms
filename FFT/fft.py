import numpy as np
import math
import matplotlib.pyplot as plt


def analyze_fft(f, title="", D=0):
    # test(f, "original " + title)
    x = discretization(10, 10/100)
    plot(x, f(x), "original " + title)
    g = fft(f(x))
    e = fft(g, inverse=True)
    x = np.ndarray.tolist(x)
    e = np.ndarray.tolist(e)
    plot(x, e, "fourier-transformed " + title, D)


def fft(a, inverse=False):
    if not inverse:
        a = np.asarray(a, dtype=float)
        n = a.shape[0]
        if n % 2 > 0 and n != 1:
            raise ValueError("size of x must be a power of 2")
        if n <= 32:
            n_arr = np.arange(n)
            k = n_arr.reshape((n, 1))
            M = np.exp(-2j * np.pi * k * n_arr / n)
            return np.dot(M, a)
        else:
            A_even = fft(a[::2])
            A_odd = fft(a[1::2])
            factor = np.exp(-2j * np.pi * np.arange(n) / n)
            return np.concatenate([A_even + factor[:n // 2] * A_odd,
                                   A_even + factor[n // 2:] * A_odd])
    else:
        return np.fft.ifft(a)


def discretization(To, Step):
    A = np.arange(0, To, Step)
    c = int(math.pow(2, (math.ceil(math.log2(len(A)))))) - len(A)
    A = np.append(A, [0] * c)
    return A


def plot(x, y, title="", coefficients_deleted=0):
    plt.title(title)
    for i in range(int(math.floor(len(y) - len(y) * coefficients_deleted)), len(y)):
        y[i] = 0
    plt.scatter(x, y, s=7)
    plt.grid(True)
    plt.show()


analyze_fft(lambda x: x)
analyze_fft(lambda x: x * x)
analyze_fft(lambda x: np.sin(x))
analyze_fft(lambda x: np.sin(x) / (x + 1e-12))
analyze_fft(lambda x: np.sin(x * x))
