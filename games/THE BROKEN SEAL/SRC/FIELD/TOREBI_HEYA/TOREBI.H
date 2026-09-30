/* Declarations shared by the Tolbi house scene's sources (resource_3b6). */
#ifndef TOREBI_H
#define TOREBI_H

#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

struct Ent { s32 a; u16 b; u16 c; };

extern u8 Data_02008bb4[];
extern u8 Data_02008dac[];
extern u8 Data_02008e00[];
extern u8 Data_02009040[];
extern u8 Data_020099d0[];
extern u8 Data_02009670[];
extern u8 Data_02009310[];
extern struct Ent Data_02009dcc[];

/* Contiguous unnamed leaf-owner run for resource_3b6. */

/* The scene step counter at 0x1d8 of the shared scene work record. */

/* 0x02000d6a serves two imports: the one-argument message at +0x04 and the
 * three-argument setter at +0x0e. `_b` is the same address with its own
 * prototype. */

/* Pool-forced message base: a SYMBOL_REF gcc cannot fold, so the two
 * sibling ids stay `adds r0, r5, #1` / `#2` off one pooled word. */

#endif
