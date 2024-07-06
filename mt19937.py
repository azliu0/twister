"""a python api for mt19937.h."""


def predict(seed, n):
    """predict the n-th random number."""
    return seed * n
