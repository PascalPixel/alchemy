#include "types.h"

#define NULL ((void *)0)
#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))
#define ActorPresentation_SetupActorEightForFlag301 Func_0200025c
#define ActorPresentation_SetupActorNineForFlag302 Func_02000314
#define ActorPresentation_SetupActorTenForFlag303 Func_0200034c
#define ActorPresentation_SetupActorNineForFlag306 Func_020004bc
#define ActorPresentation_SetupActorTenForFlag307 Func_02000528
#define ActorPresentation_SetupActorEightForFlags308And309Guarded Func_02000668
#define ActorPresentation_SetupActorEightForFlags308And309 Func_020006c0
#define ActorPresentation_SetupActorNineForFlag30a Func_020006fc
#define ActorPresentation_SetupActorTenForFlags30bAnd30d Func_020009d0
#define ActorPresentation_SetupActorElevenAt0_112 Func_02000c78
#define ActorPresentation_SetupActorElevenAt0_64 Func_02000ca8
#define ActorPresentation_SetupActorElevenAt0_80 Func_02000cd8
#define ActorPresentation_SetupActorEightForFlag313 Func_02000d38
#define ActorPresentation_SetupActorNineForFlag314 Func_02000dd8
#define ActorPresentation_SetupActorTenForFlag315 Func_02000e0c
#define ActorPresentation_AdvanceActorEightStates Func_02000e78
#define ActorPresentation_AdvanceActorTenStates Func_02000ea8
#define SceneActor_SetActor11Values1And2 Func_02000ec0

void Func_02002672(s32);
void Func_020003ec(s32, s32, s32);
s32 Func_020003f6(s32, s32, s32);
void Func_0200268c(s32);
void Func_020025a2(s32);
void Func_020024e8(s32);
void Func_020012dc(void);
void Func_0200272a(s32);
s32 Func_020004a6(s32, s32, s32);
void Func_0200273c(s32);
void Func_02002652(s32);
void Func_02002598(s32);
void Func_0200138c(void);
void Func_02002762(s32);
s32 Func_020004dc(s32, s32, s32);
void Func_02002772(s32);
void Func_02002690(s32);
void Func_020025ce(s32);
void Func_020013c2(void);
void Func_020028d2(s32);
s32 Func_0200064c(s32, s32, s32);
void Func_020028e2(s32);
void Func_020027f8(s32);
void Func_0200273e(s32);
void Func_02001636(void);
void Func_0200293e(s32);
void Func_020006b8(s32, s32, s32);
void Func_020006c2(s32, s32, s32);
void Func_02002958(s32);
void Func_02002876(s32);
void Func_020027b4(s32);
void Func_020016ac(void);
s32 Func_02002988(s32);
s32 Func_02002992(s32);
void Func_02002a94(s32);
void Func_020029ac(s32);
void Func_020029ba(s32);
void Func_0200081e(s32, s32, s32);
void Func_02002ab4(s32);
void Func_0200290a(s32);
void Func_0200197a(void);
void Func_0200085e(s32, s32, s32);
void Func_020019ba();
void Func_0200294a(s32);
void Func_020029f6(s32);
void Func_020029fc(s32);
void Func_02002ad6(s32);
void Func_02002af4(s32);
void Func_02002b12(s32);
s32 Func_0200088e(s32, s32, s32);
void Func_02002b24(s32);
void Func_02002a3a(s32);
void Func_02002980(s32);
void Func_020019f0(void);
void Func_02002de6(s32);
s32 Func_02000b62(s32, s32, s32);
void Func_02002df8(s32);
void Func_02002d0e(s32);
void Func_02002d1c(s32);
void Func_02002c5a(s32);
void Func_02001cca(void);
void Func_0200308e(s32);
s32 Func_02000e08(s32, s32, s32);
void Func_0200309e(s32);
void Func_0200185a(void);
void Func_02002ef8(s32);
void Func_02001f68(void);
void Func_020030be(s32);
s32 Func_02000e38(s32, s32, s32);
void Func_020030ce(s32);
void Func_0200188a(void);
void Func_02002f28(s32);
void Func_02001f98(void);
void Func_020030ee(s32);
s32 Func_02000e68(s32, s32, s32);
void Func_020030fe(s32);
void Func_020018ba(void);
void Func_02002f58(s32);
void Func_02001fc8(void);
void Func_0200314e(s32);
s32 Func_02000ec8(s32, s32, s32);
void Func_0200315e(s32);
void Func_02003074(s32);
void Func_02002fba(s32);
void Func_0200233a(void);
void Func_020031ee(s32);
s32 Func_02000f68(s32, s32, s32);
void Func_020031fe(s32);
void Func_0200311e(s32);
void Func_0200305c(s32);
void Func_020023dc(void);
void Func_02003222(s32);
void Func_02000f9c(s32, s32, s32);
void Func_02003232(s32);
void Func_02003148(s32);
void Func_0200308e_a(s32);
void Func_0200240e(void);
void Func_020031f8(s32, s32);
void Func_02003200(s32, s32);
void Func_02003228(s32, s32);
void Func_02003230(s32, s32);
void Func_02003240(s32, s32);
void Func_02003248(s32, s32);

void ActorPresentation_SetupActorEightForFlag301(void)
{
    Func_02002672(0xF1);
    Func_020003ec(8, 0x70, 0);
    Func_020003f6(8, 0x70, 0);
    Func_0200268c(0x121);
    Func_020025a2(0x301);
    Func_020024e8(2);
    Func_020012dc();
}

void ActorPresentation_SetupActorNineForFlag302(void)
{
    Func_0200272a(0xF1);
    Func_020004a6(9, 0, -64);
    Func_0200273c(0x121);
    Func_02002652(0x302);
    Func_02002598(2);
    Func_0200138c();
}

void ActorPresentation_SetupActorTenForFlag303(void)
{
    Func_02002762(0xF1);
    Func_020004dc(0xA, 0, 0x40);
    Func_02002772(0x121);
    Func_02002690(0x303);
    Func_020025ce(2);
    Func_020013c2();
}

void ActorPresentation_SetupActorNineForFlag306(void)
{
    Func_020028d2(0xF1);
    Func_0200064c(9, 0, 0x40);
    Func_020028e2(0x121);
    Func_020027f8(0x306);
    Func_0200273e(2);
    Func_02001636();
}

void ActorPresentation_SetupActorTenForFlag307(void)
{
    Func_0200293e(0xF1);
    Func_020006b8(10, 0, 144);
    Func_020006c2(10, 0, 128);
    Func_02002958(0x121);
    Func_02002876(0x307);
    Func_020027b4(2);
    Func_020016ac();
}

void ActorPresentation_SetupActorEightForFlags308And309Guarded(void)
{
    /* movs r0,#0xc4 / lsls r0,#2 builds 0x310. */
    if (Func_02002988((s32) 0x310) != 0) {
        return;
    }
    if (Func_02002992((s32) 0x30D) != 0) {
        return;
    }

    Func_02002a94((s32) 0xF1);
    /* movs r0,#0xc2 / lsls r0,#2 builds 0x308. */
    Func_020029ac((s32) 0x308);
    Func_020029ba((s32) 0x309);
    /* movs r1,#0x30 / negs r1,r1 */
    Func_0200081e(8, -48, 0);
    Func_02002ab4((s32) 0x121);
    Func_0200290a(2);
    Func_0200197a();
}

void ActorPresentation_SetupActorEightForFlags308And309(void)
{
    Func_02002ad6(0xF1);
    Func_020029f6(0x308);
    Func_020029fc(0x309);
    Func_0200085e(8, 0x60, 0);
    Func_02002af4(0x121);
    Func_0200294a(2);
    Func_020019ba();
}

void ActorPresentation_SetupActorNineForFlag30a(void)
{
    Func_02002b12(0xF1);
    Func_0200088e(9, -32, 0);
    Func_02002b24(0x121);
    Func_02002a3a(0x30A);
    Func_02002980(2);
    Func_020019f0();
}

void ActorPresentation_SetupActorTenForFlags30bAnd30d(void)
{
    Func_02002de6(0xF1);
    Func_02000b62(0xA, 0, -64);
    Func_02002df8(0x121);
    Func_02002d0e(0x30B);
    Func_02002d1c(0x30D);
    Func_02002c5a(2);
    Func_02001cca();
}

void ActorPresentation_SetupActorElevenAt0_112(void)
{
    Func_0200308e(241);
    Func_02000e08(11, 0, 112);
    Func_0200309e(0x121);
    Func_0200185a();
    Func_02002ef8(2);
    Func_02001f68();
}

void ActorPresentation_SetupActorElevenAt0_64(void)
{
    Func_020030be(0xF1);
    Func_02000e38(0xB, 0, 0x40);
    Func_020030ce(0x121);
    Func_0200188a();
    Func_02002f28(2);
    Func_02001f98();
}

void ActorPresentation_SetupActorElevenAt0_80(void)
{
    Func_020030ee(0xF1);
    Func_02000e68(0xB, 0, 0x50);
    Func_020030fe(0x121);
    Func_020018ba();
    Func_02002f58(2);
    Func_02001fc8();
}

void ActorPresentation_SetupActorEightForFlag313(void)
{
    Func_0200314e(0xF1);
    Func_02000ec8(8, 0, 0x70);
    Func_0200315e(0x121);
    Func_02003074(0x313);
    Func_02002fba(2);
    Func_0200233a();
}

void ActorPresentation_SetupActorNineForFlag314(void)
{
    Func_020031ee(0xF1);
    Func_02000f68(9, 0x80, 0);
    Func_020031fe(0x121);
    Func_0200311e(0x314);
    Func_0200305c(2);
    Func_020023dc();
}

void ActorPresentation_SetupActorTenForFlag315(void)
{
    Func_02003222(241);
    Func_02000f9c(10, 160, 0);
    Func_02003232(0x121);
    Func_02003148(0x315);
    Func_0200308e_a(2);
    Func_0200240e();
}

void ActorPresentation_AdvanceActorEightStates(void)
{
    Func_020031f8(8, 1);
    Func_02003200(8, 2);
}

void ActorPresentation_AdvanceActorTenStates(void)
{
    Func_02003228(10, 1);
    Func_02003230(10, 2);
}

void SceneActor_SetActor11Values1And2(void)
{
    Func_02003240(11, 1);
    Func_02003248(11, 2);
}
