import heapq
import struct
from collections import Counter


class NodoHuffman:
    def __init__(self, char, freq):
        self.char = char
        self.freq = freq
        self.left = None
        self.right = None

    def __lt__(self, other: NodoHuffman):
        return self.freq < other.freq


class CompresorHuffman:
    def __init__(self, ruta_archivo=None):
        self.ruta_archivo = ruta_archivo
        self.codigos = {}
        self.inverso_codigos = {}

    def _calcular_frecuencias(self, texto):
        return Counter(texto)

    def _construir_arbol(self, frecuencias):
        heap = [NodoHuffman(char, freq) for char, freq in frecuencias.items()]
        heapq.heapify(heap)

        # Caso borde: texto con un solo carácter único
        if len(heap) == 1:
            unico = heapq.heappop(heap)
            raiz = NodoHuffman(None, unico.freq)
            raiz.left = unico
            return raiz

        while len(heap) > 1:
            nodo1 = heapq.heappop(heap)
            nodo2 = heapq.heappop(heap)

            combinado = NodoHuffman(None, nodo1.freq + nodo2.freq)
            combinado.left = nodo1
            combinado.right = nodo2

            heapq.heappush(heap, combinado)

        return heap[0] if heap else None

    def _generar_codigos_rec(self, nodo, codigo_actual):
        if nodo is None:
            return

        if nodo.char is not None:
            # Si el árbol solo tenía un símbolo, asegurar que el código no sea vacío
            self.codigos[nodo.char] = codigo_actual if codigo_actual else "0"
            self.inverso_codigos[self.codigos[nodo.char]] = nodo.char
            return

        self._generar_codigos_rec(nodo.left, codigo_actual + "0")
        self._generar_codigos_rec(nodo.right, codigo_actual + "1")

    def _serializar_arbol(self, nodo):
        """Serializa la estructura del árbol en pre-orden:
        0 representa nodo interno.
        1 seguido del byte del carácter representa una hoja.
        """
        bits = []
        bytes_caracteres = bytearray()

        def _recorrido(n):
            if n.char is not None:
                bits.append("1")
                # Se codifica el carácter en UTF-8
                char_encoded = n.char.encode("utf-8")
                bytes_caracteres.append(len(char_encoded))
                bytes_caracteres.extend(char_encoded)
            else:
                bits.append("0")
                _recorrido(n.left)
                _recorrido(n.right)

        _recorrido(nodo)
        bits_str = "".join(bits)

        # Alineación a bytes del mapa de bits del árbol
        pad_bits = (8 - len(bits_str) % 8) % 8
        bits_str += "0" * pad_bits
        bytes_estructura = bytearray(
            int(bits_str[i : i + 8], 2) for i in range(0, len(bits_str), 8)
        )

        return pad_bits, bytes_estructura, bytes_caracteres

    def _deserializar_arbol(self, bits_iter, iter_caracteres):
        bit = next(bits_iter)
        if bit == "1":
            longitud = next(iter_caracteres)
            raw_char = bytes(next(iter_caracteres) for _ in range(longitud))
            char = raw_char.decode("utf-8")
            return NodoHuffman(char, 0)

        nodo = NodoHuffman(None, 0)
        nodo.left = self._deserializar_arbol(bits_iter, iter_caracteres)
        nodo.right = self._deserializar_arbol(bits_iter, iter_caracteres)
        return nodo

    def comprimir(self, ruta_salida):
        with open(self.ruta_archivo, "r", encoding="utf-8") as f:
            texto = f.read()

        if not texto:
            with open(ruta_salida, "wb") as salida:
                salida.write(b"")
            return

        frecuencias = self._calcular_frecuencias(texto)
        raiz = self._construir_arbol(frecuencias)
        self.codigos.clear()
        self.inverso_codigos.clear()
        self._generar_codigos_rec(raiz, "")

        # 1 Serializar el árbol
        pad_arbol, bytes_est, bytes_chars = self._serializar_arbol(raiz)

        # 2 Codificar el cuerpo del texto
        texto_codificado = "".join(self.codigos[char] for char in texto)
        pad_texto = (8 - len(texto_codificado) % 8) % 8
        texto_relleno = texto_codificado + ("0" * pad_texto)
        bytes_datos = bytearray(
            int(texto_relleno[i : i + 8], 2) for i in range(0, len(texto_relleno), 8)
        )

        # 3 Empaquetar encabezados y cuerpo
        # Formato de cabecera:
        # [1B pad_arbol][1B pad_texto][4B len(bytes_est)][4B len(bytes_chars)]
        with open(ruta_salida, "wb") as salida:
            salida.write(
                struct.pack(
                    ">BBI I", pad_arbol, pad_texto, len(bytes_est), len(bytes_chars)
                )
            )
            salida.write(bytes_est)
            salida.write(bytes_chars)
            salida.write(bytes_datos)

        print(f"Archivo comprimido con árbol embebido en: {ruta_salida}")

    def descomprimir(self, ruta_comprimido, ruta_salida):
        with open(ruta_comprimido, "rb") as f:
            contenido = f.read()

        if not contenido:
            with open(ruta_salida, "w", encoding="utf-8") as salida:
                salida.write("")
            return

        # 1 Desempaquetar cabecera fija de 10 bytes
        pad_arbol, pad_texto, len_est, len_chars = struct.unpack(
            ">BBI I", contenido[:10]
        )
        offset = 10

        bytes_est = contenido[offset : offset + len_est]
        offset += len_est
        bytes_chars = contenido[offset : offset + len_chars]
        offset += len_chars
        bytes_datos = contenido[offset:]

        # 2 Reconstruir flujo de bits del árbol
        bits_est_str = "".join(f"{b:08b}" for b in bytes_est)
        if pad_arbol > 0:
            bits_est_str = bits_est_str[:-pad_arbol]

        # 3 Reconstruir el árbol
        iter_bits_arbol = iter(bits_est_str)
        iter_chars = iter(bytes_chars)
        raiz = self._deserializar_arbol(iter_bits_arbol, iter_chars)

        # 4 Decodificar el texto con el árbol recuperado
        bits_datos_str = "".join(f"{b:08b}" for b in bytes_datos)
        if pad_texto > 0:
            bits_datos_str = bits_datos_str[:-pad_texto]

        texto_decodificado = []
        nodo_actual = raiz
        for bit in bits_datos_str:
            nodo_actual = nodo_actual.left if bit == "0" else nodo_actual.right
            if nodo_actual.char is not None:
                texto_decodificado.append(nodo_actual.char)
                nodo_actual = raiz

        resultado = "".join(texto_decodificado)
        with open(ruta_salida, "w", encoding="utf-8") as salida:
            salida.write(resultado)

        print(f"Archivo descomprimido: {ruta_salida}")


def main() -> None:
    compressor: CompresorHuffman = CompresorHuffman("./message.txt")
    # compressor.comprimir("compressed_message.bin")
    compressor.descomprimir("compressed_message.bin", "recovered.txt")


main()
