#include "types.h"

/*
 * resource_39c owner at 0x02005388, 208 bytes: spawn two secondary objects
 * off the same "current slot" record (the identical
 * `((s32 **)(Data_03001ebc + 0x14))[idx]` lookup this overlay's
 * resource_39c_c_02000350.c and resource_39c_c_02005900.c share), each
 * spawn followed by its own field-initialisation tail, then one
 * unconditional trailing call.
 *
 * Complete owner: `push {r5, r6, r7, lr}` at 0x02005388 through
 * `pop {r5, r6, r7} / pop {r0} / bx r0` at 0x2005450, followed by three
 * pool words; the next owner's prologue is at 0x02005458. No incoming
 * arguments are read before being overwritten, so `void`.
 *
 * Not found by the structural inventory walk (unindexed): reached only by
 * `bl`, resolved with `bun tools/overlay_call_targets.ts`'s `+2` rule.
 *
 * The two spawns are NOT the same shape: the first spawn's tail also
 * updates the owner record's field-9 flag byte (`(field9 & ~0xC) | 4`) and
 * calls Func_08009020(owner, 2); the second spawn's tail sets a different
 * byte (offset 0x23, not touched by the first) and calls
 * Func_08009020(owner, 1), with neither the field-9 update nor the argument
 * matching the first. Recorded exactly as compiled, not unified.
 *
 * Uncertainty: none of the four resolved callees are identified beyond call shape;
 * the slot record's fields (8/12/16 spawn source, 20 carried across, 0x50
 * owner pointer, 0x68 back-pointer) and the spawned objects' own fields
 * (0x23, 0x55, 0x64) and the owner record's 0x26/field-9 are inferred only
 * from this call shape, not independently confirmed. The final
 * Func_080f9010(130) call's role is not established.
 */

extern u8 *Data_03001ebc;
extern u32 Data_02000434;

extern u8 *Func_080090c8(s32 kind, s32 arg1, s32 arg2, s32 arg3);
extern void Func_08009098(u8 *object, void *table);
extern void Func_08009020(u8 *record, s32 arg1);
extern void Func_080f9010(s32 arg0);

void Func_02005388(void)
{
    u32 idx = Data_02000434;
    s32 *slot = ((s32 **)(Data_03001ebc + 0x14))[idx];
    u8 *slotBytes = (u8 *)slot;
    u8 *object;
    u8 *owner;

    object = Func_080090c8(26, slot[2], slot[3], slot[4]);
    if (object != 0) {
        *(s32 *)(object + 20) = slot[5];
        owner = *(u8 **)(object + 0x50);
        Func_08009098(object, (void *)0x0200de14);

        object[0x55] = 0;
        *(short *)(object + 0x64) = 0;
        *(u8 **)(object + 0x68) = slotBytes;

        if (owner != 0) {
            Func_08009020(owner, 2);
            owner[0x26] = 0;
            owner[9] = (owner[9] & ~0x0C) | 4;
        }
    }

    object = Func_080090c8(26, slot[2], slot[3], slot[4]);
    if (object != 0) {
        *(s32 *)(object + 20) = slot[5];
        owner = *(u8 **)(object + 0x50);
        Func_08009098(object, (void *)0x0200de14);

        object[0x55] = 0;
        *(short *)(object + 0x64) = 0;
        object[0x23] = 2;
        *(u8 **)(object + 0x68) = slotBytes;

        if (owner != 0) {
            Func_08009020(owner, 1);
            owner[0x26] = 0;
        }
    }

    Func_080f9010(130);
}
