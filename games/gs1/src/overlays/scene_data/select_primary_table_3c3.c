/* Contiguous unnamed state-owner run for resource_3c3. */

#include "types.h"

extern s16 Data_02000240[];
extern u8 Value_000000aa;
extern u8 Value_000000ab;
extern u8 Data_02008a40[];
extern u8 Data_02008ad0[];
extern u8 Data_02008998[];

#define SceneData_GetPrimaryTable Func_02000040

s32 SceneData_GetPrimaryTable(void) {
    s16 v = Data_02000240[224];

    if (v == (s32)&Value_000000aa) {
        return (s32)Data_02008a40;
    }
    if (v == (s32)&Value_000000ab) {
        return (s32)Data_02008ad0;
    }
    return (s32)Data_02008998;
}

#include "types.h"

#define SceneData_ReturnZero Func_02000080

/*
 * resource_3c3 owner at 0x02000080, 4 bytes: `movs r0, #0 / bx lr`.
 *
 * LEAF RESIDUE. Published at image offset 0x2c; sweep B resolved that
 * word and, before 2026-08-01, discarded it for not opening with a `push`.
 * A leaf never does -- it saves no register and returns with `bx lr`.
 *
 * Complete owner: both instructions. No prologue, no stack frame, no
 * literal pool, no callees, no argument read.
 *
 * One of the 70 rows sharing this exact body across the tree. The body is
 * shared; the identity is not -- this row is bounded by ITS overlay's
 * neighbours and published from ITS overlay's table.
 */

s32 SceneData_ReturnZero(void)
{
    return 0;
}
