// Draft, differ 2 of 1 (tla-en, 2026-11-08): not C. The Thumb entry of the
// ARM routine Func_080212dc (080212dc.c) that follows it: bx pc and a
// halfword, 0x0200, that is never run. Callers enter in Thumb and bx pc
// switches to ARM at the next word, a mode switch inside a function, so its
// form is proven assembly (S1), as f021470_entries.s, f021828_entries.s and
// glyph_arm_entries.s write the same four bytes. No C makes bx pc: the empty
// body below (bx lr against bx pc) is the closest, and a call forwarding the
// routine's three arguments gives push {lr}, bl, pop {pc}, differ 4. The four
// bytes, 78 47 00 02, are the same in all six editions; tla-ja, -de, -es, -fr
// and -it hold them in one scaffold section with the routine
// (.rom.000212d8, 0x194 bytes), which diff cannot measure ("ARM, mixed or
// unknown ISA").
// To act on it: adopt it with Func_080212dc, as the earlier entries were, as
// games/tla/src/field/<object>_entries.s naming Func_080212d8 at bx pc and an
// AlchemyUncredited_080212da label on the .2byte 0x0200, listed in
// objects.tsv before the routine for every edition (the other editions'
// .rom.000212d8 section split or removed). Func_080212dc is blocked by the two
// veneers inside its literal pool (its header); the entry alone could be
// adopted first only by splitting those five scaffold sections at 0x080212dc.
// No caller names the entry in recon or games, so it is reached by a pointer
// or a copy to RAM; its name stays a placeholder.
#include "types.h"

void Func_080212d8(void) {}
