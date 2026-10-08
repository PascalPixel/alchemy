#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "field_event.h"
#include "field_scene.h"
#include "resource_3a9.h"
#include "text/msg_ids.h"

#define FieldScene_PrepareActors ((void (*)(const struct ScenePlacement*))FieldScene_PrepareActors)


typedef struct Placement {
    u32 destination;
    u16 x;
    u16 y;
} Placement;



extern Placement KareiHeya_ArrivalPlacements[];


enum ArrivalMessage {
    MSG_CAN_LIVE_IN_PEACE_IN = 0x1a8f,
    MSG_GOING_TOLBI_ALSO = 0x1ad7,
    MSG_PLEASE_FINISH_EATING_IF_TAKING = 0x1add,
    MSG_DO_KNOW_ABOUT_CONTINENT_SOUTH = 0x1ae3,
    MSG_OUR_INN_FEELS_EMPTY_NOW = 0x1afb
};

const struct SceneEntrance *Scene_GetEntrances(void)
{
    s16 scene = gGameState.scene;

    if (scene == (s32)&SceneId_KareiHeya1) {
        return gKareiHeyaEntrances1;
    }
    if (scene == (s32)&SceneId_KareiHeya2) {
        return gKareiHeyaEntrances2;
    }
    return gKareiHeyaEntrancesOther;
}

s32 SceneData_ReturnZero(void)
{
    return 0;
}

u8 *KareiHeya_GetExits(void)
{
    return KareiHeya_Exits;
}

const struct ScenePlacement *Scene_GetPlacements(void)
{
    s32 scene = gGameState.scene;

    if (scene == (s32)&SceneId_KareiHeya1) {
        s32 entrance = gGameState.entrance;
        const struct ScenePlacement *table;

        switch (entrance) {
        case 9:
        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
        case 15:
        case 17:
            table = gKareiHeyaPlacements1Entrance9;
            break;
        default:
            table = gKareiHeyaPlacements1;
            break;
        }
        FieldScene_PrepareActors(table);
        return table;
    }
    if (scene == (s32)&SceneId_KareiHeya2) {
        return gKareiHeyaPlacements2;
    }
    return gKareiHeyaPlacementsOther;
}

const struct SceneEvent *Scene_GetEvents(void)
{
    s32 scene = gGameState.scene;

    if (scene == (s32)&SceneId_KareiHeya1) {
        s32 entrance = gGameState.entrance;

        switch (entrance) {
        case 9:
        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
        case 15:
        case 17:
            return gKareiHeyaEvents1Entrance9;
        default:
            return gKareiHeyaEvents1;
        }
    }
    if (scene == (s32)&SceneId_KareiHeya2) {
        return gKareiHeyaEvents2;
    }
    return gKareiHeyaEventsOther;
}

void SceneDialogue_RunActor12DialogueAndSetFlag910(void)
{
    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgKareiPleaseFinishEatingIfTaking);
    Engine_EventShowMessage(0xC, 0);
    Engine_GameFlagSet(0x910);
    Engine_EventEnd();
}

void SceneDialogue_RunActor16Dialogue(void)
{
    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgKareiDoKnowAboutContinentSouth);
    Engine_EventAskYesNo(16, 0);
    Engine_EventEnd();
}

void SceneDialogue_RunActor8FlaggedDialogue(void)
{
    struct FieldActor *p = Object_GetById(0);

    if ((u16)(p->facing - 0x6001) <= 0x3FFE) {
        Engine_InnOpen(7, 8);
    } else {
        Engine_EventBegin();

        if (Engine_GameFlagIsSet(0x911) != 0) {
            Engine_EventSetMessage((s32)MsgKareiOurInnFeelsEmptyNow);
            Engine_EventShowMessage(8, 0);
        } else {
            Engine_EventSetMessage((s32)MsgKareiGoingTolbiAlso);
            Engine_EventAskYesNo(8, 0);
            Engine_GameFlagSet(0x910);
        }

        Engine_EventEnd();
    }
}

void SceneDialogue_RunActor8FacingDialogue(void)
{
    void Engine_EventSetMessage(int);

    struct FieldActor *p = Object_GetById(0);

    if ((u16)(p->facing + 0x5FFF) <= 0x3FFE) {
        Engine_SanctumOpen(8);
    } else {
        Engine_EventBegin();
        Engine_EventSetMessage((s32)MsgKareiCanLiveInPeaceIn);
        Engine_EventShowMessage(8, 0);
        Engine_EventEnd();
    }
}

void FieldScene_RunArrivalPlacement(void)
{

    struct EventWork *work = gEventWork;
    u32 slot;
    s32 idx;
    struct FieldActor *p;

    Engine_EventBegin();

    for (slot = 8; slot <= 65; slot++) {
        struct FieldActor *rec = Object_GetById(slot);

        if (rec != 0) {
            rec->motion_flags = 0;
        }
    }

    switch (work->touched_trigger) {
    case 12: idx = 0; break;
    case 13: idx = 1; break;
    case 16: idx = 2; break;
    case 19: idx = 3; break;
    default: return;
    }

    Engine_AudioPlayCue(158);

    {
        u32 x = KareiHeya_ArrivalPlacements[idx].x;
        u32 y = KareiHeya_ArrivalPlacements[idx].y;

        Engine_MapAnimateCells(KareiHeya_ArrivalPlacements[idx].destination, x, y);
    }

    Engine_ActorSetSpeed(ACTOR_PARTY_LEADER, 0x00008000, 0x00004000);

    p = Object_GetById(0);
    p->motion_flags = 0;

    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 2);
    Engine_ActorCenterAndWalk(ACTOR_PARTY_LEADER, 3, -8);
    Engine_EventWait(10);

    Engine_EventRequestExit(work->touched_trigger);
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
    Engine_EventEnd();
}

s32 Scene_Initialize(void)
{
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
    if (gGameState.scene == (s32)&SceneId_KareiHeya1) {
        SceneState_ClearSlotsBySubState();
    }
    return 0;
}

void SceneState_ClearSlotsBySubState(void)
{
    s16 sub = gGameState.entrance;

    switch (sub) {
    case 3:
    {
                s32 fifth = 4;
        s32 sixth = 2;
        Engine_MapCopyCellsTo(30, 14, 30, 16, fifth, sixth);
        return;
    }
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
    case 17:
        break;
    default:
        goto other;
    }

    if (Engine_GameFlagIsSet(0x911) != 0) {

        Engine_ActorDestroy(10);
        Engine_ActorDestroy(11);
        Engine_ActorDestroy(12);
        Engine_ActorDestroy(13);
        Engine_ActorDestroy(14);
        Engine_ActorDestroy(17);
        Engine_ActorDestroy(18);
        Engine_ActorDestroy(19);
        Engine_ActorDestroy(15);
    } else {
        Engine_ActorSetChildValue(13, 2);
    }
    return;

other:
    if (Engine_GameFlagIsSet(0x911) != 0) {
        Engine_ActorDestroy(16);
        Engine_ActorDestroy(17);
    }
}
