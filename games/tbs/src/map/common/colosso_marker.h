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
#include "script_layer.h"
#include "map_tables.h"
#include "io_reg.h"
#include "io_write_queue.h"
#include "types.h"
#include "call.h"
#include "animspr.h"
#include "object.h"
#include "object_event.h"
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
  struct ObjectSystemWork *system;
  struct ObjectEvent *objectEvent;
  struct Sprite *sprite;
  s32 i;

  system = gObjectSystemWork;
  objectEvent = ObjectEvent_GetById(id);
  system->edge = 1;
  system->fill = 4;
  gColosso_WarriorStartX = objectEvent->x.fixed;
  gColosso_WarriorStartZ = objectEvent->z.fixed;
  sprite = (struct Sprite *)objectEvent->sprite;
  gColosso_WarriorStartAngle = objectEvent->facing;
  ObjectMotion_SetActionVariant(id, 2);
  objectEvent->priorityFlags |= OBJ_EVENT_PRIORITY_AUTOMATIC;
  objectEvent->facing = FACING_SOUTH;
  ObjectEvent_SetSpriteFlags(objectEvent, 3);
  Object_SelectAnimation(objectEvent, 0);
  Object_SelectAnimation(objectEvent, 1);
  Import_ObjectEventSetPosition(id, x << 16, z << 16);
  // BUG: FACING_SOUTH is passed where a local id belongs; no object event has
  // it, so the leader does not turn.
  ObjectMotion_SetAngleToward(LOCALID_PARTY_LEADER, FACING_SOUTH, 0);
  IO_WRITE_QUEUE_PUSH((u32)&REG_BLDCNT,
                      BLDCNT_TGT2_BG0 | BLDCNT_TGT2_BG1 | BLDCNT_TGT2_BG2 |
                          BLDCNT_TGT2_BG3,
                      IO_WRITE_16);
  sprite->image.objectMode = 1;
  sprite->shadow.objectMode = 1;
  Sound_PlaySong(SE_COLOSSO_WARRIOR_APPEAR);
  for (i = 0; i <= 15; i += 2) {
    objectEvent->scaleX = (i << 12) + 0x1000;
    objectEvent->scaleY = 0x1f000 - (i << 12);
    // BLEND_ALPHA(i + 1, 15 - i), whose operand order compiles differently.
    IO_WRITE_QUEUE_PUSH((u32)&REG_BLDALPHA, ((15 - i) << 8) | (i + 1),
                        IO_WRITE_16);
    WaitFrames(1);
  }
  IO_WRITE_QUEUE_PUSH((u32)&REG_BLDALPHA, BLEND_ALPHA(16, 0), IO_WRITE_16);
  objectEvent->scaleX = 0x11000;
  objectEvent->scaleY = 0xf000;
  FieldScript_WaitFrames(1);
  objectEvent->scaleX = 0x10000;
  objectEvent->scaleY = 0x10000;
  FieldScript_WaitFrames(13);
  sprite->image.objectMode = 0;
  sprite->shadow.objectMode = 0;
  ObjectMotion_SetAnimationAndWait(id, 3);
  FieldScript_WaitFrames(20);
}

/// Puts Isaac (id) back where Colosso_FadeInWarrior found him: he stands
/// (or turns south, nods and waits half a second), then returns to his kept
/// position and facing with his motion stopped, and the outline colours are
/// the field's again (edge 15, fill 0).
void Colosso_RestoreWarrior(s32 id) {
  struct ObjectSystemWork *system;
  struct ObjectEvent *objectEvent;
  s32 zero;
  u8 *control;

  system = gObjectSystemWork;
  objectEvent = ObjectEvent_GetById(id);
  if (gSaveBlock.movementMode == 1) {
    gSaveBlock.movementMode = 0;
    Import_ObjectEventSetAnimation(id, 1);
  } else {
    ObjectEvent_FaceDirection(id, FACING_SOUTH, 30);
    Import_ObjectEventSetAnimation(id, 3);
    FieldScript_WaitFrames(30);
  }
  zero = 0;
  // Stored as bytes through a pointer, the plain-C form of system->fill and
  // system->edge this site matches with (map/README.md): a struct-field
  // store at a varying address may pass the load of gColosso_WarriorStartX
  // (fixed_scalar_and_varying_struct_p), and a byte store is no struct
  // reference.
  control = (u8 *)system;
  control[offsetof(struct ObjectSystemWork, fill)] = zero;
  control[offsetof(struct ObjectSystemWork, edge)] = 15;
  objectEvent->x.fixed = gColosso_WarriorStartX;
  objectEvent->z.fixed = gColosso_WarriorStartZ;
  objectEvent->facing = gColosso_WarriorStartAngle;
  objectEvent->targetX = OBJ_EVENT_NO_TARGET;
  objectEvent->targetZ = OBJ_EVENT_NO_TARGET;
  objectEvent->velocityX = zero;
  objectEvent->velocityZ = zero;
  objectEvent->motionFlags = 3;
  objectEvent->layer = 0;
  objectEvent->y.fixed = zero;
  ((struct Object *)objectEvent)->groundHeight = zero;
  ObjectEvent_SetSpriteFlags(objectEvent, SPRITE_FLAG_SHADOW);
  Object_SelectAnimation(objectEvent, 0);
  Object_SelectAnimation(objectEvent, 1);
  WaitFrames(1);
}

/// Stops the object event where it is: no velocity and no destination.
void ObjectEvent_ResetMotionFields(s32 localId) {
  struct ObjectEvent *o = ObjectEvent_GetById(localId);

  Object_StartIdle((struct Object *)o);
  o->velocityX = 0;
  o->velocityZ = 0;
  o->targetX = OBJ_EVENT_NO_TARGET;
  o->targetZ = OBJ_EVENT_NO_TARGET;
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

/// The pointer's map callback: glide one step, then draw it while it blinks
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

  FieldScheduler_AddCallback(Colosso_UpdateMarker, TASK_PRIORITY_MAP);
}

/// Glides the pointer to screen pixel (x, y) over duration frames.
void Colosso_MovePointer(u32 x, u32 y, u32 duration) {
  gColosso_MarkerEndX = x;
  gColosso_MarkerEndY = y;
  *(u16 *)&gColosso_MarkerStartX = gColosso_MarkerX;
  *(u16 *)&gColosso_MarkerStartY = gColosso_MarkerY;
  gColosso_MarkerSteps = duration;
  gColosso_MarkerStep = 0;

  FieldScheduler_AddCallback(Colosso_UpdateMarker, TASK_PRIORITY_MAP);
}

/// Hides the pointer and frees its VRAM block.
void Colosso_HidePointer(void) {

  Task_Remove(Colosso_UpdateMarker);
  VramCache_FreeEntry(gColosso_MarkerSlot);
  gColosso_MarkerSlot = -1;
}

/// Walks the object event at 2 pixels a frame towards pixel (x, z) and returns
/// at once.
void Colosso_WalkObjectEventTo(s32 localId, s32 x, s32 z) {
  struct ObjectEvent *objectEvent = ObjectEvent_GetByLocalId(localId);

  if (objectEvent != 0) {
    s32 speed = 0x20000;
    s32 zero = 0;

    objectEvent->speed = speed;
    objectEvent->acceleration = speed >> 1;
    objectEvent->paused = zero;
    Object_StartIdle((struct Object *)objectEvent);
    Object_SelectAnimation(objectEvent, 5);
    Object_SetMoveTarget((struct Object *)objectEvent, x << 16,
                         objectEvent->y.fixed, z << 16);
  }
}

/// Walks the object event at 1.25 pixels a frame to pixel (x, z), waits until
/// it arrives and stands it there.
void Colosso_WalkObjectEventToAndStand(s32 localId, s32 x, s32 z) {
  struct ObjectEvent *objectEvent = ObjectEvent_GetByLocalId(localId);

  if (objectEvent != 0) {
    s32 speed = 0x14000;
    s32 zero = 0;

    objectEvent->speed = speed;
    objectEvent->acceleration = speed >> 1;
    objectEvent->paused = zero;
    Object_StartIdle((struct Object *)objectEvent);
    Object_SelectAnimation(objectEvent, 5);
    Object_SetMoveTarget((struct Object *)objectEvent, x << 16,
                         objectEvent->y.fixed, z << 16);
    Object_WaitForMove(objectEvent);
    Object_SelectAnimation(objectEvent, 1);
  }
}
