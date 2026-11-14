/// @file
/// Field effects that event commands start, in ROM order: the 96-particle
/// burst, an actor carried from one point to another as it circles, a whirl of
/// rings, an actor spinning away and back (Actor_DepartInRing,
/// Actor_ArriveInRing) and a spray of drops and smoke puffs.
// Part of radial.c.

#include "../radial_internal.h"
#include "sounds.h"
#include "macros.h"

struct BurstSprite {
  u8 unk00[8];
  u16 tile : 10;
  u16 unk08_10 : 6;
  u8 unk0a[2];
  s32 x;
  s32 y;
  u8 unk14[20];
};

/* VIEW(psynergy): tag struct BurstWork as struct BurstWork__psynergy;
 * module/field/event_commands.h has another definition; tried with the
 * definition the rest of the object has, it does not compile in tla-ja:
 * games/tla/src/field/event_commands/radial/cyclone.c:22: redefinition of
 * `struct BurstWork'. */
#define BurstWork BurstWork__psynergy
struct BurstWork {
  u8 tiles[384];
  struct BurstSprite sprites[96];
  struct BurstParticle particles[96];
  s16 resource;
  s16 vram;
  s16 flash;
  s16 frame;
  s16 active;
};

void Vector_AddPolarOffset(s32 magnitude, s32 angle, s32 *position);
/* VIEW(psynergy): routine Camera_WorldToScreen as void Camera_WorldToScreen(s32
 * *position); module/field/event_commands.h has void
 * Camera_WorldToScreen(struct EffectVector *position); tried as the
 * definition's void Camera_WorldToScreen(struct EffectVector *position), which
 * the object already declares, it does not compile in tla-ja:
 * games/tla/src/field/event_commands/radial/sand_sinking.c:54: conflicting
 * types for `Camera_WorldToScreen'. */
void Camera_WorldToScreen__psynergy(s32 *position) __asm__(
    "Camera_WorldToScreen");
#define Camera_WorldToScreen Camera_WorldToScreen__psynergy
#include "effect_sprite.h"

/// The 96-particle burst's task (heap slot 0x5c work, started by
/// ParticleBurst_Start). Each particle whose delay counter is between 0 and 63
/// shows the picture (delay / 8) % 6 of the burst's tiles 48 pixels out along
/// its heading, then turns by its spin, rises by its speed and spins eight
/// units faster; every counter advances. The work's flash flag is set on frame
/// 30 only, and the burst stops being active on frame 112.
void ParticleBurst_Update(void) {
  struct BurstWork *work;
  struct BurstParticle *particle;
  struct BurstSprite *sprite;
  s32 position[3];
  s32 i;

  work = (struct BurstWork *)Ram_HeapSlots->battleFxWork[0];
  particle = work->particles;

  for (i = 0; i < 96; i++, particle++) {
    sprite = &work->sprites[i];
    if ((u32)particle->delay < 64) {
      sprite->tile = work->vram + (particle->delay >> 3) % 6 * 2;
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

#include "graphics/resource.h"
#include "system/task.h"

void BattleEffect_EndX(void) {
  u8 *work = Ram_HeapSlots->battleFxWork[0];

  Task_Remove((u32)ParticleBurst_Update);
  VramCache_FreeEntry(*(s16 *)(work + 0x1b00));
}

#include "fixed_math.h"
#include "actor.h"

struct CarryWork {
  u8 unk0000[0x1b0c];
  struct Actor *follower;
  s16 state;
  s16 timer;
  s32 angle;
  s32 radius;
  s32 from[3];
  s32 to[3];
};

/// Task of the Cyclone: the whirlwind carries the leader over 128 frames from
/// where it stood to the nearby kind 2 cell (a Cyclone teleporter), circling
/// and arcing up and down on the way, its follower with it.
void Cyclone_CarryLeader(void) {
  u8 *effect = Ram_HeapSlots->effectWork;
  struct CarryWork *work = (struct CarryWork *)Ram_HeapSlots->battleFxWork[0];
  struct Actor *owner = *(struct Actor **)(effect + 16);
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
    SET_POSITION(owner, position[0], position[1], position[2]);
    if (timer > 127) {
      work->state++;
      work->timer = -1;
    }
    break;
  case 1:
    work->state = 999;
    break;
  }
  work->timer++;
  work->follower->x = owner->x;
  work->follower->y = owner->y;
  work->follower->z = owner->z;
}

/// Cyclone_CarryLeader for a kind 3 cell: the leader is carried there 32
/// pixels up, circling, without the arc.
void Cyclone_LiftLeader(void) {
  u8 *effect = Ram_HeapSlots->effectWork;
  struct CarryWork *work = (struct CarryWork *)Ram_HeapSlots->battleFxWork[0];
  struct Actor *owner = *(struct Actor **)(effect + 16);
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
    SET_POSITION(owner, position[0], position[1], position[2]);
    if (timer > 127) {
      work->state++;
      work->timer = -1;
    }
    break;
  case 1:
    owner->y -= 0x6666;
    if (*(s8 *)(effect + 52) == 1) {
      if (timer == 40)
        work->state = 999;
    } else if (timer == 40) {
      work->state++;
      work->timer = -1;
    }
    break;
  case 2:
    owner->y += 0x3cccc;
    if (timer == 40)
      work->state = 999;
    break;
  }
  work->timer++;
  work->follower->x = owner->x;
  work->follower->y = owner->y;
  work->follower->z = owner->z;
}

#include "battle/battle.h"
#include "debug.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "lib.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "battle/motobj.h"

void BattleEffect_SpinObject(struct MotionObject *object) {
  object->angle += TRIG_TURN / 16;
}

void FieldPsynergy_CycloneOnTarget(void) { FieldPsynergy_Cyclone(); }

#include "canvas_sprite.h"
#include "actor_view.h"

struct WhirlWork {
  u8 unk0000[0x1b08];
  s16 count;
  u8 unk1b0a[2];
  struct Actor *follower;
  s16 state;
  s16 timer;
  s32 angle;
  s32 radius;
  s32 from[3];
  s32 to[3];
  s8 kind;
};

void FieldPsynergy_GatherNearbyCells(void);
void BattleEffect_SetObjectUpdateMode(struct Actor *object, s32 mode);
struct Actor *Actor_Spawn(s32 kind, s32 x, s32 y, s32 z);
void Cyclone_PlotArc(s32 radius, s32 angle, u16 *offsets, u8 *pixels);
void ParticleBurst_Start(void);
void FieldPsynergy_RunTouchedEvents(void);
void FieldCamera_MoveWithinMapBounds(s32 x, s32 y, s32 z, s32 mode);

void FieldPsynergy_Cyclone(void) {
  struct WhirlWork *work = HeapSlot_Alloc(92, 0x1b38);
  u8 *event = Ram_HeapSlots->eventWork;
  u8 *effect = Ram_HeapSlots->effectWork;
  struct Actor *owner = *(struct Actor **)(effect + 16);
  s16 *points = (s16 *)(event + 0xcc0);
  s32 spread = 0x40000;
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
  BattleEffect_SetObjectUpdateMode(*(struct Actor **)(effect + 16), 0);
  owner->update = BattleEffect_SpinObject;
  owner->timer = owner->facing;
  palette = (u16 *)(OBJ_PLTT + 14 * 16);
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
  canvas = Ram_HeapSlots->unkec[1];
  pos[0] = (*(struct Actor **)(effect + 16))->x;
  pos[1] = (*(struct Actor **)(effect + 16))->y;
  pos[2] = (*(struct Actor **)(effect + 16))->z;
  Camera_WorldToScreen(pos);
  canvas->x = (s16)(pos[0] >> 16);
  canvas->y = (s16)(pos[2] >> 16);
  canvas->priority = 1;
  canvas->depth = 240;
  canvas->mode = CANVAS_SPRITE_KEPT;
  canvas->scaleX = Q16_ONE * 3 / 2;
  canvas->scaleY = Q16_ONE * 3 / 2;
  EffectSprite_Init(&canvas->sprite, 32, 32, 0xc0000000, canvas->tile);
  canvas->sprite.oam.palette256 = 1;
  canvas->sprite.oam.palette = 0;
  offsets = Ram_HeapSlots->canvasMap;
  pixels = Ram_HeapSlots->battleFxWork[1];
  ring = Actor_Spawn(280, owner->x, owner->y - 0x60000, owner->z);
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
    if (spread > 0x1c0000)
      spread = 0x1c0000;
    step = step * 123 / 128;
    canvas->angle += 0x400;
    canvas->command = 3;
    wait(1);
  }
  FieldPsynergy_FadeFromCastColor();
  work->follower = ring;
  work->from[0] = owner->x;
  work->from[1] = owner->y;
  work->from[2] = owner->z;
  work->to[0] = owner->x;
  work->to[1] = owner->y;
  work->to[2] = owner->z;
  work->angle = 0;
  work->state = 0;
  work->timer = 0;
  work->state = 0;
  if (*(s16 *)(event + 0xd40) != -1)
    mode = 3;
  else if (*(s16 *)(event + 0xd3c) != -1)
    mode = 2;
  else {
    point = points[0];
    mode = point != -1;
  }

  if (mode == 3) {
    if (work->kind)
      ParticleBurst_Start();
    for (i = 0; i < 16; i++) {
      canvas->angle += 0x400;
      canvas->command = 3;
      wait(1);
    }
    FieldCamera_MoveWithinMapBounds(-1, -1, -1, 0);
    Actor_SetPositionAndResetMotionFar(owner, owner->x, owner->y, owner->z);
    Actor_SetSpriteFlagsFar(owner, 0);
    Actor_SetSpritePriority(*(s16 *)(effect + 24), 1);
    owner->motionFlags = 0;
    frame = 0;
    wait(1);
    work->to[0] = *(s16 *)(event + 0xd40) << 16;
    work->to[1] = owner->y + 0x200000;
    work->to[2] = *(s16 *)(event + 0xd42) << 16;
    Task_Add(Cyclone_LiftLeader, 0x480);
    while (work->state != 999) {
      /* FAKEMATCH(barrier): an empty asm that opens the wait loop. */
      asm("");
      if (frame == 30)
        FieldPsynergy_RunTouchedEvents();
      wait(1);
      frame++;
    }
    Task_Remove(Cyclone_LiftLeader);
    if (*(s8 *)(effect + 52) == 0)
      owner->scaleX = 0;
    ring->scaleX = 0;
    if (work->kind)
      BattleEffect_EndX();
  } else if (mode == 2) {
    if (work->kind)
      ParticleBurst_Start();
    for (i = 0; i < 16; i++) {
      canvas->angle += 0x400;
      canvas->command = 3;
      wait(1);
    }
    FieldCamera_MoveWithinMapBounds(-1, -1, -1, 0);
    Actor_SetPositionAndResetMotionFar(owner, owner->x, owner->y, owner->z);
    Actor_SetSpriteFlagsFar(owner, 0);
    owner->motionFlags = 0;
    wait(1);
    work->to[0] = *(s16 *)(event + 0xd3c) << 16;
    work->to[1] = owner->y;
    work->to[2] = *(s16 *)(event + 0xd3e) << 16;
    Task_Add(Cyclone_CarryLeader, 0x480);
    frame = 0;
    while (work->state != 999) {
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
      canvas->angle += 0x400;
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
      owner->facing += TRIG_TURN / 16;
      canvas->angle += 0x400;
      canvas->command = 3;
      wait(1);
    }
  }
  Sound_PlayFar(SE_WHOOSH);
  Actor_ReleaseFar(ring);
  owner->update = NULL;
  owner->facing = owner->timer;
  BattleEffect_EndCanvasSprite();
  HeapSlot_Release(92);
}

struct Actor *Actor_GetById(u32 objectId);
void Event_Start(void);
void Event_ResolvePendingActions(s32 skip);
void Event_StartScreenTransition(void);
void Event_Wait(s32 frames);

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
  ring = Actor_Spawn(280, object->x, object->y, object->z);
  Actor_SelectAnimationFar(ring, 2);
  ring->scaleX = Q16_ONE * 3 / 2;
  ring->scaleY = Q16_ONE;
  x = object->x;
  y = object->y;
  z = object->z;
  spin = 0;
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_StartScreenTransition();
  for (i = 0; i < 128; i++) {
    position[0] = x;
    position[1] = y + Trig_Cos(i << 7) * 96;
    position[2] = z;
    Vector_AddPolarOffset(Trig_Cos(i << 7) << 5, spin, position);
    SET_POSITION(object, position[0], position[1], position[2]);
    spin += 0x400;
    SET_POSITION(ring, object->x, object->y - 0x60000, object->z);
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

void Event_WaitTransitionFrames(void);

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
  ring = Actor_Spawn(280, object->x, object->y, object->z);
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

#include "system/frame.h"

struct SprayParticle {
  s32 x;
  s32 y;
  s32 z;
  s32 velocityX;
  s32 velocityY;
  s32 velocityZ;
  s32 timer;
};

struct SprayWork {
  u8 unk0000[0x1000];
  struct EffectSprite drops[16];
  struct SprayParticle dropMotion[16];
  struct EffectSprite puffs[16];
  struct SprayParticle puffMotion[16];
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

struct SprayScene {
  u16 angleFraction;
  u16 angle;
  s32 x;
  s32 y;
  s32 z;
  struct Actor *mainObject;
  struct Actor *otherObject;
};

struct SprayEvent {
  u8 unk00[20];
  struct Actor *objects[80];
};

s32 BattleEffect_GetObjectDepth(struct Actor *object);
#define EffectSprite_DrawAtWorldPosition EffectSprite_DrawAtWorldPosition
s32 MapGrid_CheckCollisionFar(struct Actor *object, s32 *position);
s32 FieldPsynergy_CheckOverlap(s32 *a, s32 radiusA, s32 *b, s32 radiusB);

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
  struct SprayWork *work = (struct SprayWork *)Ram_HeapSlots->battleFxWork[0];
  struct SprayScene *scene = Ram_HeapSlots->effectWork;
  struct SprayEvent *event = Ram_HeapSlots->eventWork;
  struct EffectSprite *sprite;
  struct SprayParticle *particle;
  struct SprayParticle *puff;
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
        if (scene->otherObject != NULL)
          sprite->depth = BattleEffect_GetObjectDepth(scene->otherObject);
        else
          sprite->depth = BattleEffect_GetObjectDepth(scene->mainObject);
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
      struct Actor *other = event->objects[j];
      if (other != NULL && other->script != NULL &&
          (other->collisionFlags & 1) && other != scene->mainObject &&
          other != scene->otherObject &&
          FieldPsynergy_CheckOverlap(&particle->x, 2, &other->x,
                                     other->radius - 2) >= 0) {
        work->hit = 1;
        work->hitObject = j;
        particle->timer = -1;
        break;
      }
    }
    if ((particle->x >> 20) != (work->origin[0] >> 20) ||
        (particle->z >> 20) != (work->origin[2] >> 20)) {
      pos[0] = particle->x;
      pos[1] = particle->y;
      pos[2] = particle->z;
      if (MapGrid_CheckCollisionFar(scene->mainObject, pos) > 0)
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

  target = scene->angle - TRIG_TURN / 4;
  turn = (s16)(work->targetAngle - work->angle);
  if ((u16)(work->targetAngle - target) - (u16)(work->angle - target) ==
      0x8000) {
    work->angle = work->angle + 0x400;
  } else {
    if (turn > 0x400)
      turn = 0x400;
    if (turn < -0x400)
      turn = -0x400;
    work->angle = work->angle + turn;
  }
}

struct ControlEvent {
  u8 unk000[362];
  u16 cue;
};

void FieldPsynergy_Blaze(void);
void FieldPsynergy_Sand(void);
void FieldPsynergy_Burst(void);
void FieldPsynergy_Pound(void);
void FieldPsynergy_Scoop(void);
void FieldPsynergy_Parch(void);
void FieldPsynergy_Hover(void);
void Teleport_PlaySparks(void);
s32 Party_GetLeaderActor(void);

/// Effect 26 cast on a target: runs the effect, ignoring the target.
void FieldPsynergy_BlazeOnTarget(void) { FieldPsynergy_Blaze(); }
