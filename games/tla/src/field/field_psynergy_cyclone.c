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

/* VIEW(psynergy): routine Camera_WorldToScreen as void Camera_WorldToScreen(s32
 * *position); field/event_commands.h has void Camera_WorldToScreen(struct
 * EffectVector *position); tried as the definition's void
 * Camera_WorldToScreen(struct EffectVector *position), which the object already
 * declares, it does not compile in tla-ja:
 * games/tla/src/field/field_psynergy_sand.c:136: conflicting types for
 * `Camera_WorldToScreen'. */
void Camera_WorldToScreen__psynergy(s32 *position) __asm__(
    "Camera_WorldToScreen");
#define Camera_WorldToScreen Camera_WorldToScreen__psynergy

/// The 96-particle burst's task (heap slot 0x5c work, started by
/// ParticleBurst_Start). Each particle whose delay counter is between 0 and 63
/// shows the picture (delay / 8) % 6 of the burst's tiles 48 pixels out along
/// its heading, then turns by its spin, rises by its speed and spins eight
/// units faster; every counter advances. The work's flash flag is set on frame
/// 30 only, and the burst stops being active on frame 112.
void ParticleBurst_Update(void) {
  struct ParticleBurstWork *work;
  struct BurstParticle *particle;
  struct EffectSprite *sprite;
  s32 position[3];
  s32 i;

  work = (struct ParticleBurstWork *)Ram_HeapSlots->battleEffect;
  particle = work->particles;

  for (i = 0; i < 96; i++, particle++) {
    sprite = &work->sprites[i];
    if ((u32)particle->delay < 64) {
      sprite->oam.index = work->vram + (particle->delay >> 3) % 6 * 2;
      position[0] = particle->x;
      position[1] = particle->y;
      position[2] = particle->z;
      Vector_AddPolarOffset(px(48), particle->heading, position);
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
  u8 *work = Ram_HeapSlots->battleEffect;

  Task_Remove((u32)ParticleBurst_Update);
  VramCache_FreeEntry(*(s16 *)(work + 0x1b00));
}

/// Cyclone's work (heap slot HEAP_SLOT_BATTLE_EFFECT): the particle burst
/// (ParticleBurstWork) at its start, then the leader's ride on the whirlwind.
struct CycloneWork {
  u8 particleBurst[0x1b08];
  s16 count;
  u8 unusedAfterCount[2]; ///< no code names it
  struct Actor *follower; ///< the actor carried with the leader
  s16 state;              ///< the stage of the ride; 999 once it ends
  s16 timer;              ///< frames into the ride
  s32 angle;              ///< the whirlwind's turn, 1/65536 turns
  s32 radius;
  s32 from[3]; ///< where the ride starts, 16.16 pixels
  s32 to[3];   ///< where it ends, 16.16 pixels
  s8 kind;     ///< nonzero runs the particle burst with it
};

/// Task of the Cyclone: the whirlwind carries the leader over 128 frames from
/// where it stood to the nearby kind 2 cell (a Cyclone teleporter), circling
/// and arcing up and down on the way, its follower with it.
void Cyclone_CarryLeader(void) {
  struct FieldEffectWork *effect = Ram_HeapSlots->fieldEffectWork;
  struct CycloneWork *work = Ram_HeapSlots->battleEffect;
  struct Actor *owner = effect->mainActor;
  s32 timer = work->timer;
  s32 position[3];

  switch (work->state) {
  case 0:
    work->radius = Trig_Sin(timer << 8) << 5;
    work->angle += 0x500;
    position[0] = work->from[0] + (work->to[0] - work->from[0]) * timer / 128;
    position[1] = work->from[1] + (work->to[1] - work->from[1]) * timer / 128 +
                  (Trig_Sin(timer << 8) << 6);
    position[2] = work->from[2] + (work->to[2] - work->from[2]) * timer / 128;
    Vector_AddPolarOffset(work->radius, work->angle, position);
    SET_ACTOR_POSITION(owner, position[0], position[1], position[2]);
    if (timer > 127) {
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

/// Cyclone_CarryLeader for a kind 3 cell: the leader is carried there 32
/// pixels up, circling, without the arc.
void Cyclone_LiftLeader(void) {
  struct FieldEffectWork *effect = Ram_HeapSlots->fieldEffectWork;
  struct CycloneWork *work = Ram_HeapSlots->battleEffect;
  struct Actor *owner = effect->mainActor;
  s32 timer = work->timer;
  s32 position[3];

  switch (work->state) {
  case 0:
    work->radius = Trig_Sin(timer << 8) << 5;
    work->angle += 0x500;
    position[0] = work->from[0] + (work->to[0] - work->from[0]) * timer / 128;
    position[1] = work->from[1] + (work->to[1] - work->from[1]) * timer / 128;
    position[2] = work->from[2] + (work->to[2] - work->from[2]) * timer / 128;
    Vector_AddPolarOffset(work->radius, work->angle, position);
    SET_ACTOR_POSITION(owner, position[0], position[1], position[2]);
    if (timer > 127) {
      work->state++;
      work->timer = -1;
    }
    break;
  case 1:
    owner->y.fixed -= 0x6666;
    if (effect->childOption == 1) {
      if (timer == 40)
        work->state = FIELD_PSYNERGY_FINISHED;
    } else if (timer == 40) {
      work->state++;
      work->timer = -1;
    }
    break;
  case 2:
    owner->y.fixed += 0x3cccc;
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
  struct CycloneWork *work = HeapSlot_Alloc(HEAP_SLOT_BATTLE_EFFECT, 0x1b38);
  struct FieldWork *event = Ram_HeapSlots->fieldWork;
  struct FieldEffectWork *effect = Ram_HeapSlots->fieldEffectWork;
  struct Actor *owner = effect->mainActor;
  s16 *points = &event->psynergyCells[0];
  s32 spread = px(4);
  s32 step = 0xe000;
  struct CanvasSpriteWork *canvas;
  struct Actor *ring;
  u16 *offsets;
  u8 *pixels;
  u16 *palette;
  s32 pos[3];
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
  palette = (u16 *)&OBJ_PLTT_COLOR(14, 0);
  blue = 31 << 10;
  for (j = 0; j < 16; j++) {
    c = j + 16;
    /* FAKEMATCH(dead): an empty once-only loop. */
    do {
    } while (0);
    *palette++ = (c << 5) | blue | c;
    /* FAKEMATCH(barrier): an empty asm. */
    asm("");
  }
  BattleEffect_StartCanvasSprite();
  canvas = Ram_HeapSlots->canvasSprite;
  pos[0] = effect->mainActor->x.fixed;
  pos[1] = effect->mainActor->y.fixed;
  pos[2] = effect->mainActor->z.fixed;
  Camera_WorldToScreen(pos);
  canvas->x = (s16)FIELD_PIXEL(pos[0]);
  canvas->y = (s16)FIELD_PIXEL(pos[2]);
  canvas->priority = 1;
  canvas->depth = 240;
  canvas->mode = CANVAS_SPRITE_KEPT;
  canvas->scaleX = Q16_ONE * 3 / 2;
  canvas->scaleY = Q16_ONE * 3 / 2;
  EffectSprite_Init(&canvas->sprite, 32, 32, OBJ_SIZE_64X64, canvas->tile);
  canvas->sprite.oam.palette256 = 1;
  canvas->sprite.oam.palette = 0;
  offsets = Ram_HeapSlots->canvasMap;
  pixels = Ram_HeapSlots->battleCanvas;
  ring =
      Actor_Spawn(280, owner->x.fixed, owner->y.fixed - px(6), owner->z.fixed);
  Actor_SelectAnimationFar(ring, 2);
  ring->scaleX = Q16_ONE * 3 / 2;
  ring->scaleY = 0;
  wait(1);
  Sound_PlayFar(SE_RUSH);
  for (i = 0; i < 20; i++) {
    ring->scaleY = (i << 16) / 20;
    wait(1);
  }
  for (i = 0; i < 60; i++) {
    for (j = 0; j < 4; j++)
      Cyclone_PlotArc((s32)(spread + Random16() * 4 - (j << 16)) >> 16,
                      Random16(), offsets, pixels);
    spread += step;
    if (spread > px(28))
      spread = px(28);
    step = step * 123 / 128;
    canvas->angle += TRIG_TURN / 64;
    canvas->command = 3;
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
  if (event->unkd40 != -1)
    mode = 3;
  else if (event->unkd3c != -1)
    mode = 2;
  else {
    point = points[0];
    mode = point != -1;
  }

  if (mode == 3) {
    if (work->kind)
      ParticleBurst_Start();
    for (i = 0; i < 16; i++) {
      canvas->angle += TRIG_TURN / 64;
      canvas->command = 3;
      wait(1);
    }
    FieldCamera_MoveWithinMapBounds(-1, -1, -1, 0);
    Actor_SetPositionAndResetMotionFar(owner, owner->x.fixed, owner->y.fixed,
                                       owner->z.fixed);
    Actor_SetSpriteFlagsFar(owner, 0);
    Actor_SetSpritePriority(effect->firstActorId, 1);
    owner->motionFlags = 0;
    frame = 0;
    wait(1);
    work->to[0] = event->unkd40 << 16;
    work->to[1] = owner->y.fixed + px(32);
    work->to[2] = event->unkd42 << 16;
    Task_Add(Cyclone_LiftLeader, TASK_PASS_VBLANK);
    while (work->state != FIELD_PSYNERGY_FINISHED) {
      /* FAKEMATCH(barrier): an empty asm that opens the wait loop. */
      asm("");
      if (frame == 30)
        FieldPsynergy_RunTouchedEvents();
      wait(1);
      frame++;
    }
    Task_Remove(Cyclone_LiftLeader);
    if (effect->childOption == 0)
      owner->scaleX = 0;
    ring->scaleX = 0;
    if (work->kind)
      BattleEffect_EndX();
  } else if (mode == 2) {
    if (work->kind)
      ParticleBurst_Start();
    for (i = 0; i < 16; i++) {
      canvas->angle += TRIG_TURN / 64;
      canvas->command = 3;
      wait(1);
    }
    FieldCamera_MoveWithinMapBounds(-1, -1, -1, 0);
    Actor_SetPositionAndResetMotionFar(owner, owner->x.fixed, owner->y.fixed,
                                       owner->z.fixed);
    Actor_SetSpriteFlagsFar(owner, 0);
    owner->motionFlags = 0;
    wait(1);
    work->to[0] = event->unkd3c << 16;
    work->to[1] = owner->y.fixed;
    work->to[2] = event->unkd3e << 16;
    Task_Add(Cyclone_CarryLeader, TASK_PASS_VBLANK);
    frame = 0;
    while (work->state != FIELD_PSYNERGY_FINISHED) {
      /* FAKEMATCH(barrier): an empty asm that opens the wait loop. */
      asm("");
      if (frame == 30)
        FieldPsynergy_RunTouchedEvents();
      wait(1);
      frame++;
    }
    Task_Remove(Cyclone_CarryLeader);
    for (i = 0; i < 16; i++) {
      owner->scaleX += -0x800;
      owner->scaleY = owner->scaleX;
      wait(1);
    }
    owner->scaleX = 0;
    for (i = 0; i < 32; i++) {
      ring->scaleX += -0x800;
      ring->scaleY += -0x200;
      wait(1);
    }
    ring->scaleX = 0;
    if (work->kind)
      BattleEffect_EndX();
  } else if (mode == 1) {
    Sound_PlayFar(SE_WHOOSH);
    Actor_SelectAnimationFar(ring, 6);
    if (work->kind)
      ParticleBurst_Start();
    for (i = 0; i < 16; i++) {
      canvas->angle += TRIG_TURN / 64;
      canvas->command = 3;
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
      canvas->command = 3;
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
  s32 x, y, z;
  s32 spin;
  s32 position[3];
  s32 i;

  FieldCamera_MoveWithinMapBounds(-1, -1, -1, 0);
  object->motionFlags = 0;
  object->update = BattleEffect_SpinObject;
  ring = Actor_Spawn(280, object->x.fixed, object->y.fixed, object->z.fixed);
  Actor_SelectAnimationFar(ring, 2);
  ring->scaleX = Q16_ONE * 3 / 2;
  ring->scaleY = Q16_ONE;
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
    SET_ACTOR_POSITION(ring, object->x.fixed, object->y.fixed - px(6),
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
  ring = Actor_Spawn(280, object->x.fixed, object->y.fixed, object->z.fixed);
  Actor_SelectAnimationFar(ring, 2);
  ring->scaleX = Q16_ONE / 2;
  ring->scaleY = Q16_ONE * 3 / 4;
  for (i = 0; i < 32; i++) {
    ring->scaleX += 0x800;
    ring->scaleY += 0x200;
    wait(1);
  }
  ring->scaleX = Q16_ONE * 3 / 2;
  ring->scaleY = Q16_ONE;
  Sound_PlayFar(SE_WHOOSH);
  Actor_SelectAnimationFar(ring, 6);
  object->scaleY = object->scaleX = Q16_ONE / 2;
  for (i = 0; i < 16; i++) {
    object->scaleY = object->scaleX = object->scaleX + 0x800;
    wait(1);
  }
  Actor_SetSpriteFlagsFar(Actor_GetById(actor), 1);
  Event_Wait(10);
  Actor_ReleaseFar(ring);
  object->update = NULL;
  object->facing = angle;
}

// The spray's drops and smoke puffs, moved and drawn each frame.

/// Blaze's work (heap slot HEAP_SLOT_BATTLE_EFFECT): the flame tiles, 16 flames
/// and 16 smoke puffs with their motion, where the jet starts and where it
/// points, and the actor it has reached.
struct BlazeWork {
  u8 unused[0x1000]; ///< no code names it
  struct EffectSprite drops[16];
  struct FieldPsynergyParticle dropMotion[16];
  struct EffectSprite puffs[16];
  struct FieldPsynergyParticle puffMotion[16];
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
  s8 mode;
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
/// at most 0x400 a frame.
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
    for (i = 0; i <= 15; i++, particle++) {
      if (particle->timer == -1) {
        sprite = &work->drops[i];
        if (scene->child != NULL)
          sprite->depth = BattleEffect_GetObjectDepth(scene->child);
        else
          sprite->depth = BattleEffect_GetObjectDepth(scene->mainActor);
        if (work->mode == 3) {
          work->speed = 0x26666;
          work->lifetime = 33;
          work->gravity = px(3) / 32;
        } else {
          work->speed = 0x1cccc;
          work->lifetime = 25;
          work->gravity = px(1) / 8;
        }
        SET_POSITION(particle, work->origin[0], work->origin[1],
                     work->origin[2]);
        particle->velocityX = 0;
        particle->velocityY = -0x13333;
        particle->velocityZ = 0;
        Vector_AddPolarOffset(work->speed, work->angle, &particle->velocityX);
        particle->timer = 0;
        break;
      }
    }
  }

  work->hit = 0;
  particle = work->dropMotion;
  for (i = 0; i <= 15; i++, particle++) {
    if (particle->timer == -1)
      continue;
    sprite = &work->drops[i];
    sprite->scaleY = sprite->scaleX = particle->timer * 0xa3d + Q16_ONE / 2;
    sprite->oam.index =
        work->dropTile + (((particle->timer >> 1) + i) & 3) * 32;
    if (work->angle > TRIG_TURN / 2)
      sprite->depth--;
    else
      sprite->depth++;
    EffectSprite_DrawAtWorldPosition(sprite, particle);
    if (++particle->timer == work->lifetime)
      particle->timer = -1;
    for (j = 0; j < 80; j++) {
      struct Actor *other = event->actors[j];
      if (other != NULL && other->script != NULL &&
          (other->collisionFlags & 1) && other != scene->mainActor &&
          other != scene->child &&
          FieldPsynergy_CheckOverlap(&particle->x, 2, &other->x.fixed,
                                     other->radius - 2) >= 0) {
        work->hit = 1;
        work->hitObject = j;
        particle->timer = -1;
        break;
      }
    }
    if ((FIELD_CELL(particle->x)) != (work->origin[0] >> 20) ||
        (FIELD_CELL(particle->z)) != (work->origin[2] >> 20)) {
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
      Vector_AddPolarOffset(px(10), work->angle + TRIG_TURN / 2, pos);
      Vector_AddPolarOffset(px(12), Random16(), pos);
      puff = &work->puffMotion[i];
      SET_POSITION(puff, pos[0], pos[1], pos[2]);
      puff->timer = 0;
    }
    if (work->hit != 0)
      break;
    BattleEffect_IntegrateVector3(&particle->x, 64, work->gravity);
  }

  particle = work->puffMotion;
  for (i = 0; i <= 15; i++, particle++) {
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
      particle->y += 0xcccc;
      if (++particle->timer == 32)
        particle->timer = -1;
    }
  }

  target = scene->angle8 - TRIG_TURN / 4;
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
