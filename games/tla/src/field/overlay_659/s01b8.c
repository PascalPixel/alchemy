#include "ov659.h"

void Func_020001b8(s32 actor)
{
    u16 facing = Object_GetById(gPartyState.current_owner)->angle;

    /* FAKEMATCH: the one-pass block keeps the quadrant offset after the facing load; as a plain if the ARM9TDMI schedule builds 0x2000 first (differ 2). */
    do {
        if ((u16)((facing + 0x2000) & ~0x3fff) == 0xc000) {
            Func_02001f00(actor);
        } else {
            if (FlagChk(0x8ff))
                Func_02001e18((s32)MsgScene659No149);
            else if (FlagChk(0x8ab))
                Func_02001e18((s32)MsgScene659No81);
            else
                Func_02001e18((s32)MsgScene659No33);
            Func_02001e30(actor, 0);
        }
    } while (0);
}

void Func_0200023c(void)
{
    Func_02001d50();
    Func_02001eb8(0);
    ObjectMotion_SetAngleToward(14, 4, 0);
    ObjectMotion_Launch(14, 6, 20);
    ObjectMotion_SetAngleToward(4, 14, 0);
    Func_02001e18((s32)MsgScene659No18);
    Func_02001e30(14, 0);
    ActorTurn(14, 0x2000, 0);
    Func_02001d58();
}

void Func_0200028c(void)
{
    s32 msg;

    if (FlagChk(0x98a)) {
        if (FlagChk(0x310))
            msg = (s32)MsgScene659No111;
        else
            msg = (s32)MsgScene659No118;
        Func_02001e18(msg);
        Func_02001e30(10, 0);
    } else {
        if (FlagChk(0x210)) {
            Func_02001e18((s32)MsgScene659No116);
            Func_02001e30(10, 0);
        } else {
            Func_02001e18((s32)MsgScene659No106);
            Func_02001e30(10, 0);
        }
        FIELD_AT(Ram_HeapSlots->event_work, u16, 346) = 1;
    }
}

void Func_02000308(void)
{
    if (FlagChk(0x98a)) {
        if (FlagChk(0x310)) {
            Func_02001e18((s32)MsgScene659No112);
            Func_02001e30(11, 0);
        } else {
            Func_02001e18((s32)MsgScene659No119);
            Func_02001e30(11, 0);
        }
    } else if (FlagChk(0x210)) {
        Func_02001e18((s32)MsgScene659No117);
        Func_02001e30(11, 0);
    } else {
        Func_02001e18((s32)MsgScene659No107);
        Func_02001e30(11, 0);
    }
}

void Func_02000368(void)
{
    if (FlagChk(0x98a)) {
        if (FlagChk(0x310)) {
            Func_02001e18((s32)MsgScene659No113);
            Func_02001e30(10, 0);
        } else {
            Func_02001e18((s32)MsgScene659No120);
            Func_02001e30(10, 0);
        }
    } else {
        s32 flag = FlagChk(0x210);
        s32 msg = (s32)MsgScene659No108;

        if (flag) {
            Func_02001e18(msg);
            Func_02001e30(10, 0);
        } else {
            Func_02001e18(msg);
            Func_02001e30(10, 0);
        }
    }
}

void Func_020003c8(void)
{
    if (FlagChk(0x98a)) {
        if (FlagChk(0x310)) {
            Func_02001e18((s32)MsgScene659No114);
            Func_02001e30(11, 0);
        } else {
            Func_02001e18((s32)MsgScene659No121);
            Func_02001e30(11, 0);
        }
    } else {
        s32 flag = FlagChk(0x210);
        s32 msg = (s32)MsgScene659No109;

        if (flag) {
            Func_02001e18(msg);
            Func_02001e30(11, 0);
        } else {
            Func_02001e18(msg);
            Func_02001e30(11, 0);
        }
    }
}

void Func_02000428(void)
{
    if (!FlagChk(0x211)) {
        FlagSet(0x211);
        Func_02001e18((s32)MsgUhhhnnnUhhhnnn2);
        ObjectMotion_CommitPositionAndActivate(14, -12, 0);
        Battle_WaitMode0(20);
        Motion_SetVarCbAndRefresh(14, 2);
        Battle_WaitMode0(20);
        Func_02001e30(14, 0);
        Func_02001e48(14, 265, 30);
        Battle_WaitMode0(20);
        ObjectMotion_CommitPositionAndActivate(14, 12, 0);
        ObjectMotion_SetAngleToward(14, 4, 0);
        Func_02001e30(14, 0);
    } else {
        Func_02001e18((s32)MsgScene659No97);
        ObjectMotion_SetAngleToward(14, 4, 0);
        Func_02001e30(14, 0);
    }
}

const u8 *Func_020004c4(void)
{
    if (gPartyState.scene == (s32)&SceneId_Ov659Room2)
        return Data_02002c9c;
    if (gPartyState.entrance == 20)
        return Data_02002bb8;
    if (FlagChk(0x8ff))
        return Data_0200308c;
    if (FlagChk(0x8ab))
        return Data_02002da4;
    return Data_020028a0;
}

void Func_02000534(void)
{
    if (FlagChk(0x8ab)) {
        ActorPlace(26, 0x3780000, 0x80000);
    } else {
        Func_02001d38(Object_GetById(26), 242);
        ObjectDispatch_SetSingleChildFlags(Object_GetById(26), 0);
    }
}
