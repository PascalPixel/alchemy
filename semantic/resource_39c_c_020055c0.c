#include "types.h"

/*
 * resource_39c owner at 0x020055c0, 824 bytes: select the current slot's
 * terrain cell, update or replace the record's linked object, run the slot's
 * transition sequence, and finish by accumulating a fixed-point value into
 * the overlay runtime header.
 *
 * Complete owner: `push {r5, r6, r7, lr}` plus the r8/r9/sl/fp spill at
 * 0x020055c0 through `bx r0` at 0x020058f6. Two literal words used after the
 * inline pool are at 0x020058f8-0x020058ff; the next owner's prologue is at
 * 0x02005900. No incoming argument survives the opening global lookups, so
 * this is a void/no-argument owner.
 *
 * This row cannot use m2c as a seed: the `mov ip,pc / bx r4` at
 * 0x020058d4 calls the established relocated 16.16 helper at 0x03000118,
 * which m2c mistakes for a return and silently truncates. It is expressed as
 * an ordinary C call below. All 31 `bl` sites were resolved independently;
 * the remaining indirect helper site was checked at its register load.
 *
 * `record`, `slot`, and the 0x02010000 cell lookup are the same runtime
 * families documented by resource_39c_c_02005458.c,
 * resource_39c_c_02005900.c, and resource_39c_c_02004fcc.c. Field names stay
 * positional because this owner does not establish their game-level roles.
 */

extern s32 **Data_03001edc;
extern u8 *Data_03001ebc;
extern s32 Data_02000434;

















typedef s32 (*RelocatedMultiply_020055c0)(s32 value, s32 scale);

static s32 Fixed20_020055c0(s32 raw)
{
    if (raw < 0) {
        raw += 0xfffff;
    }
    return raw >> 20;
}

extern void Func_0200b012(s32, s32, s32 *);
extern s32 Func_0200b160();
extern s32 Func_0200b0d8();
extern s32 Func_0200b0a0();
extern u8 * Func_0200b0d6(s32, s32, s32, s32);
extern s32 Func_0200b0e2();
extern s32 Func_0200b134();
extern s32 Func_0200b112();
extern s32 Func_0200b150();
extern s32 Func_0200b212();
extern s32 Func_0200b186();
extern s32 Func_0200b18e();
extern s32 Func_0200b14c();
extern s32 Func_0200b43a();
extern s32 Func_0200b1a2();
extern s32 Func_0200b230();
extern s32 Func_0200b310();
extern s32 Func_0200b1e0();
extern s32 Func_0200b19e();
extern s32 Func_0200b262();
extern s32 Func_0200b4a2();
extern s32 Func_0200b1c0();
extern s32 Func_0200b22a();
extern s32 Func_0200b4d0();
extern s32 Func_0200b1f8();
extern void Func_0200a9e6(u8 *slot);
extern s32 Func_0200b504();
extern s32 Func_0200b222();
extern s32 Func_0200b2e4();
extern s32 Func_0200b37e();
void Func_020055c0(void)
{
    u8 *runtime = Data_03001ebc;
    u8 *slot = ((u8 **)(runtime + 0x14))[Data_02000434];
    s32 *record = *Data_03001edc;
    u8 savedSlotFlags = slot[0x55];
    s32 variant = *(short *)(0x0200de44 + (((*(u32 *)0x03001ae8 >> 4) & 15) * 2));
    s32 probe[3];
    u8 *firstCell;
    u8 *cell;
    s32 result;
    u8 *object;

    if (variant == -1) {
        return;
    }

    probe[0] = (*(s32 *)(slot + 8) & (s32)0xfff00000) + 0x80000;
    probe[1] = *(s32 *)(slot + 20);
    probe[2] = (*(s32 *)(slot + 16) & (s32)0xfff00000) + 0x80000;

    firstCell = (u8 *)0x02010000 +
        (Fixed20_020055c0(probe[2]) * 128 + Fixed20_020055c0(probe[0])) * 4;
    Func_0200b012(0x200000, variant, probe);
    cell = (u8 *)0x02010000 +
        (Fixed20_020055c0(probe[2]) * 128 + Fixed20_020055c0(probe[0])) * 4;

    if (firstCell[2] != record[1] && cell[2] == record[1] && record[0] == 0) {
        return;
    }

    Func_0200b160();
    result = Func_0200b0d8(slot, probe);
    if (result != 0) {
        return;
    }

    if (object != 0) {
        *(u16 *)(object + 100) = (u16)result;
        Func_0200b0a0(object, (void *)0x0200de2c);
        Func_0200b0a0(object, 7);
        record[6] = result;
    }
    object = (u8 *)record[6];

    if (cell[2] == record[1] && record[0] != 0) {
        u8 *source = (u8 *)record[5];

        object = Func_0200b0d6(
            26,
            *(s32 *)(source + 8),
            *(s32 *)(source + 12),
            *(s32 *)(source + 16));
        if (object != 0) {
            u8 *ownerRecord = *(u8 **)(object + 0x50);

            *(s32 *)(object + 20) = *(s32 *)(source + 20);
            Func_0200b0e2(object, (void *)0x0200de20);
            object[0x55] = (u8)result;
            *(u16 *)(object + 100) = (u16)result;
            object[0x23] = 2;
            *(s32 *)(object + 0x30) = 0x40000;
            *(s32 *)(object + 0x34) = 0x20000;
            Func_0200b134(object, probe[0], probe[1], probe[2]);

            if (ownerRecord != 0) {
                Func_0200b112(ownerRecord, 6);
                ownerRecord[0x26] = 0;
            }
            record[6] = (s32)object;
        }
    }

    record[0]--;
    if (record[0] == 0) {
        Func_0200b150((void *)record[5]);
        record[5] = 0;
        Func_0200b212(0x161);
    } else if (record[5] != 0) {
        Func_0200b186((void *)record[5], 6 - record[0]);
    }

    Func_0200b18e(slot, 6);
    Func_0200b14c(3);
    Func_0200b43a(0x98);
    Func_0200b1a2(slot, 7);

    *(s32 *)(slot + 0x34) = 0x20000;
    *(s32 *)(slot + 0x30) = 0x30000;
    slot[0x55] &= 0x7e;
    *(s32 *)(slot + 0x28) = 0x40000;

    Func_0200b230(slot, 0);
    Func_0200b310(0, *(short *)((u8 *)probe + 2), *(short *)((u8 *)probe + 10));
    Func_0200b1e0(slot, 6);
    Func_0200b19e(2);

    if (cell[2] != record[1]) {
        Func_0200b262(slot, 1);
    } else {
        Func_0200b4a2(0xd7);
    }
    Func_0200b1c0(1);
    slot[0x55] = savedSlotFlags;

    if (cell[2] == record[1] && record[6] == 0) {
        s32 frame = 0;

        Func_0200b22a(slot, 18);
        Func_0200b4d0(0xf1);
        goto check_frame;

next_frame:
            Func_0200b1f8(1);
            frame++;

check_frame:
        if ((frame & 15) == 0) {
            Func_0200a9e6(slot);
        }
        if (frame <= 31 || *(volatile u32 *)0x03001c94 == 0) {
            goto next_frame;
        }

        Func_0200b504(0x120);
        Func_0200b222(1);
        *(s32 *)(slot + 8) = record[3];
        *(s32 *)(slot + 16) = record[4];
        Func_0200b2e4(slot, 1);
    }

    Func_0200b37e();
    record[2] = 0;
    *(s32 *)(runtime + 0x1b4) +=
        ((RelocatedMultiply_020055c0)0x03000118)(*(s32 *)(runtime + 0x1b0), 0x200000);
}
