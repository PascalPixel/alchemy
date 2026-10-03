#include "SUHARA.H"

extern u8 MsgSuharaArentSurprisedFind[];
extern u8 MsgSuharaBusyEverSince[];
extern u8 MsgSuharaGetSickThinkingAboutLalivero[];
extern u8 MsgSuharaOursOnlyStore[];
extern u8 MsgSuharaSighNothinMaybe[];
extern u8 MsgSuharaWonderWhySandstorms[];

extern u8 MsgSuharaSupposeFolkBlown[];

extern u8 MsgSuharaIodem[];

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
    u16 facing = (Object_GetById(ACTOR_PARTY_LEADER)->facing + 0x2000) & ~0x3fff;
    if (facing == 0xc000) {
        Engine_ShopOpen(31, no);
    } else if (Engine_GameFlagIsSet(0x96f)) {
        s32 msg = (s32)MsgSuharaArentSurprisedFind;
        Engine_EventSetMessage(msg);
        Engine_EventOpenMessage(no, 0);
        if (Engine_EventChooseYesNo(ACTOR_PARTY_LEADER, 0) == 0) {
            Engine_EventWait(10);
            Engine_EventSetMessage(msg + 1);
        } else {
            Engine_EventSetMessage(msg + 2);
        }
        Engine_EventShowMessage(no, 0);
    } else {
        Engine_EventSetMessage((s32)MsgSuharaOursOnlyStore);
        Engine_EventShowMessage(no, 0);
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
        Engine_EventSetMessage((s32)MsgSuharaGetSickThinkingAboutLalivero);
        Engine_EventShowMessage(no, 0);
    }
}

/* Asks whether the party was blown here on the way to Babi Lighthouse, with a
 * line for each answer. */
void SuharaHeya_AskBlownHere(s32 obj)
{
    s32 msg = (s32)MsgSuharaSupposeFolkBlown;
    Engine_EventSetMessage(msg);
    Engine_EventOpenMessage(obj, 0);
    if (Engine_EventChooseYesNo(ACTOR_PARTY_LEADER, 0) == 0) {
        Engine_EventWait(10);
        Engine_EventSetMessage(msg + 1);
    } else {
        Engine_EventSetMessage(msg + 2);
    }
    Engine_EventShowMessage(obj, 0);
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
    GameFlag_Set(2480);
    if (GameFlag_IsSet(2442) == 0) {
        Audio_PlayCue(30);
        Event_Begin();
        Camera_MoveTo(24117248, -1, 6815744, 1);
        Actor_WalkToAndWait(0, 368, 160);
        Actor_FaceDirection(0, 49152, 0);
        Motion_LaunchFromFocusedObject(19, 0, -16, 49152);
        Actor_WaitForMove(19);
        Camera_WaitForMove();
        Event_SetMessage((s32)MsgSuharaIodem);
        Event_Wait(10);
        Actor_RunRepeatedMotion(20, 2);
        Event_Wait(20);
        Event_ShowMessage(20, 0);
        Event_Wait(10);
        Actor_RunRepeatedMotion(19, 2);
        Event_Wait(20);
        Actor_SetSpeed(19, 78643, 39321);
        Actor_WalkByAndWait(19, 0, -16);
        Actor_FaceDirection(19, 0, 0);
        Event_Wait(30);
        Actor_FaceDirection(19, 57344, 0);
        Event_Wait(30);
        Actor_FaceDirection(19, 0, 0);
        Event_Wait(30);
        Actor_ShowEmote(19, 256, 40);
        Event_ShowMessage(19, 0);
        Event_Wait(10);
        Actor_RunRepeatedMotion(21, 2);
        Event_Wait(20);
        Event_ShowMessage(21, 0);
        Event_Wait(10);
        Actor_SetSpeed(19, 78643, 39321);
        Actor_WalkByAndWait(19, 0, -24);
        Actor_WalkByAndWait(19, 48, 0);
        Actor_FaceDirection(19, 49152, 0);
        Event_Wait(30);
        Event_ShowMessage(19, 0);
        Event_Wait(20);
        Actor_ShowEmote(19, 256, 40);
        Event_ShowMessage(19, 0);
        Event_Wait(20);
        Actor_ShowEmote(20, 258, 40);
        Event_ShowMessage(20, 0);
        Event_Wait(10);
        Actor_FaceDirection(19, 8192, 0);
        Event_Wait(20);
        Event_ShowMessage(19, 0);
        Event_Wait(10);
        Actor_RunRepeatedMotion(21, 2);
        Event_Wait(20);
        Actor_SetAnimationAndWait(21, 4);
        Event_Wait(20);
        Event_ShowMessage(21, 0);
        Event_Wait(10);
        Actor_FaceDirection(19, 49152, 0);
        Event_Wait(30);
        Actor_ShowEmote(19, 263, 40);
        Event_ShowMessage(19, 0);
        Event_Wait(10);
        Actor_StartRepeatedMotion(20, 2);
        Actor_RunRepeatedMotion(21, 2);
        Event_Wait(20);
        Actor_SetSpeed(0, 78643, 39321);
        Actor_WalkToAndWait(0, 368, 104);
        Actor_WalkByAndWait(0, 16, 0);
        Actor_FaceDirection(0, 0, 0);
        Event_Wait(20);
        Event_Wait(10);
        Actor_SetAnimationAndWait(19, 4);
        Event_Wait(20);
        Event_ShowMessage(19, 0);
        Event_Wait(20);
        Actor_ShowEmote(19, 258, 50);
        Event_ShowMessage(19, 0);
        Event_Wait(10);
        Actor_RunRepeatedMotion(20, 2);
        Event_Wait(20);
        Event_ShowMessage(20, 0);
        Event_Wait(10);
        Actor_FaceActor(0, 20, 0);
        Actor_FaceDirection(19, 8192, 0);
        Event_Wait(20);
        Event_ShowMessage(19, 0);
        Event_Wait(10);
        Actor_RunRepeatedMotion(21, 2);
        Event_Wait(20);
        Event_ShowMessage(21, 0);
        Event_Wait(10);
        Actor_FaceActor(0, 21, 0);
        Actor_FaceDirection(19, 49152, 0);
        Event_Wait(40);
        Actor_SetAnimationAndWait(19, 3);
        Event_Wait(30);
        Event_ShowMessage(19, 0);
        Event_Wait(10);
        Actor_RunRepeatedMotion(21, 2);
        Event_Wait(20);
        Actor_SetAnimationAndWait(21, 4);
        Event_Wait(20);
        Event_ShowMessage(21, 0);
        Event_Wait(10);
        Actor_FaceEachOther(19, 0, 30);
        Event_ShowMessage(19, 0);
        Event_Wait(10);
        Actor_RunRepeatedMotion(20, 2);
        Event_Wait(20);
        Event_ShowMessage(20, 0);
        Event_Wait(10);
        Actor_FaceActor(0, 20, 0);
        Actor_FaceDirection(19, 8192, 0);
        Event_Wait(40);
        Event_Wait(10);
        Actor_RunRepeatedMotion(21, 2);
        Event_Wait(20);
        Event_ShowMessage(21, 0);
        Event_Wait(10);
        Actor_FaceActor(0, 21, 0);
        Actor_FaceDirection(19, 49152, 0);
        Event_Wait(40);
        Actor_SetAnimation(0, 3);
        Actor_SetAnimationAndWait(19, 3);
        Event_Wait(30);
        Event_Wait(10);
        Actor_SetAnimationAndWait(20, 4);
        Event_Wait(20);
        Event_ShowMessage(20, 0);
        Event_Wait(10);
        Actor_ShowEmote(0, 256, 0);
        Actor_ShowEmote(19, 256, 40);
        Event_Wait(10);
        Actor_FaceActor(0, 20, 0);
        Actor_FaceDirection(19, 8192, 0);
        Event_Wait(40);
        Event_Wait(10);
        Actor_RunRepeatedMotion(21, 2);
        Event_Wait(20);
        Event_ShowMessage(21, 0);
        Event_Wait(10);
        Actor_ShowEmote(0, 256, 50);
        Actor_FaceActor(0, 21, 0);
        Event_Wait(30);
        Actor_RunRepeatedMotion(21, 2);
        Event_Wait(30);
        Event_Wait(10);
        Actor_FaceDirection(19, 49152, 0);
        Event_Wait(30);
        Event_ShowMessage(19, 0);
        Event_Wait(10);
        Actor_RunRepeatedMotion(21, 2);
        Event_Wait(20);
        Actor_SetAnimationAndWait(21, 3);
        Event_Wait(30);
        Event_Wait(10);
        Actor_ShowEmote(19, 258, 40);
        Event_ShowMessage(19, 0);
        Event_Wait(10);
        Actor_SetAnimationAndWait(20, 4);
        Event_Wait(20);
        Event_ShowMessage(20, 0);
        Event_Wait(10);
        Actor_ShowEmote(19, 257, 50);
        Event_Wait(10);
        Actor_FaceActor(0, 20, 0);
        Actor_FaceDirection(19, 8192, 0);
        Event_Wait(20);
        Event_Wait(10);
        Actor_RunRepeatedMotion(20, 2);
        Event_Wait(20);
        Event_ShowMessage(20, 0);
        Event_Wait(10);
        Actor_ShowEmote(19, 256, 40);
        Event_ShowMessage(19, 0);
        Event_Wait(10);
        Actor_RunRepeatedMotion(21, 2);
        Event_Wait(20);
        Event_ShowMessage(21, 0);
        Actor_RunRepeatedMotion(21, 2);
        Event_Wait(20);
        Event_Wait(10);
        Actor_FaceEachOther(19, 0, 0);
        Event_Wait(30);
        Event_ShowMessage(19, 0);
        Event_Wait(10);
        Actor_SetAnimationAndWait(20, 4);
        Event_Wait(20);
        Event_ShowMessage(20, 0);
        Event_Wait(10);
        Actor_ShowEmote(0, 258, 0);
        Actor_ShowEmote(19, 258, 80);
        Actor_ShowEmote(21, 258, 50);
        Event_ShowMessage(21, 0);
        Event_Wait(10);
        Actor_RunRepeatedMotion(21, 2);
        Event_Wait(20);
        Event_ShowMessage(21, 0);
        Actor_RunRepeatedMotion(21, 3);
        Event_Wait(20);
        Event_Wait(10);
        Actor_FaceDirection(19, 49152, 0);
        Event_Wait(30);
        Actor_RunRepeatedMotion(19, 2);
        Event_Wait(10);
        Event_ShowMessage(19, 0);
        Event_Wait(10);
        Actor_RunRepeatedMotion(20, 2);
        Event_Wait(20);
        Event_ShowMessage(20, 0);
        Event_Wait(10);
        Actor_FaceActor(0, 20, 0);
        Actor_FaceDirection(19, 8192, 0);
        Event_Wait(30);
        Actor_SetAnimationAndWait(19, 3);
        Event_Wait(30);
        Event_ShowMessage(19, 0);
        Event_Wait(10);
        Actor_StartRepeatedMotion(20, 2);
        Actor_RunRepeatedMotion(21, 2);
        Event_Wait(30);
        Event_Wait(10);
        Actor_FaceEachOther(19, 0, 20);
        Actor_WalkByAndWait(19, -12, 0);
        Event_Wait(20);
        Event_OpenMessage(19, 0);
        if (Event_ChooseYesNo(0, 0) == 0) {
            Event_Wait(20);
            Event_ShowMessage(19, 0);
            gEventWork->message++;
        } else {
            Event_Wait(10);
            gEventWork->message++;
            Event_ShowMessage(19, 0);
        }
        Event_Wait(10);
        Actor_RunRepeatedMotion(19, 2);
        Event_Wait(20);
        Event_ShowMessage(19, 0);
        Event_Wait(10);
        Actor_ShowEmote(19, 258, 50);
        Event_ShowMessage(19, 0);
        Event_Wait(10);
        Actor_SetAnimationAndWait(0, 3);
        Event_Wait(20);
        Actor_SetAnimationAndWait(19, 3);
        Event_Wait(30);
        Audio_PlayCue(30);
        Actor_SetSpeed(19, 78643, 39321);
        Actor_SetAnimation(19, 2);
        record = (u8 *)Actor_Get(0);
        if (record != 0) {
            Actor_SetDestination(19, *(s16 *)(record + 10), *(s16 *)(record + 18));
        }
        Actor_WaitForMove(19);
        Actor_SetPosition(19, 0, 0);
        Event_Wait(10);
        Audio_PlayCueFromEventWork();
        Event_End();
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
