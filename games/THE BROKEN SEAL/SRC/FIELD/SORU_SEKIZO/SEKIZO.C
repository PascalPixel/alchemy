#include "STATUE_HALL.H"
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "CALL.H"

extern u8 MsgSoruSukuretaYouFoundIt[];

enum StatueHallActor {
    ACTOR_SUKURETA = ACTOR_FIRST_PLACED
};

void Event_SayThenWait(s32 speaker, s32 frames);

extern u8 MsgSoruHmphWellTold[];
extern u8 MsgSoruHonestlyDoubtUnderstand[];
extern u8 MsgSoruTryFindSolution[];
extern u8 MsgSoruWait[];
void Event_SayThenWait();
void ObjectMotion_ResetAndSetPositionInMode2();
s32 Inventory_PromptAndSetObjectMode();
s32 UiText_OpenMessageAtObject();

/* A point the camera follows, in 16.16 fixed point. */
struct FocusPoint {
    s32 x;
    s32 y;
    s32 z;
};

/* The map work begins with the point the camera follows. */
struct FocusWork {
    struct FocusPoint *focus;
};

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

    if (GameFlag_IsSet(0x818) != 0) {
        if (GameFlag_IsSet(FLAG_STATUE_TRAP_SPRUNG) == 0) {
            no = 3;
            goto apply;
        }
        goto fail;
    }
    if (GameFlag_IsSet(0x812) == 0) {
        no = 4;
apply:
        Event_RequestExit(no);
        return 1;
    }
fail:
    return -1;
}

void Scene_OpenTheHole(void)
{
    s32 i;

    { s32 k5 = 2, k6 = 1; Map_CopyCellsTo(0, 28, 17, 8, k5, k6); }
    Audio_PlayCue(200);
    for (i = 0; i != 22; i++) {
        Map_CopyCellsTo(10, 61, 17, 40, 2, 1);
        Event_Wait(4);
        Map_CopyCellsTo(8, 61, 17, 40, 2, 1);
        Event_Wait(4);
    }
    { s32 k5 = 4, k6 = 3;
      Map_CopyCellsTo(0, 59, 15, 38, k5, k6);
      Map_CopyCellsTo(4, 59, 17, 38, k5, k6); }
    Map_CopyCellsTo(8, 60, 17, 39, 2, 2);
    { s32 k5 = 17, k6 = 8; Map_CopyCellAttributes(0, 0, 2, 1, k5, k6); }
    GameFlag_Set(0x207);
    SoruSekizo_RunStatueDropScene();
}

/*
 * The statue hall of Sol Sanctum, after Robin drops the statue into the
 * hole it opened. Sukureta comes down to look, Gerald and Jasmine tell him
 * what happened, and he decides the trap is disarmed before withdrawing to
 * the Luna room to watch from safety.
 */

/* Each line follows the one before; only the first is set. */
void FieldScene_SetupStagedActors(void)
{
    struct FieldActor *leader;

    Event_Begin();
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    gEventWork->transition_frames = 32;
    Event_OpenScreen();
    Event_WaitForScreen();
    Event_Wait(20);
    Actor_SetPosition(ACTOR_SUKURETA, 0x2400000, 0xe80000);
    Event_Wait(1);
    Event_SetMessage((s32)MsgSoruSukuretaYouFoundIt);
    Event_SayThenWait(ACTOR_SUKURETA, 6);
    Actor_SetPosition(ACTOR_SUKURETA, 0x2400000, 0x1180000);
    Camera_SetSpeed(0xcccc, 0x1999);
    Camera_MoveTo(0x23e0000, -1, 0xb40000, 1);
    Actor_SetSpeed(ACTOR_SUKURETA, 0x10000, 0x8000);
    Actor_WalkToAndWait(ACTOR_SUKURETA, 0x240, 216);
    Event_Wait(20);
    Actor_Jump(ACTOR_JASMINE, 2, 0);
    Event_Wait(30);
    Event_SayThenWait(ACTOR_JASMINE, 6);
    Actor_RunRepeatedMotion(ACTOR_SUKURETA, 2);
    Event_Wait(6);
    Actor_FaceDirection(ACTOR_SUKURETA, FACING_WEST + FACING_STEP, 0);
    Event_Wait(10);
    Camera_SetSpeed(0x59999, 0xb333);
    Camera_MoveTo(0x11f0000, -1, 0xb00000, 1);
    Camera_WaitForMove();
    Event_Wait(60);
    Camera_MoveTo(0x23e0000, -1, 0xb40000, 1);
    Camera_WaitForMove();
    Event_Wait(20);
    Actor_SetAnimationAndWait(ACTOR_SUKURETA, ANIM_NOD);
    Event_Wait(10);
    Actor_FaceDirection(ACTOR_SUKURETA, FACING_NORTH, 0);
    Event_Wait(10);
    Actor_Jump(ACTOR_SUKURETA, 6, 0);
    Actor_SetSpeed(ACTOR_SUKURETA, 0x30000, 0x20000);
    Actor_WalkToAndWait(ACTOR_SUKURETA, 0x240, 184);
    Event_Wait(40);
    Event_SayThenWait(ACTOR_SUKURETA, 6);
    Actor_FaceDirection(ACTOR_SUKURETA, FACING_WEST, 0);
    Event_Wait(40);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 2);
    gEventWork->transition_frames = 16;
    Event_CloseScreen();
    Event_WaitForScreen();
    Camera_MoveTo(0x11f0000, -1, 0xb00000, 0);
    Map_Redraw();
    Task_Wait(1);
    Event_OpenScreen();
    Event_WaitForScreen();
    Event_Wait(40);
    Actor_SetPosition(ACTOR_SUKURETA, 0x1a80000, 0xc80000);
    Task_Wait(1);
    Event_SayThenWait(ACTOR_SUKURETA, 20);
    Event_CloseScreen();
    Event_WaitForScreen();
    Actor_SetPosition(ACTOR_SUKURETA, 0x2400000, 0xb80000);
    Camera_MoveTo(0x23e0000, -1, 0xb40000, 0);
    Map_Redraw();
    Task_Wait(1);
    Event_OpenScreen();
    Event_WaitForScreen();
    Event_Wait(20);
    Camera_SetSpeed(0x13333, 0x2666);
    Camera_MoveTo(0x23e0000, -1, 0x9d0000, 1);
    Camera_WaitForMove();
    Event_Wait(20);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 2);
    Event_SayThenWait(ACTOR_GERALD, 20);
    Actor_SetAnimationAndWait(ACTOR_JASMINE, ANIM_NOD);
    Event_SayThenWait(ACTOR_JASMINE, 6);
    Actor_RunRepeatedMotion(ACTOR_SUKURETA, 2);
    Event_Wait(40);
    Actor_SetAnimationAndWait(ACTOR_SUKURETA, ANIM_SHAKE_HEAD);
    Event_SayThenWait(ACTOR_SUKURETA, 80);
    Actor_ShowEmote(ACTOR_SUKURETA, EMOTE_IN_FRONT | 2, 0);
    Event_Wait(60);
    Actor_FaceDirection(ACTOR_SUKURETA, FACING_NORTH, 0);
    Event_Wait(30);
    Event_SayThenWait(ACTOR_SUKURETA, 20);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 1, 0);
    Actor_ShowEmote(ACTOR_GERALD, EMOTE_IN_FRONT | 1, 0);
    Actor_ShowEmote(ACTOR_JASMINE, EMOTE_IN_FRONT | 1, 0);
    Event_Wait(60);
    Actor_FaceDirection(ACTOR_SUKURETA, FACING_NORTH, 0);
    Event_Wait(30);
    Actor_SetAnimationAndWait(ACTOR_SUKURETA, ANIM_SHAKE_HEAD);
    Event_SayThenWait(ACTOR_SUKURETA, 6);
    Actor_FaceEachOther(ACTOR_PARTY_LEADER, ACTOR_GERALD, 0);
    Event_Wait(40);
    Actor_FaceEachOther(ACTOR_PARTY_LEADER, ACTOR_JASMINE, 0);
    Event_Wait(40);
    Actor_SetAnimationAndWait(ACTOR_SUKURETA, ANIM_NOD);
    Event_SayThenWait(ACTOR_SUKURETA, 6);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
    Actor_FaceDirection(ACTOR_JASMINE, FACING_SOUTH, 0);
    Actor_FaceDirection(ACTOR_GERALD, FACING_SOUTH, 0);
    Event_Wait(40);
    Actor_SetAnimation(ACTOR_GERALD, ANIM_NOD);
    Actor_SetAnimation(ACTOR_JASMINE, ANIM_NOD);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
    Event_Wait(20);
    Actor_SetAnimationAndWait(ACTOR_SUKURETA, ANIM_NOD);
    Event_SayThenWait(ACTOR_SUKURETA, 6);
    Camera_SetSpeed(0x19999, 0x3333);
    Camera_MoveTo(0x2400000, -1, 0xd70000, 1);
    Actor_FaceDirection(ACTOR_SUKURETA, FACING_SOUTH, 0);
    Event_Wait(10);
    Actor_Jump(ACTOR_SUKURETA, 6, 0);
    Actor_WalkToAndWait(ACTOR_SUKURETA, 0x240, 217);
    Event_Wait(20);
    Actor_WalkToAndWait(ACTOR_SUKURETA, 0x240, 0x141);
    Actor_SetPosition(ACTOR_SUKURETA, 0, 0);
    Camera_SetSpeed(0x39999, 0x7333);
    Camera_MoveTo(0x2400000, -1, 0x880000, 1);
    Camera_WaitForMove();
    Event_Wait(20);
    Actor_SetSpeed(ACTOR_GERALD, 0x10000, 0x8000);
    Actor_SetSpeed(ACTOR_JASMINE, 0x10000, 0x8000);
    Actor_SetAnimation(ACTOR_GERALD, ANIM_WALK);
    leader = Actor_Get(ACTOR_PARTY_LEADER);
    if (leader != NULL) {
        Actor_SetDestination(ACTOR_GERALD, leader->x.part.pixel, leader->z.part.pixel);
    }
    Actor_WaitForMove(ACTOR_GERALD);
    Actor_SetPosition(ACTOR_GERALD, 0, 0);
    Actor_SetAnimation(ACTOR_JASMINE, ANIM_WALK);
    leader = Actor_Get(ACTOR_PARTY_LEADER);
    if (leader != NULL) {
        Actor_SetDestination(ACTOR_JASMINE, leader->x.part.pixel, leader->z.part.pixel);
    }
    Actor_WaitForMove(ACTOR_JASMINE);
    Actor_SetPosition(ACTOR_JASMINE, 0, 0);
    GameFlag_Clear(FLAG_ARRIVAL_EVENT_PENDING);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
    gEventWork->transition_frames = 16;
    Event_End();
}

/* Staged scene for actors 8, 5, 1 and 0. The shared work pointer is fetched
 * again at the tail after the intervening calls. Runtime veneer bindings
 * belong to this module's translation-unit declaration. */
void FieldScene_RunStagedActorScene(void)
{
    u8 *work;
    s32 record;

    Engine_EventBegin();
    work = (u8 *)gEventWork;
    *(s32 *)(work + 0x1c0) = 0x100;
    *(s32 *)(work + 0x1c8) = 32;
    Event_OpenScreen();
    Engine_EventWaitForScreen();
    Engine_EventWait(20);
    Engine_ActorSetPosition(8, 0x2400000, 0x1280000);
    Engine_EventWait(1);
    Engine_EventSetMessage((s32)MsgSoruWait);
    Event_SayThenWait(8, 6);
    Camera_SetSpeed(0xcccc, 0x1999);
    Call4(Engine_CameraMoveTo, 0x23e0000, -1, 0xb40000, 1);
    Call3(Engine_ActorSetSpeed, 8, 0x10000, 0x8000);
    Call3(Engine_ActorWalkToAndWait, 8, 0x240, 216);
    Engine_EventWait(20);
    Engine_ActorJump(5, 2, 0);
    Engine_EventWait(30);
    Event_SayThenWait(5, 6);
    Actor_RunRepeatedMotion(8, 2);
    Event_Wait(6);
    Engine_ActorFaceDirection(8, 0x9000, 0);
    Engine_EventWait(10);
    Engine_CameraSetSpeed(0x59999, 0xb333);
    Camera_MoveTo(0x11f0000, -1, 0xb00000, 1);
    Engine_CameraWaitForMove();
    Engine_EventWait(60);
    Call4(Engine_CameraMoveTo, 0x23e0000, -1, 0xb40000, 1);
    Engine_CameraWaitForMove();
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(8, 3);
    Engine_EventWait(10);
    Actor_FaceDirection(8, 0xc000, 0);
    Event_Wait(10);
    Engine_ActorJump(8, 6, 0);
    Call3(Engine_ActorSetSpeed, 8, 0x30000, 0x20000);
    Call3(Engine_ActorWalkToAndWait, 8, 0x240, 184);
    Engine_EventWait(40);
    Engine_ActorSetAnimationAndWait(8, 3);
    Event_SayThenWait(8, 6);
    Actor_SetAnimation(ACTOR_GERALD, 3);
    Engine_ActorSetAnimation(5, 3);
    Engine_ActorSetAnimationAndWait(0, 3);
    Engine_EventWait(40);
    Engine_ActorRunRepeatedMotion(8, 3);
    Event_SayThenWait(8, 6);
    Engine_ActorJump(0, 2, 0);
    Engine_ActorJump(1, 2, 0);
    Engine_ActorJump(5, 2, 0);
    Event_Wait(30);
    Engine_ActorRunRepeatedMotion(1, 2);
    Event_SayThenWait(1, 6);
    Engine_ActorRunRepeatedMotion(8, 1);
    Engine_ActorSetAnimationAndWait(8, 4);
    Event_SayThenWait(8, 6);
    Actor_FaceEachOther(ACTOR_PARTY_LEADER, ACTOR_JASMINE, 0);
    Engine_EventWait(40);
    Engine_ActorFaceActor(8, 0, 0);
    Actor_FaceActor(8, ACTOR_JASMINE, 0);
    Engine_EventWait(40);
    Actor_Jump(8, 6, 0);
    Call3(Engine_ActorWalkToAndWait, 8, 0x240, 216);
    Call3(Engine_ActorFaceDirection, 8, 0x8000, 0);
    Engine_EventWait(10);
    Call3(Engine_ActorSetSpeed, 8, 0x13333, 0x9999);
    ObjectMotion_ResetAndSetPositionInMode2(8, 0x1b0, 200);
    Engine_EventWait(20);
    Camera_SetSpeed(0x26666, 0x4ccc);
    Call4(Engine_CameraMoveTo, 0x1200000, -1, 0xab0000, 1);
    Engine_CameraWaitForMove();
    Event_Wait(80);
    Engine_ActorSetAnimation(8, 1);
    Call4(Engine_CameraMoveTo, 0x23e0000, -1, 0xb40000, 1);
    Engine_EventWait(20);
    Engine_ActorFaceDirection(8, 0, 0);
    Engine_EventWait(30);
    Call3(Engine_ActorWalkToAndWait, 8, 0x240, 216);
    Call3(Engine_ActorFaceDirection, 8, 0xc000, 0);
    Engine_EventWait(10);
    Call3(Engine_ActorFaceDirection, 0, 0x4000, 0);
    Call3(Engine_ActorFaceDirection, 5, 0x4000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0x4000, 0);
    Engine_CameraSetSpeed(0xcccc, 0x1999);
    Engine_CameraMoveTo(0x23e0000, -1, 0xab0000, 1);
    Engine_ActorJump(8, 6, 0);
    Call3(Engine_ActorSetSpeed, 8, 0x30000, 0x20000);
    Engine_ActorWalkToAndWait(8, 0x240, 184);
    Engine_EventWait(80);
    Event_SayThenWait(8, 6);
    Engine_ActorSetAnimationAndWait(8, 4);
    Event_SayThenWait(8, 20);
    Engine_ActorShowEmote(5, 0x102, 0);
    Engine_EventWait(40);
    Actor_RunRepeatedMotion(ACTOR_JASMINE, 2);
    Event_SayThenWait(5, 6);
    Actor_SetAnimationAndWait(8, 3);
    Engine_ActorFaceDirection(8, 0x8000, 0);
    Engine_EventWait(40);
    Engine_ActorFaceDirection(8, 0xc000, 0);
    Event_Wait(30);
    Event_SayThenWait(8, 6);
    Call3(Engine_ActorShowEmote, 0, 0x101, 0);
    Call3(Engine_ActorShowEmote, 1, 0x101, 0);
    Actor_ShowEmote(ACTOR_JASMINE, 0x101, 0);
    Engine_EventWait(60);
    Engine_ActorSetAnimationAndWait(8, 4);
    Event_SayThenWait(8, 6);
    Engine_ActorFaceEachOther(1, 0, 0);
    Engine_EventWait(40);
    Actor_FaceEachOther(ACTOR_JASMINE, ACTOR_PARTY_LEADER, 0);
    Engine_EventWait(40);
    UiText_OpenMessageAtObject(8, 0);
    Call3(Engine_ActorFaceDirection, 0, 0x4000, 0);
    Call3(Engine_ActorFaceDirection, 5, 0x4000, 0);
    Call3(Engine_ActorFaceDirection, 1, 0x4000, 0);
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
    Call3(Engine_ActorWalkToAndWait, 8, 0x240, 216);
    Engine_EventWait(40);
    Engine_CameraSetSpeed(0xcccc, 0x1999);
    Call4(Engine_CameraMoveTo, 0x23e0000, -1, 0xbf0000, 1);
    Call3(Engine_ActorSetSpeed, 8, 0x10000, 0x8000);
    Engine_ActorWalkToAndWait(8, 0x240, 232);
    Engine_EventWait(40);
    Engine_ActorRunRepeatedMotion(8, 2);
    Engine_EventWait(40);
    Event_SayThenWait(8, 6);
    Engine_ActorFaceDirection(8, 0xc000, 0);
    Engine_EventWait(30);
    Event_SayThenWait(8, 6);
    Engine_ActorSetAnimationAndWait(8, 3);
    Call4(Engine_CameraMoveTo, 0x2400000, -1, 0xd70000, 1);
    Call3(Engine_ActorWalkToAndWait, 8, 0x23e, 0x143);
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
    Call3(Engine_ActorSetSpeed, 1, 0x10000, 0x8000);
    Call3(Engine_ActorSetSpeed, 5, 0x10000, 0x8000);
    Actor_SetAnimation(ACTOR_JASMINE, 2);
    record = Engine_ActorGet(0);
    if (record != 0) {
        Engine_ActorSetDestination(5, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Engine_ActorWaitForMove(5);
    Engine_ActorSetPosition(5, 0, 0);
    Engine_ActorSetAnimation(1, 2);
    record = Engine_ActorGet(0);
    if (record != 0) {
        Engine_ActorSetDestination(1, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Engine_ActorWaitForMove(1);
    Engine_ActorSetPosition(1, 0, 0);
    GameFlag_Clear(FLAG_ARRIVAL_EVENT_PENDING);
    work = (u8 *)gEventWork;
    *(s32 *)(work + 0x1c0) = 0x204;
    *(s32 *)(work + 0x1c8) = 16;
    Engine_EventEnd();
}

/* After the seal opens: the leader turns toward the seal from whichever side
 * of it he stands, the camera slides thirty pixels toward it, the seal's
 * cells pulse faster and faster, the opened seal is drawn, and the camera
 * slides back. */
void SoruSekizo_RunSealOpenedSequence(void)
{
    struct FocusWork *work;
    struct FieldActor *leader;
    struct FocusPoint *saved;
    struct FocusPoint point;
    s32 side;
    s32 i;

    work = ((struct FocusWork *)gMapWork);
    leader = Engine_ActorGet(0);
    if (leader->z.fixed < 0xb30000) {
        Actor_WalkToAndWait(0, 0x23f, 132);
        Actor_FaceDirection(0, 0x4000, 0);
        Event_Wait(30);
        saved = work->focus;
        point.x = leader->x.fixed;
        point.y = leader->y.fixed;
        point.z = leader->z.fixed;
        work->focus = &point;
        for (i = 0; i != 30; i++) {
            point.z += 0x10000;
            Event_Wait(1);
        }
        Event_Wait(40);
        side = 1;
    } else {
        Actor_WalkToAndWait(0, 0x241, 222);
        Actor_FaceDirection(0, 0xc000, 0);
        Event_Wait(30);
        point.x = leader->x.fixed;
        point.y = leader->y.fixed;
        point.z = leader->z.fixed;
        saved = work->focus;
        work->focus = &point;
        for (i = 0; i != 30; i++) {
            point.z -= 0x10000;
            Event_Wait(1);
        }
        Event_Wait(40);
        side = 2;
    }
    for (i = 0; i != 6; i++) {
        Map_CopyCellsTo(2, 28, 34, 10, 4, 2);
        Event_Wait(8);
        Map_CopyCellsTo(2, 30, 34, 10, 4, 2);
        Event_Wait(8);
    }
    for (i = 0; i != 10; i++) {
        Map_CopyCellsTo(2, 28, 34, 10, 4, 2);
        Event_Wait(4);
        Map_CopyCellsTo(2, 30, 34, 10, 4, 2);
        Event_Wait(4);
    }
    for (i = 0; i != 12; i++) {
        Map_CopyCellsTo(2, 28, 34, 10, 4, 2);
        Event_Wait(2);
        Map_CopyCellsTo(2, 30, 34, 10, 4, 2);
        Event_Wait(2);
    }
    Map_CopyCellsTo(2, 28, 34, 10, 4, 2);
    Map_CopyCellsTo(8, 55, 32, 40, 8, 4);
    Event_Wait(60);
    if (side == 1) {
        for (i = 0; i != 30; i++) {
            point.z -= 0x10000;
            Event_Wait(1);
        }
    } else if (side == 2) {
        for (i = 0; i != 30; i++) {
            point.z += 0x10000;
            Event_Wait(1);
        }
    }
    work->focus = saved;
}
