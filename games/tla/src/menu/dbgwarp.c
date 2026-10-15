#include "types.h"
#include "game.h"
#include "party_state.h"
#include "flag_call.h"

extern char SceneId_Ov69dRoom1;

void Event_SetDestinationFar(s32 scene, s32 entrance);

extern char SceneId_Ov665Room1;

extern char SceneId_Ov668Room1;

extern char SceneId_Ov682Room1;
void        PartyInventory_AddFar(s32 item);

extern char SceneId_Shaman4;

extern char SceneId_Ov6a8Room2;

extern char SceneId_Ov6aeRoom1;

extern char SceneId_Ov6abRoom1;

void Func_08040e64(void)
{
    Inventory_AddItemFar(4, 243);
    Inventory_AddItemFar(4, 244);
    Event_SetDestinationFar((s32)&SceneId_Ov69dRoom1, 4);
}

void Func_08040e84(void)
{
    GameFlag_SetBit(0x8ab);
    GameFlag_ClearBit(0x8a7);
    GameFlag_SetBit(0x8aa);
    Inventory_AddItemFar(4, 457);
    {
        struct PartyState *party = &gPartyState;

        /* FAKEMATCH: a one-pass block loads the party state before its offset is built (one reordered row). */
        do
        {
        } while (0);
        party->packedEvents[1] = 141;
    }
    Event_SetDestinationFar((s32)&SceneId_Ov665Room1, 1);
}

void Func_08040ed0(void)
{
}

void Func_08040ed4(void)
{
    GameFlag_SetBit(0x8ab);
    GameFlag_SetBit(0x8a7);
    GameFlag_SetBit(0x8aa);
    {
        struct PartyState *party = &gPartyState;

        /* FAKEMATCH: a one-pass block loads the party state before its offset is built (one reordered row). */
        do
        {
        } while (0);
        party->packedEvents[1] = 141;
    }
    Event_SetDestinationFar((s32)&SceneId_Ov668Room1, 1);
}

void Func_08040f14(void)
{
    PartyInventory_AddFar(439);
    PartyInventory_AddFar(440);
    PartyInventory_AddFar(441);
    GameFlag_SetBit(0x97f);
    {
        struct PartyState *party = &gPartyState;

        /* FAKEMATCH: a one-pass block loads the party state before its offset is built (one reordered row). */
        do
        {
        } while (0);
        party->packedEvents[1] = 141;
    }
    Event_SetDestinationFar((s32)&SceneId_Ov682Room1, 9);
}

void Func_08040f58(void)
{
    GameFlag_SetBit(0x9fe);
    GameFlag_SetBit(0x94c);
    GameFlag_SetBit(0x9e1);
    GameFlag_SetBit(0x94f);
    Event_SetDestinationFar((s32)&SceneId_Shaman4, 30);
}

void Func_08040f90(void)
{
    GameFlag_SetBit(0x824);
    GameFlag_SetBit(0x17c);
    GameFlag_SetBit(0x9c5);
    GameFlag_SetBit(0xa3d);
    GameFlag_SetBit(0x9c0);
    GameFlag_SetBit(0x9c1);
    GameFlag_SetBit(0x9c2);
    Event_SetDestinationFar((s32)&SceneId_Ov6a8Room2, 1);
}

void Func_08040fe4(void)
{
    Event_SetDestinationFar((s32)&SceneId_Ov6aeRoom1, 1);
}

void Func_08040ff4(void)
{
    Event_SetDestinationFar((s32)&SceneId_Ov6abRoom1, 20);
}
