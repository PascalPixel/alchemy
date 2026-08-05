#include "types.h"

/*
 * resource_3a8 owner at 0x02002008, 1,720 bytes: a linear scene cutscene
 * controlling entities 0, 2, 8 and 9, with one player-choice branch.
 *
 * The executable body starts at the saved-register prologue at 0x02002008 and
 * reaches its only interworking return at 0x020026a4.  The span also owns the
 * branch-skipped interior literal pool at 0x0200239c-0x020023bf and the
 * trailing pool at 0x020026a8-0x020026bf; the next owner starts with a fresh
 * prologue at 0x020026c0.  The eight bytes before this owner are the preceding
 * owner's pool and are deliberately not claimed here.
 *
 * All 160 BL sites were resolved independently with overlay_call_targets.ts.
 * Every site reaches the overlay's import veneer bank; the inventory below is
 * checked again by overlay_multiset_check.ts and overlay_call_order_check.ts.
 * The apparent return value in r0 is the saved link register popped into r0
 * immediately before `bx r0`, so this function is void and takes no inputs.
 *
 * Data_03001ebc is the scene-work pointer established by this overlay's exact-C
 * owner at 0x02003184.  This cutscene writes its +0x1c8 word at entry and exit,
 * advances the +0x1d8 halfword on either answer to the sole question, and
 * temporarily clears then restores bit 0 of entity bytes at +0x5a.  Numeric
 * message, sound, pose, coordinate and timing ids remain numeric because their
 * tables have not been reconstructed.
 */

u8 *Func_0808a080();
s32 Func_0808a070();
void Func_080000c0();
void Func_08009180();
void Func_0808a010();
void Func_0808a018();
void Func_0808a020();
void Func_0808a090();
void Func_0808a0c8();
void Func_0808a0d0();
void Func_0808a0f0();
void Func_0808a100();
void Func_0808a110();
void Func_0808a130();
void Func_0808a138();
void Func_0808a170();
void Func_0808a178();
void Func_0808a180();
void Func_0808a188();
void Func_0808a190();
void Func_0808a1b8();
void Func_0808a1e8();
void Func_0808a1f0();
void Func_0808a208();
void Func_0808a210();
void Func_0808a248();
void Func_0808a360();
void Func_0808a368();
void Func_0808a370();
void Func_080f9010();

extern u8 *Data_03001ebc;

void Func_02002008(void)
{
    u8 *entity;

    Func_0808a018();
    Func_0808a210(-1, -1, -1, 0);
    Func_080000c0(1);
    Func_0808a0f0(0, 0x00c00000, 0x01560000);
    Func_080000c0(1);
    Func_0808a208(0x3333, 0x666);
    Func_0808a210(0x00c00000, -1, 0x00fc0000, 1);

    Func_0808a360();
    *(s32 *)(Data_03001ebc + 0x1c8) = 40;
    Func_0808a090(0, 0xcccc, 0x6666);
    Func_0808a0d0(0, 192, 278);
    Func_0808a090(8, 0x10000, 0x8000);
    Func_0808a090(9, 0x10000, 0x8000);

    entity = Func_0808a080(8);
    *(u16 *)(entity + 6) = 0x3000;
    entity = Func_0808a080(9);
    *(u16 *)(entity + 6) = 0x5000;
    Func_080000c0(1);

    entity = Func_0808a080(8);
    entity = Func_0808a080(9);
    entity[0x5a] &= 0xfe;
    entity[0x5a] &= 0xfe;
    Func_0808a0c8(8, 184, 232);
    Func_0808a0d0(9, 198, 232);
    Func_0808a100(8, 1);
    Func_0808a010(20);

    entity = Func_0808a080(8);
    entity[0x5a] |= 1;
    entity = Func_0808a080(9);
    entity[0x5a] |= 1;
    Func_0808a010(20);
    Func_0808a110(8, 4);
    Func_0808a170(0x1b05);
    Func_0808a188(8, 0, 10);
    Func_0808a1b8(9, 0x5000, 10);
    Func_0808a110(9, 3);
    Func_0808a188(9, 0, 10);
    Func_0808a1f0(0, 0x102);
    Func_0808a010(60);

    entity = Func_0808a080(0);
    if (entity != 0) {
        Func_0808a0f0(2, *(s32 *)(entity + 8), *(s32 *)(entity + 16));
    }
    Func_0808a090(2, 0xcccc, 0x6666);
    Func_0808a0d0(2, 212, 268);
    Func_0808a1b8(0, 0xe000, 0);
    Func_0808a1b8(2, 0x6000, 20);
    Func_0808a188(0x4002, 0, 20);
    Func_0808a1b8(0, 0xc000, 0);
    Func_0808a0d0(2, 202, 254);
    Func_0808a010(20);
    Func_0808a110(2, 3);
    Func_0808a010(10);
    Func_0808a188(2, 0, 10);

    Func_0808a1e8(8, 0x101, 0);
    Func_0808a1e8(9, 0x101, 40);
    Func_0808a1b8(8, 0, 0);
    Func_0808a1b8(9, 0x8000, 40);
    Func_0808a1b8(8, 0x3000, 0);
    Func_0808a1b8(9, 0x3000, 20);
    Func_0808a1e8(2, 0x102, 60);
    Func_0808a138(2, 1);
    Func_0808a188(2, 0, 10);
    Func_0808a138(8, 2);
    Func_0808a188(8, 0, 10);
    Func_0808a1e8(8, 0x105, 60);
    Func_0808a130(8, 2);
    Func_0808a188(8, 0, 10);
    Func_0808a138(9, 2);
    Func_0808a188(9, 0, 20);
    Func_0808a138(2, 1);
    Func_0808a010(10);
    Func_0808a1b8(2, 0x6000, 10);
    Func_0808a190(0x4002, 0);
    Func_0808a010(10);
    Func_0808a138(8, 1);
    Func_0808a110(8, 4);
    Func_0808a188(8, 0, 10);
    Func_0808a1e8(2, 0x101, 40);
    Func_0808a1b8(2, 0xa000, 20);
    Func_0808a138(9, 1);
    Func_0808a110(9, 3);
    Func_0808a1b8(2, 0xc000, 0);
    Func_0808a188(9, 0, 10);
    Func_0808a1b8(2, 0x6000, 60);
    Func_0808a1b8(2, 0xc000, 10);
    Func_0808a188(2, 0, 10);
    Func_0808a1b8(8, 0, 0);
    Func_0808a1b8(9, 0x8000, 40);
    Func_0808a1b8(8, 0x3000, 0);
    Func_0808a1b8(9, 0x3000, 10);
    Func_0808a138(8, 1);
    Func_0808a178(8, 0);
    Func_0808a1b8(2, 0x6000, 0);

    if (Func_0808a070(0, 0) == 0) {
        Func_0808a138(2, 2);
        Func_0808a188(0x4002, 0, 10);
        ++*(u16 *)(Data_03001ebc + 0x1d8);
    } else {
        ++*(u16 *)(Data_03001ebc + 0x1d8);
        Func_0808a110(2, 4);
        Func_0808a188(0x4002, 0, 10);
    }

    Func_0808a138(9, 2);
    Func_0808a188(9, 0, 10);
    Func_0808a1b8(2, 0xc000, 10);
    Func_0808a110(2, 4);
    Func_0808a188(2, 0, 10);
    Func_0808a1f0(8, 0x102);
    Func_0808a010(60);
    Func_0808a188(8, 0, 10);
    Func_0808a1b8(2, 0xa000, 10);
    Func_0808a110(2, 3);
    Func_0808a188(2, 0, 10);
    Func_0808a1f0(8, 0x102);
    Func_0808a1f0(9, 0x102);
    Func_0808a010(60);
    Func_0808a138(8, 2);
    Func_0808a188(8, 0, 10);
    Func_0808a138(9, 2);
    Func_0808a1b8(9, 0x8000, 10);
    Func_0808a188(9, 0, 10);
    Func_0808a1b8(8, 0, 10);
    Func_0808a110(8, 3);
    Func_0808a1b8(8, 0x3000, 10);
    Func_0808a180(8, 0);
    Func_0808a1b8(9, 0x3000, 10);
    Func_0808a110(2, 3);
    Func_0808a1b8(2, 0x6000, 10);
    Func_0808a190(0x4002, 0);
    Func_0808a010(10);
    Func_0808a138(2, 1);
    Func_0808a188(0x4002, 0, 10);
    Func_0808a110(0, 3);
    Func_0808a110(2, 3);
    Func_0808a1b8(2, 0xc000, 10);
    Func_0808a110(2, 3);

    Func_0808a090(8, 0xcccc, 0x6666);
    Func_0808a090(9, 0xcccc, 0x6666);
    entity = Func_0808a080(8);
    *(u16 *)(entity + 6) = 0;
    *(u16 *)(entity + 6) = 0x8000;
    entity = Func_0808a080(9);
    entity = Func_0808a080(8);
    entity[0x5a] &= 0xfe;
    entity = Func_0808a080(9);
    entity[0x5a] &= 0xfe;
    Func_0808a0c8(8, 168, 232);
    Func_0808a0d0(9, 212, 232);
    Func_0808a100(8, 1);
    Func_0808a010(20);
    entity = Func_0808a080(8);
    entity[0x5a] |= 1;
    entity = Func_0808a080(9);
    entity[0x5a] |= 1;

    Func_0808a0d0(2, 192, 232);
    Func_0808a1b8(2, 0xc000, 0);
    Func_080f9010(188);
    Func_08009180(36, 23, 43, 12, 2, 2);
    Func_080000c0(5);
    Func_08009180(39, 23, 43, 12, 2, 2);
    Func_080000c0(5);
    Func_0808a0d0(2, 192, 222);
    Func_0808a0f0(2, 0, 0);
    Func_0808a090(0, 0x13333, 0x9999);
    Func_0808a0d0(0, 192, 222);
    Func_0808a0f0(0, 0, 0);
    Func_0808a100(8, 3);
    Func_0808a110(9, 3);
    Func_0808a0c8(8, 184, 232);
    Func_0808a0d0(9, 198, 232);
    Func_0808a0c8(8, 188, 212);
    Func_0808a0d0(9, 194, 212);
    Func_0808a0f0(8, 0, 0);
    Func_0808a0f0(9, 0, 0);

    *(s32 *)(Data_03001ebc + 0x1c8) = 24;
    *(s32 *)(Data_03001ebc + 0x1c0) = 0x201;
    Func_0808a368();
    Func_0808a370();
    Func_0808a248(5);
    Func_0808a020();
}
