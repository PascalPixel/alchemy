#include "types.h"

/*
 * Owner at 0x080c0ea8 (8 bytes of code + 8 bytes trailing pool).
 *
 * Census note: the raw manifest region for this gap was recorded as
 * 0x080c0ea6/18 bytes, but 0x080c0ea6-0x080c0ea7 is a two-byte alignment
 * halfword (`movs r0,r0`, `.2byte 0`) belonging to the PRECEDING function's
 * tail, not this one's body -- the real prologue-free leaf starts at
 * 0x080c0ea8. Registered span here is address+size for the true 8-byte
 * code range; see semantic/main-regions.json.
 *
 * `ldr r2,[pc,#8] / ldr r3,[pc,#4] / strh r3,[r2,#0] / bx lr`. Two pool
 * words: 0x080c0eb0 = 0x000000bf, 0x080c0eb4 = 0x04000050. 0x04000050 is
 * the GBA I/O register BLDCNT (blend control); this is a fire-and-forget
 * hardware register write setting the blend-control bits to 0xbf
 * (0b10111111) and returning -- no arguments, no return value, no other
 * side effects.
 */

#define REG_BLDCNT (*(volatile u16 *)0x04000050)

void Func_080c0ea8(void)
{
    REG_BLDCNT = 0xbf;
}
