// The ARM immediate operands for powers of two, which the blitter builder (battle_effect_moloch.c)
// adds to its clip instructions to fit them to the canvas width, as The Broken Seal's builder does.

#include "types.h"

// The 12-bit operand field that makes an ARM data-processing immediate equal to 1 << n: below 8 the
// value itself; from 8 the value 1 or 2 with the rotation in bits 8 to 11, rotated right by twice
// that (n 8 is 0xc01, 1 rotated right by 24).
#define ARM_IMMEDIATE_BIT(n) ((n) < 8 ? 1 << (n) : (12 - ((n) - 8) / 2) << 8 | 1 << ((n) & 1))

// The operand for 1 << n, by n from 0 to 15: the blitter builder adds the one for the canvas width,
// as a power of two, to words 8 and 11 of its horizontal clip template, counting from 0.
const Uint16 BlitterImmediatePowers[] = {
	ARM_IMMEDIATE_BIT(0),
	ARM_IMMEDIATE_BIT(1),
	ARM_IMMEDIATE_BIT(2),
	ARM_IMMEDIATE_BIT(3),
	ARM_IMMEDIATE_BIT(4),
	ARM_IMMEDIATE_BIT(5),
	ARM_IMMEDIATE_BIT(6),
	ARM_IMMEDIATE_BIT(7),
	ARM_IMMEDIATE_BIT(8),
	ARM_IMMEDIATE_BIT(9),
	ARM_IMMEDIATE_BIT(10),
	ARM_IMMEDIATE_BIT(11),
	ARM_IMMEDIATE_BIT(12),
	ARM_IMMEDIATE_BIT(13),
	ARM_IMMEDIATE_BIT(14),
	ARM_IMMEDIATE_BIT(15),
};
