#include "types.h"
#include "object_runtime.h"
extern u8 Data_03001ebc[];
void Func_0200a812();
s32 Func_0200a94c();
void Func_02005fbc();
void Func_0200a962();
void Func_0200a96a();
s32 Func_0200a968();
void Func_0200aa06();
void Func_0200a98c();
s32 Func_0200a982();
void Func_0200aa20();
void Func_02007658();
s32 Func_0200a99a();
void Func_0200aa38();
void Func_0200a9c0();
s32 Func_0200a9b6();
void Func_0200761a();
s32 Func_0200a9c4();
void Func_0200765c();
void Func_0200a9f0();
struct ObjectRuntime *Func_0200aa16();
struct ObjectRuntime *Func_0200aa2a();
struct ObjectRuntime *Village_GetActor();
struct ObjectRuntime *Func_0200aa52();
void Func_0200a9f6();
struct ObjectRuntime *Func_0200aa6c();
struct ObjectRuntime *Func_0200aa84();
void Func_0200aa22();
void Func_0200aa76();
void Village_FinishActorRestore();
s32 Func_0200aa86(s32);
void Func_02006360(s32);
s32 Func_0200aa96(s32);
void Func_02006370(s32);
s32 Func_0200aaa8(s32);
void Func_02006382(s32);
s32 Func_0200aac8(s32);
void Func_020062f2(s32);
s32 Func_0200aad8(s32);
void Func_02006302(s32);
s32 Func_0200aaf2(s32);
void Func_02006240(s32);
s32 Func_0200ab02(s32);
void Func_02006250(s32);
s32 Func_0200ab12(s32);
void Func_02006260(s32);

extern s16 Data_02000240[];
extern u8 Data_02009151[], Data_0200938d[], Data_02009a45[];
void Func_0200a598();
void Func_0200a7d0();
void Func_0200a7d8();
void Func_0200a7e0();
s32 Func_0200a716();
s32 Func_0200a72a();
s32 Func_0200a73e();
s32 Func_0200a752();
void Func_0200a7b4();
void Func_0200a7c8();
void Func_0200a7dc();
void Func_0200a7f0();
void Func_0200a7f8();
void Func_0200a816();
struct ObjectRuntime *Func_0200a874();
void Func_0200a812_setup();
s32 Func_0200a842();
void Func_0200a8e4();
void Func_0200a854();
void Func_0200a7f6();
void Func_0200a7f4();
void Func_0200a828();
void Func_0200a7fe();
s32 Func_0200a89c();
void Func_02007300();
void Func_02007314();
void Func_0200a844();
void Func_0200a8c4();
struct ObjectRuntime *Func_0200a922();
struct ObjectRuntime *Func_0200a92a();
void Func_0200a8c8();
void Func_0200a9f8();

static __inline__ void Call1(void (*f)(), s32 value)
{
    f(value);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

void FieldScene_SetupActorsForScene(void)
{
    struct ObjectRuntime *actor;

    Func_0200a598();
    Func_0200a7d0(9, 1);
    Func_0200a7d8(10, 1);
    Func_0200a7e0(17, 1);
    if (Func_0200a716(0x94c)) {
        Func_0200a7b4(15, 0, 0);
    }
    if (Func_0200a72a(0x949)) {
        Func_0200a7c8(11, 0, 0);
    }
    if (Func_0200a73e(0x94b)) {
        Func_0200a7dc(16, 0, 0);
    }
    if (Func_0200a752(0xf2e)) {
        Func_0200a7f0(8, 0, 0);
    }
    switch (Data_02000240[225]) {
    case 1:
    case 2:
    case 3:
        *(s32 *)(*(u8 **)Data_03001ebc + 0x1c0) = 0x200;
        Func_0200a7f8(0xe00);
        Func_0200a7f6(Data_02009151, 3200);
        Func_0200a7f4(1);
        Func_0200a828();
        Func_0200a7fe(1);
        break;
    case 10:
    case 13:
    case 20:
    case 23:
    case 24:
        *(s32 *)(*(u8 **)Data_03001ebc + 0x1c0) = 0x209;
        Func_0200a816(0xc00);
        Func_0200a812_setup(Func_0200a874(24), 0);
        if (Func_0200a842(0x314)) {
            Call3(Func_0200a8e4, 25, 0x3680000, 0x780000);
        }
        break;
    case 21:
    case 22:
        *(s32 *)(*(u8 **)Data_03001ebc + 0x1c0) = 0x200;
        Func_0200a854(0xe00);
        Func_0200a7f6(Data_0200938d, 3200);
        Func_0200a7f4(1);
        Func_0200a828();
        Func_0200a7fe(1);
        break;
    case 11:
    case 12:
        *(s32 *)(*(u8 **)Data_03001ebc + 0x1c0) = 0x200;
        if (Func_0200a89c(0x94a)) {
            Func_02007300();
        }
        break;
    case 31:
        *(s32 *)(*(u8 **)Data_03001ebc + 0x1c0) = 0x200;
        Func_02007314();
        break;
    case 14:
    case 15:
    case 16:
        Func_0200a844(Data_02009a45, 3200);
        break;
    default:
        *(s32 *)(*(u8 **)Data_03001ebc + 0x1c0) = 0x200;
        Func_0200a8c4(0xe00);
        break;
    }
    actor = Func_0200a922(8);
    Func_0200a8c8(Func_0200a92a(8), 0);
    Func_0200a9f8(8, 1);
    *(s32 *)&actor->unknown_18[0] = 0xc000;
    *(s32 *)&actor->unknown_18[4] = 0xc000;
}

void FieldScene_RestoreActorsFromFlags(void)
{
    struct ObjectRuntime *actor;

    *(s32 *)(*(u8 **)Data_03001ebc + 0x1c0) = 0x200;
    Func_0200a812();
    if (Func_0200a94c(0x943)) {
        Func_02005fbc();
    }
    Call1(Func_0200a962, 0x217);
    Call1(Func_0200a96a, 0x218);
    if (Func_0200a968(0x944)) {
        Func_0200aa06(8, 0, 0);
        Call1(Func_0200a98c, 0x217);
    }
    if (Func_0200a982(0x945)) {
        Func_0200aa20(9, 0, 0);
        Func_02007658();
    }
    if (Func_0200a99a(0x946)) {
        Func_0200aa38(10, 0, 0);
        Call1(Func_0200a9c0, 0x218);
    }
    if (Func_0200a9b6(0x947)) {
        Func_0200761a();
    }
    if (Func_0200a9c4(0x948)) {
        Func_0200765c();
    }
    Func_0200a9f0();
    actor = Func_0200aa16(8);
    if (actor != 0) {
        actor->unknown_23 = 2;
    }
    actor = Func_0200aa2a(9);
    if (actor != 0) {
        actor->unknown_23 = 2;
    }
    actor = Village_GetActor(10);
    if (actor != 0) {
        actor->unknown_23 = 2;
    }
    actor = Func_0200aa52(11);
    if (actor != 0) {
        Func_0200a9f6(actor, 0);
    }
    actor->unknown_23 = 2;
    actor = Func_0200aa6c(12);
    if (actor != 0) {
        actor->unknown_56[3] |= 0x10;
    }
    Func_0200aa22(Func_0200aa84(11), 0);
    Func_0200aa76();
    Village_FinishActorRestore(0xe00);
}

void FieldScene_ActivateThreeActorGroup(void)
{
    if (Func_0200aa86(0x35a)) {
        Func_02006360(0);
    }
    if (Func_0200aa96(0x35b)) {
        Func_02006370(1);
    }
    if (Func_0200aaa8(0x35c)) {
        Func_02006382(2);
    }
}

void FieldScene_ActivateTwoActorGroup(void)
{
    if (Func_0200aac8(0x358)) {
        Func_020062f2(0);
    }
    if (Func_0200aad8(0x359)) {
        Func_02006302(1);
    }
}

void FieldScene_ActivateAlternateActorGroup(void)
{
    if (Func_0200aaf2(0x355)) {
        Func_02006240(0);
    }
    if (Func_0200ab02(0x356)) {
        Func_02006250(1);
    }
    if (Func_0200ab12(0x357)) {
        Func_02006260(2);
    }
}
