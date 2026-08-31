#include "types.h"

#define FieldScene_RunMiddleSequence Func_02000904

/* Audited retained field-scene body at 0x02000904.
 * The complete production body preserves 62 decoded calls, 0 loop(s), and 20 explicit memory operation(s).
 * Approved GCC changes scheduling, allocation, control-flow lowering, and
 * literal placement, so the reviewed assembly remains authoritative. */

void *Func_02001792();
void Func_0200179c();
void Func_020017ac();
void *Func_020017c4();
void *Func_020017ce();
void Func_020017d4();
void Func_020017dc();
void Func_020017fa();
s32 Func_02001806();
void Func_02001812();
void Func_02001832();
void Func_02001836();
void Func_02001842();
s32 Func_02001866();
void Func_02001878();
void Func_0200187e();
void Func_02001884();
void Func_0200189c();
void Func_020018a2();
void Func_020018a4();
void Func_020018a8();
void Func_020018b8();
s32 Func_020018c2();
void Func_020018d0();
void Func_02001900();
void Func_02001906();
void Func_02001918();
void Func_0200191a();
void Func_02001924();
void *Func_0200193e();
void Func_02001944();
void *Func_0200194e();
void Func_02001952();
void *Func_0200195c();
void *Func_0200196a();
void *Func_0200197a();
void Func_02001980();
void *Func_02001988();
void Func_02001990();
void Func_02001992();
void Func_02001998();
void Func_020019ae();
s32 Func_020019bc();
s32 Func_020019e8();
void *Func_02001a12();
s32 Func_02001a16();
void Func_02001a30();
void Func_02001a38();
void Func_02001a3c();
void *Func_02001a3e();
void Func_02001a40();
void Func_02001a52();
void Func_02001a68();
void *Func_02001a6c();
void Func_02001a6e();
void *Func_02001a7a();
void Func_02001a80();
s32 Func_02001a8c();
void Func_02001a96();
s32 Func_02001aa2();

void Func_02000904(void)
{
    void *p3;
    void *p34;
    void *p35;
    void *p36;
    void *p37;
    void *p38;
    void *p39;
    void *p5;
    void *p51;
    void *p55;
    void *p59;
    void *p6;
    void *p60;

    Func_020017d4(0);
    Func_0200179c(578);
    p3 = Func_02001792(64, 32, 0, 32, 32, 32);
    Func_020017ac(64, 32, 32, 32, 0, 0);
    p5 = Func_020017ce(577);
    p6 = Func_020017c4(64, 0, 0, 32, 32, 32);
    Func_020017dc(64, 0, 32, 32);
    Func_02001842(17);
    if (Func_02001806(576) != 0) {
        Func_020017fa(0, 64, 0, 32, 32, 32);
        Func_02001812(0, 64, 32, 32);
        Func_02001878(16);
        Func_0200187e(17);
        Func_02001884(21);
    } else {
        Func_02001836(0, 32, 32, 32);
        Func_0200189c(15);
        Func_020018a2(16);
        Func_020018a8(17);
    }
    if (Func_02001866(2303) != 0) {
        Func_020018b8(18);
    } else {
        Func_02001998(170);
        Func_02001918(18, 2);
        Func_02001900(18, 3);
        Func_02001832(33590537, 3200);
    }
    Func_020018b8(303);
    Func_020018a4(0, 33, 4, 3, 20, 41);
    if (Func_020018c2(2310) != 0) {
        Func_02001944(19, 23592960, 11010048);
    }
    Func_0200191a(19);
    Func_020018d0();
    Func_02001980(22, 15);
    Func_02001988(23, 15);
    Func_02001990(24, 15);
    p34 = Func_0200193e(22);
    *(u8 *)((u8 *)(p34) + 89) |= 0x8;
    p35 = Func_0200194e(23);
    *(u8 *)((u8 *)(p35) + 89) |= 0x8;
    p36 = Func_0200195c(24);
    *(u8 *)((u8 *)(p36) + 89) |= 0x8;
    p37 = Func_0200196a(22);
    *(u8 *)((u8 *)(p37) + 35) |= 0x2;
    p38 = Func_0200197a(23);
    *(u8 *)((u8 *)(p38) + 35) |= 0x2;
    p39 = Func_02001988(24);
    *(u8 *)((u8 *)(p39) + 35) |= 0x2;
    Func_02001a30(22, 1);
    Func_02001a38(23, 1);
    Func_02001a40(24, 1);
    Func_02001906(1);
    Func_02001992();
    Func_02001a6e();
    Func_02001952();
    Func_020019ae();
    Func_02001924(1);
    if (Func_020019bc(576) == 0) {
        Func_02001a3c(8, 52953088, 47644672);
        p51 = Func_02001a12(8);
        *(u16 *)(p51 + 6) = 12288;
        Func_02001a52(9, 52035584, 54067200, 12288);
    }
    if (Func_020019e8(577) == 0) {
        Func_02001a68(10, 36700160, 46530560);
        p55 = Func_02001a3e(10);
        *(u16 *)(p55 + 6) = 4096;
        Func_02001a80(11, 37748736, 46530560, 4096);
    }
    if (Func_02001a16(578) == 0) {
        Func_02001a96(15, 19333120, 48758784);
        p59 = Func_02001a6c(15);
        *(u16 *)(p59 + 6) = 45056;
    } else {
        p60 = Func_02001a7a(15);
        *(u8 *)((u8 *)(p60) + 89) |= 0x4;
    }
    if (Func_02001a8c(17) != 0) {
    }
    if (Func_02001aa2(16) != 0) {
    }
}
