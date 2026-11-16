/// @file
/// Field effects that event commands start, in ROM order: the 96-particle
/// burst, an actor carried from one point to another as it circles, a whirl of
/// rings, an actor spinning away and back (Actor_DepartInRing,
/// Actor_ArriveInRing) and a spray of drops and smoke puffs.
// Part of field_psynergy.c.

#include "field_psynergy_internal.h"
#include "sounds.h"
#include "macros.h"
#include "field/effect_sprite.h"
#include "graphics/resource.h"
#include "system/task.h"
#include "fixed_math.h"
#include "field/actor.h"
#include "battle/battle.h"
#include "debug/debug.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "system/math_table.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "battle/motobj.h"
#include "field/canvas_sprite.h"
#include "field/actor_view.h"
#include "system/frame.h"

/// The 96-particle burst's task (ParticleBurstWork in HEAP_SLOT_BATTLE_EFFECT,
/// started by ParticleBurst_Start). Each particle whose delay counter is
/// between 0 and 63 shows the picture (delay / 8) % 6 of the burst's tiles 48
/// pixels out along its heading, then turns by its spin, rises by its speed and
/// spins eight units faster; every counter advances. The work's flash flag is
/// set on frame 30 only, and the burst stops being active on frame 112.
void ParticleBurst_Update(void) {
  struct ParticleBurstWork *work;
  struct BurstParticle *particle;
  struct EffectSprite *sprite;
  s32 position[3];
  s32 i;

  work = (struct ParticleBurstWork *)Ram_HeapSlots->battleEffect;
  particle = work->particles;

  for (i = 0; i < PARTICLE_BURST_COUNT; i++, particle++) {
    sprite = &work->sprites[i];
    if ((u32)particle->delay < 64) {
      sprite->oam.index = work->tile + (particle->delay >> 3) % 6 * 2;
      position[0] = particle->x;
      position[1] = particle->y;
      position[2] = particle->z;
      Vector_AddPolarOffset(toFIXED(48), particle->heading, position);
      Camera_WorldToScreen(position);
      sprite->x = position[0];
      sprite->y = position[2];
      EffectSprite_Draw(sprite);
      particle->heading += particle->spin;
      particle->y += particle->speed;
      particle->spin =
          particle->spin >= 0 ? particle->spin + 8 : particle->spin - 8;
    }
    particle->delay++;
  }
  work->flash = 0;
  if (work->frame == 30)
    work->flash = 1;
  if (work->frame == 112)
    work->active = 0;
  work->frame++;
}

/// Ends the particle burst (ParticleBurst_Start): stops its task and frees its
/// tiles.
void BattleEffect_EndX(void) {
  struct ParticleBurstWork *work = Ram_HeapSlots->battleEffect;

  Task_Remove((u32)ParticleBurst_Update);
  VramCache_FreeEntry(work->cacheEntry);
}

/// The frames the whirlwind takes to carry or lift the leader.
#define CYCLONE_CARRY_FRAMES 128

/// How far the whirlwind turns the leader round each frame of the ride.
#define CYCLONE_CARRY_TURN_STEP (TRIG_TURN * 5 / 256)

/// Task of the Cyclone: the whirlwind carries the leader over 128 frames from
/// where it stood to the nearby CYCLONE_CELL_CARRY cell (a Cyclone teleporter),
/// circling and arcing up and down on the way, its follower with it.
void Cyclone_CarryLeader(void) {
  struct FieldEffectWork *effect = Ram_HeapSlots->fieldEffectWork;
  struct CycloneWork *work = Ram_HeapSlots->battleEffect;
  struct Actor *owner = effect->mainActor;
  s32 timer = work->timer;
  s32 position[3];

  switch (work->state) {
  case 0:
    work->radius = Trig_Sin(timer * (TRIG_TURN / 2 / CYCLONE_CARRY_FRAMES))
                   << 5;
    work->angle += CYCLONE_CARRY_TURN_STEP;
    position[0] = work->from[0] +
                  (work->to[0] - work->from[0]) * timer / CYCLONE_CARRY_FRAMES;
    position[1] =
        work->from[1] +
        (work->to[1] - work->from[1]) * timer / CYCLONE_CARRY_FRAMES +
        (Trig_Sin(timer * (TRIG_TURN / 2 / CYCLONE_CARRY_FRAMES)) << 6);
    position[2] = work->from[2] +
                  (work->to[2] - work->from[2]) * timer / CYCLONE_CARRY_FRAMES;
    Vector_AddPolarOffset(work->radius, work->angle, position);
    SET_ACTOR_POSITION(owner, position[0], position[1], position[2]);
    if (timer >= CYCLONE_CARRY_FRAMES) {
      work->state++;
      work->timer = -1;
    }
    break;
  case 1:
    work->state = FIELD_PSYNERGY_FINISHED;
    break;
  }
  work->timer++;
  work->follower->x.fixed = owner->x.fixed;
  work->follower->y.fixed = owner->y.fixed;
  work->follower->z.fixed = owner->z.fixed;
}

/// Cyclone_CarryLeader for a CYCLONE_CELL_LIFT cell: the leader is carried
/// there 32 pixels up, circling, without the arc.
void Cyclone_LiftLeader(void) {
  struct FieldEffectWork *effect = Ram_HeapSlots->fieldEffectWork;
  struct CycloneWork *work = Ram_HeapSlots->battleEffect;
  struct Actor *owner = effect->mainActor;
  s32 timer = work->timer;
  s32 position[3];

  switch (work->state) {
  case 0:
    work->radius = Trig_Sin(timer * (TRIG_TURN / 2 / CYCLONE_CARRY_FRAMES))
                   << 5;
    work->angle += CYCLONE_CARRY_TURN_STEP;
    position[0] = work->from[0] +
                  (work->to[0] - work->from[0]) * timer / CYCLONE_CARRY_FRAMES;
    position[1] = work->from[1] +
                  (work->to[1] - work->from[1]) * timer / CYCLONE_CARRY_FRAMES;
    position[2] = work->from[2] +
                  (work->to[2] - work->from[2]) * timer / CYCLONE_CARRY_FRAMES;
    Vector_AddPolarOffset(work->radius, work->angle, position);
    SET_ACTOR_POSITION(owner, position[0], position[1], position[2]);
    if (timer >= CYCLONE_CARRY_FRAMES) {
      work->state++;
      work->timer = -1;
    }
    break;
  case 1:
    owner->y.fixed -= toFIXED(2) / 5;
    if (effect->childOption == 1) {
      if (timer == 40)
        work->state = FIELD_PSYNERGY_FINISHED;
    } else if (timer == 40) {
      work->state++;
      work->timer = -1;
    }
    break;
  case 2:
    owner->y.fixed += toFIXED(19) / 5;
    if (timer == 40)
      work->state = FIELD_PSYNERGY_FINISHED;
    break;
  }
  work->timer++;
  work->follower->x.fixed = owner->x.fixed;
  work->follower->y.fixed = owner->y.fixed;
  work->follower->z.fixed = owner->z.fixed;
}

/// Per-frame update: turns an actor 1/16 turn a frame.
void BattleEffect_SpinObject(struct MotionObject *object) {
  object->angle += TRIG_TURN / 16;
}

/// Cyclone cast on a target: runs FieldPsynergy_Cyclone.
void FieldPsynergy_CycloneOnTarget(void) { FieldPsynergy_Cyclone(); }

/// Cyclone ("Conjure wind to scatter weeds."): the caster spins in a whirlwind
/// of blue and white drawn on the canvas sprite, and the weeds on the cells
/// round it (FieldPsynergy_GatherNearbyCells) are scattered.
void FieldPsynergy_Cyclone(void) {
  struct CycloneWork *work =
      HeapSlot_Alloc(HEAP_SLOT_BATTLE_EFFECT, sizeof(struct CycloneWork));
  struct FieldWork *event = Ram_HeapSlots->fieldWork;
  struct FieldEffectWork *effect = Ram_HeapSlots->fieldEffectWork;
  struct Actor *owner = effect->mainActor;
  s16 *points = &event->psynergyCells[0];
  FIXED spread = toFIXED(4);
  s32 step = toFIXED(7) / 8;
  struct CanvasSpriteWork *canvas;
  struct Actor *ring;
  u16 *offsets;
  u8 *pixels;
  u16 *palette;
  FIXED pos[3];
  s32 mode;
  s32 frame;
  s32 i, j;
  s32 blue, c;
  s32 point;

  FieldPsynergy_FadeToCastColor();
  FieldPsynergy_GatherNearbyCells();
  BattleEffect_SetObjectUpdateMode(effect->mainActor, 0);
  owner->update = BattleEffect_SpinObject;
  owner->timer = owner->facing;
  palette = (u16 *)&OBJ_PLTT_COLOR(CANVAS_PALETTE, 0);
  blue = RGB_LEVEL_MAX << 10;
  for (j = 0; j < 16; j++) {
    c = j + 16;
    Io_Write16((c << 5) | blue | c, palette);
    palette++;
  }
  BattleEffect_StartCanvasSprite();
  canvas = Ram_HeapSlots->canvasSprite;
  pos[0] = effect->mainActor->x.fixed;
  pos[1] = effect->mainActor->y.fixed;
  pos[2] = effect->mainActor->z.fixed;
  Camera_WorldToScreen(pos);
  canvas->x = (s16)FIXEDtoINT(pos[0]);
  canvas->y = (s16)FIXEDtoINT(pos[2]);
  canvas->priority = 1;
  canvas->depth = 240;
  canvas->mode = CANVAS_SPRITE_KEPT;
  canvas->scaleX = FIXED_ONE * 3 / 2;
  canvas->scaleY = FIXED_ONE * 3 / 2;
  EffectSprite_Init(&canvas->sprite, 32, 32, OBJ_SIZE_64X64, canvas->tile);
  canvas->sprite.oam.fullColor = 1;
  canvas->sprite.oam.palette = 0;
  offsets = Ram_HeapSlots->canvasMap;
  pixels = Ram_HeapSlots->battleCanvas;
  ring = Actor_Spawn(SPRITE_WHIRLWIND, owner->x.fixed,
                     owner->y.fixed - toFIXED(6), owner->z.fixed);
  Actor_SelectAnimationFar(ring, 2);
  ring->scaleX = FIXED_ONE * 3 / 2;
  ring->scaleY = 0;
  wait(1);
  Sound_PlayFar(SE_RUSH);
  for (i = 0; i < 20; i++) {
    ring->scaleY = FIXED_ONE * i / 20;
    wait(1);
  }
  for (i = 0; i < 60; i++) {
    for (j = 0; j < 4; j++)
      Cyclone_PlotArc((s32)(spread + Random16() * 4 - (j << 16)) >> 16,
                      Random16(), offsets, pixels);
    spread += step;
    if (spread > toFIXED(28))
      spread = toFIXED(28);
    step = step * 123 / 128;
    canvas->angle += TRIG_TURN / 64;
    canvas->command = CANVAS_UPLOAD_AND_DARKEN_ONE_LEVEL;
    wait(1);
  }
  FieldPsynergy_FadeFromCastColor();
  work->follower = ring;
  work->from[0] = owner->x.fixed;
  work->from[1] = owner->y.fixed;
  work->from[2] = owner->z.fixed;
  work->to[0] = owner->x.fixed;
  work->to[1] = owner->y.fixed;
  work->to[2] = owner->z.fixed;
  work->angle = 0;
  work->state = 0;
  work->timer = 0;
  work->state = 0;
  if (event->cycloneLiftX != -1)
    mode = CYCLONE_CELL_LIFT;
  else if (event->cycloneCarryX != -1)
    mode = CYCLONE_CELL_CARRY;
  else {
    point = points[0];
    mode = point != -1;
  }

  if (mode == CYCLONE_CELL_LIFT) {
    if (work->kind)
      ParticleBurst_Start();
    for (i = 0; i < 16; i++) {
      canvas->angle += TRIG_TURN / 64;
      canvas->command = CANVAS_UPLOAD_AND_DARKEN_ONE_LEVEL;
      wait(1);
    }
    FieldCamera_MoveWithinMapBounds(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
    Actor_SetPositionAndResetMotionFar(owner, owner->x.fixed, owner->y.fixed,
                                       owner->z.fixed);
    Actor_SetSpriteFlagsFar(owner, 0);
    Actor_SetSpritePriority(effect->firstActorId, 1);
    owner->motionFlags = 0;
    frame = 0;
    wait(1);
    work->to[0] = toFIXED(event->cycloneLiftX);
    work->to[1] = owner->y.fixed + toFIXED(32);
    work->to[2] = toFIXED(event->cycloneLiftZ);
    Task_Add(Cyclone_LiftLeader, TASK_PASS_VBLANK);
    while (work->state != FIELD_PSYNERGY_FINISHED) {
      if (frame == 30)
        FieldPsynergy_RunTouchedEvents();
      frame++;
      wait(1);
    }
    Task_Remove(Cyclone_LiftLeader);
    if (effect->childOption == 0)
      owner->scaleX = 0;
    ring->scaleX = 0;
    if (work->kind)
      BattleEffect_EndX();
  } else if (mode == CYCLONE_CELL_CARRY) {
    if (work->kind)
      ParticleBurst_Start();
    for (i = 0; i < 16; i++) {
      canvas->angle += TRIG_TURN / 64;
      canvas->command = CANVAS_UPLOAD_AND_DARKEN_ONE_LEVEL;
      wait(1);
    }
    FieldCamera_MoveWithinMapBounds(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
    Actor_SetPositionAndResetMotionFar(owner, owner->x.fixed, owner->y.fixed,
                                       owner->z.fixed);
    Actor_SetSpriteFlagsFar(owner, 0);
    owner->motionFlags = 0;
    wait(1);
    work->to[0] = toFIXED(event->cycloneCarryX);
    work->to[1] = owner->y.fixed;
    work->to[2] = toFIXED(event->cycloneCarryZ);
    Task_Add(Cyclone_CarryLeader, TASK_PASS_VBLANK);
    frame = 0;
    while (work->state != FIELD_PSYNERGY_FINISHED) {
      if (frame == 30)
        FieldPsynergy_RunTouchedEvents();
      frame++;
      wait(1);
    }
    Task_Remove(Cyclone_CarryLeader);
    for (i = 0; i < 16; i++) {
      owner->scaleX += -(FIXED_ONE / 32);
      owner->scaleY = owner->scaleX;
      wait(1);
    }
    owner->scaleX = 0;
    for (i = 0; i < 32; i++) {
      ring->scaleX += -(FIXED_ONE / 32);
      ring->scaleY += -(FIXED_ONE / 128);
      wait(1);
    }
    ring->scaleX = 0;
    if (work->kind)
      BattleEffect_EndX();
  } else if (mode == CYCLONE_CELL_BURST) {
    Sound_PlayFar(SE_WHOOSH);
    Actor_SelectAnimationFar(ring, 6);
    if (work->kind)
      ParticleBurst_Start();
    for (i = 0; i < 16; i++) {
      canvas->angle += TRIG_TURN / 64;
      canvas->command = CANVAS_UPLOAD_AND_DARKEN_ONE_LEVEL;
      wait(1);
    }
    owner->update = NULL;
    owner->facing = owner->timer;
    frame = 0;
    while (work->count != 0) {
      if (frame == 30)
        FieldPsynergy_RunTouchedEvents();
      wait(1);
      frame++;
    }
    if (work->kind)
      BattleEffect_EndX();
  } else {
    Sound_PlayFar(SE_WHOOSH);
    Actor_SelectAnimationFar(ring, 6);
    for (i = 0; i < 16; i++) {
      owner->facing += FACING_STEP;
      canvas->angle += TRIG_TURN / 64;
      canvas->command = CANVAS_UPLOAD_AND_DARKEN_ONE_LEVEL;
      wait(1);
    }
  }
  Sound_PlayFar(SE_WHOOSH);
  Actor_ReleaseFar(ring);
  owner->update = NULL;
  owner->facing = owner->timer;
  BattleEffect_EndCanvasSprite();
  HeapSlot_Release(HEAP_SLOT_BATTLE_EFFECT);
}

/// The actor leaves in a ring of light (sprite 280): for 128 frames it swings
/// up and round inside the ring as the screen transition starts, then the
/// ring vanishes with a whoosh and the camera follows the actor again.
void Actor_DepartInRing(s32 actor) {
  struct Actor *object = Actor_GetById(actor);
  s32 angle = object->facing;
  u8 motionFlags = object->motionFlags;
  struct Actor *ring;
  FIXED x, y, z;
  s32 spin;
  FIXED position[3];
  s32 i;

  FieldCamera_MoveWithinMapBounds(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  object->motionFlags = 0;
  object->update = BattleEffect_SpinObject;
  ring = Actor_Spawn(SPRITE_WHIRLWIND, object->x.fixed, object->y.fixed,
                     object->z.fixed);
  Actor_SelectAnimationFar(ring, 2);
  ring->scaleX = FIXED_ONE * 3 / 2;
  ring->scaleY = FIXED_ONE;
  x = object->x.fixed;
  y = object->y.fixed;
  z = object->z.fixed;
  spin = 0;
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_StartScreenTransition();
  for (i = 0; i < 128; i++) {
    position[0] = x;
    position[1] = y + Trig_Cos(i << 7) * 96;
    position[2] = z;
    Vector_AddPolarOffset(Trig_Cos(i << 7) << 5, spin, position);
    SET_ACTOR_POSITION(object, position[0], position[1], position[2]);
    spin += TRIG_TURN / 64;
    SET_ACTOR_POSITION(ring, object->x.fixed, object->y.fixed - toFIXED(6),
                       object->z.fixed);
    wait(1);
  }
  Actor_SetPositionAndResetMotionFar(object, x, y, z);
  Sound_PlayFar(SE_WHOOSH);
  Actor_SelectAnimationFar(ring, 6);
  Event_Wait(15);
  Actor_ReleaseFar(ring);
  object->update = NULL;
  object->facing = angle;
  object->motionFlags = motionFlags;
  wait(1);
  FieldCamera_FollowActor(actor, 1);
}

/// An event command, the arrival counterpart of Actor_DepartInRing. The actor
/// spins (BattleEffect_SpinObject) at zero width and its sprite is hidden while
/// object 280 grows on it for 32 frames; object 280 then snaps to 1.5 by 1
/// (cue 136, mode 6) while the actor widens back from half size over 16 frames
/// and shows again. Object 280 is destroyed and the actor's update and facing
/// are restored.
void Actor_ArriveInRing(s32 actor) {
  struct Actor *object = Actor_GetById(actor);
  s32 angle = object->facing;
  struct Actor *ring;
  s32 i;

  object->update = BattleEffect_SpinObject;
  object->scaleX = 0;
  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_SetSpriteFlagsFar(Actor_GetById(actor), 0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  ring = Actor_Spawn(SPRITE_WHIRLWIND, object->x.fixed, object->y.fixed,
                     object->z.fixed);
  Actor_SelectAnimationFar(ring, 2);
  ring->scaleX = FIXED_ONE / 2;
  ring->scaleY = FIXED_ONE * 3 / 4;
  for (i = 0; i < 32; i++) {
    ring->scaleX += FIXED_ONE / 32;
    ring->scaleY += FIXED_ONE / 128;
    wait(1);
  }
  ring->scaleX = FIXED_ONE * 3 / 2;
  ring->scaleY = FIXED_ONE;
  Sound_PlayFar(SE_WHOOSH);
  Actor_SelectAnimationFar(ring, 6);
  object->scaleY = object->scaleX = FIXED_ONE / 2;
  for (i = 0; i < 16; i++) {
    object->scaleY = object->scaleX = object->scaleX + FIXED_ONE / 32;
    wait(1);
  }
  Actor_SetSpriteFlagsFar(Actor_GetById(actor), 1);
  Event_Wait(10);
  Actor_ReleaseFar(ring);
  object->update = NULL;
  object->facing = angle;
}

// The spray's drops and smoke puffs, moved and drawn each frame.

/// What Blaze does (BlazeWork.mode).
enum BlazeMode {
  BLAZE_FLICKER = 0,     ///< cast at nothing with no torch in reach: a flicker
  BLAZE_LIGHT_TORCH = 1, ///< cast at nothing: lights the nearest torch in reach
  BLAZE_SPRAY = 2,       ///< cast at a target: the jet of flame
  BLAZE_LONG_SPRAY =
      3 ///< the jet, faster and longer (FieldWork.blazeLongSpray)
};

/// The drops of Blaze's spray, and the smoke puffs they leave.
#define BLAZE_DROPS 16

/// Blaze's work (heap slot HEAP_SLOT_BATTLE_EFFECT): the flame tiles, 16 flames
/// and 16 smoke puffs with their motion, where the jet starts and where it
/// points, and the actor it has reached.
struct BlazeWork {
  u8 unused[0x1000]; ///< no code names it
  struct EffectSprite drops[BLAZE_DROPS];
  struct FieldPsynergyParticle dropMotion[BLAZE_DROPS];
  struct EffectSprite puffs[BLAZE_DROPS];
  struct FieldPsynergyParticle puffMotion[BLAZE_DROPS];
  s32 origin[3];
  u16 dropTile;
  u16 puffTile;
  u16 angle;
  u16 targetAngle;
  s16 hitObject;
  s8 hit;
  s8 emitting;
  s32 speed;
  s32 lifetime;
  s32 gravity;
  s8 mode; ///< enum BlazeMode
};

#define EffectSprite_DrawAtWorldPosition EffectSprite_DrawAtWorldPosition

/// The frame callback of FieldPsynergy_Blaze's spray. Every fourth frame while
/// it is emitting, a free one of sixteen drops starts at the origin with the
/// mode's speed (mode 3 faster, longer and lighter) along the spray's angle.
/// Each live drop grows, cycles its four tiles, is drawn at its position, and
/// dies after the mode's lifetime, on reaching an object of the event work's
/// list (whose index is kept and which stops the spray for the frame) or on
/// something solid; an odd drop that dies leaves one of sixteen puffs of smoke
/// that rise for 32 frames. The angle then turns toward the scene's facing by
/// at most 1/64 turn a frame.
void Blaze_UpdateSpray(void) {
  struct BlazeWork *work = (struct BlazeWork *)Ram_HeapSlots->battleEffect;
  struct FieldEffectWork *scene = Ram_HeapSlots->fieldEffectWork;
  struct FieldWork *event = Ram_HeapSlots->fieldWork;
  struct EffectSprite *sprite;
  struct FieldPsynergyParticle *particle;
  struct FieldPsynergyParticle *puff;
  s32 pos[3];
  s32 i;
  s32 j;
  u16 target;
  s32 turn;

  if (work->emitting != 0 && (gFrameCount & 3) == 0) {
    particle = work->dropMotion;
    for (i = 0; i < BLAZE_DROPS; i++, particle++) {
      if (particle->timer == -1) {
        sprite = &work->drops[i];
        if (scene->child != NULL)
          sprite->depth = BattleEffect_GetObjectDepth(scene->child);
        else
          sprite->depth = BattleEffect_GetObjectDepth(scene->mainActor);
        if (work->mode == BLAZE_LONG_SPRAY) {
          work->speed = toFIXED(12) / 5;
          work->lifetime = 33;
          work->gravity = toFIXED(3) / 32;
        } else {
          work->speed = toFIXED(9) / 5;
          work->lifetime = 25;
          work->gravity = toFIXED(1) / 8;
        }
        SET_POSITION(particle, work->origin[0], work->origin[1],
                     work->origin[2]);
        particle->velocityX = 0;
        particle->velocityY = -(toFIXED(6) / 5);
        particle->velocityZ = 0;
        Vector_AddPolarOffset(work->speed, work->angle, &particle->velocityX);
        particle->timer = 0;
        break;
      }
    }
  }

  work->hit = 0;
  particle = work->dropMotion;
  for (i = 0; i < BLAZE_DROPS; i++, particle++) {
    if (particle->timer == -1)
      continue;
    sprite = &work->drops[i];
    sprite->scaleY = sprite->scaleX =
        particle->timer * (FIXED_ONE / 25) + FIXED_ONE / 2;
    sprite->oam.index =
        work->dropTile + (((particle->timer >> 1) + i) & 3) * 32;
    if (work->angle > DEGtoANG(180))
      sprite->depth--;
    else
      sprite->depth++;
    EffectSprite_DrawAtWorldPosition(sprite, particle);
    if (++particle->timer == work->lifetime)
      particle->timer = -1;
    for (j = 0; j < FIELD_ACTOR_COUNT; j++) {
      struct Actor *other = event->actors[j];
      if (other != NULL && other->script != NULL &&
          (other->collisionFlags & ACTOR_SOLID) && other != scene->mainActor &&
          other != scene->child &&
          FieldPsynergy_CheckOverlap(&particle->x, 2, &other->x.fixed,
                                     other->radius - 2) >= 0) {
        work->hit = 1;
        work->hitObject = j;
        particle->timer = -1;
        break;
      }
    }
    if (FIELD_CELL(particle->x) != FIELD_CELL(work->origin[0]) ||
        FIELD_CELL(particle->z) != FIELD_CELL(work->origin[2])) {
      pos[0] = particle->x;
      pos[1] = particle->y;
      pos[2] = particle->z;
      if (MapGrid_CheckCollisionFar(scene->mainActor, pos) > 0)
        particle->timer = -1;
    }
    if (particle->timer == -1 && (i & 1)) {
      pos[0] = particle->x;
      pos[1] = particle->y - (Random16() << 2);
      pos[2] = particle->z;
      Vector_AddPolarOffset(toFIXED(10), work->angle + DEGtoANG(180), pos);
      Vector_AddPolarOffset(toFIXED(12), Random16(), pos);
      puff = &work->puffMotion[i];
      SET_POSITION(puff, pos[0], pos[1], pos[2]);
      puff->timer = 0;
    }
    if (work->hit != 0)
      break;
    BattleEffect_IntegrateVector3(&particle->x, 64, work->gravity);
  }

  particle = work->puffMotion;
  for (i = 0; i < BLAZE_DROPS; i++, particle++) {
    if (particle->timer >= 0) {
      sprite = &work->puffs[i];
      sprite->oam.index = work->puffTile + ((particle->timer >> 2) & 7) * 8;
      pos[0] = particle->x;
      pos[1] = particle->y;
      pos[2] = particle->z;
      Camera_WorldToScreen(pos);
      sprite->x = pos[0];
      sprite->y = pos[2];
      EffectSprite_Draw(sprite);
      particle->y += toFIXED(4) / 5;
      if (++particle->timer == 32)
        particle->timer = -1;
    }
  }

  target = scene->angle8 - DEGtoANG(90);
  turn = (s16)(work->targetAngle - work->angle);
  if ((u16)(work->targetAngle - target) - (u16)(work->angle - target) ==
      0x8000) {
    work->angle = work->angle + TRIG_TURN / 64;
  } else {
    if (turn > TRIG_TURN / 64)
      turn = TRIG_TURN / 64;
    if (turn < -(TRIG_TURN / 64))
      turn = -(TRIG_TURN / 64);
    work->angle = work->angle + turn;
  }
}

void Teleport_PlaySparks(void);

/// Blaze cast on a target: runs FieldPsynergy_Blaze.
void FieldPsynergy_BlazeOnTarget(void) { FieldPsynergy_Blaze(); }
