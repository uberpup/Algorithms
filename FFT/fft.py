import numpy as np
import math
import matplotlib.pyplot as plt


def analyze_fft(f, title="", D=0):
    x = discretization(10, 10/100)
    plot(x, f(x), "original " + title)
    g = fft(f(x))
    g = g[:int(len(g) * (1 - D))]
    e = fft(g, inverse=True)[:100]
    x = np.ndarray.tolist(x)
    e = np.ndarray.tolist(e)
    plot(x, e, "fourier-transformed " + title, D)


def fft(a, inverse=False, first_iteration=True):
    if len(a) == 1:
        return a
    c = int(math.pow(2, (math.ceil(math.log2(len(a)))))) - len(a)
    a = np.append(a, [0] * c)
    # if not inverse:
    a = np.asarray(a, dtype=complex)
    n = a.shape[0]
    n_arr = np.arange(1, n // 2 + 1, 1)
    if n % 2 > 0 and n != 1:
        raise ValueError("size of x must be a power of 2")
    A_even = fft(a[::2], inverse, first_iteration=False)
    A_odd = fft(a[1::2], inverse, first_iteration=False)
    angle = 2 * np.pi * (-1 if inverse else 1) / n
    root = complex(math.cos(angle), math.sin(angle))
    factor = np.array([root ** i for i in range(n//2)])
    return np.concatenate([(A_even + A_odd * factor) / (n if (inverse and first_iteration) else 1),
                           (A_even - A_odd * factor) / (n if (inverse and first_iteration) else 1)])


def discretization(To, Step):
    A = np.arange(0, To, Step)
    return A


def plot(x, y, title="", coefficients_deleted=0):
    plt.title(title)
    plt.scatter(x, y, s=7)
    plt.grid(True)
    plt.show()


analyze_fft(lambda x: x)
analyze_fft(lambda x: x * x)
analyze_fft(lambda x: np.sin(x))
analyze_fft(lambda x: np.sin(x) / (x + 1e-12))
analyze_fft(lambda x: np.sin(x * x))
