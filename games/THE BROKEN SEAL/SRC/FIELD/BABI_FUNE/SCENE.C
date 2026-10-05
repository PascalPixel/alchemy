#include "GLOBAL_CELLS.H"
#include "TBS_EDITION.H"
#include "TYPES.H"
#include "SCENE_IDS.H"
#include "CALL.H"
#include "FIELDOBJ.H"
#include "MAP_SCROLL.H"
#include "IO_REG.H"
#include "FIELD_EVENT.H"

extern s32 BabiFune_StoredSlot0;
extern s32 BabiFune_StoredRecord1;
extern s32 BabiFune_StoredSlot3;
extern s32 BabiFune_StoredRecord2;

extern u8 MsgFieldLooksLikeFinally[];
extern const s32 BabiFune_ActionScriptA[];
extern const s32 BabiFune_ActionScriptB[];
extern const s32 BabiFune_ActionScriptC[];
extern const s32 BabiFune_ActionScriptD[];
void Engine_MapCopyCellAttributes();
void Motion_LaunchFromFocusedObject();
s32 UiText_OpenMessageAtObject();
s32 Inventory_PromptAndSetObjectMode();
void Object_RefreshSelectorById();
void ObjectMotion_CommitPositionAndActivate();
void MusicCommand_SetPitchAndUpdateFrequency();
void BabiFune_ScheduleFade();
void Event_SetDestinationFar();

s32 BabiFune_StoreLeaderHeight(void)
{
    s32 *d;
    struct FieldActor *p;

    d = &BabiFune_StoredSlot0;
    p = Object_GetById(0);
    *d = p->y.fixed;
    return 0;
}

s32 BabiFune_StoreActorOneHeight(void)
{
    s32 *p;
    struct FieldActor *rec;

    p = &BabiFune_StoredRecord1;
    rec = Object_GetById(1);
    *p = rec->y.fixed;
    return 0;
}

s32 BabiFune_StoreActorThreeHeight(void)
{
    s32 *d;
    struct FieldActor *p;

    d = &BabiFune_StoredSlot3;
    p = Object_GetById(3);
    *d = p->y.fixed;
    return 0;
}

s32 BabiFune_StoreActorTwoHeight(void)
{
    s32 *d;
    struct FieldActor *p;

    d = &BabiFune_StoredRecord2;
    p = Object_GetById(2);
    *d = p->y.fixed;
    return 0;
}

void Scene_RunExtendedPresentationSequence(void)
{
    struct MapScrollWork *scroll;
    struct FieldActor *ship;
    struct EventWork *event;
    s32 blank;

    Engine_EventBegin();
    Call4(Engine_CameraMoveTo, -1, -1, -1, 0);
    Call6(Engine_MapCopyCellAttributes, 18, 0, 3, 1, 18, 12);
    Engine_EventSetMessage((s32)MsgFieldLooksLikeFinally);
    Call4(Motion_LaunchFromFocusedObject, 1, -10, 16, 49152);
    Call4(Motion_LaunchFromFocusedObject, 3, 0, 24, 49152);
    Motion_LaunchFromFocusedObject(2, 10, 16, 49152);
    Engine_ActorWaitForMove(1);
    Engine_EventWait(50);
    Engine_ActorRunRepeatedMotion(1, 2);
    Engine_EventWait(20);
    Engine_EventShowMessage(1, 0);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(2, 3);
    Engine_EventWait(20);
    Engine_EventShowMessage(2, 0);
    Engine_EventWait(10);
    Call3(Engine_ActorFaceDirection, 3, 16384, 0);
    Engine_EventWait(40);
    Engine_EventShowMessage(3, 0);
    Engine_EventWait(10);
    Call3(Engine_ActorFaceDirection, 0, 16384, 0);
    Call3(Engine_ActorFaceDirection, 1, 16384, 0);
    Engine_ActorFaceDirection(2, 16384, 0);
    Engine_EventWait(60);
    Engine_ActorSetAnimation(0, 3);
    Engine_ActorSetAnimation(1, 3);
    Engine_ActorSetAnimationAndWait(2, 3);
    Engine_EventWait(60);
    Engine_ActorRunRepeatedMotion(1, 2);
    Engine_EventWait(30);
    Engine_ActorFaceDirection(1, 57344, 0);
    Engine_EventWait(20);
    UiText_OpenMessageAtObject(1, 0);
    if (Inventory_PromptAndSetObjectMode(0, 0) == 0) {
        Engine_EventWait(30);
        Call3(Engine_ActorShowEmote, 1, 258, 50);
        Engine_EventShowMessage(1, 0);
        gEventWork->message++;
    } else {
        Engine_EventWait(30);
        Call3(Engine_ActorShowEmote, 1, 258, 50);
        gEventWork->message++;
        Engine_EventShowMessage(1, 0);
    }
    Engine_EventWait(10);
    Engine_ActorFaceDirection(2, 32768, 0);
    Engine_EventWait(30);
    Call3(Engine_ActorShowEmote, 2, 257, 50);
    Engine_EventShowMessage(2, 0);
    Engine_EventWait(10);
    Engine_ActorFaceDirection(1, 0, 0);
    Engine_EventWait(30);
    Call3(Engine_ActorShowEmote, 1, 257, 60);
    Engine_ActorRunRepeatedMotion(3, 2);
    Engine_EventWait(30);
    Engine_ActorFaceDirection(3, 40960, 0);
    Engine_EventWait(30);
    Engine_EventShowMessage(3, 0);
    Engine_EventWait(10);
    Call3(Engine_ActorShowEmote, 1, 256, 50);
    Engine_EventShowMessage(1, 0);
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(2, 2);
    Engine_EventWait(20);
    Engine_EventShowMessage(2, 0);
    Engine_EventWait(10);
    Engine_ActorFaceDirection(3, 49152, 0);
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(3, 4);
    Engine_EventWait(20);
    UiText_OpenMessageAtObject(3, 0);
    if (Inventory_PromptAndSetObjectMode(0, 0) == 0) {
        Engine_EventWait(20);
        Engine_ActorRunRepeatedMotion(1, 2);
        Engine_EventWait(20);
        Engine_ActorFaceDirection(1, 57344, 0);
        Engine_EventWait(20);
        Engine_EventShowMessage(1, 0);
        gEventWork->message++;
    } else {
        Engine_EventWait(20);
        Engine_ActorRunRepeatedMotion(1, 2);
        Engine_EventWait(20);
        Engine_ActorFaceDirection(1, 57344, 0);
        Engine_EventWait(20);
        gEventWork->message++;
        Engine_EventShowMessage(1, 0);
    }
    Engine_EventWait(10);
    Engine_ActorShowEmote(0, 258, 50);
    Engine_EventWait(10);
    Call3(Engine_ActorFaceDirection, 2, 40960, 0);
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(2, 3);
    Engine_EventWait(20);
    Engine_EventShowMessage(2, 0);
    Engine_EventWait(20);
    Engine_EventShowMessage(1, 0);
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(0, 3);
    Engine_EventWait(30);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(3, 3);
    Engine_EventWait(20);
    UiText_OpenMessageAtObject(3, 0);
    Call3(Engine_ActorFaceDirection, 1, 57344, 0);
    Call3(Engine_ActorFaceDirection, 3, 49152, 0);
    Engine_ActorFaceDirection(2, 40960, 0);
    Engine_EventWait(20);
    if (Inventory_PromptAndSetObjectMode(0, 0) == 0) {
        Engine_EventWait(30);
        Engine_ActorSetAnimationAndWait(3, 3);
        Engine_EventWait(20);
        Engine_EventShowMessage(3, 0);
        gEventWork->message++;
    } else {
        Engine_EventWait(30);
        Engine_ActorSetAnimationAndWait(3, 3);
        Engine_EventWait(20);
        gEventWork->message++;
        Engine_EventShowMessage(3, 0);
    }
    Engine_EventWait(10);
    Call3(Engine_ActorFaceDirection, 1, 49152, 0);
    Engine_EventWait(30);
    Engine_EventShowMessage(1, 0);
    Engine_EventWait(10);
    Call3(Engine_ActorFaceDirection, 0, 49152, 0);
    Engine_EventWait(30);
    Engine_EventWait(10);
    Call3(Engine_ActorFaceDirection, 2, 49152, 0);
    Engine_EventWait(30);
    Engine_ActorSetAnimationAndWait(2, 3);
    Engine_EventWait(20);
    Engine_EventShowMessage(2, 0);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(3, 3);
    Engine_EventWait(20);
    Engine_EventShowMessage(3, 0);
    Engine_AudioPlayCue(17);
    Engine_EventWait(10);
    Engine_ActorSetAnimation(0, 3);
    Engine_ActorSetAnimation(1, 3);
    Engine_ActorSetAnimationAndWait(2, 3);
    Engine_EventWait(40);
    Call3(Engine_ActorSetSpeed, 0, 78643, 39321);
    Call3(Engine_ActorSetSpeed, 1, 78643, 39321);
    Call3(Engine_ActorSetSpeed, 2, 78643, 39321);
    Call3(Engine_ActorSetSpeed, 3, 78643, 39321);
    Engine_ActorEnableActionCallback(0, BabiFune_ActionScriptA);
    Engine_EventWait(50);
    Call4(Engine_CameraMoveTo, 17301504, -1, 13107200, 1);
    Engine_ActorEnableActionCallback(1, BabiFune_ActionScriptB);
    Engine_EventWait(50);
    Engine_ActorEnableActionCallback(2, BabiFune_ActionScriptC);
    Object_RefreshSelectorById(2);
    Call3(Engine_ActorFaceDirection, 0, 16384, 0);
    Call3(Engine_ActorFaceDirection, 1, 16384, 0);
    Call3(Engine_ActorFaceDirection, 2, 16384, 0);
    Call3(ObjectMotion_CommitPositionAndActivate, 3, 0, -32);
    Engine_EventWait(30);
    Engine_ActorFaceDirection(3, 16384, 0);
    Engine_EventWait(60);
    Call3(Engine_ActorFaceDirection, 3, 32768, 0);
    Engine_EventWait(20);
    Engine_ActorEnableActionCallback(3, BabiFune_ActionScriptD);
    Object_RefreshSelectorById(3);
    Engine_ActorFaceDirection(3, 49152, 0);
    Engine_EventWait(20);
    Call4(Engine_CameraMoveTo, 14155776, -1, 11010048, 1);
    Engine_CameraWaitForMove();
    Engine_EventWait(20);
    Call3(Engine_ActorFaceDirection, 0, 32768, 0);
    Call3(Engine_ActorFaceDirection, 1, 32768, 0);
    Call3(Engine_ActorFaceDirection, 2, 32768, 0);
    Engine_ActorFaceDirection(3, 32768, 0);
    Engine_EventWait(40);
    Engine_ActorSetAnimation(0, 3);
    Engine_ActorSetAnimation(1, 3);
    Engine_ActorSetAnimation(3, 3);
    Engine_ActorSetAnimationAndWait(2, 3);
    Engine_EventWait(30);
    Engine_AudioPlayCue(67);
    MusicCommand_SetPitchAndUpdateFrequency(240);
    BabiFune_ScheduleFade();
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
    Engine_EventWait(30);
#else
    Engine_EventWait(80);
#endif
    scroll = gMapWork[0];
    ship = Object_GetById(8);
    ship->acceleration = 131;
    ship->speed = 0x20000;
    scroll->layers[0].speed_x = -0x3000;
    ObjectMotion_CommitPositionAndActivate(8, 60, 0);
    scroll->layers[0].speed_x = -0x6000;
    ObjectMotion_CommitPositionAndActivate(8, 60, 0);
    Engine_EventWait(80);
    Engine_EventWait(100);
    Engine_CameraSetSpeed(144179, 655);
    Engine_CameraMoveTo(52953088, -1, 11010048, 1);
    Engine_EventWait(300);
    event = gMapWork[19];
    event->start_transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    /* FAKEMATCH: keep the backdrop zero before its address calculation. */
    do {
        blank = 0;
    } while (0);
    PLTT_BACKDROP = blank;
    event->transition_frames = 96;
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
    Engine_EventWait(30);
    Engine_GameFlagSet(282);
    Event_SetDestinationFar((s32)&SceneId_Title, 9);
}
