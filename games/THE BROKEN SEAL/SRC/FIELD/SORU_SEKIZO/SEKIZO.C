#define OWN_gGameState
#include "CALLBACK_SCHEDULER.H"
#include "RESOURCE.H"
#include "MEM.H"
#include "GLOBAL_CELLS.H"
#include "STATUE_HALL.H"
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SERVICE.H"
#include "FIELD_SCENE.H"
#include "CALL.H"
#include "text/MSG_IDS.H"

extern u16 gGameState[][1];


enum StatueHallActor {
    ACTOR_SUKURETA = ACTOR_FIRST_PLACED
};

void Event_SayThenWait();
void ObjectMotion_ResetAndSetPositionInMode2();
s32 Inventory_PromptAndSetObjectMode();
s32 UiText_OpenMessageAtObject();

struct FocusPoint {
    s32 x;
    s32 y;
    s32 z;
};

struct FocusWork {
    struct FocusPoint *focus;
};

struct Ent SoruSekizo_SpriteRows[27];

void Engine_MapCopyCellAttributes();

u8 *SceneEventRuntime_GetScriptData(void)
{
    return SceneEventRuntime_ScriptData;
}

s32 SceneEventRuntime_ReturnZero(void)
{
    return 0;
}

u8 *SceneEventRuntime_GetMessageData(void)
{
    return SceneEventRuntime_MessageData;
}

u8 *SceneEventRuntime_GetActorData(void)
{
    return SceneEventRuntime_ActorData;
}

u8 *SceneEventRuntime_GetEffectData(void)
{
    return SceneEventRuntime_EffectData;
}

s32 SceneEventRuntime_SelectInitialSceneByFlags(void)
{
    s32 no;

    if (Engine_GameFlagIsSet(0x818) != 0) {
        if (Engine_GameFlagIsSet(FLAG_STATUE_TRAP_SPRUNG) == 0) {
            no = 3;
            goto apply;
        }
        goto fail;
    }
    if (Engine_GameFlagIsSet(0x812) == 0) {
        no = 4;
apply:
        Engine_EventRequestExit(no);
        return 1;
    }
fail:
    return -1;
}

void Scene_OpenTheHole(void)
{
    s32 i;

    { s32 k5 = 2, k6 = 1; Engine_MapCopyCellsTo(0, 28, 17, 8, k5, k6); }
    Engine_AudioPlayCue(200);
    for (i = 0; i != 22; i++) {
        Engine_MapCopyCellsTo(10, 61, 17, 40, 2, 1);
        Engine_EventWait(4);
        Engine_MapCopyCellsTo(8, 61, 17, 40, 2, 1);
        Engine_EventWait(4);
    }
    { s32 k5 = 4, k6 = 3;
      Engine_MapCopyCellsTo(0, 59, 15, 38, k5, k6);
      Engine_MapCopyCellsTo(4, 59, 17, 38, k5, k6); }
    Engine_MapCopyCellsTo(8, 60, 17, 39, 2, 2);
    { s32 k5 = 17, k6 = 8; Engine_MapCopyCellAttributes(0, 0, 2, 1, k5, k6); }
    Engine_GameFlagSet(0x207);
    SoruSekizo_RunStatueDropScene();
}

void FieldScene_SetupStagedActors(void)
{
    struct FieldActor *leader;

    Engine_EventBegin();
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    gEventWork->transition_frames = 32;
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Engine_EventWait(20);
    Actor_SetPosition(ACTOR_SUKURETA, 0x2400000, 0xe80000);
    Engine_EventWait(1);
    Engine_EventSetMessage((s32)MsgSoruSukuretaYouFoundIt);
    Event_SayThenWait(ACTOR_SUKURETA, 6);
    Actor_SetPosition(ACTOR_SUKURETA, 0x2400000, 0x1180000);
    Camera_SetSpeed(0xcccc, 0x1999);
    Camera_MoveTo(0x23e0000, -1, 0xb40000, 1);
    Actor_SetSpeed(ACTOR_SUKURETA, 0x10000, 0x8000);
    Actor_WalkToAndWait(ACTOR_SUKURETA, 0x240, 216);
    Engine_EventWait(20);
    Engine_ActorJump(JASMINE, 2, 0);
    Engine_EventWait(30);
    Event_SayThenWait(JASMINE, 6);
    Engine_ActorRunRepeatedMotion(ACTOR_SUKURETA, 2);
    Engine_EventWait(6);
    Actor_FaceDirection(ACTOR_SUKURETA, FACING_WEST + FACING_STEP, 0);
    Engine_EventWait(10);
    Camera_SetSpeed(0x59999, 0xb333);
    Camera_MoveTo(0x11f0000, -1, 0xb00000, 1);
    Engine_CameraWaitForMove();
    Engine_EventWait(60);
    Camera_MoveTo(0x23e0000, -1, 0xb40000, 1);
    Engine_CameraWaitForMove();
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(ACTOR_SUKURETA, ANIM_NOD);
    Engine_EventWait(10);
    Actor_FaceDirection(ACTOR_SUKURETA, FACING_NORTH, 0);
    Engine_EventWait(10);
    Engine_ActorJump(ACTOR_SUKURETA, 6, 0);
    Actor_SetSpeed(ACTOR_SUKURETA, 0x30000, 0x20000);
    Actor_WalkToAndWait(ACTOR_SUKURETA, 0x240, 184);
    Engine_EventWait(40);
    Event_SayThenWait(ACTOR_SUKURETA, 6);
    Actor_FaceDirection(ACTOR_SUKURETA, FACING_WEST, 0);
    Engine_EventWait(40);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 2);
    gEventWork->transition_frames = 16;
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
    Camera_MoveTo(0x11f0000, -1, 0xb00000, 0);
    Engine_MapRedraw();
    Engine_TaskWait(1);
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Engine_EventWait(40);
    Actor_SetPosition(ACTOR_SUKURETA, 0x1a80000, 0xc80000);
    Engine_TaskWait(1);
    Event_SayThenWait(ACTOR_SUKURETA, 20);
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
    Actor_SetPosition(ACTOR_SUKURETA, 0x2400000, 0xb80000);
    Camera_MoveTo(0x23e0000, -1, 0xb40000, 0);
    Engine_MapRedraw();
    Engine_TaskWait(1);
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Engine_EventWait(20);
    Camera_SetSpeed(0x13333, 0x2666);
    Camera_MoveTo(0x23e0000, -1, 0x9d0000, 1);
    Engine_CameraWaitForMove();
    Engine_EventWait(20);
    Engine_ActorRunRepeatedMotion(GERALD, 2);
    Event_SayThenWait(GERALD, 20);
    Engine_ActorSetAnimationAndWait(JASMINE, ANIM_NOD);
    Event_SayThenWait(JASMINE, 6);
    Engine_ActorRunRepeatedMotion(ACTOR_SUKURETA, 2);
    Engine_EventWait(40);
    Engine_ActorSetAnimationAndWait(ACTOR_SUKURETA, ANIM_SHAKE_HEAD);
    Event_SayThenWait(ACTOR_SUKURETA, 80);
    Actor_ShowEmote(ACTOR_SUKURETA, EMOTE_IN_FRONT | 2, 0);
    Engine_EventWait(60);
    Actor_FaceDirection(ACTOR_SUKURETA, FACING_NORTH, 0);
    Engine_EventWait(30);
    Event_SayThenWait(ACTOR_SUKURETA, 20);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 1, 0);
    Actor_ShowEmote(GERALD, EMOTE_IN_FRONT | 1, 0);
    Actor_ShowEmote(JASMINE, EMOTE_IN_FRONT | 1, 0);
    Engine_EventWait(60);
    Actor_FaceDirection(ACTOR_SUKURETA, FACING_NORTH, 0);
    Engine_EventWait(30);
    Engine_ActorSetAnimationAndWait(ACTOR_SUKURETA, ANIM_SHAKE_HEAD);
    Event_SayThenWait(ACTOR_SUKURETA, 6);
    Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, GERALD, 0);
    Engine_EventWait(40);
    Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, JASMINE, 0);
    Engine_EventWait(40);
    Engine_ActorSetAnimationAndWait(ACTOR_SUKURETA, ANIM_NOD);
    Event_SayThenWait(ACTOR_SUKURETA, 6);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
    Actor_FaceDirection(JASMINE, FACING_SOUTH, 0);
    Actor_FaceDirection(GERALD, FACING_SOUTH, 0);
    Engine_EventWait(40);
    Engine_ActorSetAnimation(GERALD, ANIM_NOD);
    Engine_ActorSetAnimation(JASMINE, ANIM_NOD);
    Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(ACTOR_SUKURETA, ANIM_NOD);
    Event_SayThenWait(ACTOR_SUKURETA, 6);
    Camera_SetSpeed(0x19999, 0x3333);
    Camera_MoveTo(0x2400000, -1, 0xd70000, 1);
    Actor_FaceDirection(ACTOR_SUKURETA, FACING_SOUTH, 0);
    Engine_EventWait(10);
    Engine_ActorJump(ACTOR_SUKURETA, 6, 0);
    Actor_WalkToAndWait(ACTOR_SUKURETA, 0x240, 217);
    Engine_EventWait(20);
    Actor_WalkToAndWait(ACTOR_SUKURETA, 0x240, 0x141);
    Actor_SetPosition(ACTOR_SUKURETA, 0, 0);
    Camera_SetSpeed(0x39999, 0x7333);
    Camera_MoveTo(0x2400000, -1, 0x880000, 1);
    Engine_CameraWaitForMove();
    Engine_EventWait(20);
    Actor_SetSpeed(GERALD, 0x10000, 0x8000);
    Actor_SetSpeed(JASMINE, 0x10000, 0x8000);
    Engine_ActorSetAnimation(GERALD, ANIM_WALK);
    leader = Actor_Get(ACTOR_PARTY_LEADER);
    if (leader != NULL) {
        Actor_SetDestination(GERALD, leader->x.part.pixel, leader->z.part.pixel);
    }
    Engine_ActorWaitForMove(GERALD);
    Actor_SetPosition(GERALD, 0, 0);
    Engine_ActorSetAnimation(JASMINE, ANIM_WALK);
    leader = Actor_Get(ACTOR_PARTY_LEADER);
    if (leader != NULL) {
        Actor_SetDestination(JASMINE, leader->x.part.pixel, leader->z.part.pixel);
    }
    Engine_ActorWaitForMove(JASMINE);
    Actor_SetPosition(JASMINE, 0, 0);
    GameFlag_Clear(FLAG_ARRIVAL_EVENT_PENDING);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
    gEventWork->transition_frames = 16;
    Engine_EventEnd();
}

void FieldScene_RunStagedActorScene(void)
{
    struct EventWork *work;
    struct FieldActor *record;

    Engine_EventBegin();
    work = gEventWork;
    work->start_transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    work->transition_frames = 32;
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Engine_EventWait(20);
    Engine_ActorSetPosition(8, 0x2400000, 0x1280000);
    Engine_EventWait(1);
    Engine_EventSetMessage((s32)MsgSoruWait);
    Event_SayThenWait(8, 6);
    Camera_SetSpeed(0xcccc, 0x1999);
    Camera_MoveTo(0x23e0000, -1, 0xb40000, 1);
    Actor_SetSpeed(8, 0x10000, 0x8000);
    Actor_WalkToAndWait(8, 0x240, 216);
    Engine_EventWait(20);
    Engine_ActorJump(5, 2, 0);
    Engine_EventWait(30);
    Event_SayThenWait(5, 6);
    Engine_ActorRunRepeatedMotion(8, 2);
    Engine_EventWait(6);
    Engine_ActorFaceDirection(8, 0x9000, 0);
    Engine_EventWait(10);
    Engine_CameraSetSpeed(0x59999, 0xb333);
    Camera_MoveTo(0x11f0000, -1, 0xb00000, 1);
    Engine_CameraWaitForMove();
    Engine_EventWait(60);
    Camera_MoveTo(0x23e0000, -1, 0xb40000, 1);
    Engine_CameraWaitForMove();
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(8, 3);
    Engine_EventWait(10);
    Actor_FaceDirection(8, 0xc000, 0);
    Engine_EventWait(10);
    Engine_ActorJump(8, 6, 0);
    Actor_SetSpeed(8, 0x30000, 0x20000);
    Actor_WalkToAndWait(8, 0x240, 184);
    Engine_EventWait(40);
    Engine_ActorSetAnimationAndWait(8, 3);
    Event_SayThenWait(8, 6);
    Engine_ActorSetAnimation(GERALD, 3);
    Engine_ActorSetAnimation(5, 3);
    Engine_ActorSetAnimationAndWait(0, 3);
    Engine_EventWait(40);
    Engine_ActorRunRepeatedMotion(8, 3);
    Event_SayThenWait(8, 6);
    Engine_ActorJump(0, 2, 0);
    Engine_ActorJump(1, 2, 0);
    Engine_ActorJump(5, 2, 0);
    Engine_EventWait(30);
    Engine_ActorRunRepeatedMotion(1, 2);
    Event_SayThenWait(1, 6);
    Engine_ActorRunRepeatedMotion(8, 1);
    Engine_ActorSetAnimationAndWait(8, 4);
    Event_SayThenWait(8, 6);
    Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, JASMINE, 0);
    Engine_EventWait(40);
    Engine_ActorFaceActor(8, 0, 0);
    Actor_FaceActor(8, JASMINE, 0);
    Engine_EventWait(40);
    Engine_ActorJump(8, 6, 0);
    Actor_WalkToAndWait(8, 0x240, 216);
    Actor_FaceDirection(8, 0x8000, 0);
    Engine_EventWait(10);
    Actor_SetSpeed(8, 0x13333, 0x9999);
    ObjectMotion_ResetAndSetPositionInMode2(8, 0x1b0, 200);
    Engine_EventWait(20);
    Camera_SetSpeed(0x26666, 0x4ccc);
    Camera_MoveTo(0x1200000, -1, 0xab0000, 1);
    Engine_CameraWaitForMove();
    Engine_EventWait(80);
    Engine_ActorSetAnimation(8, 1);
    Camera_MoveTo(0x23e0000, -1, 0xb40000, 1);
    Engine_EventWait(20);
    Engine_ActorFaceDirection(8, 0, 0);
    Engine_EventWait(30);
    Actor_WalkToAndWait(8, 0x240, 216);
    Actor_FaceDirection(8, 0xc000, 0);
    Engine_EventWait(10);
    Actor_FaceDirection(0, 0x4000, 0);
    Actor_FaceDirection(5, 0x4000, 0);
    Actor_FaceDirection(GERALD, 0x4000, 0);
    Engine_CameraSetSpeed(0xcccc, 0x1999);
    Engine_CameraMoveTo(0x23e0000, -1, 0xab0000, 1);
    Engine_ActorJump(8, 6, 0);
    Actor_SetSpeed(8, 0x30000, 0x20000);
    Engine_ActorWalkToAndWait(8, 0x240, 184);
    Engine_EventWait(80);
    Event_SayThenWait(8, 6);
    Engine_ActorSetAnimationAndWait(8, 4);
    Event_SayThenWait(8, 20);
    Engine_ActorShowEmote(5, 0x102, 0);
    Engine_EventWait(40);
    Engine_ActorRunRepeatedMotion(JASMINE, 2);
    Event_SayThenWait(5, 6);
    Engine_ActorSetAnimationAndWait(8, 3);
    Engine_ActorFaceDirection(8, 0x8000, 0);
    Engine_EventWait(40);
    Engine_ActorFaceDirection(8, 0xc000, 0);
    Engine_EventWait(30);
    Event_SayThenWait(8, 6);
    Actor_ShowEmote(0, 0x101, 0);
    Actor_ShowEmote(1, 0x101, 0);
    Actor_ShowEmote(JASMINE, 0x101, 0);
    Engine_EventWait(60);
    Engine_ActorSetAnimationAndWait(8, 4);
    Event_SayThenWait(8, 6);
    Engine_ActorFaceEachOther(1, 0, 0);
    Engine_EventWait(40);
    Engine_ActorFaceEachOther(JASMINE, ACTOR_PARTY_LEADER, 0);
    Engine_EventWait(40);
    UiText_OpenMessageAtObject(8, 0);
    Actor_FaceDirection(0, 0x4000, 0);
    Actor_FaceDirection(5, 0x4000, 0);
    Actor_FaceDirection(1, 0x4000, 0);
    if (Inventory_PromptAndSetObjectMode(0, 0) == 0) {
        Engine_EventSetMessage((s32)MsgSoruHonestlyDoubtUnderstand);
    } else {
        Engine_EventSetMessage((s32)MsgSoruHmphWellTold);
    }
    Event_SayThenWait(8, 6);
    Engine_EventSetMessage((s32)MsgSoruTryFindSolution);
    Engine_ActorRunRepeatedMotion(8, 2);
    Event_SayThenWait(8, 6);
    Engine_ActorShowEmote(1, 0x102, 0);
    Engine_EventWait(60);
    Event_SayThenWait(1, 6);
    Engine_ActorSetAnimationAndWait(8, 3);
    Event_SayThenWait(8, 6);
    Engine_ActorFaceDirection(8, 0x4000, 0);
    Engine_EventWait(20);
    Engine_ActorJump(8, 6, 0);
    Actor_WalkToAndWait(8, 0x240, 216);
    Engine_EventWait(40);
    Engine_CameraSetSpeed(0xcccc, 0x1999);
    Camera_MoveTo(0x23e0000, -1, 0xbf0000, 1);
    Actor_SetSpeed(8, 0x10000, 0x8000);
    Engine_ActorWalkToAndWait(8, 0x240, 232);
    Engine_EventWait(40);
    Engine_ActorRunRepeatedMotion(8, 2);
    Engine_EventWait(40);
    Event_SayThenWait(8, 6);
    Engine_ActorFaceDirection(8, 0xc000, 0);
    Engine_EventWait(30);
    Event_SayThenWait(8, 6);
    Engine_ActorSetAnimationAndWait(8, 3);
    Camera_MoveTo(0x2400000, -1, 0xd70000, 1);
    Actor_WalkToAndWait(8, 0x23e, 0x143);
    Engine_ActorSetPosition(8, 0, 0);
    Camera_SetSpeed(0x39999, 0x7333);
    Engine_CameraMoveTo(0x2400000, -1, 0x880000, 1);
    Engine_CameraWaitForMove();
    Engine_EventWait(20);
    Engine_ActorFaceDirection(5, 0, 0);
    Engine_EventWait(10);
    Event_SayThenWait(5, 6);
    Engine_ActorSetAnimation(1, 3);
    Engine_ActorSetAnimationAndWait(5, 3);
    Actor_SetSpeed(1, 0x10000, 0x8000);
    Actor_SetSpeed(5, 0x10000, 0x8000);
    Engine_ActorSetAnimation(JASMINE, 2);
    record = Object_GetById(0);
    if (record != 0) {
        Engine_ActorSetDestination(5, record->x.part.pixel, record->z.part.pixel);
    }
    Engine_ActorWaitForMove(5);
    Engine_ActorSetPosition(5, 0, 0);
    Engine_ActorSetAnimation(1, 2);
    record = Object_GetById(0);
    if (record != 0) {
        Engine_ActorSetDestination(1, record->x.part.pixel, record->z.part.pixel);
    }
    Engine_ActorWaitForMove(1);
    Engine_ActorSetPosition(1, 0, 0);
    GameFlag_Clear(FLAG_ARRIVAL_EVENT_PENDING);
    work = gEventWork;
    work->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
    work->transition_frames = 16;
    Engine_EventEnd();
}

void SoruSekizo_RunSealOpenedSequence(void)
{
    struct FocusWork *work;
    struct FieldActor *leader;
    struct FocusPoint *saved;
    struct FocusPoint point;
    s32 side;
    s32 i;

    work = gMapWork[0];
    leader = Object_GetById(0);
    if (leader->z.fixed < 0xb30000) {
        Actor_WalkToAndWait(0, 0x23f, 132);
        Actor_FaceDirection(0, 0x4000, 0);
        Engine_EventWait(30);
        saved = work->focus;
        point.x = leader->x.fixed;
        point.y = leader->y.fixed;
        point.z = leader->z.fixed;
        work->focus = &point;
        for (i = 0; i != 30; i++) {
            point.z += 0x10000;
            Engine_EventWait(1);
        }
        Engine_EventWait(40);
        side = 1;
    } else {
        Actor_WalkToAndWait(0, 0x241, 222);
        Actor_FaceDirection(0, 0xc000, 0);
        Engine_EventWait(30);
        point.x = leader->x.fixed;
        point.y = leader->y.fixed;
        point.z = leader->z.fixed;
        saved = work->focus;
        work->focus = &point;
        for (i = 0; i != 30; i++) {
            point.z -= 0x10000;
            Engine_EventWait(1);
        }
        Engine_EventWait(40);
        side = 2;
    }
    for (i = 0; i != 6; i++) {
        Map_CopyCellsTo(2, 28, 34, 10, 4, 2);
        Engine_EventWait(8);
        Map_CopyCellsTo(2, 30, 34, 10, 4, 2);
        Engine_EventWait(8);
    }
    for (i = 0; i != 10; i++) {
        Map_CopyCellsTo(2, 28, 34, 10, 4, 2);
        Engine_EventWait(4);
        Map_CopyCellsTo(2, 30, 34, 10, 4, 2);
        Engine_EventWait(4);
    }
    for (i = 0; i != 12; i++) {
        Map_CopyCellsTo(2, 28, 34, 10, 4, 2);
        Engine_EventWait(2);
        Map_CopyCellsTo(2, 30, 34, 10, 4, 2);
        Engine_EventWait(2);
    }
    Map_CopyCellsTo(2, 28, 34, 10, 4, 2);
    Map_CopyCellsTo(8, 55, 32, 40, 8, 4);
    Engine_EventWait(60);
    if (side == 1) {
        for (i = 0; i != 30; i++) {
            point.z -= 0x10000;
            Engine_EventWait(1);
        }
    } else if (side == 2) {
        for (i = 0; i != 30; i++) {
            point.z += 0x10000;
            Engine_EventWait(1);
        }
    }
    work->focus = saved;
}

void SceneEffect_UpdateScrollingSpriteRows(void)
{
    s32 *cp = &((struct Cam *)gMapWork[0])->x;
    struct Ent *e = SoruSekizo_SpriteRows;
    s32 sx = cp[0] / 65536;
    s32 sy = 80 - cp[1] / 65536;
    s32 v;
    u32 i;

    if ((u32)(sy + 16) <= 175) {
        v = (SoruSekizo_ScrollPhase >> 10) - sx;
        v |= -32;
        {
            for (i = 0; i <= 8; i++) {
                e->f06 = v;
                e->f04 = sy;
                Runtime_PushSlotEntry(e, 0);
                v += 32;
                e++;
            }
        }
        v = (SoruSekizo_ScrollPhase >> 9) - sx;
        v |= -32;
        {
            for (i = 0; i <= 8; i++) {
                e->f06 = v;
                e->f04 = sy;
                Runtime_PushSlotEntry(e, 0);
                v += 32;
                e++;
            }
        }
        v = (SoruSekizo_ScrollPhase >> 8) - sx;
        v |= -32;
        {
            for (i = 0; i <= 8; i++) {
                e->f06 = v;
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
                e->f04 = sy + 6;
#else
                e->f04 = sy + 8;
#endif
                Runtime_PushSlotEntry(e, 0);
                v += 32;
                e++;
            }
        }
    }
    SoruSekizo_ScrollPhase += 0x80;
}

void SceneState_RunWhenSlotZeroFacingC000(void)
{
    u16 *p = Object_GetById(0);
    if (p[3] == 0xc000) {
        Engine_LeaderCheckAhead();
    }
}

void SceneState_RunWhenActorZeroFacing4000(void)
{
    u16 *p = Object_GetById(0);
    if (p[3] == 0x4000) {
        Engine_LeaderCheckAhead();
    }
}

void FieldScene_RunPrimarySequenceHead(void)
{
    if (GameFlag_IsSet(GATE_CODE) == 0) {
        Engine_EventBegin();
        Object_GetById(TARGET_ID);
        Actor_SetSpeed(TARGET_ID, 13107, 0x00001999);
        Actor_WalkToAndWait(TARGET_ID, 504, 152);
        Engine_EventEnd();
    }
}

s32 Scene_RunGuardSequenceB(void)
{
    struct FieldActor *record;

    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
    Scene_RunGuardSequenceC();
    GameFlag_Set(0x144);
    record = Object_GetById(18);
    record->collision_flags = 0;
    {
        u8 flags = record->priority_flags | 2;

        record->priority_flags = flags;
    }
    Engine_ActorSetSpriteFlags((s32)Object_GetById(18), 0);
    Object_GetById(18)->priority_flags &= 254;
    Engine_ActorSetSpritePriority(18, 1);
    if ((u32)((gGameState[225][0] - 3) << 16) > 0x10000) {
        Actor_SetPosition(JASMINE, 0, 0);
        Actor_SetPosition(GERALD, 0, 0);
    }
    if (GameFlag_IsSet(0x818) != 0) {
        Actor_SetPosition(18, 0x1200000, 0xb20000);
        Actor_SetPosition(17, 0x6480000, 0x6480000);
        Actor_SetPosition(10, 0xe80000, 0x780000);
        Actor_SetPosition(12, 0x1580000, 0x780000);
        Actor_SetPosition(10, 0xe80000, 0x780000);
        Map_CopyCellsTo(0, 59, 15, 38, 4, 3);
        Actor_SetPosition(12, 0x1580000, 0x780000);
        Map_CopyCellsTo(4, 59, 17, 38, 4, 3);
        Map_CopyCellsTo(8, 60, 17, 39, 2, 2);
        Map_CopyCellAttributes(0, 1, 2, 1, 17, 7);
    } else if (GameFlag_IsSet(FLAG_LEFT_BEAM_SHINING) != 0
                && GameFlag_IsSet(FLAG_RIGHT_BEAM_SHINING) != 0) {
        Actor_SetPosition(10, 0xe80000, 0x780000);
        Actor_SetPosition(12, 0x1580000, 0x780000);
        Map_CopyCellsTo(0, 28, 17, 8, 2, 1);
        Actor_SetPosition(10, 0xe80000, 0x780000);
        Map_CopyCellsTo(0, 59, 15, 38, 4, 3);
        Actor_SetPosition(12, 0x1580000, 0x780000);
        Map_CopyCellsTo(4, 59, 17, 38, 4, 3);
        Map_CopyCellsTo(8, 60, 17, 39, 2, 2);
        Map_CopyCellAttributes(0, 0, 2, 1, 17, 8);
    } else {
        if (GameFlag_IsSet(FLAG_LEFT_BEAM_SHINING) != 0) {
            Actor_SetPosition(10, 0xe80000, 0x780000);
            Map_CopyCellsTo(0, 59, 15, 38, 4, 3);
        }
        if (GameFlag_IsSet(FLAG_RIGHT_BEAM_SHINING) != 0) {
            Actor_SetPosition(12, 0x1580000, 0x780000);
            Map_CopyCellsTo(4, 59, 17, 38, 4, 3);
        }
    }
    if (GameFlag_IsSet(0x80b) != 0) {
        Actor_SetPosition(9, 0x1f80000, 0x980000);
        Map_CopyCellsTo(2, 28, 34, 10, 2, 1);
        Map_CopyCellsTo(2, 30, 16, 10, 2, 1);
        Map_CopyCellsTo(0, 55, 32, 40, 4, 3);
    }
    if (GameFlag_IsSet(0x80c) != 0) {
        Actor_SetPosition(11, 0x2880000, 0x980000);
        Map_CopyCellsTo(4, 28, 36, 10, 2, 1);
        Map_CopyCellsTo(4, 30, 18, 10, 2, 1);
        Map_CopyCellsTo(4, 55, 36, 40, 4, 3);
    }
    if (GameFlag_IsSet(0x80d) != 0) {
        Actor_SetPosition(13, 0x1f80000, 0xc80000);
        Map_CopyCellsTo(2, 29, 34, 11, 2, 1);
        Map_CopyCellsTo(2, 31, 16, 11, 2, 1);
        Map_CopyCellsTo(0, 58, 32, 43, 4, 1);
    }
    if (GameFlag_IsSet(0x80e) != 0) {
        Actor_SetPosition(15, 0x2880000, 0xc80000);
        Map_CopyCellsTo(4, 29, 36, 11, 2, 1);
        Map_CopyCellsTo(4, 31, 18, 11, 2, 1);
        Map_CopyCellsTo(4, 58, 36, 43, 4, 1);
    }
    {
    s16 *state = (s16 *)gGameState;

    if (state[225] == 3) {
        if (GameFlag_IsSet(0x30a) != 0) {
            Actor_SetPosition(GERALD, 0, 0);
            Actor_SetPosition(JASMINE, 0, 0);
        } else if (GameFlag_IsSet(0x109) == 0) {
            FieldScene_SetupStagedActors();
            GameFlag_Set(0x30a);
        }
    }
    if (state[225] == 4) {
        if (GameFlag_IsSet(0x30b) != 0) {
            Actor_SetPosition(GERALD, 0, 0);
            Actor_SetPosition(JASMINE, 0, 0);
        } else if (GameFlag_IsSet(0x109) == 0) {
            FieldScene_RunStagedActorScene();
            GameFlag_Set(0x30b);
        }
    }
    }
    if (GameFlag_IsSet(0x814) != 0) {
        BattleFx_SetQueuedSoundAndPlay(141);
        Work_SetValuesIfNonNegative(0x10000, 0x10000, 0x10000);
        InitializeSceneRecordBuffer();
    }
    return 0;
}

void Scene_RunGuardSequenceC(void)
{
    u32 i;
    s32 value;
    s32 *p;
    s32 buf;

    p = (s32 *)SoruSekizo_SpriteRows;
    buf = (s32)Runtime_AllocateBlock(14, 0x400);
    Resource_DecodeByteLz((s32)SoruSekizo_SpriteRowsGfx, buf);
    value = Engine_VramLoad(Resource_FindFreeEntry(), 128, buf);
    for (i = 0; i < 9; i++) {
        s32 *q = p;

        *q++ = 0;
        *q++ = 0x40004000;
        p += 3;
        *q = value | 0xac00;
    }
    value = Engine_VramLoad(Resource_FindFreeEntry(), 128, buf + 128);
    for (i = 0; i < 9; i++) {
        s32 *q = p;

        *q++ = 0;
        *q++ = 0x40004000;
        p += 3;
        *q = value | 0xdc00;
    }
    value = Engine_VramLoad(Resource_FindFreeEntry(), 128, buf + 0x100);
    for (i = 0; i < 9; i++) {
        s32 *q = p;

        *q++ = 0;
        *q++ = 0x40004000;
        p += 3;
        *q = value | 0xc00;
    }
    Engine_HeapRelease(14);
    {
        s32 size = 0xc80;

        Engine_TaskAddCallback((s32)SceneEffect_UpdateScrollingSpriteRows, size);
    }
}

void FieldScene_CallWhenCheck9_31_9(void)
{
    if (SceneActor_IsActorAtTile(9, 31, 9) != 0) {
        SoruSekizo_OpenNorthwestSeal();
    }
}

void FieldScene_RunGuardedStep11(void)
{
    if (SceneActor_IsActorAtTile(11, 40, 9) != 0) {
        SoruSekizo_OpenNortheastSeal();
    }
}

void FieldScene_RunGuardedStep13(void)
{
    if (SceneActor_IsActorAtTile(13, 31, 12) != 0) {
        SoruSekizo_OpenSouthwestSeal();
    }
}

void FieldScene_RunGuardedStep15(void)
{
    if (SceneActor_IsActorAtTile(15, 40, 12) != 0) {
        SoruSekizo_OpenSoutheastSeal();
    }
}

void ConfigureSceneAndCheckActors(void)
{
    Field_SetMapCellCollision(2, 0x00d00000, 0x00700000, 0);
    if (SceneActor_IsActorAtTile(10, 14, 7) != 0) {
        Scene_ShineLeftBeam();
    }
}

void ConfigureAlternateSceneAndCheckActors(void)
{
    Field_SetMapCellCollision(2, 23068672, 7340032, 0);
    if (SceneActor_IsActorAtTile(12, 21, 7) != 0) {
        Scene_ShineRightBeam();
    }
}

void FieldScene_RunClosingSequence(void)
{
    struct FieldActor *first;
    s32 kind;
    struct FieldActor *second;

    first = Object_GetById(0);
    kind = first->x.fixed >> 20;
    second = Object_GetById(0);
    if ((second->z.fixed >> 20) == 8) {
        if ((u32)(kind - 17) <= 1) {
            Field_SetMapCellCollision(2, 0x1100000, 0x800000, 255);
            Field_SetMapCellCollision(2, 0x1200000, 0x800000, 255);
        }
    }
}

void SoruSekizo_BlockLeftTriggerCell(void)
{
    s32 x = Object_GetById(0)->x.fixed >> 20;
    s32 y = Object_GetById(0)->z.fixed >> 20;

    if (y == 7 && (u32)(x - 13) <= 1) {
        Field_SetMapCellCollision(2, 0xd00000, 0x700000, 255);
    }
}

void SoruSekizo_BlockRightTriggerCell(void)
{
    s32 x = Object_GetById(0)->x.fixed >> 20;
    s32 y = Object_GetById(0)->z.fixed >> 20;

    if (y == 7 && (u32)(x - 21) <= 1) {
        Field_SetMapCellCollision(2, 0x1600000, 0x700000, 255);
    }
}

void SoruSekizo_RunStatueDropScene(void)
{
    struct FieldActor *statue;
    struct FieldActor *leader;
    s32 z;

    statue = Object_GetById(17);
    Field_SetMapCellCollision(2, 0x1100000, 0x800000, 0);
    Field_SetMapCellCollision(2, 0x1200000, 0x800000, 0);
    if (statue != NULL) {
        z = statue->z.fixed;
        Engine_EventBegin();
        if ((z >> 20) == 8) {
            if (Engine_GameFlagIsSet(0x207) == 0) {
                leader = Object_GetById(0);
                if ((u32)(leader->z.fixed >> 19) <= 17) {
                    Actor_WalkToAndWait(0, 0x121, 158);
                    leader = Object_GetById(0);
                    leader->facing = FACING_NORTH;
                }
            }
            if (Engine_GameFlagIsSet(0x816) != 0 && Engine_GameFlagIsSet(0x817) != 0) {
                Engine_GameFlagSet(0x818);
                Camera_SetSpeed(0x20000, 0x4000);
                Camera_MoveTo(0x11e0000, -1, 0x920000, 1);
                Engine_CameraWaitForMove();
                Object_GetById(17)->steering_flags &= ~ACTOR_FACES_MOTION;
                Actor_SetSpeed(17, 0x30000, 0x10000);
                statue->motion_flags = 0;
                Engine_ActorSetSpritePriority(17, 3);
                Engine_AudioPlayCue(189);
                Engine_ActorSetDestination(17, 0x120, 178);
                Engine_EventWait(8);
                Actor_SetPosition(18, 0x1200000, 0xb20000);
                statue->target_x = ACTOR_NO_TARGET;
                statue->target_y = ACTOR_NO_TARGET;
                statue->target_z = ACTOR_NO_TARGET;
                statue->x.fixed = 0;
                statue->y.fixed = 0;
                statue->z.fixed = 0;
                statue->velocity_x = 0;
                statue->velocity_y = 0;
                statue->velocity_z = 0;
                Engine_ActorSetPosition(17, 0, 0);
                Work_SetValuesIfNonNegative(0x10000, 0x10000, 0x10000);
                Engine_EventWait(10);
                Engine_AudioPlayCue(141);
                Work_SetValuesIfNonNegative(0x30000, 0x30000, 0x10000);
                Engine_EventWait(10);
                Work_SetValuesIfNonNegative(0x50000, 0x50000, 0x10000);
                Engine_EventWait(35);
                Work_SetValuesIfNonNegative(0x40000, 0x40000, 0x10000);
                Engine_EventWait(20);
                Work_SetValuesIfNonNegative(0x30000, 0x30000, 0x10000);
                Engine_EventWait(30);
                Work_SetValuesIfNonNegative(0x20000, 0x20000, 0x10000);
                Engine_EventWait(40);
                Engine_AudioPlayCue(0x121);
                Work_SetValuesIfNonNegative(0x10000, 0x10000, 0x10000);
                Engine_EventWait(10);
                Work_SetValuesIfNonNegative(-1, -1, 0xe666);
                Engine_EventWait(60);
                Engine_AudioPlayCue(188);
                if (Engine_GameFlagIsSet(0x80b) != 0 && Engine_GameFlagIsSet(0x80c) != 0
                    && Engine_GameFlagIsSet(0x80d) != 0 && Engine_GameFlagIsSet(0x80e) != 0) {
                    Engine_GameFlagSet(0x80f);
                }
                Engine_EventWait(40);
                Engine_MessageShowCentered((s32)MsgSoruSomethingClicked, 1);
                Engine_MapCopyCellAttributes(0, 1, 2, 1, 17, 8);
                Engine_MapCopyCellAttributes(17, 9, 2, 1, 17, 7);
            }
        }
        Engine_EventEnd();
    }
}

void FieldScene_RunScene37bSequenceA(void)
{
    u32 i;
    struct FieldActor *record;

    record = Object_GetById(17);
    if (record != 0) {
        if ((record->z.fixed >> 20) == 8) {
            Engine_EventBegin();
            Audio_PlayCue(185);
            Actor_SetSpeed(17, 0x3333, 0x1999);
            Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x3333, 0x1999);
            Object_GetById(17)->steering_flags &= ~ACTOR_FACES_MOTION;
            Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 8);
            record = Object_GetById(0);
            Actor_SetDestination(ACTOR_PARTY_LEADER, record->x.part.pixel, 136);
            Actor_SetDestination(17, 0x120, 120);
            Engine_ActorWaitForMove(17);
            Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 1);
            Engine_EventEnd();
        }
    }
}

void FieldScene_RunFiveValueStep9(void)
{
    SoruSekizo_RunEventSequence(9, 31, 9, 30, 9);
    SoruSekizo_OpenNorthwestSeal();
}

void FieldScene_RunFiveValueStep11(void)
{
    SoruSekizo_RunEventSequence(11, 40, 9, 41, 9);
    SoruSekizo_OpenNortheastSeal();
}

void FieldScene_ApplyRect13_31_12_30_12(void)
{
    SoruSekizo_RunEventSequence(13, 31, 12, 30, 12);
    SoruSekizo_OpenSouthwestSeal();
}

void FieldScene_RunFiveValueStep15(void)
{
    SoruSekizo_RunEventSequence(15, 40, 12, 41, 12);
    SoruSekizo_OpenSoutheastSeal();
}

void FieldScene_ApplyRect10_14_7_13_7(void)
{
    SoruSekizo_RunEventSequence(10, 14, 7, 13, 7);
    Scene_ShineLeftBeam();
}

void SceneActor_UseActorTenCellAndNext(void)
{
    struct FieldActor *record = Object_GetById(10);
    s32 cell;

    if (record == 0) {
        return;
    }

    cell = record->z.fixed >> 20;
    SoruSekizo_RunEventSequence(10, 13, cell + 1, 13, cell);
}

void SceneActor_MoveActor10ByRow(void)
{
    s32 *p = Object_GetById(10);
    if (p != NULL) {
        s32 v = p[4] >> 20;
        SoruSekizo_RunEventSequence(10, 13, v - 1, 13, v);
    }
}

void FieldScene_ApplyRect12_21_7_22_7(void)
{
    SoruSekizo_RunEventSequence(12, 21, 7, 22, 7);
    Scene_ShineRightBeam();
}

void SceneActor_ApplyActorTwelveZCellPair(void)
{
    s32 *p = Object_GetById(12);
    if (p != NULL) {
        s32 v = p[4] >> 20;
        SoruSekizo_RunEventSequence(12, 22, v + 1, 22, v);
    }
}

void SceneActor_RunSlot12ColumnStep(void)
{
    s32 *p = Object_GetById(12);
    if (p != NULL) {
        s32 v = p[4] >> 20;
        SoruSekizo_RunEventSequence(12, 22, v - 1, 22, v);
    }
}

void SoruSekizo_RunEventSequence(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4)
{
    s32 p10;
    s32 p10b;
    s32 p10c;
    s32 p8;
    s32 p8b;
    s32 p9;
    s32 p9b;
    s32 record;

    p9 = a0;
    p8 = a1;
    p10 = a2;
    Engine_EventBegin();
    Engine_AudioPlayCue(185);
    Actor_SetSpeed(p9, 0x3333, 0x1999);
    Engine_ActorSetSpeed(0, 0x3333, 0x1999);
    Object_GetById(p9)->steering_flags &= ~ACTOR_FACES_MOTION;
    Engine_ActorSetAnimation(0, 8);
    Engine_ActorSetDestination(0, ((a3 << 4) + 8), ((a4 << 4) + 8));
    p8b = ((s32)p8 << 4);
    p10b = ((s32)p10 << 4);
    Engine_ActorSetDestination(p9, (p8b + 8), (p10b + 8));
    Engine_ActorWaitForMove(p9);
    Engine_ActorSetAnimation(0, 1);
    Engine_EventEnd();
    p9b = ((a3 << 4) + 8);
    p10c = ((a4 << 4) + 8);
}

s32 SceneActor_IsActorAtTile(s32 no, s32 x, s32 z)
{
    s32 *p = Object_GetById(no);
    if (p == NULL || (p[2] >> 20) != x) {
        return 0;
    }
    if ((p[4] >> 20) != z) {
        return 0;
    }
    return 1;
}

void SoruSekizo_OpenNorthwestSeal(void)
{
    s32 *p = (s32 *)&gSealScene;
    p[0] = 0;
    p[1] = 55;
    p[2] = 32;
    p[3] = 40;
    p[4] = 4;
    p[5] = 3;
    p[6] = 2;
    p[7] = 30;
    p[8] = 34;
    p[9] = 10;
    p[10] = 2;
    p[11] = 1;
    p[12] = 2;
    p[13] = 28;
    p[14] = 34;
    p[15] = 10;
    p[16] = 2;
    p[17] = 1;
    p[18] = 2;
    p[19] = 30;
    p[20] = 16;
    p[21] = 10;
    p[22] = 2;
    p[23] = 1;
    p[24] = 0x80b;
    p[25] = 0x4000;
    p[26] = 500;
    p[27] = 132;
    p[28] = 8;
    p[29] = 55;
    p[30] = 32;
    p[31] = 40;
    p[32] = 4;
    p[33] = 3;
    p[34] = 2;
    p[35] = 30;
    p[36] = 34;
    p[37] = 10;
    p[38] = 2;
    p[39] = 1;
    p[40] = 2;
    p[41] = 28;
    p[42] = 16;
    p[43] = 10;
    p[44] = 2;
    p[45] = 1;
    p[46] = 9;
    p[47] = 488;
    p[48] = 152;
    SoruSekizo_OpenSeal();
}

void SoruSekizo_OpenNortheastSeal(void)
{
    s32 *p = (s32 *)&gSealScene;
    p[0] = 4;
    p[1] = 55;
    p[2] = 36;
    p[3] = 40;
    p[4] = 4;
    p[5] = 3;
    p[6] = 4;
    p[7] = 30;
    p[8] = 36;
    p[9] = 10;
    p[10] = 2;
    p[11] = 1;
    p[12] = 4;
    p[13] = 28;
    p[14] = 36;
    p[15] = 10;
    p[16] = 2;
    p[17] = 1;
    p[18] = 4;
    p[19] = 30;
    p[20] = 18;
    p[21] = 10;
    p[22] = 2;
    p[23] = 1;
    p[24] = 0x80c;
    p[25] = 0x4000;
    p[26] = 654;
    p[27] = 132;
    p[28] = 12;
    p[29] = 55;
    p[30] = 36;
    p[31] = 40;
    p[32] = 4;
    p[33] = 3;
    p[34] = 4;
    p[35] = 30;
    p[36] = 36;
    p[37] = 10;
    p[38] = 2;
    p[39] = 1;
    p[40] = 4;
    p[41] = 28;
    p[42] = 18;
    p[43] = 10;
    p[44] = 2;
    p[45] = 1;
    p[46] = 11;
    p[47] = 664;
    p[48] = 152;
    SoruSekizo_OpenSeal();
}

void SoruSekizo_OpenSouthwestSeal(void)
{
    s32 *p = (s32 *)&gSealScene;
    p[0] = 0;
    p[1] = 58;
    p[2] = 32;
    p[3] = 43;
    p[4] = 4;
    p[5] = 1;
    p[6] = 2;
    p[7] = 31;
    p[8] = 34;
    p[9] = 11;
    p[10] = 2;
    p[11] = 1;
    p[12] = 2;
    p[13] = 29;
    p[14] = 34;
    p[15] = 11;
    p[16] = 2;
    p[17] = 1;
    p[18] = 2;
    p[19] = 31;
    p[20] = 16;
    p[21] = 11;
    p[22] = 2;
    p[23] = 1;
    p[24] = 0x80d;
    p[25] = 0xc000;
    p[26] = 500;
    p[27] = 216;
    p[28] = 8;
    p[29] = 58;
    p[30] = 32;
    p[31] = 43;
    p[32] = 4;
    p[33] = 1;
    p[34] = 2;
    p[35] = 31;
    p[36] = 34;
    p[37] = 11;
    p[38] = 2;
    p[39] = 1;
    p[40] = 2;
    p[41] = 29;
    p[42] = 16;
    p[43] = 11;
    p[44] = 2;
    p[45] = 1;
    p[46] = 13;
    p[47] = 488;
    p[48] = 200;
    SoruSekizo_OpenSeal();
}

void SoruSekizo_OpenSoutheastSeal(void)
{
    s32 *p = (s32 *)&gSealScene;
    p[0] = 4;
    p[1] = 58;
    p[2] = 36;
    p[3] = 43;
    p[4] = 4;
    p[5] = 1;
    p[6] = 4;
    p[7] = 31;
    p[8] = 36;
    p[9] = 11;
    p[10] = 2;
    p[11] = 1;
    p[12] = 4;
    p[13] = 29;
    p[14] = 36;
    p[15] = 11;
    p[16] = 2;
    p[17] = 1;
    p[18] = 4;
    p[19] = 31;
    p[20] = 18;
    p[21] = 11;
    p[22] = 2;
    p[23] = 1;
    p[24] = 0x80e;
    p[25] = 0xc000;
    p[26] = 0x28e;
    p[27] = 216;
    p[28] = 12;
    p[29] = 58;
    p[30] = 36;
    p[31] = 43;
    p[32] = 4;
    p[33] = 1;
    p[34] = 4;
    p[35] = 31;
    p[36] = 36;
    p[37] = 11;
    p[38] = 2;
    p[39] = 1;
    p[40] = 4;
    p[41] = 29;
    p[42] = 18;
    p[43] = 11;
    p[44] = 2;
    p[45] = 1;
    p[46] = 15;
    p[47] = 664;
    p[48] = 200;
    SoruSekizo_OpenSeal();
}
