#include "garoh_interiors.h"

/* FAKEMATCH: the arguments are set up as for a call whose value is used (flags before speaker); the opener is declared void in scene_call.h for the scenes that match with that. */
#define Message_Open(speaker, flags) ((s32 (*)(s32, s32))UiText_OpenMessageAtObject)(speaker, flags)

void Func_02000318(s32 actor)
{
    s32 message;

    Func_020007f4();
    Func_02000884(0);
    message = (s32)MsgGarohInteriors01;
    Func_02000844(message);
    Message_Open(actor, 0);
    if (Inventory_PromptAndSetObjectMode(gPartyState.current_owner, 0) == 0) {
        Func_02000844(message + 2);
        Message_Open(actor, 0);
        if (Inventory_PromptAndSetObjectMode(gPartyState.current_owner, 0) == 0) {
            Func_0200085c(actor, 0);
        } else {
            ((struct EventWork1c4 *)Ram_HeapSlots->event_work)->count++;
            Func_0200085c(actor, 0);
        }
    } else {
        Func_0200085c(actor, 0);
    }
    Func_020007fc();
}

void Func_0200039c(void)
{
    Func_020007f4();
    Func_02000884(0);
    if (GameFlag_Test(0x91b))
        Func_02000844((s32)MsgGarohInteriors60);
    else
        Func_02000844((s32)MsgGarohInteriors25);
    Func_0200085c(17, 0);
    Func_020007fc();
}

void Func_020003dc(void)
{
    Func_020007f4();
    Func_02000884(0);
    if (GameFlag_Test(0x91b))
        Func_02000844((s32)MsgGarohInteriors61);
    else
        Func_02000844((s32)MsgGarohInteriors26);
    Func_0200085c(18, 0);
    Func_020007fc();
}

void Func_0200041c(void)
{
    Func_020007f4();
    Func_02000884(0);
    if (GameFlag_Test(0x91b))
        Func_02000844((s32)MsgGarohInteriors64);
    else
        Func_02000844((s32)MsgGarohInteriors29);
    Func_0200085c(17, 0);
    Func_020007fc();
}

void Func_0200045c(void)
{
    Func_020007f4();
    Func_02000884(0);
    if (GameFlag_Test(0x91b))
        Func_02000844((s32)MsgGarohInteriors65);
    else
        Func_02000844((s32)MsgGarohInteriors30);
    Func_0200085c(18, 0);
    Func_020007fc();
}

void Func_0200049c(void)
{
    s32 message = (s32)MsgGarohInteriors47;

    Func_02000844(message);
    UiText_OpenMessageAtObject(11, 0);
    if (Inventory_PromptAndSetObjectMode(Func_0200088c(), 0) == 0) {
        Battle_WaitMode0(10);
        Func_02000844(message + 1);
    } else {
        Battle_WaitMode0(20);
        Func_02000844(message + 2);
    }
    Func_0200085c(11, 0);
    GameFlag_SetBit(0x91a);
}

// Returns the room's table, chosen by flag 0x91B, the scene and, in the
// first room, the entrance.
const u8 *Func_020004f0(void)
{
    s32 moved = GameFlag_Test(0x91b);
    struct PartyState *party = &gPartyState;
    s32 room1 = (s32)&SceneId_Ov660Room1;
    s32 scene;

    if (moved) {
        scene = party->scene;
        if (scene == room1)
            return Data_02000e44;
        if (scene == (s32)&SceneId_Ov660Room2)
            return Data_02000fe8;
    } else {
        scene = party->scene;
        if (scene == room1) {
            if (party->entrance == 99)
                return Data_02000e44;
            return Data_02000b80;
        }
        if (scene == (s32)&SceneId_Ov660Room2)
            return Data_02000d18;
    }
    if (scene == (s32)&SceneId_Ov660Room3)
        return Data_02000de4;
    return Data_02000b5c;
}
