/* This file is part of the dynarmic project.
 * Copyright (c) 2018 MerryMage
 * This software may be used and distributed according to the terms of the GNU
 * General Public License version 2 or any later version.
 */

#include <array>

#include "common/aes.h"
#include "common/common_types.h"

namespace Dynarmic::Common {

using SubstitutionTable = std::array<u8, 256>;

// See section 5.1.1 Figure 7 in FIPS 197
constexpr SubstitutionTable substitution_box{{
    // 0   1     2     3     4     5     6     7     8     9     A     B     C     D     E     F
    0x63, 0x7C, 0x77, 0x7B, 0xF2, 0x6B, 0x6F, 0xC5, 0x30, 0x01, 0x67, 0x2B, 0xFE, 0xD7, 0xAB, 0x76,
    0xCA, 0x82, 0xC9, 0x7D, 0xFA, 0x59, 0x47, 0xF0, 0xAD, 0xD4, 0xA2, 0xAF, 0x9C, 0xA4, 0x72, 0xC0,
    0xB7, 0xFD, 0x93, 0x26, 0x36, 0x3F, 0xF7, 0xCC, 0x34, 0xA5, 0xE5, 0xF1, 0x71, 0xD8, 0x31, 0x15,
    0x04, 0xC7, 0x23, 0xC3, 0x18, 0x96, 0x05, 0x9A, 0x07, 0x12, 0x80, 0xE2, 0xEB, 0x27, 0xB2, 0x75,
    0x09, 0x83, 0x2C, 0x1A, 0x1B, 0x6E, 0x5A, 0xA0, 0x52, 0x3B, 0xD6, 0xB3, 0x29, 0xE3, 0x2F, 0x84,
    0x53, 0xD1, 0x00, 0xED, 0x20, 0xFC, 0xB1, 0x5B, 0x6A, 0xCB, 0xBE, 0x39, 0x4A, 0x4C, 0x58, 0xCF,
    0xD0, 0xEF, 0xAA, 0xFB, 0x43, 0x4D, 0x33, 0x85, 0x45, 0xF9, 0x02, 0x7F, 0x50, 0x3C, 0x9F, 0xA8,
    0x51, 0xA3, 0x40, 0x8F, 0x92, 0x9D, 0x38, 0xF5, 0xBC, 0xB6, 0xDA, 0x21, 0x10, 0xFF, 0xF3, 0xD2,
    0xCD, 0x0C, 0x13, 0xEC, 0x5F, 0x97, 0x44, 0x17, 0xC4, 0xA7, 0x7E, 0x3D, 0x64, 0x5D, 0x19, 0x73,
    0x60, 0x81, 0x4F, 0xDC, 0x22, 0x2A, 0x90, 0x88, 0x46, 0xEE, 0xB8, 0x14, 0xDE, 0x5E, 0x0B, 0xDB,
    0xE0, 0x32, 0x3A, 0x0A, 0x49, 0x06, 0x24, 0x5C, 0xC2, 0xD3, 0xAC, 0x62, 0x91, 0x95, 0xE4, 0x79,
    0xE7, 0xC8, 0x37, 0x6D, 0x8D, 0xD5, 0x4E, 0xA9, 0x6C, 0x56, 0xF4, 0xEA, 0x65, 0x7A, 0xAE, 0x08,
    0xBA, 0x78, 0x25, 0x2E, 0x1C, 0xA6, 0xB4, 0xC6, 0xE8, 0xDD, 0x74, 0x1F, 0x4B, 0xBD, 0x8B, 0x8A,
    0x70, 0x3E, 0xB5, 0x66, 0x48, 0x03, 0xF6, 0x0E, 0x61, 0x35, 0x57, 0xB9, 0x86, 0xC1, 0x1D, 0x9E,
    0xE1, 0xF8, 0x98, 0x11, 0x69, 0xD9, 0x8E, 0x94, 0x9B, 0x1E, 0x87, 0xE9, 0xCE, 0x55, 0x28, 0xDF,
    0x8C, 0xA1, 0x89, 0x0D, 0xBF, 0xE6, 0x42, 0x68, 0x41, 0x99, 0x2D, 0x0F, 0xB0, 0x54, 0xBB, 0x16
}};

// See section 5.3.2 Figure 14 in FIPS 197
constexpr SubstitutionTable inverse_substitution_box{{
    // 0   1     2     3     4     5     6     7     8     9     A     B     C     D     E     F
    0x52, 0x09, 0x6A, 0xD5, 0x30, 0x36, 0xA5, 0x38, 0xBF, 0x40, 0xA3, 0x9E, 0x81, 0xF3, 0xD7, 0xFB,
    0x7C, 0xE3, 0x39, 0x82, 0x9B, 0x2F, 0xFF, 0x87, 0x34, 0x8E, 0x43, 0x44, 0xC4, 0xDE, 0xE9, 0xCB,
    0x54, 0x7B, 0x94, 0x32, 0xA6, 0xC2, 0x23, 0x3D, 0xEE, 0x4C, 0x95, 0x0B, 0x42, 0xFA, 0xC3, 0x4E,
    0x08, 0x2E, 0xA1, 0x66, 0x28, 0xD9, 0x24, 0xB2, 0x76, 0x5B, 0xA2, 0x49, 0x6D, 0x8B, 0xD1, 0x25,
    0x72, 0xF8, 0xF6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xD4, 0xA4, 0x5C, 0xCC, 0x5D, 0x65, 0xB6, 0x92,
    0x6C, 0x70, 0x48, 0x50, 0xFD, 0xED, 0xB9, 0xDA, 0x5E, 0x15, 0x46, 0x57, 0xA7, 0x8D, 0x9D, 0x84,
    0x90, 0xD8, 0xAB, 0x00, 0x8C, 0xBC, 0xD3, 0x0A, 0xF7, 0xE4, 0x58, 0x05, 0xB8, 0xB3, 0x45, 0x06,
    0xD0, 0x2C, 0x1E, 0x8F, 0xCA, 0x3F, 0x0F, 0x02, 0xC1, 0xAF, 0xBD, 0x03, 0x01, 0x13, 0x8A, 0x6B,
    0x3A, 0x91, 0x11, 0x41, 0x4F, 0x67, 0xDC, 0xEA, 0x97, 0xF2, 0xCF, 0xCE, 0xF0, 0xB4, 0xE6, 0x73,
    0x96, 0xAC, 0x74, 0x22, 0xE7, 0xAD, 0x35, 0x85, 0xE2, 0xF9, 0x37, 0xE8, 0x1C, 0x75, 0xDF, 0x6E,
    0x47, 0xF1, 0x1A, 0x71, 0x1D, 0x29, 0xC5, 0x89, 0x6F, 0xB7, 0x62, 0x0E, 0xAA, 0x18, 0xBE, 0x1B,
    0xFC, 0x56, 0x3E, 0x4B, 0xC6, 0xD2, 0x79, 0x20, 0x9A, 0xDB, 0xC0, 0xFE, 0x78, 0xCD, 0x5A, 0xF4,
    0x1F, 0xDD, 0xA8, 0x33, 0x88, 0x07, 0xC7, 0x31, 0xB1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xEC, 0x5F,
    0x60, 0x51, 0x7F, 0xA9, 0x19, 0xB5, 0x4A, 0x0D, 0x2D, 0xE5, 0x7A, 0x9F, 0x93, 0xC9, 0x9C, 0xEF,
    0xA0, 0xE0, 0x3B, 0x4D, 0xAE, 0x2A, 0xF5, 0xB0, 0xC8, 0xEB, 0xBB, 0x3C, 0x83, 0x53, 0x99, 0x61,
    0x17, 0x2B, 0x04, 0x7E, 0xBA, 0x77, 0xD6, 0x26, 0xE1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0C, 0x7D
}};

// See section 4.2.1 in FIPS 197.
static constexpr u8 xtime(u8 x) {
    return static_cast<u8>((x << 1) ^ (((x >> 7) & 1) * 0x1B));
}

// Galois Field multiplication.
static constexpr u8 Multiply(u8 x, u8 y) {
    return static_cast<u8>(((y & 1) * x) ^
            ((y >> 1 & 1) * xtime(x)) ^
            ((y >> 2 & 1) * xtime(xtime(x))) ^
            ((y >> 3 & 1) * xtime(xtime(xtime(x)))) ^
            ((y >> 4 & 1) * xtime(xtime(xtime(xtime(x))))));
}

static void ShiftRows(AESState& out_state, const AESState& state) {
    // Move zeroth row over
    out_state[0] = state[0];
    out_state[4] = state[4];
    out_state[8] = state[8];
    out_state[12] = state[12];

    // Rotate first row 1 column left.
    u8 temp = state[1];
    out_state[1] = state[5];
    out_state[5] = state[9];
    out_state[9] = state[13];
    out_state[13] = temp;

    // Rotate second row 2 columns left
    temp = state[2];
    out_state[2] = state[10];
    out_state[10] = temp;

    temp = state[6];
    out_state[6] = state[14];
    out_state[14] = temp;

    // Rotate third row 3 columns left
    temp = state[3];
    out_state[3] = state[15];
    out_state[15] = state[11];
    out_state[11] = state[7];
    out_state[7] = temp;
}

static void InverseShiftRows(AESState& out_state, const AESState& state) {
    // Move zeroth row over
    out_state[0] = state[0];
    out_state[4] = state[4];
    out_state[8] = state[8];
    out_state[12] = state[12];

    // Rotate first row 1 column right.
    u8 temp = state[13];
    out_state[13] = state[9];
    out_state[9] = state[5];
    out_state[5] = state[1];
    out_state[1] = temp;

    // Rotate second row 2 columns right
    temp = state[2];
    out_state[2] = state[10];
    out_state[10] = temp;

    temp = state[6];
    out_state[6] = state[14];
    out_state[14] = temp;

    // Rotate third row 3 columns right
    temp = state[3];
    out_state[3] = state[7];
    out_state[7] = state[11];
    out_state[11] = state[15];
    out_state[15] = temp;
}

static void SubBytes(AESState& state, const SubstitutionTable& table) {
    for (size_t i = 0; i < 4; i++) {
        for (size_t j = 0; j < 4; j++) {
            state[4 * i + j] = table[state[4 * i + j]];
        }
    }
}

void DecryptSingleRound(AESState& out_state, const AESState& state) {
    InverseShiftRows(out_state, state);
    SubBytes(out_state, inverse_substitution_box);
}

void EncryptSingleRound(AESState& out_state, const AESState& state) {
    ShiftRows(out_state, state);
    SubBytes(out_state, substitution_box);
}

void MixColumns(AESState& out_state, const AESState& state) {
    for (size_t i = 0; i < out_state.size(); i += 4) {
        const u8 a = state[i];
        const u8 b = state[i + 1];
        const u8 c = state[i + 2];
        const u8 d = state[i + 3];

        const u8 tmp = a ^ b ^ c ^ d;

        out_state[i + 0] = a ^ xtime(a ^ b) ^ tmp;
        out_state[i + 1] = b ^ xtime(b ^ c) ^ tmp;
        out_state[i + 2] = c ^ xtime(c ^ d) ^ tmp;
        out_state[i + 3] = d ^ xtime(d ^ a) ^ tmp;
    }
}

void InverseMixColumns(AESState& out_state, const AESState& state) {
    for (size_t i = 0; i < out_state.size(); i += 4) {
        const u8 a = state[i];
        const u8 b = state[i + 1];
        const u8 c = state[i + 2];
        const u8 d = state[i + 3];

        out_state[i + 0] = Multiply(a, 0x0E) ^ Multiply(b, 0x0B) ^ Multiply(c, 0x0D) ^ Multiply(d, 0x09);
        out_state[i + 1] = Multiply(a, 0x09) ^ Multiply(b, 0x0E) ^ Multiply(c, 0x0B) ^ Multiply(d, 0x0D);
        out_state[i + 2] = Multiply(a, 0x0D) ^ Multiply(b, 0x09) ^ Multiply(c, 0x0E) ^ Multiply(d, 0x0B);
        out_state[i + 3] = Multiply(a, 0x0B) ^ Multiply(b, 0x0D) ^ Multiply(c, 0x09) ^ Multiply(d, 0x0E);
    }
}

} // namespace Dynarmic::Common
