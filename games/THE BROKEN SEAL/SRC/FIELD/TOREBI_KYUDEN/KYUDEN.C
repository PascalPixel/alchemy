#include "TYPES.H"
#include "FIELD_SERVICE.H"
#include "FIELD_SCENE.H"
#include "KYUDEN.H"

extern const struct SceneEntrance gTorebiKyudenEntrances2[];
extern const struct SceneEntrance gTorebiKyudenEntrancesOther[];

extern const struct ScenePlacement gTorebiKyudenPlacements2[];
extern const struct ScenePlacement gTorebiKyudenPlacementsAfterColosso[];
extern const struct ScenePlacement gTorebiKyudenPlacementsColosso[];
extern const struct ScenePlacement gTorebiKyudenPlacementsOther[];

extern u8 MsgTorebiTiredFeelFreeRest[];
extern u8 MsgTorebiCameRestBefore[];
extern u8 MsgTorebiPlanningEnterColosso[];
extern u8 MsgTorebiRunBabisSoldiers[];
extern u8 MsgTorebiUseFourBeds[];
extern u8 MsgTorebiWeHaveJustEnoughExtra[];

extern u8 MsgTorebiCantFightBecauseLittleIndigestion[];
extern u8 MsgTorebiEvenIfEscapedBabiPalace[];

extern u8 MsgTorebiGetUp[];
extern u8 MsgTorebiEasternShoresKaragol[];
extern u8 MsgTorebiFoundCloakBall[];
extern u8 MsgTorebiMeetBabi[];

extern const u16 TorebiKyuden_CellSteps[];
extern u8 MsgTorebiCallsName[];
extern u8 MsgTorebiIodemIodem[];
extern u8 MsgTorebiLikeSleep[];
extern u8 MsgTorebiWaitingCompanions[];

extern u8 MsgTorebiArent[];

extern u8 MsgTorebiBabiWaitingForAtColosseum[];
extern u8 MsgTorebiRobinIdReallyLikeThank[];
extern u8 MsgTorebiWarriorsWhoStayed[];

extern u8 MsgFieldPeeredWell[];
extern u8 MsgTorebiItsFilledWithFreshClean[];

extern const struct SceneEvent gTorebiKyudenEvents2AfterColosso[];
extern const struct SceneEvent gTorebiKyudenEvents2Colosso[];
extern const struct SceneEvent gTorebiKyudenEvents2[];
extern const struct SceneEvent gTorebiKyudenEventsAfterColosso[];
extern const struct SceneEvent gTorebiKyudenEventsColosso[];
extern const struct SceneEvent gTorebiKyudenEventsOther[];

void FieldScene_RunMainCutsceneSequence(void);
void FieldScene_RunBranchingActorSequence(void);
void FieldScene_RunScene3b8SequenceB(void);

/* The stats an owner keeps: current HP and PP follow their maximums. */
struct OwnerState {
    u8 unknown_00[0x34];
    u16 max_hp;
    u16 max_pp;
    u16 hp;
    u16 pp;
};


/* Restore the whole party: every owner's HP and PP to their maximums, and
 * the three companions back into the active party. */
static __inline__ void Party_RestoreAll(void)
{
    s16 owners[8];
    struct OwnerState *state;
    s32 cnt;
    s32 i;

    cnt = Party_ListActiveOwners(owners);
    for (i = 0; i < cnt; i++) {
        state = Owner_GetState(owners[i]);
        state->hp = state->max_hp;
        state->pp = state->max_pp;
        Owner_RecalculateRatios(owners[i]);
    }
    Party_AddActiveOwner(1);
    Party_AddActiveOwner(2);
    Party_AddActiveOwner(3);
    InventorySnapshot_Restore();
}

/* Where the party appears in the palace; the second scene has its own. */
const struct SceneEntrance *Scene_GetEntrances(void)
{
    if (gGameState.scene == (s32)&SceneId_TorebiKyuden2) {
        return gTorebiKyudenEntrances2;
    }
    return gTorebiKyudenEntrancesOther;
}

s32 SceneData_GetTableca7c(void)
{
    return (s32)TorebiKyuden_EmptyTable;
}

s32 SceneData_GetTableca8c(void)
{
    return (s32)TorebiKyuden_MessageTable;
}

/* The actors placed in the palace: the second scene has its own; the
   others change once Colosso is under way (flag 0x962) and once it is over
   (flag 0x950). */
const struct ScenePlacement *Scene_GetPlacements(void)
{
    if (gGameState.scene == (s32)&SceneId_TorebiKyuden2) {
        return gTorebiKyudenPlacements2;
    }
    if (GameFlag_IsSet(0x950) != 0) {
        return gTorebiKyudenPlacementsAfterColosso;
    }
    if (GameFlag_IsSet(0x962) != 0) {
        return gTorebiKyudenPlacementsColosso;
    }
    return gTorebiKyudenPlacementsOther;
}

/* The palace guest rooms: the steward asks whether the party met Babi's
 * soldiers, the room keeper offers the four beds or a free rest, and the
 * last host asks about Colosso. Each question loads its first message once
 * and adds to it for the answers. */
void FieldScene_RunBranchedSteps1FF1(s32 a)
{
    s32 k = (s32)MsgTorebiRunBabisSoldiers;

    Event_SetMessage(k);
    Event_OpenMessage(a, 0);
    if (Event_ChooseYesNo(0, 0) == 0)
        Event_SetMessage(k + 1);
    else
        Event_SetMessage(k + 2);
    Event_ShowMessage(a, 0);
}

void RunOpeningAuxiliarySequence(s32 a)
{

    u8 *ret;
    s16 v;
    s32 c;
    s32 t;

    ret = Actor_Get(ACTOR_PARTY_LEADER);
    v = (*(u16 *)(ret + 6) + 0x2000) & 0xc000;
    Event_Begin();
    Battle_ResetEffectCounter();
    if (GameFlag_IsSet(512) == 0) {
        GameFlag_Set(512);
        GameFlag_Clear(0x969);
        Event_SetMessage((s32)MsgTorebiWeHaveJustEnoughExtra);
        Event_ShowMessage(a, 0);
        Event_Wait(10);
        t = v << 16;
        c = 0x4000;
        if (t == (0x4000 << 16)) {
            Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 40, 104);
            Actor_FaceDirection(ACTOR_PARTY_LEADER, 0, 0);
        }
        Actor_SetSpeed(a, 0x10000, 0x8000);
        Actor_WalkByAndWait(a, 0, -48);
        Actor_WalkByAndWait(a, 64, 0);
        Actor_FaceDirection(a, c, 0);
    } else {
        GameFlag_Clear(512);
        GameFlag_Set(0x969);
        Actor_FaceDirection(a, 0x4000, 0);
        Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 120, 96);
        Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
        Event_Wait(20);
        c = (s32)MsgTorebiUseFourBeds;
        Event_SetMessage(c);
        Event_OpenMessage(a, 0);
        if (Event_ChooseYesNo(0, 0) == 0) {
            Event_SetMessage(c + 1);
            Event_ShowMessage(a, 0);
        } else {
            Event_SetMessage(c + 2);
            Event_ShowMessage(a, 0);
        }
        Event_Wait(10);
        Actor_SetAnimationAndWait(a, 3);
        Event_Wait(20);
        Actor_WalkByAndWait(a, -64, 0);
        Actor_WalkByAndWait(a, 0, 48);
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
        Actor_SetPosition(a, 56 << 16, 120 << 16);
#endif
    }
    Event_End();
}

void FieldScene_RunScene3b8_02000264(s32 a0)
{
    u32 i;
    s32 record;
    s32 base6_2241;

    Event_Begin();
    Battle_ResetEffectCounter();
    if (GameFlag_IsSet(0x966) == 0) {
        GameFlag_Set(0x966);
        GameFlag_Set(0x967);
        Actor_FaceDirection(a0, 0x4000, 0);
        Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 120, 96);
        Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
        Event_Wait(20);
        base6_2241 = (s32)MsgTorebiCameRestBefore;
        Event_SetMessage(base6_2241);
        Event_OpenMessage(a0, 0);
        if (Event_ChooseYesNo(0, 0) == 0) {
            Event_Wait(10);
            Event_SetMessage((base6_2241 + 1));
        } else {
            Event_SetMessage((base6_2241 + 2));
        }
        Event_ShowMessage(a0, 0);
        Event_Wait(10);
        Actor_SetAnimationAndWait(a0, 3);
        Event_Wait(20);
        Actor_SetSpeed(a0, 0x10000, 0x8000);
        Actor_WalkByAndWait(a0, -64, 0);
        Actor_WalkByAndWait(a0, 0, 48);
    } else {
        Event_SetMessage((s32)MsgTorebiTiredFeelFreeRest);
        Event_OpenMessage(a0, 0);
    }
    Event_End();
}

void FieldScene_RunBranchedSteps2006(s32 a)
{
    s32 k = (s32)MsgTorebiPlanningEnterColosso;

    Event_SetMessage(k);
    Event_OpenMessage(a, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        Event_Wait(10);
        Actor_ShowEmote(a, 0x102, 0x28);
        Event_SetMessage(k + 1);
    } else {
        Event_Wait(10);
        Actor_ShowEmote(a, 0x105, 0x28);
        Event_SetMessage(k + 2);
    }
    Event_ShowMessage(a, 0);
}

void RunMiddleAuxiliarySequence(s32 a)
{
    u8 *obj;
    u8 *q;

    obj = (u8 *)Actor_Get(a);
    Event_Begin();
    q = TorebiKyuden_MiddleActionScript;
    Actor_EnableActionCallback(a, q);
    Event_SetMessage((s32)MsgTorebiCantFightBecauseLittleIndigestion);
    Event_ShowMessage(a, 0);
    Actor_Stop(a);
    *(s32 *)(obj + 28) = 0x10000;
    *(s32 *)(obj + 24) = 0x10000;
    Event_Wait(30);
    Actor_RunRepeatedMotion(a, 2);
    Event_Wait(30);
    Actor_RunRepeatedMotion(a, 2);
    Event_Wait(60);
    Event_ShowMessage(a, 0);
    Event_Wait(20);
    Actor_ShowEmote(a, 258, 60);
    Actor_RunRepeatedMotion(a, 2);
    Event_Wait(30);
    Actor_RunRepeatedMotion(a, 2);
    Event_Wait(30);
    Actor_RunRepeatedMotion(a, 2);
    Event_Wait(30);
    Actor_EnableActionCallback(a, q);
    Event_ShowMessage(a, 0);
    Actor_FaceDirection(a, 0xe000, 0);
    Event_Wait(10);
    *(s32 *)(obj + 28) = 0x10000;
    *(s32 *)(obj + 24) = 0x10000;
    Actor_EnableActionCallback(a, q);
    Event_End();
}

void FieldScene_RunScene3b8_0200049c(s32 unused0, s32 a1)
{
    Event_Begin();
    Event_SetMessage((s32)MsgTorebiEvenIfEscapedBabiPalace);
    Event_ShowMessage(a1, 0);
    if (GameFlag_IsSet(0x968) == 0) {
        GameFlag_Set(0x968);
        Psynergy_Cancel();
        Event_Wait(50);
        Actor_ShowEmote(a1, 0x100, 70);
        Actor_FaceActor(a1, ACTOR_PARTY_LEADER, 40);
        Event_ShowMessage(a1, 0);
        Event_Wait(30);
        Actor_SetAnimationAndWait(a1, 4);
        Event_Wait(20);
        Event_ShowMessage(a1, 0);
        Actor_FaceDirection(a1, 0x8000, 0);
    }
    Event_End();
}

/* The palace talks: the questions about meeting Babi and the eastern
 * shores past Karagol, the Cloak Ball found in the guest room, and the
 * morning the party wakes in the palace (FieldScene_RunScene3b8SequenceB).
 * The questions load their first message once and add to it for the
 * answers. */
void SceneDialogue_ShowMessage22a8Branch(s32 a)
{
    s32 k = (s32)MsgTorebiMeetBabi;

    Event_SetMessage(k);
    Event_OpenMessage(a, 0);
    if (Event_ChooseYesNo(0, 0) == 0)
        Event_SetMessage(k + 1);
    else
        Event_SetMessage(k + 2);
    Event_ShowMessage(a, 0);
}

void SceneDialogue_RunChoiceSequence22ab(s32 no)
{
    s32 msg = (s32)MsgTorebiEasternShoresKaragol;

    Event_SetMessage(msg);
    Event_OpenMessage(no, 0);
    if (Event_ChooseYesNo(0, 0) == 0)
        Event_SetMessage(msg + 1);
    else
        Event_SetMessage(msg + 2);
    Event_ShowMessage(no, 0);
}

void SceneDialogue_RunChoiceSequence2352(void)
{
    s32 msg;

    Event_Begin();
    Battle_ResetEffectCounter();
    msg = (s32)MsgTorebiFoundCloakBall;
    Event_SetMessage(msg);
    Event_ShowMessage(-1, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(14, 2);
    Event_Wait(30);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 14, 30);
    Event_OpenMessage(14, 0);
    if (Event_ChooseYesNo(0, 0) != 0) {
        Event_SetMessage(msg + 2);
        Event_ShowMessage(14, 0);
    } else {
        Event_Wait(20);
        Event_SetMessage(msg + 3);
        Event_ShowMessage(14, 0);
        Event_Wait(10);
        Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
        Event_Wait(30);
        Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 0);
        Event_Wait(30);
        Actor_SetPosition(16, 0, 0);
        Item_ShowFound(ITEM_CLOAK_BALL, 3);
        Actor_SetAnimation(ACTOR_PARTY_LEADER, 1);
        Party_GiveItem(ITEM_CLOAK_BALL, 0);
        GameFlag_Set(0xf31);
    }
}

void FieldScene_RunScene3b8SequenceB(void)
{
    struct FieldActor *record;

    Event_Begin();
    Event_SetMessage((s32)MsgTorebiGetUp);
    Actor_Get(0)->active = 0;
    Actor_Get(10)->active = 0;
    Task_Wait(1);
    *(volatile u16 *)0x04000000 = 0x1140;
    Event_ShowMessage(-1, 0);
    *(volatile u16 *)0x04000000 = 0x140;
    Actor_Get(0)->active = 1;
    Actor_Get(10)->active = 1;
    Actor_SetAnimation(0, 31);
    record = Actor_Get(0);
    Actor_SetSpriteFlags(record, 0);
    Actor_SetPosition(1, 0x780000, 0x680000);
    Actor_SetPosition(3, 0x680000, 0x500000);
    Actor_SetPosition(2, 0x780000, 0x780000);
    Actor_FaceDirection(1, 0, 0);
    Actor_FaceDirection(3, 0, 0);
    Actor_FaceDirection(2, 0xe000, 0);
    gEventWork->transition_frames = 60;
    Event_SetStatus1c6();
    Event_WaitValue1c8Frames();
    Event_Wait(20);
    gEventWork->transition_frames = 24;
    Actor_SetSpeed(3, 0x10000, 0x8000);
    Actor_WalkByAndWait(ACTOR_MIA, 16, 0);
    Actor_FaceDirection(3, 0x2000, 0);
    Event_Wait(20);
    Actor_RunRepeatedMotion(3, 2);
    Event_Wait(30);
    Event_ShowMessage(3, 0);
    Event_Wait(10);
    record = Actor_Get(0);
    record->z.fixed += -0x30000;
    record = Actor_Get(0);
    record->target_z += -0x30000;
    Actor_SetAnimation(0, 32);
    Event_Wait(40);
    Actor_SetAnimationAndWait(0, 34);
    Event_Wait(30);
    Actor_SetAnimation(0, 33);
    Event_Wait(50);
    Actor_RunRepeatedMotion(1, 2);
    Event_Wait(30);
    Event_ShowMessage(ACTOR_GERALD, 0);
    Event_Wait(10);
    Actor_ShowEmote(0, 0x105, 60);
    Event_Wait(20);
    Actor_ShowEmote(1, 0x102, 60);
    Event_ShowMessage(1, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(1, 4);
    Event_Wait(20);
    Event_ShowMessage(1, 0);
    Event_Wait(10);
    Actor_ShowEmote(0, 0x102, 80);
    Actor_ShowEmote(2, 0x106, 60);
    Actor_FaceDirection(2, 0xc000, 0);
    Event_Wait(30);
    Actor_SetAnimationAndWait(2, 4);
    Event_Wait(20);
    Event_ShowMessage(2, 0);
    Event_Wait(10);
    Actor_FaceDirection(1, 0x4000, 0);
    Event_Wait(30);
    Actor_RunRepeatedMotion(1, 2);
    Event_Wait(45);
    Actor_FaceDirection(1, 0, 0);
    Actor_FaceDirection(2, 0xe000, 0);
    Event_Wait(30);
    Event_OpenMessage(1, 0);
    if (Event_ChooseYesNo(-1, 0) != 0) {
        Event_Wait(10);
        Actor_SetAnimationAndWait(0, 34);
        Event_Wait(20);
        Actor_SetAnimationAndWait(1, 3);
        Event_Wait(20);
        Event_ShowMessage(1, 0);
        Event_Wait(10);
        Actor_SetAnimationAndWait(0, 33);
        Event_Wait(30);
        Actor_SetAnimationAndWait(1, 3);
        Event_Wait(20);
        Event_ShowMessage(ACTOR_GERALD, 0);
        gEventWork->message += 1;
    } else {
        Event_Wait(10);
        Actor_SetAnimationAndWait(0, 33);
        gEventWork->message += 2;
        Event_Wait(30);
        Actor_SetAnimationAndWait(1, 3);
        Event_Wait(20);
        Event_ShowMessage(1, 0);
    }
    Event_Wait(10);
    Actor_SetSpeed(1, 0x10000, 0x8000);
    Actor_WalkByAndWait(1, -16, 0);
    Actor_FaceDirection(1, 0, 0);
    Event_Wait(35);
    Actor_Jump(0, 6, 0);
    Actor_SetSpeed(0, 0x1e666, 0xf333);
    Actor_WalkByAndWait(0, -32, 0);
    record = Actor_Get(0);
    Actor_SetSpriteFlags(record, 1);
    Actor_FaceDirection(3, 0x4000, 0);
    Actor_FaceDirection(2, 0xc000, 0);
    Event_Wait(40);
    Actor_SetAnimation(0, 3);
    Event_Wait(30);
    Actor_SetAnimation(2, 3);
    Actor_SetAnimation(1, 3);
    Actor_SetAnimationAndWait(3, 3);
    Event_Wait(30);
    Actor_SetSpeed(1, 0x13333, 0x9999);
    Actor_SetSpeed(3, 0x13333, 0x9999);
    Actor_SetSpeed(2, 0x13333, 0x9999);
    Actor_SetAnimation(1, 2);
    record = Actor_Get(0);
    if (record != 0) {
        ObjectMotion_ResetAndSetPosition(1, record->x.part.pixel, record->z.part.pixel);
    }
    Actor_WaitForMove(1);
    Actor_SetPosition(1, 0, 0);
    Actor_SetAnimation(3, 2);
    record = Actor_Get(0);
    if (record != 0) {
        ObjectMotion_ResetAndSetPosition(3, record->x.part.pixel, record->z.part.pixel);
    }
    Actor_WaitForMove(3);
    Actor_SetPosition(3, 0, 0);
    Actor_SetAnimation(2, 2);
    record = Actor_Get(ACTOR_PARTY_LEADER);
    if (record != 0) {
        ObjectMotion_ResetAndSetPosition(2, record->x.part.pixel, record->z.part.pixel);
    }
    Actor_WaitForMove(2);
    Actor_SetPosition(2, 0, 0);
    Event_Wait(10);
    Event_End();
}

void FieldScene_RunScene3b8SequenceA(void)
{
    s32 record;
    s16 dir;
    u16 facing;

    record = Actor_Get(0);
    dir = (*(u16 *)(record + 6) + 0x2000) & -0x4000;
    Event_Begin();
    Battle_ResetEffectCounter();
    Event_SetMessage((s32)MsgTorebiLikeSleep);
    Event_OpenMessage(-1, 0);
    if (Event_ChooseYesNo(0, 0) != 0) {
    } else {
        if (GameFlag_IsSet(0x96a) != 0) {
            Event_Wait(20);
            Inn_PlaySleep(0);
            goto L_02000fe2;
        }
        Event_Wait(20);
        facing = dir;
        if (facing == 0) {
            Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 128, 120);
        }
        if (facing == 0x8000) {
            Actor_WalkToAndWait(0, 240, 120);
        }
        Actor_WalkToAndWait(0, 184, 120);
        Actor_FaceDirection(0, 0, 0);
        Event_Wait(10);
        Motion_LaunchFromFocusedObject(1, 16, 0, 0x8000);
        Object_RefreshSelectorById(1);
        Event_Wait(10);
        Event_ShowMessage(1, 0);
        Event_Wait(10);
        Actor_SetAnimationAndWait(0, 3);
        Event_Wait(20);
        Actor_SetSpeed(1, 0x10000, 0x8000);
        Actor_WalkByAndWait(ACTOR_GERALD, 40, 0);
        Actor_WalkByAndWait(1, 0, -32);
        Actor_FaceDirection(1, 0x6000, 0);
        Event_Wait(20);
        Actor_FaceDirection(0, 0x8000, 0);
        Event_Wait(20);
        Motion_LaunchFromFocusedObject(2, -16, 0, 0);
        Object_RefreshSelectorById(2);
        Event_Wait(10);
        Event_ShowMessage(2, 0);
        Event_Wait(10);
        Actor_SetAnimationAndWait(0, 3);
        Event_Wait(20);
        Actor_SetSpeed(2, 0x10000, 0x8000);
        Actor_WalkByAndWait(2, -40, 0);
        Actor_WalkByAndWait(2, 0, 40);
        Actor_FaceDirection(2, 0xe000, 0);
        Event_Wait(20);
        Actor_FaceDirection(0, 0, 0);
        Event_Wait(20);
        Motion_LaunchFromFocusedObject(3, 16, 0, 0x8000);
        Object_RefreshSelectorById(3);
        Event_Wait(10);
        Event_ShowMessage(3, 0);
        Event_Wait(10);
        Actor_SetAnimationAndWait(0, 3);
        Event_Wait(20);
        Actor_SetSpeed(3, 0x10000, 0x8000);
        Actor_WalkByAndWait(3, 40, 0);
        Actor_WalkByAndWait(3, 0, 40);
        Actor_FaceDirection(3, 0xa000, 0);
        Event_Wait(20);
        Camera_MoveTo(-1, -1, -1, 0);
        Actor_WalkByAndWait(0, -56, 0);
        Actor_WalkByAndWait(0, 0, -32);
        Actor_FaceDirection(0, 0x2000, 0);
        Event_Wait(30);
        Actor_SetAnimationAndWait(3, 3);
        Event_Wait(10);
        Event_ShowMessage(3, 0);
        Actor_SetAnimationAndWait(1, 3);
        Event_Wait(10);
        if (GameFlag_IsSet(0x96a) == 0) {
            Event_ShowMessage(1, 0);
        } else {
            AdvanceMessageCursor(1);
        }
        Actor_SetAnimationAndWait(2, 3);
        Event_Wait(10);
        if (GameFlag_IsSet(0x96a) == 0) {
            Event_ShowMessage(2, 0);
        } else {
            AdvanceMessageCursor(1);
        }
        Event_Wait(20);
        Actor_FaceDirection(0, 0, 0);
        Actor_FaceDirection(1, 0x8000, 0);
        Actor_FaceDirection(3, 0x8000, 0);
        Actor_FaceDirection(2, 0, 0);
        Inn_PlaySleep(0);
        Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x2000, 0);
        Actor_FaceDirection(1, 0x6000, 0);
        Actor_FaceDirection(3, 0xa000, 0);
        Actor_FaceDirection(2, 0xe000, 0);
        Event_Wait(20);
        Actor_SetAnimationAndWait(1, 3);
        Event_Wait(10);
        Event_ShowMessage(1, 0);
        Actor_SetAnimationAndWait(ACTOR_IVAN, 3);
        Event_Wait(10);
        if (GameFlag_IsSet(0x96a) == 0) {
            Event_ShowMessage(2, 0);
        } else {
            AdvanceMessageCursor(1);
        }
        Actor_SetAnimationAndWait(3, 3);
        Event_Wait(10);
        if (GameFlag_IsSet(0x96a) == 0) {
            Event_ShowMessage(ACTOR_MIA, 0);
        } else {
            AdvanceMessageCursor(1);
        }
        Actor_WalkByAndWait(1, 0, 32);
        Actor_WalkBy(1, -112, 0);
        Actor_WalkByAndWait(3, 0, -40);
        Actor_WalkBy(ACTOR_MIA, -112, 0);
        Event_Wait(50);
        Actor_WalkByAndWait(2, 0, -24);
        Actor_WaitForMove(ACTOR_GERALD);
        Actor_FaceDirection(0, 0x4000, 0);
        Actor_WalkByAndWait(1, 0, -16);
        Actor_WaitForMove(3);
        Actor_FaceDirection(3, 0xc000, 0);
        Event_Wait(20);
        Event_ShowMessage(ACTOR_GERALD, 0);
        Event_Wait(10);
        Actor_SetAnimationAndWait(0, 3);
        Event_Wait(20);
        Actor_SetSpeed(1, 0x13333, 0x9999);
        Actor_SetSpeed(2, 0x13333, 0x9999);
        Actor_SetSpeed(3, 0x13333, 0x9999);
        Actor_SetAnimation(1, 2);
        record = Actor_Get(0);
        if (record != 0) {
            ObjectMotion_ResetAndSetPosition(1, *(s16 *)(record + 10), *(s16 *)(record + 18));
        }
        Actor_WaitForMove(ACTOR_GERALD);
        Actor_SetPosition(1, 0, 0);
        Actor_SetAnimation(3, 2);
        record = Actor_Get(ACTOR_PARTY_LEADER);
        if (record != 0) {
            ObjectMotion_ResetAndSetPosition(3, *(s16 *)(record + 10), *(s16 *)(record + 18));
        }
        Actor_WaitForMove(ACTOR_MIA);
        Actor_SetPosition(3, 0, 0);
        Actor_SetAnimation(2, 2);
        record = Actor_Get(0);
        if (record != 0) {
            ObjectMotion_ResetAndSetPosition(2, *(s16 *)(record + 10), *(s16 *)(record + 18));
        }
        Actor_WaitForMove(2);
        Actor_SetPosition(2, 0, 0);
        GameFlag_Set(0x96a);
    }
    Event_End();
    L_02000fe2:;
}

void RunPrimaryEffectSequence(void)
{
    GameFlag_Set(2411);
    Event_Begin();
    Battle_ResetEffectCounter();
    Event_SetMessage((s32)MsgTorebiIodemIodem);
    Actor_WalkToAndWait(0, 520, 424);
    Actor_FaceDirection(0, 57344, 0);
    Camera_MoveTo(36700160, -1, 24117248, 1);
    Camera_WaitForMove();
    Actor_SetSpeed(20, 65536, 32768);
    Actor_WalkByAndWait(20, 40, 0);
    Actor_WalkToAndWait(20, 584, 360);
    Event_ShowMessage(21, 0);
    Event_Wait(10);
    Actor_ShowEmote(20, 257, 40);
    Actor_FaceDirection(20, 32768, 0);
    Actor_SetSpeed(21, 131072, 65536);
    Actor_SetSpeed(22, 131072, 65536);
    Actor_WalkTo(21, 528, 352);
    Actor_WalkToAndWait(22, 528, 368);
    Task_Wait(3);
    Actor_SetAnimation(21, 1);
    Event_Wait(30);
    Event_Wait(10);
    Actor_SetAnimationAndWait(22, 3);
    Event_Wait(30);
    Event_ShowMessage(22, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(20, 2);
    Event_Wait(20);
    Actor_WalkByAndWait(20, -16, 0);
    Event_Wait(10);
    Event_ShowMessage(20, 0);
    Event_Wait(10);
    Actor_SetAnimation(21, 4);
    Actor_SetAnimationAndWait(22, 4);
    Event_Wait(20);
    Event_Wait(10);
    Actor_ShowEmote(20, 261, 40);
    Event_ShowMessage(20, 0);
    Event_Wait(10);
    Actor_ShowEmote(21, 258, 40);
    Actor_SetSpeed(21, 65536, 32768);
    Actor_WalkByAndWait(21, 8, 0);
    Event_Wait(20);
    Event_ShowMessage(21, 0);
    Event_Wait(10);
    Actor_ShowEmote(20, 257, 80);
    Event_Wait(10);
    Actor_ShowEmote(22, 258, 40);
    Actor_SetSpeed(22, 65536, 32768);
    Actor_WalkByAndWait(22, 8, 0);
    Event_Wait(20);
    Event_ShowMessage(22, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(20, 4);
    Event_Wait(20);
    Event_ShowMessage(20, 0);
    Event_Wait(10);
    Actor_SetAnimation(21, 3);
    Actor_SetAnimationAndWait(22, 3);
    Event_Wait(30);
    Event_Wait(10);
    Actor_RunRepeatedMotion(21, 2);
    Event_Wait(20);
    Event_ShowMessage(21, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(22, 4);
    Event_Wait(20);
    Event_ShowMessage(22, 0);
    Event_Wait(10);
    Actor_ShowEmote(20, 261, 40);
    Event_ShowMessage(20, 0);
    Event_Wait(10);
    Actor_ShowEmote(21, 257, 40);
    Event_ShowMessage(21, 0);
    Event_Wait(10);
    Actor_ShowEmote(20, 258, 40);
    Event_ShowMessage(20, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(22, 2);
    Event_Wait(20);
    Event_ShowMessage(22, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(20, 3);
    Event_Wait(30);
    Event_Wait(10);
    Actor_RunRepeatedMotion(21, 2);
    Event_Wait(20);
    Event_ShowMessage(21, 0);
    Event_Wait(10);
    Actor_ShowEmote(20, 258, 40);
    Event_ShowMessage(20, 0);
    Event_Wait(20);
    Actor_FaceEachOther(21, 22, 60);
    Actor_FaceActor(21, 20, 0);
    Actor_FaceActor(22, 20, 0);
    Event_Wait(20);
    Event_Wait(10);
    Actor_RunRepeatedMotion(20, 2);
    Event_Wait(20);
    Event_ShowMessage(20, 0);
    Event_Wait(10);
    Actor_ShowEmote(21, 256, 40);
    Event_ShowMessage(21, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(22, 3);
    Event_Wait(30);
    Event_ShowMessage(22, 0);
    Event_Wait(10);
    Actor_SetAttachedEffect(20, 258);
    Actor_RunRepeatedMotion(20, 2);
    Event_Wait(30);
    Event_ShowMessage(20, 0);
    Event_Wait(10);
    Actor_ShowEmote(21, 257, 40);
    Event_ShowMessage(21, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(20, 3);
    Event_Wait(30);
    Event_Wait(10);
    Actor_ShowEmote(22, 257, 40);
    Event_ShowMessage(22, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(20, 4);
    Event_Wait(20);
    Event_ShowMessage(20, 0);
    Event_Wait(20);
    Actor_FaceEachOther(21, 22, 60);
    Actor_FaceActor(21, 20, 0);
    Actor_FaceActor(22, 20, 0);
    Event_Wait(20);
    Event_Wait(10);
    Actor_ShowEmote(20, 261, 40);
    Event_ShowMessage(20, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(21, 2);
    Event_Wait(20);
    Event_ShowMessage(21, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(22, 3);
    Event_Wait(30);
    Event_ShowMessage(22, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(20, 3);
    Event_Wait(30);
    Event_Wait(10);
    Actor_RunRepeatedMotion(22, 2);
    Event_Wait(20);
    Event_ShowMessage(22, 0);
    Event_Wait(10);
    Actor_ShowEmote(20, 258, 40);
    Event_ShowMessage(20, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(21, 2);
    Event_Wait(20);
    Event_ShowMessage(21, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(20, 4);
    Event_Wait(20);
    Event_ShowMessage(20, 0);
    Event_Wait(10);
    Actor_ShowEmote(22, 256, 40);
    Event_ShowMessage(22, 0);
    Event_Wait(10);
    Actor_ShowEmote(20, 258, 40);
    Event_ShowMessage(20, 0);
    Event_Wait(10);
    Actor_ShowEmote(21, 257, 40);
    Event_ShowMessage(21, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(20, 4);
    Event_Wait(20);
    Event_ShowMessage(20, 0);
    Event_Wait(20);
    Actor_FaceEachOther(21, 22, 60);
    Actor_FaceActor(21, 20, 0);
    Actor_FaceActor(22, 20, 0);
    Event_Wait(20);
    Event_Wait(10);
    Actor_RunRepeatedMotion(20, 2);
    Event_Wait(20);
    Event_ShowMessage(20, 0);
    Event_Wait(10);
    Actor_SetAttachedEffect(21, 258);
    Actor_SetAttachedEffect(22, 258);
    Actor_StartRepeatedMotion(21, 2);
    Actor_RunRepeatedMotion(22, 2);
    Event_Wait(30);
    Event_Wait(10);
    Actor_FaceDirection(20, 0, 0);
    Event_Wait(20);
    Event_ShowMessage(20, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(21, 4);
    Event_Wait(20);
    Event_ShowMessage(21, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(22, 4);
    Event_Wait(20);
    Event_ShowMessage(22, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(20, 2);
    Event_Wait(20);
    Actor_FaceDirection(20, 32768, 0);
    Event_Wait(20);
    Event_ShowMessage(20, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(21, 3);
    Event_Wait(30);
    Event_ShowMessage(21, 0);
    Event_Wait(20);
    Actor_ShowEmote(20, 261, 40);
    Event_ShowMessage(20, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(22, 3);
    Event_Wait(30);
    Event_ShowMessage(22, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(20, 3);
    Event_Wait(40);
    Actor_FaceEachOther(21, 22, 60);
    Actor_SetAnimationAndWait(21, 3);
    Event_Wait(20);
    Actor_SetAnimationAndWait(22, 3);
    Event_Wait(20);
    Actor_FaceDirection(22, 16384, 0);
    Event_Wait(40);
    Actor_SetSpeed(21, 85196, 42598);
    Actor_SetSpeed(22, 85196, 42598);
    Actor_WalkBy(21, 0, 120);
    Actor_WalkByAndWait(22, 0, 120);
    Actor_SetPosition(21, 0, 0);
    Actor_SetPosition(22, 0, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(20, 2);
    Event_Wait(40);
    Actor_SetSpeed(20, 52428, 26214);
    Actor_WalkByAndWait(20, -16, 0);
    Event_Wait(30);
    Event_ShowMessage(20, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(20, 4);
    Event_Wait(30);
    Actor_FaceDirection(20, 0, 0);
    Event_Wait(30);
    Actor_SetSpeed(20, 65536, 32768);
    Actor_WalkByAndWait(20, 120, 0);
    Actor_WalkByAndWait(20, 60, 0);
    Actor_SetPosition(20, 0, 0);
    Event_End();
}

void FieldScene_RunBranchingActorSequence(void)
{
    s32 record;
    s32 step;
    s32 pick;
    s32 state;
    u8 *work;

    Event_Begin();
    Event_SetMessage((s32)MsgTorebiCallsName);
    Actor_SetAnimation(0, 31);
    record = Actor_Get(0);
    Actor_SetSpriteFlags(record, 0);
    Actor_SetPosition(1, 0x680000, 0x680000);
    Actor_SetPosition(3, 0x580000, 0x780000);
    Actor_SetPosition(2, 0x780000, 0x780000);
    Actor_FaceDirection(1, 0x4000, 0);
    Actor_FaceDirection(3, 0, 0);
    Actor_FaceDirection(2, 0x8000, 0);
    state = (s32)gWork[0];
    *(s32 *)(state + 0x1c0) = 0x100;
    *(s32 *)(state + 0x1c8) = 12;
    Task_Wait(1);
    DisplayTransition_InitializeBattleEffectState(9);
    work = gWork[4];
    SetHalf((u16 *)(work + 0x52a), 0);
    SetHalf((u16 *)(work + 0x534), 0x1f1f);
    SetHalf((u16 *)(work + 0x536), 1);
    Event_SetStatus1c6();
    Event_WaitValue1c8Frames();
    for (step = 1; step <= 5; step++) {
        Task_Wait(3);
        *(u16 *)(work + 0x52a) = step;
    }
    Event_Wait(40);
    Actor_RunRepeatedMotion(0, 2);
    Event_Wait(30);
    for (step = 5; step <= 31; step++) {
        Task_Wait(3);
        *(u16 *)(work + 0x52a) = step;
    }
    SetHalf((u16 *)(work + 0x536), 31);
    state = (s32)gWork[0];
    *(s32 *)(state + 0x1c0) = 0x209;
    *(s32 *)(state + 0x1c8) = 24;
    Event_Wait(20);
    Actor_ShowEmote(1, 0x100, 50);
    Actor_FaceActor(1, 0, 40);
    Actor_RunRepeatedMotion(1, 2);
    Event_Wait(30);
    Event_Wait(10);
    Actor_ShowEmote(2, 0x101, 40);
    Actor_FaceDirection(2, 0xe000, 0);
    Event_Wait(30);
    Event_ShowMessage(2, 0);
    Event_Wait(10);
    Event_ShowMessage(3, 0);
    Actor_FaceDirection(3, 0xe000, 0);
    Event_Wait(30);
    Actor_RunRepeatedMotion(3, 2);
    Event_Wait(30);
    Actor_SetSpeed(3, 0x10000, 0x8000);
    Actor_WalkByAndWait(3, 0, -40);
    Actor_WalkByAndWait(3, 32, 0);
    Actor_FaceDirection(3, 0x2000, 0);
    Event_Wait(10);
    Event_ShowMessage(3, 0);
    record = Actor_Get(0);
    *(s32 *)(record + 16) += -0x30000;
    record = Actor_Get(0);
    *(s32 *)(record + 64) += -0x30000;
    Actor_SetAnimation(0, 32);
    Event_Wait(40);
    Actor_SetAnimationAndWait(0, 34);
    Event_Wait(30);
    Actor_SetAnimation(0, 33);
    Event_Wait(40);
    Actor_SetAnimationAndWait(1, 4);
    Event_Wait(20);
    Event_ShowMessage(1, 0);
    Event_Wait(10);
    Actor_ShowEmote(0, 0x102, 80);
    Actor_ShowEmote(2, 0x100, 50);
    Event_OpenMessage(2, 0);
    if (Event_ChooseYesNo(-1, 0) == 0) {
        Event_Wait(10);
        Actor_SetAnimationAndWait(0, 33);
        Event_Wait(20);
        Actor_ShowEmote(1, 0x103, 40);
        Actor_Jump(1, 4, 13);
        Actor_Jump(1, 4, 30);
        Event_ShowMessage(1, 0);
        *(u16 *)(gWork[0] + 0x1d8) += 1;
    } else {
        Event_Wait(10);
        Actor_SetAnimationAndWait(0, 34);
        Event_Wait(20);
        Actor_ShowEmote(1, 0x103, 40);
        Actor_Jump(1, 4, 13);
        Actor_Jump(1, 4, 30);
        *(u16 *)(gWork[0] + 0x1d8) += 1;
        Event_ShowMessage(1, 0);
    }
    Event_Wait(10);
    Actor_FaceDirection(2, 0xa000, 0);
    Event_Wait(30);
    Actor_SetAnimationAndWait(2, 4);
    Event_Wait(20);
    Actor_FaceDirection(1, 0x2000, 0);
    Event_Wait(30);
    Actor_RunRepeatedMotion(1, 2);
    Event_Wait(40);
    Actor_FaceDirection(1, 0xe000, 0);
    Actor_FaceDirection(2, 0xe000, 0);
    Event_Wait(30);
    Actor_SetAnimationAndWait(1, 3);
    Event_Wait(30);
    Event_ShowMessage(1, 0);
    Event_Wait(10);
    Actor_FaceDirection(3, 0x6000, 0);
    Event_Wait(50);
    Actor_FaceDirection(3, 0x2000, 0);
    Event_Wait(35);
    Actor_ShowEmote(3, 0x108, 50);
    Event_Wait(10);
    Actor_SetAnimationAndWait(3, 3);
    Event_Wait(30);
    Event_ShowMessage(3, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(2, 2);
    Event_Wait(20);
    Event_OpenMessage(2, 0);
    if (Event_ChooseYesNo(-1, 0) == 0) {
        Event_Wait(10);
        Actor_SetAnimationAndWait(0, 33);
        Event_Wait(20);
        Actor_ShowEmote(1, 0x107, 40);
        Event_ShowMessage(1, 0);
        pick = 0;
        *(u16 *)(gWork[0] + 0x1d8) += 1;
    } else {
        Event_Wait(10);
        Actor_SetAnimationAndWait(0, 34);
        Event_Wait(20);
        Actor_ShowEmote(1, 0x107, 40);
        *(u16 *)(gWork[0] + 0x1d8) += 1;
        pick = 1;
        Event_ShowMessage(1, 0);
    }
    Event_Wait(10);
    Actor_FaceDirection(2, 0xa000, 0);
    Event_Wait(30);
    Actor_SetAnimationAndWait(2, 4);
    Event_Wait(20);
    Actor_FaceDirection(1, 0x2000, 0);
    Event_Wait(30);
    Actor_RunRepeatedMotion(1, 2);
    Event_Wait(40);
    Actor_FaceDirection(1, 0xe000, 0);
    Actor_FaceDirection(2, 0xe000, 0);
    Event_Wait(30);
    Actor_SetAnimationAndWait(1, 3);
    Event_Wait(30);
    Event_ShowMessage(1, 0);
    Event_Wait(10);
    Actor_FaceDirection(3, 0x6000, 0);
    Event_Wait(50);
    Actor_FaceDirection(3, 0x2000, 0);
    Event_Wait(35);
    Actor_ShowEmote(3, 0x108, 50);
    Event_Wait(10);
    Actor_SetAnimationAndWait(3, 3);
    Event_Wait(30);
    Event_ShowMessage(3, 0);
    if (pick == 0) {
        Event_Wait(10);
        Actor_SetAnimationAndWait(2, 3);
        Event_Wait(30);
        Event_ShowMessage(2, 0);
        Event_Wait(10);
        Actor_RunRepeatedMotion(1, 2);
        Event_Wait(30);
        Event_ShowMessage(1, 0);
        *(u16 *)(gWork[0] + 0x1d8) += 2;
    } else {
        *(u16 *)(gWork[0] + 0x1d8) += 2;
        Event_Wait(10);
        Actor_SetAnimationAndWait(2, 4);
        Event_Wait(20);
        Event_ShowMessage(2, 0);
        Event_Wait(10);
        Actor_RunRepeatedMotion(1, 2);
        Event_Wait(20);
        Event_ShowMessage(1, 0);
    }
    Event_Wait(10);
    Actor_ShowEmote(0, 0x102, 60);
    Event_Wait(10);
    Actor_SetAnimationAndWait(1, 3);
    Event_Wait(30);
    Event_ShowMessage(1, 0);
    Event_Wait(20);
    Actor_FaceEachOther(2, 3, 40);
    Actor_FaceDirection(3, 0x2000, 0);
    Actor_FaceDirection(2, 0xe000, 0);
    Event_Wait(30);
    while ((gKeysHeld & 240) == 0) {
        Task_Wait(1);
    }
    Actor_Jump(0, 6, 0);
    Actor_SetSpeed(0, 0x1e666, 0xf333);
    Actor_WalkByAndWait(0, -32, -8);
    record = Actor_Get(0);
    Actor_SetSpriteFlags(record, 1);
    Event_Wait(20);
    Actor_FaceDirection(0, 0xc000, 0);
    Actor_FaceDirection(3, 0x4000, 0);
    Actor_FaceDirection(2, 0xc000, 0);
    Event_Wait(20);
    Event_Wait(10);
    Actor_RunRepeatedMotion(3, 2);
    Event_Wait(20);
    Event_ShowMessage(3, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(1, 2);
    Event_Wait(20);
    Event_ShowMessage(1, 0);
    Event_Wait(20);
    Actor_SetAnimationAndWait(2, 3);
    Event_Wait(30);
    Actor_FaceDirection(0, 0x4000, 0);
    Actor_FaceDirection(1, 0x2000, 0);
    Event_Wait(30);
    Event_ShowMessage(2, 0);
    Event_Wait(20);
    Actor_SetAnimation(0, 3);
    Actor_SetAnimation(1, 3);
    Actor_SetAnimationAndWait(3, 3);
    Event_Wait(30);
    Actor_SetSpeed(1, 0x13333, 0x9999);
    Actor_SetSpeed(3, 0x13333, 0x9999);
    Actor_SetSpeed(2, 0x13333, 0x9999);
    Actor_SetAnimation(1, 2);
    record = Actor_Get(0);
    if (record != 0) {
        ObjectMotion_ResetAndSetPosition(1, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Actor_WaitForMove(1);
    Actor_SetPosition(1, 0, 0);
    Actor_SetAnimation(3, 2);
    record = Actor_Get(0);
    if (record != 0) {
        ObjectMotion_ResetAndSetPosition(3, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Actor_WaitForMove(3);
    Actor_SetPosition(3, 0, 0);
    Actor_SetAnimation(2, 2);
    record = Actor_Get(0);
    if (record != 0) {
        ObjectMotion_ResetAndSetPosition(2, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Actor_WaitForMove(2);
    Actor_SetPosition(2, 0, 0);
    Event_Wait(10);
    Event_End();
}

void FieldScene_RunMainCutsceneSequence(void)
{
    s16 *position;

    Audio_PlayCue(30);
    Event_Begin();
    Event_SetMessage((s32)MsgTorebiWaitingCompanions);
    Event_SetStatus1c6();
    Event_WaitValue1c8Frames();
    Camera_MoveTo(0xd80000, -1, 0x2e00000, 1);
    Camera_WaitForMove();
    Event_Wait(20);
    Event_Wait(10);
    Event_ShowMessage(20, 0);
    Event_Wait(10);
    Actor_SetSpeed(14, 0xcccc, 0x6666);
    Actor_WalkByAndWait(14, 0, 16);
    Event_Wait(10);
    Actor_SetAnimationAndWait(14, 3);
    Event_Wait(30);
    Event_ShowMessage(14, 0);
    Event_Wait(20);
    Event_ShowMessage(20, 0);
    Object_AttachWorkTargetToObject(0, 1);
    Camera_WaitForMove();
    Event_Wait(40);
    Actor_SetSpeed(0, 0x10000, 0x8000);
    Actor_WalkToAndWait(0, 208, 0x2f8);
    Event_Wait(10);
    Camera_MoveTo(0xd80000, -1, 0x2e00000, 1);
    Motion_LaunchFromFocusedObject(1, -16, 16, 0xc000);
    Motion_LaunchFromFocusedObject(3, 0, 24, 0xc000);
    Motion_LaunchFromFocusedObject(2, 16, 16, 0xc000);
    Actor_WaitForMove(1);
    Event_Wait(30);
    Event_Wait(10);
    Actor_SetAnimationAndWait(14, 3);
    Event_Wait(30);
    Event_ShowMessage(14, 0);

    if (GameFlag_IsSet(0x951) != 0) {
        Event_Wait(10);
        Actor_SetAnimationAndWait(20, 3);
        Event_Wait(20);
        Event_ShowMessage(20, 0);
        Event_Wait(10);
        Actor_FaceDirection(14, 0xa000, 0);
        Event_Wait(30);
        Actor_SetAnimationAndWait(14, 3);
        Event_Wait(20);
        Event_ShowMessage(14, 0);
        Event_Wait(10);
        Actor_SetAnimationAndWait(20, 3);
        Event_Wait(20);
        Event_ShowMessage(20, 0);
        Actor_FaceDirection(20, 0xc000, 0);
        Event_Wait(20);
        Actor_SetSpeed(20, 0x10000, 0x8000);
        Actor_WalkByAndWait(20, 0, -16);
        Event_Wait(40);
        Actor_SetAnimationAndWait(20, 3);
        Event_Wait(40);
        Actor_FaceDirection(20, 0x4000, 0);
        Event_Wait(20);
        Actor_WalkByAndWait(20, 0, 32);
        Actor_FaceDirection(14, 0x8000, 0);
        Event_Wait(10);
        Actor_FaceDirection(20, 0, 0);
        Event_Wait(30);
        Actor_SetAnimationAndWait(20, 3);
        Event_Wait(30);
        Actor_FaceDirection(20, 0x4000, 0);
        Event_Wait(20);
        Actor_SetSpeed(20, 0xcccc, 0x6666);
        Actor_Get(20)->unknown_5a &= 0xfe;
        Actor_WalkByAndWait(20, 0, -16);
        Actor_Get(20)->unknown_5a |= 1;
        Actor_FaceDirection(14, 0x4000, 0);
        Event_Wait(40);
        Actor_SetSpeed(14, 0xcccc, 0x6666);
        Actor_WalkByAndWait(14, 0, 16);
        Event_Wait(40);
        UiText_DrawQuantity(164, 2);
        Event_ShowMessage(-1, 0);
        Item_ShowFound(164, 3);
        Party_GiveItem(164, 0);
        Actor_FaceDirection(0, 0xc000, 0);
        Event_Wait(30);
        Actor_WalkByAndWait(14, 0, -16);
        Actor_FaceDirection(14, 0x4000, 0);
        Event_Wait(30);
        Actor_RunRepeatedMotion(14, 2);
        Event_Wait(20);
        Event_OpenMessage(14, 0);
        AdvanceMessage(2);
    } else {
        AdvanceMessage(5);
        Event_Wait(10);
        Actor_SetAnimationAndWait(20, 3);
        Event_Wait(20);
        Event_ShowMessage(20, 0);
        Actor_FaceDirection(14, 0xa000, 0);
        Event_Wait(40);
        Actor_SetAnimationAndWait(14, 3);
        Event_Wait(20);
        Actor_SetAnimationAndWait(20, 3);
        Event_Wait(30);
        Actor_FaceDirection(14, 0x4000, 0);
        Event_Wait(30);
        Actor_RunRepeatedMotion(14, 2);
        Event_Wait(20);
        Event_OpenMessage(14, 0);
    }

    if (Event_ChooseYesNo(0, 0) == 0) {
        Event_Wait(20);
        Actor_SetAnimationAndWait(20, 3);
        Event_Wait(20);
        Event_ShowMessage(20, 0);
        AdvanceMessage(1);
    } else {
        Event_Wait(10);
        Actor_SetAnimationAndWait(20, 3);
        Event_Wait(20);
        AdvanceMessage(1);
        Event_ShowMessage(20, 0);
    }

    Event_Wait(10);
    Actor_FaceDirection(14, 0xa000, 0);
    Event_Wait(20);
    Actor_SetAnimationAndWait(14, 3);
    Event_Wait(30);
    Event_ShowMessage(14, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(20, 3);
    Event_Wait(30);
    Event_Wait(10);
    Actor_Jump(ACTOR_GERALD, 4, 13);
    Actor_Jump(1, 4, 30);
    Event_ShowMessage(1, 0);
    Event_Wait(10);
    Actor_FaceDirection(14, 0x4000, 0);
    Actor_RunRepeatedMotion(20, 2);
    Event_Wait(20);
    Actor_SetSpeed(14, 0x19999, 0xcccc);
    Actor_WalkByAndWait(20, 0, 16);
    Task_Wait(2);
    Actor_FaceDirection(20, 0x2000, 0);
    Event_Wait(10);
    Event_ShowMessage(20, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(14, 4);
    Event_Wait(30);
    Actor_FaceActor(14, 20, 30);
    Event_ShowMessage(14, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(20, 2);
    Event_Wait(20);
    Actor_FaceEachOther(14, ACTOR_GERALD, 0);
    Event_Wait(40);
    Event_ShowMessage(14, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(1, 2);
    Event_Wait(30);
    Actor_FaceActor(1, 2, 30);
    Event_Wait(10);
    Actor_RunRepeatedMotion(2, 2);
    Event_Wait(20);
    Actor_FaceActor(2, 1, 30);
    Event_ShowMessage(2, 0);
    Event_Wait(10);
    Actor_ShowEmote(1, 0x106, 50);
    Event_Wait(10);
    Actor_SetAnimationAndWait(14, 3);
    Event_Wait(30);
    Event_ShowMessage(14, 0);
    Event_Wait(10);
    Actor_FaceDirection(1, 0xc000, 0);
    Actor_FaceDirection(2, 0xc000, 0);
    Event_Wait(30);
    Event_Wait(10);
    Actor_RunRepeatedMotion(20, 2);
    Event_Wait(20);
    Event_ShowMessage(20, 0);
    Event_Wait(10);
    Actor_FaceEachOther(1, 0, 0);
    Actor_FaceEachOther(3, 2, 50);
    Actor_FaceDirection(0, 0xc000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xc000, 0);
    Actor_FaceDirection(3, 0xc000, 0);
    Actor_FaceDirection(2, 0xc000, 0);
    Event_Wait(30);
    Event_Wait(10);
    Actor_SetAnimationAndWait(20, 4);
    Event_Wait(20);
    Event_ShowMessage(20, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(14, 4);
    Event_Wait(20);
    Event_ShowMessage(14, 0);
    Event_Wait(10);
    Actor_ShowEmote(2, 0x101, 40);
    Event_ShowMessage(2, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(14, 2);
    Event_Wait(20);
    Event_OpenMessage(14, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        Event_Wait(20);
        Actor_SetAnimationAndWait(14, 3);
        Event_Wait(30);
        Event_ShowMessage(14, 0);
        AdvanceMessage(1);
    } else {
        Event_Wait(10);
        Actor_SetAnimationAndWait(14, 4);
        Event_Wait(20);
        AdvanceMessage(1);
        Event_ShowMessage(14, 0);
    }

    Event_Wait(10);
    Actor_ShowEmote(20, 0x102, 40);
    Event_ShowMessage(20, 0);
    Event_Wait(20);
    Actor_SetAnimation(0, 3);
    Actor_SetAnimation(ACTOR_GERALD, 3);
    Actor_SetAnimation(3, 3);
    Actor_SetAnimationAndWait(ACTOR_IVAN, 3);
    Event_Wait(50);
    Actor_FaceActor(14, 20, 60);
    Actor_FaceDirection(14, 0x4000, 0);
    Event_Wait(20);
    Event_ShowMessage(14, 0);
    Event_Wait(10);
    Actor_ShowEmote(20, 0x102, 40);
    Event_ShowMessage(20, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(20, 4);
    Event_Wait(20);
    Event_ShowMessage(20, 0);
    Event_Wait(10);
    Actor_ShowEmote(0, 0x102, 40);
    Event_Wait(10);
    Actor_SetAnimationAndWait(14, 4);
    Event_Wait(20);
    Event_ShowMessage(14, 0);
    Event_Wait(10);
    Actor_FaceEachOther(1, 0, 50);
    Actor_FaceDirection(0, 0xc000, 0);
    Actor_FaceDirection(1, 0xc000, 0);
    Event_Wait(30);
    Event_Wait(10);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
    Actor_SetAnimationAndWait(1, 3);
    Event_Wait(30);
    Event_Wait(10);
    Actor_SetAnimationAndWait(14, 3);
    Event_Wait(30);
    Event_ShowMessage(14, 0);
    Event_Wait(10);
    Actor_ShowEmote(3, 0x100, 40);
    Event_ShowMessage(3, 0);
    Event_Wait(10);
    Actor_ShowEmote(14, 0x102, 50);
    Actor_FaceActor(20, 14, 50);
    Event_Wait(10);
    Actor_RunRepeatedMotion(14, 2);
    Event_Wait(20);
    Event_ShowMessage(14, 0);
    Event_Wait(20);
    Actor_FaceEachOther(1, 0, 0);
    Actor_FaceEachOther(3, 2, 50);
    Actor_FaceDirection(0, 0xc000, 0);
    Actor_FaceDirection(1, 0xc000, 0);
    Actor_FaceDirection(3, 0xc000, 0);
    Actor_FaceDirection(2, 0xc000, 0);
    Event_Wait(50);
    Actor_FaceDirection(20, 0x2000, 0);
    Event_Wait(20);
    Event_ShowMessage(20, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(1, 2);
    Event_Wait(20);
    Event_ShowMessage(1, 0);
    Event_Wait(20);
    Actor_FaceDirection(14, 0x8000, 0);
    Event_Wait(40);
    Actor_ShowEmote(14, 0x105, 60);
    Event_ShowMessage(14, 0);
    Event_Wait(10);
    Actor_ShowEmote(2, 0x100, 40);
    Event_ShowMessage(2, 0);
    Event_Wait(10);
    Actor_FaceEachOther(14, 2, 40);
    Actor_SetAnimationAndWait(14, 3);
    Event_Wait(30);
    Event_ShowMessage(14, 0);
    Event_Wait(10);
    Actor_FaceEachOther(1, 0, 0);
    Actor_FaceEachOther(3, 2, 60);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    Actor_FaceDirection(1, 0xc000, 0);
    Actor_FaceDirection(3, 0xc000, 0);
    Actor_FaceDirection(2, 0xc000, 0);
    Event_Wait(50);
    Actor_RunRepeatedMotion(20, 2);
    Event_Wait(20);
    Event_ShowMessage(20, 0);
    Event_Wait(10);
    Actor_FaceEachOther(1, 0, 40);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 2);
    Event_Wait(20);
    Event_OpenMessage(1, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        Event_Wait(20);
        Event_ShowMessage(1, 0);
        AdvanceMessage(3);
    } else {

        Event_Wait(10);

        {
            u8 **scene_address = (u8 **)&gEventWork;

            AdvanceMessageAt(scene_address, 1);
            Event_OpenMessage(1, 0);
            if (Event_ChooseYesNo(0, 0) == 0) {
                Event_Wait(20);
                Event_ShowMessage(1, 0);
                AdvanceMessageAt(scene_address, 1);
            } else {
                AdvanceMessageAt(scene_address, 1);
                Event_ShowMessage(1, 0);
            }
        }
    }

    Event_Wait(10);
    Actor_SetAnimationAndWait(14, 4);
    Event_Wait(20);
    Event_ShowMessage(14, 0);
    Event_Wait(10);
    Actor_FaceDirection(0, 0xc000, 0);
    Actor_FaceDirection(1, 0xc000, 0);
    Event_Wait(40);
    Actor_RunRepeatedMotion(1, 2);
    Event_Wait(20);
    Event_Wait(10);
    Actor_SetAnimationAndWait(14, 3);
    Event_Wait(30);
    Event_ShowMessage(14, 0);
    Event_Wait(10);
    Actor_ShowEmote(3, 0x101, 40);
    Event_ShowMessage(3, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(14, 2);
    Event_Wait(20);
    Event_ShowMessage(14, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(2, 2);
    Event_Wait(20);
    Event_ShowMessage(2, 0);
    Event_Wait(10);
    Actor_FaceActor(20, 14, 40);
    Actor_RunRepeatedMotion(20, 2);
    Event_Wait(20);
    Event_ShowMessage(20, 0);
    Event_Wait(20);
    Actor_ShowEmote(14, 0x105, 70);
    Event_ShowMessage(14, 0);
    Event_Wait(10);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 0);
    Actor_ShowEmote(1, 0x101, 0);
    Actor_ShowEmote(ACTOR_MIA, 0x101, 0);
    Actor_ShowEmote(2, 0x101, 40);
    Event_Wait(10);
    Actor_SetAnimationAndWait(14, 4);
    Event_Wait(20);
    Event_ShowMessage(14, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(14, 3);
    Event_Wait(30);
    Event_ShowMessage(14, 0);
    Event_Wait(20);
    Actor_SetAnimationAndWait(ACTOR_GERALD, 3);
    Event_Wait(30);
    Event_ShowMessage(1, 0);
    Event_Wait(20);
    Actor_SetAnimationAndWait(14, 3);
    Event_Wait(30);
    Event_Wait(10);
    Actor_ShowEmote(ACTOR_IVAN, 0x101, 40);
    Event_ShowMessage(2, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(14, 3);
    Event_Wait(40);
    Actor_FaceActor(0, 2, 0);
    Actor_FaceActor(ACTOR_GERALD, ACTOR_IVAN, 0);
    Actor_FaceActor(3, 2, 0);
    Actor_FaceActor(20, 2, 0);
    Event_Wait(50);
    Event_Wait(10);
    Actor_RunRepeatedMotion(2, 2);
    Event_Wait(20);
    Event_ShowMessage(2, 0);
    Event_Wait(10);
    Actor_ShowEmote(14, 0x101, 40);
    Event_ShowMessage(14, 0);
    Event_Wait(10);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
    Actor_SetAnimation(1, 3);
    Actor_SetAnimationAndWait(3, 3);
    Event_Wait(30);
    Event_Wait(10);
    Actor_SetAnimationAndWait(2, 4);
    Event_Wait(20);
    Event_ShowMessage(2, 0);
    Event_Wait(20);
    Actor_SetAnimationAndWait(20, 3);
    Event_Wait(30);
    Actor_FaceActor(20, 14, 30);
    Actor_FaceDirection(0, 0xc000, 0);
    Actor_FaceDirection(1, 0xc000, 0);
    Actor_FaceDirection(3, 0xc000, 0);
    Event_Wait(20);
    Event_ShowMessage(20, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(14, 3);
    Event_Wait(30);
    Event_ShowMessage(14, 0);
    Event_Wait(10);
    Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Actor_StartRepeatedMotion(1, 2);
    Actor_StartRepeatedMotion(3, 2);
    Actor_RunRepeatedMotion(2, 2);
    Event_Wait(30);
    Event_Wait(10);
    Actor_RunRepeatedMotion(20, 2);
    Event_Wait(20);
    Event_ShowMessage(20, 0);
    Event_Wait(10);
    Actor_FaceDirection(14, 0x8000, 0);
    Event_Wait(20);
    Actor_SetAnimationAndWait(14, 4);
    Event_Wait(20);
    Event_ShowMessage(14, 0);
    Event_Wait(10);
    Actor_FaceEachOther(1, 0, 0);
    Actor_FaceEachOther(3, 2, 50);
    Actor_FaceDirection(0, 0xc000, 0);
    Actor_FaceDirection(1, 0xc000, 0);
    Actor_FaceDirection(ACTOR_MIA, 0xc000, 0);
    Actor_FaceDirection(2, 0xc000, 0);
    Event_Wait(40);
    Actor_FaceDirection(14, 0x4000, 0);
    Event_Wait(30);
    Actor_ShowEmote(14, 0x102, 40);
    Event_ShowMessage(14, 0);
    Event_Wait(10);
    Actor_ShowEmote(20, 0x102, 40);
    Event_ShowMessage(20, 0);
    Event_Wait(20);
    Actor_FaceEachOther(14, 20, 40);
    Actor_RunRepeatedMotion(14, 2);
    Event_Wait(20);
    Event_ShowMessage(14, 0);
    Event_Wait(10);
    Actor_SetAttachedEffect(20, 0x102);
    Actor_RunRepeatedMotion(20, 2);
    Event_Wait(20);
    Event_ShowMessage(20, 0);
    Actor_SetSpeed(20, 0x19999, 0xcccc);
    Actor_WalkByAndWait(20, 0, 24);
    Actor_FaceActor(0, 20, 0);
    Actor_FaceActor(1, 20, 0);
    Actor_FaceActor(3, 20, 0);
    Actor_FaceActor(2, 20, 0);
    Actor_FaceDirection(14, 0x4000, 0);
    Event_ShowMessage(14, 0);
    Event_Wait(10);
    Actor_ShowEmote(20, 0x100, 60);
    Actor_FaceActor(20, 14, 0);
    Actor_FaceDirection(0, 0xc000, 0);
    Actor_FaceDirection(1, 0xc000, 0);
    Actor_FaceDirection(3, 0xc000, 0);
    Actor_FaceDirection(2, 0xc000, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(20, 2);
    Event_Wait(20);
    Event_ShowMessage(20, 0);
    Event_Wait(30);
    Actor_ShowEmote(14, 0x105, 40);
    Event_ShowMessage(14, 0);
    Event_Wait(10);
    Actor_ShowEmote(20, 0x101, 60);
    Actor_SetSpeed(20, 0x13333, 0x9999);
    Actor_WalkByAndWait(20, 0, -24);
    Actor_FaceDirection(20, 0, 0);
    Event_Wait(20);
    Event_ShowMessage(20, 0);
    Event_Wait(10);
    Actor_FaceDirection(14, 0x8000, 0);
    Event_Wait(20);
    Actor_SetAnimationAndWait(14, 3);
    Event_Wait(30);
    Event_Wait(10);
    Actor_RunRepeatedMotion(20, 2);
    Event_Wait(20);
    Event_ShowMessage(20, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(14, 4);
    Event_Wait(20);
    Event_ShowMessage(14, 0);
    Event_Wait(10);
    Actor_ShowEmote(20, 0x102, 40);
    Event_ShowMessage(20, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(14, 3);
    Event_Wait(30);
    Event_ShowMessage(14, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(1, 2);
    Event_Wait(20);
    Event_ShowMessage(1, 0);
    Event_Wait(10);
    Actor_FaceDirection(14, 0x4000, 0);
    Event_Wait(30);
    Actor_SetAnimationAndWait(14, 3);
    Event_Wait(30);
    Event_ShowMessage(14, 0);
    Event_Wait(10);
    Actor_ShowEmote(3, 0x102, 40);
    Event_ShowMessage(3, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(14, 3);
    Event_Wait(30);
    Event_ShowMessage(14, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(2, 3);
    Event_Wait(30);
    Event_ShowMessage(2, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(14, 3);
    Event_Wait(30);
    Event_ShowMessage(14, 0);
    Event_Wait(10);
    Actor_FaceEachOther(1, 0, 0);
    Actor_FaceEachOther(3, 2, 50);
    Actor_FaceDirection(0, 0xc000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xc000, 0);
    Actor_FaceDirection(3, 0xc000, 0);
    Actor_FaceDirection(2, 0xc000, 0);
    Event_Wait(30);
    Event_Wait(10);
    Actor_RunRepeatedMotion(14, 2);
    Event_Wait(20);
    Event_ShowMessage(14, 0);
    Event_Wait(20);
    Actor_ShowEmote(0, 0x101, 0);
    Actor_ShowEmote(1, 0x101, 0);
    Actor_ShowEmote(3, 0x101, 0);
    Actor_ShowEmote(ACTOR_IVAN, 0x101, 50);
    Event_Wait(10);
    Actor_SetAnimationAndWait(14, 3);
    Event_Wait(30);
    Event_ShowMessage(14, 0);
    Event_Wait(10);
    Actor_ShowEmote(20, 0x102, 40);
    Event_ShowMessage(20, 0);
    Event_Wait(10);
    Actor_FaceEachOther(14, 20, 30);
    Event_ShowMessage(14, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(14, 4);
    Event_Wait(20);
    Event_ShowMessage(14, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(1, 2);
    Event_Wait(20);
    Event_ShowMessage(1, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(14, 2);
    Event_Wait(20);
    Actor_FaceDirection(14, 0x4000, 0);
    Event_Wait(20);
    Event_ShowMessage(14, 0);
    Event_Wait(10);
    Actor_ShowEmote(20, 0x101, 40);
    Event_ShowMessage(20, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(14, 3);
    Event_Wait(30);
    Actor_FaceDirection(14, 0x8000, 0);
    Event_Wait(20);
    Event_ShowMessage(14, 0);
    Event_Wait(10);
    Actor_ShowEmote(2, 0x101, 40);
    Event_ShowMessage(2, 0);
    Event_Wait(10);
    Actor_FaceDirection(14, 0x4000, 0);
    Actor_FaceDirection(20, 0x2000, 0);
    Event_Wait(30);
    Actor_SetAnimationAndWait(14, 3);
    Event_Wait(30);
    Event_Wait(10);
    Actor_RunRepeatedMotion(3, 2);
    Event_Wait(20);
    Event_ShowMessage(ACTOR_MIA, 0);
    Event_Wait(10);
    Actor_ShowEmote(14, 0x102, 40);
    Event_ShowMessage(14, 0);
    Event_Wait(20);
    Actor_SetAnimation(0, 3);
    Actor_SetAnimation(1, 3);
    Actor_SetAnimation(3, 3);
    Actor_SetAnimationAndWait(ACTOR_IVAN, 3);
    Event_Wait(30);
    Event_Wait(20);
    Actor_ShowEmote(14, 0x105, 60);
    Event_ShowMessage(14, 0);
    Event_Wait(10);
    Actor_ShowEmote(1, 0x101, 40);
    Event_ShowMessage(1, 0);
    Event_Wait(10);
    Actor_ShowEmote(14, 0x100, 40);
    Event_ShowMessage(14, 0);
    Event_Wait(10);
    Actor_FaceEachOther(1, 0, 50);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    Actor_FaceDirection(1, 0xc000, 0);
    Event_Wait(30);
    Event_Wait(10);
    Actor_SetAnimationAndWait(14, 3);
    Event_Wait(30);
    Event_ShowMessage(14, 0);
    Event_Wait(10);
    Actor_ShowEmote(3, 0x101, 40);
    Event_ShowMessage(ACTOR_MIA, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(14, 4);
    Event_Wait(20);
    Event_ShowMessage(14, 0);
    Event_Wait(10);
    Actor_ShowEmote(0, 0x101, 0);
    Actor_ShowEmote(ACTOR_GERALD, 0x101, 0);
    Actor_ShowEmote(2, 0x101, 0);
    Actor_ShowEmote(3, 0x101, 40);
    Event_Wait(10);
    Actor_SetAnimationAndWait(14, 4);
    Event_Wait(20);
    Event_ShowMessage(14, 0);
    Event_Wait(10);
    Actor_ShowEmote(2, 0x100, 40);
    Event_OpenMessage(2, 0);
    if (Event_ChooseYesNo(14, 0) == 0) {
        Event_Wait(10);
        Event_ShowMessage(2, 0);
        AdvanceMessage(1);
    } else {
        Event_Wait(10);
        AdvanceMessage(1);
        Event_ShowMessage(2, 0);
    }

    Event_Wait(10);
    Actor_RunRepeatedMotion(14, 2);
    Event_Wait(20);
    Event_ShowMessage(14, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(14, 4);
    Event_Wait(20);
    Event_ShowMessage(14, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(20, 2);
    Event_Wait(20);
    Actor_FaceDirection(20, 0, 0);
    Event_Wait(20);
    Event_ShowMessage(20, 0);
    Event_Wait(10);
    Actor_FaceActor(14, 20, 40);
    Actor_SetAnimationAndWait(14, 4);
    Event_Wait(30);
    Event_ShowMessage(14, 0);
    Event_Wait(10);
    Actor_FaceEachOther(1, 0, 40);
    Event_ShowMessage(1, 0);
    Actor_FaceDirection(14, 0x4000, 0);
    Actor_FaceDirection(20, 0x2000, 0);
    Event_Wait(30);
    Event_ShowMessage(14, 0);
    Event_Wait(10);
    Actor_FaceDirection(0, 0xc000, 0);
    Actor_FaceDirection(1, 0xc000, 0);
    Event_Wait(30);
    Actor_FaceEachOther(3, 2, 60);
    Actor_FaceDirection(3, 0xc000, 0);
    Actor_FaceDirection(2, 0xc000, 0);
    Event_Wait(30);
    Event_ShowMessage(3, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(2, 4);
    Event_Wait(20);
    Event_ShowMessage(2, 0);
    Event_Wait(10);
    Actor_FaceEachOther(20, 14, 60);
    Actor_FaceDirection(20, 0x2000, 0);
    Actor_FaceDirection(14, 0x4000, 0);
    Event_Wait(30);
    Actor_FaceDirection(14, 0x4000, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(14, 3);
    Event_Wait(30);
    Event_ShowMessage(14, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(20, 2);
    Event_Wait(20);
    Event_OpenMessage(20, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        Event_Wait(20);
        Actor_ShowEmote(1, 0x102, 40);
        Event_ShowMessage(1, 0);
        AdvanceMessage(1);
    } else {
        Event_Wait(10);
        AdvanceMessage(1);
        Event_ShowMessage(1, 0);
    }

    Event_Wait(10);
    Actor_RunRepeatedMotion(20, 2);
    Event_Wait(20);
    Event_ShowMessage(20, 0);
    Event_Wait(10);
    Actor_ShowEmote(20, 0x100, 40);
    Event_ShowMessage(20, 0);
    Event_Wait(20);
    Actor_SetAnimation(0, 3);
    Actor_SetAnimation(1, 3);
    Actor_SetAnimation(3, 3);
    Actor_SetAnimationAndWait(ACTOR_IVAN, 3);
    Event_Wait(30);
    Event_Wait(10);
    Actor_ShowEmote(14, 0x100, 40);
    Event_ShowMessage(14, 0);
    Event_Wait(10);
    Actor_FaceActor(20, 14, 40);
    Event_Wait(10);
    Actor_ShowEmote(20, 0x102, 40);
    Event_ShowMessage(20, 0);
    Event_Wait(10);
    Actor_FaceActor(14, 20, 40);
    Actor_SetAnimationAndWait(14, 3);
    Event_Wait(30);
    Event_ShowMessage(14, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(20, 2);
    Event_Wait(20);
    Event_ShowMessage(20, 0);
    Actor_FaceDirection(14, 0x4000, 0);
    Event_Wait(20);
    Actor_RunRepeatedMotion(14, 2);
    Event_Wait(20);
    Event_ShowMessage(14, 0);
    Event_Wait(10);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 80);
    Actor_ShowEmote(20, 0x102, 70);
    Actor_SetAnimationAndWait(14, 4);
    Event_Wait(20);
    Event_ShowMessage(14, 0);
    Event_Wait(10);
    Actor_FaceEachOther(1, 0, 0);
    Actor_FaceEachOther(3, 2, 50);
    Actor_FaceDirection(0, 0xc000, 0);
    Actor_FaceDirection(1, 0xc000, 0);
    Actor_FaceDirection(3, 0xc000, 0);
    Actor_FaceDirection(2, 0xc000, 0);
    Event_Wait(30);
    Event_Wait(10);
    Actor_ShowEmote(20, 0x106, 50);
    Event_ShowMessage(20, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(14, 3);
    Event_Wait(30);
    Actor_FaceDirection(20, 0x2000, 0);
    Event_ShowMessage(20, 0);
    Event_Wait(10);
    Actor_ShowEmote(14, 0x108, 40);
    Event_OpenMessage(14, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        Event_Wait(20);
        Actor_ShowEmote(14, 0x100, 40);
        Event_ShowMessage(14, 0);
        AdvanceMessage(1);
    } else {
        Event_Wait(10);
        Actor_ShowEmote(14, 0x100, 40);
        AdvanceMessage(1);
        Event_ShowMessage(14, 0);
    }

    Event_Wait(10);
    Actor_RunRepeatedMotion(20, 2);
    Event_Wait(20);
    Event_ShowMessage(20, 0);
    Event_Wait(10);
    Actor_FaceActor(14, 20, 40);
    Actor_SetAnimationAndWait(14, 3);
    Event_Wait(30);
    Event_ShowMessage(20, 0);
    Event_Wait(10);
    Actor_FaceActor(20, 14, 40);
    Event_ShowMessage(20, 0);
    Event_Wait(10);
    Actor_ShowEmote(14, 0x100, 40);
    Actor_FaceDirection(14, 0x8000, 0);
    Event_Wait(20);
    Event_ShowMessage(14, 0);
    Event_Wait(10);
    Actor_ShowEmote(20, 0x100, 40);
    Event_ShowMessage(20, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(14, 2);
    Event_Wait(20);
    Event_ShowMessage(14, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(20, 2);
    Event_Wait(20);
    Event_ShowMessage(20, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(20, 3);
    Event_Wait(30);
    Actor_SetAnimationAndWait(14, 3);
    Event_Wait(30);
    Actor_FaceDirection(20, 0x2000, 0);
    Event_Wait(20);
    Object_LinkObjectAndSetCallback(0, 20);
    Object_LinkObjectAndSetCallback(1, 20);
    Object_LinkObjectAndSetCallback(3, 20);
    Object_LinkObjectAndSetCallback(2, 20);
    Actor_SetSpeed(20, 0x10000, 0x8000);
    Actor_WalkByAndWait(20, 0, 32);
    Actor_FaceDirection(20, 0, 0);
    Actor_FaceDirection(14, 0x4000, 0);
    Event_Wait(20);
    Event_ShowMessage(20, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(20, 3);
    Event_Wait(30);
    Actor_WalkByAndWait(1, 16, 0);
    Actor_WalkBy(20, 0, 80);
    Event_Wait(40);
    Actor_WalkByAndWait(ACTOR_GERALD, -16, 0);
    Actor_FaceDirection(1, 0x4000, 0);
    Actor_FaceDirection(3, 0x4000, 0);
    Actor_WaitForMove(20);
    Event_Wait(80);
    Actor_SetPosition(20, 0, 0);
    Actor_FaceDirection(0, 0xc000, 0);
    Actor_FaceDirection(1, 0xc000, 0);
    Actor_FaceDirection(3, 0xc000, 0);
    Actor_FaceDirection(2, 0xc000, 0);
    Event_Wait(30);
    Object_LinkObjectAndSetCallback(0, 14);
    Object_LinkObjectAndSetCallback(1, 14);
    Object_LinkObjectAndSetCallback(3, 14);
    Object_LinkObjectAndSetCallback(2, 14);
    Event_Wait(30);
    Actor_SetAnimationAndWait(14, 3);
    Event_Wait(30);
    Actor_SetSpeed(14, 0xcccc, 0x6666);
    Actor_WalkByAndWait(14, 0, 24);
    Actor_WalkByAndWait(14, -80, 0);
    Event_Wait(10);
    Actor_FaceDirection(14, 0, 0);
    Event_Wait(20);
    Event_ShowMessage(14, 0);
    Event_Wait(20);
    Actor_FaceDirection(14, 0x4000, 0);
    Event_Wait(20);
    Actor_WalkByAndWait(14, 0, 48);
    Actor_WalkByAndWait(14, -64, 0);
    Actor_Stop(0);
    Actor_Stop(ACTOR_GERALD);
    Actor_Stop(3);
    Actor_Stop(2);
    Actor_SetPosition(14, 0, 0);
    Event_Wait(20);
    Actor_FaceEachOther(0, 3, 0);
    Actor_FaceEachOther(1, 2, 0);
    Event_Wait(30);
    Actor_SetAnimation(0, 3);
    Actor_SetAnimation(1, 3);
    Actor_SetAnimation(3, 3);
    Actor_SetAnimationAndWait(ACTOR_IVAN, 3);
    Event_Wait(30);
    Audio_PlayCue(17);
    Actor_SetSpeed(1, 0x13333, 0x9999);
    Actor_SetSpeed(2, 0x13333, 0x9999);
    Actor_SetSpeed(ACTOR_MIA, 0x13333, 0x9999);
    Actor_SetAnimation(1, 2);
    position = Actor_Get(0);
    if (position != 0)
        ObjectMotion_ResetAndSetPosition(1, position[5], position[9]);
    Actor_WaitForMove(1);
    Actor_SetPosition(1, 0, 0);
    Actor_SetAnimation(ACTOR_IVAN, 2);
    position = Actor_Get(ACTOR_PARTY_LEADER);
    if (position != 0)
        ObjectMotion_ResetAndSetPosition(2, position[5], position[9]);
    Actor_WaitForMove(2);
    Actor_SetPosition(2, 0, 0);
    Actor_SetAnimation(3, 2);
    position = Actor_Get(0);
    if (position != 0)
        ObjectMotion_ResetAndSetPosition(3, position[5], position[9]);
    Actor_WaitForMove(3);
    Actor_SetPosition(3, 0, 0);
    Event_Wait(10);
    Audio_PlayCueFromEventWork();
    Event_End();
}

void FieldScene_RunScene3b8_02003d40(void)
{
    struct EventWork *work;

    work = (struct EventWork *)gWork[0];
    Event_Begin();
    Audio_PlayCue(158);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x8000, 0x4000);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 2);
    if (work->touched_trigger == 32) {
        Map_ClearLayerEntryFlag(1);
        Event_Wait(10);
        Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, -16);
    } else {
        if (work->touched_trigger == 30) {
            Map_ClearLayerEntryFlag(4);
            Event_Wait(10);
            Actor_CenterAndWalk(ACTOR_PARTY_LEADER, 3, -16);
        } else {
            Map_ClearLayerEntryFlag(2);
            Event_Wait(10);
            Actor_CenterAndWalk(ACTOR_PARTY_LEADER, 3, -16);
        }
    }
    Event_Wait(16);
    Event_RequestExit(work->touched_trigger);
    Map_SetLayerEntryFlag(1);
    Map_SetLayerEntryFlag(2);
    Map_SetLayerEntryFlag(4);
    Event_End();
}

void RunSceneEffectSetup(void)
{
    Event_Begin();
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 32768, 16384);
    Audio_PlayCue(158);
    Map_AnimateCells(TorebiKyuden_CellSteps, 36, 10);
    Actor_CenterAndWalk(0, 2, -16);
    Event_Wait(16);
    Event_RequestExit(2);
    Event_End();
}

/* Actor 8's question once flag 2412 is set: the leader walks over, and a
 * no brings actors 8 and 9 into a longer exchange. */
void RunSupplementalSequenceOne(void)
{
    s32 p;
    GameFlag_Set(2412);
    Event_Begin();
    Battle_ResetEffectCounter();
    Actor_FaceDirection(8, 20480, 0);
    Actor_FaceDirection(9, 12288, 0);
    Actor_WalkToAndWait(0, 200, 272);
    Actor_FaceDirection(0, 49152, 0);
    Event_Wait(20);
    p = (s32)MsgTorebiArent;
    Event_SetMessage(p);
    Event_OpenMessage(8, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        Event_Wait(20);
        Event_SetMessage(p + 1);
        Event_ShowMessage(8, 0);
    } else {
        Event_Wait(20);
        Event_SetMessage(p + 2);
        Event_ShowMessage(8, 0);
        Event_Wait(20);
        Actor_FaceEachOther(8, 9, 60);
        Actor_FaceDirection(9, 12288, 0);
        Event_Wait(40);
        Actor_RunRepeatedMotion(9, 2);
        Event_Wait(30);
        Actor_FaceEachOther(8, 9, 30);
        Actor_SetAnimationAndWait(9, 3);
        Event_Wait(30);
        Actor_ShowEmote(8, 258, 50);
        Actor_FaceDirection(8, 20480, 0);
        Actor_FaceDirection(9, 12288, 0);
        Event_Wait(20);
        Actor_SetAnimationAndWait(8, 4);
        Event_Wait(20);
        Event_ShowMessage(8, 0);
        Event_Wait(10);
        Actor_RunRepeatedMotion(8, 2);
        Event_Wait(20);
        Event_ShowMessage(8, 0);
    }
    Event_End();
}

void SceneDialogue_ThankForSavingBabi(void)
{
    u32 i;
    s32 record;

    if (GameFlag_IsSet(0x96d) == 0) {
        GameFlag_Set(0x96d);
        Event_SetMessage((s32)MsgTorebiRobinIdReallyLikeThank);
        Event_ShowMessage(9, 0);
    } else {
        Event_SetMessage((s32)MsgTorebiBabiWaitingForAtColosseum);
        Event_ShowMessage(9, 0);
    }
}

void SceneDialogue_AskIfLeavingPalace(s32 a)
{
    s32 k = (s32)MsgTorebiWarriorsWhoStayed;

    Event_SetMessage(k);
    Event_OpenMessage(a, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        Event_SetMessage(k + 1);
        Event_ShowMessage(a, 0);
    } else {
        Event_SetMessage(k + 2);
        Event_ShowMessage(a, 0);
    }
}

void FieldScene_RunStepWithValue29e0(void)
{
    Event_Begin();
    Message_ShowCentered((s32)MsgFieldPeeredWell, 1);
    Message_ShowCentered((s32)MsgTorebiItsFilledWithFreshClean, 1);
    Event_End();
}

/* What the palace answers: in each scene, one table while Colosso is under
   way (flag 0x962), one once it is over (flag 0x950), one before. */
const struct SceneEvent *Scene_GetEvents(void)
{
    if (gGameState.scene == (s32)&SceneId_TorebiKyuden2) {
        if (GameFlag_IsSet(0x950) != 0) {
            return gTorebiKyudenEvents2AfterColosso;
        }
        if (GameFlag_IsSet(0x962) != 0) {
            return gTorebiKyudenEvents2Colosso;
        }
        return gTorebiKyudenEvents2;
    }
    if (GameFlag_IsSet(0x950) != 0) {
        return gTorebiKyudenEventsAfterColosso;
    }
    if (GameFlag_IsSet(0x962) != 0) {
        return gTorebiKyudenEventsColosso;
    }
    return gTorebiKyudenEventsOther;
}

/* The palace's scene start, entry veneer 0. */

/* Babi's Palace entry: record the entrance flags, then outside the second scene restore the lighthouse-item scene and the guards, set the entrance selector and, arriving by entrance 99 or 98, restore the party and run its scene. */
s32 TorebiKyuden_ApplyEntryState(void)
{
    struct FieldActor *actor;
    struct FieldSprite *sprite;
    u8 *buf;
    s32 set;

    Map_SetLayerEntryFlag(1);
    Map_SetLayerEntryFlag(2);
    Map_SetLayerEntryFlag(4);
    if (gGameState.entrance == 90) {
        GameFlag_Set(0x962);
    }
    if (gGameState.entrance == 91) {
        GameFlag_Set(0x962);
        GameFlag_Set(0x950);
    }
    if (gGameState.scene != (s32)&SceneId_TorebiKyuden2) {
        if (gGameState.entrance == 11) {
            GameFlag_Clear(0x12f);
        }
        if (GameFlag_IsSet(0x950)) {
            set = GameFlag_IsSet(0xf31);
            if (set) {
                Actor_SetPosition(16, 0, 0);
            } else {
                actor = Actor_Get(16);
                actor->unknown_5c = 1;
                actor->motion_flags = set;
                sprite = actor->sprite;
                actor->y.fixed = 0x40000;
                sprite->part_count = set;
                sprite->full_color = 0;
                sprite->palette = 0;
                buf = Heap_Allocate(17, 0x608);
                Item_LoadIcon(205);
                Vram_Load(sprite->vram_block, 128, buf + 0x400);
                Heap_Release(17);
            }
            if (gGameState.entrance == 33 && !GameFlag_IsSet(0x96f)) {
                GameFlag_Set(0x96f);
                Actor_SetPosition(14, 0xd00000, 0x2c00000);
                FieldScene_RunMainCutsceneSequence();
            }
            Actor_SetAnimation(14, 5);
            Actor_SetSpriteFlags(Actor_Get(14), 0);
        } else if (GameFlag_IsSet(0x962) && !GameFlag_IsSet(0x966)) {
            Actor_SetPosition(10, 0x780000, 0x480000);
        }
        gEventWork->start_transition = 0x209;
        Actor_Get(9)->collision_flags |= 4;
        if (gGameState.entrance == 99) {
            Party_RestoreAll();
            FieldScene_RunBranchingActorSequence();
            gGameState.entrance = 8;
        }
        if (gGameState.entrance == 98) {
            Party_RestoreAll();
            GameFlag_Set(0x966);
            GameFlag_Set(0x967);
            Actor_SetPosition(10, 0x380000, 0x780000);
            Actor_FaceDirection(10, 0xf000, 0);
            FieldScene_RunScene3b8SequenceB();
            gGameState.entrance = 8;
        }
    }
    return 0;
}
