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
#include "event_layer.h"
#include "scene_tables.h"
#include "io_reg.h"
#include "io_write_queue.h"
#include "types.h"
#include "call.h"
#include "animspr.h"
#include "actor.h"
#include "actor.h"
#include "task.h"
#include "vram_cache.h"
#include "songs.h"

// The sites of the three Colosso finals matches (Colosseum Final 1, 2 and 3):
// what a stage's guide uses to describe the stage. Isaac appears on the stage
// to show how it is crossed, a blinking pointer moves over the screen, and
// Isaac is put back where he stood.

/// Isaac (id) appears at pixel (x, z) on the stage, facing south: he fades in
/// through the backgrounds while he stretches from thin and tall to his own
/// shape, settles and nods. While he is shown his sprite is outlined, in edge
/// colour 1 and fill colour 4. His position and facing are kept for
/// Colosso_RestoreWarrior.
void Colosso_FadeInWarrior(s32 id, s32 x, s32 z) {
  struct ActorSystemWork *system;
  struct Actor *actor;
  struct Sprite *sprite;
  s32 i;

  system = gActorSystemWork;
  actor = Actor_GetById(id);
  system->edge = 1;
  system->fill = 4;
  gColosso_WarriorStartX = actor->x.fixed;
  gColosso_WarriorStartZ = actor->z.fixed;
  sprite = (struct Sprite *)actor->sprite;
  gColosso_WarriorStartAngle = actor->facing;
  Actor_SetSpritePriority(id, 2);
  actor->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
  actor->facing = FACING_SOUTH;
  Actor_SetSpriteFlags(actor, 3);
  Actor_SelectAnimation(actor, 0);
  Actor_SelectAnimation(actor, 1);
  Import_ActorSetPosition(id, x << 16, z << 16);
  // BUG: FACING_SOUTH is passed where an actor id belongs; no actor has
  // it, so the leader does not turn.
  Actor_SetAngleToward(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
  IO_WRITE_QUEUE_PUSH((u32)&REG_BLDCNT,
                      BLDCNT_TGT2_BG0 | BLDCNT_TGT2_BG1 | BLDCNT_TGT2_BG2 |
                          BLDCNT_TGT2_BG3,
                      IO_WRITE_16);
  sprite->image.objectMode = 1;
  sprite->shadow.objectMode = 1;
  Sound_PlaySong(SE_COLOSSO_WARRIOR_APPEAR);
  for (i = 0; i <= 15; i += 2) {
    actor->scaleX = (i << 12) + 0x1000;
    actor->scaleY = 0x1f000 - (i << 12);
    // BLEND_ALPHA(i + 1, 15 - i), whose operand order compiles differently.
    IO_WRITE_QUEUE_PUSH((u32)&REG_BLDALPHA, ((15 - i) << 8) | (i + 1),
                        IO_WRITE_16);
    WaitFrames(1);
  }
  IO_WRITE_QUEUE_PUSH((u32)&REG_BLDALPHA, BLEND_ALPHA(16, 0), IO_WRITE_16);
  actor->scaleX = 0x11000;
  actor->scaleY = 0xf000;
  Event_WaitFrames(1);
  actor->scaleX = 0x10000;
  actor->scaleY = 0x10000;
  Event_WaitFrames(13);
  sprite->image.objectMode = 0;
  sprite->shadow.objectMode = 0;
  Actor_SetAnimationAndWait(id, 3);
  Event_WaitFrames(20);
}

/// Puts Isaac (id) back where Colosso_FadeInWarrior found him: he stands
/// (or turns south, nods and waits half a second), then returns to his kept
/// position and facing with his motion stopped, and the outline colours are
/// the field's again (edge 15, fill 0).
void Colosso_RestoreWarrior(s32 id) {
  struct ActorSystemWork *system;
  struct Actor *actor;
  s32 zero;
  u8 *control;

  system = gActorSystemWork;
  actor = Actor_GetById(id);
  if (gAdventure.movementMode == 1) {
    gAdventure.movementMode = 0;
    Import_ActorSetAnimation(id, 1);
  } else {
    Actor_FaceDirection(id, FACING_SOUTH, 30);
    Import_ActorSetAnimation(id, 3);
    Event_WaitFrames(30);
  }
  zero = 0;
  // Stored as bytes through a pointer, the plain-C form of system->fill and
  // system->edge this site matches with (scene/README.md): a struct-field
  // store at a varying address may pass the load of gColosso_WarriorStartX
  // (fixed_scalar_and_varying_struct_p), and a byte store is no struct
  // reference.
  control = (u8 *)system;
  control[offsetof(struct ActorSystemWork, fill)] = zero;
  control[offsetof(struct ActorSystemWork, edge)] = 15;
  actor->x.fixed = gColosso_WarriorStartX;
  actor->z.fixed = gColosso_WarriorStartZ;
  actor->facing = gColosso_WarriorStartAngle;
  actor->targetX = ACTOR_NO_TARGET;
  actor->targetZ = ACTOR_NO_TARGET;
  actor->velocityX = zero;
  actor->velocityZ = zero;
  actor->motionFlags = 3;
  actor->layer = 0;
  actor->y.fixed = zero;
  (actor)->groundHeight = zero;
  Actor_SetSpriteFlags(actor, SPRITE_FLAG_SHADOW);
  Actor_SelectAnimation(actor, 0);
  Actor_SelectAnimation(actor, 1);
  WaitFrames(1);
}

/// Stops the actor where it is: no velocity and no destination.
void Actor_ResetMotionFields(s32 actorId) {
  struct Actor *o = Actor_GetById(actorId);

  Actor_StartIdle(o);
  o->velocityX = 0;
  o->velocityZ = 0;
  o->targetX = ACTOR_NO_TARGET;
  o->targetZ = ACTOR_NO_TARGET;
}

// The guide's pointer: one 16x16 sprite centred on a point of the screen,
// shown for 13 frames of every 20, that can glide from point to point.

/// Loads the pointer's picture into a free VRAM block, once.
static void Colosso_EnsureMarkerSlot(void) {

  if (gColosso_MarkerSlot == -1) {
    gColosso_MarkerSlot = Resource_LoadFixedBlockBIntoFreeSlot();
  }
}

/// The pointer's glide start on one axis, read signed or unsigned.
union MarkerCoordinate {
  s16 signedValue; ///< screen pixels
  u16 value;       ///< screen pixels
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

/// The pointer's task: glide one step, then draw it while it blinks
/// on (a glide keeps it on).
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

  oam = gColosso_PointerOam;
  tile = gVramCacheEntries[gColosso_MarkerSlot].offset >> 5;
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
    Oam_AddSpriteAtDepth(oam, 255);
  } else if (gColosso_MarkerBlink > 19) {
    gColosso_MarkerBlink = 0;
  }
}

/// Shows the pointer at screen pixel (x, y), drawn at priority style & 3.
void Colosso_ShowPointer(u32 x, u32 y, u32 style) {
  Colosso_EnsureMarkerSlot();

  gColosso_MarkerX = x;
  gColosso_MarkerY = y;
  gColosso_MarkerPriority = (style & 3);
  gColosso_MarkerBlink = 0;
  gColosso_MarkerSteps = 0;

  Task_AddFrame(Colosso_UpdateMarker, TASK_PASS_FRAME);
}

/// Glides the pointer to screen pixel (x, y) over duration frames.
void Colosso_MovePointer(u32 x, u32 y, u32 duration) {
  gColosso_MarkerEndX = x;
  gColosso_MarkerEndY = y;
  *(u16 *)&gColosso_MarkerStartX = gColosso_MarkerX;
  *(u16 *)&gColosso_MarkerStartY = gColosso_MarkerY;
  gColosso_MarkerSteps = duration;
  gColosso_MarkerStep = 0;

  Task_AddFrame(Colosso_UpdateMarker, TASK_PASS_FRAME);
}

/// Hides the pointer and frees its VRAM block.
void Colosso_HidePointer(void) {

  Task_Remove(Colosso_UpdateMarker);
  VramCache_FreeEntry(gColosso_MarkerSlot);
  gColosso_MarkerSlot = -1;
}

/// Walks the actor at 2 pixels a frame towards pixel (x, z) and returns
/// at once.
void Colosso_WalkActorTo(s32 actorId, s32 x, s32 z) {
  struct Actor *actor = Actor_Lookup(actorId);

  if (actor != 0) {
    s32 speed = 0x20000;
    s32 zero = 0;

    actor->speedLimit = speed;
    actor->acceleration = speed >> 1;
    actor->paused = zero;
    Actor_StartIdle(actor);
    Actor_SelectAnimation(actor, 5);
    Actor_SetMoveTarget(actor, x << 16, actor->y.fixed, z << 16);
  }
}

/// Walks the actor at 1.25 pixels a frame to pixel (x, z), waits until
/// it arrives and stands it there.
void Colosso_WalkActorToAndStand(s32 actorId, s32 x, s32 z) {
  struct Actor *actor = Actor_Lookup(actorId);

  if (actor != 0) {
    s32 speed = 0x14000;
    s32 zero = 0;

    actor->speedLimit = speed;
    actor->acceleration = speed >> 1;
    actor->paused = zero;
    Actor_StartIdle(actor);
    Actor_SelectAnimation(actor, 5);
    Actor_SetMoveTarget(actor, x << 16, actor->y.fixed, z << 16);
    Actor_WaitForMove(actor);
    Actor_SelectAnimation(actor, 1);
  }
}
