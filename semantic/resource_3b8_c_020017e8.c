#include "types.h"

/*
 * Resource 3b8 cutscene at 0x020017e8.
 *
 * Complete owner: `push {r5, r6, r7, lr}` at 0x020017e8 and the matching
 * `pop {r5, r6, r7} / pop {r0} / bx r0` interworking return at 0x02001ff6.
 * Two mid-function literal pools are branched over, 0x02001a8a-0x02001aaf and
 * 0x02001d7a-0x02001d87, and a third at 0x02001ffc-0x02002013 follows the
 * return.  All three are data.
 *
 * Workspace access goes through the IWRAM pointer array at 0x03001ebc, the
 * same base the other owners in this overlay use: word 0 is the scene record
 * (fields at +0x1c0, +0x1c8 and the halfword counter at +0x1d8) and word 4,
 * i.e. the pointer at 0x03001ecc, is a second record whose halfwords at
 * +0x52a/+0x534/+0x536 this owner drives.  The two counted loops write the
 * cursor at +0x52a *after* each call, so they leave 5 and 31 behind
 * respectively.
 *
 * Resolver-confirmed main-image symbols name every external call below.
 * Old-style declarations keep the source honest where a shared engine helper
 * is reached with more than one argument shape.
 */

void Func_080000c0();
void Func_080091e0();
void Func_0808a010();
void Func_0808a018();
void Func_0808a020();
s32 Func_0808a070();
u8 *Func_0808a080();
void Func_0808a090();
void Func_0808a0b8();
void Func_0808a0e8();
void Func_0808a0f0();
void Func_0808a100();
void Func_0808a110();
void Func_0808a128();
void Func_0808a138();
void Func_0808a148();
void Func_0808a150();
void Func_0808a170();
void Func_0808a178();
void Func_0808a180();
void Func_0808a1b8();
void Func_0808a1e8();
void Func_0808a300();
void Func_0808a360();
void Func_0808a370();
void Func_0808a580();

void Func_020017e8(void)
{
    u32 *slotBase = (u32 *)0x03001ebc;
    u8 *state;
    u16 *cursor;
    u8 *record;
    s32 index;
    s32 branch;

    Func_0808a018();
    Func_0808a170(0x228c);
    Func_0808a100(0, 31);
    /* Pass the selected scene record directly to the engine handoff. */
    Func_080091e0(Func_0808a080(0), 0);
    Func_0808a0f0(1, 0x680000, 0x680000);
    Func_0808a0f0(3, 0x580000, 0x780000);
    Func_0808a0f0(2, 0x780000, 0x780000);
    Func_0808a1b8(1, 0x4000, 0);
    Func_0808a1b8(3, 0, 0);
    Func_0808a1b8(2, 0x8000, 0);

    *(u32 *)((u8 *)slotBase[0] + 0x1c8) = 12;
    *(u32 *)((u8 *)slotBase[0] + 0x1c0) = 0x100;

    Func_080000c0(1);
    Func_0808a300(9);

    /* A second workspace, from the pointer one word further on at
     * 0x03001ecc. */
    state = (u8 *)slotBase[4];
    *(u16 *)(state + 0x536) = 1;
    Func_0808a360();
    cursor = (u16 *)(state + 0x52a);
    *(u16 *)(state + 0x534) = 0x1f1f;
    Func_0808a370();
    *cursor = 0;

    for (index = 1; index <= 5; index++) {
        *cursor = (u16)index;
        Func_080000c0(3);
    }

    Func_0808a010(40);
    Func_0808a138(0, 2);
    Func_0808a010(30);

    *(u16 *)(state + 0x536) = 31;
    for (index = 5; index <= 31; index++) {
        *cursor = (u16)index;
        Func_080000c0(3);
    }

    *(u32 *)((u8 *)slotBase[0] + 0x1c0) = 0x209;
    *(u32 *)((u8 *)slotBase[0] + 0x1c8) = 24;

    Func_0808a010(20);
    Func_0808a1e8(1, 256, 50);
    Func_0808a148(1, 0, 40);
    Func_0808a138(1, 2);
    Func_0808a010(30);
    Func_0808a010(10);
    Func_0808a1e8(2, 0x101, 40);
    Func_0808a1b8(2, 0xe000, 0);
    Func_0808a010(30);
    Func_0808a180(2, 0);
    Func_0808a010(10);
    Func_0808a180(3, 0);
    Func_0808a1b8(3, 0xe000, 0);
    Func_0808a010(30);
    Func_0808a138(3, 2);
    Func_0808a010(30);
    Func_0808a090(3, 0x10000, 0x8000);
    Func_0808a580(3, 0, -40);
    Func_0808a580(3, 32, 0);
    Func_0808a1b8(3, 0x2000, 0);
    Func_0808a010(10);
    Func_0808a180(3, 0);

    *(u32 *)(record + 16) += 0xfffd0000;
    record = Func_0808a080(0);
    *(u32 *)(record + 64) += 0xfffd0000;
    record = Func_0808a080(0);

    Func_0808a100(0, 32);
    Func_0808a010(40);
    Func_0808a110(0, 34);
    Func_0808a010(30);
    Func_0808a100(0, 33);
    Func_0808a010(40);
    Func_0808a110(1, 4);
    Func_0808a010(20);
    Func_0808a180(1, 0);
    Func_0808a010(10);
    Func_0808a1e8(0, 0x102, 80);
    Func_0808a1e8(2, 256, 50);
    Func_0808a178(2, 0);

    if (Func_0808a070(-1, 0) == 0) {
        Func_0808a010(10);
        Func_0808a110(0, 33);
        Func_0808a010(20);
        Func_0808a1e8(1, 0x103, 40);
        Func_0808a128(1, 4, 13);
        Func_0808a128(1, 4, 30);
        Func_0808a180(1, 0);
        *(u16 *)((u8 *)slotBase[0] + 0x1d8) += 1;
    } else {
        Func_0808a010(10);
        Func_0808a110(0, 34);
        Func_0808a010(20);
        Func_0808a1e8(1, 0x103, 40);
        Func_0808a128(1, 4, 13);
        Func_0808a128(1, 4, 30);
        Func_0808a180(1, 0);
        *(u16 *)((u8 *)slotBase[0] + 0x1d8) += 1;
    }

    Func_0808a010(10);
    Func_0808a1b8(2, 0xa000, 0);
    Func_0808a010(30);
    Func_0808a110(2, 4);
    Func_0808a010(20);
    Func_0808a1b8(1, 0x2000, 0);
    Func_0808a010(30);
    Func_0808a138(1, 2);
    Func_0808a010(40);
    Func_0808a1b8(1, 0xe000, 0);
    Func_0808a1b8(2, 0xe000, 0);
    Func_0808a010(30);
    Func_0808a110(1, 3);
    Func_0808a010(30);
    Func_0808a180(1, 0);
    Func_0808a010(10);
    Func_0808a1b8(3, 0x6000, 0);
    Func_0808a010(50);
    Func_0808a1b8(3, 0x2000, 0);
    Func_0808a010(35);
    Func_0808a1e8(3, 264, 50);
    Func_0808a010(10);
    Func_0808a110(3, 3);
    Func_0808a010(30);
    Func_0808a180(3, 0);
    Func_0808a010(10);
    Func_0808a138(2, 2);
    Func_0808a010(20);
    Func_0808a178(2, 0);

    if (Func_0808a070(-1, 0) == 0) {
        Func_0808a010(10);
        Func_0808a110(0, 33);
        Func_0808a010(20);
        *(u16 *)((u8 *)slotBase[0] + 0x1d8) += 1;
        Func_0808a1e8(1, 0x107, 40);
        Func_0808a180(1, 0);
    } else {
        branch = 0;
        Func_0808a010(10);
        Func_0808a110(0, 34);
        Func_0808a010(20);
        Func_0808a1e8(1, 0x107, 40);
        *(u16 *)((u8 *)slotBase[0] + 0x1d8) += 1;
        branch = 1;
        Func_0808a180(1, 0);
    }

    Func_0808a010(10);
    Func_0808a1b8(2, 0xa000, 0);
    Func_0808a010(30);
    Func_0808a110(2, 4);
    Func_0808a010(20);
    Func_0808a1b8(1, 0x2000, 0);
    Func_0808a010(30);
    Func_0808a138(1, 2);
    Func_0808a010(40);
    Func_0808a1b8(1, 0xe000, 0);
    Func_0808a1b8(2, 0xe000, 0);
    Func_0808a010(30);
    Func_0808a110(1, 3);
    Func_0808a010(30);
    Func_0808a180(1, 0);
    Func_0808a010(10);
    Func_0808a1b8(3, 0x6000, 0);
    Func_0808a010(50);
    Func_0808a1b8(3, 0x2000, 0);
    Func_0808a010(35);
    Func_0808a1e8(3, 264, 50);
    Func_0808a010(10);
    Func_0808a110(3, 3);
    Func_0808a010(30);
    Func_0808a180(3, 0);

    if (branch == 0) {
        Func_0808a010(10);
        Func_0808a110(2, 3);
        Func_0808a010(30);
        Func_0808a180(2, 0);
        Func_0808a010(10);
        Func_0808a138(1, 2);
        Func_0808a010(30);
        Func_0808a180(1, 0);
        *(u16 *)((u8 *)slotBase[0] + 0x1d8) += 2;
    } else {
        *(u16 *)((u8 *)slotBase[0] + 0x1d8) += 2;
        Func_0808a010(10);
        Func_0808a110(2, 4);
        Func_0808a010(20);
        Func_0808a180(2, 0);
        Func_0808a010(10);
        Func_0808a138(1, 2);
        Func_0808a010(20);
        Func_0808a180(1, 0);
    }

    Func_0808a010(10);
    Func_0808a1e8(0, 258, 60);
    Func_0808a010(10);
    Func_0808a110(1, 3);
    Func_0808a010(30);
    Func_0808a180(1, 0);
    Func_0808a010(20);
    Func_0808a150(2, 3, 40);
    Func_0808a1b8(3, 0x2000, 0);
    Func_0808a1b8(2, 0xe000, 0);
    Func_0808a010(30);

    /* Busy wait on the IWRAM status word at 0x03001ae8, pumping the frame
     * import until any of bits 4-7 is set. */
    while ((*(volatile u32 *)0x03001ae8 & 0xf0) == 0)
        Func_080000c0(1);

    Func_0808a128(0, 6, 0);
    Func_0808a090(0, 0x1e666, 0xf333);
    Func_0808a580(0, -32, -8);
    /* Same r0 carry idiom as the head. */
    Func_080091e0(Func_0808a080(0), 1);
    Func_0808a010(20);
    Func_0808a1b8(0, 0xc000, 0);
    Func_0808a1b8(3, 0x4000, 0);
    Func_0808a1b8(2, 0xc000, 0);
    Func_0808a010(20);
    Func_0808a010(10);
    Func_0808a138(3, 2);
    Func_0808a010(20);
    Func_0808a180(3, 0);
    Func_0808a010(10);
    Func_0808a138(1, 2);
    Func_0808a010(20);
    Func_0808a180(1, 0);
    Func_0808a010(20);
    Func_0808a110(2, 3);
    Func_0808a010(30);
    Func_0808a1b8(0, 0x4000, 0);
    Func_0808a1b8(1, 0x2000, 0);
    Func_0808a010(30);
    Func_0808a180(2, 0);
    Func_0808a010(20);
    Func_0808a100(0, 3);
    Func_0808a100(1, 3);
    Func_0808a110(3, 3);
    Func_0808a010(30);
    Func_0808a090(1, 0x13333, 0x9999);
    Func_0808a090(3, 0x13333, 0x9999);
    Func_0808a090(2, 0x13333, 0x9999);

    Func_0808a100(1, 2);
    record = Func_0808a080(0);
    if (record != 0)
        Func_0808a0b8(1, *(s16 *)(record + 10), *(s16 *)(record + 18));
    Func_0808a0e8(1);
    Func_0808a0f0(1, 0, 0);

    Func_0808a100(3, 2);
    record = Func_0808a080(0);
    if (record != 0)
        Func_0808a0b8(3, *(s16 *)(record + 10), *(s16 *)(record + 18));
    Func_0808a0e8(3);
    Func_0808a0f0(3, 0, 0);

    Func_0808a100(2, 2);
    record = Func_0808a080(0);
    if (record != 0)
        Func_0808a0b8(2, *(s16 *)(record + 10), *(s16 *)(record + 18));
    Func_0808a0e8(2);
    Func_0808a0f0(2, 0, 0);

    /* Reached with r0 = 10, not as the record query above. */
    Func_0808a010(10);
    Func_0808a020();
}
