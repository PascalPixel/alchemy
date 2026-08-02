typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed short s16;
typedef signed int s32;

/*
 * resource_3b3 owner at 0x020003a8, 384 bytes: code through the sole unwind
 * return at 0x0200050a-0x02000519, alignment at 0x0200051a, and its three-word
 * pool through 0x02000527.  The next recorded owner begins at 0x02000528.
 *
 * The routine moves actor 0 through a direction-indexed packed X/Z step, but
 * only after proving that the destination and the cell one unit above the
 * intervening actor are unoccupied.  Func_02000350 is this overlay's exact-C
 * collision lookup: it compares the three coarse coordinates in `position`
 * against actor slots 8..65.  A flagged collision aborts the move.
 *
 * The word table at 0x0200aca0 has one packed signed displacement per high
 * nibble of actor 0's angle.  Its high half supplies X and its low half Z.
 * The sibling owners in resource_3a6, resource_3b3 and resource_3be have the
 * same code/control-flow family but independently located tables.
 *
 * Call accounting: all thirteen reachable BL sites are represented below in
 * source/machine order: Func_02000350 x3, Func_08009080 x2, and one each of
 * the other seven targets.  There is no call_via or m2c truncation hazard.
 */

s32 *Func_02000350();
void Func_080000c0();
void Func_08009080();
void Func_08009150();
void Func_08009158();
s32 Func_080091d8();
u8 *Func_0808a080();
void Func_0808a5e8();
void Func_080f9010();

void Func_020003a8(void)
{
    u8 *leader = Func_0808a080(0);
    const s32 *steps = (const s32 *)0x0200aca0;
    s32 packed = steps[*(u16 *)(leader + 6) >> 12];
    s32 position[3];
    u8 *actor;
    u8 *blocked;

    position[0] = *(s32 *)(leader + 8) + (packed & (s32)0xffff0000);
    position[1] = *(s32 *)(leader + 12);
    position[2] = *(s32 *)(leader + 16) + (s32)((u32)packed << 16);

    actor = (u8 *)Func_02000350(position, leader);
    if (actor == 0)
        return;

    position[0] = *(s32 *)(actor + 8) + (packed & (s32)0xffff0000);
    position[1] = *(s32 *)(actor + 12);
    position[2] = *(s32 *)(actor + 16) + (s32)((u32)packed << 16);
    blocked = (u8 *)Func_02000350(position, actor);
    if (blocked != 0 && (blocked[0x59] & 1) != 0)
        return;

    position[0] = *(s32 *)(actor + 8);
    position[1] = *(s32 *)(actor + 12) + 0x00100000;
    position[2] = *(s32 *)(actor + 16);
    blocked = (u8 *)Func_02000350(position, actor);
    if (blocked != 0 && (blocked[0x59] & 1) != 0)
        return;

    actor[0x22] = 2;
    position[0] = *(s32 *)(actor + 8) + (packed & (s32)0xffff0000);
    position[1] = *(s32 *)(actor + 12);
    position[2] = *(s32 *)(actor + 16) + (s32)((u32)packed << 16);
    if (Func_080091d8(actor, position) > 0)
        return;
    if (actor[0x62] != 0)
        return;

    Func_08009080(leader, 8);
    Func_080000c0(15);
    Func_080f9010(185);

    *(s32 *)(actor + 0x30) = 0x3333;
    *(s32 *)(actor + 0x34) = 0x3333;
    Func_08009150(actor, position[0], position[1], position[2]);

    *(s32 *)(leader + 0x30) = 0x3333;
    *(s32 *)(leader + 0x34) = 0x3333;
    Func_08009150(leader, position[0], position[1], position[2]);
    Func_08009158(actor);
    Func_0808a5e8();

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
    Func_08009080(leader, 1);
}
