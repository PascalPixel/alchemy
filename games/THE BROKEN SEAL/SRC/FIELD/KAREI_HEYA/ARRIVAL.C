#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

#include "RESOURCE_3A9.H"
extern u8 MsgKareiCanLiveInPeaceIn[];
extern u8 MsgKareiDoKnowAboutContinentSouth[];
extern u8 MsgKareiGoingTolbiAlso[];
extern u8 MsgKareiOurInnFeelsEmptyNow[];
extern u8 MsgKareiPleaseFinishEatingIfTaking[];


/* Table selection, dialogue and arrival scripts for resource_3a9. */
typedef struct Placement {
    u32 destination;
    u16 x;
    u16 y;
} Placement;

extern Placement KareiHeya_ArrivalPlacements[];   /* In-image placement table, four entries. */

u8 *Object_GetById(s32);

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

/* Actor 8's dialogue, branched on flag 0x911. Engine_EventShowMessage and
 * Engine_EventSetMessage are two imports sharing one call word: the two-argument
 * gesture in the first arm, the one-argument message in the second. */
void SceneDialogue_RunActor8FlaggedDialogue(void)
{
    u8 *p = Object_GetById(0);

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

    u8 *p = Object_GetById(0);

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
    u8 *Object_GetById();

    u8 *work = *(u8 **)&gEventWork;
    u32 slot;
    s32 idx;
    u8 *p;

    Event_Begin();

    for (slot = 8; slot <= 65; slot++) {
        u8 *rec = Object_GetById(slot);

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

    p = Object_GetById(0);
    p[85] = 0;

    Actor_SetAnimation(ACTOR_PARTY_LEADER, 2);
    Actor_CenterAndWalk(ACTOR_PARTY_LEADER, 3, -8);
    Event_Wait(10);

    Event_RequestExit(*(s16 *)(work + 364));
    Event_CloseScreen();
    Event_WaitForScreen();
    Event_End();
}
