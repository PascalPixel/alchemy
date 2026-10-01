#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "KYUDEN.H"
#include "CALL.H"

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

void Map_SetLayerEntryFlagFar(s32 value);
s32 Party_ListActiveOwners(s16 *owners);
void Owner_RecalculateRatios(s32 owner);
void InventorySnapshot_Restore(void);
void Party_AddActiveOwner(s32 owner);
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

struct OwnerState *Owner_GetState(s32 owner);

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
    Battle_ResetEffectCounterFar();
    if (Value1(Engine_GameFlagIsSet, 512) == 0) {
        Engine_GameFlagSet(512);
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
        Call3(Engine_ActorSetSpeed, a, 0x10000, 0x8000);
        Call3(Engine_ActorWalkByAndWait, a, 0, -48);
        Actor_WalkByAndWait(a, 64, 0);
        Actor_FaceDirection(a, c, 0);
    } else {
        Engine_GameFlagClear(512);
        GameFlag_Set(0x969);
        Call3(Engine_ActorFaceDirection, a, 0x4000, 0);
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
        Call3(Engine_ActorWalkByAndWait, a, -64, 0);
        Actor_WalkByAndWait(a, 0, 48);
    }
    Event_End();
}

void FieldScene_RunScene3b8_02000264(s32 a0)
{
    u32 i;
    s32 record;
    s32 base6_2241;

    Event_Begin();
    Battle_ResetEffectCounterFar();
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
            Engine_EventWait(10);
            Event_SetMessage((base6_2241 + 1));
        } else {
            Event_SetMessage((base6_2241 + 2));
        }
        Event_ShowMessage(a0, 0);
        Event_Wait(10);
        Engine_ActorSetAnimationAndWait(a0, 3);
        Engine_EventWait(20);
        Actor_SetSpeed(a0, 0x10000, 0x8000);
        Actor_WalkByAndWait(a0, -64, 0);
        Engine_ActorWalkByAndWait(a0, 0, 48);
    } else {
        Engine_EventSetMessage((s32)MsgTorebiTiredFeelFreeRest);
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

    obj = (u8 *)((s32 (*)())Object_GetById)();
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
    Call3(Engine_ActorFaceDirection, a, 0xe000, 0);
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
    Engine_EventShowMessage(a, 0);
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
    Battle_ResetEffectCounterFar();
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

    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgTorebiGetUp);
    Object_GetById(0)->active = 0;
    Object_GetById(10)->active = 0;
    Engine_TaskWait(1);
    *(volatile u16 *)0x04000000 = 0x1140;
    Call2(Engine_EventShowMessage, -1, 0);
    *(volatile u16 *)0x04000000 = 0x140;
    Object_GetById(0)->active = 1;
    Object_GetById(10)->active = 1;
    Engine_ActorSetAnimation(0, 31);
    record = Object_GetById(0);
    Engine_ActorSetSpriteFlags(record, 0);
    Call3(Engine_ActorSetPosition, 1, 0x780000, 0x680000);
    Call3(Engine_ActorSetPosition, 3, 0x680000, 0x500000);
    Call3(Engine_ActorSetPosition, 2, 0x780000, 0x780000);
    Engine_ActorFaceDirection(1, 0, 0);
    Engine_ActorFaceDirection(3, 0, 0);
    Call3(Engine_ActorFaceDirection, 2, 0xe000, 0);
    gEventWork->transition_frames = 60;
    Event_SetStatus1c6Far();
    Event_WaitValue1c8FramesFar();
    Engine_EventWait(20);
    gEventWork->transition_frames = 24;
    Call3(Engine_ActorSetSpeed, 3, 0x10000, 0x8000);
    Actor_WalkByAndWait(ACTOR_MIA, 16, 0);
    Engine_ActorFaceDirection(3, 0x2000, 0);
    Engine_EventWait(20);
    Engine_ActorRunRepeatedMotion(3, 2);
    Engine_EventWait(30);
    Engine_EventShowMessage(3, 0);
    Engine_EventWait(10);
    record = Object_GetById(0);
    record->z.fixed += -0x30000;
    record = Object_GetById(0);
    record->target_z += -0x30000;
    Engine_ActorSetAnimation(0, 32);
    Engine_EventWait(40);
    Engine_ActorSetAnimationAndWait(0, 34);
    Engine_EventWait(30);
    Engine_ActorSetAnimation(0, 33);
    Engine_EventWait(50);
    Engine_ActorRunRepeatedMotion(1, 2);
    Engine_EventWait(30);
    Event_ShowMessage(ACTOR_GERALD, 0);
    Engine_EventWait(10);
    Engine_ActorShowEmote(0, 0x105, 60);
    Engine_EventWait(20);
    Call3(Engine_ActorShowEmote, 1, 0x102, 60);
    Engine_EventShowMessage(1, 0);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(1, 4);
    Engine_EventWait(20);
    Engine_EventShowMessage(1, 0);
    Engine_EventWait(10);
    Call3(Engine_ActorShowEmote, 0, 0x102, 80);
    Call3(Engine_ActorShowEmote, 2, 0x106, 60);
    Engine_ActorFaceDirection(2, 0xc000, 0);
    Engine_EventWait(30);
    Engine_ActorSetAnimationAndWait(2, 4);
    Engine_EventWait(20);
    Engine_EventShowMessage(2, 0);
    Engine_EventWait(10);
    Engine_ActorFaceDirection(1, 0x4000, 0);
    Engine_EventWait(30);
    Engine_ActorRunRepeatedMotion(1, 2);
    Engine_EventWait(45);
    Engine_ActorFaceDirection(1, 0, 0);
    Engine_ActorFaceDirection(2, 0xe000, 0);
    Engine_EventWait(30);
    Engine_EventOpenMessage(1, 0);
    if (Engine_EventChooseYesNo(-1, 0) != 0) {
        Engine_EventWait(10);
        Engine_ActorSetAnimationAndWait(0, 34);
        Engine_EventWait(20);
        Engine_ActorSetAnimationAndWait(1, 3);
        Engine_EventWait(20);
        Engine_EventShowMessage(1, 0);
        Engine_EventWait(10);
        Engine_ActorSetAnimationAndWait(0, 33);
        Engine_EventWait(30);
        Engine_ActorSetAnimationAndWait(1, 3);
        Engine_EventWait(20);
        Event_ShowMessage(ACTOR_GERALD, 0);
        gEventWork->message += 1;
    } else {
        Engine_EventWait(10);
        Engine_ActorSetAnimationAndWait(0, 33);
        gEventWork->message += 2;
        Engine_EventWait(30);
        Engine_ActorSetAnimationAndWait(1, 3);
        Engine_EventWait(20);
        Engine_EventShowMessage(1, 0);
    }
    Engine_EventWait(10);
    Call3(Engine_ActorSetSpeed, 1, 0x10000, 0x8000);
    Call3(Engine_ActorWalkByAndWait, 1, -16, 0);
    Engine_ActorFaceDirection(1, 0, 0);
    Engine_EventWait(35);
    Engine_ActorJump(0, 6, 0);
    Call3(Engine_ActorSetSpeed, 0, 0x1e666, 0xf333);
    Engine_ActorWalkByAndWait(0, -32, 0);
    record = Object_GetById(0);
    Engine_ActorSetSpriteFlags(record, 1);
    Call3(Engine_ActorFaceDirection, 3, 0x4000, 0);
    Engine_ActorFaceDirection(2, 0xc000, 0);
    Engine_EventWait(40);
    Engine_ActorSetAnimation(0, 3);
    Event_Wait(30);
    Engine_ActorSetAnimation(2, 3);
    Engine_ActorSetAnimation(1, 3);
    Engine_ActorSetAnimationAndWait(3, 3);
    Engine_EventWait(30);
    Call3(Engine_ActorSetSpeed, 1, 0x13333, 0x9999);
    Call3(Engine_ActorSetSpeed, 3, 0x13333, 0x9999);
    Call3(Engine_ActorSetSpeed, 2, 0x13333, 0x9999);
    Engine_ActorSetAnimation(1, 2);
    record = Object_GetById(0);
    if (record != 0) {
        ObjectMotion_ResetAndSetPositionFar(1, record->x.part.pixel, record->z.part.pixel);
    }
    Engine_ActorWaitForMove(1);
    Engine_ActorSetPosition(1, 0, 0);
    Engine_ActorSetAnimation(3, 2);
    record = Object_GetById(0);
    if (record != 0) {
        ObjectMotion_ResetAndSetPositionFar(3, record->x.part.pixel, record->z.part.pixel);
    }
    Engine_ActorWaitForMove(3);
    Engine_ActorSetPosition(3, 0, 0);
    Engine_ActorSetAnimation(2, 2);
    record = Object_GetById(ACTOR_PARTY_LEADER);
    if (record != 0) {
        ObjectMotion_ResetAndSetPositionFar(2, record->x.part.pixel, record->z.part.pixel);
    }
    Engine_ActorWaitForMove(2);
    Engine_ActorSetPosition(2, 0, 0);
    Engine_EventWait(10);
    Engine_EventEnd();
}

void FieldScene_RunScene3b8SequenceA(void)
{
    s32 record;
    s16 dir;
    u16 facing;

    record = Object_GetById(0);
    dir = (*(u16 *)(record + 6) + 0x2000) & -0x4000;
    Engine_EventBegin();
    Battle_ResetEffectCounterFar();
    Engine_EventSetMessage((s32)MsgTorebiLikeSleep);
    Value2(Engine_EventOpenMessage, -1, 0);
    if (Engine_EventChooseYesNo(0, 0) != 0) {
    } else {
        if (Value1(Engine_GameFlagIsSet, 0x96a) != 0) {
            Engine_EventWait(20);
            Inn_PlaySleepFar(0);
            goto L_02000fe2;
        }
        Engine_EventWait(20);
        facing = dir;
        if (facing == 0) {
            Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 128, 120);
        }
        if (facing == 0x8000) {
            Engine_ActorWalkToAndWait(0, 240, 120);
        }
        Engine_ActorWalkToAndWait(0, 184, 120);
        Engine_ActorFaceDirection(0, 0, 0);
        Engine_EventWait(10);
        Motion_LaunchFromFocusedObjectFar(1, 16, 0, 0x8000);
        Object_RefreshSelectorByIdFar(1);
        Engine_EventWait(10);
        Engine_EventShowMessage(1, 0);
        Engine_EventWait(10);
        Engine_ActorSetAnimationAndWait(0, 3);
        Event_Wait(20);
        Call3(Engine_ActorSetSpeed, 1, 0x10000, 0x8000);
        Actor_WalkByAndWait(ACTOR_GERALD, 40, 0);
        Call3(Engine_ActorWalkByAndWait, 1, 0, -32);
        Engine_ActorFaceDirection(1, 0x6000, 0);
        Engine_EventWait(20);
        Engine_ActorFaceDirection(0, 0x8000, 0);
        Engine_EventWait(20);
        Motion_LaunchFromFocusedObjectFar(2, -16, 0, 0);
        Object_RefreshSelectorByIdFar(2);
        Engine_EventWait(10);
        Engine_EventShowMessage(2, 0);
        Engine_EventWait(10);
        Engine_ActorSetAnimationAndWait(0, 3);
        Engine_EventWait(20);
        Call3(Engine_ActorSetSpeed, 2, 0x10000, 0x8000);
        Call3(Engine_ActorWalkByAndWait, 2, -40, 0);
        Engine_ActorWalkByAndWait(2, 0, 40);
        Engine_ActorFaceDirection(2, 0xe000, 0);
        Engine_EventWait(20);
        Engine_ActorFaceDirection(0, 0, 0);
        Engine_EventWait(20);
        Motion_LaunchFromFocusedObjectFar(3, 16, 0, 0x8000);
        Object_RefreshSelectorByIdFar(3);
        Engine_EventWait(10);
        Engine_EventShowMessage(3, 0);
        Engine_EventWait(10);
        Engine_ActorSetAnimationAndWait(0, 3);
        Engine_EventWait(20);
        Call3(Engine_ActorSetSpeed, 3, 0x10000, 0x8000);
        Engine_ActorWalkByAndWait(3, 40, 0);
        Engine_ActorWalkByAndWait(3, 0, 40);
        Engine_ActorFaceDirection(3, 0xa000, 0);
        Engine_EventWait(20);
        Call4(Engine_CameraMoveTo, -1, -1, -1, 0);
        Call3(Engine_ActorWalkByAndWait, 0, -56, 0);
        Call3(Engine_ActorWalkByAndWait, 0, 0, -32);
        Engine_ActorFaceDirection(0, 0x2000, 0);
        Engine_EventWait(30);
        Engine_ActorSetAnimationAndWait(3, 3);
        Event_Wait(10);
        Engine_EventShowMessage(3, 0);
        Engine_ActorSetAnimationAndWait(1, 3);
        Engine_EventWait(10);
        if (Engine_GameFlagIsSet(0x96a) == 0) {
            Engine_EventShowMessage(1, 0);
        } else {
            AdvanceMessageCursor(1);
        }
        Engine_ActorSetAnimationAndWait(2, 3);
        Engine_EventWait(10);
        if (GameFlag_IsSet(0x96a) == 0) {
            Engine_EventShowMessage(2, 0);
        } else {
            AdvanceMessageCursor(1);
        }
        Engine_EventWait(20);
        Engine_ActorFaceDirection(0, 0, 0);
        Call3(Engine_ActorFaceDirection, 1, 0x8000, 0);
        Call3(Engine_ActorFaceDirection, 3, 0x8000, 0);
        Engine_ActorFaceDirection(2, 0, 0);
        Inn_PlaySleepFar(0);
        Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x2000, 0);
        Call3(Engine_ActorFaceDirection, 1, 0x6000, 0);
        Call3(Engine_ActorFaceDirection, 3, 0xa000, 0);
        Engine_ActorFaceDirection(2, 0xe000, 0);
        Engine_EventWait(20);
        Engine_ActorSetAnimationAndWait(1, 3);
        Engine_EventWait(10);
        Engine_EventShowMessage(1, 0);
        Actor_SetAnimationAndWait(ACTOR_IVAN, 3);
        Engine_EventWait(10);
        if (Engine_GameFlagIsSet(0x96a) == 0) {
            Engine_EventShowMessage(2, 0);
        } else {
            AdvanceMessageCursor(1);
        }
        Engine_ActorSetAnimationAndWait(3, 3);
        Engine_EventWait(10);
        if (GameFlag_IsSet(0x96a) == 0) {
            Event_ShowMessage(ACTOR_MIA, 0);
        } else {
            AdvanceMessageCursor(1);
        }
        Engine_ActorWalkByAndWait(1, 0, 32);
        Call3(Engine_ActorWalkBy, 1, -112, 0);
        Call3(Engine_ActorWalkByAndWait, 3, 0, -40);
        Actor_WalkBy(ACTOR_MIA, -112, 0);
        Event_Wait(50);
        Call3(Engine_ActorWalkByAndWait, 2, 0, -24);
        Actor_WaitForMove(ACTOR_GERALD);
        Call3(Engine_ActorFaceDirection, 0, 0x4000, 0);
        Call3(Engine_ActorWalkByAndWait, 1, 0, -16);
        Engine_ActorWaitForMove(3);
        Engine_ActorFaceDirection(3, 0xc000, 0);
        Engine_EventWait(20);
        Event_ShowMessage(ACTOR_GERALD, 0);
        Engine_EventWait(10);
        Engine_ActorSetAnimationAndWait(0, 3);
        Engine_EventWait(20);
        Call3(Engine_ActorSetSpeed, 1, 0x13333, 0x9999);
        Call3(Engine_ActorSetSpeed, 2, 0x13333, 0x9999);
        Call3(Engine_ActorSetSpeed, 3, 0x13333, 0x9999);
        Engine_ActorSetAnimation(1, 2);
        record = Object_GetById(0);
        if (record != 0) {
            ObjectMotion_ResetAndSetPositionFar(1, *(s16 *)(record + 10), *(s16 *)(record + 18));
        }
        Actor_WaitForMove(ACTOR_GERALD);
        Engine_ActorSetPosition(1, 0, 0);
        Engine_ActorSetAnimation(3, 2);
        record = Object_GetById(ACTOR_PARTY_LEADER);
        if (record != 0) {
            ObjectMotion_ResetAndSetPositionFar(3, *(s16 *)(record + 10), *(s16 *)(record + 18));
        }
        Actor_WaitForMove(ACTOR_MIA);
        Engine_ActorSetPosition(3, 0, 0);
        Engine_ActorSetAnimation(2, 2);
        record = Object_GetById(0);
        if (record != 0) {
            ObjectMotion_ResetAndSetPositionFar(2, *(s16 *)(record + 10), *(s16 *)(record + 18));
        }
        Engine_ActorWaitForMove(2);
        Engine_ActorSetPosition(2, 0, 0);
        GameFlag_Set(0x96a);
    }
    Engine_EventEnd();
    L_02000fe2:;
}

void RunPrimaryEffectSequence(void)
{
    Engine_GameFlagSet(2411);
    Engine_EventBegin();
    Battle_ResetEffectCounterFar();
    Engine_EventSetMessage((s32)MsgTorebiIodemIodem);
    Call3(Engine_ActorWalkToAndWait, 0, 520, 424);
    Call3(Engine_ActorFaceDirection, 0, 57344, 0);
    Camera_MoveTo(36700160, -1, 24117248, 1);
    Engine_CameraWaitForMove();
    Call3(Engine_ActorSetSpeed, 20, 65536, 32768);
    Engine_ActorWalkByAndWait(20, 40, 0);
    Call3(Engine_ActorWalkToAndWait, 20, 584, 360);
    Event_ShowMessage(21, 0);
    Engine_EventWait(10);
    Call3(Engine_ActorShowEmote, 20, 257, 40);
    Call3(Engine_ActorFaceDirection, 20, 32768, 0);
    Call3(Engine_ActorSetSpeed, 21, 131072, 65536);
    Call3(Engine_ActorSetSpeed, 22, 131072, 65536);
    Call3(Engine_ActorWalkTo, 21, 528, 352);
    Engine_ActorWalkToAndWait(22, 528, 368);
    Engine_TaskWait(3);
    Actor_SetAnimation(21, 1);
    Engine_EventWait(30);
    Engine_EventWait(10);
    Actor_SetAnimationAndWait(22, 3);
    Engine_EventWait(30);
    Event_ShowMessage(22, 0);
    Engine_EventWait(10);
    Actor_RunRepeatedMotion(20, 2);
    Engine_EventWait(20);
    Call3(Engine_ActorWalkByAndWait, 20, -16, 0);
    Engine_EventWait(10);
    Event_ShowMessage(20, 0);
    Engine_EventWait(10);
    Actor_SetAnimation(21, 4);
    Actor_SetAnimationAndWait(22, 4);
    Engine_EventWait(20);
    Engine_EventWait(10);
    Call3(Engine_ActorShowEmote, 20, 261, 40);
    Event_ShowMessage(20, 0);
    Engine_EventWait(10);
    Call3(Engine_ActorShowEmote, 21, 258, 40);
    Call3(Engine_ActorSetSpeed, 21, 65536, 32768);
    Engine_ActorWalkByAndWait(21, 8, 0);
    Engine_EventWait(20);
    Event_ShowMessage(21, 0);
    Engine_EventWait(10);
    Call3(Engine_ActorShowEmote, 20, 257, 80);
    Engine_EventWait(10);
    Call3(Engine_ActorShowEmote, 22, 258, 40);
    Call3(Engine_ActorSetSpeed, 22, 65536, 32768);
    Engine_ActorWalkByAndWait(22, 8, 0);
    Engine_EventWait(20);
    Event_ShowMessage(22, 0);
    Engine_EventWait(10);
    Actor_SetAnimationAndWait(20, 4);
    Engine_EventWait(20);
    Event_ShowMessage(20, 0);
    Engine_EventWait(10);
    Actor_SetAnimation(21, 3);
    Actor_SetAnimationAndWait(22, 3);
    Engine_EventWait(30);
    Engine_EventWait(10);
    Actor_RunRepeatedMotion(21, 2);
    Engine_EventWait(20);
    Event_ShowMessage(21, 0);
    Engine_EventWait(10);
    Actor_SetAnimationAndWait(22, 4);
    Engine_EventWait(20);
    Event_ShowMessage(22, 0);
    Engine_EventWait(10);
    Call3(Engine_ActorShowEmote, 20, 261, 40);
    Event_ShowMessage(20, 0);
    Engine_EventWait(10);
    Call3(Engine_ActorShowEmote, 21, 257, 40);
    Event_ShowMessage(21, 0);
    Engine_EventWait(10);
    Call3(Engine_ActorShowEmote, 20, 258, 40);
    Event_ShowMessage(20, 0);
    Engine_EventWait(10);
    Actor_RunRepeatedMotion(22, 2);
    Engine_EventWait(20);
    Event_ShowMessage(22, 0);
    Engine_EventWait(10);
    Actor_SetAnimationAndWait(20, 3);
    Engine_EventWait(30);
    Engine_EventWait(10);
    Actor_RunRepeatedMotion(21, 2);
    Engine_EventWait(20);
    Event_ShowMessage(21, 0);
    Engine_EventWait(10);
    Call3(Engine_ActorShowEmote, 20, 258, 40);
    Event_ShowMessage(20, 0);
    Engine_EventWait(20);
    Engine_ActorFaceEachOther(21, 22, 60);
    Engine_ActorFaceActor(21, 20, 0);
    Engine_ActorFaceActor(22, 20, 0);
    Engine_EventWait(20);
    Engine_EventWait(10);
    Actor_RunRepeatedMotion(20, 2);
    Engine_EventWait(20);
    Event_ShowMessage(20, 0);
    Engine_EventWait(10);
    Call3(Engine_ActorShowEmote, 21, 256, 40);
    Event_ShowMessage(21, 0);
    Engine_EventWait(10);
    Actor_SetAnimationAndWait(22, 3);
    Engine_EventWait(30);
    Event_ShowMessage(22, 0);
    Engine_EventWait(10);
    Actor_SetAttachedEffect(20, 258);
    Actor_RunRepeatedMotion(20, 2);
    Engine_EventWait(30);
    Event_ShowMessage(20, 0);
    Engine_EventWait(10);
    Call3(Engine_ActorShowEmote, 21, 257, 40);
    Event_ShowMessage(21, 0);
    Engine_EventWait(10);
    Actor_SetAnimationAndWait(20, 3);
    Engine_EventWait(30);
    Engine_EventWait(10);
    Call3(Engine_ActorShowEmote, 22, 257, 40);
    Event_ShowMessage(22, 0);
    Engine_EventWait(10);
    Actor_SetAnimationAndWait(20, 4);
    Engine_EventWait(20);
    Event_ShowMessage(20, 0);
    Engine_EventWait(20);
    Engine_ActorFaceEachOther(21, 22, 60);
    Engine_ActorFaceActor(21, 20, 0);
    Engine_ActorFaceActor(22, 20, 0);
    Engine_EventWait(20);
    Engine_EventWait(10);
    Call3(Engine_ActorShowEmote, 20, 261, 40);
    Event_ShowMessage(20, 0);
    Engine_EventWait(10);
    Actor_RunRepeatedMotion(21, 2);
    Engine_EventWait(20);
    Event_ShowMessage(21, 0);
    Engine_EventWait(10);
    Actor_SetAnimationAndWait(22, 3);
    Engine_EventWait(30);
    Event_ShowMessage(22, 0);
    Engine_EventWait(10);
    Actor_SetAnimationAndWait(20, 3);
    Engine_EventWait(30);
    Engine_EventWait(10);
    Actor_RunRepeatedMotion(22, 2);
    Engine_EventWait(20);
    Event_ShowMessage(22, 0);
    Engine_EventWait(10);
    Call3(Engine_ActorShowEmote, 20, 258, 40);
    Event_ShowMessage(20, 0);
    Engine_EventWait(10);
    Actor_RunRepeatedMotion(21, 2);
    Engine_EventWait(20);
    Event_ShowMessage(21, 0);
    Engine_EventWait(10);
    Actor_SetAnimationAndWait(20, 4);
    Engine_EventWait(20);
    Event_ShowMessage(20, 0);
    Engine_EventWait(10);
    Call3(Engine_ActorShowEmote, 22, 256, 40);
    Event_ShowMessage(22, 0);
    Engine_EventWait(10);
    Call3(Engine_ActorShowEmote, 20, 258, 40);
    Event_ShowMessage(20, 0);
    Engine_EventWait(10);
    Call3(Engine_ActorShowEmote, 21, 257, 40);
    Event_ShowMessage(21, 0);
    Engine_EventWait(10);
    Actor_SetAnimationAndWait(20, 4);
    Engine_EventWait(20);
    Event_ShowMessage(20, 0);
    Engine_EventWait(20);
    Engine_ActorFaceEachOther(21, 22, 60);
    Engine_ActorFaceActor(21, 20, 0);
    Engine_ActorFaceActor(22, 20, 0);
    Engine_EventWait(20);
    Engine_EventWait(10);
    Actor_RunRepeatedMotion(20, 2);
    Engine_EventWait(20);
    Event_ShowMessage(20, 0);
    Engine_EventWait(10);
    Actor_SetAttachedEffect(21, 258);
    Actor_SetAttachedEffect(22, 258);
    Actor_StartRepeatedMotion(21, 2);
    Actor_RunRepeatedMotion(22, 2);
    Engine_EventWait(30);
    Engine_EventWait(10);
    Engine_ActorFaceDirection(20, 0, 0);
    Engine_EventWait(20);
    Event_ShowMessage(20, 0);
    Engine_EventWait(10);
    Actor_SetAnimationAndWait(21, 4);
    Engine_EventWait(20);
    Event_ShowMessage(21, 0);
    Engine_EventWait(10);
    Actor_SetAnimationAndWait(22, 4);
    Engine_EventWait(20);
    Event_ShowMessage(22, 0);
    Engine_EventWait(10);
    Actor_RunRepeatedMotion(20, 2);
    Engine_EventWait(20);
    Call3(Engine_ActorFaceDirection, 20, 32768, 0);
    Engine_EventWait(20);
    Event_ShowMessage(20, 0);
    Engine_EventWait(10);
    Actor_SetAnimationAndWait(21, 3);
    Engine_EventWait(30);
    Event_ShowMessage(21, 0);
    Engine_EventWait(20);
    Call3(Engine_ActorShowEmote, 20, 261, 40);
    Event_ShowMessage(20, 0);
    Engine_EventWait(10);
    Actor_SetAnimationAndWait(22, 3);
    Engine_EventWait(30);
    Event_ShowMessage(22, 0);
    Engine_EventWait(10);
    Actor_SetAnimationAndWait(20, 3);
    Engine_EventWait(40);
    Engine_ActorFaceEachOther(21, 22, 60);
    Actor_SetAnimationAndWait(21, 3);
    Engine_EventWait(20);
    Actor_SetAnimationAndWait(22, 3);
    Engine_EventWait(20);
    Engine_ActorFaceDirection(22, 16384, 0);
    Engine_EventWait(40);
    Call3(Engine_ActorSetSpeed, 21, 85196, 42598);
    Call3(Engine_ActorSetSpeed, 22, 85196, 42598);
    Engine_ActorWalkBy(21, 0, 120);
    Engine_ActorWalkByAndWait(22, 0, 120);
    Engine_ActorSetPosition(21, 0, 0);
    Engine_ActorSetPosition(22, 0, 0);
    Engine_EventWait(10);
    Actor_RunRepeatedMotion(20, 2);
    Engine_EventWait(40);
    Call3(Engine_ActorSetSpeed, 20, 52428, 26214);
    Engine_ActorWalkByAndWait(20, -16, 0);
    Engine_EventWait(30);
    Event_ShowMessage(20, 0);
    Engine_EventWait(10);
    Actor_SetAnimationAndWait(20, 4);
    Engine_EventWait(30);
    Engine_ActorFaceDirection(20, 0, 0);
    Engine_EventWait(30);
    Call3(Engine_ActorSetSpeed, 20, 65536, 32768);
    Engine_ActorWalkByAndWait(20, 120, 0);
    Engine_ActorWalkByAndWait(20, 60, 0);
    Engine_ActorSetPosition(20, 0, 0);
    Engine_EventEnd();
}

void FieldScene_RunBranchingActorSequence(void)
{
    s32 record;
    s32 step;
    s32 pick;
    s32 state;
    u8 *work;

    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgTorebiCallsName);
    Engine_ActorSetAnimation(0, 31);
    record = Object_GetById(0);
    Engine_ActorSetSpriteFlags(record, 0);
    Call3(Engine_ActorSetPosition, 1, 0x680000, 0x680000);
    Call3(Engine_ActorSetPosition, 3, 0x580000, 0x780000);
    Call3(Engine_ActorSetPosition, 2, 0x780000, 0x780000);
    Call3(Engine_ActorFaceDirection, 1, 0x4000, 0);
    Engine_ActorFaceDirection(3, 0, 0);
    Call3(Engine_ActorFaceDirection, 2, 0x8000, 0);
    state = (s32)gWork[0];
    *(s32 *)(state + 0x1c0) = 0x100;
    *(s32 *)(state + 0x1c8) = 12;
    Engine_TaskWait(1);
    DisplayTransition_InitializeBattleEffectStateFar(9);
    work = gWork[4];
    SetHalf((u16 *)(work + 0x52a), 0);
    SetHalf((u16 *)(work + 0x534), 0x1f1f);
    SetHalf((u16 *)(work + 0x536), 1);
    Event_SetStatus1c6Far();
    Event_WaitValue1c8FramesFar();
    for (step = 1; step <= 5; step++) {
        Engine_TaskWait(3);
        *(u16 *)(work + 0x52a) = step;
    }
    Engine_EventWait(40);
    Engine_ActorRunRepeatedMotion(0, 2);
    Engine_EventWait(30);
    for (step = 5; step <= 31; step++) {
        Engine_TaskWait(3);
        *(u16 *)(work + 0x52a) = step;
    }
    SetHalf((u16 *)(work + 0x536), 31);
    state = (s32)gWork[0];
    *(s32 *)(state + 0x1c0) = 0x209;
    *(s32 *)(state + 0x1c8) = 24;
    Engine_EventWait(20);
    Call3(Engine_ActorShowEmote, 1, 0x100, 50);
    Engine_ActorFaceActor(1, 0, 40);
    Engine_ActorRunRepeatedMotion(1, 2);
    Engine_EventWait(30);
    Engine_EventWait(10);
    Call3(Engine_ActorShowEmote, 2, 0x101, 40);
    Call3(Engine_ActorFaceDirection, 2, 0xe000, 0);
    Engine_EventWait(30);
    Engine_EventShowMessage(2, 0);
    Engine_EventWait(10);
    Engine_EventShowMessage(3, 0);
    Engine_ActorFaceDirection(3, 0xe000, 0);
    Engine_EventWait(30);
    Engine_ActorRunRepeatedMotion(3, 2);
    Engine_EventWait(30);
    Call3(Engine_ActorSetSpeed, 3, 0x10000, 0x8000);
    Call3(Engine_ActorWalkByAndWait, 3, 0, -40);
    Engine_ActorWalkByAndWait(3, 32, 0);
    Engine_ActorFaceDirection(3, 0x2000, 0);
    Engine_EventWait(10);
    Engine_EventShowMessage(3, 0);
    record = Object_GetById(0);
    *(s32 *)(record + 16) += -0x30000;
    record = Object_GetById(0);
    *(s32 *)(record + 64) += -0x30000;
    Engine_ActorSetAnimation(0, 32);
    Engine_EventWait(40);
    Engine_ActorSetAnimationAndWait(0, 34);
    Engine_EventWait(30);
    Engine_ActorSetAnimation(0, 33);
    Engine_EventWait(40);
    Engine_ActorSetAnimationAndWait(1, 4);
    Engine_EventWait(20);
    Engine_EventShowMessage(1, 0);
    Engine_EventWait(10);
    Call3(Engine_ActorShowEmote, 0, 0x102, 80);
    Call3(Engine_ActorShowEmote, 2, 0x100, 50);
    Engine_EventOpenMessage(2, 0);
    if (Engine_EventChooseYesNo(-1, 0) == 0) {
        Engine_EventWait(10);
        Engine_ActorSetAnimationAndWait(0, 33);
        Engine_EventWait(20);
        Call3(Engine_ActorShowEmote, 1, 0x103, 40);
        Engine_ActorJump(1, 4, 13);
        Engine_ActorJump(1, 4, 30);
        Engine_EventShowMessage(1, 0);
        *(u16 *)(gWork[0] + 0x1d8) += 1;
    } else {
        Engine_EventWait(10);
        Engine_ActorSetAnimationAndWait(0, 34);
        Engine_EventWait(20);
        Call3(Engine_ActorShowEmote, 1, 0x103, 40);
        Engine_ActorJump(1, 4, 13);
        Engine_ActorJump(1, 4, 30);
        *(u16 *)(gWork[0] + 0x1d8) += 1;
        Engine_EventShowMessage(1, 0);
    }
    Engine_EventWait(10);
    Engine_ActorFaceDirection(2, 0xa000, 0);
    Engine_EventWait(30);
    Engine_ActorSetAnimationAndWait(2, 4);
    Engine_EventWait(20);
    Call3(Engine_ActorFaceDirection, 1, 0x2000, 0);
    Engine_EventWait(30);
    Engine_ActorRunRepeatedMotion(1, 2);
    Engine_EventWait(40);
    Call3(Engine_ActorFaceDirection, 1, 0xe000, 0);
    Engine_ActorFaceDirection(2, 0xe000, 0);
    Engine_EventWait(30);
    Engine_ActorSetAnimationAndWait(1, 3);
    Engine_EventWait(30);
    Engine_EventShowMessage(1, 0);
    Engine_EventWait(10);
    Engine_ActorFaceDirection(3, 0x6000, 0);
    Engine_EventWait(50);
    Engine_ActorFaceDirection(3, 0x2000, 0);
    Engine_EventWait(35);
    Engine_ActorShowEmote(3, 0x108, 50);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(3, 3);
    Engine_EventWait(30);
    Engine_EventShowMessage(3, 0);
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(2, 2);
    Engine_EventWait(20);
    Engine_EventOpenMessage(2, 0);
    if (Engine_EventChooseYesNo(-1, 0) == 0) {
        Engine_EventWait(10);
        Engine_ActorSetAnimationAndWait(0, 33);
        Engine_EventWait(20);
        Call3(Engine_ActorShowEmote, 1, 0x107, 40);
        Engine_EventShowMessage(1, 0);
        pick = 0;
        *(u16 *)(gWork[0] + 0x1d8) += 1;
    } else {
        Engine_EventWait(10);
        Engine_ActorSetAnimationAndWait(0, 34);
        Engine_EventWait(20);
        Call3(Engine_ActorShowEmote, 1, 0x107, 40);
        *(u16 *)(gWork[0] + 0x1d8) += 1;
        pick = 1;
        Engine_EventShowMessage(1, 0);
    }
    Engine_EventWait(10);
    Engine_ActorFaceDirection(2, 0xa000, 0);
    Engine_EventWait(30);
    Engine_ActorSetAnimationAndWait(2, 4);
    Engine_EventWait(20);
    Call3(Engine_ActorFaceDirection, 1, 0x2000, 0);
    Engine_EventWait(30);
    Engine_ActorRunRepeatedMotion(1, 2);
    Engine_EventWait(40);
    Call3(Engine_ActorFaceDirection, 1, 0xe000, 0);
    Engine_ActorFaceDirection(2, 0xe000, 0);
    Engine_EventWait(30);
    Engine_ActorSetAnimationAndWait(1, 3);
    Engine_EventWait(30);
    Engine_EventShowMessage(1, 0);
    Engine_EventWait(10);
    Engine_ActorFaceDirection(3, 0x6000, 0);
    Engine_EventWait(50);
    Engine_ActorFaceDirection(3, 0x2000, 0);
    Engine_EventWait(35);
    Engine_ActorShowEmote(3, 0x108, 50);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(3, 3);
    Engine_EventWait(30);
    Engine_EventShowMessage(3, 0);
    if (pick == 0) {
        Engine_EventWait(10);
        Engine_ActorSetAnimationAndWait(2, 3);
        Engine_EventWait(30);
        Engine_EventShowMessage(2, 0);
        Engine_EventWait(10);
        Engine_ActorRunRepeatedMotion(1, 2);
        Engine_EventWait(30);
        Engine_EventShowMessage(1, 0);
        *(u16 *)(gWork[0] + 0x1d8) += 2;
    } else {
        *(u16 *)(gWork[0] + 0x1d8) += 2;
        Engine_EventWait(10);
        Engine_ActorSetAnimationAndWait(2, 4);
        Engine_EventWait(20);
        Engine_EventShowMessage(2, 0);
        Engine_EventWait(10);
        Engine_ActorRunRepeatedMotion(1, 2);
        Engine_EventWait(20);
        Engine_EventShowMessage(1, 0);
    }
    Engine_EventWait(10);
    Engine_ActorShowEmote(0, 0x102, 60);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(1, 3);
    Engine_EventWait(30);
    Engine_EventShowMessage(1, 0);
    Engine_EventWait(20);
    Engine_ActorFaceEachOther(2, 3, 40);
    Call3(Engine_ActorFaceDirection, 3, 0x2000, 0);
    Engine_ActorFaceDirection(2, 0xe000, 0);
    Engine_EventWait(30);
    while ((gKeysHeld & 240) == 0) {
        Engine_TaskWait(1);
    }
    Engine_ActorJump(0, 6, 0);
    Call3(Engine_ActorSetSpeed, 0, 0x1e666, 0xf333);
    Engine_ActorWalkByAndWait(0, -32, -8);
    record = Object_GetById(0);
    Engine_ActorSetSpriteFlags(record, 1);
    Engine_EventWait(20);
    Call3(Engine_ActorFaceDirection, 0, 0xc000, 0);
    Call3(Engine_ActorFaceDirection, 3, 0x4000, 0);
    Engine_ActorFaceDirection(2, 0xc000, 0);
    Engine_EventWait(20);
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(3, 2);
    Engine_EventWait(20);
    Engine_EventShowMessage(3, 0);
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(1, 2);
    Engine_EventWait(20);
    Engine_EventShowMessage(1, 0);
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(2, 3);
    Engine_EventWait(30);
    Call3(Engine_ActorFaceDirection, 0, 0x4000, 0);
    Engine_ActorFaceDirection(1, 0x2000, 0);
    Engine_EventWait(30);
    Engine_EventShowMessage(2, 0);
    Engine_EventWait(20);
    Engine_ActorSetAnimation(0, 3);
    Engine_ActorSetAnimation(1, 3);
    Engine_ActorSetAnimationAndWait(3, 3);
    Engine_EventWait(30);
    Call3(Engine_ActorSetSpeed, 1, 0x13333, 0x9999);
    Call3(Engine_ActorSetSpeed, 3, 0x13333, 0x9999);
    Call3(Engine_ActorSetSpeed, 2, 0x13333, 0x9999);
    Engine_ActorSetAnimation(1, 2);
    record = Object_GetById(0);
    if (record != 0) {
        ObjectMotion_ResetAndSetPositionFar(1, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Engine_ActorWaitForMove(1);
    Engine_ActorSetPosition(1, 0, 0);
    Engine_ActorSetAnimation(3, 2);
    record = Object_GetById(0);
    if (record != 0) {
        ObjectMotion_ResetAndSetPositionFar(3, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Engine_ActorWaitForMove(3);
    Engine_ActorSetPosition(3, 0, 0);
    Engine_ActorSetAnimation(2, 2);
    record = Object_GetById(0);
    if (record != 0) {
        ObjectMotion_ResetAndSetPositionFar(2, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Engine_ActorWaitForMove(2);
    Engine_ActorSetPosition(2, 0, 0);
    Engine_EventWait(10);
    Engine_EventEnd();
}

void FieldScene_RunMainCutsceneSequence(void)
{
    s16 *position;

    Call1(Engine_AudioPlayCue, 30);
    Engine_EventBegin();
    Call1(Engine_EventSetMessage, (s32)MsgTorebiWaitingCompanions);
    Event_SetStatus1c6Far();
    Event_WaitValue1c8FramesFar();
    Call4(Engine_CameraMoveTo, 0xd80000, -1, 0x2e00000, 1);
    Engine_CameraWaitForMove();
    Engine_EventWait(20);
    Engine_EventWait(10);
    Event_ShowMessage(20, 0);
    Engine_EventWait(10);
    Call3(Engine_ActorSetSpeed, 14, 0xcccc, 0x6666);
    Engine_ActorWalkByAndWait(14, 0, 16);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(14, 3);
    Engine_EventWait(30);
    Engine_EventShowMessage(14, 0);
    Engine_EventWait(20);
    Engine_EventShowMessage(20, 0);
    Object_AttachWorkTargetToObjectFar(0, 1);
    Engine_CameraWaitForMove();
    Engine_EventWait(40);
    Call3(Engine_ActorSetSpeed, 0, 0x10000, 0x8000);
    Call3(Engine_ActorWalkToAndWait, 0, 208, 0x2f8);
    Engine_EventWait(10);
    Camera_MoveTo(0xd80000, -1, 0x2e00000, 1);
    Call4(Motion_LaunchFromFocusedObjectFar, 1, -16, 16, 0xc000);
    Call4(Motion_LaunchFromFocusedObjectFar, 3, 0, 24, 0xc000);
    Call4(Motion_LaunchFromFocusedObjectFar, 2, 16, 16, 0xc000);
    Engine_ActorWaitForMove(1);
    Engine_EventWait(30);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(14, 3);
    Engine_EventWait(30);
    Engine_EventShowMessage(14, 0);

    if (Engine_GameFlagIsSet(0x951) != 0) {
        Engine_EventWait(10);
        Engine_ActorSetAnimationAndWait(20, 3);
        Engine_EventWait(20);
        Engine_EventShowMessage(20, 0);
        Engine_EventWait(10);
        Actor_FaceDirection(14, 0xa000, 0);
        Engine_EventWait(30);
        Engine_ActorSetAnimationAndWait(14, 3);
        Engine_EventWait(20);
        Engine_EventShowMessage(14, 0);
        Engine_EventWait(10);
        Engine_ActorSetAnimationAndWait(20, 3);
        Engine_EventWait(20);
        Engine_EventShowMessage(20, 0);
        Call3(Engine_ActorFaceDirection, 20, 0xc000, 0);
        Engine_EventWait(20);
        Call3(Engine_ActorSetSpeed, 20, 0x10000, 0x8000);
        Call3(Engine_ActorWalkByAndWait, 20, 0, -16);
        Event_Wait(40);
        Engine_ActorSetAnimationAndWait(20, 3);
        Engine_EventWait(40);
        Call3(Engine_ActorFaceDirection, 20, 0x4000, 0);
        Engine_EventWait(20);
        Engine_ActorWalkByAndWait(20, 0, 32);
        Engine_ActorFaceDirection(14, 0x8000, 0);
        Engine_EventWait(10);
        Engine_ActorFaceDirection(20, 0, 0);
        Engine_EventWait(30);
        Engine_ActorSetAnimationAndWait(20, 3);
        Engine_EventWait(30);
        Call3(Engine_ActorFaceDirection, 20, 0x4000, 0);
        Engine_EventWait(20);
        Call3(Engine_ActorSetSpeed, 20, 0xcccc, 0x6666);
        Object_GetById(20)->unknown_5a &= 0xfe;
        Call3(Engine_ActorWalkByAndWait, 20, 0, -16);
        Object_GetById(20)->unknown_5a |= 1;
        Call3(Engine_ActorFaceDirection, 14, 0x4000, 0);
        Engine_EventWait(40);
        Call3(Engine_ActorSetSpeed, 14, 0xcccc, 0x6666);
        Engine_ActorWalkByAndWait(14, 0, 16);
        Engine_EventWait(40);
        UiText_DrawQuantity(164, 2);
        Engine_EventShowMessage(-1, 0);
        Engine_ItemShowFound(164, 3);
        Engine_PartyGiveItem(164, 0);
        Engine_ActorFaceDirection(0, 0xc000, 0);
        Engine_EventWait(30);
        Call3(Engine_ActorWalkByAndWait, 14, 0, -16);
        Actor_FaceDirection(14, 0x4000, 0);
        Engine_EventWait(30);
        Engine_ActorRunRepeatedMotion(14, 2);
        Engine_EventWait(20);
        Engine_EventOpenMessage(14, 0);
        AdvanceMessage(2);
    } else {
        AdvanceMessage(5);
        Engine_EventWait(10);
        Engine_ActorSetAnimationAndWait(20, 3);
        Engine_EventWait(20);
        Engine_EventShowMessage(20, 0);
        Engine_ActorFaceDirection(14, 0xa000, 0);
        Event_Wait(40);
        Actor_SetAnimationAndWait(14, 3);
        Engine_EventWait(20);
        Actor_SetAnimationAndWait(20, 3);
        Engine_EventWait(30);
        Actor_FaceDirection(14, 0x4000, 0);
        Engine_EventWait(30);
        Engine_ActorRunRepeatedMotion(14, 2);
        Engine_EventWait(20);
        Engine_EventOpenMessage(14, 0);
    }

    if (Event_ChooseYesNo(0, 0) == 0) {
        Engine_EventWait(20);
        Engine_ActorSetAnimationAndWait(20, 3);
        Event_Wait(20);
        Engine_EventShowMessage(20, 0);
        AdvanceMessage(1);
    } else {
        Event_Wait(10);
        Engine_ActorSetAnimationAndWait(20, 3);
        Event_Wait(20);
        AdvanceMessage(1);
        Engine_EventShowMessage(20, 0);
    }

    Event_Wait(10);
    Actor_FaceDirection(14, 0xa000, 0);
    Engine_EventWait(20);
    Actor_SetAnimationAndWait(14, 3);
    Engine_EventWait(30);
    Engine_EventShowMessage(14, 0);
    Engine_EventWait(10);
    Actor_SetAnimationAndWait(20, 3);
    Event_Wait(30);
    Engine_EventWait(10);
    Actor_Jump(ACTOR_GERALD, 4, 13);
    Engine_ActorJump(1, 4, 30);
    Engine_EventShowMessage(1, 0);
    Engine_EventWait(10);
    Call3(Engine_ActorFaceDirection, 14, 0x4000, 0);
    Engine_ActorRunRepeatedMotion(20, 2);
    Engine_EventWait(20);
    Actor_SetSpeed(14, 0x19999, 0xcccc);
    Engine_ActorWalkByAndWait(20, 0, 16);
    Task_Wait(2);
    Actor_FaceDirection(20, 0x2000, 0);
    Event_Wait(10);
    Engine_EventShowMessage(20, 0);
    Event_Wait(10);
    Engine_ActorSetAnimationAndWait(14, 4);
    Event_Wait(30);
    Engine_ActorFaceActor(14, 20, 30);
    Engine_EventShowMessage(14, 0);
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(20, 2);
    Engine_EventWait(20);
    Actor_FaceEachOther(14, ACTOR_GERALD, 0);
    Engine_EventWait(40);
    Engine_EventShowMessage(14, 0);
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(1, 2);
    Engine_EventWait(30);
    Engine_ActorFaceActor(1, 2, 30);
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(2, 2);
    Engine_EventWait(20);
    Engine_ActorFaceActor(2, 1, 30);
    Engine_EventShowMessage(2, 0);
    Engine_EventWait(10);
    Engine_ActorShowEmote(1, 0x106, 50);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(14, 3);
    Engine_EventWait(30);
    Engine_EventShowMessage(14, 0);
    Event_Wait(10);
    Call3(Engine_ActorFaceDirection, 1, 0xc000, 0);
    Call3(Engine_ActorFaceDirection, 2, 0xc000, 0);
    Engine_EventWait(30);
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(20, 2);
    Event_Wait(20);
    Engine_EventShowMessage(20, 0);
    Engine_EventWait(10);
    Engine_ActorFaceEachOther(1, 0, 0);
    Engine_ActorFaceEachOther(3, 2, 50);
    Call3(Engine_ActorFaceDirection, 0, 0xc000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xc000, 0);
    Call3(Engine_ActorFaceDirection, 3, 0xc000, 0);
    Engine_ActorFaceDirection(2, 0xc000, 0);
    Engine_EventWait(30);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(20, 4);
    Engine_EventWait(20);
    Engine_EventShowMessage(20, 0);
    Engine_EventWait(10);
    Actor_SetAnimationAndWait(14, 4);
    Engine_EventWait(20);
    Engine_EventShowMessage(14, 0);
    Engine_EventWait(10);
    Call3(Engine_ActorShowEmote, 2, 0x101, 40);
    Engine_EventShowMessage(2, 0);
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(14, 2);
    Engine_EventWait(20);
    Engine_EventOpenMessage(14, 0);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
        Engine_EventWait(20);
        Engine_ActorSetAnimationAndWait(14, 3);
        Engine_EventWait(30);
        Engine_EventShowMessage(14, 0);
        AdvanceMessage(1);
    } else {
        Engine_EventWait(10);
        Engine_ActorSetAnimationAndWait(14, 4);
        Engine_EventWait(20);
        AdvanceMessage(1);
        Event_ShowMessage(14, 0);
    }

    Engine_EventWait(10);
    Call3(Engine_ActorShowEmote, 20, 0x102, 40);
    Engine_EventShowMessage(20, 0);
    Engine_EventWait(20);
    Engine_ActorSetAnimation(0, 3);
    Actor_SetAnimation(ACTOR_GERALD, 3);
    Engine_ActorSetAnimation(3, 3);
    Actor_SetAnimationAndWait(ACTOR_IVAN, 3);
    Engine_EventWait(50);
    Engine_ActorFaceActor(14, 20, 60);
    Engine_ActorFaceDirection(14, 0x4000, 0);
    Engine_EventWait(20);
    Event_ShowMessage(14, 0);
    Engine_EventWait(10);
    Call3(Engine_ActorShowEmote, 20, 0x102, 40);
    Event_ShowMessage(20, 0);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(20, 4);
    Engine_EventWait(20);
    Event_ShowMessage(20, 0);
    Engine_EventWait(10);
    Call3(Engine_ActorShowEmote, 0, 0x102, 40);
    Event_Wait(10);
    Engine_ActorSetAnimationAndWait(14, 4);
    Engine_EventWait(20);
    Engine_EventShowMessage(14, 0);
    Engine_EventWait(10);
    Engine_ActorFaceEachOther(1, 0, 50);
    Call3(Engine_ActorFaceDirection, 0, 0xc000, 0);
    Call3(Engine_ActorFaceDirection, 1, 0xc000, 0);
    Engine_EventWait(30);
    Engine_EventWait(10);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
    Engine_ActorSetAnimationAndWait(1, 3);
    Engine_EventWait(30);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(14, 3);
    Engine_EventWait(30);
    Event_ShowMessage(14, 0);
    Event_Wait(10);
    Call3(Engine_ActorShowEmote, 3, 0x100, 40);
    Engine_EventShowMessage(3, 0);
    Event_Wait(10);
    Call3(Engine_ActorShowEmote, 14, 0x102, 50);
    Engine_ActorFaceActor(20, 14, 50);
    Event_Wait(10);
    Engine_ActorRunRepeatedMotion(14, 2);
    Engine_EventWait(20);
    Event_ShowMessage(14, 0);
    Engine_EventWait(20);
    Engine_ActorFaceEachOther(1, 0, 0);
    Engine_ActorFaceEachOther(3, 2, 50);
    Call3(Engine_ActorFaceDirection, 0, 0xc000, 0);
    Call3(Engine_ActorFaceDirection, 1, 0xc000, 0);
    Call3(Engine_ActorFaceDirection, 3, 0xc000, 0);
    Call3(Engine_ActorFaceDirection, 2, 0xc000, 0);
    Engine_EventWait(50);
    Engine_ActorFaceDirection(20, 0x2000, 0);
    Engine_EventWait(20);
    Engine_EventShowMessage(20, 0);
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(1, 2);
    Event_Wait(20);
    Engine_EventShowMessage(1, 0);
    Engine_EventWait(20);
    Engine_ActorFaceDirection(14, 0x8000, 0);
    Engine_EventWait(40);
    Call3(Engine_ActorShowEmote, 14, 0x105, 60);
    Engine_EventShowMessage(14, 0);
    Engine_EventWait(10);
    Call3(Engine_ActorShowEmote, 2, 0x100, 40);
    Engine_EventShowMessage(2, 0);
    Event_Wait(10);
    Engine_ActorFaceEachOther(14, 2, 40);
    Engine_ActorSetAnimationAndWait(14, 3);
    Engine_EventWait(30);
    Engine_EventShowMessage(14, 0);
    Engine_EventWait(10);
    Engine_ActorFaceEachOther(1, 0, 0);
    Engine_ActorFaceEachOther(3, 2, 60);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    Call3(Engine_ActorFaceDirection, 1, 0xc000, 0);
    Call3(Engine_ActorFaceDirection, 3, 0xc000, 0);
    Engine_ActorFaceDirection(2, 0xc000, 0);
    Engine_EventWait(50);
    Engine_ActorRunRepeatedMotion(20, 2);
    Engine_EventWait(20);
    Engine_EventShowMessage(20, 0);
    Engine_EventWait(10);
    Engine_ActorFaceEachOther(1, 0, 40);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 2);
    Engine_EventWait(20);
    Engine_EventOpenMessage(1, 0);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
        Engine_EventWait(20);
        Engine_EventShowMessage(1, 0);
        AdvanceMessage(3);
    } else {
        do {
            Engine_EventWait(10);
        } while (0);
        {
            u8 **scene_address = (u8 **)&gEventWork;

            AdvanceMessageAt(scene_address, 1);
            Engine_EventOpenMessage(1, 0);
            if (Event_ChooseYesNo(0, 0) == 0) {
                Engine_EventWait(20);
                Engine_EventShowMessage(1, 0);
                AdvanceMessageAt(scene_address, 1);
            } else {
                AdvanceMessageAt(scene_address, 1);
                Engine_EventShowMessage(1, 0);
            }
        }
    }

    Event_Wait(10);
    Engine_ActorSetAnimationAndWait(14, 4);
    Engine_EventWait(20);
    Event_ShowMessage(14, 0);
    Engine_EventWait(10);
    Call3(Engine_ActorFaceDirection, 0, 0xc000, 0);
    Call3(Engine_ActorFaceDirection, 1, 0xc000, 0);
    Engine_EventWait(40);
    Engine_ActorRunRepeatedMotion(1, 2);
    Engine_EventWait(20);
    Engine_EventWait(10);
    Actor_SetAnimationAndWait(14, 3);
    Engine_EventWait(30);
    Event_ShowMessage(14, 0);
    Engine_EventWait(10);
    Call3(Engine_ActorShowEmote, 3, 0x101, 40);
    Engine_EventShowMessage(3, 0);
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(14, 2);
    Engine_EventWait(20);
    Engine_EventShowMessage(14, 0);
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(2, 2);
    Engine_EventWait(20);
    Engine_EventShowMessage(2, 0);
    Engine_EventWait(10);
    Actor_FaceActor(20, 14, 40);
    Actor_RunRepeatedMotion(20, 2);
    Engine_EventWait(20);
    Engine_EventShowMessage(20, 0);
    Engine_EventWait(20);
    Call3(Engine_ActorShowEmote, 14, 0x105, 70);
    Engine_EventShowMessage(14, 0);
    Engine_EventWait(10);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 0);
    Call3(Engine_ActorShowEmote, 1, 0x101, 0);
    Actor_ShowEmote(ACTOR_MIA, 0x101, 0);
    Call3(Engine_ActorShowEmote, 2, 0x101, 40);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(14, 4);
    Event_Wait(20);
    Engine_EventShowMessage(14, 0);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(14, 3);
    Engine_EventWait(30);
    Engine_EventShowMessage(14, 0);
    Engine_EventWait(20);
    Actor_SetAnimationAndWait(ACTOR_GERALD, 3);
    Engine_EventWait(30);
    Engine_EventShowMessage(1, 0);
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(14, 3);
    Engine_EventWait(30);
    Engine_EventWait(10);
    Actor_ShowEmote(ACTOR_IVAN, 0x101, 40);
    Engine_EventShowMessage(2, 0);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(14, 3);
    Engine_EventWait(40);
    Engine_ActorFaceActor(0, 2, 0);
    Actor_FaceActor(ACTOR_GERALD, ACTOR_IVAN, 0);
    Engine_ActorFaceActor(3, 2, 0);
    Engine_ActorFaceActor(20, 2, 0);
    Event_Wait(50);
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(2, 2);
    Event_Wait(20);
    Engine_EventShowMessage(2, 0);
    Engine_EventWait(10);
    Actor_ShowEmote(14, 0x101, 40);
    Engine_EventShowMessage(14, 0);
    Engine_EventWait(10);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
    Engine_ActorSetAnimation(1, 3);
    Engine_ActorSetAnimationAndWait(3, 3);
    Engine_EventWait(30);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(2, 4);
    Engine_EventWait(20);
    Engine_EventShowMessage(2, 0);
    Engine_EventWait(20);
    Actor_SetAnimationAndWait(20, 3);
    Engine_EventWait(30);
    Engine_ActorFaceActor(20, 14, 30);
    Call3(Engine_ActorFaceDirection, 0, 0xc000, 0);
    Call3(Engine_ActorFaceDirection, 1, 0xc000, 0);
    Call3(Engine_ActorFaceDirection, 3, 0xc000, 0);
    Engine_EventWait(20);
    Engine_EventShowMessage(20, 0);
    Event_Wait(10);
    Engine_ActorSetAnimationAndWait(14, 3);
    Engine_EventWait(30);
    Engine_EventShowMessage(14, 0);
    Engine_EventWait(10);
    Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Engine_ActorStartRepeatedMotion(1, 2);
    Engine_ActorStartRepeatedMotion(3, 2);
    Engine_ActorRunRepeatedMotion(2, 2);
    Event_Wait(30);
    Event_Wait(10);
    Engine_ActorRunRepeatedMotion(20, 2);
    Engine_EventWait(20);
    Engine_EventShowMessage(20, 0);
    Engine_EventWait(10);
    Call3(Engine_ActorFaceDirection, 14, 0x8000, 0);
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(14, 4);
    Engine_EventWait(20);
    Engine_EventShowMessage(14, 0);
    Engine_EventWait(10);
    Engine_ActorFaceEachOther(1, 0, 0);
    Engine_ActorFaceEachOther(3, 2, 50);
    Call3(Engine_ActorFaceDirection, 0, 0xc000, 0);
    Call3(Engine_ActorFaceDirection, 1, 0xc000, 0);
    Actor_FaceDirection(ACTOR_MIA, 0xc000, 0);
    Call3(Engine_ActorFaceDirection, 2, 0xc000, 0);
    Engine_EventWait(40);
    Call3(Engine_ActorFaceDirection, 14, 0x4000, 0);
    Engine_EventWait(30);
    Call3(Engine_ActorShowEmote, 14, 0x102, 40);
    Engine_EventShowMessage(14, 0);
    Engine_EventWait(10);
    Call3(Engine_ActorShowEmote, 20, 0x102, 40);
    Engine_EventShowMessage(20, 0);
    Engine_EventWait(20);
    Engine_ActorFaceEachOther(14, 20, 40);
    Engine_ActorRunRepeatedMotion(14, 2);
    Engine_EventWait(20);
    Engine_EventShowMessage(14, 0);
    Event_Wait(10);
    Call2(Engine_ActorSetAttachedEffect, 20, 0x102);
    Engine_ActorRunRepeatedMotion(20, 2);
    Engine_EventWait(20);
    Event_ShowMessage(20, 0);
    Actor_SetSpeed(20, 0x19999, 0xcccc);
    Engine_ActorWalkByAndWait(20, 0, 24);
    Engine_ActorFaceActor(0, 20, 0);
    Engine_ActorFaceActor(1, 20, 0);
    Engine_ActorFaceActor(3, 20, 0);
    Engine_ActorFaceActor(2, 20, 0);
    Actor_FaceDirection(14, 0x4000, 0);
    Event_ShowMessage(14, 0);
    Engine_EventWait(10);
    Call3(Engine_ActorShowEmote, 20, 0x100, 60);
    Engine_ActorFaceActor(20, 14, 0);
    Call3(Engine_ActorFaceDirection, 0, 0xc000, 0);
    Call3(Engine_ActorFaceDirection, 1, 0xc000, 0);
    Call3(Engine_ActorFaceDirection, 3, 0xc000, 0);
    Call3(Engine_ActorFaceDirection, 2, 0xc000, 0);
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(20, 2);
    Engine_EventWait(20);
    Engine_EventShowMessage(20, 0);
    Event_Wait(30);
    Actor_ShowEmote(14, 0x105, 40);
    Engine_EventShowMessage(14, 0);
    Engine_EventWait(10);
    Call3(Engine_ActorShowEmote, 20, 0x101, 60);
    Call3(Engine_ActorSetSpeed, 20, 0x13333, 0x9999);
    Call3(Engine_ActorWalkByAndWait, 20, 0, -24);
    Engine_ActorFaceDirection(20, 0, 0);
    Engine_EventWait(20);
    Engine_EventShowMessage(20, 0);
    Engine_EventWait(10);
    Call3(Engine_ActorFaceDirection, 14, 0x8000, 0);
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(14, 3);
    Engine_EventWait(30);
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(20, 2);
    Engine_EventWait(20);
    Engine_EventShowMessage(20, 0);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(14, 4);
    Engine_EventWait(20);
    Engine_EventShowMessage(14, 0);
    Engine_EventWait(10);
    Call3(Engine_ActorShowEmote, 20, 0x102, 40);
    Engine_EventShowMessage(20, 0);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(14, 3);
    Engine_EventWait(30);
    Engine_EventShowMessage(14, 0);
    Event_Wait(10);
    Engine_ActorRunRepeatedMotion(1, 2);
    Engine_EventWait(20);
    Engine_EventShowMessage(1, 0);
    Engine_EventWait(10);
    Call3(Engine_ActorFaceDirection, 14, 0x4000, 0);
    Engine_EventWait(30);
    Engine_ActorSetAnimationAndWait(14, 3);
    Engine_EventWait(30);
    Engine_EventShowMessage(14, 0);
    Engine_EventWait(10);
    Call3(Engine_ActorShowEmote, 3, 0x102, 40);
    Engine_EventShowMessage(3, 0);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(14, 3);
    Engine_EventWait(30);
    Engine_EventShowMessage(14, 0);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(2, 3);
    Engine_EventWait(30);
    Engine_EventShowMessage(2, 0);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(14, 3);
    Engine_EventWait(30);
    Engine_EventShowMessage(14, 0);
    Engine_EventWait(10);
    Engine_ActorFaceEachOther(1, 0, 0);
    Engine_ActorFaceEachOther(3, 2, 50);
    Call3(Engine_ActorFaceDirection, 0, 0xc000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xc000, 0);
    Call3(Engine_ActorFaceDirection, 3, 0xc000, 0);
    Call3(Engine_ActorFaceDirection, 2, 0xc000, 0);
    Engine_EventWait(30);
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(14, 2);
    Engine_EventWait(20);
    Engine_EventShowMessage(14, 0);
    Engine_EventWait(20);
    Call3(Engine_ActorShowEmote, 0, 0x101, 0);
    Call3(Engine_ActorShowEmote, 1, 0x101, 0);
    Call3(Engine_ActorShowEmote, 3, 0x101, 0);
    Actor_ShowEmote(ACTOR_IVAN, 0x101, 50);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(14, 3);
    Engine_EventWait(30);
    Event_ShowMessage(14, 0);
    Engine_EventWait(10);
    Call3(Engine_ActorShowEmote, 20, 0x102, 40);
    Engine_EventShowMessage(20, 0);
    Engine_EventWait(10);
    Engine_ActorFaceEachOther(14, 20, 30);
    Engine_EventShowMessage(14, 0);
    Engine_EventWait(10);
    Actor_SetAnimationAndWait(14, 4);
    Engine_EventWait(20);
    Engine_EventShowMessage(14, 0);
    Event_Wait(10);
    Engine_ActorRunRepeatedMotion(1, 2);
    Engine_EventWait(20);
    Engine_EventShowMessage(1, 0);
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(14, 2);
    Engine_EventWait(20);
    Call3(Engine_ActorFaceDirection, 14, 0x4000, 0);
    Engine_EventWait(20);
    Engine_EventShowMessage(14, 0);
    Engine_EventWait(10);
    Call3(Engine_ActorShowEmote, 20, 0x101, 40);
    Engine_EventShowMessage(20, 0);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(14, 3);
    Event_Wait(30);
    Actor_FaceDirection(14, 0x8000, 0);
    Engine_EventWait(20);
    Engine_EventShowMessage(14, 0);
    Engine_EventWait(10);
    Call3(Engine_ActorShowEmote, 2, 0x101, 40);
    Engine_EventShowMessage(2, 0);
    Engine_EventWait(10);
    Call3(Engine_ActorFaceDirection, 14, 0x4000, 0);
    Engine_ActorFaceDirection(20, 0x2000, 0);
    Engine_EventWait(30);
    Engine_ActorSetAnimationAndWait(14, 3);
    Engine_EventWait(30);
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(3, 2);
    Engine_EventWait(20);
    Event_ShowMessage(ACTOR_MIA, 0);
    Engine_EventWait(10);
    Call3(Engine_ActorShowEmote, 14, 0x102, 40);
    Engine_EventShowMessage(14, 0);
    Engine_EventWait(20);
    Engine_ActorSetAnimation(0, 3);
    Engine_ActorSetAnimation(1, 3);
    Engine_ActorSetAnimation(3, 3);
    Actor_SetAnimationAndWait(ACTOR_IVAN, 3);
    Engine_EventWait(30);
    Engine_EventWait(20);
    Call3(Engine_ActorShowEmote, 14, 0x105, 60);
    Engine_EventShowMessage(14, 0);
    Event_Wait(10);
    Call3(Engine_ActorShowEmote, 1, 0x101, 40);
    Engine_EventShowMessage(1, 0);
    Engine_EventWait(10);
    Call3(Engine_ActorShowEmote, 14, 0x100, 40);
    Engine_EventShowMessage(14, 0);
    Engine_EventWait(10);
    Engine_ActorFaceEachOther(1, 0, 50);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    Engine_ActorFaceDirection(1, 0xc000, 0);
    Engine_EventWait(30);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(14, 3);
    Engine_EventWait(30);
    Engine_EventShowMessage(14, 0);
    Engine_EventWait(10);
    Call3(Engine_ActorShowEmote, 3, 0x101, 40);
    Event_ShowMessage(ACTOR_MIA, 0);
    Engine_EventWait(10);
    Actor_SetAnimationAndWait(14, 4);
    Engine_EventWait(20);
    Event_ShowMessage(14, 0);
    Engine_EventWait(10);
    Call3(Engine_ActorShowEmote, 0, 0x101, 0);
    Actor_ShowEmote(ACTOR_GERALD, 0x101, 0);
    Call3(Engine_ActorShowEmote, 2, 0x101, 0);
    Engine_ActorShowEmote(3, 0x101, 40);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(14, 4);
    Engine_EventWait(20);
    Engine_EventShowMessage(14, 0);
    Engine_EventWait(10);
    Call3(Engine_ActorShowEmote, 2, 0x100, 40);
    Engine_EventOpenMessage(2, 0);
    if (Engine_EventChooseYesNo(14, 0) == 0) {
        Event_Wait(10);
        Engine_EventShowMessage(2, 0);
        AdvanceMessage(1);
    } else {
        Engine_EventWait(10);
        AdvanceMessage(1);
        Engine_EventShowMessage(2, 0);
    }

    Event_Wait(10);
    Engine_ActorRunRepeatedMotion(14, 2);
    Engine_EventWait(20);
    Event_ShowMessage(14, 0);
    Event_Wait(10);
    Engine_ActorSetAnimationAndWait(14, 4);
    Engine_EventWait(20);
    Engine_EventShowMessage(14, 0);
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(20, 2);
    Event_Wait(20);
    Engine_ActorFaceDirection(20, 0, 0);
    Engine_EventWait(20);
    Engine_EventShowMessage(20, 0);
    Engine_EventWait(10);
    Engine_ActorFaceActor(14, 20, 40);
    Engine_ActorSetAnimationAndWait(14, 4);
    Engine_EventWait(30);
    Engine_EventShowMessage(14, 0);
    Engine_EventWait(10);
    Engine_ActorFaceEachOther(1, 0, 40);
    Engine_EventShowMessage(1, 0);
    Actor_FaceDirection(14, 0x4000, 0);
    Call3(Engine_ActorFaceDirection, 20, 0x2000, 0);
    Engine_EventWait(30);
    Engine_EventShowMessage(14, 0);
    Engine_EventWait(10);
    Call3(Engine_ActorFaceDirection, 0, 0xc000, 0);
    Call3(Engine_ActorFaceDirection, 1, 0xc000, 0);
    Event_Wait(30);
    Engine_ActorFaceEachOther(3, 2, 60);
    Call3(Engine_ActorFaceDirection, 3, 0xc000, 0);
    Engine_ActorFaceDirection(2, 0xc000, 0);
    Engine_EventWait(30);
    Engine_EventShowMessage(3, 0);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(2, 4);
    Engine_EventWait(20);
    Engine_EventShowMessage(2, 0);
    Engine_EventWait(10);
    Engine_ActorFaceEachOther(20, 14, 60);
    Call3(Engine_ActorFaceDirection, 20, 0x2000, 0);
    Call3(Engine_ActorFaceDirection, 14, 0x4000, 0);
    Engine_EventWait(30);
    Engine_ActorFaceDirection(14, 0x4000, 0);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(14, 3);
    Event_Wait(30);
    Engine_EventShowMessage(14, 0);
    Engine_EventWait(10);
    Actor_RunRepeatedMotion(20, 2);
    Engine_EventWait(20);
    Engine_EventOpenMessage(20, 0);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
        Engine_EventWait(20);
        Call3(Engine_ActorShowEmote, 1, 0x102, 40);
        Engine_EventShowMessage(1, 0);
        AdvanceMessage(1);
    } else {
        Engine_EventWait(10);
        AdvanceMessage(1);
        Engine_EventShowMessage(1, 0);
    }

    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(20, 2);
    Engine_EventWait(20);
    Event_ShowMessage(20, 0);
    Engine_EventWait(10);
    Call3(Engine_ActorShowEmote, 20, 0x100, 40);
    Engine_EventShowMessage(20, 0);
    Event_Wait(20);
    Engine_ActorSetAnimation(0, 3);
    Engine_ActorSetAnimation(1, 3);
    Engine_ActorSetAnimation(3, 3);
    Actor_SetAnimationAndWait(ACTOR_IVAN, 3);
    Engine_EventWait(30);
    Engine_EventWait(10);
    Call3(Engine_ActorShowEmote, 14, 0x100, 40);
    Engine_EventShowMessage(14, 0);
    Engine_EventWait(10);
    Engine_ActorFaceActor(20, 14, 40);
    Engine_EventWait(10);
    Call3(Engine_ActorShowEmote, 20, 0x102, 40);
    Engine_EventShowMessage(20, 0);
    Engine_EventWait(10);
    Engine_ActorFaceActor(14, 20, 40);
    Engine_ActorSetAnimationAndWait(14, 3);
    Engine_EventWait(30);
    Engine_EventShowMessage(14, 0);
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(20, 2);
    Engine_EventWait(20);
    Engine_EventShowMessage(20, 0);
    Engine_ActorFaceDirection(14, 0x4000, 0);
    Engine_EventWait(20);
    Engine_ActorRunRepeatedMotion(14, 2);
    Engine_EventWait(20);
    Engine_EventShowMessage(14, 0);
    Engine_EventWait(10);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 80);
    Call3(Engine_ActorShowEmote, 20, 0x102, 70);
    Engine_ActorSetAnimationAndWait(14, 4);
    Engine_EventWait(20);
    Engine_EventShowMessage(14, 0);
    Engine_EventWait(10);
    Engine_ActorFaceEachOther(1, 0, 0);
    Engine_ActorFaceEachOther(3, 2, 50);
    Call3(Engine_ActorFaceDirection, 0, 0xc000, 0);
    Call3(Engine_ActorFaceDirection, 1, 0xc000, 0);
    Call3(Engine_ActorFaceDirection, 3, 0xc000, 0);
    Engine_ActorFaceDirection(2, 0xc000, 0);
    Engine_EventWait(30);
    Engine_EventWait(10);
    Call3(Engine_ActorShowEmote, 20, 0x106, 50);
    Engine_EventShowMessage(20, 0);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(14, 3);
    Engine_EventWait(30);
    Actor_FaceDirection(20, 0x2000, 0);
    Event_ShowMessage(20, 0);
    Engine_EventWait(10);
    Call3(Engine_ActorShowEmote, 14, 0x108, 40);
    Engine_EventOpenMessage(14, 0);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
        Event_Wait(20);
        Call3(Engine_ActorShowEmote, 14, 0x100, 40);
        Event_ShowMessage(14, 0);
        AdvanceMessage(1);
    } else {
        Engine_EventWait(10);
        Call3(Engine_ActorShowEmote, 14, 0x100, 40);
        AdvanceMessage(1);
        Event_ShowMessage(14, 0);
    }

    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(20, 2);
    Engine_EventWait(20);
    Event_ShowMessage(20, 0);
    Engine_EventWait(10);
    Engine_ActorFaceActor(14, 20, 40);
    Engine_ActorSetAnimationAndWait(14, 3);
    Engine_EventWait(30);
    Engine_EventShowMessage(20, 0);
    Engine_EventWait(10);
    Engine_ActorFaceActor(20, 14, 40);
    Engine_EventShowMessage(20, 0);
    Engine_EventWait(10);
    Call3(Engine_ActorShowEmote, 14, 0x100, 40);
    Call3(Engine_ActorFaceDirection, 14, 0x8000, 0);
    Engine_EventWait(20);
    Event_ShowMessage(14, 0);
    Event_Wait(10);
    Call3(Engine_ActorShowEmote, 20, 0x100, 40);
    Engine_EventShowMessage(20, 0);
    Engine_EventWait(10);
    Actor_RunRepeatedMotion(14, 2);
    Engine_EventWait(20);
    Engine_EventShowMessage(14, 0);
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(20, 2);
    Event_Wait(20);
    Engine_EventShowMessage(20, 0);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(20, 3);
    Event_Wait(30);
    Engine_ActorSetAnimationAndWait(14, 3);
    Engine_EventWait(30);
    Engine_ActorFaceDirection(20, 0x2000, 0);
    Engine_EventWait(20);
    Object_LinkObjectAndSetCallbackFar(0, 20);
    Object_LinkObjectAndSetCallbackFar(1, 20);
    Object_LinkObjectAndSetCallbackFar(3, 20);
    Object_LinkObjectAndSetCallbackFar(2, 20);
    Call3(Engine_ActorSetSpeed, 20, 0x10000, 0x8000);
    Engine_ActorWalkByAndWait(20, 0, 32);
    Engine_ActorFaceDirection(20, 0, 0);
    Call3(Engine_ActorFaceDirection, 14, 0x4000, 0);
    Engine_EventWait(20);
    Event_ShowMessage(20, 0);
    Engine_EventWait(10);
    Actor_SetAnimationAndWait(20, 3);
    Engine_EventWait(30);
    Engine_ActorWalkByAndWait(1, 16, 0);
    Engine_ActorWalkBy(20, 0, 80);
    Engine_EventWait(40);
    Actor_WalkByAndWait(ACTOR_GERALD, -16, 0);
    Call3(Engine_ActorFaceDirection, 1, 0x4000, 0);
    Call3(Engine_ActorFaceDirection, 3, 0x4000, 0);
    Actor_WaitForMove(20);
    Engine_EventWait(80);
    Engine_ActorSetPosition(20, 0, 0);
    Call3(Engine_ActorFaceDirection, 0, 0xc000, 0);
    Call3(Engine_ActorFaceDirection, 1, 0xc000, 0);
    Call3(Engine_ActorFaceDirection, 3, 0xc000, 0);
    Engine_ActorFaceDirection(2, 0xc000, 0);
    Event_Wait(30);
    Object_LinkObjectAndSetCallbackFar(0, 14);
    Object_LinkObjectAndSetCallbackFar(1, 14);
    Object_LinkObjectAndSetCallbackFar(3, 14);
    Object_LinkObjectAndSetCallbackFar(2, 14);
    Engine_EventWait(30);
    Engine_ActorSetAnimationAndWait(14, 3);
    Engine_EventWait(30);
    Actor_SetSpeed(14, 0xcccc, 0x6666);
    Engine_ActorWalkByAndWait(14, 0, 24);
    Engine_ActorWalkByAndWait(14, -80, 0);
    Engine_EventWait(10);
    Engine_ActorFaceDirection(14, 0, 0);
    Engine_EventWait(20);
    Event_ShowMessage(14, 0);
    Engine_EventWait(20);
    Actor_FaceDirection(14, 0x4000, 0);
    Engine_EventWait(20);
    Actor_WalkByAndWait(14, 0, 48);
    Engine_ActorWalkByAndWait(14, -64, 0);
    Engine_ActorStop(0);
    Actor_Stop(ACTOR_GERALD);
    Engine_ActorStop(3);
    Engine_ActorStop(2);
    Actor_SetPosition(14, 0, 0);
    Engine_EventWait(20);
    Engine_ActorFaceEachOther(0, 3, 0);
    Engine_ActorFaceEachOther(1, 2, 0);
    Engine_EventWait(30);
    Engine_ActorSetAnimation(0, 3);
    Engine_ActorSetAnimation(1, 3);
    Engine_ActorSetAnimation(3, 3);
    Actor_SetAnimationAndWait(ACTOR_IVAN, 3);
    Engine_EventWait(30);
    Engine_AudioPlayCue(17);
    Call3(Engine_ActorSetSpeed, 1, 0x13333, 0x9999);
    Call3(Engine_ActorSetSpeed, 2, 0x13333, 0x9999);
    Actor_SetSpeed(ACTOR_MIA, 0x13333, 0x9999);
    Engine_ActorSetAnimation(1, 2);
    position = Object_GetById(0);
    if (position != 0)
        ObjectMotion_ResetAndSetPositionFar(1, position[5], position[9]);
    Engine_ActorWaitForMove(1);
    Engine_ActorSetPosition(1, 0, 0);
    Actor_SetAnimation(ACTOR_IVAN, 2);
    position = Actor_Get(ACTOR_PARTY_LEADER);
    if (position != 0)
        ObjectMotion_ResetAndSetPositionFar(2, position[5], position[9]);
    Engine_ActorWaitForMove(2);
    Engine_ActorSetPosition(2, 0, 0);
    Engine_ActorSetAnimation(3, 2);
    position = Object_GetById(0);
    if (position != 0)
        ObjectMotion_ResetAndSetPositionFar(3, position[5], position[9]);
    Engine_ActorWaitForMove(3);
    Engine_ActorSetPosition(3, 0, 0);
    Call1(Engine_EventWait, 10);
    Audio_PlayCueFromEventWorkFar();
    Engine_EventEnd();
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
        Map_ClearLayerEntryFlagFar(1);
        Event_Wait(10);
        Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, -16);
    } else {
        if (work->touched_trigger == 30) {
            Map_ClearLayerEntryFlagFar(4);
            Event_Wait(10);
            Actor_CenterAndWalk(ACTOR_PARTY_LEADER, 3, -16);
        } else {
            Map_ClearLayerEntryFlagFar(2);
            Event_Wait(10);
            Actor_CenterAndWalk(ACTOR_PARTY_LEADER, 3, -16);
        }
    }
    Event_Wait(16);
    Event_RequestExit(work->touched_trigger);
    Map_SetLayerEntryFlagFar(1);
    Map_SetLayerEntryFlagFar(2);
    Map_SetLayerEntryFlagFar(4);
    Event_End();
}

void RunSceneEffectSetup(void)
{
    Event_Begin();
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 32768, 16384);
    Audio_PlayCue(158);
    Map_AnimateCells(TorebiKyuden_CellSteps, 36, 10);
    SetSceneEffectOffset(0, 2, -16);
    Event_Wait(16);
    Event_RequestExit(2);
    Event_End();
}

/* Actor 8's question once flag 2412 is set: the leader walks over, and a
 * no brings actors 8 and 9 into a longer exchange. */
void RunSupplementalSequenceOne(void)
{
    s32 p;
    Engine_GameFlagSet(2412);
    Engine_EventBegin();
    Battle_ResetEffectCounterFar();
    Call3(Engine_ActorFaceDirection, 8, 20480, 0);
    Call3(Engine_ActorFaceDirection, 9, 12288, 0);
    Call3(Engine_ActorWalkToAndWait, 0, 200, 272);
    Engine_ActorFaceDirection(0, 49152, 0);
    Engine_EventWait(20);
    p = (s32)MsgTorebiArent;
    Engine_EventSetMessage(p);
    Engine_EventOpenMessage(8, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        Engine_EventWait(20);
        Engine_EventSetMessage(p + 1);
        Engine_EventShowMessage(8, 0);
    } else {
        Engine_EventWait(20);
        Event_SetMessage(p + 2);
        Engine_EventShowMessage(8, 0);
        Engine_EventWait(20);
        Engine_ActorFaceEachOther(8, 9, 60);
        Call3(Engine_ActorFaceDirection, 9, 12288, 0);
        Engine_EventWait(40);
        Actor_RunRepeatedMotion(9, 2);
        Engine_EventWait(30);
        Engine_ActorFaceEachOther(8, 9, 30);
        Actor_SetAnimationAndWait(9, 3);
        Engine_EventWait(30);
        Actor_ShowEmote(8, 258, 50);
        Call3(Engine_ActorFaceDirection, 8, 20480, 0);
        Engine_ActorFaceDirection(9, 12288, 0);
        Engine_EventWait(20);
        Engine_ActorSetAnimationAndWait(8, 4);
        Engine_EventWait(20);
        Engine_EventShowMessage(8, 0);
        Engine_EventWait(10);
        Engine_ActorRunRepeatedMotion(8, 2);
        Engine_EventWait(20);
        Engine_EventShowMessage(8, 0);
    }
    Engine_EventEnd();
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

    Map_SetLayerEntryFlagFar(1);
    Map_SetLayerEntryFlagFar(2);
    Map_SetLayerEntryFlagFar(4);
    if (gGameState.entrance == 90) {
        Engine_GameFlagSet(0x962);
    }
    if (gGameState.entrance == 91) {
        Engine_GameFlagSet(0x962);
        Engine_GameFlagSet(0x950);
    }
    if (gGameState.scene != (s32)&SceneId_TorebiKyuden2) {
        if (gGameState.entrance == 11) {
            Engine_GameFlagClear(0x12f);
        }
        if (Engine_GameFlagIsSet(0x950)) {
            set = Engine_GameFlagIsSet(0xf31);
            if (set) {
                Engine_ActorSetPosition(16, 0, 0);
            } else {
                actor = Object_GetById(16);
                actor->unknown_5c = 1;
                actor->motion_flags = set;
                sprite = actor->sprite;
                actor->y.fixed = 0x40000;
                sprite->part_count = set;
                sprite->full_color = 0;
                sprite->palette = 0;
                buf = Engine_HeapAllocate(17, 0x608);
                Engine_ItemLoadIcon(205);
                Engine_VramLoad(sprite->vram_block, 128, buf + 0x400);
                Engine_HeapRelease(17);
            }
            if (gGameState.entrance == 33 && !Value1(Engine_GameFlagIsSet, 0x96f)) {
                Engine_GameFlagSet(0x96f);
                Call3(Engine_ActorSetPosition, 14, 0xd00000, 0x2c00000);
                FieldScene_RunMainCutsceneSequence();
            }
            Engine_ActorSetAnimation(14, 5);
            Engine_ActorSetSpriteFlags(Object_GetById(14), 0);
        } else if (Engine_GameFlagIsSet(0x962) && !Engine_GameFlagIsSet(0x966)) {
            Call3(Engine_ActorSetPosition, 10, 0x780000, 0x480000);
        }
        gEventWork->start_transition = 0x209;
        Object_GetById(9)->collision_flags |= 4;
        if (gGameState.entrance == 99) {
            Party_RestoreAll();
            FieldScene_RunBranchingActorSequence();
            gGameState.entrance = 8;
        }
        if (gGameState.entrance == 98) {
            Party_RestoreAll();
            Engine_GameFlagSet(0x966);
            Engine_GameFlagSet(0x967);
            Call3(Engine_ActorSetPosition, 10, 0x380000, 0x780000);
            Engine_ActorFaceDirection(10, 0xf000, 0);
            FieldScene_RunScene3b8SequenceB();
            gGameState.entrance = 8;
        }
    }
    return 0;
}
