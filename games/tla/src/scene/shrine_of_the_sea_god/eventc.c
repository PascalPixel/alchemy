#include "shrine_of_the_sea_god.h"
#include "call.h"

// Actor 14, linked to the leader, walks to (280, 432); then flags 0x876 and
// 0x877 are set.
void Func_020024e4(void)
{
    struct ShrineOfTheSeaGodObject *object = Object_GetById(13);
    s32                             angle;

    Func_020039d8();
    Func_02003b08(0);
    Value3((s32 (*)())Func_02003aa8, gPartyState.currentOwner, 0x100, 0);
    Func_02003ad0()->motionFlags = 0;
    ShrineOfTheSeaGod_CamMove(328 << 16, 32 << 16, 552 << 16, 1);
    Func_02003ac8();
    /* FAKEMATCH: the angle in a local is built before the coordinates, as the native does (differ 8 as constants). */
    angle = 0xc000;
    ShrineOfTheSeaGod_ObjPlaceTurn(14, 328 << 16, 624 << 16, angle);
    WaitFrames(1);
    Object_LinkObjectAndSetCallback(gPartyState.currentOwner, 14);
    object->priorityFlags &= ~2;
    ObjectMotion_SetActionVariant(14, 3);
    ShrineOfTheSeaGod_ObjSpeed(14, 0x19999, 0xcccc);
    ShrineOfTheSeaGod_ObjWalk(14, 328, 568);
    ShrineOfTheSeaGod_ObjWalk(14, 328, 496);
    ShrineOfTheSeaGod_ObjWalk(14, 280, 432);
    ObjectMotion_EnableActionAndResetMotion(gPartyState.currentOwner);
    WaitFrames(1);
    angle = 0x4000;
    ShrineOfTheSeaGod_ObjPlaceTurn(14, 280 << 16, 328 << 16, angle);
    ObjectMotion_SetActionVariant(14, 2);
    Object_GetById(14)->priorityFlags |= 1;
    {
        s32 bit = 2;

        /* FAKEMATCH: ⚓️ builds the bit before it loads the flags and fills the load's delay with WaitFrames' argument; plain, the load comes first and the argument last (differ 4). */
        asm volatile("" : "+l"(bit));
        object->priorityFlags |= bit;
    }
    WaitFrames(1);
    ShrineOfTheSeaGod_FlagSet(0x876);
    ShrineOfTheSeaGod_FlagSet(0x877);
    Func_020039e0();
}

void Func_020025fc(void)
{
    s32 done = ShrineOfTheSeaGod_FlagCheck(0x876);

    if (done == 0)
    {
        Func_020039d8();
        Func_02003b08(0);
        Func_02003aa8(gPartyState.currentOwner, 0x100, 0);
        Func_02003ad0()->motionFlags = done;
        ShrineOfTheSeaGod_CamMove(328 << 16, 48 << 16, 552 << 16, 1);
        Func_02003ac8();
        /* FAKEMATCH: reusing the old flag result for the angle retains the native early constant construction; the plain unbound angle differs 2. */
        done = 0x4000;
        ShrineOfTheSeaGod_ObjPlaceTurn(14, 424 << 16, 520 << 16, done);
        WaitFrames(1);
        Object_LinkObjectAndSetCallback(gPartyState.currentOwner, 14);
        ObjectMotion_SetActionVariant(14, 1);
        ShrineOfTheSeaGod_ObjSpeed(14, 0x19999, 0xcccc);
        ShrineOfTheSeaGod_ObjWalk(14, 380, 552);
        ShrineOfTheSeaGod_ObjWalk(14, 264, 552);
        ShrineOfTheSeaGod_ObjWalk(14, 264, 540);
        ObjectMotion_EnableActionAndResetMotion(gPartyState.currentOwner);
        WaitFrames(1);
        ShrineOfTheSeaGod_ObjPlace(14, 0, 0);
        ObjectMotion_SetActionVariant(14, 2);
        Object_GetById(14)->priorityFlags |= 1;
        WaitFrames(1);
        ShrineOfTheSeaGod_FlagSet(0x876);
        Func_020039e0();
    }
}

void Func_020026f8(void)
{
    u8 off;

    if (GameFlag_Test(0x877))
    {
        Func_020039d8();
        Func_02003b08(0);
        off = 0;
        Func_02003ad0()->motionFlags = off;
        ShrineOfTheSeaGod_CamMove(320 << 16, -1, 300 << 16, 1);
        ShrineOfTheSeaGod_Jump(14, 0x100, 40);
        ShrineOfTheSeaGod_ObjSpeed(14, 0x13333, 0x9999);
        ShrineOfTheSeaGod_ObjWalk(14, 345, 296);
        ShrineOfTheSeaGod_ObjWalk(14, 392, 296);
        Object_LinkObjectAndSetCallback(14, gPartyState.currentOwner);
        Object_GetById(14)->state = off;
        WaitFrames(10);
        GameFlag_SetBit(0x87c);
        Func_020039e0();
    }
}

void Func_0200279c(void)
{
    Func_020039d8();
    Func_02003b08(0);
    Func_02003ad0()->motionFlags = 0;
    ObjectMotion_EnableActionAndResetMotion(14);
    WaitFrames(1);
    ShrineOfTheSeaGod_ObjSpeed(14, 0x26666, 0x13333);
}

void Func_020027d4(void)
{
    Object_LinkObjectAndSetCallback(14, gPartyState.currentOwner);
    Battle_WaitMode0(5);
    Func_020039e0();
}

void Func_020027f8(void)
{
    Func_0200279c();
    ShrineOfTheSeaGod_CamMove(392 << 16, 16 << 16, 216 << 16, 1);
    ShrineOfTheSeaGod_ObjWalk(14, 392, 216);
    Object_GetById(14)->state = 1;
    Func_020027d4();
}

void Func_02002830(void)
{
    struct ShrineOfTheSeaGodObject *block = Object_GetById(14);
    u8                             *state;
    s32                             flag;

    Func_0200279c();
    ShrineOfTheSeaGod_CamMove(360 << 16, 16 << 16, 216 << 16, 1);
    ShrineOfTheSeaGod_ObjWalk(14, 360, 216);
    flag = GameFlag_Test(0x207);
    state = &block->state;
    if (flag)
    {
        block->facing = 0x8000;
        ShrineOfTheSeaGod_ObjEmote(14, 258);
        ShrineOfTheSeaGod_Wait(20);
        *state = 4;
        GameFlag_SetBit(0x206);
    }
    else
    {
        ShrineOfTheSeaGod_Wait(5);
        ShrineOfTheSeaGod_CamMove(296 << 16, 16 << 16, 216 << 16, 1);
        Func_02003b48(153);
        ObjectDispatch_SetSingleChildFlags(Object_GetById(14), 0);
        ShrineOfTheSeaGod_ObjSpeed(14, 0x4cccc, 0x26666);
        block->speed = 0x80000;
        ObjectMotion_SetPositionAndCommit(14, 328, 216);
        ObjectDispatch_SetSingleChildFlags(Object_GetById(14), 1);
        ShrineOfTheSeaGod_Wait(10);
        ShrineOfTheSeaGod_ObjSpeed(14, 0x19999, 0xcccc);
        ShrineOfTheSeaGod_ObjWalk(14, 296, 216);
        *state = 2;
    }
    Func_020027d4();
}

void Func_0200291c(void)
{
    Func_0200279c();
    ShrineOfTheSeaGod_CamMove(280 << 16, 16 << 16, 296 << 16, 1);
    ShrineOfTheSeaGod_ObjWalk(14, 296, 248);
    ShrineOfTheSeaGod_ObjWalk(14, 280, 248);
    ShrineOfTheSeaGod_ObjWalk(14, 280, 296);
    Object_GetById(14)->state = 3;
    Func_020027d4();
}

void Func_0200296c(void)
{
    Func_0200279c();
    ShrineOfTheSeaGod_CamMove(392 << 16, 16 << 16, 296 << 16, 1);
    ShrineOfTheSeaGod_ObjWalk(14, 392, 296);
    Object_GetById(14)->state = 0;
    Func_020027d4();
}

void Func_020029a4(void)
{
    Func_0200279c();
    ShrineOfTheSeaGod_CamMove(280 << 16, 16 << 16, 296 << 16, 1);
    ShrineOfTheSeaGod_ObjWalk(14, 280, 296);
    Object_GetById(14)->state = 3;
    Func_020027d4();
}

void Func_020029dc(void)
{
    Func_0200279c();
    ShrineOfTheSeaGod_CamMove(296 << 16, 16 << 16, 216 << 16, 1);
    ShrineOfTheSeaGod_ObjWalk(14, 280, 248);
    ShrineOfTheSeaGod_ObjWalk(14, 296, 248);
    ShrineOfTheSeaGod_ObjWalk(14, 296, 216);
    Object_GetById(14)->state = 2;
    Func_020027d4();
}

void Func_02002a2c(void)
{
    struct ShrineOfTheSeaGodObject *block = Object_GetById(14);
    u8                             *state;
    s32                             flag;

    Func_0200279c();
    ShrineOfTheSeaGod_CamMove(328 << 16, 16 << 16, 216 << 16, 1);
    ShrineOfTheSeaGod_ObjWalk(14, 328, 216);
    flag = GameFlag_Test(0x207);
    state = &block->state;
    if (flag)
    {
        block->facing = 0;
        ShrineOfTheSeaGod_ObjEmote(14, 258);
        ShrineOfTheSeaGod_Wait(20);
        *state = 5;
        GameFlag_SetBit(0x206);
    }
    else
    {
        ShrineOfTheSeaGod_Wait(5);
        ShrineOfTheSeaGod_CamMove(392 << 16, 16 << 16, 216 << 16, 1);
        Func_02003b48(153);
        ObjectDispatch_SetSingleChildFlags(Object_GetById(14), 0);
        ShrineOfTheSeaGod_ObjSpeed(14, 0x4cccc, 0x26666);
        block->speed = 0x80000;
        ObjectMotion_SetPositionAndCommit(14, 360, 216);
        ObjectDispatch_SetSingleChildFlags(Object_GetById(14), 1);
        ShrineOfTheSeaGod_Wait(10);
        ShrineOfTheSeaGod_ObjSpeed(14, 0x19999, 0xcccc);
        ShrineOfTheSeaGod_ObjWalk(14, 392, 216);
        *state = 1;
    }
    Func_020027d4();
}

void Func_02002b18(void)
{
    Func_0200279c();
    ShrineOfTheSeaGod_CamMove(392 << 16, 16 << 16, 296 << 16, 1);
    ShrineOfTheSeaGod_ObjWalk(14, 392, 296);
    Object_GetById(14)->state = 0;
    Func_020027d4();
}

void Func_02002b50(void)
{
    struct ShrineOfTheSeaGodObject *block = Object_GetById(14);
    u8                             *state;
    s32                             flag;

    Func_0200279c();
    flag = GameFlag_Test(0x207);
    state = &block->state;
    if (flag)
    {
        block->facing = 0x8000;
        ShrineOfTheSeaGod_ObjEmote(14, 258);
        ShrineOfTheSeaGod_Wait(20);
        *state = 4;
        GameFlag_SetBit(0x206);
    }
    else
    {
        ShrineOfTheSeaGod_CamMove(296 << 16, 16 << 16, 216 << 16, 1);
        Func_02003b48(153);
        ObjectDispatch_SetSingleChildFlags(Object_GetById(14), 0);
        ShrineOfTheSeaGod_ObjSpeed(14, 0x4cccc, 0x26666);
        block->speed = 0x80000;
        ObjectMotion_SetPositionAndCommit(14, 328, 216);
        ObjectDispatch_SetSingleChildFlags(Object_GetById(14), 1);
        ShrineOfTheSeaGod_Wait(10);
        ShrineOfTheSeaGod_ObjSpeed(14, 0x19999, 0xcccc);
        ShrineOfTheSeaGod_ObjWalk(14, 296, 216);
        *state = 2;
    }
    Func_020027d4();
}

void Func_02002c18(void)
{
    Func_0200279c();
    ShrineOfTheSeaGod_CamMove(392 << 16, 16 << 16, 216 << 16, 1);
    ShrineOfTheSeaGod_ObjWalk(14, 392, 216);
    Object_GetById(14)->state = 1;
    Func_020027d4();
}

void Func_02002c50(void)
{
    Func_0200279c();
    ShrineOfTheSeaGod_CamMove(296 << 16, 16 << 16, 216 << 16, 1);
    ShrineOfTheSeaGod_ObjWalk(14, 296, 216);
    Object_GetById(14)->state = 2;
    Func_020027d4();
}

void Func_02002c88(void)
{
    struct ShrineOfTheSeaGodObject *block = Object_GetById(14);
    u8                             *state;
    s32                             flag;

    Func_0200279c();
    flag = GameFlag_Test(0x207);
    state = &block->state;
    if (flag)
    {
        block->facing = 0;
        ShrineOfTheSeaGod_ObjEmote(14, 258);
        ShrineOfTheSeaGod_Wait(20);
        *state = 5;
        GameFlag_SetBit(0x206);
    }
    else
    {
        ShrineOfTheSeaGod_CamMove(392 << 16, 16 << 16, 216 << 16, 1);
        Func_02003b48(153);
        ObjectDispatch_SetSingleChildFlags(Object_GetById(14), 0);
        ShrineOfTheSeaGod_ObjSpeed(14, 0x4cccc, 0x26666);
        block->speed = 0x80000;
        ObjectMotion_SetPositionAndCommit(14, 360, 216);
        ObjectDispatch_SetSingleChildFlags(Object_GetById(14), 1);
        ShrineOfTheSeaGod_Wait(10);
        ShrineOfTheSeaGod_ObjSpeed(14, 0x19999, 0xcccc);
        ShrineOfTheSeaGod_ObjWalk(14, 392, 216);
        *state = 1;
    }
    Func_020027d4();
}
