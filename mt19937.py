"""Python api for mt19937.h."""

import ctypes
import os

if not os.path.exists("./libmt19937.so"):
    raise FileNotFoundError(
        "libmt19937.so not found. build the library first with `make`"
    )

lib = ctypes.CDLL("./libmt19937.so")


class MT19937State(ctypes.Structure):
    """Manages state for the twister."""

    _fields_ = [("mt", ctypes.c_uint32 * 624), ("index", ctypes.c_int)]


lib.seed_rand.argtypes = [ctypes.POINTER(MT19937State), ctypes.c_uint32]
lib.seed_rand.restype = None

lib.gen_rand.argtypes = [ctypes.POINTER(MT19937State)]
lib.gen_rand.restype = ctypes.c_uint32

lib.gen_rand_no_state_update.argtypes = [ctypes.POINTER(MT19937State)]
lib.gen_rand_no_state_update.restype = ctypes.c_uint32

lib.predict.argtypes = [
    ctypes.POINTER(MT19937State),
    ctypes.POINTER(ctypes.c_uint32),
]
lib.predict.restype = ctypes.c_uint32


class MT19937:
    """MT19937 class."""

    def __init__(self):
        """Init mt19937."""
        self.rand = MT19937State()

    def seed_rand(self, seed: int):
        """Seed mt19937."""
        if seed == 0:
            raise ValueError("Seed must not be 0")
        lib.seed_rand(ctypes.byref(self.rand), seed)

    def gen_rand(self):
        """Generate random number."""
        return lib.gen_rand(ctypes.byref(self.rand))

    def gen_rand_no_state_update(self):
        """Generate random number without state update."""
        return lib.gen_rand_no_state_update(ctypes.byref(self.rand))

    def predict(self, nums: list[int]) -> int:
        """Predict the next number in the sequence.

        This is p much the only method that matters.

        Args:
            nums: the last 624 numbers in the sequence, should all fit in uint32

        Returns:
            int: the next number in the sequence
        """
        if len(nums) != 624:
            raise ValueError("nums must be an array of 624 numbers")
        nums_array = (ctypes.c_uint32 * 624)(*nums)
        return lib.predict(ctypes.byref(self.rand), nums_array)
