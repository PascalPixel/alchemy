#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "SCENE_IDS.H"
extern u8 MsgTorebiBuddyWannaTry[];
extern u8 MsgTorebiYaDontEnough[];

s32 Party_GetAverageLevel(void);
void ObjectTable_Snapshot(void);
void UiWork_PushValueSlot(s32 value, s32 digits);
void Event_SetPair1c0AndSetValue170(s32 value, s32 mode);

/* Tolbi game attendant: quote ten coins per level; without enough coins say so, otherwise record the coins and, if the player accepts, walk in and start the game area. */
void TorebiIzumi_PayToPlay(void)
{
    u32 cost;
    u32 coins;

    cost = Party_GetAverageLevel() * 10;
    Engine_EventBegin();
    coins = gGameState.coins;
    if (coins < cost) {
        Engine_EventSetMessage((s32)MsgTorebiYaDontEnough);
        Engine_EventOpenMessage(9, 0);
        return;
    }
    *(u32 *)gSceneState = coins;
    ObjectTable_Snapshot();
    Engine_EventSetMessage((s32)MsgTorebiBuddyWannaTry);
    UiWork_PushValueSlot(cost, 5);
    Engine_EventOpenMessage(9, 0);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
        Engine_EventShowMessage(9, 0);
        Engine_ActorWalkToAndWait(0, 120, 128);
        Engine_ActorWalkToAndWait(0, 120, 152);
        Engine_ActorFaceDirection(0, 0x8000, 0);
        Engine_EventWait(20);
        Event_SetPair1c0AndSetValue170(0x1fd, 0);
        Party_SetFields1ceAnd1d0((s32)&SceneId_TorebiIzumi1, 13);
    } else {
        gEventWork->message++;
        Engine_EventShowMessage(9, 0);
    }
    Engine_EventEnd();
}
