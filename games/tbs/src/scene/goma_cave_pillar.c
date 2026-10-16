#include "script.h"
#include "version.h"
#include "global_cells.h"
#include "unit/import.h"
#include "unit/scene/goma_cave_pillar.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
#include "unit/field/field.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "mapcopy.h"
#include "types.h"
#include "animspr.h"
#include "field_event.h"
#include "field_scene.h"
#include "staged_actor.h"
#include "b5_context.h"
#include "edition.h"
#include "tbs_edition.h"
#include "field_obj.h"
#include "game_state.h"
#include "field_service.h"
#include "text/msg_ids.h"
#include "fixed_point_position.h"
#include "ability_ids.h"
#include "item_ids.h"
#include "objcmd.h"

struct ScriptTable {
  u8 *script[3];
};

// A pillar fragment's FieldActor view: speed and acceleration move its two
// plane axes, and work is the signed plane selector.
struct PillarEffect {
  u8 unk00[8];
  s32 x;
  s32 y;
  s32 z;
  u8 unk14[0x1c];
  s32 rateFirst;
  s32 rateSecond;
  s32 targetX;
  s32 targetY;
  s32 targetZ;
  u8 unk44[0x22];
  s16 plane;
};

enum PillarEffectPlane { PLANE_XY = 0, PLANE_XZ = 1, PLANE_YZ = 2 };

// Shared movement routines stay in their linked order.
#include "common/object_staged_actor.h"

// Actor 13 reacts before actor 12 starts Djinni battle 40.
static void EV12_TALK(void) {
  Actor_ShowEmote(13, 256, 0);
  Engine_ActorJump(13, 2, 0);
  Field_RequestDjinnBattle(12, 40);
}

// Advances the fragment's selected coordinate plane and keeps its targets in
// step.
static void
SceneEffect_AdvancePositionByAxisMode(union FieldObject *fieldObject) {
  struct PillarEffect *effect = (struct PillarEffect *)fieldObject;
  s16 plane = effect->plane;

  switch (plane) {
  case PLANE_XY:
    effect->x += effect->rateFirst;
    effect->targetX = effect->x;
    effect->y += effect->rateSecond;
    effect->targetY = effect->y;
    break;
  case PLANE_XZ:
    effect->x += effect->rateFirst;
    effect->targetX = effect->x;
    effect->z += effect->rateSecond;
    effect->targetZ = effect->z;
    break;
  case PLANE_YZ:
    effect->y += effect->rateFirst;
    effect->targetY = effect->y;
    effect->z += effect->rateSecond;
    effect->targetZ = effect->z;
    break;
  }
}

// Applies child value 15 at the end of a fragment's lifetime.
static s32 OverlayObject_ApplyValue15(struct ScriptInterpreter *interpreter) {
  struct FieldActor *actor = (struct FieldActor *)interpreter;
  ObjectGroup_SetChildValue(actor, 15);
  return 0;
}

// Creates a fragment with two axis rates, a script and packed plane/priority.
static void GomaCavePillar_SpawnPillarEffect(s32 x, s32 y, s32 z, s32 firstRate,
                                         s32 secondRate, s32 flags, s32 layer) {
  struct ScriptTable table;
  struct FieldActor *leader;
  struct FieldActor *obj;
  struct AnimationObject *sprite;
  u32 selection;

  leader = Object_GetById(ACTOR_PARTY_LEADER);
  table = *(struct ScriptTable *)gGomaCavePillar_PillarScripts;
  obj = Engine_ObjectCreate(222, x, y, z);
  if (obj == 0)
    return;
  sprite = (struct AnimationObject *)obj->sprite;
  Object_SetMode(obj, (flags + 1) & 15);
  Engine_ObjectSetScript(obj, table.script[flags & 15]);
  ObjectGroup_SetChildValue(obj, ((u32)flags >> 16) & 15);
  obj->motionFlags = 0;
  sprite->flags = 0;
  obj->update = SceneEffect_AdvancePositionByAxisMode;
  obj->speed = firstRate;
  obj->acceleration = secondRate;
  obj->work = *(u16 *)&layer;
  selection = (u32)layer >> 16;
  switch (selection) {
  case 0:
    sprite->image.priority =
        ((struct AnimationObject *)leader->sprite)->image.priority;
    break;
  case 1:
  case 2:
  case 3:
    obj->priorityFlags &= 0xfe;
    sprite->image.priority = selection;
    break;
  }
}

// Returns entrances through the overlay's byte-pointer API.
u8 *SceneData_GetScriptTable(void) { return gGomaCavePillar_Scripts; }

// This scene has no exit regions.
s32 SceneData_ReturnZero(void) { return 0; }

// Returns the exits to the surrounding Goma scenes.
u8 *SceneData_GetMessageTable(void) { return gGomaCavePillar_Messages; }

// Returns the placed pillar actors.
u8 *SceneData_GetActorTable(void) { return gGomaCavePillar_Actors; }

// Copies the moved pillar's collision pattern and records its selected side.
static void EV08_PSYNERGY(void) {
  struct FieldActor *actor;
  s32 x;
  s32 destinationX;
  s32 destinationZ;

  actor = Object_GetById(8);
  x = actor->x.fixed;
  if (x < 0)
    x += 0xfffff;
  x >>= 20;
  Engine_EventBegin();
  if (x == 20) {
    destinationX = 18;
    destinationZ = 6;
    Map_CopyCellAttributes(18, 40, 6, 3, destinationX, destinationZ);
    Engine_GameFlagClear(0x302);
  } else {
    destinationX = 18;
    destinationZ = 6;
    Map_CopyCellAttributes(24, 40, 6, 3, destinationX, destinationZ);
    Engine_GameFlagSet(0x302);
  }
  Engine_EventEnd();
}

// Emits falling fragments every eighth frame, twice on each sixteenth frame.
static void FieldScene_RunPrimarySequence(void) {
  s32 flags;
  s32 a0;
  s32 a2;
  struct FieldActor *rec4;
  struct FieldActor *rec7a, *reca;
  s32 v1a, v2a, v3a, ta, ua, na;
  struct FieldActor *rec7b, *recb;
  s32 v1b, v2b, v3b, tb, ub, nb;

  flags = gFrameCount & 7;
  if (flags == 0) {
    rec7a = Object_GetById(9);
    v1a = Engine_RandomNext();
    a0 = rec7a->x.fixed;
    a0 += ((((u32)(((v1a << 1) + v1a) << 2)) >> 16) << 16);
    rec4 = Object_GetById(9);
    reca = Object_GetById(9);
    a2 = reca->z.fixed;
    a2 += 0x60000;
    v2a = Engine_RandomNext();
    ta = ((u32)((v2a << 2) + v2a) >> 16);
    ua = (((ta << 1) + ta) << 2) + ta;
    na = ua << 6;
    na -= ua;
    na <<= 3;
    na += ta;
    v3a = (u32)(Engine_RandomNext() << 1) >> 16;
    na = -na;
    GomaCavePillar_SpawnPillarEffect(a0, rec4->y.fixed, a2, 0, na, v3a, flags);
    flags = gFrameCount & 15;
    if (flags == 0) {
      rec7b = Object_GetById(9);
      v1b = Engine_RandomNext();
      a0 = rec7b->x.fixed;
      a0 += ((((u32)(((v1b << 1) + v1b) << 2)) >> 16) << 16);
      rec4 = Object_GetById(9);
      recb = Object_GetById(9);
      a2 = recb->z.fixed;
      a2 += 0x60000;
      v2b = Engine_RandomNext();
      tb = ((u32)((v2b << 2) + v2b) >> 16);
      ub = (((tb << 1) + tb) << 2) + tb;
      nb = ub << 6;
      nb -= ub;
      nb <<= 3;
      nb += tb;
      v3b = (u32)(Engine_RandomNext() << 1) >> 16;
      nb = -nb;
      GomaCavePillar_SpawnPillarEffect(a0, rec4->y.fixed, a2, 0, nb, v3b, flags);
    }
  }
}

s32 gGomaCavePillar_Actor13Frames;

// Breaks actor 9's pillar only after it has moved into column 25.
static void EV09_PSYNERGY(void) {
  s32 kind;
  s32 zero;
  void (*fragmentCallback)(void);
  s32 a0;
  s32 a2;
  struct FieldActor *rec4;
  struct FieldActor *r1;
  struct FieldActor *spriteActor;
  struct FieldActor *blendActor;
  struct FieldActor *r4, *r5;
  struct FieldActor *movedActor;
  struct FieldActor *r7, *rec7;
  s32 v1, v2, v3, t, u, n;
  struct FieldActor *p0;

  r1 = Object_GetById(9);
  kind = r1->x.fixed / 0x100000;
  Engine_EventBegin();
  if (kind == 25) {
    p0 = Object_GetById(11);
    zero = 0;
    p0->terrainId = 1;
    spriteActor = Object_GetById(11);
    Engine_ActorSetSpriteFlags(spriteActor, 0);
    Engine_ActorSetChildValue(11, 14);
    blendActor = Object_GetById(11);
    Engine_ObjectSetBlendMode(blendActor, 1);
    Engine_ActorSetPosition(11, 0x19e0000, 0xf00000);
    Engine_EventWait(10);
    // Start the falling fragments before moving the pillar out of the way.
    fragmentCallback = FieldScene_RunPrimarySequence;
    FieldScheduler_AddCallback(fragmentCallback, 0xc80);
    Engine_AudioPlayCue(141);
    Engine_ActorSetDestinationOffset(9, 1, 0);
    Engine_ActorWaitForMove(9);
    Engine_EventWait(10);
    Engine_ActorSetDestinationOffset(9, 2, 0);
    Engine_ActorWaitForMove(9);
    r4 = Object_GetById(9);
    r4->bounce = zero;
    r5 = Object_GetById(9);
    r5->gravity = 0x9999;
    Engine_EventWait(3);
    Engine_ActorSetSpeed(9, 0x28000, 0x4000);
    Engine_AudioPlayCue(0x120);
    Engine_ActorSetDestination(9, 0x1a0, 200);
    movedActor = Object_GetById(9);
    Engine_ActorSetSpriteFlags(movedActor, 0);
    Engine_TaskRemoveCallback(fragmentCallback);
    Engine_EventWait(12);
    Engine_AudioPlayCue(189);
    rec7 = Object_GetById(9);
    v1 = Engine_RandomNext();
    a0 = rec7->x.fixed;
    a0 += ((((u32)(((v1 << 1) + v1) << 2)) >> 16) << 16);
    rec4 = Object_GetById(9);
    r7 = Object_GetById(9);
    a2 = r7->z.fixed;
    a2 += 0x60000;
    v2 = Engine_RandomNext();
    t = ((u32)((v2 << 2) + v2) >> 16);
    u = (((t << 1) + t) << 2) + t;
    n = u << 6;
    n -= u;
    n <<= 3;
    n += t;
    v3 = (u32)(Engine_RandomNext() << 1) >> 16;
    n = -n;
    GomaCavePillar_SpawnPillarEffect(a0, rec4->y.fixed, a2, zero, n, v3, zero);
    Engine_EventWait(20);
    // Wait for the work-value transition before replacing the map cells.
    Engine_AudioPlayCue(154);
    Map_SetShake(0x50000, 0x50000, 0x10000);
    Map_SetShake(-1, -1, 0xe666);
    Engine_MapRenderWaitForValues();
    Engine_ActorSetPosition(9, 0, 0);
    Engine_ActorSetPosition(11, 0, 0);
    Engine_GameFlagSet(0x300);
    Map_CopyCellAttributes(21, 45, 4, 2, 21, 11);
  }
  Engine_EventEnd();
}

// At cell (38,14), bursts actor 10's pillar into six planar fragments.
static void FieldScene_RunPillarBurst(void) {
  s32 x;
  s32 z;

  x = Object_GetById(10)->x.fixed / 0x100000;
  z = Object_GetById(10)->z.fixed / 0x100000;
  if (x == 38 && z == 14) {
    Object_GetById(10)->y.fixed = -0x20000;
    Object_GetById(10)->targetY = Object_GetById(10)->y.fixed;
    Engine_AudioPlayCue(188);
    GomaCavePillar_SpawnPillarEffect(Object_GetById(10)->x.fixed,
                                 Object_GetById(10)->y.fixed,
                                 Object_GetById(10)->z.fixed, 0x8000, 0, 0, 1);
    GomaCavePillar_SpawnPillarEffect(
        Object_GetById(10)->x.fixed, Object_GetById(10)->y.fixed,
        Object_GetById(10)->z.fixed, 0x6666, 0x6666, 0, 1);
    GomaCavePillar_SpawnPillarEffect(
        Object_GetById(10)->x.fixed, Object_GetById(10)->y.fixed,
        Object_GetById(10)->z.fixed, -0x6666, 0x6666, 0, 1);
    GomaCavePillar_SpawnPillarEffect(Object_GetById(10)->x.fixed,
                                 Object_GetById(10)->y.fixed,
                                 Object_GetById(10)->z.fixed, -0x8000, 0, 0, 1);
    GomaCavePillar_SpawnPillarEffect(
        Object_GetById(10)->x.fixed, Object_GetById(10)->y.fixed,
        Object_GetById(10)->z.fixed, 0x6666, -0x6666, 0, 1);
    GomaCavePillar_SpawnPillarEffect(
        Object_GetById(10)->x.fixed, Object_GetById(10)->y.fixed,
        Object_GetById(10)->z.fixed, -0x6666, -0x6666, 0, 1);
    Engine_GameFlagSet(0x301);
  }
}

// Runs the pillar burst as an event after Psynergy moves it into place.
static void EV10_PSYNERGY(void) {
  Engine_EventBegin();
  FieldScene_RunPillarBurst();
  Engine_EventEnd();
}

// Advances the staged movement, then checks the same pillar burst position.
static void EV10_SCRIPT(void) {
  Engine_EventBegin();
  StagedActor_AdvancePair();
  FieldScene_RunPillarBurst();
  Engine_EventEnd();
}

// Jumps the gap: international editions use the shared forward-jump routine.
static void EV20_TOUCH(void) {
#if TBS_PATCH_1
  Field_TryJumpForward();
#else
  struct FieldActor *obj;
  struct FixedPointPosition target;
  struct FixedPointPosition *pos;
  u8 *flags;
  u8 saved;

  obj = Object_GetById(0);
  flags = &obj->motionFlags;
  saved = *flags;
  target.x = Object_GetById(0)->x.fixed - 0x200000;
  pos = &target;
  pos->y = Object_GetById(0)->y.fixed;
  pos->z = Object_GetById(0)->z.fixed;
  if (FieldObject_CheckMovementCollision(obj, pos) == 0) {
    Engine_EventBegin();
    Object_SetMode(obj, 6);
    Engine_TaskWait(6);
    Engine_AudioPlayCue(152);
    Object_SetMode(obj, 7);
    obj->speed = 0x30000;
    obj->acceleration = 0x20000;
    obj->velocityY = 0x40000;
    *flags &= 0x7e;
    Engine_ActorSetSpriteFlags(obj, 0);
    Engine_ActorMoveToAndWait(0, *(s16 *)((u8 *)pos + 2),
                              *(s16 *)((u8 *)pos + 10));
    Object_SetMode(obj, 6);
    Engine_ActorSetSpriteFlags(obj, 1);
    *flags = saved;
    Engine_EventEnd();
  }
#endif
}

// Returns the scene's talk, Psynergy and raised-event table.
u8 *SceneData_GetExtraTable(void) { return gGomaCavePillar_Extras; }

// Times actor 13's idle reactions until the Djinni event is no longer pending.
static void GomaCavePillar_DriveActor13Idle(void) {
  struct EventWork *work;
  s32 frame;

  work = gEventWork;
  frame = ++gGomaCavePillar_Actor13Frames;
  switch (frame) {
  case 60:
    Actor_FaceDirection(13, FACING_SOUTHEAST, 0);
    Engine_ActorShowEmote(13, 2, 0);
    break;
  case 180:
    Engine_ActorStartRepeatedMotion(13, 3);
    break;
  case 240:
  case 270:
    Engine_ActorJump(13, 4, 0);
    break;
  case 480:
    Engine_ActorSetAnimation(13, 4);
    break;
  }
  if (gGameState.pendingDjinnEvent == 0)
    work->raisedTrigger = 99;
}

// Ends the idle callback, shows actor 13's reaction and walks the actor away.
static void EV99_RAISED(void) {
  Engine_TaskRemoveCallback(GomaCavePillar_DriveActor13Idle);
  Engine_EventBegin();
  Actor_ShowEmote(13, 0x100, 30);
  Engine_ActorRunRepeatedMotion(13, 2);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
  Engine_EventSetMessage(MsgGomaGotWowThatsPrettyImpressive);
  Event_ShowMessage(13, 0);
  Engine_ActorSetAnimationAndWait(13, 3);
  Engine_EventWait(30);
  Object_GetById(10)->priorityFlags &= ~ACTOR_PRIORITY_UNDERFOOT;
  Actor_SetSpeed(13, 0x20000, 0x10000);
  Actor_WalkToAndWait(13, 0x258, 216);
  Actor_WalkToAndWait(13, 0x258, 248);
  Actor_WalkToAndWait(13, 0x238, 0x128);
  Actor_SetPosition(13, 0, 0);
  Object_GetById(10)->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
  GameFlag_Set(0x869);
  Engine_EventEnd();
}

// Restores moved/broken pillars and starts the post-Djinni reaction if needed.
s32 FieldScene_SetupPillarsOnEntry(void) {
  struct FieldActor *actor;
  struct EventWork *work;

  work = gEventWork;
  work->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
  work->transitionFrames = 24;
  Object_GetById(9)->collisionFlags |= 16;
  if (GameFlag_IsSet(0x302) != 0) {
    Actor_SetPosition(8, 0x1580000, 0x680000);
    Map_CopyCellAttributes(24, 40, 6, 3, 18, 6);
  } else {
    Map_CopyCellAttributes(18, 40, 6, 3, 18, 6);
  }
  if (GameFlag_IsSet(0x300) != 0) {
    Actor_SetPosition(9, 0, 0);
    Map_CopyCellAttributes(21, 45, 4, 2, 21, 11);
  }
  if (GameFlag_IsSet(0x301) != 0) {
    Actor_SetPosition(10, 0x2680000, 0xe80000);
    if (gGameState.entrance == 2 || gGameState.entrance == 3) {
      Object_GetById(10)->terrainId = 2;
      actor = Object_GetById(10);
      actor->y.fixed--;
      Object_GetById(10)->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
      Map_CopyCellAttributes(36, 48, 5, 1, 36, 14);
    }
  }
  if (gGameState.entrance == 99) {
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Actor_SetPosition(9, 0x1800000, 0xc00000);
    Engine_EventWait(60);
    Object_GetById(9)->terrainId = 2;
    Actor_MoveToAndWait(9, 0x198, 192);
    Engine_EventWait(60);
    EV09_PSYNERGY();
  }
  if (gGameState.pendingDjinnEvent != 0) {
    gGomaCavePillar_Actor13Frames = 0;
    Task_AddCallback(GomaCavePillar_DriveActor13Idle, 0xc80);
  }
  return 0;
}

// One step per facing: X pixels in the high halfword, Z pixels in the low half.
#define DIRECTION_STEP(dx, dz) (((dx) << 16) | ((dz) & 0xffff))

s32 gStagedActor_DirectionSteps[] = {
    DIRECTION_STEP(16, 0),  DIRECTION_STEP(16, 0),  DIRECTION_STEP(16, 0),
    DIRECTION_STEP(0, 16),  DIRECTION_STEP(0, 16),  DIRECTION_STEP(0, 16),
    DIRECTION_STEP(0, 16),  DIRECTION_STEP(-16, 0), DIRECTION_STEP(-16, 0),
    DIRECTION_STEP(-16, 0), DIRECTION_STEP(-16, 0), DIRECTION_STEP(0, -16),
    DIRECTION_STEP(0, -16), DIRECTION_STEP(0, -16), DIRECTION_STEP(0, -16),
    DIRECTION_STEP(16, 0),
};

// clang-format off
u32 gStagedActor_FootprintKinds[] = {
    207, 205, 228, 229, 298, 297,
};

s32 gStagedActor_FootprintBounds[] = {
    -32, -8, 32, 8,
    -8, -32, 8, 32,
    -32, -16, 32, 0,
    -8, -32, 8, 32,
    -32, -8, 32, 8,
    -8, -32, 8, 32,
};

s32 *gGomaCavePillar_PillarScripts[] = {
    gGomaCavePillar_PillarScript0,
    gGomaCavePillar_PillarScript1,
    gGomaCavePillar_PillarScript2,
};

s32 gGomaCavePillar_PillarScript0[] = {
    SC_WAIT(1),
    SC_LOOP(22, SC_TOP),
    SC_CALL(OverlayObject_ApplyValue15),
    SC_WAIT(1),
    SC_FREE,
};

s32 gGomaCavePillar_PillarScript1[] = {
    SC_WAIT(1),
    SC_LOOP(44, SC_TOP),
    SC_CALL(OverlayObject_ApplyValue15),
    SC_WAIT(1),
    SC_FREE,
};

s32 gGomaCavePillar_PillarScript2[] = {
    SC_WAIT(1),
    SC_LOOP(126, SC_TOP),
    SC_CALL(OverlayObject_ApplyValue15),
    SC_WAIT(1),
    SC_FREE,
};

struct SceneEntrance gGomaCavePillar_Scripts[] = {
    { 0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { 1, CONDITION_ALWAYS, 88, 0, 184, FACING_SOUTH, 0, 8, 8, 728, 504, 0 },
    { 2, CONDITION_ALWAYS, 584, 0, 184, FACING_SOUTH, 0, 8, 8, 728, 504, 0 },
    { 3, CONDITION_ALWAYS, 520, 0, 232, FACING_SOUTH, 0, 8, 8, 728, 504, 0 },
    { 4, CONDITION_ALWAYS, 232, 0, 184, FACING_SOUTH, 0, 8, 8, 728, 504, 0 },
    { 5, CONDITION_ALWAYS, 536, 0, 408, FACING_SOUTH, 0, 8, 8, 728, 504, 0 },
    { 99, CONDITION_ALWAYS, 376, 0, 200, FACING_SOUTH, 0, 8, 8, 728, 504, 0 },
    { SCENE_TABLE_END },
};

u32 gGomaCavePillar_Messages[] = {
    SCENE_EXITS(27),
    SCENE_EXIT(1, 28, 2),
    SCENE_EXIT(2, 26, 1),
    SCENE_EXIT(3, 26, 2),
    SCENE_EXIT(4, 26, 3),
    SCENE_EXIT(5, 28, 3),
    SCENE_EXITS_END,
};

struct SceneEntrance gGomaCavePillar_Actors[] = {
    { 211, CONDITION_ALWAYS, 1, 0, 0, 0x148, 0, 0, 0, 104, 16384, 2 },
    { 212, CONDITION_ALWAYS, 1, 0, 0, 0x180, 0, 0, 0, 192, 16384, 2 },
    { 211, CONDITION_ALWAYS, 1, 0, 0, 0x248, 0, 0, 0, 280, 16384, 2 },
    { 223, CONDITION_ALWAYS, 1, 0, 0, 0x8, 0, 0, 0, 8, 16384, 2 },
    { 92, 0x58, 1, 0, 0, 0x298, 0, 0, 0, 216, 16384, 0 },
    { 134, 0x869, 1, 0, 0, 0x274, 0, 0, 0, 184, 8192, 1 },
    { SCENE_TABLE_END },
};

struct SceneEvent gGomaCavePillar_Extras[] = {
    { 0x21, 5, CONDITION_ALWAYS, 1 },
    { 0x31, 6, CONDITION_ALWAYS, 2 },
    { 0x31, 7, CONDITION_ALWAYS, 3 },
    { 0x31, 8, CONDITION_ALWAYS, 4 },
    { 0x21, 9, CONDITION_ALWAYS, 5 },
    { 0x202, 10, 0x301, EVENT_SCRIPT(EV10_SCRIPT) },
    { TOUCH_WALKING_INTO(FACING_WEST), 20, CONDITION_ALWAYS, EVENT_SCRIPT(EV20_TOUCH) },
    { EVENT_TALK, 12, CONDITION_ALWAYS, EVENT_SCRIPT(EV12_TALK) },
    { EVENT_TALK, 13, 0xa8, MSGNO(MsgVaultDidYouSeeSomethingStrangeOver) },
    { EVENT_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgVaultHmmmItMustBeHidingSomewhere) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, 0xa8, MSGNO(MsgVaultItJustWhackedMeInThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgVaultIKnowitsKindItllBeBack) },
    { PSYNERGY_ON_ACTOR(0x8c), 8, CONDITION_ALWAYS, EVENT_SCRIPT(EV08_PSYNERGY) },
    { PSYNERGY_ON_ACTOR(0x8c), 9, 0x300, EVENT_SCRIPT(EV09_PSYNERGY) },
    { PSYNERGY_ON_ACTOR(0x8c), 10, 0x301, EVENT_SCRIPT(EV10_PSYNERGY) },
    { SEARCH(SEARCH_CHEST), 101, 0xf4f, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { EVENT_RAISED, 99, CONDITION_ALWAYS, EVENT_SCRIPT(EV99_RAISED) },
    { SCENE_EVENTS_END },
};
// clang-format on
