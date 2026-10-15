#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "field_event.h"
#include "field_scene.h"
#include "io_reg.h"
#include "io_write_queue.h"
#include "types.h"
#include "call.h"
#include "animspr.h"
#include "object_runtime.h"
#include "field_obj.h"
#include "callback_scheduler.h"
#include "vram_block.h"

struct CompetitorState {
  u8 unk00[6];
  u8 mode;
  u8 stage;
};

void Colosso_FadeInCompetitor(s32 id, s32 x, s32 z) {
  struct CompetitorState *state;
  struct FieldActor *actor;
  struct AnimationObject *sprite;
  s32 i;

  state = (struct CompetitorState *)gMenuCtrlWork;
  actor = Object_GetById(id);
  state->mode = 1;
  state->stage = 4;
  gColosso_CompetitorStartX = actor->x.fixed;
  gColosso_CompetitorStartZ = actor->z.fixed;
  sprite = (struct AnimationObject *)actor->sprite;
  gColosso_CompetitorStartAngle = actor->facing;
  Engine_ActorSetSpritePriority(id, 2);
  actor->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
  actor->facing = FACING_SOUTH;
  Engine_ActorSetSpriteFlags(actor, 3);
  Object_SetMode(actor, 0);
  Object_SetMode(actor, 1);
  Engine_ActorSetPosition(id, x << 16, z << 16);
  Engine_ActorFaceActor(0, 0x4000, 0);
  IO_WRITE_QUEUE_PUSH(0x4000050, 0xf00, 0x20000);
  sprite->part[0].objectMode = 1;
  sprite->part[1].objectMode = 1;
  Engine_AudioPlayCue(252);
  for (i = 0; i <= 15; i += 2) {
    actor->scaleX = (i << 12) + 0x1000;
    actor->scaleY = 0x1f000 - (i << 12);
    IO_WRITE_QUEUE_PUSH(0x4000052, ((15 - i) << 8) | (i + 1), 0x20000);
    Engine_TaskWait(1);
  }
  IO_WRITE_QUEUE_PUSH(0x4000052, 16, 0x20000);
  actor->scaleX = 0x11000;
  actor->scaleY = 0xf000;
  Engine_EventWait(1);
  actor->scaleX = 0x10000;
  actor->scaleY = 0x10000;
  Engine_EventWait(13);
  sprite->part[0].objectMode = 0;
  sprite->part[1].objectMode = 0;
  Engine_ActorSetAnimationAndWait(id, 3);
  Engine_EventWait(20);
}

void Colosso_RestoreCompetitor(s32 id) {
  struct CompetitorState *state;
  struct FieldActor *actor;
  s32 zero;
  u8 *control;

  state = (struct CompetitorState *)gMenuCtrlWork;
  actor = Object_GetById(id);
  if (gGameState.movementMode == 1) {
    gGameState.movementMode = 0;
    Engine_ActorSetAnimation(id, 1);
  } else {
    Actor_FaceDirection(id, FACING_SOUTH, 30);
    Engine_ActorSetAnimation(id, 3);
    Engine_EventWait(30);
  }
  zero = 0;
  /* FAKEMATCH: needed to match: byte accesses keep the stage zero before the
   * start-position load. As state->stage and state->mode stores, of zero, a
   * literal 0 or zero = 0: differ 6. Measured 2026-10-13: a struct-field store
   * at a varying address may pass the scalar load of gColosso_CompetitorStartX
   * (fixed_scalar_and_varying_struct_p); a byte store through a pointer is no
   * struct reference. The actor held as union FieldObject with field stores:
   * differ 6; gMenuCtrlWork->fill and ->edge: differ 40. */
  control = (u8 *)state;
  control[(u32)&((struct CompetitorState *)0)->stage] = zero;
  control[(u32)&((struct CompetitorState *)0)->mode] = 15;
  actor->x.fixed = gColosso_CompetitorStartX;
  actor->z.fixed = gColosso_CompetitorStartZ;
  actor->facing = gColosso_CompetitorStartAngle;
  actor->targetX = ACTOR_NO_TARGET;
  actor->targetZ = ACTOR_NO_TARGET;
  actor->velocityX = zero;
  actor->velocityZ = zero;
  actor->motionFlags = 3;
  actor->terrainId = 0;
  actor->y.fixed = zero;
  ((struct ObjectRuntime *)actor)->terrainHeight = zero;
  Engine_ActorSetSpriteFlags(actor, 1);
  Object_SetMode(actor, 0);
  Object_SetMode(actor, 1);
  Engine_TaskWait(1);
}

void OverlayObject_ResetMotionFields(s32 actor) {
  struct FieldActor *o = Object_GetById(actor);

  ObjectDispatch_InitFromTable6(o);
  o->velocityX = 0;
  o->velocityZ = 0;
  o->targetX = ACTOR_NO_TARGET;
  o->targetZ = ACTOR_NO_TARGET;
}

static void Colosso_EnsureMarkerSlot(void) {

  if (gColosso_MarkerSlot == -1) {
    gColosso_MarkerSlot = Resource_LoadFixedBlockBIntoFreeSlot();
  }
}

union MarkerCoordinate {
  s16 signedValue;
  u16 value;
};

extern s16 gColosso_MarkerSteps;
extern s16 gColosso_MarkerStep;
extern s16 gColosso_MarkerX;
extern union MarkerCoordinate gColosso_MarkerStartX;
extern s16 gColosso_MarkerEndX;
extern s16 gColosso_MarkerY;
extern union MarkerCoordinate gColosso_MarkerStartY;
extern s16 gColosso_MarkerEndY;
extern s16 gColosso_MarkerBlink;
extern s16 gColosso_MarkerPriority;

static void Colosso_UpdateMarker(void) {
  s32 tile;
  s32 total;
  s32 step;
  s32 *oam;
  s16 *steps;
  s32 *dst;
  s32 x;
  s32 y;
  s32 diff;
  s32 base;
  s16 *py;
  union MarkerCoordinate *sy;

  oam = gColosso_MarkerOam;
  tile = gVramBlockCache[gColosso_MarkerSlot].offset >> 5;
  steps = &gColosso_MarkerSteps;
  total = *steps;
  if (total != 0) {
    step = ++gColosso_MarkerStep;
    py = &gColosso_MarkerX;
    sy = &gColosso_MarkerStartX;
    diff = gColosso_MarkerEndX - sy->signedValue;
    base = sy->value;
    *py = base + diff * (s16)step / total;
    gColosso_MarkerY =
        gColosso_MarkerStartY.value +
        (gColosso_MarkerEndY - gColosso_MarkerStartY.signedValue) * (s16)step /
            total;
    if ((s16)step >= total)
      *steps = 0;
    gColosso_MarkerBlink = 0;
  }
  if (++gColosso_MarkerBlink <= 13) {
    /* FAKEMATCH: needed to match: the size bit in a local puts the priority
     * after it. As the literal 0x40000000 in the expression, in any of four
     * operand orders: differ 6. */
    s32 size = 0x40000000;

    x = gColosso_MarkerX;
    y = gColosso_MarkerY;
    dst = oam;
    *dst++ = 0;
    *dst++ = (y - 8) | ((x - 8) << 16) | size | (gColosso_MarkerPriority << 28);
    *dst = tile | 0x400;
    Runtime_PushSlotEntry(oam, 255);
  } else if (gColosso_MarkerBlink > 19) {
    gColosso_MarkerBlink = 0;
  }
}

void SceneEffect_SeedMarkerAndInstallTask(u32 x, u32 y, u32 style) {
  Colosso_EnsureMarkerSlot();

  gColosso_MarkerX = x;
  gColosso_MarkerY = y;
  gColosso_MarkerPriority = (style & 3);
  gColosso_MarkerBlink = 0;
  gColosso_MarkerSteps = 0;

  FieldScheduler_AddCallback((s32)Colosso_UpdateMarker, 0xc80);
}

void SceneState_StartMarkerMove(u32 x, u32 y, u32 duration) {
  gColosso_MarkerEndX = x;
  gColosso_MarkerEndY = y;
  *(u16 *)&gColosso_MarkerStartX = gColosso_MarkerX;
  *(u16 *)&gColosso_MarkerStartY = gColosso_MarkerY;
  gColosso_MarkerSteps = duration;
  gColosso_MarkerStep = 0;

  FieldScheduler_AddCallback((s32)Colosso_UpdateMarker, 0xc80);
}

void SceneEffect_RemoveMarkerTask(void) {

  Engine_TaskRemoveCallback(Colosso_UpdateMarker);
  Resource_ResetEntry(gColosso_MarkerSlot);
  gColosso_MarkerSlot = -1;
}

void SceneActor_StartMode5MoveToTile(s32 a, s32 b, s32 c) {
  struct FieldActor *o = Engine_ActorLookup(a);

  if (o != 0) {
    s32 v = 0x20000;
    s32 z = 0;

    o->speed = v;
    o->acceleration = v >> 1;
    o->paused = z;
    ObjectDispatch_InitFromTable6(o);
    Object_SetMode(o, 5);
    Engine_ObjectSetPosition(o, b << 16, o->y.fixed, c << 16);
  }
}

void OverlayObject_PlaceWithScale14000(s32 a, s32 b, s32 c) {
  struct FieldActor *o = Engine_ActorLookup(a);

  if (o != 0) {
    s32 v = 0x14000;
    s32 z = 0;

    o->speed = v;
    o->acceleration = v >> 1;
    o->paused = z;
    ObjectDispatch_InitFromTable6(o);
    Object_SetMode(o, 5);
    Engine_ObjectSetPosition(o, b << 16, o->y.fixed, c << 16);
    Object_CommitPosition(o);
    Object_SetMode(o, 1);
  }
}
