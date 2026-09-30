#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "RESOURCE_3A9.H"

extern const struct SceneEntrance gKareiHeyaEntrances1[];
extern const struct SceneEntrance gKareiHeyaEntrances2[];
extern const struct SceneEntrance gKareiHeyaEntrancesOther[];

/* Table selection, dialogue and arrival scripts for resource_3a9. */
typedef struct Placement {
    u32 destination;
    u16 x;
    u16 y;
} Placement;

extern u8 KareiHeya_Exits[];

extern const struct ScenePlacement gKareiHeyaPlacements1Entrance9[];
extern const struct ScenePlacement gKareiHeyaPlacements1[];
extern const struct ScenePlacement gKareiHeyaPlacements2[];
extern const struct ScenePlacement gKareiHeyaPlacementsOther[];
extern const struct SceneEvent gKareiHeyaEvents1Entrance9[];
extern const struct SceneEvent gKareiHeyaEvents1[];
extern const struct SceneEvent gKareiHeyaEvents2[];
extern const struct SceneEvent gKareiHeyaEventsOther[];
void FieldScene_PrepareActors(const struct ScenePlacement *placements);

extern u8 MsgKareiCanLiveInPeaceIn[];
extern u8 MsgKareiDoKnowAboutContinentSouth[];
extern u8 MsgKareiGoingTolbiAlso[];
extern u8 MsgKareiOurInnFeelsEmptyNow[];
extern u8 MsgKareiPleaseFinishEatingIfTaking[];
extern Placement KareiHeya_ArrivalPlacements[];

void SceneState_ClearSlotsBySubState(void);

enum ArrivalMessage {
    MSG_CAN_LIVE_IN_PEACE_IN = 0x1a8f,
    MSG_GOING_TOLBI_ALSO = 0x1ad7,
    MSG_PLEASE_FINISH_EATING_IF_TAKING = 0x1add,
    MSG_DO_KNOW_ABOUT_CONTINENT_SOUTH = 0x1ae3,
    MSG_OUR_INN_FEELS_EMPTY_NOW = 0x1afb
};

/* Where the party appears in the scene it enters. */
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

/* The exit table, the third entry the main image calls. */
u8 *KareiHeya_GetExits(void)
{
    return KareiHeya_Exits;
}

/* The actors placed in the Kalay houses. In the first scene the entrances
   9 to 15 and 17 have their own table; the first scene's table is prepared
   before it is returned. */
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

/* What the Kalay houses answer, chosen as their placements are. */
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

/* In-image placement table, four entries. */
void SceneDialogue_RunActor12DialogueAndSetFlag910(void)
{
    Event_Begin();
    Event_SetMessage((s32)MsgKareiPleaseFinishEatingIfTaking);
    Event_ShowMessage(0xC, 0);
    GameFlag_Set(0x910);
    Event_End();
}

void SceneDialogue_RunActor16Dialogue(void)
{
    Event_Begin();
    Event_SetMessage((s32)MsgKareiDoKnowAboutContinentSouth);
    Event_AskYesNo(16, 0);
    Event_End();
}

void SceneDialogue_RunActor8FlaggedDialogue(void)
{
    u8 *p = (u8 *)Object_GetById(0);

    /* Band guard: facing in 0x6001..0x9fff. The test is spelled as the short
     * arm's condition, which is what reproduces the branch. */
    if ((u16)(*(u16 *)(p + 6) - 0x6001) <= 0x3FFE) {
        Inn_Open(7, 8);
    } else {
        Event_Begin();

        if (GameFlag_IsSet(0x911) != 0) {
            Event_SetMessage((s32)MsgKareiOurInnFeelsEmptyNow);
            Event_ShowMessage(8, 0);
        } else {
            Event_SetMessage((s32)MsgKareiGoingTolbiAlso);
            Event_AskYesNo(8, 0);
            GameFlag_Set(0x910);           /* 145 << 4 */
        }

        Event_End();
    }
}

void SceneDialogue_RunActor8FacingDialogue(void)
{
    void Event_SetMessage(int);

    u8 *p = (u8 *)Object_GetById(0);

    /* Band guard: facing in 0xa001..0xdfff. The test is spelled as the short
     * arm's condition, which is what reproduces the branch. */
    if ((u16)(*(u16 *)(p + 6) + 0x5FFF) <= 0x3FFE) {
        Sanctum_Open(8);
    } else {
        Event_Begin();
        Event_SetMessage((s32)MsgKareiCanLiveInPeaceIn);
        Event_ShowMessage(8, 0);
        Event_End();
    }
}

/*
 * Scene arrival: clears the residue byte at +85 of every scene slot from 8 to
 * 65, then looks the sub-state up in the placement table and places the
 * player from that entry. Sub-states other than 12, 13, 16 and 19 return
 * without touching anything. The loop skips a null record but the later clear
 * of the player's own +85 does not test for null; that asymmetry is real.
 * 158 is read as a cue id from its argument position and is not verified.
 */
void FieldScene_RunArrivalPlacement(void)
{

    u8 *work = *(u8 **)&gEventWork;
    u32 slot;
    s32 idx;
    u8 *p;

    Event_Begin();

    for (slot = 8; slot <= 65; slot++) {
        u8 *rec = (u8 *)Object_GetById(slot);

        if (rec != 0) {
            rec[85] = 0;
        }
    }

    /* The sub-state slot is read twice, here and for Engine_EventRequestExit below, and
     * both reads are kept. */
    switch (*(s16 *)(work + 364)) {         /* 182 << 1 */
    case 12: idx = 0; break;
    case 13: idx = 1; break;
    case 16: idx = 2; break;
    case 19: idx = 3; break;
    default: return;
    }

    Audio_PlayCue(158);

    {
        u32 x = KareiHeya_ArrivalPlacements[idx].x;
        u32 y = KareiHeya_ArrivalPlacements[idx].y;

        Map_AnimateCells(KareiHeya_ArrivalPlacements[idx].destination, x, y);
    }

    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x00008000, 0x00004000);

    p = (u8 *)Object_GetById(0);
    p[85] = 0;

    Actor_SetAnimation(ACTOR_PARTY_LEADER, 2);
    Actor_CenterAndWalk(ACTOR_PARTY_LEADER, 3, -8);
    Event_Wait(10);

    Event_RequestExit(*(s16 *)(work + 364));
    Event_CloseScreen();
    Event_WaitForScreen();
    Event_End();
}

/* The Kalay houses' scene start: open with the window transition; the first
   scene clears the slots its entrance calls for. */
s32 Scene_Initialize(void)
{
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
    if (gGameState.scene == (s32)&SceneId_KareiHeya1) {
        SceneState_ClearSlotsBySubState();
    }
    return 0;
}

/*
 * Clears the set of scene slots this sub-state leaves behind. Sub-state 16
 * takes the last arm even though it lies inside 9..17, so the test is not
 * written as a range. 0x911 is read as an event-flag id from its argument
 * position, and the six-argument call's argument meanings are not
 * established.
 */
void SceneState_ClearSlotsBySubState(void)
{
    s16 sub = gGameState.entrance;

    switch (sub) {
    case 3:
    {
        /* The last two arguments travel on the stack. */
        s32 fifth = 4;
        s32 sixth = 2;
        Map_CopyCellsTo(30, 14, 30, 16, fifth, sixth);
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

    /* sub is 9..15 or 17. */
    if (GameFlag_IsSet(0x911) != 0) {
        /* Nine distinct call sites, not a loop; the trailing 15 is out of
         * order and is kept that way. */
        Actor_Destroy(10);
        Actor_Destroy(11);
        Actor_Destroy(12);
        Actor_Destroy(13);
        Actor_Destroy(14);
        Actor_Destroy(17);
        Actor_Destroy(18);
        Actor_Destroy(19);
        Actor_Destroy(15);
    } else {
        Actor_SetChildValue(13, 2);
    }
    return;

other:
    if (GameFlag_IsSet(0x911) != 0) {
        Actor_Destroy(16);
        Actor_Destroy(17);
    }
}
