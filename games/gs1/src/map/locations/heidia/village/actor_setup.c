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

static __inline__ void Call1(void (*f)(), s32 value)
{
    f(value);
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
