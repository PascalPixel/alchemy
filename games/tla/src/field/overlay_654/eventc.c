#include "kabe.h"

void Func_020025fc(void)
{
    s32 done = Kabe_FlagChk(0x876);

    if (done == 0) {
        Func_020039d8();
        Func_02003b08(0);
        Func_02003aa8(gPartyState.current_owner, 0x100, 0);
        Func_02003ad0()->motion_flags = done;
        Kabe_CamMove(328 << 16, 48 << 16, 552 << 16, 1);
        Func_02003ac8();
        Kabe_ObjPlaceTurn(14, 424 << 16, 520 << 16, 0x4000);
        WaitFrames(1);
        Object_LinkObjectAndSetCallback(gPartyState.current_owner, 14);
        ObjectMotion_SetActionVariant(14, 1);
        Kabe_ObjSpeed(14, 0x19999, 0xcccc);
        Kabe_ObjWalk(14, 380, 552);
        Kabe_ObjWalk(14, 264, 552);
        Kabe_ObjWalk(14, 264, 540);
        ObjectMotion_EnableActionAndResetMotion(gPartyState.current_owner);
        WaitFrames(1);
        Kabe_ObjPlace(14, 0, 0);
        ObjectMotion_SetActionVariant(14, 2);
        Object_GetById(14)->priority_flags |= 1;
        WaitFrames(1);
        Kabe_FlagSet(0x876);
        Func_020039e0();
    }
}

void Func_020026f8(void)
{
    u8 off;

    if (GameFlag_Test(0x877)) {
        Func_020039d8();
        Func_02003b08(0);
        off = 0;
        Func_02003ad0()->motion_flags = off;
        Kabe_CamMove(320 << 16, -1, 300 << 16, 1);
        Kabe_Jump(14, 0x100, 40);
        Kabe_ObjSpeed(14, 0x13333, 0x9999);
        Kabe_ObjWalk(14, 345, 296);
        Kabe_ObjWalk(14, 392, 296);
        Object_LinkObjectAndSetCallback(14, gPartyState.current_owner);
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
    Func_02003ad0()->motion_flags = 0;
    ObjectMotion_EnableActionAndResetMotion(14);
    WaitFrames(1);
    Kabe_ObjSpeed(14, 0x26666, 0x13333);
}

void Func_020027d4(void)
{
    Object_LinkObjectAndSetCallback(14, gPartyState.current_owner);
    Battle_WaitMode0(5);
    Func_020039e0();
}

void Func_020027f8(void)
{
    Func_0200279c();
    Kabe_CamMove(392 << 16, 16 << 16, 216 << 16, 1);
    Kabe_ObjWalk(14, 392, 216);
    Object_GetById(14)->state = 1;
    Func_020027d4();
}

void Func_02002830(void)
{
    struct KabeObject *block = Object_GetById(14);
    u8 *state;
    s32 flag;

    Func_0200279c();
    Kabe_CamMove(360 << 16, 16 << 16, 216 << 16, 1);
    Kabe_ObjWalk(14, 360, 216);
    flag = GameFlag_Test(0x207);
    state = &block->state;
    if (flag) {
        block->facing = 0x8000;
        Kabe_ObjEmote(14, 258);
        Kabe_Wait(20);
        *state = 4;
        GameFlag_SetBit(0x206);
    } else {
        Kabe_Wait(5);
        Kabe_CamMove(296 << 16, 16 << 16, 216 << 16, 1);
        Func_02003b48(153);
        ObjectDispatch_SetSingleChildFlags(Object_GetById(14), 0);
        Kabe_ObjSpeed(14, 0x4cccc, 0x26666);
        block->speed = 0x80000;
        ObjectMotion_SetPositionAndCommit(14, 328, 216);
        ObjectDispatch_SetSingleChildFlags(Object_GetById(14), 1);
        Kabe_Wait(10);
        Kabe_ObjSpeed(14, 0x19999, 0xcccc);
        Kabe_ObjWalk(14, 296, 216);
        *state = 2;
    }
    Func_020027d4();
}

void Func_0200291c(void)
{
    Func_0200279c();
    Kabe_CamMove(280 << 16, 16 << 16, 296 << 16, 1);
    Kabe_ObjWalk(14, 296, 248);
    Kabe_ObjWalk(14, 280, 248);
    Kabe_ObjWalk(14, 280, 296);
    Object_GetById(14)->state = 3;
    Func_020027d4();
}

void Func_0200296c(void)
{
    Func_0200279c();
    Kabe_CamMove(392 << 16, 16 << 16, 296 << 16, 1);
    Kabe_ObjWalk(14, 392, 296);
    Object_GetById(14)->state = 0;
    Func_020027d4();
}

void Func_020029a4(void)
{
    Func_0200279c();
    Kabe_CamMove(280 << 16, 16 << 16, 296 << 16, 1);
    Kabe_ObjWalk(14, 280, 296);
    Object_GetById(14)->state = 3;
    Func_020027d4();
}

void Func_020029dc(void)
{
    Func_0200279c();
    Kabe_CamMove(296 << 16, 16 << 16, 216 << 16, 1);
    Kabe_ObjWalk(14, 280, 248);
    Kabe_ObjWalk(14, 296, 248);
    Kabe_ObjWalk(14, 296, 216);
    Object_GetById(14)->state = 2;
    Func_020027d4();
}

void Func_02002a2c(void)
{
    struct KabeObject *block = Object_GetById(14);
    u8 *state;
    s32 flag;

    Func_0200279c();
    Kabe_CamMove(328 << 16, 16 << 16, 216 << 16, 1);
    Kabe_ObjWalk(14, 328, 216);
    flag = GameFlag_Test(0x207);
    state = &block->state;
    if (flag) {
        block->facing = 0;
        Kabe_ObjEmote(14, 258);
        Kabe_Wait(20);
        *state = 5;
        GameFlag_SetBit(0x206);
    } else {
        Kabe_Wait(5);
        Kabe_CamMove(392 << 16, 16 << 16, 216 << 16, 1);
        Func_02003b48(153);
        ObjectDispatch_SetSingleChildFlags(Object_GetById(14), 0);
        Kabe_ObjSpeed(14, 0x4cccc, 0x26666);
        block->speed = 0x80000;
        ObjectMotion_SetPositionAndCommit(14, 360, 216);
        ObjectDispatch_SetSingleChildFlags(Object_GetById(14), 1);
        Kabe_Wait(10);
        Kabe_ObjSpeed(14, 0x19999, 0xcccc);
        Kabe_ObjWalk(14, 392, 216);
        *state = 1;
    }
    Func_020027d4();
}

void Func_02002b18(void)
{
    Func_0200279c();
    Kabe_CamMove(392 << 16, 16 << 16, 296 << 16, 1);
    Kabe_ObjWalk(14, 392, 296);
    Object_GetById(14)->state = 0;
    Func_020027d4();
}

void Func_02002b50(void)
{
    struct KabeObject *block = Object_GetById(14);
    u8 *state;
    s32 flag;

    Func_0200279c();
    flag = GameFlag_Test(0x207);
    state = &block->state;
    if (flag) {
        block->facing = 0x8000;
        Kabe_ObjEmote(14, 258);
        Kabe_Wait(20);
        *state = 4;
        GameFlag_SetBit(0x206);
    } else {
        Kabe_CamMove(296 << 16, 16 << 16, 216 << 16, 1);
        Func_02003b48(153);
        ObjectDispatch_SetSingleChildFlags(Object_GetById(14), 0);
        Kabe_ObjSpeed(14, 0x4cccc, 0x26666);
        block->speed = 0x80000;
        ObjectMotion_SetPositionAndCommit(14, 328, 216);
        ObjectDispatch_SetSingleChildFlags(Object_GetById(14), 1);
        Kabe_Wait(10);
        Kabe_ObjSpeed(14, 0x19999, 0xcccc);
        Kabe_ObjWalk(14, 296, 216);
        *state = 2;
    }
    Func_020027d4();
}

void Func_02002c18(void)
{
    Func_0200279c();
    Kabe_CamMove(392 << 16, 16 << 16, 216 << 16, 1);
    Kabe_ObjWalk(14, 392, 216);
    Object_GetById(14)->state = 1;
    Func_020027d4();
}

void Func_02002c50(void)
{
    Func_0200279c();
    Kabe_CamMove(296 << 16, 16 << 16, 216 << 16, 1);
    Kabe_ObjWalk(14, 296, 216);
    Object_GetById(14)->state = 2;
    Func_020027d4();
}

void Func_02002c88(void)
{
    struct KabeObject *block = Object_GetById(14);
    u8 *state;
    s32 flag;

    Func_0200279c();
    flag = GameFlag_Test(0x207);
    state = &block->state;
    if (flag) {
        block->facing = 0;
        Kabe_ObjEmote(14, 258);
        Kabe_Wait(20);
        *state = 5;
        GameFlag_SetBit(0x206);
    } else {
        Kabe_CamMove(392 << 16, 16 << 16, 216 << 16, 1);
        Func_02003b48(153);
        ObjectDispatch_SetSingleChildFlags(Object_GetById(14), 0);
        Kabe_ObjSpeed(14, 0x4cccc, 0x26666);
        block->speed = 0x80000;
        ObjectMotion_SetPositionAndCommit(14, 360, 216);
        ObjectDispatch_SetSingleChildFlags(Object_GetById(14), 1);
        Kabe_Wait(10);
        Kabe_ObjSpeed(14, 0x19999, 0xcccc);
        Kabe_ObjWalk(14, 392, 216);
        *state = 1;
    }
    Func_020027d4();
}
