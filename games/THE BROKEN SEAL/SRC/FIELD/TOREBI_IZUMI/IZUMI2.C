#include "GLOBAL_CELLS.H"
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "SCENE_IDS.H"

struct Workspace {
    u8 unknown_000[356];
    struct FieldActor actor;
};

extern const u16 TorebiIzumi_AlphaSteps[];

extern u8 MsgTorebiBadComeBack[];
extern u8 MsgTorebiBadLikeUse[];
extern u8 MsgTorebiComeAgain[];
extern u8 MsgTorebiGetRidItems[];
extern u8 MsgTorebiStepRightEnjoy[];
extern u8 MsgTorebiStepRightTry[];
s32 PartyInventory_CountItem(s32 item);
void ObjectTable_Snapshot(void);
void UiWork_PushValueSlot(s32 value, s32 digits);
s32 PartyInventory_CountFreeSlots(void);
void Event_SetPair1c0AndSetValue170(s32 a, s32 b);
void Party_SetFields1ceAnd1d0(s32 scene, s32 entrance);

extern u8 MsgTorebiBuddyWannaTry[];
extern u8 MsgTorebiYaDontEnough[];
s32 Party_GetAverageLevel(void);
void Event_SetPair1c0AndSetValue170(s32 value, s32 mode);

/* Lifts the workspace actor sixteen pixels with a cue, then steps the blend alpha through its eight-entry table. */
void TorebiIzumi_RiseAndFadeIn(void)
{
    u8 *p = gMapWork[0];
    s32 i;

    Engine_AudioPlayCue(216);
    p += 356;
    for (i = 0; i < 16; i++) {
        ((struct FieldActor *)p)->y.fixed -= 0x10000;
        Engine_TaskWait(4);
    }
    p = (u8 *)TorebiIzumi_AlphaSteps;
    for (i = 0; i < 8; i++) {
        {
            s32 mode = 0x3f42;

            *(volatile u16 *)0x04000050 = mode;
        }
        *(volatile u16 *)0x04000052 = *(u16 *)p;
        p += 2;
        /* FAKEMATCH: the empty do/while keeps the step pointer's advance ahead of the wait's argument. */
        do { } while (0);
        Engine_TaskWait(8);
    }
}

/* The lucky wheels barker: counts the game tickets (item 228), offers a
 * game, warns when the bag is nearly full (six or fewer free slots) and
 * starts the lucky wheels when the player agrees. */
void TorebiIzumi_OfferLuckyWheels(s32 spoken)
{
    s32 tickets = PartyInventory_CountItem(228);
    s32 room;

    ObjectTable_Snapshot();
    if (spoken == 0) {
        s32 message = (s32)MsgTorebiStepRightTry;

        Engine_EventSetMessage(message);
        Engine_EventShowMessage(8, 0);
        if (tickets == 0) {
            Engine_EventShowMessage(8, 0);
            return;
        }
        Engine_EventSetMessage(message + 2);
        UiWork_PushValueSlot(tickets, 5);
        Engine_EventOpenMessage(8, 0);
        if (Engine_EventChooseYesNo(0, 0) != 0) {
            Engine_EventShowMessage(8, 0);
            return;
        }
        room = PartyInventory_CountFreeSlots();
        if (room == 0) {
            Engine_EventSetMessage(message + 4);
            Engine_EventOpenMessage(8, 0);
        } else {
            if (room > 6) {
                goto play;
            }
            Engine_EventSetMessage(message + 5);
            Engine_EventOpenMessage(8, 0);
        }
        if (room > 6 || Engine_EventChooseYesNo(0, 0) == 0) {
            goto play;
        }
        Engine_EventSetMessage((s32)MsgTorebiGetRidItems);
        Engine_EventShowMessage(8, 0);
        return;
    }
    if (tickets == 0) {
        Engine_EventSetMessage((s32)MsgTorebiBadComeBack);
        Engine_EventShowMessage(8, 0);
        return;
    }
    Engine_EventSetMessage((s32)MsgTorebiBadLikeUse);
    Engine_EventOpenMessage(8, 0);
    if (Engine_EventChooseYesNo(0, 0) != 0) {
        Engine_EventSetMessage((s32)MsgTorebiComeAgain);
        Engine_EventShowMessage(8, 0);
        return;
    }
play:
    Engine_EventSetMessage((s32)MsgTorebiStepRightEnjoy);
    Engine_EventShowMessage(8, 0);
    Event_SetPair1c0AndSetValue170(508, 0);
    Party_SetFields1ceAnd1d0((s32)&SceneId_TorebiIzumi1, 12);
}

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
