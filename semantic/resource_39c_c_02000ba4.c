#include "types.h"

/*
 * resource_39c owner at 0x02000ba4, 284 bytes: map the current object's
 * signed key through a six-entry table, derive a position and two extents
 * from the matching four-word delta record, then issue one placement call
 * and two indexed setup calls. Returns 1 on success and 0 when the key is
 * absent.
 *
 * Complete owner: `push {r5, r6, r7, lr}` plus the r8/sl spill at
 * 0x02000ba4 through the real-value `pop {r1} / bx r1` return at
 * 0x02000cb0-0x02000cb2. Its three referenced literal words occupy
 * 0x02000cb4-0x02000cbf; the next owner's prologue is at 0x02000cc0.
 * One incoming argument is forwarded to the opening object lookup.
 *
 * The six-key table is 0x0200dd90 and the matching four-word records are
 * at 0x0200dda8. These addresses recur in this overlay's still-undrafted
 * rectangle-search family, but this owner alone does not establish their
 * game-level names. The runtime pointer at 0x03001e70 is likewise kept
 * positional rather than conflated with the nearby 0x03001ebc/0x03001edc
 * families.
 */

extern u8 *Data_03001e70;

extern u8 *Func_0808a080(s32 arg0);
extern void Func_080091c0(s32, s32, s32, s32, s32, s32);
extern s32 Func_02000528(u32, s32, s32, u32, u32, s32);

static s32 Abs_02000ba4(s32 value)
{
    return value < 0 ? -value : value;
}

s32 Func_02000ba4(s32 arg0)
{
    static const s32 *const keys = (const s32 *)0x0200dd90;
    static const s32 *const deltas = (const s32 *)0x0200dda8;
    u8 *object = Func_0808a080(arg0);
    s32 key = *(short *)(*(u8 **)(object + 0x50) + 0x28);
    s32 index;
    s32 baseX;
    s32 baseZ;
    s32 positionX;
    s32 positionZ;
    s32 extentX;
    s32 extentZ;
    s32 globalX;
    s32 globalZ;

    for (index = 0; index <= 5; index++) {
        if (keys[index] == key) {
            break;
        }
    }
    if (index > 5) {
        return 0;
    }

    baseX = *(s32 *)(object + 8);
    baseZ = *(s32 *)(object + 16);

    positionX = (baseX + (deltas[index * 4] << 16)) >> 20;
    positionZ = (baseZ + (deltas[index * 4 + 1] << 16)) >> 20;
    extentX = (Abs_02000ba4(deltas[index * 4]) +
               Abs_02000ba4(deltas[index * 4 + 2])) >> 4;
    extentZ = (Abs_02000ba4(deltas[index * 4 + 1]) +
               Abs_02000ba4(deltas[index * 4 + 3])) >> 4;

    globalX = (*(s32 *)(Data_03001e70 + 0x13c) >> 20) + positionX;
    globalZ = (*(s32 *)(Data_03001e70 + 0x140) >> 20) + positionZ;

    Func_080091c0(positionX, positionZ, extentX, extentZ, globalX, globalZ);
    Func_02000528(0, positionX, positionZ, (u32)extentX, (u32)extentZ, 255);
    Func_02000528(2, positionX, positionZ, (u32)extentX, (u32)extentZ, 255);
    return 1;
}
