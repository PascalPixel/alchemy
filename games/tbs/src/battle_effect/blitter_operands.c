/* @file */
/* The ARM immediate operands for powers of two, which the blitter builder */
/* (battle_effect_blitter.c) adds to its clip words to fit the canvas width. */

#include "battle_effect/blitter_operands.h"
#include "types.h"

/* The 12-bit operand field that makes an ARM data-processing immediate equal */
/* to 1 << n: below 8 the value itself; from 8 the value 1 or 2 with the */
/* rotation in bits 8 to 11, rotated right by twice that (n 8 is 0xc01, 1 */
/* rotated right by 24). */
#define ARM_IMMEDIATE_BIT(n)                                                   \
	((n) < 8 ? 1 << (n) : (12 - ((n) - 8) / 2) << 8 | 1 << ((n) & 1))

/* ARM_IMMEDIATE_BIT of each bit number in turn. */
#define ARM_IMMEDIATE_POWERS                                                   \
	ARM_IMMEDIATE_BIT(0), ARM_IMMEDIATE_BIT(1), ARM_IMMEDIATE_BIT(2),          \
		ARM_IMMEDIATE_BIT(3), ARM_IMMEDIATE_BIT(4), ARM_IMMEDIATE_BIT(5),      \
		ARM_IMMEDIATE_BIT(6), ARM_IMMEDIATE_BIT(7), ARM_IMMEDIATE_BIT(8),      \
		ARM_IMMEDIATE_BIT(9), ARM_IMMEDIATE_BIT(10), ARM_IMMEDIATE_BIT(11),    \
		ARM_IMMEDIATE_BIT(12), ARM_IMMEDIATE_BIT(13), ARM_IMMEDIATE_BIT(14),   \
		ARM_IMMEDIATE_BIT(15)

/* Unused: a copy of Blitter_ImmediatePowers that nothing reads. */
u16 Blitter_UnusedImmediatePowers[] = {ARM_IMMEDIATE_POWERS};
/* The operand for 1 << n, by n from 0 to 15. btEffectBuildBlitter */
/* (battle_effect_blitter.c) adds the entry for the canvas width's power of two
 */
/* (7 for the 128-pixel canvas) to two of the instructions that clip a picture
 */
/* at the canvas's left and right edges, so that they hold the width. */
u16 Blitter_ImmediatePowers[] = {ARM_IMMEDIATE_POWERS};
