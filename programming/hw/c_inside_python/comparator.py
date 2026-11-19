import ctypes

import numpy

lib = ctypes.CDLL("./libhamming.dylib")

lib.hamming_distance_256.argtypes = [
    ctypes.POINTER(ctypes.c_uint64),  # Q
    ctypes.POINTER(ctypes.c_uint64),  # B
    ctypes.c_size_t,  # N
    ctypes.POINTER(ctypes.c_uint16),  # D
]
lib.hamming_distance_256.restype = None  # void


def hamming_distances(Q: numpy.ndarray, B: numpy.ndarray) -> numpy.ndarray:
    """
    Q: numpy.ndarray de shape (4,) y dtype numpy.uint64 (o (32,) uint8)
    B: numpy.ndarray de shape (N, 4) y dtype numpy.uint64
    Regresa distancias D: numpy.ndarray de shape (N,) y dtype numpy.uint16
    """
    to_compare: numpy.ndarray = numpy.ascontiguousarray(Q, dtype=numpy.uint64)
    base: numpy.ndarray = numpy.ascontiguousarray(B, dtype=numpy.uint64)

    N = base.shape[0]
    D: numpy.ndarray = numpy.empty(N, dtype=numpy.uint16)

    to_compare_ptr = to_compare.ctypes.data_as(ctypes.POINTER(ctypes.c_uint64))
    base_ptr = base.ctypes.data_as(ctypes.POINTER(ctypes.c_uint64))
    distances_ptr = D.ctypes.data_as(ctypes.POINTER(ctypes.c_uint16))

    # Invocar la función en C
    lib.hamming_distance_256(to_compare_ptr, base_ptr, N, distances_ptr)

    return D


def main() -> None:
    # Caso de prueba
    # Q: todo en ceros
    # Candidato 0: idéntico a Q (distancia esperada: 0)
    # Candidato 1: todo en 1s (distancia esperada: 256)
    # Candidato 2: exactamente un bit en 1 (distancia esperada: 1)

    Q: numpy.ndarray = numpy.array([0, 0, 0, 0], dtype=numpy.uint64)
    B = numpy.array(
        [
            [0, 0, 0, 0],
            [
                0xFFFFFFFFFFFFFFFF,
                0xFFFFFFFFFFFFFFFF,
                0xFFFFFFFFFFFFFFFF,
                0xFFFFFFFFFFFFFFFF,
            ],
            [1, 0, 0, 0],
        ],
        dtype=numpy.uint64,
    )

    distances: numpy.ndarray = hamming_distances(Q, B)
    print("Distancias calculadas:", distances.tolist())  # Salida esperada: [0, 256, 1]


main()
