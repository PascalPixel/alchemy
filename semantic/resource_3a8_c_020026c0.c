#include "types.h"

/*
 * resource_3a8 owner at 0x020026c0, 2,756 bytes: a second linear scene
 * cutscene, staging entities 0-3 and 8-11 through two camera phases and a
 * single answer-dependent interlude.
 *
 * Complete owner: the saved-register prologue starts at 0x020026c0 and the
 * sole interworking return is at 0x02003154.  The span includes the interior
 * pools at 0x02002ae0-0x02002b1b and 0x02002d90-0x02002da3, both skipped by
 * unconditional branches, plus the trailing pool at 0x02003158-0x02003183.
 * The next exact-C owner starts at 0x02003184.
 *
 * All 249 BL sites resolve to 37 import veneers.  The source keeps their
 * machine order and reproduces the few non-call effects: four randomized
 * entity counters, three nullable position copies, the answer-state increment,
 * one scene-object byte clear, two scene phase words, and entity 9's halfword.
 * Numeric coordinates, script pointers, sounds and presentation ids remain
 * numeric where their tables have not been reconstructed.
 */

s32 Func_080000f8();
s32 Func_0808a070();
u8 *Func_0808a080();
u8 *Func_0808a228();
void Func_080000c0();
void Func_08009128();
void Func_08009180();
void Func_080091c0();
void Func_080091e0();
void Func_0808a010();
void Func_0808a018();
void Func_0808a020();
void Func_0808a090();
void Func_0808a098();
void Func_0808a0a8();
void Func_0808a0b0();
void Func_0808a0c8();
void Func_0808a0d0();
void Func_0808a0f0();
void Func_0808a100();
void Func_0808a110();
void Func_0808a128();
void Func_0808a130();
void Func_0808a138();
void Func_0808a170();
void Func_0808a178();
void Func_0808a188();
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
extern u8 Data_0200c21c[];
extern u8 Data_0200c230[];

void Func_020026c0(void)
{
    u8 *entity;
    u8 *object;

    Func_0808a018();
    Func_0808a210(-1, -1, -1, 0);
    Func_080000c0(1);
    Func_0808a0f0(0, 0, 0);
    Func_0808a0f0(9, 0x01b80000, 0x020a0000);
    entity = Func_0808a080(9);
    Func_080091e0(entity, 0);
    Func_0808a210(0x01b80000, -1, 0x020a0000, 0);
    Func_08009128();
    Func_080000c0(1);
    Func_0808a360();

    Func_0808a208(0x19999, 0x3333);
    Func_0808a210(0x01b80000, -1, 0x01900000, 1);
    Func_080f9010(141);
    Func_0808a090(9, 0x19999, 0xcccc);
    Func_0808a0d0(9, 440, 400);
    Func_0808a208(0xc000, 0x1800);
    Func_0808a210(0x01b80000, -1, 0x012c0000, 1);
    Func_0808a090(9, 0xcccc, 0x6666);
    Func_0808a0d0(9, 440, 300);
    Func_0808a100(9, 0);
    Func_080f9010(0x121);
    Func_0808a010(40);

    Func_0808a090(11, 0xcccc, 0x6666);
    Func_0808a0f0(11, 0x01b70000, 0x01320000);
    Func_0808a128(11, 4, 0);
    Func_0808a0d0(11, 0x1b7, 312);
    Func_0808a0d0(11, 416, 312);
    Func_0808a0d0(11, 400, 256);
    Func_0808a1b8(11, 0x3000, 40);
    Func_0808a210(0x019a0000, -1, 0x01180000, 1);

    Func_0808a090(10, 0x9999, 0x4ccc);
    Func_0808a0f0(10, 0x01b70000, 0x01320000);
    Func_0808a128(10, 4, 0);
    Func_0808a0d0(10, 0x1b7, 312);
    Func_0808a0d0(10, 416, 312);
    Func_0808a0d0(10, 388, 270);
    Func_0808a1b8(10, 0xd000, 10);
    Func_0808a1b8(11, 0x5000, 10);

    Func_0808a090(0, 0xcccc, 0x6666);
    Func_0808a0f0(0, 0x01b70000, 0x01320000);
    Func_0808a128(0, 4, 0);
    Func_0808a0d0(0, 0x1b7, 312);
    Func_0808a0d0(0, 416, 312);
    Func_0808a0d0(0, 388, 300);
    Func_0808a1b8(0, 0xc000, 40);
    entity = Func_0808a080(0);
    *(u16 *)(entity + 102) = (u16)(((u32)Func_080000f8() * 5) >> 12);
    Func_0808a098(0, Data_0200c21c);

    Func_0808a128(11, 2, 20);
    Func_0808a110(11, 3);
    Func_0808a170(0x256f);
    Func_0808a188(11, 0, 10);
    Func_0808a110(10, 3);

    entity = Func_0808a080(0);
    if (entity != 0)
        Func_0808a0f0(1, *(s32 *)(entity + 8), *(s32 *)(entity + 16));
    entity = Func_0808a080(0);
    if (entity != 0)
        Func_0808a0f0(2, *(s32 *)(entity + 8), *(s32 *)(entity + 16));
    entity = Func_0808a080(0);
    if (entity != 0)
        Func_0808a0f0(3, *(s32 *)(entity + 8), *(s32 *)(entity + 16));

    Func_0808a090(1, 0xcccc, 0x6666);
    Func_0808a090(2, 0xcccc, 0x6666);
    Func_0808a090(3, 0xcccc, 0x6666);
    Func_0808a0c8(1, 378, 310);
    Func_0808a0c8(2, 400, 288);
    Func_0808a0d0(3, 410, 308);
    Func_0808a100(1, 1);
    Func_0808a100(2, 1);
    Func_0808a1b8(1, 0xc000, 0);
    Func_0808a1b8(2, 0xc000, 0);
    Func_0808a1b8(3, 0xc000, 40);

    entity = Func_0808a080(1);
    *(u16 *)(entity + 102) = (u16)(((u32)Func_080000f8() * 5) >> 12);
    entity = Func_0808a080(2);
    *(u16 *)(entity + 102) = (u16)(((u32)Func_080000f8() * 5) >> 12);
    entity = Func_0808a080(3);
    *(u16 *)(entity + 102) = (u16)(((u32)Func_080000f8() * 5) >> 12);
    Func_0808a098(1, Data_0200c21c);
    Func_0808a098(2, Data_0200c21c);
    Func_0808a098(3, Data_0200c21c);
    Func_0808a1e8(2, 0x101, 60);
    Func_0808a188(0x2002, 0, 10);
    Func_0808a1e8(10, 0x100, 40);
    Func_0808a1b8(10, 0x3000, 20);
    Func_0808a188(10, 0, 10);
    Func_0808a0a8(0);
    Func_0808a0a8(1);
    Func_0808a0a8(2);
    Func_0808a0a8(3);
    Func_080000c0(1);

    Func_0808a1b8(2, 0xc000, 10);
    Func_0808a110(2, 3);
    Func_0808a1b8(0, 0xc000, 0);
    Func_0808a1b8(1, 0xc000, 0);
    Func_0808a1b8(3, 0xc000, 20);
    Func_0808a110(10, 3);
    Func_0808a188(10, 0, 10);
    Func_0808a1e8(3, 0x101, 60);
    Func_0808a188(0x2003, 0, 10);
    Func_0808a110(10, 3);
    Func_0808a188(10, 0, 10);
    Func_0808a1e8(1, 0x101, 60);
    Func_0808a188(1, 0, 10);
    Func_0808a1b8(10, 0x5000, 0);
    Func_0808a138(11, 2);
    Func_0808a188(11, 0, 10);
    Func_0808a1b8(0, 0x6000, 0);
    Func_0808a1b8(1, 0xe000, 0);
    Func_0808a1b8(2, 0x2000, 0);
    Func_0808a1b8(3, 0xa000, 20);
    Func_0808a100(0, 3);
    Func_0808a100(1, 3);
    Func_0808a100(2, 3);
    Func_0808a110(3, 3);
    Func_0808a010(10);

    Func_0808a1b8(0, 0xc000, 0);
    Func_0808a1b8(1, 0xc000, 0);
    Func_0808a1b8(2, 0xc000, 0);
    Func_0808a1b8(3, 0xc000, 10);
    Func_0808a1b8(10, 0xd000, 10);
    Func_0808a110(10, 3);
    Func_0808a188(10, 0, 10);
    Func_0808a110(11, 3);
    Func_0808a1f0(2, 0x102);
    Func_0808a010(40);
    Func_0808a130(2, 2);
    Func_0808a188(0x2002, 0, 10);
    Func_0808a1b8(11, 0x3000, 10);
    Func_0808a100(11, 4);
    Func_0808a188(11, 0, 10);
    Func_0808a1e8(3, 0x101, 40);
    Func_0808a188(0x2003, 0, 10);
    Func_0808a1b8(10, 0x3000, 10);
    Func_0808a1e8(10, 0x108, 20);
    Func_0808a188(10, 0, 10);
    Func_0808a138(1, 2);
    Func_0808a188(1, 0, 10);
    Func_0808a1b8(11, 0x5000, 0);
    Func_0808a1b8(10, 0x5000, 10);
    Func_0808a100(11, 4);
    Func_0808a188(11, 0, 10);
    Func_0808a1b8(0, 0x6000, 0);
    Func_0808a1b8(1, 0xe000, 40);
    Func_0808a1b8(0, 0xc000, 0);
    Func_0808a1b8(1, 0xc000, 10);
    Func_0808a110(11, 4);
    Func_0808a188(11, 0, 10);
    Func_0808a110(10, 4);
    Func_0808a188(10, 0, 10);

    Func_0808a1e8(0, 0x105, 0);
    Func_0808a1e8(1, 0x105, 0);
    Func_0808a1e8(2, 0x105, 0);
    Func_0808a1e8(3, 0x105, 60);
    Func_0808a1b8(10, 0x3000, 10);
    Func_0808a100(10, 3);
    Func_0808a178(10, 0);
    Func_0808a1b8(1, 0xe000, 0);
    Func_0808a1b8(2, 0x6000, 0);
    Func_0808a1b8(3, 0xa000, 0);

    if (Func_0808a070(0, 0) == 0) {
        Func_0808a010(20);
        *(u16 *)(Data_03001ebc + 0x1d8) += 3;
    } else {
        Func_0808a010(20);
        Func_0808a138(11, 2);
        Func_0808a188(11, 0, 40);
        Func_0808a188(11, 0, 10);
        Func_0808a1e8(3, 0x106, 40);
        Func_0808a1b8(3, 0xc000, 20);
        Func_0808a188(0x2003, 0, 10);
    }

    Func_0808a1b8(1, 0xc000, 0);
    Func_0808a1b8(2, 0xc000, 0);
    Func_0808a1b8(3, 0xc000, 10);
    Func_0808a100(1, 3);
    Func_0808a100(2, 3);
    Func_0808a110(3, 3);
    Func_0808a010(20);
    Func_0808a1b8(8, 0x3000, 0);
    Func_0808a110(10, 3);

    Func_0808a208(0x10000, 0x2000);
    Func_0808a210(0x01180000, -1, 0x00c80000, 1);
    Func_0808a0d0(10, 0x14d, 222);
    Func_0808a0d0(10, 284, 198);
    Func_0808a1b8(10, 0x8000, 10);
    Func_0808a110(10, 3);
    Func_0808a1b8(8, 0, 10);
    Func_0808a110(8, 3);
    Func_0808a090(8, 0xcccc, 0x6666);
    Func_0808a0d0(8, 268, 198);
    Func_0808a1b8(8, 0xc000, 10);
    Func_0808a138(8, 2);
    Func_080f9010(125);
    Func_08009180(71, 60, 76, 11, 2, 1);
    Func_080091c0(71, 60, 2, 1, 16, 11);
    Func_0808a010(20);
    Func_0808a0d0(8, 246, 198);
    Func_0808a1b8(8, 0, 20);
    object = Func_0808a228();
    object[85] = 0;

    Func_0808a208(0x9999, 0x1333);
    Func_0808a210(0x00f80000, -1, 0x00aa0000, 1);
    Func_0808a0d0(10, 270, 198);
    Func_0808a0d0(10, 270, 174);
    Func_0808a0d0(10, 224, 170);
    Func_0808a0d0(10, 210, 158);
    Func_0808a0d0(10, 246, 148);
    Func_0808a0d0(10, 246, 142);
    Func_0808a0f0(10, 0, 0);
    *(s32 *)(Data_03001ebc + 0x1c0) = 0x202;
    Func_0808a368();
    Func_0808a370();

    Func_0808a0f0(9, 0x01b80000, 0x01540000);
    entity = Func_0808a080(9);
    *(u16 *)(entity + 6) = 0x4000;
    Func_0808a210(0x017c0000, -1, 0x01180000, 0);
    Func_08009128();
    Func_080000c0(10);
    Func_0808a360();
    Func_0808a370();
    Func_0808a010(40);
    Func_0808a110(11, 3);
    Func_0808a010(20);
    Func_0808a100(0, 3);
    Func_0808a100(1, 3);
    Func_0808a100(2, 3);
    Func_0808a110(3, 3);

    Func_0808a0d0(11, 420, 282);
    Func_0808a0d0(11, 420, 312);
    Func_0808a0d0(11, 0x1b7, 312);
    Func_0808a0d0(11, 0x1b7, 306);
    Func_0808a0f0(11, 0, 0);
    Func_0808a098(1, Data_0200c230);
    Func_0808a098(2, Data_0200c230);
    Func_0808a0b0(3, Data_0200c230);
    Func_0808a210(0x019a0000, -1, 0x012c0000, 1);
    Func_0808a0d0(0, 416, 312);
    Func_0808a0d0(0, 0x1b7, 312);
    Func_0808a0d0(0, 0x1b7, 306);
    Func_0808a0f0(0, 0, 0);
    Func_080f9010(141);

    Func_0808a090(9, 0xcccc, 0x6666);
    Func_0808a208(0x10000, 0x2000);
    Func_0808a210(0x01b80000, -1, 0x01a40000, 1);
    Func_0808a0d0(9, 440, 420);
    Func_0808a208(0x20000, 0x4000);
    Func_0808a210(0x01b80000, -1, 0x02580000, 1);
    Func_0808a090(9, 0x19999, 0xcccc);
    Func_0808a0d0(9, 440, 500);
    Func_0808a0c8(9, 440, 600);
    Func_080f9010(0x121);

    *(s32 *)(Data_03001ebc + 0x1c8) = 24;
    *(s32 *)(Data_03001ebc + 0x1c0) = 0x100;
    Func_0808a368();
    Func_0808a370();
    Func_0808a248(10);
    Func_0808a020();
}
