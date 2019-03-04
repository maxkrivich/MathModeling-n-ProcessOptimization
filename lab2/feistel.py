#!/bin/python3.7
"""
Feistel network implementation
Maksym Kryvych
17.02.2019
Task №8
"""

import typing


# Cipher params
ROUNDS = 10
BLOCK_SIZE = 2  # bytes

# Sbox
T1 = bytes([0x2, 0x5, 0x1, 0x9, 0xE, 0xA, 0xC, 0x8,
      0x6, 0x4, 0x7, 0xF, 0xD, 0xB, 0x0, 0x3])

# Master key
K = 0x89AE  # '1000100110101110'


def round_keys(key: int) -> typing.List[bytes]:
    _key = bin(key)[2:]
    keys = [bytes([key & 0xFF])]

    for rot in range(2, ROUNDS+1):
        tmp_key = _key[-(rot-1):] + _key[:-(rot-1)]
        keys.append(bytes([int(tmp_key, 2) & 0xFF]))

    return keys


def bytes_xor(b1: bytes, b2: bytes) -> bytes:
    result = []
    for bit1, bit2 in zip(b1, b2):
        result.append(bytes([bit1 ^ bit2]))
    return b''.join(result)


def round_func(a: bytes, b: bytes) -> bytes:
    return T1[bytes_xor(a, b)[0] % 16]


def split_into_blocks(message: str) -> typing.Union[bytes, bytes]:
    message = bytes(message, 'utf-8')
    for i in range(0, len(message), BLOCK_SIZE):
        block = message[i: i + BLOCK_SIZE]
        if len(block) == BLOCK_SIZE:
            yield block[:BLOCK_SIZE//2], block[BLOCK_SIZE//2:]
        else:
            block = block + b' '*(BLOCK_SIZE - len(block))
            yield block[:BLOCK_SIZE//2], block[BLOCK_SIZE//2:]


def encrypt(msg: str, keys: typing.List[bytes]) -> bytes:
    gen = split_into_blocks(msg)
    result = []
    l, r = next(gen)
    try:
        while True:
            for i in range(ROUNDS):
                l, r = r, bytes_xor(l, bytes([round_func(r, keys[i])]))
            result.extend([r, l])
            l, r = next(gen)
    except StopIteration:
        return b''.join(result)


def decrypt(msg: str, keys: typing.List[bytes]) -> bytes:
    return encrypt(msg, list(reversed(keys))).strip()


def main():
    keys = round_keys(K)
    print(f"Rounds: {ROUNDS}\nBlocksize: {BLOCK_SIZE} bytes\nMaster key: {hex(K)}")
    print("Round keys:")
    print(', '.join([key.hex() for key in keys]))
    print("S-block:")
    print(', '.join(hex(t) for t in T1))
    print('-'*25)

    msg = input('Input text: ')  # 'Test Feistel network cipher'
    enc = encrypt(msg, keys).decode('utf-8')
    print(f'Cipher text: {enc}')
    dec = decrypt(enc, keys).decode('utf-8')
    print(f'Paintext: {dec}')
    assert msg == dec

if __name__ == '__main__':
    main()
