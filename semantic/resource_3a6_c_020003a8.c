#include "types.h"

/*
 * resource_3a6 owner at 0x020003a8, 384 bytes: code through the sole unwind
 * return at 0x0200050a-0x02000519, alignment at 0x0200051a, and its three-word
 * pool through 0x02000527.  The next recorded owner begins at 0x02000528.
 *
 * The routine moves actor 0 through a direction-indexed packed X/Z step, but
 * only after proving that the destination and the cell one unit above the
 * intervening actor are unoccupied.  Func_02000350 is this overlay's exact-C
 * collision lookup: it compares the three coarse coordinates in `position`
 * against actor slots 8..65.  A flagged collision aborts the move.
 *
 * The word table at 0x02009e48 has one packed signed displacement per high
 * nibble of actor 0's angle.  Its high half supplies X and its low half Z.
 * The sibling owners in resource_3a6, resource_3b3 and resource_3be have the
 * same code/control-flow family but independently located tables.
 *
 * Call accounting: all thirteen reachable BL sites are represented below in
 * source/machine order: Func_02000350 x3, Func_08009080 x2, and one each of
 * the other seven targets.  There is no call_via or m2c truncation hazard.
 */











extern u8 * Func_02002074();
extern s32 * Func_0200073e();
extern s32 * Func_0200076a();
extern s32 * Func_02000796();
extern s32 Func_020020f8();
extern void Func_020020d0();
extern void Func_020020c0();
extern void Func_02002206();
extern void Func_0200210e();
extern void Func_0200211e();
extern void Func_0200212c();
extern void Func_02002228();
extern void Func_0200213c();
void Func_020003a8(void)
{
    s32 permuted_17;
    s32 permuted_9;
    s32 permuted_11;
    u8 *leader = Func_02002074(0);
    const s32 *steps = (const s32 *)0x02009e48;
    s32 packed = steps[*(u16 *)(leader + 6) >> 12];
    s32 position[3];
    u8 *actor;
    u8 *blocked;

    position[1] = *(s32 *)(leader + 12);
    permuted_9 = *(s32 *)(leader + 8) + (packed & (s32)0xffff0000);
    permuted_11 = *(s32 *)(leader + 16) + (s32)((u32)packed << 16);
    position[0]  = permuted_9;
    position[2]  = permuted_11;

    actor = (u8 *)Func_0200073e(position, leader);
    if (actor == 0)
        return;

    position[0] = *(s32 *)(actor + 8) + (packed & (s32)0xffff0000);
    position[1] = *(s32 *)(actor + 12);
    permuted_17 = *(s32 *)(actor + 16) + (s32)((u32)packed << 16);
    blocked = (u8 *)Func_0200076a(position, actor);
    if (blocked != 0 && (blocked[0x59] & 1) != 0)
        return;
    position[2]  = permuted_17;

    position[2] = *(s32 *)(actor + 16);
    blocked = (u8 *)Func_02000796(position, actor);
    position[0] = *(s32 *)(actor + 8);
    position[1] = *(s32 *)(actor + 12) + 0x00100000;
    if (blocked != 0 && (blocked[0x59] & 1) != 0)
        return;

    actor[0x22] = 2;
    position[0] = *(s32 *)(actor + 8) + (packed & (s32)0xffff0000);
    position[1] = *(s32 *)(actor + 12);
    position[2] = *(s32 *)(actor + 16) + (s32)((u32)packed << 16);
    if (Func_020020f8(actor, position) > 0)
        return;
    if (actor[0x62] != 0)
        return;

    Func_020020d0(leader, 8);
    Func_020020c0(15);
    Func_02002206(185);

    *(s32 *)(actor + 0x30) = 0x3333;
    *(s32 *)(actor + 0x34) = 0x3333;
    Func_0200210e(actor, position[0], position[1], position[2]);

    *(s32 *)(leader + 0x30) = 0x3333;
    *(s32 *)(leader + 0x34) = 0x3333;
    Func_0200211e(leader, position[0], position[1], position[2]);
    Func_0200212c(actor);
    Func_02002228();

    *(s32 *)(actor + 8) = position[0];
    *(s32 *)(actor + 16) = position[2];
    *(s32 *)(actor + 0x24) = 0;
    *(s32 *)(actor + 0x2c) = 0;

    *(s32 *)(leader + 0x38) = (s32)0x80000000;
    *(s32 *)(leader + 0x40) = (s32)0x80000000;
    *(s32 *)(leader + 0x24) = 0;
    *(s32 *)(leader + 0x2c) = 0;
    *(s32 *)(leader + 8) = (s32)*(s16 *)(leader + 0x0a) << 16;
    *(s32 *)(leader + 16) = (s32)*(s16 *)(leader + 0x12) << 16;
    Func_0200213c(leader, 1);
}
