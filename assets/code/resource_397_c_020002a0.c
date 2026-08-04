#include "resource_397.h"

#include "types.h"

/*
 * resource_397 owner at 0x020002a0, 64 bytes: recompute the three alpha-blend
 * workspace fields that this resource's "re-arm" owner (Func_0200015c) and
 * its root (Func_02000200) publish through BLDCNT/BLDALPHA.
 *
 * Complete owner: no push/pop -- the body touches only r0-r3 and returns with
 * a bare `bx lr` at 0x020002ca, followed by the five-word literal pool
 * 0x020002cc-0x020002e0 that closes the 64-byte span (0x03001e70,
 * 0x02008610, 0x02008614, 0x03001e40, 0x02008616).  The next owner's
 * prologue is at 0x020002e0, already tracked as
 * assets/code/resource_397_c_020002e0.c.  No calls -- matches
 * `bun tools/overlay_call_targets.ts resource_397 02a0` reporting none.
 *
 * The three destinations -- Data_02008610 (s32), Data_02008614 (u16),
 * Data_02008616 (u16) -- are the exact symbols and types already declared in
 * the byte-exact sibling assets/code/resource_397_c_0200026c.c, which reads
 * them back (`if (Data_04000006 >= Data_02008610) ...`), so this owner is
 * plausibly what arms the values that read consumes. Data_03001e70 keeps the
 * pointer typing established in assets/code/resource_373_c_02000244.c
 * (`extern u8 *Data_03001e70`), and Data_03001e40 keeps the `volatile s32`
 * typing established in assets/code/resource_371_c_02003f88.c.
 *
 * The workspace record is read at fixed offsets +0x106 and +0x10a from
 * `Data_03001e70 + 0x104` -- no argument selects the offset, so this owner
 * always targets the same sub-record. The field at +0x10a is read once, into
 * Data_02008610's "192 minus" computation; the field at +0x106 is read once
 * and reused for both Data_02008614 (as-is) and Data_02008616 (minus
 * `Data_03001e40 >> 2`).
 *
 * Uncertainty: the sub-record layout at workspace+0x104 and the meaning of
 * the two s16 fields are not resolved beyond their type (loaded with
 * `ldrsh`, so signed 16-bit); the 192 constant and the `>> 2` scale are
 * copied as encoded, not reinterpreted. No null check on Data_03001e70
 * precedes the dereference, unlike resource_373_c_02000244.c's owner -- this
 * one is presumably only ever called once the workspace is known live.
 */

extern u8 *Data_03001e70;
extern volatile u32 Data_03001e40;
extern s32 Data_02008610;
extern u16 Data_02008614;
extern u16 Data_02008616;

void Func_020002a0(void)
{
    s32 recordOffset;
    u8 *record;

    recordOffset = 0x104;
    record = Data_03001e70;
    record += recordOffset;

    {
        s16 fieldA = *(s16 *)(record + 6);
        Data_02008610 = 192 - fieldA;
    }
    {
        u16 *fieldBOutput = &Data_02008614;
        u16 *fieldCOutput;
        s16 fieldB = *(s16 *)(record + 2);
        u32 blendPhase;
        *fieldBOutput = (u16)fieldB;
        blendPhase = Data_03001e40;
        fieldCOutput = &Data_02008616;
        blendPhase >>= 2;
        *fieldCOutput = (u16)(fieldB - blendPhase);
    }
}
