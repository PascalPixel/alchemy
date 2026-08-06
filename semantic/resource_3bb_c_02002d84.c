#include "types.h"

/*
 * resource_3bb owner at 0x02002d84, 192 bytes: either clear a one-shot
 * flag cell and forward to a single callee, or run a three-call setup
 * sequence, then reset a caller record's fields and copy three pointer
 * cells into a spawned object.
 *
 * Complete owner: `push {r5, r6, r7, lr}` at 0x02002d84 through `pop
 * {r5, r6, r7} / pop {r0} / bx r0` at 0x02002e3c-0x02002e40; a two-word
 * inline literal pool sits mid-body (0x02002e2c-0x02002e33, reached
 * only by the `b.n 0x2002e3c` at 0x02002e2a -- the same inline-pool
 * shape resource_39c_c_02002844.c/02002e08.c and
 * resource_3bb_c_02001ba8.c/02003138.c document); the next owner's
 * prologue is at 0x02002e44, after one alignment halfword. Void.
 *
 * Not found by the structural inventory walk (unindexed): reached only by
 * `bl`, resolved with `bun tools/overlay_call_targets.ts`'s `+2` rule.
 *
 * `current = *Data_03001e68` is a single-dereference "current object"
 * global -- one level shallower than resource_39c_c_02005900.c's
 * `Data_03001edc` (a different overlay, different address, same shape).
 * The flag cell (0x02000432, the same `Data_02000240`-relative table
 * this overlay reads elsewhere) is cleared on the "already set" path.
 *
 * Uncertainty: none of the eight callees are identified beyond call
 * shape; `object`'s many reset fields (6, 7, 8, 12, 16, 20, 0x22, 0x24,
 * 0x2c, 0x38, 0x40, 0x55) are inferred only from this call shape.
 */

extern u8 *Data_03001e68;









extern u8 * Func_02006cde(s32 arg0);
extern void Func_02006d42(s32 arg0, s32 arg1);
extern void Func_02006db2(s32 arg0, s32 arg1, s32 arg2);
extern void Func_02006d62(s32 arg0, s32 arg1);
extern void Func_02006d00(s32 arg0);
extern void Func_02006cb2(u8 *object, s32 arg1);
extern void Func_02006c62(u8 *object, s32 arg1);
extern void Func_02006c6a(u8 *object, s32 arg1);
extern void Func_02006bc8(s32 arg0);
void Func_02002d84(s32 arg0)
{
    u8 *flagCell = (u8 *)0x02000432;
    u8 *object = Func_02006cde(arg0);
    u8 *current = Data_03001e68;

    if (*flagCell == 1) {
        *flagCell = 0;
        Func_02006d42(arg0, 1);
    } else {
        Func_02006db2(arg0, 0x4000, 30);
        Func_02006d62(arg0, 3);
        Func_02006d00(30);
    }

    object[0x55] = 3;
    current[7] = 0;
    current[6] = 15;
    *(u32 *)(object + 8) = *(u32 *)0x0200cca4;
    *(u32 *)(object + 16) = *(u32 *)0x0200cbfc;
    *(u16 *)(object + 6) = (u16)*(u32 *)0x0200cc28;
    *(u32 *)(object + 0x40) = 0x80000000;
    *(u32 *)(object + 0x38) = 0x80000000;
    object[0x22] = 0;
    *(u32 *)(object + 0x24) = 0;
    *(u32 *)(object + 0x2c) = 0;
    *(u32 *)(object + 12) = 0;
    *(u32 *)(object + 20) = 0;

    Func_02006cb2(object, 1);
    Func_02006c62(object, 0);
    Func_02006c6a(object, 1);
    Func_02006bc8(1);
}
