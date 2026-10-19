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
#include "songs.h"

// The Colosso finals' three courses (Colosso Log, River and Wall): what a
// stage's guide uses to describe the stage. Isaac appears on the stage to show
// how it is crossed, a blinking pointer moves over the screen, and Isaac is
// put back where he stood.

struct CompetitorState {
  u8 unk00[6];
  u8 mode;
  u8 stage;
};

// Isaac (id) appears at pixel (x, z) on the stage, facing south: he fades in
// through the backgrounds while he stretches from thin and tall to his own
// shape, settles and nods. His position and facing are kept for
// Colosso_RestoreCompetitor.
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
  IO_WRITE_QUEUE_PUSH((u32)&REG_BLDCNT,
                      BLDCNT_TGT2_BG0 | BLDCNT_TGT2_BG1 | BLDCNT_TGT2_BG2 |
                          BLDCNT_TGT2_BG3,
                      0x20000);
  sprite->image.objectMode = 1;
  sprite->shadow.objectMode = 1;
  Engine_AudioPlayCue(SE_252);
  for (i = 0; i <= 15; i += 2) {
    actor->scaleX = (i << 12) + 0x1000;
    actor->scaleY = 0x1f000 - (i << 12);
    IO_WRITE_QUEUE_PUSH((u32)&REG_BLDALPHA, ((15 - i) << 8) | (i + 1), 0x20000);
    Engine_TaskWait(1);
  }
  IO_WRITE_QUEUE_PUSH((u32)&REG_BLDALPHA, BLEND_ALPHA(16, 0), 0x20000);
  actor->scaleX = 0x11000;
  actor->scaleY = 0xf000;
  Engine_EventWait(1);
  actor->scaleX = 0x10000;
  actor->scaleY = 0x10000;
  Engine_EventWait(13);
  sprite->image.objectMode = 0;
  sprite->shadow.objectMode = 0;
  Engine_ActorSetAnimationAndWait(id, 3);
  Engine_EventWait(20);
}

// Put Isaac (id) back where Colosso_FadeInCompetitor found him: he stands
// (or turns south, nods and waits half a second), then returns to his kept
// position and facing with his motion stopped.
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
  // Stored as bytes through a pointer, the plain-C form of state->stage and
  // state->mode this site matches with (scene/README.md): a struct-field store
  // at a varying address may pass the load of gColosso_CompetitorStartX
  // (fixed_scalar_and_varying_struct_p), and a byte store is no struct
  // reference.
  control = (u8 *)state;
  control[offsetof(struct CompetitorState, stage)] = zero;
  control[offsetof(struct CompetitorState, mode)] = 15;
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

// Stop the actor where it is: no velocity and no destination.
void OverlayObject_ResetMotionFields(s32 actor) {
  struct FieldActor *o = Object_GetById(actor);

  ObjectDispatch_InitFromTable6(o);
  o->velocityX = 0;
  o->velocityZ = 0;
  o->targetX = ACTOR_NO_TARGET;
  o->targetZ = ACTOR_NO_TARGET;
}

// The guide's pointer: one 16x16 sprite centred on a point of the screen,
// shown for 13 frames of every 20, that can glide from point to point.

// Load the pointer's picture into a free VRAM block, once.
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

// The pointer's scene callback: glide one step, then draw it while it blinks
// on (a glide keeps it on).
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
    /* FAKEMATCH(temporary): the size bit (0x40000000) in a local puts the
     * priority after it; as the literal in the expression, 3 of 124
     * instructions differ in tbs-en. */
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

// Show the pointer at screen pixel (x, y), drawn at priority style & 3.
void SceneEffect_SeedMarkerAndInstallTask(u32 x, u32 y, u32 style) {
  Colosso_EnsureMarkerSlot();

  gColosso_MarkerX = x;
  gColosso_MarkerY = y;
  gColosso_MarkerPriority = (style & 3);
  gColosso_MarkerBlink = 0;
  gColosso_MarkerSteps = 0;

  FieldScheduler_AddCallback(Colosso_UpdateMarker, TASK_PRIORITY_SCENE);
}

// Glide the pointer to screen pixel (x, y) over duration frames.
void SceneState_StartMarkerMove(u32 x, u32 y, u32 duration) {
  gColosso_MarkerEndX = x;
  gColosso_MarkerEndY = y;
  *(u16 *)&gColosso_MarkerStartX = gColosso_MarkerX;
  *(u16 *)&gColosso_MarkerStartY = gColosso_MarkerY;
  gColosso_MarkerSteps = duration;
  gColosso_MarkerStep = 0;

  FieldScheduler_AddCallback(Colosso_UpdateMarker, TASK_PRIORITY_SCENE);
}

// Hide the pointer and free its VRAM block.
void SceneEffect_RemoveMarkerTask(void) {

  Engine_TaskRemoveCallback(Colosso_UpdateMarker);
  Resource_ResetEntry(gColosso_MarkerSlot);
  gColosso_MarkerSlot = -1;
}

// Walk the actor at 2 pixels a frame towards pixel (x, z) and return at once.
void SceneActor_StartMode5MoveToTile(s32 actorId, s32 x, s32 z) {
  struct FieldActor *actor = Engine_ActorLookup(actorId);

  if (actor != 0) {
    s32 speed = 0x20000;
    s32 zero = 0;

    actor->speed = speed;
    actor->acceleration = speed >> 1;
    actor->paused = zero;
    ObjectDispatch_InitFromTable6(actor);
    Object_SetMode(actor, 5);
    Engine_ObjectSetPosition(actor, x << 16, actor->y.fixed, z << 16);
  }
}

// Walk the actor at 1.25 pixels a frame to pixel (x, z), wait until it
// arrives and stand it there.
void OverlayObject_PlaceWithScale14000(s32 actorId, s32 x, s32 z) {
  struct FieldActor *actor = Engine_ActorLookup(actorId);

  if (actor != 0) {
    s32 speed = 0x14000;
    s32 zero = 0;

    actor->speed = speed;
    actor->acceleration = speed >> 1;
    actor->paused = zero;
    ObjectDispatch_InitFromTable6(actor);
    Object_SetMode(actor, 5);
    Engine_ObjectSetPosition(actor, x << 16, actor->y.fixed, z << 16);
    FieldObject_WaitForMove(actor);
    Object_SetMode(actor, 1);
  }
}
