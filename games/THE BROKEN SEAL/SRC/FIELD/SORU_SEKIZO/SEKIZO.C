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

/* The scrolling sprite rows: three rows of nine, after the seal scene. */
struct Ent SoruSekizo_SpriteRows[27];

void SetMapCellCollision();
extern u8 MsgSoruSomethingClicked[];
void Engine_EventBegin();
s32 Engine_GameFlagIsSet();
void Engine_ActorWalkToAndWait();
void Engine_CameraSetSpeed();
void Engine_CameraMoveTo();
void Engine_CameraWaitForMove();
void Engine_ActorSetSpritePriority();
void Engine_AudioPlayCue();
void Engine_ActorSetDestination();
void Engine_ActorSetPosition();
void Engine_WorkSetValuesIfNonNegative();
void Engine_MessageShowCentered();
void Engine_MapCopyCellAttributes();
void Engine_EventEnd();
void Engine_ActorSetAnimation();
void Engine_ActorWaitForMove();

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
    record = Object_GetById(0);
    if (record != 0) {
        Engine_ActorSetDestination(5, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Engine_ActorWaitForMove(5);
    Engine_ActorSetPosition(5, 0, 0);
    Engine_ActorSetAnimation(1, 2);
    record = Object_GetById(0);
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
    leader = Object_GetById(0);
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

void SceneEffect_UpdateScrollingSpriteRows(void)
{
    s32 *cp = &gMapWork->x;
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
                e->f04 = sy + 8;
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
        Leader_CheckAhead();
    }
}

void SceneState_RunWhenActorZeroFacing4000(void)
{
    u16 *p = Object_GetById(0);
    if (p[3] == 0x4000) {
        Leader_CheckAhead();
    }
}

/* If the code-2059 check passes, runs a short setup/configuration sequence
 * for id 9: two no-argument calls bracket a select call and two calls each
 * taking a pair of numeric arguments. */
void FieldScene_RunPrimarySequenceHead(void)
{
    if (GameFlag_IsSet(GATE_CODE) == 0) {
        Event_Begin();
        ((void (*)())Object_GetById)(TARGET_ID);
        Actor_SetSpeed(TARGET_ID, 13107, 0x00001999); /* object_id, speed_limit, acceleration */
        Actor_WalkToAndWait(TARGET_ID, 504, 152); /* object_id, x=504, z=152 */
        Event_End();
    }
}

s32 Scene_RunGuardSequenceB(void)
{
    u8 *record;

    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
    Scene_RunGuardSequenceC();
    GameFlag_Set(0x144);
    record = (u8 *)Object_GetById(18);
    record[89] = 0;
    {
        u8 flags = record[35] | 2;

        record[35] = flags;
    }
    Actor_SetSpriteFlags((s32)Object_GetById(18), 0);
    *(u8 *)((s32)Object_GetById(18) + 35) &= 254;
    Actor_SetSpritePriority(18, 1);
    if ((u32)((gCell[225][0] - 3) << 16) > 0x10000) {
        Actor_SetPosition(ACTOR_JASMINE, 0, 0);
        Actor_SetPosition(ACTOR_GERALD, 0, 0);
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
    s16 *state = (s16 *)gCell;

    if (state[225] == 3) {
        if (GameFlag_IsSet(0x30a) != 0) {
            Actor_SetPosition(ACTOR_GERALD, 0, 0);
            Actor_SetPosition(ACTOR_JASMINE, 0, 0);
        } else if (GameFlag_IsSet(0x109) == 0) {
            FieldScene_SetupStagedActors();
            GameFlag_Set(0x30a);
        }
    }
    if (state[225] == 4) {
        if (GameFlag_IsSet(0x30b) != 0) {
            Actor_SetPosition(ACTOR_GERALD, 0, 0);
            Actor_SetPosition(ACTOR_JASMINE, 0, 0);
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
    buf = Runtime_AllocateBlock(14, 0x400);
    Resource_DecodeByteLz((s32)SoruSekizo_SpriteRowsGfx, buf);
    value = Vram_Load(Resource_FindFreeEntry(), 128, buf);
    for (i = 0; i < 9; i++) {
        s32 *q = p;

        *q++ = 0;
        *q++ = 0x40004000;
        p += 3;
        *q = value | 0xac00;
    }
    value = Vram_Load(Resource_FindFreeEntry(), 128, buf + 128);
    for (i = 0; i < 9; i++) {
        s32 *q = p;

        *q++ = 0;
        *q++ = 0x40004000;
        p += 3;
        *q = value | 0xdc00;
    }
    value = Vram_Load(Resource_FindFreeEntry(), 128, buf + 0x100);
    for (i = 0; i < 9; i++) {
        s32 *q = p;

        *q++ = 0;
        *q++ = 0x40004000;
        p += 3;
        *q = value | 0xc00;
    }
    Heap_Release(14);
    {
        s32 size = 0xc80;

        Engine_TaskAddCallback((s32)SceneEffect_UpdateScrollingSpriteRows, size);
    }
}

void FieldScene_CallWhenCheck9_31_9(void)
{
    if (SceneActor_IsActorAtTile(9, 31, 9) != 0) {
        SceneData_InitTableA980();
    }
}

void FieldScene_RunGuardedStep11(void)
{
    if (SceneActor_IsActorAtTile(11, 40, 9) != 0) {
        SceneData_FillTableA980();
    }
}

void FieldScene_RunGuardedStep13(void)
{
    if (SceneActor_IsActorAtTile(13, 31, 12) != 0) {
        SceneData_InitTableA980AndRunB();
    }
}

void FieldScene_RunGuardedStep15(void)
{
    if (SceneActor_IsActorAtTile(15, 40, 12) != 0) {
        SceneData_BuildTableA980();
    }
}

void ConfigureSceneAndCheckActors(void)
{
    ConfigureScene(2, 0x00d00000, 0x00700000, 0);
    if (SceneActor_IsActorAtTile(10, 14, 7) != 0) {
        Scene_ShineLeftBeam();
    }
}

void ConfigureAlternateSceneAndCheckActors(void)
{
    ConfigureScene_02003a30(2, 23068672, 7340032, 0);
    if (SceneActor_IsActorAtTile(12, 21, 7) != 0) {
        Scene_ShineRightBeam();
    }
}

void FieldScene_RunClosingSequence(void)
{
    s32 first;
    s32 kind;
    s32 second;

    first = Object_GetById(0);
    kind = *(s32 *)(first + 8) >> 20;
    second = Object_GetById(0);
    if ((*(s32 *)(second + 16) >> 20) == 8) {
        if ((u32)(kind - 17) <= 1) {
            Call4(SetMapCellCollision, 2, 0x1100000, 0x800000, 255);
            Call4(SetMapCellCollision, 2, 0x1200000, 0x800000, 255);
        }
    }
}

void SoruSekizo_CheckTileTrigger0166C(void)
{
    s32 x = *(s32 *)((s32)Object_GetById(0) + 8) >> 20;
    s32 y = *(s32 *)((s32)Object_GetById(0) + 16) >> 20;

    if (y == 7 && (u32)(x - 13) <= 1) {
        Call4(SetMapCellCollision, 2, 0xd00000, 0x700000, 255);
    }
}

void SoruSekizo_CheckTileTrigger016A4(void)
{
    s32 x = *(s32 *)((s32)Object_GetById(0) + 8) >> 20;
    s32 y = *(s32 *)((s32)Object_GetById(0) + 16) >> 20;

    if (y == 7 && (u32)(x - 21) <= 1) {
        Call4(SetMapCellCollision, 2, 0x1600000, 0x700000, 255);
    }
}

void SoruSekizo_RunStatueDropScene(void)
{
    u32 i;
    u8 *rec8;
    s32 record;
    u8 *p5;
    s32 zero;

    rec8 = (u8 *)Object_GetById(17);
    Call4(SetMapCellCollision, 2, 0x1100000, 0x800000, 0);
    Call4(SetMapCellCollision, 2, 0x1200000, 0x800000, 0);
    if ((s32)rec8 == 0) {
    } else {
        p5 = *(s32 *)((s32)rec8 + 16);
        Engine_EventBegin();
        if (((s32)p5 >> 20) != 8) {
        } else {
            if (Engine_GameFlagIsSet(0x207) == 0) {
                record = (u8 *)Object_GetById(0);
                if ((u32)(*(s32 *)(record + 16) >> 19) <= 17) {
                    Call3(Engine_ActorWalkToAndWait, 0, 0x121, 158);
                    record = (u8 *)Object_GetById(0);
                    {
                        s32 shown = 0xc000;
                    
                        *(u16 *)(record + 6) = shown;
                    }
                }
            }
            if (Engine_GameFlagIsSet(0x816) == 0) {
            } else {
                if (Engine_GameFlagIsSet(0x817) == 0) {
                } else {
                    Engine_GameFlagSet(0x818);
                    Call2(Engine_CameraSetSpeed, 0x20000, 0x4000);
                    Call4(Engine_CameraMoveTo, 0x11e0000, -1, 0x920000, 1);
                    Engine_CameraWaitForMove();
                    *(u8 *)((u8 *)Object_GetById(17) + 90) &= 254;
                    Value3(Engine_ActorSetSpeed, 17, 0x30000, 0x10000);
                    zero = 0;
                    rec8[85] = zero;
                    Engine_ActorSetSpritePriority(17, 3);
                    Engine_AudioPlayCue(189);
                    Engine_ActorSetDestination(17, 0x120, 178);
                    ((void (*)())Engine_EventWait)(8);
                    Call3(Engine_ActorSetPosition, 18, 0x1200000, 0xb20000);
                    *(s32 *)((s32)rec8 + 56) = -0x80000000;
                    *(s32 *)((s32)rec8 + 60) = -0x80000000;
                    *(s32 *)((s32)rec8 + 64) = -0x80000000;
                    *(s32 *)((s32)rec8 + 8) = zero;
                    *(s32 *)((s32)rec8 + 12) = zero;
                    *(s32 *)((s32)rec8 + 16) = zero;
                    *(s32 *)((s32)rec8 + 36) = zero;
                    *(s32 *)((s32)rec8 + 40) = zero;
                    *(s32 *)((s32)rec8 + 44) = zero;
                    Engine_ActorSetPosition(17, 0, 0);
                    Call3(Engine_WorkSetValuesIfNonNegative, 0x10000, 0x10000, 0x10000);
                    Engine_EventWait(10);
                    Engine_AudioPlayCue(141);
                    Call3(Engine_WorkSetValuesIfNonNegative, 0x30000, 0x30000, 0x10000);
                    Engine_EventWait(10);
                    Call3(Engine_WorkSetValuesIfNonNegative, 0x50000, 0x50000, 0x10000);
                    Engine_EventWait(35);
                    Call3(Engine_WorkSetValuesIfNonNegative, 0x40000, 0x40000, 0x10000);
                    Engine_EventWait(20);
                    Call3(Engine_WorkSetValuesIfNonNegative, 0x30000, 0x30000, 0x10000);
                    Engine_EventWait(30);
                    Call3(Engine_WorkSetValuesIfNonNegative, 0x20000, 0x20000, 0x10000);
                    Engine_EventWait(40);
                    Engine_AudioPlayCue(0x121);
                    Call3(Engine_WorkSetValuesIfNonNegative, 0x10000, 0x10000, 0x10000);
                    Engine_EventWait(10);
                    Call3(Engine_WorkSetValuesIfNonNegative, -1, -1, 0xe666);
                    Engine_EventWait(60);
                    Engine_AudioPlayCue(188);
                    if (Engine_GameFlagIsSet(0x80b) != 0) {
                        if (Engine_GameFlagIsSet(0x80c) != 0) {
                            if (Engine_GameFlagIsSet(0x80d) != 0) {
                                if (Engine_GameFlagIsSet(0x80e) != 0) {
                                    Engine_GameFlagSet(0x80f);
                                }
                            }
                        }
                    }
                    Engine_EventWait(40);
                    Engine_MessageShowCentered((s32)MsgSoruSomethingClicked, 1);
                    Engine_MapCopyCellAttributes(0, 1, 2, 1, 17, 8);
                    Engine_MapCopyCellAttributes(17, 9, 2, 1, 17, 7);
                }
            }
        }
        Engine_EventEnd();
    }
}

void FieldScene_RunScene37bSequenceA(void)
{
    u32 i;
    s32 record;

    record = Object_GetById(17);
    if (record != 0) {
        if ((*(s32 *)(record + 16) >> 20) == 8) {
            Event_Begin();
            Audio_PlayCue(185);
            Actor_SetSpeed(17, 0x3333, 0x1999);
            Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x3333, 0x1999);
            *(u8 *)((s32)Object_GetById(17) + 90) &= 254;
            Actor_SetAnimation(ACTOR_PARTY_LEADER, 8);
            record = Object_GetById(0);
            Actor_SetDestination(ACTOR_PARTY_LEADER, *(s16 *)(record + 10), 136);
            Actor_SetDestination(17, 0x120, 120);
            Actor_WaitForMove(17);
            Actor_SetAnimation(ACTOR_PARTY_LEADER, 1);
            Event_End();
        }
    }
}

void FieldScene_RunFiveValueStep9(void)
{
    SoruSekizo_RunEventSequence(9, 31, 9, 30, 9);
    SceneData_InitTableA980();
}

void FieldScene_RunFiveValueStep11(void)
{
    SoruSekizo_RunEventSequence(11, 40, 9, 41, 9);
    SceneData_FillTableA980();
}

void FieldScene_ApplyRect13_31_12_30_12(void)
{
    SoruSekizo_RunEventSequence(13, 31, 12, 30, 12);
    SceneData_InitTableA980AndRunB();
}

void FieldScene_RunFiveValueStep15(void)
{
    SoruSekizo_RunEventSequence(15, 40, 12, 41, 12);
    SceneData_BuildTableA980();
}

void FieldScene_ApplyRect10_14_7_13_7(void)
{
    SoruSekizo_RunEventSequence(10, 14, 7, 13, 7);
    Scene_ShineLeftBeam();
}

/*
 * Fetches scene record 10 and, when it exists, hands a coarse coordinate
 * derived from it to a five-argument routine, which receives both the
 * coordinate and the coordinate plus one; the fifth argument travels on the
 * stack. The `>> 20` reduction to a cell index is by analogy with the rest of
 * the tree and is not verified, and the repeated 13 is as written.
 */
void SceneActor_UseActorTenCellAndNext(void)
{
    u8 *record = Object_GetById(10);
    s32 cell;

    if (record == 0) {
        return;
    }

    cell = *(s32 *)(record + 16) >> 20;
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
    Call3(Engine_ActorSetSpeed, p9, 0x3333, 0x1999);
    Engine_ActorSetSpeed(0, 0x3333, 0x1999);
    *(u8 *)((s32)Object_GetById(p9) + 90) &= 254;
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

void SceneData_InitTableA980(void)
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

void SceneData_FillTableA980(void)
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

void SceneData_InitTableA980AndRunB(void)
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

void SceneData_BuildTableA980(void)
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
