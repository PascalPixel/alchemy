#include "types.h"
#include "party_state.h"
#include "scene_call.h"
#include "text/msg_ids.h"

struct DeriActor {
    u8 unknown_00[6];
    u16 facing;
};

struct DeriActor *Object_GetById(s32 id);
void Object_SetModeById(s32 actor, s32 mode);
void Func_02000ddc(s32 slot, s32 actor);
void Func_02000dec(s32 slot, s32 actor);
s32 Func_02000dd4(void);
void Func_02000de4(s32 actor);

void Func_02000270(s32 actor)
{
    u16 facing = Object_GetById(gPartyState.current_owner)->facing;

    /* FAKEMATCH: keeps the quadrant offset after the facing load. */
    __asm__("");

    if ((u16)((facing + 0x2000) & ~0x3fff) == 0xc000) {
        Func_02000ddc(1, actor);
    } else {
        Engine_EventBegin();
        Engine_EventResolvePendingActions(0);
        Engine_EventSetMessage((s32)MsgDeriHeyaSouthRoadDanger);
        Engine_EventShowMessage(actor, 0);
        Engine_EventEnd();
    }
}

void Func_020002d0(s32 actor)
{
    u16 facing = Object_GetById(gPartyState.current_owner)->facing;

    /* FAKEMATCH: keeps the quadrant offset after the facing load. */
    __asm__("");

    if ((u16)((facing + 0x2000) & ~0x3fff) == 0xc000) {
        Func_02000ddc(2, actor);
    } else {
        Engine_EventBegin();
        Engine_EventResolvePendingActions(0);
        Engine_EventSetMessage((s32)MsgDeriHeyaNewTravelers);
        Engine_EventShowMessage(actor, 0);
        Engine_EventEnd();
    }
}

void Func_02000330(s32 actor)
{
    u16 facing = Object_GetById(gPartyState.current_owner)->facing;

    /* FAKEMATCH: keeps the quadrant offset after the facing load. */
    __asm__("");

    if ((u16)((facing + 0x2000) & ~0x3fff) == 0x8000) {
        Func_02000dec(0, actor);
    } else {
        Engine_EventBegin();
        Engine_EventResolvePendingActions(0);
        Engine_EventSetMessage((s32)MsgDeriHeyaDinnerChoice);
        Engine_EventShowMessage(actor, 0);
        Engine_EventEnd();
    }
}

void Func_02000390(s32 actor)
{
    s32 msg;
    u16 facing = Object_GetById(gPartyState.current_owner)->facing;

    /* FAKEMATCH: keeps the quadrant offset after the facing load. */
    __asm__("");

    if ((u16)((facing + 0x2000) & ~0x3fff) == 0xc000) {
        if (GameFlag_Test(0x847) == 0) {
            Engine_EventBegin();
            Engine_EventResolvePendingActions(0);
            msg = (s32)MsgDeriHeyaTravelersFreeTime;
            Engine_EventSetMessage(msg);
            UiText_OpenMessageAtObject(actor, 0);
            if (Inventory_PromptAndSetObjectMode(Func_02000dd4(), 0) == 0) {
                Battle_WaitMode0(10);
                Engine_EventSetMessage(msg + 1);
            } else {
                Battle_WaitMode0(20);
                Engine_EventSetMessage(msg + 2);
            }
            Engine_EventShowMessage(actor, 0);
            Engine_EventEnd();
            GameFlag_SetBit(0x847);
        } else {
            Func_02000de4(actor);
        }
    } else {
        Engine_EventBegin();
        Engine_EventResolvePendingActions(0);
        msg = (s32)MsgDeriHeyaTravelersFreeTime;
        Engine_EventSetMessage(msg);
        UiText_OpenMessageAtObject(actor, 0);
        if (Inventory_PromptAndSetObjectMode(Func_02000dd4(), 0) == 0) {
            Battle_WaitMode0(10);
            Engine_EventSetMessage(msg + 1);
        } else {
            Battle_WaitMode0(20);
            Engine_EventSetMessage(msg + 2);
        }
        Engine_EventShowMessage(actor, 0);
        Engine_EventEnd();
        GameFlag_SetBit(0x847);
    }
}

void Func_02000498(s32 actor)
{
    u16 facing = Object_GetById(gPartyState.current_owner)->facing;

    /* FAKEMATCH: keeps the quadrant offset after the facing load. */
    __asm__("");

    if ((u16)((facing + 0x2000) & ~0x3fff) == 0xc000) {
        Func_02000de4(actor);
    } else {
        Engine_EventBegin();
        Engine_EventResolvePendingActions(0);
        Engine_EventSetMessage((s32)MsgDeriHeyaLeaveForMadora);
        Engine_EventShowMessage(actor, 0);
        Engine_EventEnd();
    }
}

void Func_020004f4(s32 actor)
{
    u16 facing = Object_GetById(gPartyState.current_owner)->facing;

    /* FAKEMATCH: keeps the quadrant offset after the facing load. */
    __asm__("");

    if ((u16)((facing + 0x2000) & ~0x3fff) == 0xc000) {
        Func_02000ddc(1, actor);
    } else {
        Engine_EventBegin();
        Engine_EventResolvePendingActions(0);
        Engine_EventSetMessage((s32)MsgDeriHeyaPirateTreasure);
        Engine_EventShowMessage(actor, 0);
        Engine_EventEnd();
    }
}

void Func_02000554(s32 actor)
{
    u16 facing = Object_GetById(gPartyState.current_owner)->facing;

    /* FAKEMATCH: keeps the quadrant offset after the facing load. */
    __asm__("");

    if ((u16)((facing + 0x2000) & ~0x3fff) == 0xc000) {
        Func_02000ddc(2, actor);
    } else {
        Engine_EventBegin();
        Engine_EventResolvePendingActions(0);
        Engine_EventSetMessage((s32)MsgDeriHeyaSpaciousVillage);
        Engine_EventShowMessage(actor, 0);
        Engine_EventEnd();
    }
}

void Func_020005b4(s32 actor)
{
    u16 facing = Object_GetById(gPartyState.current_owner)->facing;

    /* FAKEMATCH: keeps the quadrant offset after the facing load. */
    __asm__("");

    if ((u16)((facing + 0x2000) & ~0x3fff) == 0x8000) {
        Func_02000dec(0, actor);
    } else {
        Engine_EventBegin();
        Engine_EventResolvePendingActions(0);
        Engine_EventSetMessage((s32)MsgDeriHeyaFishermenWaiting);
        Engine_EventShowMessage(actor, 0);
        Engine_EventEnd();
    }
}

void Func_02000614(void)
{
    s32 msg;

    Engine_EventBegin();
    Engine_EventResolvePendingActions(0);
    msg = (s32)MsgDeriHeyaGoingToMadora;
    Engine_EventSetMessage(msg);
    UiText_OpenMessageAtObject(12, 0);
    if (Inventory_PromptAndSetObjectMode(Func_02000dd4(), 0) == 0) {
        Battle_WaitMode0(10);
        Engine_EventSetMessage(msg + 1);
    } else {
        Battle_WaitMode0(20);
        Engine_EventSetMessage(msg + 2);
    }
    Engine_EventShowMessage(12, 0);
    Engine_EventEnd();
}

void Func_0200066c(void)
{
    s32 msg;

    Engine_EventBegin();
    Engine_EventResolvePendingActions(0);
    msg = (s32)MsgDeriHeyaLookingForBoat;
    Engine_EventSetMessage(msg);
    UiText_OpenMessageAtObject(14, 0);
    if (Inventory_PromptAndSetObjectMode(Func_02000dd4(), 0) == 0) {
        Battle_WaitMode0(10);
        Engine_EventSetMessage(msg + 1);
    } else {
        Battle_WaitMode0(20);
        Engine_EventSetMessage(msg + 2);
    }
    Engine_EventShowMessage(14, 0);
    Engine_EventEnd();
}

void Func_020006c4(void)
{
    Engine_EventBegin();
    Engine_EventResolvePendingActions(0);
    Engine_EventSetMessage((s32)MsgDeriHeyaWatchingPractice);
    Engine_EventShowMessage(26, 0);
    Battle_WaitMode0(30);
    Object_SetModeById(26, 3);
    Battle_WaitMode0(30);
    Engine_EventShowMessage(26, 0);
    Engine_EventEnd();
}

