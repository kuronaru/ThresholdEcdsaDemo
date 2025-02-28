import ctypes
from ctypes import c_int, c_longlong, POINTER

# 加载DLL
libc = ctypes.CDLL('libc/build/libcecdsa.dll')

# 定义函数的参数和返回类型

# codemath.h 函数
libc.BinPow.argtypes = [c_int, c_int, c_int]
libc.BinPow.restype = c_int

libc.ExGCD.argtypes = [c_int, c_int, POINTER(c_int), POINTER(c_int)]
libc.ExGCD.restype = c_int

libc.ExGcdInv.argtypes = [c_int, c_int]
libc.ExGcdInv.restype = c_int

libc.ModSqrt.argtypes = [c_int, c_int]
libc.ModSqrt.restype = c_int

# Point 结构体
class Point(ctypes.Structure):
    _fields_ = [("x", c_int), ("y", c_int)]

# EllipticCurve 类
class EllipticCurve(ctypes.Structure):
    _fields_ = [("a", c_int), ("b", c_int), ("p", c_int), ("G", Point)]

# 定义 EllipticCurve 方法的参数和返回类型
libc.EllipticCurve_new.argtypes = [c_int]
libc.EllipticCurve_new.restype = POINTER(EllipticCurve)

libc.EllipticCurve_new_with_params.argtypes = [c_int, c_int, c_int, Point]
libc.EllipticCurve_new_with_params.restype = POINTER(EllipticCurve)

libc.EllipticCurve_plusPoint.argtypes = [POINTER(EllipticCurve), Point, Point]
libc.EllipticCurve_plusPoint.restype = Point

libc.EllipticCurve_minusPoint.argtypes = [POINTER(EllipticCurve), Point, Point]
libc.EllipticCurve_minusPoint.restype = Point

libc.EllipticCurve_multiplyPoint.argtypes = [POINTER(EllipticCurve), Point, c_int]
libc.EllipticCurve_multiplyPoint.restype = Point

libc.EllipticCurve_getParamList.argtypes = [POINTER(EllipticCurve)]
libc.EllipticCurve_getParamList.restype = POINTER(c_int)

libc.EllipticCurve_getPointG.argtypes = [POINTER(EllipticCurve)]
libc.EllipticCurve_getPointG.restype = Point

# 示例：使用 EllipticCurve 方法
def create_elliptic_curve(p):
    return libc.EllipticCurve_new(p)

def create_elliptic_curve_with_params(a, b, p, G):
    return libc.EllipticCurve_new_with_params(a, b, p, G)

def elliptic_curve_plus_point(curve, pa, pb):
    return libc.EllipticCurve_plusPoint(curve, pa, pb)

def elliptic_curve_minus_point(curve, pa, pb):
    return libc.EllipticCurve_minusPoint(curve, pa, pb)

def elliptic_curve_multiply_point(curve, pa, k):
    return libc.EllipticCurve_multiplyPoint(curve, pa, k)

def elliptic_curve_get_param_list(curve):
    param_list_ptr = libc.EllipticCurve_getParamList(curve)
    return [param_list_ptr[i] for i in range(3)]  # 假设参数列表有3个元素

def elliptic_curve_get_point_g(curve):
    return libc.EllipticCurve_getPointG(curve)

# BigNum 结构体
class BigNum(ctypes.Structure):
    _fields_ = [("a", c_int * 500), ("len", c_int)]

# 定义 BigNum 函数的参数和返回类型
libc.BinPowBigNum.argtypes = [BigNum, BigNum, BigNum]
libc.BinPowBigNum.restype = BigNum

libc.ExGCDBigNum.argtypes = [BigNum, BigNum, POINTER(BigNum), POINTER(BigNum)]
libc.ExGCDBigNum.restype = BigNum

libc.ExGcdInvBigNum.argtypes = [BigNum, BigNum]
libc.ExGcdInvBigNum.restype = BigNum

libc.FermatInv.argtypes = [c_int, c_int]
libc.FermatInv.restype = c_int

libc.ModSqrtBigNum.argtypes = [BigNum, BigNum]
libc.ModSqrtBigNum.restype = BigNum

# 示例：使用 BigNum 函数
def bin_pow(a, b, p):
    return libc.BinPow(a, b, p)

def ex_gcd(a, b):
    x = c_int()
    y = c_int()
    result = libc.ExGCD(a, b, ctypes.byref(x), ctypes.byref(y))
    return result, x.value, y.value

def ex_gcd_inv(a, b):
    return libc.ExGcdInv(a, b)

def mod_sqrt(a, n):
    return libc.ModSqrt(a, n)

def bin_pow_bignum(a, b, p):
    return libc.BinPowBigNum(a, b, p)

def ex_gcd_bignum(a, b):
    x = BigNum()
    y = BigNum()
    result = libc.ExGCDBigNum(a, b, ctypes.byref(x), ctypes.byref(y))
    return result, x, y

def ex_gcd_inv_bignum(a, b):
    return libc.ExGcdInvBigNum(a, b)

def fermat_inv(a, b):
    return libc.FermatInv(a, b)

def mod_sqrt_bignum(a, n):
    return libc.ModSqrtBigNum(a, n)