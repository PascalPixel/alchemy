#include "SUHARA.H"
#include "CALL.H"

extern u8 MsgSuharaArentSurprisedFind[];
extern u8 MsgSuharaBusyEverSince[];
extern u8 MsgSuharaGetSickThinkingAboutLalivero[];
extern u8 MsgSuharaOursOnlyStore[];
extern u8 MsgSuharaSighNothinMaybe[];
extern u8 MsgSuharaWonderWhySandstorms[];

extern u8 MsgSuharaSupposeFolkBlown[];

extern u8 MsgSuharaIodem[];
void Motion_LaunchFromFocusedObject();
void Audio_PlayCueFromEventWork(void);

/* The room's scene tables, which the main image asks for through the
 * overlay's entry veneers. */
u8 *SuharaHeya_GetEntrances(void)
{
    return gSuharaHeyaEntrances;
}

u8 *SuharaHeya_GetRegions(void)
{
    return gSuharaHeyaRegions;
}

u8 *SuharaHeya_GetExits(void)
{
    return gSuharaHeyaExits;
}

/* Flag 0x96f selects the later placements. */
s32 SuharaHeya_SelectPlacements(void)
{
    if (Engine_GameFlagIsSet(0x96f) != 0) {
        return (s32)gSuharaHeyaPlacements96f;
    }
    return (s32)gSuharaHeyaPlacements;
}

/* The house's counters: a shop or the inn when the leader faces them,
 * otherwise a line chosen by flag 0x96f. */
void Dialogue_HandleFacingChoice(s32 no)
{
    u16 facing = (Actor_Get(ACTOR_PARTY_LEADER)->facing + 0x2000) & ~0x3fff;
    if (facing == 0xc000) {
        Engine_ShopOpen(31, no);
    } else if (Engine_GameFlagIsSet(0x96f)) {
        s32 msg = (s32)MsgSuharaArentSurprisedFind;
        Event_SetMessage(msg);
        Event_OpenMessage(no, 0);
        if (Event_ChooseYesNo(ACTOR_PARTY_LEADER, 0) == 0) {
            Event_Wait(10);
            Event_SetMessage(msg + 1);
        } else {
            Event_SetMessage(msg + 2);
        }
        Event_ShowMessage(no, 0);
    } else {
        Event_SetMessage((s32)MsgSuharaOursOnlyStore);
        Event_ShowMessage(no, 0);
    }
}

void Dialogue_HandleFacingBranch(s32 no)
{
    u16 facing = (((u16 *)Object_GetById(ACTOR_PARTY_LEADER))[3] + 0x2000) & ~0x3fff;
    if (facing == 0xc000) {
        Inn_CheckIn(10, no);
    } else if (Engine_GameFlagIsSet(0x96f)) {
        Engine_EventSetMessage((s32)MsgSuharaBusyEverSince);
        Engine_EventShowMessage(no, 0);
    } else {
        Engine_EventSetMessage((s32)MsgSuharaSighNothinMaybe);
        Engine_EventShowMessage(no, 0);
    }
}

void Dialogue_HandleFacingAction(s32 no)
{
    u16 facing = (((u16 *)Object_GetById(ACTOR_PARTY_LEADER))[3] + 0x2000) & ~0x3fff;
    if (facing == 0xc000) {
        Shop_ConfirmAct(no);
    } else if (Engine_GameFlagIsSet(0x96f)) {
        Engine_EventSetMessage((s32)MsgSuharaWonderWhySandstorms);
        Engine_EventShowMessage(no, 0);
    } else {
        Event_SetMessage((s32)MsgSuharaGetSickThinkingAboutLalivero);
        Engine_EventShowMessage(no, 0);
    }
}

/* Asks whether the party was blown here on the way to Babi Lighthouse, with a
 * line for each answer. */
void SuharaHeya_AskBlownHere(s32 obj)
{
    s32 msg = (s32)MsgSuharaSupposeFolkBlown;
    Event_SetMessage(msg);
    Event_OpenMessage(obj, 0);
    if (Event_ChooseYesNo(ACTOR_PARTY_LEADER, 0) == 0) {
        Event_Wait(10);
        Event_SetMessage(msg + 1);
    } else {
        Event_SetMessage(msg + 2);
    }
    Event_ShowMessage(obj, 0);
}

void State_ApplyCounter16cThenCall7b(void)
{
    u8 *state = (u8 *)gEventWork;
    s16 *cnt = (s16 *)(state + 0x16C);

    Engine_EventRequestExit(*cnt);
    Engine_AudioPlayCue(0x7B);
}

/*
 * Runs the primary script for this scene: a long fixed sequence driving
 * actors 10, 19, 20, 21, 30 and 40 through position, pose and timing steps,
 * guarded by an initial skip check.
 */
void Scene_RunPrimaryScript(void)
{
    u8 *record;
    Engine_GameFlagSet(2480);
    if (Engine_GameFlagIsSet(2442) == 0) {
        Engine_AudioPlayCue(30);
        Engine_EventBegin();
        Engine_CameraMoveTo(24117248, -1, 6815744, 1);
        Call3(Engine_ActorWalkToAndWait, 0, 368, 160);
        Call3(Engine_ActorFaceDirection, 0, 49152, 0);
        Call4(Motion_LaunchFromFocusedObject, 19, 0, -16, 49152);
        Engine_ActorWaitForMove(19);
        Engine_CameraWaitForMove();
        Engine_EventSetMessage((s32)MsgSuharaIodem);
        Engine_EventWait(10);
        Engine_ActorRunRepeatedMotion(20, 2);
        Engine_EventWait(20);
        Engine_EventShowMessage(20, 0);
        Engine_EventWait(10);
        Engine_ActorRunRepeatedMotion(19, 2);
        Engine_EventWait(20);
        Call3(Engine_ActorSetSpeed, 19, 78643, 39321);
        Call3(Engine_ActorWalkByAndWait, 19, 0, -16);
        Engine_ActorFaceDirection(19, 0, 0);
        Engine_EventWait(30);
        Engine_ActorFaceDirection(19, 57344, 0);
        Engine_EventWait(30);
        Engine_ActorFaceDirection(19, 0, 0);
        Engine_EventWait(30);
        Call3(Engine_ActorShowEmote, 19, 256, 40);
        Engine_EventShowMessage(19, 0);
        Engine_EventWait(10);
        Engine_ActorRunRepeatedMotion(21, 2);
        Engine_EventWait(20);
        Engine_EventShowMessage(21, 0);
        Engine_EventWait(10);
        Call3(Engine_ActorSetSpeed, 19, 78643, 39321);
        Call3(Engine_ActorWalkByAndWait, 19, 0, -24);
        Engine_ActorWalkByAndWait(19, 48, 0);
        Call3(Engine_ActorFaceDirection, 19, 49152, 0);
        Engine_EventWait(30);
        Engine_EventShowMessage(19, 0);
        Engine_EventWait(20);
        Call3(Engine_ActorShowEmote, 19, 256, 40);
        Engine_EventShowMessage(19, 0);
        Engine_EventWait(20);
        Call3(Engine_ActorShowEmote, 20, 258, 40);
        Engine_EventShowMessage(20, 0);
        Engine_EventWait(10);
        Call3(Engine_ActorFaceDirection, 19, 8192, 0);
        Engine_EventWait(20);
        Engine_EventShowMessage(19, 0);
        Engine_EventWait(10);
        Engine_ActorRunRepeatedMotion(21, 2);
        Engine_EventWait(20);
        Engine_ActorSetAnimationAndWait(21, 4);
        Engine_EventWait(20);
        Engine_EventShowMessage(21, 0);
        Engine_EventWait(10);
        Call3(Engine_ActorFaceDirection, 19, 49152, 0);
        Engine_EventWait(30);
        Call3(Engine_ActorShowEmote, 19, 263, 40);
        Engine_EventShowMessage(19, 0);
        Engine_EventWait(10);
        Engine_ActorStartRepeatedMotion(20, 2);
        Engine_ActorRunRepeatedMotion(21, 2);
        Engine_EventWait(20);
        Call3(Engine_ActorSetSpeed, 0, 78643, 39321);
        Call3(Engine_ActorWalkToAndWait, 0, 368, 104);
        Engine_ActorWalkByAndWait(0, 16, 0);
        Engine_ActorFaceDirection(0, 0, 0);
        Engine_EventWait(20);
        Engine_EventWait(10);
        Engine_ActorSetAnimationAndWait(19, 4);
        Engine_EventWait(20);
        Engine_EventShowMessage(19, 0);
        Engine_EventWait(20);
        Call3(Engine_ActorShowEmote, 19, 258, 50);
        Engine_EventShowMessage(19, 0);
        Engine_EventWait(10);
        Engine_ActorRunRepeatedMotion(20, 2);
        Engine_EventWait(20);
        Engine_EventShowMessage(20, 0);
        Engine_EventWait(10);
        Engine_ActorFaceActor(0, 20, 0);
        Call3(Engine_ActorFaceDirection, 19, 8192, 0);
        Engine_EventWait(20);
        Engine_EventShowMessage(19, 0);
        Engine_EventWait(10);
        Engine_ActorRunRepeatedMotion(21, 2);
        Engine_EventWait(20);
        Engine_EventShowMessage(21, 0);
        Engine_EventWait(10);
        Engine_ActorFaceActor(0, 21, 0);
        Call3(Engine_ActorFaceDirection, 19, 49152, 0);
        Engine_EventWait(40);
        Engine_ActorSetAnimationAndWait(19, 3);
        Engine_EventWait(30);
        Engine_EventShowMessage(19, 0);
        Engine_EventWait(10);
        Engine_ActorRunRepeatedMotion(21, 2);
        Engine_EventWait(20);
        Engine_ActorSetAnimationAndWait(21, 4);
        Engine_EventWait(20);
        Engine_EventShowMessage(21, 0);
        Engine_EventWait(10);
        Engine_ActorFaceEachOther(19, 0, 30);
        Engine_EventShowMessage(19, 0);
        Engine_EventWait(10);
        Engine_ActorRunRepeatedMotion(20, 2);
        Engine_EventWait(20);
        Engine_EventShowMessage(20, 0);
        Engine_EventWait(10);
        Engine_ActorFaceActor(0, 20, 0);
        Call3(Engine_ActorFaceDirection, 19, 8192, 0);
        Engine_EventWait(40);
        Engine_EventWait(10);
        Engine_ActorRunRepeatedMotion(21, 2);
        Engine_EventWait(20);
        Engine_EventShowMessage(21, 0);
        Engine_EventWait(10);
        Engine_ActorFaceActor(0, 21, 0);
        Call3(Engine_ActorFaceDirection, 19, 49152, 0);
        Engine_EventWait(40);
        Engine_ActorSetAnimation(0, 3);
        Engine_ActorSetAnimationAndWait(19, 3);
        Engine_EventWait(30);
        Engine_EventWait(10);
        Engine_ActorSetAnimationAndWait(20, 4);
        Engine_EventWait(20);
        Engine_EventShowMessage(20, 0);
        Engine_EventWait(10);
        Call3(Engine_ActorShowEmote, 0, 256, 0);
        Call3(Engine_ActorShowEmote, 19, 256, 40);
        Engine_EventWait(10);
        Engine_ActorFaceActor(0, 20, 0);
        Call3(Engine_ActorFaceDirection, 19, 8192, 0);
        Engine_EventWait(40);
        Engine_EventWait(10);
        Engine_ActorRunRepeatedMotion(21, 2);
        Engine_EventWait(20);
        Engine_EventShowMessage(21, 0);
        Engine_EventWait(10);
        Call3(Engine_ActorShowEmote, 0, 256, 50);
        Engine_ActorFaceActor(0, 21, 0);
        Engine_EventWait(30);
        Engine_ActorRunRepeatedMotion(21, 2);
        Engine_EventWait(30);
        Engine_EventWait(10);
        Call3(Engine_ActorFaceDirection, 19, 49152, 0);
        Engine_EventWait(30);
        Engine_EventShowMessage(19, 0);
        Engine_EventWait(10);
        Engine_ActorRunRepeatedMotion(21, 2);
        Engine_EventWait(20);
        Engine_ActorSetAnimationAndWait(21, 3);
        Engine_EventWait(30);
        Engine_EventWait(10);
        Call3(Engine_ActorShowEmote, 19, 258, 40);
        Engine_EventShowMessage(19, 0);
        Engine_EventWait(10);
        Engine_ActorSetAnimationAndWait(20, 4);
        Engine_EventWait(20);
        Engine_EventShowMessage(20, 0);
        Engine_EventWait(10);
        Engine_ActorShowEmote(19, 257, 50);
        Engine_EventWait(10);
        Engine_ActorFaceActor(0, 20, 0);
        Call3(Engine_ActorFaceDirection, 19, 8192, 0);
        Engine_EventWait(20);
        Engine_EventWait(10);
        Engine_ActorRunRepeatedMotion(20, 2);
        Engine_EventWait(20);
        Engine_EventShowMessage(20, 0);
        Engine_EventWait(10);
        Call3(Engine_ActorShowEmote, 19, 256, 40);
        Engine_EventShowMessage(19, 0);
        Engine_EventWait(10);
        Engine_ActorRunRepeatedMotion(21, 2);
        Engine_EventWait(20);
        Engine_EventShowMessage(21, 0);
        Engine_ActorRunRepeatedMotion(21, 2);
        Engine_EventWait(20);
        Engine_EventWait(10);
        Engine_ActorFaceEachOther(19, 0, 0);
        Engine_EventWait(30);
        Engine_EventShowMessage(19, 0);
        Engine_EventWait(10);
        Engine_ActorSetAnimationAndWait(20, 4);
        Engine_EventWait(20);
        Engine_EventShowMessage(20, 0);
        Engine_EventWait(10);
        Call3(Engine_ActorShowEmote, 0, 258, 0);
        Call3(Engine_ActorShowEmote, 19, 258, 80);
        Call3(Engine_ActorShowEmote, 21, 258, 50);
        Engine_EventShowMessage(21, 0);
        Engine_EventWait(10);
        Engine_ActorRunRepeatedMotion(21, 2);
        Engine_EventWait(20);
        Engine_EventShowMessage(21, 0);
        Engine_ActorRunRepeatedMotion(21, 3);
        Engine_EventWait(20);
        Engine_EventWait(10);
        Engine_ActorFaceDirection(19, 49152, 0);
        Engine_EventWait(30);
        Engine_ActorRunRepeatedMotion(19, 2);
        Engine_EventWait(10);
        Engine_EventShowMessage(19, 0);
        Engine_EventWait(10);
        Engine_ActorRunRepeatedMotion(20, 2);
        Engine_EventWait(20);
        Engine_EventShowMessage(20, 0);
        Engine_EventWait(10);
        Engine_ActorFaceActor(0, 20, 0);
        Engine_ActorFaceDirection(19, 8192, 0);
        Engine_EventWait(30);
        Engine_ActorSetAnimationAndWait(19, 3);
        Engine_EventWait(30);
        Engine_EventShowMessage(19, 0);
        Engine_EventWait(10);
        Engine_ActorStartRepeatedMotion(20, 2);
        Engine_ActorRunRepeatedMotion(21, 2);
        Engine_EventWait(30);
        Engine_EventWait(10);
        Engine_ActorFaceEachOther(19, 0, 20);
        Engine_ActorWalkByAndWait(19, -12, 0);
        Engine_EventWait(20);
        Engine_EventOpenMessage(19, 0);
        if (Engine_EventChooseYesNo(0, 0) == 0) {
            Engine_EventWait(20);
            Engine_EventShowMessage(19, 0);
            gEventWork->message++;
        } else {
            Engine_EventWait(10);
            gEventWork->message++;
            Engine_EventShowMessage(19, 0);
        }
        Engine_EventWait(10);
        Engine_ActorRunRepeatedMotion(19, 2);
        Engine_EventWait(20);
        Engine_EventShowMessage(19, 0);
        Engine_EventWait(10);
        Call3(Engine_ActorShowEmote, 19, 258, 50);
        Engine_EventShowMessage(19, 0);
        Engine_EventWait(10);
        Engine_ActorSetAnimationAndWait(0, 3);
        Engine_EventWait(20);
        Engine_ActorSetAnimationAndWait(19, 3);
        Engine_EventWait(30);
        Engine_AudioPlayCue(30);
        Call3(Engine_ActorSetSpeed, 19, 78643, 39321);
        Engine_ActorSetAnimation(19, 2);
        record = (u8 *)Object_GetById(0);
        if (record != 0) {
            Engine_ActorSetDestination(19, *(s16 *)(record + 10), *(s16 *)(record + 18));
        }
        Engine_ActorWaitForMove(19);
        Engine_ActorSetPosition(19, 0, 0);
        Engine_EventWait(10);
        Audio_PlayCueFromEventWork();
        Engine_EventEnd();
    }
}

s32 SuharaHeya_SelectEvents(void)
{
    if (Engine_GameFlagIsSet(0x96F) != 0) {
        return (s32)gSuharaHeyaEvents96f;
    }
    return (s32)gSuharaHeyaEvents;
}

s32 Scene_InitActorRecords(void)
{
    union SceneActor *work;
    if (gGameState.entrance == 90)
        Engine_GameFlagSet(0x96f);
    ((s32 *)gEventWork)[112] = 521;
    ((s32 *)gEventWork)[114] = 24;
    ((union SceneActor *)Object_GetById(12))->bytes[89] |= 4;
    ((union SceneActor *)Object_GetById(13))->bytes[89] |= 4;
    work = (union SceneActor *)Object_GetById(20);
    work->fields.record->field_26 = 0;
    work->fields.record->angle = 0x4000;
    {
        /* The mask is built in a local, not folded into the store. */
        struct SceneRecord *record = work->fields.record;
        s32 flags = ~12;

        flags = flags & record->flags;
        record->flags = flags | 4;
    }
    work = (union SceneActor *)Object_GetById(21);
    work->fields.record->field_26 = 0;
    work->fields.record->angle = 0x4000;
    work->bytes[85] = 2;
    work->fields.y = 0;
    return 0;
}
