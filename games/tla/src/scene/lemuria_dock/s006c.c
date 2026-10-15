#include "room.h"

void Func_0200006c(void)
{
    struct PartyState *party = &gPartyState;

    if (party->entrance == 2) {
        Func_0200046c();
        Func_02000524(0);
        if (Object_GetById(8)->x == 0) {
            ObjectMotion_CommitPositionAndActivate_(4, 0, -16);
            Func_0200052c_(8, 0, -16, 0x3000);
            ObjectMotion_CommitCurrentPositionAndActivate(8);
            ObjectMotion_SetSpeedParameters_(8, 0x13333, 0x9999);
            ObjectMotion_SetPositionAndReset_(8, 472, 392);
            Func_020004f4_(8, 0x5000);
        } else {
            struct ObjectRuntime *leader;

            ObjectMotion_CommitPositionAndActivate_(4, 0, 16);
            ObjectMotion_SetSpeedParameters_(8, 0x20000, 0x10000);
            ObjectMotion_SetPositionAndReset_(8, 440, 440);
            ObjectMotion_SetSpeedParameters_(8, 0x13333, 0x9999);
            Object_SetModeById(8, 2);
            leader = Object_GetById(party->current_owner);
            if (leader != NULL)
                ObjectMotion_ResetAndSetPosition(8, S16_AT(leader, 10), S16_AT(leader, 18));
            ObjectMotion_CommitCurrentPositionAndActivate(8);
            Func_020004bc(8, 0, 0);
        }
        Func_02000474();
    }
}

void Func_02000158(void)
{
    struct PartyState *party;
    s32 id;
    s32 limit;
    s32 z;

    if (SCENE_WORK->unknown_166 != 0) {
        ObjectMotion_SetAngleToward(8, gPartyState.current_owner, 0);
        Func_02000504_(8, 0x100, 30);
    }
    Func_020004dc((s32)MsgAreWeSetting);
    UiText_OpenMessageAtObject(8, 0);
    if (Inventory_PromptAndSetObjectMode(Func_0200053c(), 0) == 0) {
        Battle_WaitMode0(15);
        Func_020004ec(8, 0);
        Battle_WaitMode0(5);
        ObjectMotion_Launch(8, 4, 0);
        Func_020004f4(8, 0);
        Battle_WaitMode0(5);
        party = &gPartyState;
        z = Object_GetById(party->current_owner)->z;
        /* FAKEMATCH: ⚓️ loads the leader's z before it builds the limit */
        asm volatile("" : "+l"(z));
        limit = 0x1880000;
        id = party->current_owner;
        if (z < limit)
            ObjectMotion_SetPositionAndReset_(id, 472, 376);
        else
            ObjectMotion_SetPositionAndReset_(id, 472, 408);
        id = 4;
        /* FAKEMATCH: ⚓️ builds the id before the zero */
        asm volatile("" : "+l"(id));
        Func_020004f4(id, 0);
        Battle_WaitMode0(10);
        {
            struct PartyState *state = &gPartyState;

            /* FAKEMATCH: a one-pass block loads the party state before its offset is built (one reordered row). */
            do {
            } while (0);
            state->owner_override = 1;
        }
        Func_0200050c(1);
    } else {
        Battle_WaitMode0(20);
        SCENE_WORK->unknown_1c4++;
        Func_020004ec(8, 0);
    }
}

void Func_02000254(const struct WarpTarget *warp, s32 door, s32 mode)
{
    s32 id;

    Func_0200046c();
    Func_02000524(0);
    Func_02000544(158);
    Func_02000454(warp->scene, warp->x, warp->z);
    Object_GetById(gPartyState.current_owner)->motion_flags = 2;
    ObjectMotion_SetSpeedParameters(gPartyState.current_owner, 0x8000, 0x4000);
    Object_SetModeById(gPartyState.current_owner, 2);
    id = gPartyState.current_owner;
    if (mode == 0)
        ObjectMotion_CenterOnColumn_(id, 2, -8);
    else
        ObjectMotion_OffsetPositionAndResetMotion_(id, 0, -8);
    Battle_WaitMode0(10);
    Func_0200050c(door);
    Event_FinishScreenTransition();
    Event_WaitTransitionFrames();
    Func_02000474();
}

void Func_020002dc(s32 door)
{
    Func_02000254(&Data_02000610, door, 0);
}
