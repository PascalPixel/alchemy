/// @file
/// Field effects that event commands start, in ROM order: the 96-particle
/// burst, an actor carried from one point to another as it circles, a whirl of
/// rings, an actor spinning away and back (Func_080e3698, Func_080e37e0) and a
/// spray of drops and smoke puffs.

// ---- games/tla/src/field/event_commands/f0e2c38.c ----

#include "types.h"
#include "ram_buffer.h"

struct BurstSprite {
  u8 unk00[8];
  u16 tile : 10;
  u16 unk08_10 : 6;
  u8 unk0a[2];
  s32 x;
  s32 y;
  u8 unk14[20];
};

struct BurstParticle {
  s32 x;
  s32 y;
  s32 z;
  s32 heading;
  s32 speed;
  s32 spin;
  s32 delay;
};

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
void Camera_WorldToScreen(s32 *position);
#include "effect_sprite.h"

/// The 96-particle burst's task (heap slot 0x5c work, started by
/// Func_080e2acc). Each particle whose delay counter is between 0 and 63 shows
/// the picture (delay / 8) % 6 of the burst's tiles 48 pixels out along its
/// heading, then turns by its spin, rises by its speed and spins eight units
/// faster; every counter advances. The work's flash flag is set on frame 30
/// only, and the burst stops being active on frame 112.
void Func_080e2c38(void) {
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
      Vector_AddPolarOffset(0x300000, particle->heading, position);
      Camera_WorldToScreen(position);
      sprite->x = position[0];
      sprite->y = position[2];
      Func_080eb01c(sprite);
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

#include "resource.h"
#include "task.h"

void BattleEffect_EndX(void) {
  u8 *work = Ram_HeapSlots->battleFxWork[0];

  Task_Remove((u32)Func_080e2c38);
  Resource_ResetEntry(*(s16 *)(work + 0x1b00));
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

void Func_080e2d64(void) {
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
    owner->x = position[0];
    owner->y = position[1];
    owner->z = position[2];
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

void Func_080e2ec0(void) {
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
    owner->x = position[0];
    owner->y = position[1];
    owner->z = position[2];
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

#include "battle.h"
#include "debug.h"
/* VIEW(f0e2c38): routine BattleEffect_DropObjectAtAnchor as void BattleEffect_DropObjectAtAnchor(u32 objectId); the definition has void BattleEffect_DropObjectAtAnchor(s32 id). */
extern void BattleEffect_DropObjectAtAnchor__f0e2c38(u32 objectId) __asm__("BattleEffect_DropObjectAtAnchor");
#define BattleEffect_DropObjectAtAnchor BattleEffect_DropObjectAtAnchor__f0e2c38
#include "field.h"
#undef BattleEffect_DropObjectAtAnchor
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "motobj.h"

void BattleEffect_SpinObject(struct MotionObject *object) {
  object->angle += 0x1000;
}

void BattleEffect_CallSpinEffect(void) { Func_080e3074(); }

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

void Func_080e2264(void);
void BattleEffect_SetObjectUpdateMode(struct Actor *object, s32 mode);
struct Actor *Actor_Spawn(s32 kind, s32 x, s32 y, s32 z);
void Func_080e2a68(s32 radius, s32 angle, u16 *offsets, u8 *pixels);
void Func_080e2acc(void);
void Func_080e249c(void);
void Camera_MoveWithinMapBounds(s32 x, s32 y, s32 z, s32 mode);

void Func_080e3074(void) {
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

  BattleEffect_InitializeSharedScene();
  Func_080e2264();
  BattleEffect_SetObjectUpdateMode(*(struct Actor **)(effect + 16), 0);
  owner->update = BattleEffect_SpinObject;
  owner->timer = owner->facing;
  palette = (u16 *)0x050003c0;
  blue = 31 << 10;
  for (j = 0; j < 16; j++) {
    c = j + 16;
    // An empty once-only loop keeps c's add ahead of the colour's copy;
    // without it, 2 of 713 instructions differ in tla-en.
    do {
    } while (0);
    *palette++ = (c << 5) | blue | c;
    /* FAKEMATCH(barrier): an empty asm that ends the loop's body, which keeps
     * the counters' updates after the colour's store; without it, 4 of 713
     * instructions differ in tla-en. */
    asm("");
  }
  Func_080eb824();
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
  canvas->scaleX = 0x18000;
  canvas->scaleY = 0x18000;
  Func_080eaf98(&canvas->sprite, 32, 32, 0xc0000000, canvas->tile);
  canvas->sprite.oam.palette256 = 1;
  canvas->sprite.oam.palette = 0;
  offsets = Ram_HeapSlots->canvasMap;
  pixels = Ram_HeapSlots->battleFxWork[1];
  ring = Actor_Spawn(280, owner->x, owner->y - 0x60000, owner->z);
  Actor_SetMode(ring, 2);
  ring->scaleX = 0x18000;
  ring->scaleY = 0;
  wait(1);
  Sound_PlayFar(207);
  for (i = 0; i < 20; i++) {
    ring->scaleY = (i << 16) / 20;
    wait(1);
  }
  for (i = 0; i < 60; i++) {
    for (j = 0; j < 4; j++)
      Func_080e2a68((s32)(spread + Random16() * 4 - (j << 16)) >> 16,
                    Random16(), offsets, pixels);
    spread += step;
    if (spread > 0x1c0000)
      spread = 0x1c0000;
    step = step * 123 / 128;
    canvas->angle += 0x400;
    canvas->command = 3;
    wait(1);
  }
  BattleEffect_PrepareBufferInterpolation();
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
      Func_080e2acc();
    for (i = 0; i < 16; i++) {
      canvas->angle += 0x400;
      canvas->command = 3;
      wait(1);
    }
    Camera_MoveWithinMapBounds(-1, -1, -1, 0);
    Actor_SetPositionAndResetMotionFar(owner, owner->x, owner->y, owner->z);
    Actor_SetSingleChildFlagsFar(owner, 0);
    Actor_SetSpritePriority(*(s16 *)(effect + 24), 1);
    owner->motionFlags = 0;
    frame = 0;
    wait(1);
    work->to[0] = *(s16 *)(event + 0xd40) << 16;
    work->to[1] = owner->y + 0x200000;
    work->to[2] = *(s16 *)(event + 0xd42) << 16;
    Task_Add(Func_080e2ec0, 0x480);
    while (work->state != 999) {
      /* FAKEMATCH(barrier): an empty asm that opens the wait loop, which keeps
       * 999's copy to r8 ahead of the state's address; without it, 2 of 713
       * instructions differ in tla-en. */
      asm("");
      if (frame == 30)
        Func_080e249c();
      wait(1);
      frame++;
    }
    Task_Remove(Func_080e2ec0);
    if (*(s8 *)(effect + 52) == 0)
      owner->scaleX = 0;
    ring->scaleX = 0;
    if (work->kind)
      BattleEffect_EndX();
  } else if (mode == 2) {
    if (work->kind)
      Func_080e2acc();
    for (i = 0; i < 16; i++) {
      canvas->angle += 0x400;
      canvas->command = 3;
      wait(1);
    }
    Camera_MoveWithinMapBounds(-1, -1, -1, 0);
    Actor_SetPositionAndResetMotionFar(owner, owner->x, owner->y, owner->z);
    Actor_SetSingleChildFlagsFar(owner, 0);
    owner->motionFlags = 0;
    wait(1);
    work->to[0] = *(s16 *)(event + 0xd3c) << 16;
    work->to[1] = owner->y;
    work->to[2] = *(s16 *)(event + 0xd3e) << 16;
    Task_Add(Func_080e2d64, 0x480);
    frame = 0;
    while (work->state != 999) {
      /* FAKEMATCH(barrier): an empty asm that opens the wait loop, which keeps
       * 999's copy to r8 ahead of the state's address; without it, 2 of 713
       * instructions differ in tla-en. */
      asm("");
      if (frame == 30)
        Func_080e249c();
      wait(1);
      frame++;
    }
    Task_Remove(Func_080e2d64);
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
    Sound_PlayFar(136);
    Actor_SetMode(ring, 6);
    if (work->kind)
      Func_080e2acc();
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
        Func_080e249c();
      wait(1);
      frame++;
    }
    if (work->kind)
      BattleEffect_EndX();
  } else {
    Sound_PlayFar(136);
    Actor_SetMode(ring, 6);
    for (i = 0; i < 16; i++) {
      owner->facing += 0x1000;
      canvas->angle += 0x400;
      canvas->command = 3;
      wait(1);
    }
  }
  Sound_PlayFar(136);
  Actor_Destroy(ring);
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

void Func_080e3698(s32 actor) {
  struct Actor *object = Actor_GetById(actor);
  s32 angle = object->facing;
  u8 motionFlags = object->motionFlags;
  struct Actor *ring;
  s32 x, y, z;
  s32 spin;
  s32 position[3];
  s32 i;

  Camera_MoveWithinMapBounds(-1, -1, -1, 0);
  object->motionFlags = 0;
  object->update = BattleEffect_SpinObject;
  ring = Actor_Spawn(280, object->x, object->y, object->z);
  Actor_SetMode(ring, 2);
  ring->scaleX = 0x18000;
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
    object->x = position[0];
    object->y = position[1];
    object->z = position[2];
    spin += 0x400;
    ring->x = object->x;
    ring->y = object->y - 0x60000;
    ring->z = object->z;
    wait(1);
  }
  Actor_SetPositionAndResetMotionFar(object, x, y, z);
  Sound_PlayFar(136);
  Actor_SetMode(ring, 6);
  Event_Wait(15);
  Actor_Destroy(ring);
  object->update = NULL;
  object->facing = angle;
  object->motionFlags = motionFlags;
  wait(1);
  Camera_FollowActor(actor, 1);
}

void Event_WaitTransitionFrames(void);

/// An event command, the arrival counterpart of Func_080e3698. The actor
/// spins (BattleEffect_SpinObject) at zero width and its sprite is hidden while
/// object 280 grows on it for 32 frames; object 280 then snaps to 1.5 by 1
/// (cue 136, mode 6) while the actor widens back from half size over 16 frames
/// and shows again. Object 280 is destroyed and the actor's update and facing
/// are restored.
void Func_080e37e0(s32 actor) {
  struct Actor *object = Actor_GetById(actor);
  s32 angle = object->facing;
  struct Actor *ring;
  s32 i;

  object->update = BattleEffect_SpinObject;
  object->scaleX = 0;
  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_SetSingleChildFlagsFar(Actor_GetById(actor), 0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  ring = Actor_Spawn(280, object->x, object->y, object->z);
  Actor_SetMode(ring, 2);
  ring->scaleX = 0x8000;
  ring->scaleY = 0xc000;
  for (i = 0; i < 32; i++) {
    ring->scaleX += 0x800;
    ring->scaleY += 0x200;
    wait(1);
  }
  ring->scaleX = 0x18000;
  ring->scaleY = Q16_ONE;
  Sound_PlayFar(136);
  Actor_SetMode(ring, 6);
  object->scaleY = object->scaleX = 0x8000;
  for (i = 0; i < 16; i++) {
    object->scaleY = object->scaleX = object->scaleX + 0x800;
    wait(1);
  }
  Actor_SetSingleChildFlagsFar(Actor_GetById(actor), 1);
  Event_Wait(10);
  Actor_Destroy(ring);
  object->update = NULL;
  object->facing = angle;
}

// The spray's drops and smoke puffs, moved and drawn each frame.

#include "frame.h"

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
void EffectSprite_DrawAtWorldPosition(struct EffectSprite *sprite,
                                      struct SprayParticle *particle);
s32 Func_08020210(struct Actor *object, s32 *position);
s32 Func_080dbe80(s32 *a, s32 radiusA, s32 *b, s32 radiusB);

/// The frame callback of Func_080e3d04's spray. Every fourth frame while it
/// is emitting, a free one of sixteen drops starts at the origin with the
/// mode's speed (mode 3 faster, longer and lighter) along the spray's angle.
/// Each live drop grows, cycles its four tiles, is drawn at its position, and
/// dies after the mode's lifetime, on reaching an object of the event work's
/// list (whose index is kept and which stops the spray for the frame) or on
/// something solid; an odd drop that dies leaves one of sixteen puffs of smoke
/// that rise for 32 frames. The angle then turns toward the scene's facing by
/// at most 0x400 a frame.
void Func_080e38c8(void) {
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
          work->gravity = 0x1800;
        } else {
          work->speed = 0x1cccc;
          work->lifetime = 25;
          work->gravity = 0x2000;
        }
        particle->x = work->origin[0];
        particle->y = work->origin[1];
        particle->z = work->origin[2];
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
    sprite->scaleY = sprite->scaleX = particle->timer * 0xa3d + 0x8000;
    sprite->oam.index =
        work->dropTile + (((particle->timer >> 1) + i) & 3) * 32;
    if (work->angle > 0x8000)
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
          Func_080dbe80(&particle->x, 2, &other->x, other->radius - 2) >= 0) {
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
      if (Func_08020210(scene->mainObject, pos) > 0)
        particle->timer = -1;
    }
    if (particle->timer == -1 && (i & 1)) {
      pos[0] = particle->x;
      pos[1] = particle->y - (Random16() << 2);
      pos[2] = particle->z;
      Vector_AddPolarOffset(0xa0000, work->angle + 0x8000, pos);
      Vector_AddPolarOffset(0xc0000, Random16(), pos);
      puff = &work->puffMotion[i];
      puff->x = pos[0];
      puff->y = pos[1];
      puff->z = pos[2];
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
      Func_080eb01c(sprite);
      particle->y += 0xcccc;
      if (++particle->timer == 32)
        particle->timer = -1;
    }
  }

  target = scene->angle - 0x4000;
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

void Func_080e3d04(void);
void Func_080e4a34(void);
void Func_080e572c(void);
void Func_080e5d5c(void);
void Func_080e68d0(void);
void Func_080e7844(void);
void Func_080e9af4(void);
void Func_080e74d8(void);
s32 Party_GetLeaderActor(void);

/// Effect 26 cast on a target: runs the effect, ignoring the target.
void BattleEffect_CallEffect26(void) { Func_080e3d04(); }

// ---- games/tla/src/field/event/f0e3d04.c ----
#include "input.h"


/* VIEW(f0e3d04): tag struct SprayWork as struct SprayWork__f0e3d04; f0e2c38.c has another definition. */
struct SprayWork__f0e3d04 {
  u8 tiles[0x1000];
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

struct FxScene {
  u16 angleFraction;
  u16 angle;
  s32 x;
  s32 y;
  s32 z;
  struct Actor *mainObject;
  struct Actor *child;
  s16 firstObjectId;
  s16 secondObjectId;
  u16 action;
  s16 fieldKind;
  s8 enabled;
};

struct FxEvent {
  u8 unk00[20];
  struct Actor *objects[80];
  u8 unk154[0xd30 - 0x154];
  u8 unkd30;
  u8 unkd31[7];
  s8 unkd38;
};

struct ObjectAnimation {
  u8 unk00[40];
  s16 *definition;
};

struct ObjectMetadata {
  u8 unk00[8];
  s8 height;
};

extern const s32 Data_080f0f90[];
extern char ResourceId_RedFlameBurst, ResourceId_VioletFlameBurst,
    ResourceId_YellowFlameBurst, ResourceId_BlueFlameBurst,
    ResourceId_DustPuffA;

void *HeapSlot_Alloc(s32 slot, s32 size);
void HeapSlot_Release(s32 slot);
s32 Func_08020330(struct Actor *object);
void Field_StartColorTransition(s32 frames, s32 color);
s32 Func_080cdac0(s32 effect, s32 from, s32 to);
struct ObjectMetadata *Resource_GetMetadataRecordFarFar(s32 id);
struct Actor *Actor_Spawn(s32 kind, s32 x, s32 y, s32 z);
void Actor_Destroy(struct Actor *object);
void wait(s32 frames);
void BattleEffect_SetObjectUpdateMode(struct Actor *object, s32 mode);
void *Resource_GetTableEntry(s32 resourceId);
s32 Resource_DecodeType01(const void *source, void *destination);
s32 Resource_FindFreeEntry(void);
s32 BattleEffect_GetObjectDepth(struct Actor *object);
s32 BattleEffect_GetObjectPriority(struct Actor *object);
/* VIEW(f0e3d04): routine Func_080ce458 as void *Func_080ce458(u32 kind, s32 entry, s32 arg); battle.h has struct TriggerEvent *Func_080ce458(s32 filter, s32 kind, s32 selector). */
extern void *Func_080ce458__f0e3d04(u32 kind, s32 entry, s32 arg) __asm__("Func_080ce458");
/* VIEW(f0e3d04): routine Field_RunPsynergyEvent as void Field_RunPsynergyEvent(void *event, s32 a, s32 b); event_work.h has s32 Field_RunPsynergyEvent(struct PendingEvent *event, s32 mode, s32 target). */
extern void Field_RunPsynergyEvent__f0e3d04(void *event, s32 a, s32 b) __asm__("Field_RunPsynergyEvent");
void BattleEffect_PrepareBufferInterpolation(void);
void Func_080e38c8(void);

#define OBJECT_DEFINITION(object)                                              \
  (((struct ObjectAnimation *)(object)->sprite)->definition)

void Func_080e3d04(void) {
  struct SprayWork__f0e3d04 *work;
  struct FxEvent *event;
  struct FxScene *scene;
  struct Actor *caster;
  struct Actor *object;
  struct EffectSprite *sprite;
  union {
    struct Actor *object;
    struct SprayParticle *particle;
  } cursor;
  void *pending;
  s32 pos[3];
  s32 kind;
  s32 look;
  s32 best;
  s32 distance;
  s32 height;
  s32 id;
  s32 resourceId;
  s32 dropSlot;
  s32 puffSlot;
  s32 tile;
  s32 i;
  s32 j;
  s32 k;
  s32 mode;

  work = HeapSlot_Alloc(92, sizeof(struct SprayWork__f0e3d04));
  scene = Ram_HeapSlots->effectWork;
  event = Ram_HeapSlots->eventWork;
  caster = scene->mainObject;
  look = 0;
  object = scene->child;
  if (Func_08020330(object) == 418) {
    kind = ((u8 *)OBJECT_DEFINITION(object))[5];
    if (kind == 4)
      look = 1;
    if (kind == 1)
      look = 2;
    if (kind == 3)
      look = 3;
  }
  Field_StartColorTransition(8, Data_080f0f90[look]);
  work->mode = 0;
  if (scene->enabled != 0) {
    work->origin[0] = scene->x;
    work->origin[1] = scene->y + 0x100000;
    work->origin[2] = scene->z;
    best = 9999;
    for (i = 0; i < 80; i++) {
      cursor.object = event->objects[i];
      id = OBJECT_DEFINITION(cursor.object)[0];
      if (cursor.object != NULL && cursor.object->script != NULL &&
          (id == 410 || id == 304 || id == 386 || id == 418)) {
        distance = Func_080cdac0(scene->fieldKind, scene->firstObjectId, i);
        if (distance != -1 && best > distance) {
          height = Resource_GetMetadataRecordFarFar(
                       OBJECT_DEFINITION(cursor.object)[0])
                       ->height
                   << 16;
          work->mode = 1;
          work->origin[0] = cursor.object->x;
          work->origin[1] = cursor.object->y + height;
          work->origin[2] = cursor.object->z;
          scene->secondObjectId = i;
          scene->child = cursor.object;
          best = distance;
        }
      }
    }
  } else {
    if (event->unkd38 == 0)
      work->mode = 2;
    else
      work->mode = 3;
    if (scene->child != NULL)
      height =
          Resource_GetMetadataRecordFarFar(OBJECT_DEFINITION(scene->child)[0])
              ->height
          << 16;
    else
      height = 0x140000;
    work->origin[0] = scene->x;
    work->origin[1] = scene->y + height;
    work->origin[2] = scene->z;
  }

  mode = work->mode;
  if (mode == 1) {
    pos[0] = work->origin[0];
    pos[1] = work->origin[1];
    pos[2] = work->origin[2];
    object = Actor_Spawn(366, pos[0], pos[1], pos[2]);
    if (object == NULL)
      goto end;
    ((u8 *)object)[85] = 0;
    object->scaleX = 0x14000;
    object->scaleY = 0x14000;
    Actor_SetMode(object, 2);
    Sound_PlayFar(186);
    wait(50);
    Actor_Destroy(object);
    wait(10);
    goto end;
  }
  if (mode == 0) {
    pos[0] = work->origin[0];
    pos[1] = work->origin[1];
    pos[2] = work->origin[2];
    object = Actor_Spawn(367, pos[0], pos[1], pos[2]);
    if (object == NULL)
      goto end;
    ((u8 *)object)[85] = mode;
    Actor_SetMode(object, 2);
    Sound_PlayFar(186);
    wait(45);
    Actor_Destroy(object);
    wait(10);
    goto end;
  }

  Sound_PlayFar(207);
  BattleEffect_SetObjectUpdateMode(scene->mainObject, 0);
  work->angle = scene->angle;
  work->targetAngle = scene->angle;
  switch (look) {
  case 0:
    resourceId = (s32)&ResourceId_RedFlameBurst;
    break;
  case 1:
    resourceId = (s32)&ResourceId_VioletFlameBurst;
    break;
  case 2:
    resourceId = (s32)&ResourceId_YellowFlameBurst;
    break;
  case 3:
    resourceId = (s32)&ResourceId_BlueFlameBurst;
    break;
  default:
    resourceId = (s32)&ResourceId_RedFlameBurst;
    break;
  }
  Resource_DecodeType01(Resource_GetTableEntry(resourceId), work);
  dropSlot = Resource_FindFreeEntry();
  tile = VramCache_Load(dropSlot, 0x1000, work);
  work->dropTile = tile;
  cursor.particle = work->dropMotion;
  for (j = 0; j < 16; j++) {
    sprite = &work->drops[j];

    Func_080eaf98(sprite, 15, 24, 0x80000000, tile);
    sprite->depth = BattleEffect_GetObjectDepth(caster) + 10;
    sprite->oam.priority = BattleEffect_GetObjectPriority(caster);
    sprite->oam.palette256 = 1;
    sprite->oam.palette = 0;
    cursor.particle->timer = -1;
    cursor.particle++;
  }
  Resource_DecodeType01(Resource_GetTableEntry((s32)&ResourceId_DustPuffA),
                        work);
  puffSlot = Resource_FindFreeEntry();
  tile = VramCache_Load(puffSlot, 0x800, work);
  work->puffTile = tile;
  cursor.particle = work->puffMotion;
  for (j = 0; j < 16; j++) {
    sprite = &work->puffs[j];

    Func_080eaf98(sprite, 8, 15, 0x40000000, tile);
    sprite->scaleY = sprite->scaleX = 0xc000;
    sprite->depth = 250;
    sprite->oam.priority = 0;
    sprite->oam.palette256 = 1;
    sprite->oam.palette = 0;
    cursor.particle->timer = -1;
    cursor.particle++;
  }
  wait(1);
  work->emitting = 1;
  Task_Add(Func_080e38c8, 0x480);
  for (k = 0; k < 80; k++) {
    wait(1);
    if (work->hit != 0) {
      pending = Func_080ce458__f0e3d04(0x60000005, scene->fieldKind,
                              (s16)(work->hitObject | 0x100));
      if (pending != NULL)
        Field_RunPsynergyEvent__f0e3d04(pending, scene->firstObjectId, work->hitObject);
      if (Func_080ce458__f0e3d04(0x20000005, scene->fieldKind,
                        (s16)(work->hitObject | 0x100)) != NULL) {
        event->unkd30 = work->hitObject;
        break;
      }
    }
    if (gInput.pressed & 2)
      break;
  }
  Sound_PlayFar(395);
  work->emitting = 0;
  for (k = 0; k < 60; k++) {
    cursor.particle = work->dropMotion;
    for (j = 0; j < 16 && cursor.particle->timer == -1; j++, cursor.particle++)
      ;
    if (j == 16)
      break;
    wait(1);
  }
  Task_Remove(Func_080e38c8);
  Resource_ResetEntry(dropSlot);
  Resource_ResetEntry(puffSlot);
end:
  BattleEffect_PrepareBufferInterpolation();
  HeapSlot_Release(92);
}

// Whether a script object stands where it may not.

/// The script objects (Ram_HeapSlots->scriptObjects).
#define SCRIPT_OBJECT_COUNT 64

/// The event work's mode in which the cell test is skipped.
#define EVENT_MODE_NO_CELL_TEST 3

s32 Func_08020308(s32 x, s32 z, s32 layer, s32 height);
s32 Field_CheckSpheresOverlapFar(s32 *a, s32 radiusA, s32 *b, s32 radiusB);

/// Says whether object is blocked: 1 when, outside event mode 3,
/// Func_08020308 reports its cell, ORed with 2 when it overlaps any other
/// collidable script object (each radius less 2 pixels).
s32 Func_080e4244(struct Actor *object) {
  s32 result;
  struct Actor *other;
  s32 i;

  s8 *work = Ram_HeapSlots->eventWork;
  result = 0;
  if (work[394] != EVENT_MODE_NO_CELL_TEST) {
    if (Func_08020308(object->x, object->z, 0, object->terrainHeight) != 0)
      result = 1;
  }
  other = Ram_HeapSlots->scriptObjects;
  for (i = SCRIPT_OBJECT_COUNT - 1; i >= 0; i--, other++) {
    if (other->script != NULL && (other->collisionFlags & 1) &&
        other != object) {
      if (Field_CheckSpheresOverlapFar(&object->x, object->radius - 2,
                                       &other->x, other->radius - 2) >= 0)
        result |= 2;
    }
  }
  return result;
}

#include "iwram_call.h"
#include "dma.h"
#include "io_reg.h"
#include "heap_release.h"

struct CueEvent {
  u8 unk000[358];
  u16 cue;
};

/* VIEW(f0e3d04): variable gMainRoutineWork as extern u8 gMainRoutineWork[]; main_routine_work.h has extern struct MainRoutineWork gMainRoutineWork. */
extern u8 gMainRoutineWork__f0e3d04[] __asm__("gMainRoutineWork");

s32 Func_080dbcd8(s32 x, s32 z, s32 layer);
s32 Func_080dbd48(s32 x, s32 z, s32 layer);

void Func_080e42d4(void) {
  struct CueEvent *event = Ram_HeapSlots->eventWork;
  u8 *party = gMainRoutineWork__f0e3d04;
  struct Actor *object = ObjectTable_Get(*(s32 *)(party + 532));
  s32 reason = 0;

  if (*(s8 *)(party + 707) != 0)
    reason = 1;
  if (Func_080dbcd8(object->x, object->z, object->terrainId) == 0)
    reason = 2;
  if (party[530] == 5 && *(s16 *)((u8 *)object + 102) != 0 &&
      object->timer == 0) {
    s32 x;
    s32 z;
    s32 back;

    if (Func_080dbd48(object->x, object->z, 2) == 0)
      reason = 3;
    x = object->x & 0xfff00000;
    z = object->z & 0xfff00000;
    back = -0x80000;
    /* FAKEMATCH(barrier): back through an empty asm, so its -0x80000 loads
     * before the forward offset is built; without it, 10 of 93 instructions
     * differ in tla-en. */
    asm("" : "+l"(back));
    /* FAKEMATCH(dead): an empty once-only block that keeps the forward offset's
     * add after that load; without it, 4 of 93 instructions differ in tla-en.
     */
    do {
    } while (0);
    if (Func_080dbd48(x + 0x80000, z + back, 2) == 0)
      reason = 4;
  }
  if (reason != 0 && (Func_080e4244(object) & 2) == 0)
    event->cue = 0x208b;
}

/// The heap slot (a byte offset into Ram_HeapSlots) that holds the RAM copy of
/// the ARM picture decoder while a picture is drawn.
#define HEAP_SLOT_PICTURE_DECODER 80

// How a picture is stored.
#define PICTURE_FORMAT_TYPE01 1 ///< one type-01 stream
#define PICTURE_FORMAT_COMPRESSED                                              \
  3 ///< compressed, then run through the decoder

struct PictureSource {
  u8 unk00[5];
  u8 decodeMode;
  u8 unk06;
  u8 format;
  const void **pictures;
  u8 unk0c[10];
  u8 picture;
};

typedef void (*PictureDecoder)(const void *source, void *destination, s32 mode);

extern const u8 BattleEffect_DecodePictureCode[];
extern const u8 BattleEffect_DecodePictureCopyEnd[];
/// The decoder with its tables and work area (ld_script.ld). The block is
/// larger than the copy, which stops before the work area.
extern u8 BattleEffect_DecodePictureBlockSize[];

/// Draws the current picture into a cleared buffer of the given
/// size. The ARM picture decoder runs from RAM: when no copy is loaded yet,
/// this loads one for the call and releases it afterwards.
void Func_080e43a4(struct PictureSource *source, void *buffer, s32 size) {
  s32 decoderLoaded = 1;
  PictureDecoder decode = Ram_HeapSlots->unk4c[1];

  if (decode == NULL) {
    Dma_Set(BattleEffect_DecodePictureCode,
            HeapSlot_Alloc(HEAP_SLOT_PICTURE_DECODER,
                           (s32)BattleEffect_DecodePictureBlockSize),
            DMA_COPY_WORDS((u32)(BattleEffect_DecodePictureCopyEnd -
                                 BattleEffect_DecodePictureCode) >>
                           2),
            REG_DMA3);
    decode = Ram_HeapSlots->unk4c[1];
    decoderLoaded = 0;
  }

  Iwram_ClearWords(buffer, size);
  if (source->format == PICTURE_FORMAT_TYPE01) {
    Resource_DecodeType01(source->pictures[source->picture], buffer);
  } else if (source->format == PICTURE_FORMAT_COMPRESSED) {
    const void *stream =
        Iwram_Decompress(source->pictures[source->picture], buffer);

    if (stream != NULL)
      decode(stream, buffer, 0);
  } else {
    decode(source->pictures[source->picture], buffer, source->decodeMode);
  }

  if (!decoderLoaded)
    HeapSlot_Release(HEAP_SLOT_PICTURE_DECODER);
}
#undef OBJECT_DEFINITION
#undef SCRIPT_OBJECT_COUNT
#undef EVENT_MODE_NO_CELL_TEST
#undef HEAP_SLOT_PICTURE_DECODER
#undef PICTURE_FORMAT_TYPE01
#undef PICTURE_FORMAT_COMPRESSED

// ---- games/tla/src/field/event/f0e446c.c ----
/* VIEW(f0e446c): tag struct EffectSprite as struct EffectSprite__f0e446c; effect_sprite.h has another definition. */
struct EffectSprite__f0e446c {
  u8 unk00[4];
  u16 attr0_0 : 13;
  u16 color : 1;
  u16 attr0_14 : 2;
  u16 attr1;
  u16 tile : 10;
  u16 priority : 2;
  u16 palette : 4;
  u8 unk0a[2];
  s32 x;
  s32 y;
  s32 scaleX;
  s32 scaleY;
  u8 unk1c[2];
  u16 depth;
  u8 unk20[8];
};

struct EffectScene {
  u16 mode;
  u8 unk02[14];
  struct Actor *mainObject;
};

struct EffectParticle {
  s32 x;
  s32 y;
  s32 z;
  s32 velocityX;
  s32 velocityY;
  s32 velocityZ;
  s32 timer;
};

/* VIEW(f0e446c): routine Func_080eaf98 as void Func_080eaf98(struct EffectSprite__f0e446c *sprite, s32 width, s32 height, u32 shape, s32 tiles); effect_sprite.h has void Func_080eaf98(struct EffectSprite *sprite, s32 pivotX, s32 pivotY, u32 attributes, s32 tile). */
extern void Func_080eaf98__f0e446c(struct EffectSprite__f0e446c *sprite, s32 width, s32 height,
                   u32 shape, s32 tiles) __asm__("Func_080eaf98");
s32 BattleEffect_GetObjectDepth(struct Actor *object);
s32 BattleEffect_GetObjectPriority(struct Actor *object);
/* VIEW(f0e446c): routine Func_080eb01c as void Func_080eb01c(struct EffectSprite__f0e446c *sprite); effect_sprite.h has void Func_080eb01c(struct EffectSprite *sprite). */
extern void Func_080eb01c__f0e446c(struct EffectSprite__f0e446c *sprite) __asm__("Func_080eb01c");
void Camera_WorldToScreen(s32 *position);
void Vector_AddPolarOffset(s32 magnitude, s32 angle, s32 *position);
/* VIEW(f0e446c): routine BattleEffect_IntegrateVector2 as void BattleEffect_IntegrateVector2(struct EffectParticle *particle, s32 frames, s32 flags); battle.h has void BattleEffect_IntegrateVector2(s32 *vector, s32 damping, s32 gravity). */
extern void BattleEffect_IntegrateVector2__f0e446c(struct EffectParticle *particle, s32 frames,
                                   s32 flags) __asm__("BattleEffect_IntegrateVector2");
u32 Random16(void);
s32 Trig_Sin(s32 angle);

static inline s32 WrapClearWords(void *destination, s32 size) {
  /* FAKEMATCH(wrapper): the clear through this inline, so its arguments are set
   * before the entry's address is loaded; as a direct call, 4 of 77
   * instructions differ in tla-en. */
  return Iwram_ClearWords(destination, size);
}
void Func_080e446c(u8 *source, u8 *destination, s32 phase) {
  s32 width = 32;
  s32 height = 32;
  s32 column;
  s32 row;

  WrapClearWords(destination, 0x400);
  for (column = 0; column < width; column++) {
    s32 wave = (Trig_Sin(column << 13) << 3) + 0x80000;

    for (row = 0; row < height; row++) {
      s32 base = phase * 0x14ccc;
      s32 rowOffset = row * 0x40000 - 0x7c0000;
      s32 shift = base + rowOffset - wave;

      shift >>= 16;
      if (shift < 0) {
        destination[row * width + column] = source[row * width + column];
      } else if (shift + row < height) {
        destination[((shift + row) << 5) + column] =
            source[row * width + column];
      }
    }
  }
}

#include "event_work.h"

void Func_080e4510(struct EffectSprite__f0e446c *sprite, s32 tiles) {
  struct EffectScene *scene = Ram_HeapSlots->effectWork;
  struct EventWork *event = Ram_HeapSlots->eventWork;
  s32 position[3];

  Func_080eaf98__f0e446c(sprite, 16, 30, 0x80000000, tiles);
  sprite->depth = BattleEffect_GetObjectDepth(scene->mainObject);
  sprite->priority = BattleEffect_GetObjectPriority(scene->mainObject);
  sprite->color = 1;
  sprite->palette = 0;
  if (event->controlMode == 3) {
    sprite->scaleX = 0xc000;
    sprite->scaleY = 0xc000;
  } else {
    sprite->scaleX = scene->mainObject->scaleX;
    sprite->scaleY = scene->mainObject->scaleY;
  }
  position[0] = scene->mainObject->x;
  position[1] = scene->mainObject->y;
  position[2] = scene->mainObject->z;
  Camera_WorldToScreen(position);
  sprite->x = position[0];
  sprite->y = position[2];
}

/// Forwards to Func_080eb01c, which draws an effect sprite.
void Func_080e45a0(void *sprite) { Func_080eb01c__f0e446c(sprite); }

void Func_080e45a8(struct EffectSprite__f0e446c *sprite, struct EffectParticle *particle,
                   s32 unused, s32 tiles, s32 high) {
  struct Actor *mainObject =
      ((struct EffectScene *)Ram_HeapSlots->effectWork)->mainObject;
  s32 position[3];
  s32 y;

  position[0] = mainObject->x;
  position[1] = y = mainObject->y;
  position[2] = mainObject->z;
  if (high) {
    position[1] = y + 0x20000;
    Vector_AddPolarOffset(0x20000, Random16(), position);
  } else {
    Vector_AddPolarOffset(0x40000, Random16(), position);
  }
  particle->x = position[0];
  particle->y = position[1];
  particle->z = position[2];
  position[2] = 0;
  position[0] = 0;
  if (high)
    Vector_AddPolarOffset(0x3333, Random16(), position);
  else
    Vector_AddPolarOffset(0xcccc, Random16(), position);
  particle->velocityX = position[0];
  particle->velocityY = 0;
  particle->velocityZ = position[2];
  Func_080eaf98__f0e446c(sprite, 8, 8, 0x40000000, tiles);
  sprite->scaleX = 0x3333;
  sprite->scaleY = 0x3333;
  sprite->priority = BattleEffect_GetObjectPriority(mainObject);
  sprite->depth = BattleEffect_GetObjectDepth(mainObject) + 2;
  sprite->color = 1;
  sprite->palette = 0;
}

// ---- games/tla/src/field/event_commands/growing_particle.c ----
// A sprite particle that grows as it drifts, and Func_080e4730, which sets up
// 64 of them as a dust puff.

void Camera_WorldToScreen(s32 *position);
/* VIEW(growing_particle): routine BattleEffect_IntegrateVector2 as void BattleEffect_IntegrateVector2(s32 *vector, s32 damping, s32 gravity); f0e446c has void BattleEffect_IntegrateVector2(struct EffectParticle *particle, s32 frames, s32 flags). */
extern void BattleEffect_IntegrateVector2__growing_particle(s32 *vector, s32 damping, s32 gravity) __asm__("BattleEffect_IntegrateVector2");

/// Advances one particle by a frame. For its first 32 frames its sprite
/// steps through eight pictures of eight tiles from tiles, four frames each,
/// grows by a tenth of its size a frame to full size, is placed on screen at
/// the particle's position and drawn, and the particle moves on; after that
/// only its age counts on. Reached from Func_080e4730 and Func_080e4a34.
void Func_080e46ac(struct EffectSprite *sprite, struct EffectParticle *particle,
                   s32 unused, s32 tiles) {
  s32 position[3];
  s32 scale;
  s32 picture;

  if ((u32)particle->timer <= 31) {
    picture = (particle->timer >> 2) & 7;
    sprite->oam.index = tiles + picture * 8;
    scale = sprite->scaleX + 0x1999;
    sprite->scaleX = scale;
    if (scale > Q16_ONE)
      scale = Q16_ONE;
    sprite->scaleX = scale;
    sprite->scaleY = scale;
    position[0] = particle->x;
    position[1] = particle->y;
    position[2] = particle->z;
    Camera_WorldToScreen(position);
    sprite->x = position[0];
    sprite->y = position[2];
    Func_080eb01c(sprite);
    BattleEffect_IntegrateVector2__growing_particle(&particle->x, 60, 0);
  }
  particle->timer++;
}


struct PendingEvent;

/* VIEW(growing_particle): variable gMainRoutineWork as extern u8 gMainRoutineWork[]; main_routine_work.h has extern struct MainRoutineWork gMainRoutineWork. */
extern u8 gMainRoutineWork__growing_particle[] __asm__("gMainRoutineWork");
extern u8 Data_02000452;

void Func_080dc954(void);
void Actor_SetOthersPaused(struct Actor *excludedObject, s32 groupMode,
                           s32 action);
void Func_080e42d4(void);
void *Resource_GetTableEntry(s32 resourceId);
extern char ResourceId_DustPuffA;
s32 Resource_DecodeType01(const void *source, void *destination);
s32 Resource_FindFreeEntry(void);
s32 VramCache_Load(u32 slot, u32 size, const void *source);
s32 Resource_ResetEntry(u32 slot);
/* VIEW(growing_particle): routine Func_080e45a8 as void Func_080e45a8(void *sprite, void *particle, s32 index, s32 tiles, s32 high); the definition has void Func_080e45a8(struct EffectSprite__f0e446c *sprite, struct EffectParticle *particle, s32 unused, s32 tiles, s32 high). */
extern void Func_080e45a8__growing_particle(void *sprite, void *particle, s32 index, s32 tiles,
                   s32 high) __asm__("Func_080e45a8");
/* VIEW(growing_particle): routine Func_080e4510 as void Func_080e4510(void *sprite, s32 tiles); the definition has void Func_080e4510(struct EffectSprite__f0e446c *sprite, s32 tiles). */
extern void Func_080e4510__growing_particle(void *sprite, s32 tiles) __asm__("Func_080e4510");
void Func_080e446c(u8 *source, u8 *destination, s32 phase);
void Func_080e45a0(void *sprite);
/* VIEW(growing_particle): routine Func_080e43a4 as void Func_080e43a4(void *source, void *buffer, s32 size); the definition has void Func_080e43a4(struct PictureSource *source, void *buffer, s32 size). */
extern void Func_080e43a4__growing_particle(void *source, void *buffer, s32 size) __asm__("Func_080e43a4");
void Camera_MoveWithinMapBounds(s32 x, s32 y, s32 z, s32 mode);
void Actor_SetPositionAndResetMotionFar(struct Actor *object, s32 x, s32 y,
                                        s32 z);
void Animation_SetIndexAndInitializeObjectsFar(struct Actor *object, s32 index);
void Animation_ApplyChildValueToFirstFar(void *animation, s32 value);
struct Actor *Actor_GetById(u32 objectId);
void Camera_FollowActor(s32 actor, s32 mode);
s32 Func_080ce31c(s32 kind);
/* VIEW(growing_particle): routine Func_080ce458 as struct PendingEvent *Func_080ce458(u32 filter, s32 kind, s32 selector); battle.h has struct TriggerEvent *Func_080ce458(s32 filter, s32 kind, s32 selector). */
extern struct PendingEvent *Func_080ce458__growing_particle(u32 filter, s32 kind, s32 selector) __asm__("Func_080ce458");
s32 Field_RunPsynergyEvent(struct PendingEvent *event, s32 mode, s32 argument);

void Func_080e4730(void) {
  u8 *work = HeapSlot_Alloc(92, 0x192c);
  u8 *effect = Ram_HeapSlots->effectWork;
  u8 *event = Ram_HeapSlots->eventWork;
  s32 pictureEntry;
  struct Actor *owner = *(struct Actor **)(effect + 16);
  s32 particleEntry;
  s32 particleTiles;
  u8 *motionFlags;
  u8 *rippled;

  s32 sprite;
  u8 *particle;
  u8 *particleSprite;
  u8 *animation;
  u8 *party;
  s32 pictureOffset;
  s32 ripple;
  s32 frame;
  s32 i;
  s32 delay;
  s32 mask;

  Func_080dc954();
  gMainRoutineWork__growing_particle[620] = 0;
  Actor_SetOthersPaused(owner, 1, 0);
  Task_Remove(Func_080e42d4);
  Resource_DecodeType01(Resource_GetTableEntry((s32)&ResourceId_DustPuffA),
                        work);
  particleEntry = Resource_FindFreeEntry();
  particleTiles = VramCache_Load(particleEntry, 0x800, work);
  i = 0;
  particle = work + 0x1200;
  delay = 0;
  particleSprite = work + 0x800;
  for (; i < 64; i++) {
    Func_080e45a8__growing_particle(particleSprite, particle, i, particleTiles,
                  *(s16 *)(event + 0xd48));
    *(s32 *)(particle + 24) = delay;
    particleSprite += 40;
    delay -= 2;
    particle += 28;
  }
  Camera_MoveWithinMapBounds(-1, -1, -1, 0);
  motionFlags = &owner->motionFlags;
  *motionFlags = 0;
  // Set before the move: 63 is held in a saved register across both calls.
  mask = 63;
  Actor_SetPositionAndResetMotionFar(owner, owner->x, owner->y + 0xa0000,
                                     owner->z);
  party = gMainRoutineWork__growing_particle;
  Animation_SetIndexAndInitializeObjectsFar(owner, *(s32 *)(party + 532));
  ((u8 *)owner->sprite)[5] &= mask;
  mask &= ((u8 *)owner->sprite)[7];
  mask |= 128;
  ((u8 *)owner->sprite)[7] = mask;
  if (*(s8 *)(event + 394) == 3)
    *(s32 *)((u8 *)owner->sprite + 12) = 0xc000;
  owner->facing = 0x4000;
  Actor_SetMode(owner, 0);
  Animation_ApplyChildValueToFirstFar(owner->sprite, 15);
  wait(1);
  Animation_ApplyChildValueToFirstFar(owner->sprite, 0);
  {
    void *source = *(void **)((u8 *)owner->sprite + 40);

    sprite = 0x400;
    {
      /* FAKEMATCH(pin): the copy's size pinned to r2, so it is set before
       * work's copy to r1; as Func_080e43a4(source, work, sprite), 2 of 332
       * instructions differ in tla-en. */
      register s32 size asm("r2") = sprite;

      Func_080e43a4__growing_particle(source, work, size);
    }
  }
  Animation_ApplyChildValueToFirstFar(owner->sprite, 15);
  /* FAKEMATCH(barrier): an empty asm reading party, which nothing reads after
   * the animation's set, keeps it live in r6 to here; without it, 79 of 332
   * instructions differ in tla-en. */
  asm("" : : "r"(party));
  pictureEntry = Resource_FindFreeEntry();
  sprite = VramCache_Load(pictureEntry, sprite, NULL);
  Func_080e4510__growing_particle(work + 0x1900, sprite);
  wait(1);

  frame = 0;
  rippled = work + 0x400;
  ripple = 0;
  pictureOffset = sprite * 32;
  for (; frame < 78; frame++) {
    if (frame == 0)
      Sound_PlayFar(544);
    if (frame <= 64) {
      Func_080e446c(work, rippled, 92 - ripple / 2);
      ((void (*)(void *, s32, s32, void *))Ram_HeapSlots->unk4c[2])(
          rippled, 32, 32, (u8 *)0x06010000 + pictureOffset);
      Func_080e45a0(work + 0x1900);
    }
    if (frame == 66) {
      Actor_SetMode(owner, 1);
      Animation_ApplyChildValueToFirstFar(owner->sprite, 0);
      Actor_SetSingleChildFlagsFar(Actor_GetById(*(s16 *)(effect + 24)), 1);
      Camera_FollowActor(*(s16 *)(effect + 24), 1);
      Sound_PlayFar(539);
      if (*(s8 *)(event + 394) == 3) {
        Data_02000452 = 1;
        *motionFlags = 2;
      } else {
        Data_02000452 = 0;
        *motionFlags = 3;
      }
      if (*(s16 *)(event + 0xd48) == 0)
        Field_RunPsynergyEvent(
            Func_080ce458__growing_particle(0x40000005, *(s16 *)(effect + 30),
                          Func_080ce31c(*(s16 *)(effect + 30))),
            *(s16 *)(effect + 24), -1);
    }
    i = 0;
    particle = work + 0x1200;
    particleSprite = work + 0x800;
    for (; i < 30; i++) {
      Func_080e46ac(particleSprite, particle, i, particleTiles);
      particleSprite += 40;
      particle += 28;
    }
    wait(1);
    ripple += 3;
  }
  Actor_SetOthersPaused(owner, 0, 16);
  Resource_ResetEntry(pictureEntry);
  Resource_ResetEntry(particleEntry);
  HeapSlot_Release(92);
}


void Func_080e3d04(void);
void Func_080e4a34(void);
void Func_080e572c(void);
void Func_080e5d5c(void);
void Func_080e68d0(void);
void Func_080e7844(void);
void Func_080e9af4(void);
void Func_080e74d8(void);
void BattleEffect_PrepareBufferInterpolation(void);
s32 Party_GetLeaderActor(void);

/// Psynergy effect 23 cast on a target: runs its effect.
void BattleEffect_CallEffect23(void) { Func_080e4a34(); }

// ---- games/tla/src/field/event_commands/f0e4a34.c ----
#include "field_psynergy_work.h"
#include "main_routine_work.h"
#include "system/io_reg.h"

extern char ResourceId_DustPuffA;

struct GrabEventWork {
  u8 unk000[394];
  s8 mode;
  u8 unk18b[0xd48 - 395];
  s16 standing;
  s16 phase;
};

void Func_080dc954(void);
void Actor_SetOthersPaused(struct Actor *excludedActor, s32 groupMode,
                           s32 action);
s32 Func_080dbcd8(s32 x, s32 z, s32 terrain);
void BattleEffect_InitializeSharedScene(void);
void BattleEffect_SetObjectUpdateMode(struct Actor *actor, s32 mode);
void BattleEffect_PrepareBufferInterpolation(void);
void Func_080e42d4(void);
void Func_080e4730(void);
void *Resource_GetTableEntry(s32 resourceId);
s32 Resource_DecodeType01(const void *source, void *destination);
s32 Resource_FindFreeEntry(void);
s32 VramCache_Load(u32 slot, u32 size, const void *source);
s32 Resource_ResetEntry(u32 slot);
/* VIEW(f0e4a34): routine Func_080e45a8 as void Func_080e45a8(void *sprite, void *particle, s32 index, s32 tiles, s32 high); the definition has void Func_080e45a8(struct EffectSprite__f0e446c *sprite, struct EffectParticle *particle, s32 unused, s32 tiles, s32 high). */
extern void Func_080e45a8__f0e4a34(void *sprite, void *particle, s32 index, s32 tiles,
                   s32 high) __asm__("Func_080e45a8");
/* VIEW(f0e4a34): routine Func_080e4510 as void Func_080e4510(void *sprite, s32 tiles); the definition has void Func_080e4510(struct EffectSprite__f0e446c *sprite, s32 tiles). */
extern void Func_080e4510__f0e4a34(void *sprite, s32 tiles) __asm__("Func_080e4510");
void Func_080e446c(u8 *source, u8 *destination, s32 phase);
void Func_080e45a0(void *sprite);
/* VIEW(f0e4a34): routine Func_080e46ac as void Func_080e46ac(void *sprite, void *particle, s32 index, s32 tiles); the definition has void Func_080e46ac(struct EffectSprite *sprite, struct EffectParticle *particle, s32 unused, s32 tiles). */
extern void Func_080e46ac__f0e4a34(void *sprite, void *particle, s32 index, s32 tiles) __asm__("Func_080e46ac");
/* VIEW(f0e4a34): routine Func_080e43a4 as void Func_080e43a4(void *source, void *buffer, s32 size); the definition has void Func_080e43a4(struct PictureSource *source, void *buffer, s32 size). */
extern void Func_080e43a4__f0e4a34(void *source, void *buffer, s32 size) __asm__("Func_080e43a4");
void Camera_MoveWithinMapBounds(s32 x, s32 y, s32 z, s32 mode);
void Actor_SetPositionAndResetMotionFar(struct Actor *actor, s32 x, s32 y,
                                        s32 z);
void Animation_SetIndexAndInitializeObjectsFar(struct Actor *actor, s32 index);
void Animation_ApplyChildValueToFirstFar(void *animation, s32 value);
struct Actor *Actor_GetById(u32 objectId);
void Camera_FollowActor(s32 actor, s32 mode);

void Func_080e4a34(void) {
  u8 *work = HeapSlot_Alloc(92, 0x192c);
  struct BattleFxScene *effect = Ram_HeapSlots->effectWork;
  struct GrabEventWork *event = Ram_HeapSlots->eventWork;
  struct Actor *owner;
  s32 pictureEntry;
  s32 particleEntry;
  s32 particleTiles;
  s32 savedY;
  u8 savedFlags;
  u8 *motionFlags;
  s32 pictureTiles;
  u8 *sprite;
  u8 *particle;
  s32 frame;
  s32 delay;
  s32 i;

  owner = effect->mainObject;
  savedY = owner->y;
  motionFlags = &owner->motionFlags;
  savedFlags = *motionFlags;
  if (*(u16 *)((u8 *)owner->sprite + 20) != 0x2020)
    goto end;
  Func_080dc954();
  Actor_SetOthersPaused(owner, 1, 0);
  if (Func_080dbcd8(owner->x, owner->z, owner->terrainId) == 0)
    event->standing = 1;
  else
    event->standing = 0;
  BattleEffect_InitializeSharedScene();
  Resource_DecodeType01(Resource_GetTableEntry((s32)&ResourceId_DustPuffA),
                        work);
  particleEntry = Resource_FindFreeEntry();
  particleTiles = VramCache_Load(particleEntry, 0x800, work);
  i = 0;
  particle = work + 0x1200;
  delay = -12;
  sprite = work + 0x800;
  for (; i < 64; i++) {
    Func_080e45a8__f0e4a34(sprite, particle, i, particleTiles, event->standing);
    *(s32 *)(particle + 24) = delay;
    sprite += 40;
    delay -= 2;
    particle += 28;
  }
  Camera_MoveWithinMapBounds(-1, -1, -1, 0);
  BattleEffect_SetObjectUpdateMode(effect->mainObject, 0);
  Actor_SetPositionAndResetMotionFar(owner, owner->x, owner->y, owner->z);
  *motionFlags = 0;
  wait(2);
  for (frame = 0; frame < 40; frame++) {
    if (frame > 0)
      owner->y += 0x3999;
    if (frame & 31)
      Sound_PlayFar(566);
    owner->facing += 0x2000;
    wait(1);
  }
  owner->facing = 0x4000;
  wait(1);
  Func_080e43a4__f0e4a34(*(void **)((u8 *)owner->sprite + 40), work, 0x400);
  pictureEntry = Resource_FindFreeEntry();
  pictureTiles = VramCache_Load(pictureEntry, 0x400, NULL);
  Func_080e4510__f0e4a34(work + 0x1900, pictureTiles);
  frame = 15;
  for (; frame <= 92; frame++) {
    Func_080e446c(work, work + 0x400, frame * 3 / 2);
    ((void (*)(void *, s32, s32, void *))Ram_HeapSlots->unk4c[2])(
        work + 0x400, 32, 32, (u8 *)OBJ_VRAM0 + pictureTiles * 32);
    Func_080e45a0(work + 0x1900);
    i = 0;
    particle = work + 0x1200;
    sprite = work + 0x800;
    for (; i <= 32; i++) {
      Func_080e46ac__f0e4a34(sprite, particle, i, particleTiles);
      sprite += 40;
      particle += 28;
    }
    if (frame == 16)
      Animation_ApplyChildValueToFirstFar(owner->sprite, 15);
    if (frame == 45)
      Sound_PlayFar(136);
    if (frame == 60) {
      Animation_SetIndexAndInitializeObjectsFar(owner, 251);
      ((u8 *)owner->sprite)[5] &= 63;
      ((u8 *)owner->sprite)[7] = (((u8 *)owner->sprite)[7] & 63) | 64;
      Actor_SetMode(owner, 1);
      Actor_SetSingleChildFlagsFar(Actor_GetById(effect->firstObjectId), 0);
      Camera_FollowActor(effect->firstObjectId, 1);
      owner->y = savedY;
      *motionFlags = savedFlags;
    }
    if (frame == 61)
      Animation_ApplyChildValueToFirstFar(owner->sprite, 0);
    wait(1);
  }
  if (event->mode == 3)
    gMainRoutineWork.renderMode = 9;
  else
    gMainRoutineWork.renderMode = 5;
  owner->timer = 0;
  owner->unk66 = 0;
  event->phase = 3;
  gMainRoutineWork.pendingThird = 1;
  if (event->standing == 0)
    Task_Add(Func_080e42d4, 0x480);
  Actor_SetOthersPaused(owner, 0, 16);
  Resource_ResetEntry(pictureEntry);
  Resource_ResetEntry(particleEntry);
  BattleEffect_PrepareBufferInterpolation();
end:
  HeapSlot_Release(92);
  if (event->standing != 0)
    Func_080e4730();
}


struct GlowParticle {
  s32 x;
  s32 y;
  s32 z;
  s32 velocityX;
  s32 velocityY;
  s32 velocityZ;
  s32 timer;
};

struct GlowWork {
  u8 tiles[0x400];
  struct GlowParticle sparks[32];
  struct EffectSprite sparkSprites[32];
  struct EffectSprite burstSprites[32];
  struct GlowParticle bursts[32];
  s16 phase;
  s16 timer;
  s16 shake;
  u16 burstTile;
  s16 nextBurst;
  s16 newBursts;
  s8 paletteMode;
};


extern const u16 Data_080f0fa0[];

s32 Trig_Sin(s32 angle);
void Camera_WorldToScreen(s32 *position);
void Vector_AddPolarOffset(s32 distance, s32 angle, s32 *position);
void BattleEffect_IntegrateVector2(s32 *vector, s32 damping, s32 gravity);
/* VIEW(f0e4a34): routine EffectSprite_DrawAtWorldPosition as void EffectSprite_DrawAtWorldPosition(struct EffectSprite *sprite, struct GlowParticle *particle); f0e2c38 has void EffectSprite_DrawAtWorldPosition(struct EffectSprite *sprite, struct SprayParticle *particle). */
extern void EffectSprite_DrawAtWorldPosition__f0e4a34(struct EffectSprite *sprite,
                                      struct GlowParticle *particle) __asm__("EffectSprite_DrawAtWorldPosition");
void Map_SetScreenShakeFar(s32 x, s32 y, s32 decay);

#define GLOW_NEXT_PHASE(work) ((work)->phase++, (work)->timer = -1)

void Func_080e4d9c(void) {
  struct GlowWork *work = (struct GlowWork *)Ram_HeapSlots->battleFxWork[0];
  struct FxScene *scene = Ram_HeapSlots->effectWork;
  struct CanvasSpriteWork *canvas = Ram_HeapSlots->unkec[1];
  struct GlowParticle *particle;
  struct EffectSprite *sprite;
  volatile u16 *palette;
  s32 pos[3];
  s32 i;
  s32 *frames;
  s32 red;
  s32 blue;

  work->shake = 0;
  if (work->paletteMode == 1) {
    palette = (volatile u16 *)0x050003c0;
    i = 0;
    frames = &canvas->drawnFrames;
    red = 31;
    blue = 31 << 10;
    for (; i <= 31; i++) {
      s32 green =
          (((Trig_Sin((((u32)*frames >> 1) + i << 16) + 0x90000 >> 5) * 12) >>
            16) +
           16)
          << 5;

      // An empty once-only loop keeps the green term's shift ahead of blue's
      // copy; without it, 2 of 950 instructions differ in tla-en.
      do {
      } while (0);
      *palette++ = green | blue | red;
      // An empty once-only loop that ends the body keeps i's update after the
      // colour's store; without it, 4 of 950 instructions differ in tla-en.
      do {
      } while (0);
    }
  } else if (work->paletteMode == 2) {
    Dma_Set(Data_080f0fa0, (void *)0x050003e0, DMA_ENABLE << 16 | 0x10,
            REG_DMA3);
    work->paletteMode = 0;
  }
  pos[0] = scene->x;
  pos[1] = scene->y;
  pos[2] = scene->z;
  Camera_WorldToScreen(pos);
  canvas->x = (s16)(pos[0] >> 16);
  canvas->y = (s16)(pos[2] >> 16);

  if (scene->enabled == 0) {
    particle = work->sparks;
    for (i = 0; i < 32; i++) {
      if ((u32)particle->timer <= 29) {
        sprite = &work->sparkSprites[i];
        sprite->x = particle->x + (canvas->x << 16);
        sprite->y = particle->y + (canvas->y << 16);
        Func_080eb01c(sprite);
        BattleEffect_IntegrateVector2(&particle->x, 60, 0);
      }
      particle->timer++;
      particle++;
    }
    for (i = 0; i < work->newBursts; i++) {
      particle = &work->bursts[work->nextBurst];
      particle->timer = 0;
      particle->x = scene->x;
      particle->y = scene->y;
      particle->z = scene->z;
      Vector_AddPolarOffset(Random16() * 12 + 0x40000, Random16(),
                            &particle->x);
      work->nextBurst = ++work->nextBurst % 32;
    }
    work->newBursts = 0;
    i = 0;
    particle = work->bursts;
    for (; i <= 31; i++, particle++) {
      if ((u32)particle->timer > 71)
        continue;
      sprite = &work->burstSprites[i];
      switch (particle->timer) {
      case 6:
        sprite->oam.index = work->burstTile + 4;
        break;
      case 12:
        sprite->oam.index = work->burstTile + 8;
        break;
      case 22:
        sprite->oam.index = work->burstTile + 12;
        break;
      case 32:
        sprite->oam.index = work->burstTile + 16;
        break;
      case 42:
        sprite->oam.index = work->burstTile + 20;
        break;
      case 52:
        sprite->oam.index = work->burstTile + 24;
        break;
      case 62:
        sprite->oam.index = work->burstTile + 28;
        break;
      }
      EffectSprite_DrawAtWorldPosition__f0e4a34(sprite, particle);
      particle->timer++;
    }
    switch (work->phase) {
    case 0:
      if (work->timer == 0)
        Sound_PlayFar(140);
      if (work->timer == 10)
        GLOW_NEXT_PHASE(work);
      break;
    case 1:
      canvas->scaleX += 0x300;
      canvas->scaleY = canvas->scaleX;
      if (canvas->scaleX >= 0x8000) {
        work->phase++;
        work->timer = 0;
      }
      break;
    case 2:
      if (work->timer == 40)
        GLOW_NEXT_PHASE(work);
      break;
    case 3:
      if (work->timer == 0)
        work->paletteMode = 2;
      if (work->timer == 0)
        Sound_PlayFar(144);
      if ((work->timer & 7) == 0) {
        Map_SetScreenShakeFar(0x30000, 0x30000, 0xe666);
        work->newBursts = 1;
        work->shake = 2;
      }
      canvas->command = CANVAS_UPLOAD_AND_FADE3;
      if (work->timer == 16)
        GLOW_NEXT_PHASE(work);
      break;
    case 4:
      if ((work->timer & 15) == 0)
        Sound_PlayFar(144);
      if ((work->timer & 7) == 0) {
        Map_SetScreenShakeFar(0x30000, 0x30000, 0xe666);
        work->newBursts = 1;
        work->shake = 2;
      }
      if (work->timer == 128)
        GLOW_NEXT_PHASE(work);
      break;
    case 5:
      if (work->timer == 35)
        GLOW_NEXT_PHASE(work);
      break;
    case 6:
      if (work->timer == 0)
        work->newBursts = 10;
      if (work->timer == 4)
        work->newBursts = 10;
      if (work->timer == 0) {
        Map_SetScreenShakeFar(0xc0000, 0xc0000, 0xe666);
        Sound_PlayFar(145);
        if (work->timer == 0)
          work->shake = 3;
      }
      if (work->timer == 20)
        work->shake = 1;
      if (work->timer == 72) {
        work->phase = 999;
        work->timer = -1;
      }
      break;
    }
  } else {
    particle = work->sparks;
    for (i = 0; i < 12; i++) {
      if ((u32)particle->timer <= 29) {
        sprite = &work->sparkSprites[i];
        sprite->x = particle->x + (canvas->x << 16);
        sprite->y = particle->y + (canvas->y << 16);
        Func_080eb01c(sprite);
        BattleEffect_IntegrateVector2(&particle->x, 60, 0);
      }
      particle->timer++;
      particle++;
    }
    for (i = 0; i < work->newBursts; i++) {
      particle = &work->bursts[work->nextBurst];
      particle->timer = 0;
      particle->x = scene->x;
      particle->y = scene->y;
      particle->z = scene->z;
      work->nextBurst = ++work->nextBurst % 32;
    }
    work->newBursts = 0;
    i = 0;
    particle = work->bursts;
    for (; i <= 31; i++, particle++) {
      if ((u32)particle->timer > 47)
        continue;
      sprite = &work->burstSprites[i];
      switch (particle->timer) {
      case 4:
        sprite->oam.index = work->burstTile + 4;
        break;
      case 8:
        sprite->oam.index = work->burstTile + 8;
        break;
      case 14:
        sprite->oam.index = work->burstTile + 12;
        break;
      case 22:
        sprite->oam.index = work->burstTile + 16;
        break;
      case 28:
        sprite->oam.index = work->burstTile + 20;
        break;
      case 35:
        sprite->oam.index = work->burstTile + 24;
        break;
      case 41:
        sprite->oam.index = work->burstTile + 28;
        break;
      }
      EffectSprite_DrawAtWorldPosition__f0e4a34(sprite, particle);
      particle->timer++;
    }
    switch (work->phase) {
    case 0:
      if (work->timer == 0)
        Sound_PlayFar(140);
      if (work->timer == 10)
        GLOW_NEXT_PHASE(work);
      break;
    case 1:
      canvas->scaleX += 0x500;
      canvas->scaleY = canvas->scaleX;
      if (canvas->scaleX > 0x4ccb) {
        work->phase++;
        work->timer = 0;
      }
      break;
    case 2:
      if (work->timer == 16)
        GLOW_NEXT_PHASE(work);
      break;
    case 3:
      if (work->timer == 0) {
        work->paletteMode = 2;
        if (work->timer == 0) {
          Map_SetScreenShakeFar(0x30000, 0x30000, 0xe666);
          work->newBursts = 1;
          Sound_PlayFar(144);
        }
      }
      canvas->command = CANVAS_UPLOAD_AND_FADE3;
      if (work->timer == 16)
        GLOW_NEXT_PHASE(work);
      break;
    case 4:
      if (work->timer == 5)
        BattleEffect_SetObjectUpdateMode(scene->mainObject, 0);
      if (work->timer == 20) {
        work->phase = 999;
        work->timer = -1;
      }
      break;
    }
  }
  work->timer++;
}


void Func_080e3d04(void);
void Func_080e572c(void);
void Func_080e5d5c(void);
void Func_080e68d0(void);
void Func_080e7844(void);
void Func_080e9af4(void);
void Func_080e74d8(void);
s32 Party_GetLeaderActor(void);

/// Runs field Psynergy animation 21 for the dispatcher's targeted path.
void BattleEffect_CallEffect21(void) { Func_080e572c(); }
#undef GLOW_NEXT_PHASE

// ---- games/tla/src/field/event_commands/f0e572c.c ----


/* VIEW(f0e572c): tag struct FxScene as struct FxScene__f0e572c; f0e2c38.c has another definition. */
struct FxScene__f0e572c {
  u16 facing;
  u16 angle;
  s32 x;
  s32 y;
  s32 z;
  struct Actor *mainObject;
  struct Actor *child;
  s16 firstObjectId;
  s16 secondObjectId;
  u16 action;
  s16 fieldKind;
  s8 enabled;
};

extern const u8 Data_080f3984[];
extern char ResourceId_FieldEffectExplosion, ResourceId_GlowDisc;

void *HeapSlot_Alloc(s32 slot, s32 size);
void HeapSlot_Release(s32 slot);
void BattleEffect_InitializeSharedScene(void);
void Camera_WorldToScreen(s32 *position);
s32 Func_080db9a8(struct Actor *object, s32 facing);
s32 BattleEffect_GetObjectDepth(struct Actor *object);
s32 Resource_FindFreeEntry(void);
s32 VramCache_Load(u32 slot, u32 size, const void *source);
s32 Resource_ResetEntry(u32 slot);
void *Resource_GetTableEntry(s32 resourceId);
s32 Resource_DecodeType01(const void *source, void *destination);
u32 Random16(void);
s32 Trig_Sin(s32 angle);
s32 Trig_Cos(s32 angle);
void wait(s32 frames);
s32 Func_080ce31c(s32 kind);
/* VIEW(f0e572c): routine Func_080ce458 as void *Func_080ce458(u32 kind, s32 entry, s32 arg); growing_particle has struct PendingEvent *Func_080ce458(u32 filter, s32 kind, s32 selector). */
extern void *Func_080ce458__f0e572c(u32 kind, s32 entry, s32 arg) __asm__("Func_080ce458");
/* VIEW(f0e572c): routine Field_RunPsynergyEvent as void Field_RunPsynergyEvent(void *event, s32 a, s32 b); growing_particle has s32 Field_RunPsynergyEvent(struct PendingEvent *event, s32 mode, s32 argument). */
extern void Field_RunPsynergyEvent__f0e572c(void *event, s32 a, s32 b) __asm__("Field_RunPsynergyEvent");
void BattleEffect_PrepareBufferInterpolation(void);
void Func_080e4d9c(void);

void Func_080e572c(void) {
  struct GlowWork *work = HeapSlot_Alloc(92, sizeof(struct GlowWork));
  struct FxScene__f0e572c *scene = Ram_HeapSlots->effectWork;
  struct CanvasSpriteWork *canvas;
  struct EffectSprite *sprite;
  struct GlowParticle *particle;
  void *pending;
  s32 pos[3];
  s32 sparkEntry;
  s32 sparkTile;
  s32 burstEntry;
  s32 tile;
  s32 radius;
  s32 angle;
  s32 kind;
  s32 frame;
  s32 (*mul)(s32, s32);
  struct EffectSprite *burstSprite;
  s32 i;

  scene->y += 0xa0000;
  BattleEffect_InitializeSharedScene();
  Func_080eb824();
  canvas = Ram_HeapSlots->unkec[1];
  pos[0] = scene->x;
  pos[1] = scene->y;
  pos[2] = scene->z;
  Camera_WorldToScreen(pos);
  canvas->x = (s16)(pos[0] >> 16);
  canvas->y = (s16)(pos[2] >> 16);
  canvas->priority = Func_080db9a8(scene->mainObject, scene->facing);
  canvas->depth = BattleEffect_GetObjectDepth(scene->mainObject) + 2;
  canvas->mode = CANVAS_SPRITE_256_COLORS;
  canvas->scaleX = 0;
  canvas->scaleY = 0;
  if (scene->facing == 0xc000)
    canvas->depth -= 4;

  sparkEntry = Resource_FindFreeEntry();
  sparkTile = VramCache_Load(sparkEntry, 64, Data_080f3984);
  for (i = 0, particle = work->sparks, mul = Iwram_MulQ16,
      sprite = work->sparkSprites;
       i < 32;) {
    Func_080eaf98(sprite, 4, 4, 0, sparkTile);
    sprite->oam.palette256 = 1;
    sprite->oam.palette = 0;
    sprite->oam.priority = canvas->priority;
    sprite->depth = canvas->depth + 1;
    sprite->scaleY = sprite->scaleX = 0x10000;
    radius = Random16() * 20 + 0x140000;
    angle = Random16();
    particle->x = mul(Trig_Cos(angle), radius);
    particle->y = mul(Trig_Sin(angle), radius);
    radius /= 12;
    angle += 0x8000;
    particle->velocityX = mul(Trig_Cos(angle), radius);
    particle->velocityY = mul(Trig_Sin(angle), radius);
    particle->timer = -i * 2;
    i++;
    sprite++;
    particle++;
  }

  Resource_DecodeType01(
      Resource_GetTableEntry((s32)&ResourceId_FieldEffectExplosion), work);
  burstEntry = Resource_FindFreeEntry();
  tile = VramCache_Load(burstEntry, 0x400, work);
  work->burstTile = tile;
  particle = work->bursts;
  burstSprite = work->burstSprites;
  for (i = 0; i < 32; i++) {
    Func_080eaf98(burstSprite, 8, 8, 0x40000000, tile);
    burstSprite->depth = canvas->depth + 1;
    burstSprite->oam.priority = canvas->priority;
    burstSprite->oam.palette256 = 0;
    burstSprite->oam.palette = 15;
    particle->timer = -1;
    burstSprite++;
    particle++;
  }
  Resource_DecodeType01(Resource_GetTableEntry((s32)&ResourceId_GlowDisc),
                        Ram_HeapSlots->battleFxWork[1]);
  canvas->command = CANVAS_UPLOAD;
  Sound_PlayFar(130);
  wait(1);
  work->phase = 0;
  work->timer = 0;
  work->paletteMode = 1;
  work->nextBurst = 0;
  work->newBursts = 0;
  Task_Add(Func_080e4d9c, 0x480);
  kind = scene->fieldKind;
  pending = Func_080ce458__f0e572c(0x50000005, kind, Func_080ce31c(kind));
  frame = 0;
  while (work->phase != 999) {
    wait(1);
    if (work->phase == 999)
      frame = 999;
    Field_RunPsynergyEvent__f0e572c(pending, work->shake, frame);
    frame++;
  }
  Task_Remove(Func_080e4d9c);
  Resource_ResetEntry(sparkEntry);
  Resource_ResetEntry(burstEntry);
  BattleEffect_PrepareBufferInterpolation();
  BattleEffect_EndCanvasSprite();
  HeapSlot_Release(92);
}


struct ShineParticle {
  s32 x;
  s32 y;
  u8 unk08[16];
  s32 timer;
};

struct ShineSprite {
  u8 unk00[12];
  s32 x;
  s32 y;
  u8 unk14[20];
};

struct ShineWork {
  u8 unk0000[0x1000];
  struct ShineParticle particles[12];
  u8 unk1150[0x230];
  struct ShineSprite sprites[12];
  u8 unk1560[0x348];
  struct Actor *object;
  s16 state;
  s16 timer;
  s8 sparkles;
  u8 fading;
};

struct BlendWork {
  u8 unk00[160];
  u32 phase;
  u8 unka4[12];
  s32 scaleX;
  s32 scaleY;
  u8 unkb8[9];
  u8 mode;
};

/* VIEW(f0e572c): routine BattleEffect_IntegrateVector2 as void BattleEffect_IntegrateVector2(struct ShineParticle *particle, s32 frames, s32 flags); f0e4a34 has void BattleEffect_IntegrateVector2(s32 *vector, s32 damping, s32 gravity). */
extern void BattleEffect_IntegrateVector2__f0e572c(struct ShineParticle *particle, s32 frames,
                                   s32 flags) __asm__("BattleEffect_IntegrateVector2");
void Map_SetScreenShakeFar(s32 a, s32 b, s32 c);

void Func_080e5a64(void) {
  struct ShineWork *work = (struct ShineWork *)Ram_HeapSlots->battleFxWork[0];
  struct BlendWork *blend = Ram_HeapSlots->unkec[1];
  struct ShineParticle *particle;
  struct ShineSprite *sprite;
  s32 i;
  s32 base, blue;
  s32 color;
  s32 red, green;
  u32 *phase;

  work->fading = 0;
  if (work->state == 1) {
    i = 0;
    /* FAKEMATCH(temporary): the phase read through a pointer kept across the
     * loop, whose address is built ahead of the loop's constants; as
     * blend->phase, 4 of 302 instructions differ in tla-en. */
    phase = &blend->phase;
    // The phase offset built in two steps stays in a register across the loop;
    // as base = 0x90000, 48 of 302 instructions differ in tla-en.
    base = 9;
    base <<= 16;
    blue = 0x7c00;
    // The loop is written with goto: as for (; i < 32; i++), 60 of 302
    // instructions differ in tla-en.
  palette:
    red = Trig_Sin(((((*phase >> 1) + i) << 16) + base) >> 5) * 12;
    green = Trig_Sin(((((*phase >> 1) + i) << 16) + base) >> 5) * 12;
    red = (red >> 16) + 16;
    green = (green >> 16) + 16;
    // The colour in a once-only block, so red and green are finished before it
    // is put together; as plain statements, 10 of 302 instructions differ in
    // tla-en.
    do {
      color = (green << 5) | blue | red;
      *(u16 *)(0x050003c0 + i * 2) = color;
    } while (0);
    if (++i < 32)
      goto palette;
  }
  if (work->sparkles) {
    particle = work->particles;
    sprite = work->sprites;
    for (i = 0; i < 12; i++, sprite++, particle++) {
      if (particle->timer >= 0 && particle->timer < 16) {
        sprite->x = particle->x;
        sprite->y = particle->y;
        Func_080eb01c(sprite);
        BattleEffect_IntegrateVector2__f0e572c(particle, 64, 0);
      }
      particle->timer++;
    }
  }
  switch (work->state) {
  case 0:
    work->object->y -= 0x38000;
    if (work->timer == 8) {
      blend->scaleX = 0x2000;
      blend->scaleY = 0x2000;
      Map_SetScreenShakeFar(0x14ccc, 0x14ccc, 0x10000);
      Sound_PlayFar(567);
      work->sparkles = 1;
      work->fading = 1;
    }
    if (work->timer == 8) {
      work->state++;
      work->timer = -1;
    }
    break;
  case 1:
    blend->scaleX += 0x1000;
    blend->scaleY = blend->scaleX;
    if (blend->scaleX > 0xcccb) {
      blend->scaleX = 0xcccc;
      blend->scaleY = 0xcccc;
      work->state++;
      work->timer = -1;
    }
    break;
  case 2:
    blend->mode = 3;
    if (work->timer == 32) {
      work->state++;
      work->timer = -1;
    }
    break;
  case 3:
    if (work->timer == 0)
      Map_SetScreenShakeFar(1, 1, 1);
    if (work->timer == 10) {
      work->state++;
      work->timer = -1;
    }
    break;
  case 4:
    work->object->y += 0x18000;
    if (work->timer == 13)
      work->state = 999;
    break;
  }
  work->timer++;
}

// ---- games/tla/src/field/event_commands/psynergy_effects_10.c ----
/* VIEW(psynergy_effects_10): tag struct EffectSprite as struct EffectSprite__psynergy_effects_10; effect_sprite.h has another definition. */
struct EffectSprite__psynergy_effects_10 {
  u8 unk00[4];
  u16 attr0_0 : 13;
  u16 color : 1;
  u16 attr0_14 : 2;
  u16 attr1;
  u16 tile : 10;
  u16 priority : 2;
  u16 palette : 4;
  u8 unk0a[2];
  s32 x;
  s32 y;
  s32 scaleX;
  s32 scaleY;
  u8 unk1c[2];
  u16 depth;
  u8 unk20[8];
};


struct RiseWork {
  u8 unk0000[0x18a8];
  struct Actor *object;
  s16 done;
  s16 timer;
};

struct AnchorWork {
  u8 unk0000[0x1930];
  struct Actor *anchor;
};

struct ModeEvent {
  u8 unk000[352];
  u16 cue;
};

/* VIEW(psynergy_effects_10): routine Func_080eb01c as void Func_080eb01c(struct EffectSprite__psynergy_effects_10 *sprite); effect_sprite.h has void Func_080eb01c(struct EffectSprite *sprite). */
extern void Func_080eb01c__psynergy_effects_10(struct EffectSprite__psynergy_effects_10 *sprite) __asm__("Func_080eb01c");
void Camera_WorldToScreen(s32 *position);
/* VIEW(psynergy_effects_10): routine BattleEffect_IntegrateVector3 as void BattleEffect_IntegrateVector3(struct EffectParticle *particle, s32 frames, s32 flags); battle.h has void BattleEffect_IntegrateVector3(s32 *vector, s32 damping, s32 gravity). */
extern void BattleEffect_IntegrateVector3__psynergy_effects_10(struct EffectParticle *particle, s32 frames,
                                   s32 flags) __asm__("BattleEffect_IntegrateVector3");
struct Actor *Actor_GetById(u32 objectId);
void wait(s32 frames);
s32 Trig_Sin(s32 angle);

/// Per-frame callback: until done, moves the effect object down 2.5 pixels
/// and shrinks it by 1/64 each frame, marking done at frame 8.
void BattleEffect_SinkAndShrinkObject(void) {
  struct RiseWork *work = Ram_HeapSlots->battleFxWork[0];

  if (work->done == 0) {
    struct Actor *object = work->object;

    object->y += -0x28000;
    object->scaleX += -0x400;
    object->scaleY += -0x400;
    if (work->timer == 8)
      work->done = 999;
  }
  work->timer++;
}


void Func_080e3d04(void);
void Func_080e4a34(void);
void Func_080e572c(void);
void Func_080e5d5c(void);
void Func_080e68d0(void);
void Func_080e7844(void);
void Func_080e9af4(void);
void Func_080e74d8(void);
void BattleEffect_PrepareBufferInterpolation(void);
s32 Party_GetLeaderActor(void);

/// Runs field effect 18 (Func_080e5d5c) for the request dispatcher.
void BattleEffect_CallEffect18(void) { Func_080e5d5c(); }

// ---- games/tla/src/field/event_commands/f0e5d5c.c ----
/* VIEW(f0e5d5c): tag struct ShineParticle as struct ShineParticle__f0e5d5c; f0e2c38.c has another definition. */
struct ShineParticle__f0e5d5c {
  s32 x;
  s32 y;
  s32 unk08;
  s32 velocityX;
  s32 velocityY;
  s32 unk14;
  s32 timer;
};

/* VIEW(f0e5d5c): tag struct ShineWork as struct ShineWork__f0e5d5c; f0e2c38.c has another definition. */
struct ShineWork__f0e5d5c {
  u8 tiles[0x1000];
  struct ShineParticle__f0e5d5c particles[32];
  struct EffectSprite sprites[32];
  struct EffectSprite glow;
  struct Actor *object;
  s16 state;
  s16 timer;
  s8 sparkles;
  s8 fading;
};

/* VIEW(f0e5d5c): tag struct FxScene as struct FxScene__f0e5d5c; f0e2c38.c has another definition. */
struct FxScene__f0e5d5c {
  u16 facing;
  u16 angle;
  s32 x;
  s32 y;
  s32 z;
  struct Actor *mainObject;
  struct Actor *child;
  s16 firstObjectId;
  s16 secondObjectId;
  u16 action;
  s16 fieldKind;
  s8 enabled;
  u8 unk21[0x7a3];
  s16 fading;
  s16 frame;
};

/* VIEW(f0e5d5c): tag struct FxEvent as struct FxEvent__f0e5d5c; f0e2c38.c has another definition. */
struct FxEvent__f0e5d5c {
  u8 unk00[20];
  struct Actor *objects[80];
};

extern char ResourceId_GlowDisc, ResourceId_GoldStar;

void *HeapSlot_Alloc(s32 slot, s32 size);
void HeapSlot_Release(s32 slot);
void Vector_AddPolarOffset(s32 distance, s32 angle, s32 *position);
s32 Func_080cda84(s32 id, struct Actor *object, s32 *position);
s32 Func_080dbe80(s32 *a, s32 radiusA, s32 *b, s32 radiusB);
void BattleEffect_InitializeSharedScene(void);
struct Actor *Actor_Spawn(s32 kind, s32 x, s32 y, s32 z);
void Actor_Destroy(struct Actor *object);
void Func_080dba5c(void);
s32 BattleEffect_GetObjectPriority(struct Actor *object);
void Camera_WorldToScreen(s32 *position);
void *Resource_GetTableEntry(s32 resourceId);
s32 Resource_DecodeType01(const void *source, void *destination);
s32 Resource_FindFreeEntry(void);
s32 VramCache_Load(u32 slot, u32 size, const void *source);
s32 Resource_ResetEntry(u32 slot);
u32 Random16(void);
s32 Trig_Sin(s32 angle);
s32 Trig_Cos(s32 angle);
void wait(s32 frames);
void Func_080dbed0(struct Actor *object, s32 x, s32 y, s32 z);
void Func_080dbf94(struct Actor *object, s32 x, s32 y, s32 z);
void BattleEffect_SinkAndShrinkObject(void);
struct Actor *BattleEffect_SpawnScatterParticle(s32 x, s32 y, s32 z);
s32 Func_080ce31c(s32 kind);
/* VIEW(f0e5d5c): routine Func_080ce458 as void *Func_080ce458(u32 kind, s32 entry, s32 arg); growing_particle has struct PendingEvent *Func_080ce458(u32 filter, s32 kind, s32 selector). */
extern void *Func_080ce458__f0e5d5c(u32 kind, s32 entry, s32 arg) __asm__("Func_080ce458");
/* VIEW(f0e5d5c): routine Field_RunPsynergyEvent as void Field_RunPsynergyEvent(void *event, s32 a, s32 b); growing_particle has s32 Field_RunPsynergyEvent(struct PendingEvent *event, s32 mode, s32 argument). */
extern void Field_RunPsynergyEvent__f0e5d5c(void *event, s32 a, s32 b) __asm__("Field_RunPsynergyEvent");
void BattleEffect_PrepareBufferInterpolation(void);
void Func_080e5a64(void);

void Func_080e5d5c(void) {
  struct ShineWork__f0e5d5c *work = HeapSlot_Alloc(92, 0x18b4);
  struct FxScene__f0e5d5c *scene = Ram_HeapSlots->effectWork;
  struct FxEvent__f0e5d5c *event = Ram_HeapSlots->eventWork;
  struct Actor *child = scene->child;
  struct Actor *caster = scene->mainObject;
  struct Actor *object;
  struct Actor *shine;
  struct CanvasSpriteWork *canvas;
  struct EffectSprite *sprite;
  struct EffectSprite *glow;
  void *pending;
  s32 pos[3];
  s32 starEntry;
  s32 starTile;
  s32 glowEntry;
  s32 inReach;
  s32 lift;
  s32 glowTile;
  s32 spot;
  s32 size = 0x800;
  s32 rise;
  s32 speed;
  s32 angle;
  s32 frame;
  s32 j;
  struct ShineParticle__f0e5d5c *particle;

  lift = 0x200000;
  if (child == NULL) {
    scene->x = caster->x;
    scene->y = caster->y;
    scene->z = caster->z;
    Vector_AddPolarOffset(lift, scene->facing, &scene->x);
  } else if (child->motionFlags & 4) {
    /* FAKEMATCH(dead): lift cleared under a test of the ground's height, a
     * store the line after next overwrites, which sends scene to r4 and the
     * 0x200000 to r0 for the y store; without it, 12 of 639 instructions
     * differ in tla-en. */
    if (child->terrainHeight == 300)
      lift = 0;
    scene->y = child->terrainHeight;
    lift = child->y - child->terrainHeight + 0x200000;
  }
  if (scene->enabled != 0) {
    // As x != -1 this splits the -1 and loses the mvns.
    spot = ~Func_080cda84(scene->fieldKind, caster, &scene->x);
    inReach = (u32)(-spot | spot) >> 31;
    if (inReach == 1) {
      for (j = 0; j < 80; j++) {
        object = event->objects[j];
        if (object != NULL && object->script != NULL &&
            (object->collisionFlags & 1) &&
            Func_080dbe80(&scene->x, 4, &object->x, object->radius - 2) >= 0)
          goto outOfReach;
      }
    }
  } else {
    inReach = 1;
    if (child != NULL && ((u8 *)child)[99] != 0) {
    outOfReach:
      inReach = 0;
    }
  }

  BattleEffect_InitializeSharedScene();
  Sound_PlayFar(138);
  shine = Actor_Spawn(352, 0, 0, 0);
  work->object = shine;
  if (shine == NULL)
    goto end;
  shine->motionFlags = 0;
  if (*(u16 *)((u8 *)event + 0xd4c) & 2) {
    if ((shine->animationKind & 15) == 1) {
      ((struct EffectSprite *)shine->sprite)->oam.priority = 1;
      ((u8 *)shine)[35] &= 254;
    }
  } else {
    ((u8 *)shine)[35] = 1;
  }
  shine->facing = caster->facing;
  Actor_SetMode(shine, 2);
  if (!(*(u16 *)((u8 *)event + 0xd4c) & 1))
    Func_080dba5c();

  Func_080eb824();
  canvas = Ram_HeapSlots->unkec[1];
  canvas->priority = BattleEffect_GetObjectPriority(scene->mainObject);
  canvas->depth = 1;
  canvas->mode = CANVAS_SPRITE_256_COLORS;
  canvas->scaleX = 0;
  canvas->scaleY = 0;
  pos[0] = scene->x;
  pos[1] = scene->y;
  pos[2] = scene->z;
  Camera_WorldToScreen(pos);
  canvas->x = (s16)(pos[0] >> 16);
  canvas->y = (s16)(pos[2] >> 16);
  Resource_DecodeType01(Resource_GetTableEntry((s32)&ResourceId_GlowDisc),
                        Ram_HeapSlots->battleFxWork[1]);
  canvas->command = CANVAS_UPLOAD;

  // A once-only block: without it the canvas mode load is scheduled one row
  // later (6 differ).
  do {
    Iwram_FillWords(work, size, 0);
  } while (0);
  glowEntry = Resource_FindFreeEntry();
  glowTile = VramCache_Load(glowEntry, size, work);
  glow = &work->glow;
  Func_080eaf98(glow, 32, 32, 0xc0000000, glowTile);
  glow->oam.priority = 0;
  glow->oam.palette256 = 0;
  glow->oam.palette = 0;
  glow->depth = 0;
  glow->scaleY = glow->scaleX = 0xcccc;
  pos[0] = scene->x;
  pos[1] = scene->y;
  pos[2] = scene->z;
  Camera_WorldToScreen(pos);
  glow->x = pos[0];
  glow->y = pos[2];

  Resource_DecodeType01(Resource_GetTableEntry((s32)&ResourceId_GoldStar),
                        work);
  starEntry = Resource_FindFreeEntry();
  starTile = VramCache_Load(starEntry, 0x100, work);
  for (j = 0, particle = work->particles; j < 32; j++, particle++) {
    sprite = &work->sprites[j];
    speed = Random16() * 18 / 10;
    speed += 0x28000;
    angle = j * 0x1555 + (Random16() >> 5);
    pos[0] = scene->x;
    pos[1] = scene->y;
    pos[2] = scene->z;
    Vector_AddPolarOffset(0x40000, angle, pos);
    Camera_WorldToScreen(pos);
    particle->x = pos[0];
    particle->y = pos[2];
    particle->velocityX = Iwram_MulQ16(Trig_Cos(angle), speed);
    particle->velocityY = Iwram_MulQ16(Trig_Sin(angle), speed);
    particle->timer = -j;
    Func_080eaf98(sprite, 8, 8, 0x40000000, starTile);
    sprite->oam.palette256 = 1;
    sprite->oam.palette = 0;
    sprite->oam.priority = 0;
    sprite->depth = 240;
    sprite->scaleY = sprite->scaleX = 0xcccc;
  }
  wait(1);
  Func_080dbed0(shine, scene->x, scene->y + lift, scene->z);
  Actor_SetMode(shine, 1);
  wait(15);
  rise = 24;
  do {
    shine->y += 0x8000;
    rise--;
    wait(1);
  } while (rise >= 0);
  wait(15);
  work->state = 0;
  work->timer = 0;
  work->sparkles = 0;
  if (!inReach) {
    Task_Add(BattleEffect_SinkAndShrinkObject, 0x480);
    while (work->state != 999)
      wait(1);
    Task_Remove(BattleEffect_SinkAndShrinkObject);
    for (j = 15; j >= 0; j--)
      BattleEffect_SpawnScatterParticle(shine->x, shine->y, shine->z);
    Sound_PlayFar(114);
    wait(1);
  } else {
    pending = Func_080ce458__f0e5d5c(0x50000005, scene->fieldKind,
                            Func_080ce31c(scene->fieldKind));
    Task_Add(Func_080e5a64, 0x480);
    frame = 0;
    while (work->state != 999) {
      wait(1);
      scene->fading = work->fading;
      scene->frame = frame;
      if (((s8 *)event)[394] != 3 && !(*(u16 *)((u8 *)event + 0xd4c) & 1)) {
        glow = &work->glow;
        Func_080eb01c(glow);
      }
      Field_RunPsynergyEvent__f0e5d5c(pending, work->fading, frame++);
    }
    Task_Remove(Func_080e5a64);
    wait(10);
    Sound_PlayFar(114);
    Func_080dbf94(shine, shine->x, shine->y, shine->z);
  }
  Actor_Destroy(shine);
  Resource_ResetEntry(starEntry);
  Resource_ResetEntry(glowEntry);
  BattleEffect_EndCanvasSprite();
end:
  BattleEffect_PrepareBufferInterpolation();
  HeapSlot_Release(92);
}


void Func_080e62f4(u8 *source, u8 *destination, s32 phase) {
  s32 width = 32;
  s32 height;
  s32 column;
  s32 row;

  Iwram_ClearWords(destination, 32 * 32);
  // Set after the clear, which keeps it out of the call's registers.
  height = 32;
  for (column = 0; column < width; column++) {
    s32 wave = (Trig_Sin(column << 13) << 3) + Q16_ONE * 8;

    for (row = 0; row < height; row++) {
      s32 position = phase * 0x19999 - Q16_ONE * 124 + row * (Q16_ONE * 4);
      // In two steps: the wave subtracted, then the shift.
      s32 shift = position - wave;
      shift >>= 16;

      if (shift >= 0 && shift < height)
        destination[(shift << 5) + column] = source[row * width + column];
    }
  }
}

// ---- games/tla/src/field/event_commands/psynergy_effects_11.c ----
#include "macros.h"

/* VIEW(psynergy_effects_11): tag struct EffectSprite as struct EffectSprite__psynergy_effects_11; effect_sprite.h has another definition. */
struct EffectSprite__psynergy_effects_11 {
  u8 unk00[4];
  u16 attr0_0 : 13;
  u16 color : 1;
  u16 attr0_14 : 2;
  u16 attr1;
  u16 tile : 10;
  u16 priority : 2;
  u16 palette : 4;
  u8 unk0a[2];
  s32 x;
  s32 y;
  s32 scaleX;
  s32 scaleY;
  u8 unk1c[2];
  u16 depth;
  u8 unk20[8];
};





/* VIEW(psynergy_effects_11): routine Func_080eb01c as void Func_080eb01c(struct EffectSprite__psynergy_effects_11 *sprite); effect_sprite.h has void Func_080eb01c(struct EffectSprite *sprite). */
extern void Func_080eb01c__psynergy_effects_11(struct EffectSprite__psynergy_effects_11 *sprite) __asm__("Func_080eb01c");
void Camera_WorldToScreen(s32 *position);
/* VIEW(psynergy_effects_11): routine BattleEffect_IntegrateVector3 as void BattleEffect_IntegrateVector3(struct EffectParticle *particle, s32 frames, s32 flags); battle.h has void BattleEffect_IntegrateVector3(s32 *vector, s32 damping, s32 gravity). */
extern void BattleEffect_IntegrateVector3__psynergy_effects_11(struct EffectParticle *particle, s32 frames,
                               s32 flags) __asm__("BattleEffect_IntegrateVector3");
struct Actor *Actor_GetById(u32 objectId);
void wait(s32 frames);

/// Animates a particle's sprite through 8 tiles (4 frames each), grows its
/// scale by 0.1 a frame up to 1.0, draws it at the particle's projected
/// position and advances the particle.
void BattleEffect_UpdateGrowingParticleSprite(struct EffectSprite__psynergy_effects_11 *sprite, struct EffectParticle *particle,
                   s32 tiles) {
  s32 position[3];
  s32 scale;

  sprite->tile = tiles + ((particle->timer >> 2) & 7) * 8;
  scale = sprite->scaleX + 0x1999;
  sprite->scaleX = scale;
  if (scale > Q16_ONE)
    scale = Q16_ONE;
  sprite->scaleX = scale;
  sprite->scaleY = scale;
  SET_VECTOR(position, particle->x, particle->y, particle->z);
  Camera_WorldToScreen(position);
  sprite->x = position[0];
  sprite->y = position[2];
  Func_080eb01c__psynergy_effects_11(sprite);
  BattleEffect_IntegrateVector3__psynergy_effects_11(particle, 60, 0);
}

// ---- games/tla/src/field/event_commands/f0e6400.c ----

struct Work080e6400 {
  u8 tiles[0x400];
  u8 picture[0x400];
  struct EffectSprite sprites[64];
  struct EffectParticle particles[64];
  struct EffectSprite pictureSprite;
  s16 phase;
  s16 timer;
  s16 particleTile;
  s16 pictureTile;
  struct Actor *anchor;
  struct Actor *other;
  s16 mode;
};

void Func_080e62f4(u8 *source, u8 *destination, s32 phase);
/* VIEW(f0e6400): routine BattleEffect_UpdateGrowingParticleSprite as void BattleEffect_UpdateGrowingParticleSprite(struct EffectSprite *sprite, struct EffectParticle *particle, s32 tiles); the definition has void BattleEffect_UpdateGrowingParticleSprite(struct EffectSprite__psynergy_effects_11 *sprite, struct EffectParticle *particle, s32 tiles). */
extern void BattleEffect_UpdateGrowingParticleSprite__f0e6400(struct EffectSprite *sprite,
                                              struct EffectParticle *particle,
                                              s32 tiles) __asm__("BattleEffect_UpdateGrowingParticleSprite");
/* VIEW(f0e6400): routine EffectSprite_DrawAtWorldPosition as void EffectSprite_DrawAtWorldPosition(struct ScreenSprite *sprite, s32 *position); f0e4a34 has void EffectSprite_DrawAtWorldPosition(struct EffectSprite *sprite, struct GlowParticle *particle). */
extern void EffectSprite_DrawAtWorldPosition__f0e6400(struct ScreenSprite *sprite,
                                      s32 *position) __asm__("EffectSprite_DrawAtWorldPosition");
u32 Random16(void);

#define NEXT_PHASE(work) ((work)->phase++, (work)->timer = -1)

void Func_080e6400(void) {
  struct Work080e6400 *work;
  struct Actor *anchor;
  struct EffectSprite *sprite;
  struct EffectParticle *particle;
  u8 *vram;
  u8 *picture;
  s16 *phase;
  s32 jitter;
  s32 i;

  work = (struct Work080e6400 *)Ram_HeapSlots->battleFxWork[0];
  anchor = work->anchor;
  if (work->mode == 0) {
    switch (work->phase) {
    case 0:
      if (work->timer == 1)
        work->other->scaleX = Q16_ONE;
      if (work->timer == 4)
        NEXT_PHASE(work);
      break;
    case 1:
      if (work->timer == 8)
        Actor_SetMode(anchor, 3);
      if (work->timer <= 15)
        anchor->y += 0x24ccc;
      if (work->timer == 28)
        NEXT_PHASE(work);
      break;
    case 2:
      if (work->timer == 25)
        work->other->scaleX = 0;
      if (work->timer == 50)
        work->phase = 999;
      break;
    }
    phase = &work->phase;
    if (*phase == 2) {
      picture = work->picture;
      vram = (u8 *)OBJ_VRAM0 + work->pictureTile * TILE_SIZE_4BPP;
      Func_080e62f4(work->tiles, picture, work->timer + 14);
      // The heap copy of Render_CopyStrided called as returning the source
      // pointer it leaves in r0, which moves picture to r0 after the width's
      // 32; called as void, 2 of 596 instructions differ in tla-en.
      ((const u8 *(*)(const u8 *, s32, s32, void *))Ram_HeapSlots->unk4c[2])(
          picture, 32, 32, vram);
      sprite = &work->pictureSprite;
      Func_080eb01c(sprite);
    }
    if (*phase == 2) {
      for (i = 0, particle = work->particles; i < 18; i++, particle++) {
        sprite = &work->sprites[i];
        if (particle->timer >= 0 && particle->timer < 32)
          BattleEffect_UpdateGrowingParticleSprite__f0e6400(sprite, particle,
                                                   work->particleTile);
        particle->timer++;
      }
    }
  } else if (work->mode == 3) {
    switch (work->phase) {
    case 0:
      if (work->timer == 1)
        work->other->scaleX = Q16_ONE;
      if (work->timer == 4)
        NEXT_PHASE(work);
      break;
    case 1:
      if (work->timer == 8)
        Actor_SetMode(work->anchor, 3);
      if (work->timer <= 15)
        anchor->y += 0x24ccc;
      if (work->timer == 28)
        NEXT_PHASE(work);
      break;
    case 2:
      if (work->timer == 70)
        work->other->scaleX = 0;
      if (work->timer == 90)
        work->phase = 999;
      break;
    }
    if (work->phase == 2) {
      for (i = 0, particle = work->particles; i < 64; i++, particle++) {
        if (particle->timer >= 0 && particle->timer < 45) {
          sprite = &work->sprites[i];
          EffectSprite_DrawAtWorldPosition__f0e6400(sprite, particle);
          if (particle->timer < 42) {
            particle->y -= Q16_ONE;
            jitter = Random16();
            particle->x += jitter - (s32)Random16();
          }
        }
        particle->timer++;
      }
    }
  } else if (work->mode == 1) {
    switch (work->phase) {
    case 0:
      if (work->timer == 8)
        NEXT_PHASE(work);
      break;
    case 1:
      if (work->timer == 0)
        Actor_SetMode(work->anchor, 4);
      anchor->y += 0x24ccc;
      if (work->timer == 15)
        NEXT_PHASE(work);
      break;
    case 2:
      if (work->timer == 20)
        work->phase = 999;
      break;
    }
  } else {
    switch (work->phase) {
    case 0:
      if (work->timer == 8)
        NEXT_PHASE(work);
      break;
    case 1:
      if (work->timer == 0)
        Actor_SetMode(work->anchor, 4);
      anchor->y += 0x24ccc;
      if (work->timer == 15)
        NEXT_PHASE(work);
      break;
    case 2:
      if (work->timer == 30)
        work->phase = 999;
      break;
    }
    particle = work->particles;
    sprite = work->sprites;
    for (i = 0; i < 20; i++, sprite++, particle++) {
      if (particle->timer >= 0 && particle->timer < 32)
        BattleEffect_UpdateGrowingParticleSprite__f0e6400(sprite, particle,
                                                 work->particleTile);
      particle->timer++;
    }
  }
  work->timer++;
}


void Func_080e3d04(void);
void Func_080e4a34(void);
void Func_080e572c(void);
void Func_080e5d5c(void);
void Func_080e68d0(void);
void Func_080e7844(void);
void Func_080e9af4(void);
void Func_080e74d8(void);
void BattleEffect_PrepareBufferInterpolation(void);
void wait(s32 frames);
s32 Party_GetLeaderActor(void);

/// Effect 20 cast on a target: runs the effect, ignoring the target.
void BattleEffect_CallEffect20(void) { Func_080e68d0(); }

#include "field/event_work.h"

struct RiseParticle {
  s32 x;
  s32 y;
  s32 z;
  s32 velocityX;
  s32 velocityY;
  s32 velocityZ;
  s32 timer;
};

/* VIEW(f0e6400): tag struct RiseWork as struct RiseWork__f0e6400; f0e2c38.c has another definition. */
struct RiseWork__f0e6400 {
  u8 tiles[0x400];
  u8 picture[0x400];
  struct EffectSprite sprites[64];
  struct RiseParticle particles[64];
  struct EffectSprite pictureSprite;
  s16 phase;
  s16 timer;
  s16 particleTile;
  s16 pictureTile;
  struct Actor *object;
  struct Actor *other;
  s16 mode;
};

/* VIEW(f0e6400): tag struct FxScene as struct FxScene__f0e6400; f0e2c38.c has another definition. */
struct FxScene__f0e6400 {
  u16 facing;
  u16 angle;
  s32 x;
  s32 y;
  s32 z;
  struct Actor *mainObject;
  struct Actor *child;
  s16 firstObjectId;
  s16 secondObjectId;
  u16 action;
  s16 fieldKind;
  s8 enabled;
  u8 unk21[0x20];
  s8 alternate;
  u8 unk42[0x782];
  s16 stage;
  s16 frame;
};

// The event work with its flag word past the shared struct. Read as a member,
// the flag word takes this struct's alias set, which orders its reads after
// s32 stores but not after an Actor's u8 stores, as the listing has them.
/* VIEW(f0e6400): tag struct FxEvent as struct FxEvent__f0e6400; f0e2c38.c has another definition. */
struct FxEvent__f0e6400 {
  struct EventWork work;
  u8 unkd48[4];
  /// Bit 0 clear runs Func_080dba5c; bit 1 set fixes the risen object's sprite
  /// priority at 1 directly, else its priority flag is set.
  u16 unkd4c;
};

STATIC_ASSERT(offsetof(struct FxEvent__f0e6400, unkd4c) == 0xd4c);

extern char ResourceId_DustPuffA, ResourceId_DustPuffB, ResourceId_WhiteSparkle,
    ResourceId_Data1E9;

void *HeapSlot_Alloc(s32 slot, s32 size);
void HeapSlot_Release(s32 slot);
void Vector_AddPolarOffset(s32 distance, s32 angle, s32 *position);
void BattleEffect_InitializeSharedScene(void);
struct Actor *Actor_Spawn(s32 kind, s32 x, s32 y, s32 z);
void Actor_Destroy(struct Actor *object);
void Func_080dba5c(void);
void Func_080dbed0(struct Actor *object, s32 x, s32 y, s32 z);
void Func_080dbf94(struct Actor *object, s32 x, s32 y, s32 z);
s32 Func_08020330(struct Actor *object);
s32 Func_080dbe80(s32 *a, s32 radiusA, s32 *b, s32 radiusB);
s32 Func_080dbdc8(s32 x, s32 z, s32 layer);
s32 Func_080cda84(s32 id, struct Actor *object, s32 *position);
s32 Func_080dbc04(s32 x, s32 z, s32 layer);
struct Actor *BattleEffect_SpawnScatterParticle(s32 x, s32 y, s32 z);
void *Resource_GetTableEntry(s32 resourceId);
s32 Resource_DecodeType01(const void *source, void *destination);
s32 Resource_FindFreeEntry(void);
s32 BattleEffect_GetObjectDepth(struct Actor *object);
s32 BattleEffect_GetObjectPriority(struct Actor *object);
void Camera_WorldToScreen(s32 *position);
s32 Func_080ce31c(s32 kind);
void Func_080e716c(void);

void Func_080e68d0(void) {
  struct RiseWork__f0e6400 *work = HeapSlot_Alloc(92, 0x193c);
  struct FxScene__f0e6400 *scene = Ram_HeapSlots->effectWork;
  struct FxEvent__f0e6400 *event = Ram_HeapSlots->eventWork;
  struct Actor *caster = scene->mainObject;
  struct Actor *other = NULL;
  struct Actor *object;
  struct Actor *target;
  struct EffectSprite *sprite;
  struct RiseParticle *particle;
  void *pending;
  void *data;
  s32 particleEntry = 96;
  s32 particleTile;
  s32 pictureEntry = 96;
  s32 pictureTile;
  s32 pos[3];
  s32 failed;
  s8 *alternate;
  s32 stage;
  s32 frame;
  s32 i;

  if (event->work.controlMode != 3) {
    scene->x = (caster->x & 0xfff00000) + 0x80000;
    scene->y = caster->y;
    scene->z = (caster->z & 0xfff00000) + 0x80000;
    Vector_AddPolarOffset(0x100000, scene->facing, &scene->x);
  }
  BattleEffect_InitializeSharedScene();
  object = Actor_Spawn(353, 0, 0, 0);
  work->object = object;
  if (object == NULL)
    goto end;
  object->motionFlags = 0;
  if (event->unkd4c & 2) {
    if ((object->animationKind & 15) == 1) {
      ((struct EffectSprite *)object->sprite)->oam.priority = 1;
      object->priorityFlags &= 254;
    }
  } else {
    object->priorityFlags = 1;
  }
  object->facing = caster->facing;
  Actor_SetMode(object, 0);
  alternate = &scene->alternate;
  if (*alternate != 0)
    Animation_ApplyChildValuesFar(object, 6);
  if (*alternate == 0)
    other = Actor_Spawn(354, scene->x, scene->y, scene->z);
  else
    other = Actor_Spawn(355, scene->x, scene->y, scene->z);
  work->other = other;
  if (other == NULL)
    goto end;
  other->z += 0x30000;
  other->motionFlags = 0;
  other->priorityFlags = 1;
  other->scaleX = 0;
  if (!(event->unkd4c & 1))
    Func_080dba5c();
  Sound_PlayFar(138);
  Func_080dbed0(object, scene->x, scene->y + 0x200000, scene->z);
  wait(10);
  Actor_SetMode(object, 1);
  wait(15);
  for (i = 9; i >= 0; i--) {
    object->y += -0x28000;
    wait(1);
  }
  Actor_SetMode(object, 2);
  Sound_PlayFar(132);

  failed = 0;
  for (i = 0; i < 80; i++) {
    target = event->work.objects[i];
    if (target != NULL && target->script != NULL &&
        (Func_08020330(target) == 406 || (target->collisionFlags & 1)) &&
        target != scene->mainObject && ((u8 *)target->sprite)[27] != 0 &&
        Func_080dbe80(&scene->x, 4, &target->x, target->radius - 2) >= 0) {
      failed = 1;
      break;
    }
  }
  if (Func_080dbdc8(scene->x, scene->z, caster->terrainId) == 255)
    failed = 4;
  if (Func_080cda84(scene->fieldKind, caster, &scene->x) == -1)
    failed = 2;
  if (Func_080dbc04(scene->x, scene->z, caster->terrainId) == 0)
    failed = 3;
  if (failed) {
    wait(2);
    for (i = 15; i >= 0; i--)
      BattleEffect_SpawnScatterParticle(object->x, object->y, object->z);
    Sound_PlayFar(114);
    wait(1);
    goto end;
  }

  if (scene->enabled != 0) {
    if (Random16() < 0x1000)
      work->mode = 1;
    else if (*alternate == 0)
      work->mode = 0;
    else
      work->mode = 3;
  } else {
    work->mode = 2;
  }

  if (work->mode == 0) {
    Resource_DecodeType01(Resource_GetTableEntry((s32)&ResourceId_DustPuffA),
                          work);
    particleEntry = Resource_FindFreeEntry();
    particleTile = VramCache_Load(particleEntry, 0x800, work);
    work->particleTile = particleTile;
    i = 0;
    particle = work->particles;
    for (; i < 64; i++, particle++) {
      sprite = &work->sprites[i];
      Func_080eaf98(sprite, 8, 8, 0x40000000, particleTile);
      sprite->oam.palette256 = 1;
      sprite->oam.palette = 0;
      sprite->oam.priority = BattleEffect_GetObjectPriority(other);
      sprite->depth = BattleEffect_GetObjectDepth(other) + 1;
      sprite->scaleY = sprite->scaleX = 0x3333;
      particle->x = scene->x;
      particle->y = scene->y;
      particle->z = scene->z;
      Vector_AddPolarOffset(0x20000, Random16(), &particle->x);
      particle->velocityX = 0;
      particle->velocityY = 0;
      particle->velocityZ = 0;
      Vector_AddPolarOffset(0x13333, Random16(), &particle->velocityX);
      particle->timer = -i * 2;
    }
  } else if (work->mode == 2) {
    if (*alternate == 0)
      data = Resource_GetTableEntry((s32)&ResourceId_DustPuffA);
    else
      data = Resource_GetTableEntry((s32)&ResourceId_DustPuffB);
    Resource_DecodeType01(data, work);
    particleEntry = Resource_FindFreeEntry();
    particleTile = VramCache_Load(particleEntry, 0x800, work);
    work->particleTile = particleTile;
    i = 0;
    particle = work->particles;
    for (; i < 64; i++, particle++) {
      sprite = &work->sprites[i];
      Func_080eaf98(sprite, 8, 8, 0x40000000, particleTile);
      sprite->oam.palette256 = 1;
      sprite->oam.palette = 0;
      sprite->oam.priority = BattleEffect_GetObjectPriority(other);
      sprite->depth = BattleEffect_GetObjectDepth(other) + 1;
      particle->x = scene->x;
      particle->y = scene->y;
      particle->z = scene->z;
      Vector_AddPolarOffset(0x20000, Random16(), &particle->x);
      particle->velocityX = 0;
      particle->velocityY = 0;
      particle->velocityZ = 0;
      Vector_AddPolarOffset(0x10000, Random16(), &particle->velocityX);
      particle->timer = -i;
    }
  } else if (work->mode == 3) {
    Resource_DecodeType01(Resource_GetTableEntry((s32)&ResourceId_WhiteSparkle),
                          work);
    particleEntry = Resource_FindFreeEntry();
    particleTile = VramCache_Load(particleEntry, 0x80, work);
    work->particleTile = particleTile;
    i = 0;
    particle = work->particles;
    sprite = work->sprites;
    for (; i < 64; i++) {
      Func_080eaf98(sprite, 4, 4, 0, particleTile + (i & 1) * 2);
      sprite->oam.palette256 = 1;
      sprite->oam.palette = 0;
      sprite->oam.priority = BattleEffect_GetObjectPriority(object);
      sprite->depth = BattleEffect_GetObjectDepth(object) - 1;
      particle->x = scene->x;
      particle->y = scene->y + 0x280000;
      particle->z = scene->z;
      Vector_AddPolarOffset(Random16() * 4 + 0x30000, Random16(), &particle->x);
      particle->timer = -i * 3 / 4;
      sprite++;
      particle++;
    }
  }

  if (work->mode == 0) {
    Resource_DecodeType01(Resource_GetTableEntry((s32)&ResourceId_Data1E9),
                          work);
    pictureEntry = Resource_FindFreeEntry();
    pictureTile = VramCache_Load(pictureEntry, 0x400, NULL);
    work->pictureTile = pictureTile;
    sprite = &work->pictureSprite;
    Func_080eaf98(sprite, 16, 0, 0x80000000, pictureTile);
    sprite->depth = BattleEffect_GetObjectDepth(scene->mainObject);
    sprite->oam.priority = BattleEffect_GetObjectPriority(scene->mainObject);
    sprite->oam.palette256 = 1;
    sprite->oam.palette = 0;
    pos[0] = object->x;
    pos[1] = object->y + 0x200000;
    pos[2] = object->z;
    Camera_WorldToScreen(pos);
    sprite->x = pos[0];
    sprite->y = pos[2];
  }
  if (work->mode == 0 || work->mode == 3) {
    wait(6);
    work->other->scaleX = 0x10000;
    wait(4);
  } else {
    wait(10);
  }

  work->phase = 0;
  work->timer = 0;
  {
    // A local holding one constant: keeps 0x480's load where the native has it.
    s32 order = 0x480;

    Task_Add(Func_080e6400, order);
  }
  if (work->mode == 1) {
    frame = 0;
    while (work->phase != 999) {
      if (frame == 24)
        Func_080e716c();
      wait(1);
      frame++;
    }
  } else {
    pending = Func_080ce458(0x50000005, scene->fieldKind,
                            Func_080ce31c(scene->fieldKind));
    frame = 0;
    while (work->phase != 999) {
      stage = 0;
      wait(1);
      if (frame == 30)
        stage = 1;
      if (frame == 50)
        stage = 2;
      {
        /* FAKEMATCH(barrier): frame copied through an empty asm, so its move
         * to r2 comes before pending's to r0; as
         * Field_RunPsynergyEvent(pending, stage, frame), 2 of 938 instructions
         * differ in tla-en. */
        s32 frameCopy;

        asm("" : "=r"(frameCopy) : "0"(frame));
        scene->stage = stage;
        scene->frame = frame;
        Field_RunPsynergyEvent(pending, stage, frameCopy);
      }
      frame++;
    }
    Field_RunPsynergyEvent(pending, 999, frame);
  }
  Task_Remove(Func_080e6400);
  Sound_PlayFar(114);
  Func_080dbf94(object, object->x, object->y, object->z);
  if (particleEntry != 96)
    Resource_ResetEntry(particleEntry);
  if (pictureEntry != 96)
    Resource_ResetEntry(pictureEntry);
end:
  Actor_Destroy(object);
  Actor_Destroy(other);
  BattleEffect_PrepareBufferInterpolation();
  HeapSlot_Release(92);
}




struct Actor *Actor_GetById(u32 objectId);

/// Puts object id at the effect's anchor (if any) and lets it fall under
/// gravity, waits 50 frames, then cues the event with id | 0x1000.
void BattleEffect_DropObjectAtAnchor(s32 id) {
  struct ModeEvent *event = Ram_HeapSlots->eventWork;
  struct Actor *anchor;
  struct Actor *object;

  anchor = ((struct AnchorWork *)Ram_HeapSlots->battleFxWork[0])->anchor;
  object = Actor_GetById(id);
  if (anchor != NULL) {
    SET_POSITION(object, anchor->x, anchor->y, anchor->z);
  }
  if (*((s8 *)event + 394) == 3) {
    object->motionFlags = 2;
    object->terrainHeight = 0;
  } else {
    object->motionFlags = 3;
  }
  object->gravity = 0x170a;
  wait(50);
  event->cue = id | 0x1000;
}

#include "battle_unit.h"

struct DropEvent {
  u8 unk000[0xd34];
  u16 item;
};

struct Actor *Func_080200c0(s32 kind, s32 x, s32 y, s32 z);
void *Unit_Get(s32 owner);
s32 PartyInventory_FindRoomFar(s32 item);
void Func_080d3b28(s32 id, s32 item);
extern const u16 gData_080f0fc0[];

/// Object 79 at the anchor, fetched or spawned (kind 489), takes a random item
/// id from gData_080f0fc0's sixteen (0x8001 when PartyInventory_FindRoomFar
/// finds no room) or, past 0xc000 of the roll, a 0x8000-flagged value scaled
/// by the unit's level, and is handed to BattleEffect_DropObjectAtAnchor.
void Func_080e716c(void) {
  struct AnchorWork *work = (struct AnchorWork *)Ram_HeapSlots->battleFxWork[0];
  struct Actor *anchor;
  struct EventWork *event;
  struct BattleUnit *unit;
  struct Actor *object;
  s32 item;
  s32 id = 79;

  anchor = work->anchor;
  event = Ram_HeapSlots->eventWork;
  unit = Unit_Get(gMainRoutineWork.currentOwner);

  object = Actor_GetById(id);
  if (object == NULL) {
    object = Func_080200c0(489, anchor->x, anchor->y, anchor->z);
    if (object == NULL)
      return;
    event->objects[id] = object;
  }
  if (Random16() < 0xc000) {
    item = gData_080f0fc0[(Random16() * 16) >> 16];
    if (PartyInventory_FindRoomFar(item) == -1)
      item = 0x8001;
  } else {
    item = (((Random16() * unit->level) * 2) >> 16) + 1 | 0x8000;
  }
  object->x = anchor->x;
  object->y = anchor->y;
  object->z = anchor->z;
  Func_080d3b28(id, item);
  ((struct DropEvent *)event)->item = item;
  BattleEffect_DropObjectAtAnchor(id);
}


#define WHIRL_COUNT 100

struct WhirlSpark {
  s32 x;
  s32 y;
  s32 z;
  s32 angle;
  s32 unk10;
  s32 radius;
  s32 age;
};

// Heap block 92: the decoded picture, then the sparks and their sprites.
/* VIEW(f0e6400): tag struct WhirlWork as struct WhirlWork__f0e6400; f0e2c38.c has another definition. */
struct WhirlWork__f0e6400 {
  u8 picture[0x400];
  struct WhirlSpark sparks[WHIRL_COUNT];
  struct EffectSprite sprites[WHIRL_COUNT];
};

extern char ResourceId_ColouredOrbs;

void Func_080dc954(void);

void Func_080e7238(void) {
  s32 position[3];
  struct WhirlWork__f0e6400 *work = HeapSlot_Alloc(92, sizeof(*work));
  struct Actor *object = ObjectTable_Get(Party_GetLeaderActor());
  struct WhirlSpark *spark;
  struct EffectSprite *sprite;
  s32 next;
  s32 state;
  s32 entry;
  s32 count;
  s32 tiles;
  s32 i;
  s32 angle;
  s32 frame;
  s32 radius;

  Resource_DecodeType01(Resource_GetTableEntry((s32)&ResourceId_ColouredOrbs),
                        work);
  entry = Resource_FindFreeEntry();
  tiles = VramCache_Load(entry, 0x400, work);
  for (i = 0, spark = work->sparks; i < WHIRL_COUNT; i++, spark++) {
    sprite = &work->sprites[i];
    Func_080eaf98(sprite, 4, 4, 0, tiles + (i & 15) * 2);
    sprite->oam.palette256 = 1;
    sprite->oam.palette = 0;
    sprite->oam.priority = 0;
    sprite->depth = 240;
    spark->age = -1;
  }
  Sound_PlayFar(200);
  wait(20);
  object->scaleX = 0;
  object->scaleY = 0;
  Actor_SetSingleChildFlagsFar(object, 0);

  count = 2;
  state = 0;
  next = 0;
  frame = 0;
  do {
    if (count != 0) {
      radius = 0x500000;
      i = count;
      do {
        angle = Random16();
        spark = &work->sparks[next];
        spark->age = 0;
        spark->x = object->x;
        spark->y = object->y + Random16() * 20;
        spark->z = object->z;
        Vector_AddPolarOffset(Random16() << 3, angle, &spark->x);
        next++;
        spark->angle = angle;
        spark->radius = radius;
        next %= WHIRL_COUNT;
        i--;
      } while (i != 0);
    }

    spark = work->sparks;
    sprite = work->sprites;
    for (i = 0; i < WHIRL_COUNT; i++, sprite++, spark++) {
      if (spark->age >= 0) {
        if ((u32)spark->age < 20) {
          s32 sparkAngle = spark->angle;
          s32 sparkRadius = spark->radius;
          position[0] = spark->x;
          position[1] = spark->y;
          position[2] = spark->z;
          Vector_AddPolarOffset(sparkRadius, sparkAngle, position);
          Camera_WorldToScreen(position);
          sprite->x = position[0];
          sprite->y = position[2];
          Func_080eb01c(sprite);
          spark->angle += 0x800;
          spark->radius += -0x40000;
        }
        if (++spark->age == 20)
          spark->age = -1;
      }
    }

    switch (state) {
    case 0:
      if (frame == 20)
        count = 3;
      if (frame == 30) {
        count = 5;
        state = 1;
        frame = -1;
      }
      break;
    case 1:
      object->facing += 0x2000;
      object->scaleX = (frame << 16) / 30;
      if (object->scaleX > 0x10000)
        object->scaleX = 0x10000;
      object->scaleY = object->scaleX;
      if (frame == 40) {
        state = 2;
        frame = -1;
      }
      break;
    case 2:
      if (frame == 0) {
        object->facing = 0x4000;
        Actor_SetSingleChildFlagsFar(object, 1);
        count = 3;
      }
      if (frame == 10)
        count = 1;
      if (frame == 20)
        count = 0;
      if (frame == 40)
        state = 999;
      break;
    }
    wait(1);
    frame++;
  } while (state != 999);
  Resource_ResetEntry(entry);
  HeapSlot_Release(92);
  Func_080dc954();
}

#define SPARK_COUNT 100

struct SparkParticle {
  s32 x;
  s32 y;
  s32 z;
  s32 velocityX;
  s32 velocityY;
  s32 velocityZ;
  s32 age;
};

// Heap block 92: the decoded picture, then the sparks and their sprites.
struct SparkWork {
  u8 picture[0x400];
  struct SparkParticle particles[SPARK_COUNT];
  struct EffectSprite sprites[SPARK_COUNT];
};

void Actor_ArmCallback(s32 actor, s32 angle, s32 frames);
void Actor_ApplyRandomChildValues(void);

void Func_080e74d8(void) {
  struct SparkWork *work = HeapSlot_Alloc(92, sizeof(*work));
  s32 owner = Party_GetLeaderActor();
  struct Actor *object = ObjectTable_Get(Party_GetLeaderActor());
  struct SparkParticle *particle;
  struct EffectSprite *sprite;
  s32 state;
  s32 entry;
  s32 count;
  s32 tiles;
  s32 i;
  s32 spin;
  s32 angle;
  s32 next;
  s32 frame;
  s32 zero = 0;
  s32 age;

  wait(20);
  BattleEffect_InitializeSharedScene();
  Resource_DecodeType01(Resource_GetTableEntry((s32)&ResourceId_ColouredOrbs),
                        work);
  entry = Resource_FindFreeEntry();
  tiles = VramCache_Load(entry, 0x400, work);
  for (i = 0, particle = work->particles; i < SPARK_COUNT; i++, particle++) {
    sprite = &work->sprites[i];
    Func_080eaf98(sprite, 4, 4, 0, tiles + (i & 15) * 2);
    sprite->oam.palette256 = 1;
    sprite->oam.palette = 0;
    sprite->oam.priority = 0;
    sprite->depth = 240;
    particle->age = -1;
  }
  wait(10);
  Actor_ArmCallback(owner, 0x4000, 0);
  wait(20);
  Actor_SetMode(object, 28);
  object->update = Actor_ApplyRandomChildValues;
  Sound_PlayFar(131);
  wait(30);
  Sound_PlayFar(220);
  Actor_SetMode(object, 3);
  for (spin = 0, frame = 49; frame >= 0; frame--, spin += 60) {
    object->facing += spin + 0x1000;
    wait(1);
  }

  count = 2;
  state = 0;
  next = 0;
  frame = 0;
  do {
    for (i = 0; i < count; i++) {
      angle = Random16();
      particle = &work->particles[next];
      particle->age = zero;
      particle->x = object->x;
      particle->y = object->y + Random16() * 20;
      particle->z = object->z;
      Vector_AddPolarOffset(Random16() << 3, angle, &particle->x);
      if (next & 1) {
        particle->velocityX = zero;
        particle->velocityY = -0x40000;
        particle->velocityZ = zero;
        Vector_AddPolarOffset((Random16() << 2) + 0x40000, angle,
                              &work->particles[next].velocityX);
      } else {
        particle->velocityX = 0;
        particle->velocityY = 0x50000;
        particle->velocityZ = 0;
        Vector_AddPolarOffset((Random16() << 2) + 0x40000, angle,
                              &work->particles[next].velocityX);
      }
      next++;
      next %= SPARK_COUNT;
    }

    for (i = 0, particle = work->particles, sprite = work->sprites;
         i < SPARK_COUNT; i++, sprite++, particle++) {
      age = particle->age;
      if (age >= 0) {
        if ((u32)age < 20) {
          EffectSprite_DrawAtWorldPosition__f0e6400(sprite, particle);
          // Reread in each arm: the two calls share one bl only this way.
          if (i & 1) {
            BattleEffect_IntegrateVector3(&particle->x, 63, 0x8000);
            age = particle->age;
          } else {
            BattleEffect_IntegrateVector3(&particle->x, 63, -0xa000);
            age = particle->age;
          }
        }
        particle->age = ++age;
        if (age == 20)
          particle->age = -1;
      }
    }

    switch (state) {
    case 0:
      if (frame == 10)
        Sound_PlayFar(198);
      object->facing += 0x2000;
      if (frame == 40)
        count = 3;
      if (frame == 50)
        count = 4;
      if (frame == 60) {
        count = 5;
        state = 1;
        frame = -1;
      }
      break;
    case 1:
      object->facing += 0x2000;
      object->scaleX = 0x10000 - (frame << 16) / 30;
      if (object->scaleX < 0)
        object->scaleX = 0;
      object->scaleY = object->scaleX;
      Actor_SetSingleChildFlagsFar(object, 0);
      if (frame == 30) {
        state = 2;
        frame = -1;
      }
      break;
    case 2:
      count = 0;
      if (frame == 20)
        state = 999;
      break;
    }
    wait(1);
    frame++;
  } while (state != 999);
  Resource_ResetEntry(entry);
  HeapSlot_Release(92);
}

/// Runs Func_080e74d8, fades the palettes back to the saved ones and waits
/// 10 frames.
void Func_080e7804(void) {
  Func_080e74d8();
  BattleEffect_PrepareBufferInterpolation();
  wait(10);
}

/// Field Psynergy kind 30's targeted handler.
void Func_080e7818(void) {}

/// Field Psynergy kind 30.
void BattleEffect_RunEffect30(void) {
  ObjectTable_Get(Party_GetLeaderActor());
  ((struct ControlEvent *)Ram_HeapSlots->eventWork)->cue = 0xfc89;
}

/// Field Psynergy kind 22 on a chosen target: runs kind 22's animation.
void BattleEffect_CallEffect22(void) { Func_080e7844(); }

#include "window.h"

struct PuffParticle {
  s32 x;
  s32 y;
  s32 z;
  s32 sway;
  s32 rise;
  s32 swayRate;
  s32 timer;
};

struct PuffWork {
  u8 tiles[0x1000];
  struct EffectSprite disc;
  u8 unk1028[0x28];
  s32 discX;
  s32 discY;
  s32 discScaleX;
  s32 discScaleY;
  struct EffectSprite sprites[64];
  struct PuffParticle particles[64];
};

/* VIEW(f0e6400): tag struct ObjectAnimation as struct ObjectAnimation__f0e6400; f0e2c38.c has another definition. */
struct ObjectAnimation__f0e6400 {
  u8 unk00[24];
  u8 unk18;
  u8 unk19[15];
  s16 *definition;
};

extern char ResourceId_GlowDisc, ResourceId_WhitePuff;

s32 Func_080dbd5c(s32 x, s32 z, s32 layer);
s32 Func_080db9a8(struct Actor *object, s32 facing);
void BattleEffect_ApplyColorToTargetBuffer(s32 packed, s32 mode);
void Func_080dbdd4(s32 x, s32 z, s32 layer, s32 value);
void Actor_SetHorizontalPositionWithTerrain(s32 id, s32 a, s32 b);
void BattleEffect_FadeToSavedPalettes(s32 frames);

void Func_080e7844(void) {
  struct PuffWork *work = HeapSlot_Alloc(92, 0x2160);
  struct HeapSlots *slots;
  struct FxScene__f0e6400 *scene = Ram_HeapSlots->effectWork;
  struct WindowWork *vramWork = (struct WindowWork *)Ram_HeapSlots->windowTiles;
  s32 offset; // byte offset of the event work's 80 object pointers
  struct Actor *caster;
  struct Actor *object;
  struct ObjectAnimation__f0e6400 *animation;
  struct EffectSprite *sprite;
  struct EffectSprite *cursor;
  struct EffectSprite *fresh;
  struct PuffParticle *particle;
  s32 screen[3];
  s32 pos[3];
  s32 phase;
  s32 timer;
  s32 frame;
  s32 entry;
  s32 tile;
  s32 next;
  s32 spawn;
  s32 fade;
  s32 result;
  s32 found;
  s32 kind;
  void *pending;
  s32 scaleY;
  s32 scaleX;
  s32 y;
  s32 x;
  s32 red;
  s32 green;
  s32 blue;
  s32 add;
  s32 packed;
  s32 i;
  u8 *dst;

  caster = scene->mainObject;
  found = -1;
  fade = -1;
  pending = NULL;
  BattleEffect_InitializeSharedScene();
  pos[0] = caster->x;
  pos[1] = caster->y;
  pos[2] = caster->z;
  Vector_AddPolarOffset(0x200000, scene->facing, pos);

  // The kind and the heap slots are locals that hold one value each, set
  // after i so the kind's constant is built where the listing builds it.
  i = 0;
  kind = 290;
  slots = Ram_HeapSlots;
  for (offset = 20; i < 80; i++, offset += 4) {
    object = *(struct Actor **)((u8 *)slots->eventWork + offset);
    animation = object->sprite;
    if (object->script != NULL && animation->definition[0] == kind &&
        animation->unk18 == 1 &&
        Func_080dbe80(pos, 4, &object->x, object->radius - 2) >= 0) {
      result = 2;
      found = i;
      Actor_GetById(i)->priorityFlags |= 2;
      goto loaded;
    }
  }
  if (Func_080cda84(scene->fieldKind, caster, pos) == -1) {
    result = 0;
    // DEAD: pending is already NULL; the ROM stores it again.
    pending = NULL;
  } else if (Func_080dbd5c(pos[0], pos[2], caster->terrainId) != 0) {
    pending = Func_080ce458(0x50000005, scene->fieldKind,
                            Func_080dbdc8(pos[0], pos[2], caster->terrainId));
    result = 2;
    if (pending == NULL)
      result = 1;
  } else {
    result = 0;
  }
loaded:
  {
    void *source = Resource_GetTableEntry((s32)&ResourceId_GlowDisc);
    dst = (u8 *)0x06011000 + vramWork->glyphTiles * 32;
    Resource_DecodeType01(source, work);
  }
  Dma_Set(work, dst, (DMA_ENABLE | DMA_32BIT) << 16 | 0x400, REG_DMA3);
  screen[0] = pos[0];
  screen[1] = pos[1];
  screen[2] = pos[2];
  Camera_WorldToScreen(screen);
  work->discX = screen[0];
  work->discY = screen[2];
  work->discScaleX = 0;
  work->discScaleY = 0;
  Resource_DecodeType01(Resource_GetTableEntry((s32)&ResourceId_WhitePuff),
                        work);
  entry = Resource_FindFreeEntry();
  tile = VramCache_Load(entry, 0x800, work);
  particle = work->particles;
  for (i = 0; i < 64; i++) {
    fresh = &work->sprites[i];
    Func_080eaf98(fresh, 8, 15, 0x40000000, tile);
    fresh->depth = 250;
    fresh->oam.priority = 0;
    fresh->oam.palette256 = 1;
    fresh->oam.palette = 0;
    particle->timer = -1;
    particle++;
  }
  Sound_PlayFar(130);
  wait(1);
  next = 0;
  spawn = 0;
  phase = 0;
  timer = 0;
  frame = 0;
  do {
    for (i = spawn; i != 0; i--) {
      s32 slot = next & 63;
      sprite = &work->sprites[slot];
      sprite->scaleX = 0x3333;
      sprite->scaleY = 0x3333;
      particle = &work->particles[slot];
      particle->x = pos[0];
      particle->y = pos[1];
      particle->z = pos[2];
      Vector_AddPolarOffset(Random16() * 12 + 0x90000, Random16(),
                            &particle->x);
      particle->sway = (Random16() << 5) >> 16;
      particle->swayRate = ((Random16() << 9) >> 16) + 256;
      particle->rise = Random16() + 0x3333;
      particle->timer = 0;
      next = (next + 1) & 63;
    }
    particle = work->particles;
    cursor = work->sprites;
    for (i = 63; i >= 0; i--) {
      if (particle->timer >= 0) {
        s32 grown;
        /* FAKEMATCH(temporary): sprite holds a copy of cursor; as cursor used
         * directly, 57 of 871 instructions differ in tla-en. */
        sprite = cursor;
        sprite->oam.index = tile + ((particle->timer >> 2) & 7) * 8;
        grown = sprite->scaleX + 0x1999;
        // DEAD: stored again after the clamp; the ROM keeps both stores.
        sprite->scaleX = grown;
        if (grown > 0x10000)
          grown = 0x10000;
        sprite->scaleX = grown;
        sprite->scaleY = grown;
        screen[0] = particle->x;
        screen[1] = particle->y;
        screen[2] = particle->z;
        Camera_WorldToScreen(screen);
        sprite->x =
            screen[0] + (Trig_Sin(particle->sway * particle->swayRate) << 3);
        sprite->y = screen[2];
        Func_080eb01c(sprite);
        particle->sway++;
        particle->y += particle->rise;
        if (++particle->timer == 32)
          particle->timer = -1;
      }
      cursor++;
      particle++;
    }

    for (i = 0; i <= 31; i++) {
      if (fade == -1)
        add = 0;
      else
        add = fade / 4;
      blue = Trig_Sin((frame / 2 + i) << 11); // blue reused for green's sine
      green = (blue * 8 >> 16) + 23;
      blue = Trig_Sin(((frame / 4 + i) << 11) + 0x4800);
      blue = (blue * 10 >> 16) + 12;
      red = add + 28;
      if (red > 31)
        red = 31;
      green += add;
      if (green > 31)
        green = 31;
      blue += add;
      if (blue > 31)
        blue = 31;
      do { // once-only block: the listing schedules the store with it
        packed = (blue << 10) | (green << 5) | red;
        ((u16 *)0x05000200)[255 - i] = packed;
      } while (0);
    }
    if (fade != -1 && fade <= 126)
      fade++;

    x = work->discX;
    y = work->discY;
    scaleX = work->discScaleX;
    scaleY = work->discScaleY;
    packed = vramWork->glyphTiles + 128;
    sprite = &work->disc;
    Func_080eaf98(sprite, 31, 31, 0xc0000000, packed);
    sprite->oam.palette256 = 1;
    sprite->oam.palette = 0;
    sprite->oam.priority = Func_080db9a8(scene->mainObject, scene->facing);
    sprite->depth = 1;
    sprite->x = x;
    sprite->y = y;
    sprite->scaleX = scaleX;
    sprite->scaleY = scaleY;
    Func_080eb01c(sprite);

    if (result == 0) {
      switch (phase) {
      case 0:
        if (timer == 10)
          Sound_PlayFar(542);
        work->discScaleX += 0x300;
        work->discScaleY = work->discScaleX;
        if (work->discScaleX > 0x3fff) {
          phase = 1;
          timer = -1;
        }
        break;
      case 1:
        if (timer == 30) {
          phase = 2;
          timer = -1;
        }
        break;
      case 2:
        spawn = 0;
        work->discScaleX += -0x300;
        work->discScaleY = work->discScaleX;
        if (work->discScaleX <= 0)
          phase = 999;
        break;
      }
    }
    if (result == 1) {
      switch (phase) {
      case 0:
        if (timer == 30)
          Sound_PlayFar(140);
        work->discScaleX += 0x300;
        work->discScaleY = work->discScaleX;
        if (work->discScaleX > 0xbfff) {
          phase = 1;
          timer = -1;
        }
        break;
      case 1:
        spawn = 2;
        if (timer == 40) {
          phase = 2;
          timer = -1;
        }
        break;
      case 2:
        spawn = 1;
        if (timer == 40) {
          phase = 3;
          timer = -1;
        }
        break;
      case 3:
        spawn = 0;
        work->discScaleX += -0x400;
        work->discScaleY = work->discScaleX;
        if (work->discScaleX <= 0)
          phase = 999;
        break;
      }
    }
    if (result == 2) {
      switch (phase) {
      case 0:
        if (timer == 40)
          Sound_PlayFar(140);
        work->discScaleX += 0x300;
        work->discScaleY = work->discScaleX;
        if (work->discScaleX > 0xbfff) {
          phase = 1;
          timer = -1;
        }
        break;
      case 1:
        if (timer == 40) {
          fade = 0;
          BattleEffect_ApplyColorToTargetBuffer(0x7fff, 0);
          BattleEffect_StartBufferInterpolation(100);
        }
        spawn = 2;
        if (timer == 100) {
          phase = 2;
          timer = -1;
        }
        break;
      case 2:
        spawn = 1;
        if (timer == 80)
          phase = 999;
        break;
      }
    }
    wait(1);
    timer++;
    frame++;
  } while (phase != 999);

  if (found != -1) {
    struct Actor *marked = Actor_GetById(found);
    Func_080dbdd4(marked->x, marked->z, marked->terrainId, 0);
    Actor_SetHorizontalPositionWithTerrain(found, 0, 0);
  }
  if (pending != NULL)
    Field_RunPsynergyEvent(pending, scene->firstObjectId,
                           scene->secondObjectId);
  Resource_ResetEntry(entry);
  if (result == 2)
    BattleEffect_FadeToSavedPalettes(100);
  else
    BattleEffect_PrepareBufferInterpolation();
  HeapSlot_Release(92);
}


void BattleEffect_PlotCanvasPixel(s32 x, s32 y, s32 color);

/// Plots one pixel of the 128x64 canvas on a circle around its bottom middle
/// (63, 63): a radius (16.16) and an angle, the cosine leftward and the sine
/// upward.
void Func_080e7f94(s32 unused, s32 radius, s32 angle, s32 color) {
  /* FAKEMATCH(pin): the angle pinned to r8, which leaves the multiply's entry
   * to r10; as angle itself, 12 of 39 instructions differ in tla-en. */
  register s32 turn asm("r8") = angle;
  s32 center = 63;
  s32 x = Iwram_MulQ16(Trig_Cos(turn), radius);
  s32 y = Iwram_MulQ16(Trig_Sin(turn), radius);

  BattleEffect_PlotCanvasPixel(center - (x >> 16), center - (y >> 16), color);
}

extern const u16 gData_080f39c4[];

/// Copy a 16x16 image to the BG map, clipping cells outside the 30x20-tile
/// screen.
void Func_080e7fec(s32 x, s32 y) {
  const u16 *tiles = gData_080f39c4;
  s32 row;
  s32 column;
  s32 sx;
  s32 sy;
  s32 (*fill)(void *, s32, u32);

  // The opening in a once-only block keeps the destination's load after the
  // fill value's 0; as plain statements, 4 of 66 instructions differ in
  // tla-en.
  do {
    x /= 8;
    y /= 8;
    fill = Iwram_FillWordsEntry;
    /* FAKEMATCH(barrier): an empty asm taking the fill routine's entry, so its
     * load comes before the size; without it, 4 of 66 instructions differ in
     * tla-en. */
    asm volatile("" : : "l"(fill));
    fill((void *)0x06002000, 0x400, 0);
  } while (0);
  for (row = 0, sy = y; row < 16; row++, sy++) {
    if (sy < 0)
      continue;
    if (sy > 19)
      break;
    for (sx = x, column = 0; column < 16; column++, sx++) {
      if (sx < 0)
        continue;
      if (sx > 29)
        break;
      ((u16 *)0x06002000)[row * 32 + column] = tiles[row * 16 + column];
    }
  }
}

#include "bg_scroll.h"

struct Unk_080e807cEntry {
  s32 x;
  s32 y;
  s32 z;
  s32 unk0c;
  s32 unk10;
  s32 unk14;
  s32 timer;
};

struct Unk_080e807cWork {
  u8 unk0000[0x2100];
  struct EffectSprite sprites[16];
  u8 unk2380[0x280];
  struct Unk_080e807cEntry entries[16];
  u8 unk27c0[0x1c2];
  u16 tile;
  s8 unk2984;
  s8 unk2985;
  s8 unk2986;
  u8 unk2987;
  u16 phase;
};

struct Unk_080e807cEvent {
  u8 unk000[0x166];
  u16 unk166;
  u8 unk168[0xb45];
  s8 unkcad;
  s16 count;
};

void Func_080e807c(void) {
  struct Unk_080e807cWork *work = Ram_HeapSlots->battleFxWork[0];
  struct Unk_080e807cEvent *event = Ram_HeapSlots->eventWork;
  struct Actor *actor = Actor_GetById(gMainRoutineWork.currentOwner);
  s32 position[3];
  s32 *pos = position;
  struct Unk_080e807cEntry *entry;
  struct EffectSprite *sprite;
  s32 x;
  s32 y;
  s32 i;
  s32 timer;
  u16 *phase;
  u16 *tile;
  s32 red;

  pos[0] = actor->x;
  pos[1] = actor->y + 0x100000;
  pos[2] = actor->z;
  if (work->unk2985 != 0)
    Vector_AddPolarOffset(0x20000, -(event->count << 11), pos);
  Camera_WorldToScreen(pos);
  x = pos[0] >> 16;
  gBgScroll[0].x = 64 - x;
  y = pos[2] >> 16;
  gBgScroll[0].y = 64 - y;
  Func_080e7fec(x - 64, y - 64);

  if (work->unk2986 != 0) {
    i = 0;
    phase = &work->phase;
    red = 28;
  cycle:
    {
      /* FAKEMATCH(pin): green pinned to r5, so the call's result is shifted
       * straight into it; as a plain local, 17 of 272 instructions differ in
       * tla-en. */
      register s32 green asm("r5") = Trig_Sin(((*phase >> 1) + i) << 11) * 8;
      s32 blue;
      s32 color;

      blue = Trig_Sin((((*phase >> 2) + i) << 11) + 0x4800) * 10;
      green = (green >> 16) + 23;
      blue = ((blue >> 16) + 12) << 10;
      green <<= 5;
      // An empty once-only loop keeps red's 28 after green's or; without it, 2
      // of 272 instructions differ in tla-en.
      do {
      } while (0);
      color = blue | green | red;

      ((u16 *)0x050001e0)[i] = color;
      ((u16 *)0x050003e0)[i] = color;
    }
    if (++i < 16)
      goto cycle;
    work->phase++;
  }

  if (work->unk2984 != 0) {
    entry = work->entries;
    sprite = work->sprites;
    tile = &work->tile;
    for (i = 15; i >= 0; i--, sprite++, entry++) {
      timer = entry->timer;
      if (timer == 0) {
        sprite->angle = Random16();
        pos[2] = timer;
        pos[1] = timer;
        pos[0] = timer;
        Vector_AddPolarOffset(0x360000, sprite->angle + 0x2000, pos);
        entry->x = pos[0];
        entry->y = pos[1] + 0x100000;
        entry->z = pos[2];
        timer = entry->timer;
      }
      if (timer >= 0) {
        sprite->oam.index = *tile + ((timer >> 2) & 3) * 4;
        pos[0] = actor->x + entry->x;
        pos[1] = actor->y + entry->y;
        pos[2] = actor->z + entry->z;
        EffectSprite_DrawAtWorldPosition__f0e6400(sprite, pos);
        timer = entry->timer;
      }
      if ((entry->timer = timer + 1) == 16)
        entry->timer = 0;
    }
  }

  if (event->unkcad != 0 && event->count != 0)
    event->count--;
  if (event->count == 0)
    event->unk166 = 0x209b;
}
#undef NEXT_PHASE
#undef WHIRL_COUNT
#undef SPARK_COUNT

// ---- games/tla/src/field/event_commands/f0e82cc.c ----
struct RingMote {
  s32 x;
  s32 y;
  s32 z;
  s32 unk0c;
  s32 unk10;
  s32 unk14;
  s32 timer;
};

struct RingWork {
  u8 tiles[0x1000];
  struct EffectSprite sparkSprites[64];
  struct RingMote sparks[64];
  struct EffectSprite sprites[32];
  struct RingMote motes[32];
  u16 entry;
  u16 tile;
  s8 sparkling;
  s8 orbiting;
  s8 cycling;
  u8 unk2987;
  u16 phase;
};

struct RingEvent {
  u8 unk000[0xcac];
  s8 up;
  s8 unkcad;
  s16 timer;
};

/* VIEW(f0e82cc): tag struct FxScene as struct FxScene__f0e82cc; f0e2c38.c has another definition. */
struct FxScene__f0e82cc {
  u16 facing;
  u16 angle;
  s32 x;
  s32 y;
  s32 z;
  struct Actor *mainObject;
};

extern char ResourceId_StarSparkleFade, ResourceId_RingArcs, ResourceId_Data1ED;

void *HeapSlot_AllocEwram(s32 slot, s32 size);
void HeapSlot_Release(s32 slot);
void BattleEffect_InitializeSharedScene(void);
void Camera_WorldToScreen(s32 *position);
void wait(s32 frames);
void *Resource_GetTableEntry(s32 resourceId);
s32 Resource_DecodeType01(const void *source, void *destination);
s32 Resource_FindFreeEntry(void);
void Func_080e807c(void);
/* VIEW(f0e82cc): routine Func_080e7f94 as void Func_080e7f94(u8 *canvas, s32 radius, s32 angle, s32 color); the definition has void Func_080e7f94(s32 unused, s32 radius, s32 angle, s32 color). */
extern void Func_080e7f94__f0e82cc(u8 *canvas, s32 radius, s32 angle, s32 color) __asm__("Func_080e7f94");
void Vector_AddPolarOffset(s32 distance, s32 angle, s32 *position);
/* VIEW(f0e82cc): routine EffectSprite_DrawAtWorldPosition as void EffectSprite_DrawAtWorldPosition(struct ScreenSprite *sprite, s32 *position); f0e4a34 has void EffectSprite_DrawAtWorldPosition(struct EffectSprite *sprite, struct GlowParticle *particle). */
extern void EffectSprite_DrawAtWorldPosition__f0e82cc(struct ScreenSprite *sprite,
                                      s32 *position) __asm__("EffectSprite_DrawAtWorldPosition");
u32 Random16(void);
void BattleEffect_PrepareBufferInterpolation(void);

void Func_080e82cc(void) {
  struct RingWork *work = HeapSlot_AllocEwram(92, 0x298c);
  struct FxScene__f0e82cc *scene = Ram_HeapSlots->effectWork;
  struct RingEvent *event = Ram_HeapSlots->eventWork;
  struct Actor *caster = scene->mainObject;
  struct CanvasSpriteWork *canvas;
  s32 entry;
  s32 tile;
  s32 spin;
  s32 angle;
  s32 repeat;
  s32 drawing;
  s32 *pos;
  struct EffectSprite *sprite;
  struct RingMote *mote;
  s32 position[3];
  s16 off; // Cycling is switched off through a halfword zero.
  s32 phase;
  s32 timer;
  s32 radius;
  s32 i;
  u8 *picture;

  spin = 0;
  angle = 0;
  repeat = 1;
  drawing = 0;
  if (event->up == 0) {
    BattleEffect_InitializeSharedScene();
    gCpuLoadDisplayEnabled = angle;
    Func_080eb824();
    canvas = Ram_HeapSlots->unkec[1];
    canvas->mode = angle;
    pos = position;
    pos[0] = scene->mainObject->x;
    pos[1] = scene->mainObject->y + 0x100000;
    pos[2] = scene->mainObject->z;
    Camera_WorldToScreen(pos);
    {
      s32 x = *((s16 *)pos + 1);
      s32 y;

      gBgScroll[0].x = 64 - x;
      y = *((s16 *)pos + 5);
      gBgScroll[0].y = 64 - y;
    }
    {
      s32 (*fill)(void *, s32, u32);

      // An empty once-only loop, and the fill routine's entry in a local, load
      // the entry ahead of the size's constant; as a direct Iwram_FillWords
      // call, 10 of 563 instructions differ in tla-en.
      do {
      } while (0);
      fill = Iwram_FillWords;
      fill((void *)0x06002000, 0x800, 0);
    }

    // The picture's release and allocation in a once-only block, which ends
    // scheduling at both sides; without it, 8 of 563 instructions differ in
    // tla-en.
    do {
      HeapSlot_Release(96);
      picture = HeapSlot_AllocEwram(96, 0x2000);
    } while (0);
    Iwram_FillWords(picture, 0x2000, 0);

    wait(1);
    Dma_Set(picture, (void *)0x06000000, (DMA_ENABLE | DMA_32BIT) << 16 | 0x800,
            REG_DMA3);
    {
      s32 control;

      control = 0x480;
      // An empty once-only loop keeps the register's address after the
      // control value; without it, 2 of 563 instructions differ in tla-en.
      do {
      } while (0);
      REG_BG0CNT = control;
    }
    Resource_DecodeType01(
        Resource_GetTableEntry((s32)&ResourceId_StarSparkleFade), work);
    entry = Resource_FindFreeEntry();
    tile = VramCache_Load(entry, 0x100, work);
    for (i = 0, mote = work->sparks; i < 64; i++, mote++) {
      sprite = &work->sparkSprites[i];
      Func_080eaf98(sprite, 4, 4, 0, tile);
      sprite->oam.palette256 = 1;
      sprite->oam.palette = 0;
      sprite->oam.priority = 0;
      sprite->depth = 240;
      mote->timer = -i / 4;
    }
    work->sparkling = 0;
    work->orbiting = 0;
    work->cycling = 1;
    work->phase = 0;
    event->up = 1;
    event->unkcad = 1;
    event->timer = 999;
    off = 0;
    Task_Add(Func_080e807c, 0x480);
    wait(1);
    work->cycling = off;
    phase = 0;
    timer = 0;
    do {
      if (drawing) {
        radius = spin;
        for (i = 0; i < 8; i++) {
          Func_080e7f94__f0e82cc(picture, 0x340000, radius, 243);
          Func_080e7f94__f0e82cc(picture, 0x350000, radius, 247);
          Func_080e7f94__f0e82cc(picture, 0x360000, radius, 251);
          Func_080e7f94__f0e82cc(picture, 0x370000, radius, 255);
          radius += 128;
        }
        canvas->command = CANVAS_UPLOAD_TO_BG;
      }
      for (i = 0, mote = work->sparks; i < 64; i++, mote++) {
        sprite = &work->sparkSprites[i];
        if (mote->timer == 0) {
          s32 heading = angle + ((i & 1) << 15);

          mote->x = caster->x;
          mote->y = caster->y + 0x100000;
          mote->z = caster->z;
          Vector_AddPolarOffset(0x370000, heading, &mote->x);
          Vector_AddPolarOffset(Random16() * 6, Random16(), &mote->x);
        }
        if ((u32)mote->timer <= 15) {
          sprite->oam.index = tile + (mote->timer & 3) * 2;
          EffectSprite_DrawAtWorldPosition__f0e82cc(sprite, mote);
          mote->y += -0x10000;
        }
        mote->timer++;
        if (repeat && mote->timer == 16)
          mote->timer = 0;
      }
      switch (phase) {
      case 0:
        angle = timer << 10;
        if (timer == 32) {
          phase = 1;
          timer = -1;
        }
        break;
      case 1:
        drawing = 1;
        {
          s32 turn = timer << 10;

          spin = turn;
          angle = turn;
        }
        if (timer == 32) {
          phase = 2;
          timer = -1;
        }
        break;
      case 2:
        repeat = 0;
        if (timer == 16)
          phase = 999;
        break;
      }
      wait(1);
      timer++;
    } while (phase != 999);

    Resource_DecodeType01(Resource_GetTableEntry((s32)&ResourceId_RingArcs),
                          picture);
    Dma_Set(picture, (void *)0x06000000, (DMA_ENABLE | DMA_32BIT) << 16 | 0x800,
            REG_DMA3);
    Resource_DecodeType01(Resource_GetTableEntry((s32)&ResourceId_Data1ED),
                          work);
    {
      s32 heading;
      s32 moteEntry = Resource_FindFreeEntry();
      s32 moteTile = VramCache_Load(moteEntry, 0x200, work);
      s32 *place;

      work->entry = moteEntry;
      work->tile = moteTile;
      work->sparkling = 1;
      work->orbiting = 1;
      for (i = 0, mote = work->motes; i < 32; i++) {
        heading = Random16();
        sprite = &work->sprites[i];
        Func_080eaf98(sprite, 8, 8, 0x40000000, moteTile);
        sprite->depth = 240;
        sprite->oam.priority = 0;
        sprite->oam.palette256 = 0;
        sprite->oam.palette = 15;
        sprite->angle = heading + 0x4000;
        place = position;
        place[0] = scene->mainObject->x;
        place[1] = scene->mainObject->y + 0x100000;
        place[2] = scene->mainObject->z;
        Camera_WorldToScreen(place);
        Vector_AddPolarOffset(0x400000, heading, place);
        mote->x = place[0];
        mote->y = place[2];
        mote->timer = -i;
        mote++;
      }
    }
    Resource_ResetEntry(entry);
    BattleEffect_PrepareBufferInterpolation();
  }
  work->cycling = 1;
  event->up = 1;
  event->unkcad = 1;
  event->timer = 600;
  Task_Add(Func_080e807c, 0x480);
}


// One mote: a position and a velocity (BattleEffect_IntegrateVector3's vector)
// and a frame count, negative before it starts.
struct BurstMote {
  s32 x;
  s32 y;
  s32 z;
  s32 vx;
  s32 vy;
  s32 vz;
  s32 timer;
};

/* VIEW(f0e82cc): tag struct BurstWork as struct BurstWork__f0e82cc; f0e2c38.c has another definition. */
struct BurstWork__f0e82cc {
  u8 unk0000[0x1000];
  struct EffectSprite sprites[64];
  struct BurstMote motes[64];
  u8 unk2100[0x880];
  s16 resource;
};

struct BurstEvent {
  u8 unk000[0xcac];
  s8 pending;
  u8 unkcad;
  u16 unkcae;
};

struct Actor *Actor_GetById(u32 objectId);
void Window_LoadGraphics(void);
/* VIEW(f0e82cc): routine BattleEffect_IntegrateVector3 as void BattleEffect_IntegrateVector3(s32 *vector, s32 damping, s32 gravity); psynergy_effects_11 has void BattleEffect_IntegrateVector3(struct EffectParticle *particle, s32 frames, s32 flags). */
extern void BattleEffect_IntegrateVector3__f0e82cc(s32 *vector, s32 damping, s32 gravity) __asm__("BattleEffect_IntegrateVector3");
extern char ResourceId_SparkleFade;

/// The burst of 64 sparkles that Func_080e807c's effect ends with: once the
/// event work asks for it, it plays sound 167, fills the tiles at 0x06002000,
/// reloads the window graphics, loads
/// the sparkle picture (FieldEffect_SparkleFade) and scatters the sparkles
/// around the current owner, starting a quarter of them a frame later than
/// the last, then moves and animates them for 40 frames before releasing the
/// effect.
void Func_080e87c4(void) {
  struct BurstWork__f0e82cc *work = Ram_HeapSlots->battleFxWork[0];
  struct BurstEvent *event = Ram_HeapSlots->eventWork;
  struct Actor *actor = Actor_GetById(gMainRoutineWork.currentOwner);
  struct BurstMote *mote;
  struct EffectSprite *sprite;
  s32 slot;
  s32 tile;
  s32 angle;
  s32 i;
  s32 frame;

  if (event->pending == 0)
    return;
  event->pending = 0;
  event->unkcad = 0;
  event->unkcae = 0;
  Task_Remove(Func_080e807c);
  wait(1);
  // A once-only block: its end holds the order around it (as plain
  // statements, 6 of 226 differ in tla-en).
  do {
    Sound_PlayFar(167);
    {
      s32 (*fill)(void *, s32, u32);
      // A once-only block: the fill's entry loads ahead of the size (as a
      // direct Iwram_FillWords call, 2 of 226 differ).
      do {
      } while (0);
      fill = Iwram_FillWords;
      fill((void *)0x06002000, 0x500, 0xf000f000);
    }
    Window_LoadGraphics();
  } while (0);
  {
    // A local that holds one constant: the control word is built with a
    // shift; stored as a constant it loads from the pool (60 of 226 differ).
    s32 control = 0x400;

    REG_BG0CNT = control;
  }
  gBgScroll[0].x = 0;
  gBgScroll[0].y = 0;
  gCpuLoadDisplayEnabled = 1;
  wait(1);
  Resource_DecodeType01(Resource_GetTableEntry((s32)&ResourceId_SparkleFade),
                        work);
  slot = Resource_FindFreeEntry();
  tile = VramCache_Load(slot, 0x100, work);

  i = 0;
  mote = work->motes;
  sprite = work->sprites;
  for (; i < 64; i++, sprite++, mote++) {
    angle = Random16();
    mote->x = actor->x;
    mote->y = actor->y + 0x100000;
    mote->z = actor->z;
    Vector_AddPolarOffset(Random16() * 4 + 0x340000, angle, &mote->x);
    mote->vx = 0;
    mote->vy = -0x40000;
    mote->vz = 0;
    Vector_AddPolarOffset(Random16() * 3 + 0x20000, angle, &mote->vx);
    mote->timer = -i / 4;
    Func_080eaf98(sprite, 4, 4, 0, tile);
    sprite->oam.palette256 = 1;
    sprite->oam.palette = 0;
    sprite->oam.priority = 0;
    sprite->depth = 240;
  }
  wait(1);

  for (frame = 0; frame < 40; frame++) {
    struct EffectSprite *spark;
    mote = work->motes;
    spark = work->sprites;
    for (i = 63; i >= 0; i--, spark++, mote++) {
      /* FAKEMATCH(barrier): an empty asm that opens the loop's body, which
       * keeps the index's 0xfffffc00 mask loaded inside the loop rather than
       * hoisted to r9; without it, 36 of 226 instructions differ in tla-en. */
      asm("");
      if (mote->timer >= 0) {
        spark->oam.index = tile + (mote->timer & 3) * 2;
        EffectSprite_DrawAtWorldPosition__f0e82cc(spark, mote);
        BattleEffect_IntegrateVector3__f0e82cc(&mote->x, 62, 0x8000);
      }
      mote->timer++;
    }
    wait(1);
  }
  Resource_ResetEntry(slot);
  Resource_ResetEntry(work->resource);
  BattleEffect_EndCanvasSprite();
  HeapSlot_Release(92);
}

#include "map.h"
#include "view_move.h"
#include "version.h"

// How far the view stays from the edge it is beside, in pixels.
#define VIEW_EDGE_MARGIN 12

// A map's view bounds, in 8-pixel units.
struct MapBounds {
  s32 minX;
  s32 maxX;
  s32 minZ;
  s32 maxZ;
};

void Func_08020328(s32 map, struct MapBounds *bounds);
void Map_SetScreenShakeFar(s32 shakeX, s32 shakeY, s32 decay);

// Opens the whole map to the view: keeps the view centre's position and the
// map's view bounds for Func_080e8c9c to restore, notes which edges the view
// is beside, and sets the bounds to the map's own. Where the view may not
// move (the view is held, or the field is in control mode 3), it shakes the
// screen along the asked direction instead.
void Func_080e89e4(s32 direction) {
  struct EventWork *event = Ram_HeapSlots->eventWork;
  struct Actor *center = event->viewCenter;
  struct MapWork *map = Ram_HeapSlots->mapWork;
  s32 viewX = map->viewX >> 16;
  s32 viewZ = map->viewZ >> 16;
  struct ViewMoveWork *work = Ram_HeapSlots->battleFxWork[0];
  struct MapBounds bounds;
  s32 minX;
  s32 maxX;
  s32 minZ;
  s32 maxZ;
  s32 left;
  s32 right;
  s32 top;
  s32 bottom;

  // Japanese does not check the map's rectangle list here.
#if TLA_PATCH_1
  if (*(s32 *)map->unk0fc != 0) {
    if (direction & VIEW_MOVE_X) {
      Map_SetScreenShakeFar(0x30000, 0, 0x10000);
      return;
    }
    if (direction & VIEW_MOVE_Z) {
      Map_SetScreenShakeFar(0, 0x30000, 0x10000);
      return;
    }
  }
#endif
  if (event->controlMode == 3) {
    if (direction & VIEW_MOVE_X) {
      Map_SetScreenShakeFar(0x30000, 0, 0x10000);
      return;
    }
    if (direction & VIEW_MOVE_Z) {
      Map_SetScreenShakeFar(0, 0x30000, 0x10000);
      return;
    }
  }

  /* FAKEMATCH(barrier): an empty asm before the bounds are fetched, the first
   * of three that lengthen viewX's and viewZ's lives so the allocator ranks
   * them below the bounds; without it, 33 of 157 instructions differ in
   * tla-ja. */
  __asm__("");
  /* FAKEMATCH(barrier): the second; without it, 33 of 157 instructions differ
   * in tla-ja. */
  __asm__("");
  /* FAKEMATCH(barrier): the third; without it, 33 of 157 instructions differ
   * in tla-ja. */
  __asm__("");
  Func_08020328(gMainRoutineWork.map, &bounds);
  minX = bounds.minX;
  maxX = bounds.maxX;
  minZ = bounds.minZ;
  maxZ = bounds.maxZ;
  left = minX * 8;
  right = maxX * 8;
  top = minZ * 8;
  bottom = maxZ * 8;
  work->x = center->x;
  work->y = center->y;
  work->z = center->z;

  work->edgeX = 0;
  if (viewX < left + VIEW_EDGE_MARGIN)
    work->edgeX = -1;
  if (viewX + 240 >= right - VIEW_EDGE_MARGIN)
    work->edgeX = 1;
  work->edgeZ = 0;
  if (viewZ < top + VIEW_EDGE_MARGIN)
    work->edgeZ = -1;
  if (viewZ + 160 >= bottom - VIEW_EDGE_MARGIN)
    work->edgeZ = 1;

  work->savedMinX = map->minX;
  work->savedMaxX = map->maxX;
  work->savedMinZ = map->minZ;
  work->savedMaxZ = map->maxZ;
  map->minX = minX << 19;
  map->maxX = maxX << 19;
  map->minZ = minZ << 19;
  map->maxZ = maxZ << 19;
  Actor_ResetMotion(center);
}


/* VIEW(f0e82cc): macro ABS as ABS__f0e82cc; another part or header has another definition. */
#define ABS__f0e82cc(x) ((x) >= 0 ? (x) : -(x))

/// Swings the view centre away from where Func_080e89e4 found it, along the
/// asked directions, by amplitude times the sine of the phase. The swing goes
/// into the room: away from the edge the view is beside, or forward when it is
/// beside neither. A held view does not move.
void Func_080e8b44(s32 direction, s32 phase, s32 amplitude) {
  struct EventWork *event = Ram_HeapSlots->eventWork;
  struct ViewMoveWork *work = Ram_HeapSlots->battleFxWork[0];
  struct Actor *center = event->viewCenter;
  s32 x;
  s32 z;
  s32 edge;

  // Japanese does not check the map's rectangle list here.
#if TLA_PATCH_1
  if (*(s32 *)Ram_HeapSlots->mapWork->unk0fc != 0)
    return;
#endif
  if (event->controlMode == 3)
    return;

  if (direction & VIEW_MOVE_X) {
    edge = work->edgeX;
    if (edge == -1)
      x = work->x + Iwram_MulQ16(ABS__f0e82cc(Trig_Sin(phase << 12)), amplitude);
    else if (edge == 1)
      x = work->x + Iwram_MulQ16(-ABS__f0e82cc(Trig_Sin(phase << 12)), amplitude);
    else
      x = work->x + Iwram_MulQ16(ABS__f0e82cc(Trig_Sin(phase << 12)), amplitude);
  } else {
    x = work->x;
  }

  if (direction & VIEW_MOVE_Z) {
    edge = work->edgeZ;
    if (edge == -1)
      z = work->z + Iwram_MulQ16(ABS__f0e82cc(Trig_Sin(phase << 12)), amplitude);
    else if (edge == 1)
      z = work->z + Iwram_MulQ16(-ABS__f0e82cc(Trig_Sin(phase << 12)), amplitude);
    else
      z = work->z + Iwram_MulQ16(ABS__f0e82cc(Trig_Sin(phase << 12)), amplitude);
  } else {
    z = work->z;
  }

  center->x = x;
  center->y = work->y;
  center->z = z;
}

/* VIEW(f0e82cc): tag struct EffectScene as struct EffectScene__f0e82cc; f0e2c38.c has another definition. */
struct EffectScene__f0e82cc {
  u8 unk00[0x18];
  s16 firstObjectId;
};

void Camera_FollowActor(s32 id, s32 mode);

// Ends a view move: gives the map back the view bounds Func_080e89e4 kept
// and lets the view follow the scene's first object again. Where the view
// was held, it only stops the screen shake.
void Func_080e8c9c(void) {
  struct ViewMoveWork *work = Ram_HeapSlots->battleFxWork[0];
  struct EffectScene__f0e82cc *scene = Ram_HeapSlots->effectWork;
  struct EventWork *event = Ram_HeapSlots->eventWork;
  struct MapWork *map = Ram_HeapSlots->mapWork;

#if TLA_PATCH_1
  if (*(s32 *)map->unk0fc != 0 || event->controlMode == 3) {
#else
  if (event->controlMode == 3) {
#endif
    Map_SetScreenShakeFar(0, 0, 0);
  } else {
    map->minX = work->savedMinX;
    map->maxX = work->savedMaxX;
    map->minZ = work->savedMinZ;
    map->maxZ = work->savedMaxZ;
    Camera_FollowActor(scene->firstObjectId, 1);
  }
}


struct FxGroup {
  u8 unk00[32];
  struct Actor *objects[8];
  u16 values[8];
  s32 count;
};

union FxSprite {
  u16 halves[10];
  u32 words[5];
};

s32 Func_080cdac0(s32 effect, s32 from, s32 to);

/// Gathers up to eight live objects, other than the scene's main object, for
/// which Func_080cdac0 measures a distance from the first object, with each
/// one's sprite value.
void ViewMove_SaveNearbySprites(void) {
  struct BattleFxScene *scene = Ram_HeapSlots->effectWork;
  struct EventWork *work = Ram_HeapSlots->eventWork;
  struct FxGroup *group = (struct FxGroup *)Ram_HeapSlots->battleFxWork[0];
  struct Actor **objects;
  struct Actor *obj;
  s32 i;

  group->count = 0;
  objects = work->objects;
  for (i = 0; i < 80; i++) {
    obj = *objects++;
    if (obj == NULL || obj->script == NULL || obj == scene->mainObject)
      continue;
    if (Func_080cdac0((s16)(scene->fieldKind | 0x100), scene->firstObjectId,
                      i) == -1)
      continue;
    group->objects[group->count] = obj;
    group->values[group->count] = ((union FxSprite *)obj->sprite)->halves[9];
    if (++group->count == 8)
      break;
  }
}

/// Writes each gathered object's saved sprite value back.
void ViewMove_RestoreNearbySprites(void) {
  struct FxGroup *group = (struct FxGroup *)Ram_HeapSlots->battleFxWork[0];
  s32 i;

  for (i = 0; i < group->count; i++)
    ((union FxSprite *)group->objects[i]->sprite)->halves[9] = group->values[i];
}
#undef VIEW_EDGE_MARGIN
#undef ABS__f0e82cc

// ---- games/tla/src/field/event_commands/f0e8db0.c ----
// Field Psynergy kind 19 on a chosen object (BattleEffect_DispatchRequestKind's
// case 19): Func_080e9090's sideways sway, stronger, with the object's
// picture held in the work.

#define SHAKE_SPRITE_COUNT 64
#define PICTURE_SIZE 0x1000
#define PICTURE_MAX_PIXELS 0x800

// The sway's amplitude (16.16 pixels) and how it ramps.
#define SWAY_START 0x20000
#define SWAY_PEAK 0xc0000
#define SWAY_RAMP 0x1999   // 0.1 pixels a frame
#define SWAY_SHAKE 0x40000 // sprites shake only past this
#define SWAY_HOLD_FRAMES 40

#define SE_SWAY 152

enum {
  SWAY_GROW,
  SWAY_HOLD,
  SWAY_SHRINK,
  SWAY_SETTLE,
  SWAY_DONE = 999,
};

struct ShakeParticle {
  s32 x;
  s32 y;
  s32 z;
  s32 velocityX;
  s32 velocityY;
  s32 velocityZ;
  s32 timer; // -1 when free
};

struct ShakeWork {
  struct ViewMoveWork view;
  u8 picture[PICTURE_SIZE];
  struct EffectSprite sprites[SHAKE_SPRITE_COUNT];
  struct ShakeParticle particles[SHAKE_SPRITE_COUNT];
};

// An object's sprite; the sway shakes it sideways by its offset.
struct ObjectSprite {
  u8 unk00[0x12];
  u16 offsetX;
  u8 width;
  u8 height;
  u8 unk16[3];
  u8 unk19;
  u8 unk1a[0xe];
  void *pictures;
};

/* VIEW(f0e8db0): tag struct EffectScene as struct EffectScene__f0e8db0; f0e2c38.c has another definition. */
struct EffectScene__f0e8db0 {
  u8 unk00[0x10];
  void *mainObject;
  struct Actor *target;
};

// The two 8x8 tiles of the shaken sprites (Resource_Data1EE): the row
// needs a name in directory.s before this adopts.
extern char ResourceId_ShakeTiles;

void BattleEffect_InitializeSharedScene(void);
/* VIEW(f0e8db0): routine BattleEffect_SetObjectUpdateMode as void BattleEffect_SetObjectUpdateMode(void *object, s32 mode); f0e4a34 has void BattleEffect_SetObjectUpdateMode(struct Actor *actor, s32 mode). */
extern void BattleEffect_SetObjectUpdateMode__f0e8db0(void *object, s32 mode) __asm__("BattleEffect_SetObjectUpdateMode");
void BattleEffect_PrepareBufferInterpolation(void);
void Animation_ApplyChildValueFar(void *sprite, s32 value);
/* VIEW(f0e8db0): routine Func_080e43a4 as void Func_080e43a4(void *source, void *buffer, s32 size); the definition has void Func_080e43a4(struct PictureSource *source, void *buffer, s32 size). */
extern void Func_080e43a4__f0e8db0(void *source, void *buffer, s32 size) __asm__("Func_080e43a4");
void wait(s32 frames);

// Sways the view from side to side around the chosen object, as kind 19 does
// with no object: the swing grows from 2 to 12 pixels, holds 40 frames,
// shrinks back to 2 and stops. Past 4 pixels the nearby objects' sprites
// shake with it. The work also takes the object's picture and 64 sprites of
// two tiles, which nothing here draws. An object whose picture is larger
// than 2,048 pixels gets no sway.
void Func_080e8db0(void) {
  struct ShakeWork *work = HeapSlot_Alloc(HEAP_SLOT_VIEW_MOVE, sizeof(*work));
  struct EffectScene__f0e8db0 *scene = Ram_HeapSlots->effectWork;
  struct EventWork *event = Ram_HeapSlots->eventWork;
  struct Actor *target = scene->target;
  struct ObjectSprite *targetSprite = target->sprite;
  s32 tiles;
  s32 shown;
  s32 width;
  s32 showing;
  s32 frameCount;
  s32 phase;
  s32 i;
  s32 tile;
  s32 amplitude;
  s32 state;
  s32 frames;
  struct ShakeParticle *particle;

  if (target != NULL && targetSprite != NULL &&
      targetSprite->width * targetSprite->height > PICTURE_MAX_PIXELS)
    goto release;

  BattleEffect_InitializeSharedScene();
  Resource_DecodeType01(Resource_GetTableEntry((s32)&ResourceId_ShakeTiles),
                        work->picture);
  tiles = Resource_FindFreeEntry();
  tile = VramCache_Load(tiles, 64, work->picture);
  for (i = 0, particle = work->particles; i < SHAKE_SPRITE_COUNT;
       i++, particle++) {
    struct EffectSprite *sprite = &work->sprites[i];

    Func_080eaf98(sprite, 4, 4, 0, tile + (i & 1));
    sprite->oam.palette256 = 0;
    sprite->oam.palette = 14;
    sprite->oam.priority = 0;
    sprite->depth = 240;
    particle->timer = -1;
  }
  BattleEffect_SetObjectUpdateMode__f0e8db0(scene->mainObject, 0);
  if (target != NULL && targetSprite != NULL) {
    Animation_ApplyChildValueFar(targetSprite, 0);
    Func_080e43a4__f0e8db0(targetSprite->pictures, work->picture,
                  targetSprite->width * targetSprite->height);
  }
  Func_080e89e4(VIEW_MOVE_X);
  ViewMove_SaveNearbySprites();
  wait(10);

  shown = -1;
  width = targetSprite->width;
  showing = 0;
  phase = 0;
  frameCount = 0;
  amplitude = SWAY_START;
  state = SWAY_GROW;
  frames = 0;
  do {
    if ((frameCount & 15) == 15)
      Sound_PlayFar(SE_SWAY);

    // Shake the gathered objects' sprites with the view.
    if (event->controlMode == 2) {
      for (i = 0; i < work->view.count; i++) {
        struct ObjectSprite *sprite = work->view.objects[i]->sprite;
        s32 offset = work->view.spriteOffsets[i];

        if (amplitude > SWAY_SHAKE)
          sprite->offsetX =
              offset + Iwram_MulQ16(Trig_Sin(phase * 3 << 11),
                                    (amplitude - SWAY_SHAKE) / 128);
        else
          sprite->offsetX = offset;
      }
    }

    Func_080e8b44(VIEW_MOVE_X, phase++, amplitude);
    switch (state) {
    case SWAY_GROW:
      if (frames == 40)
        showing = 1;
      amplitude += SWAY_RAMP;
      if (amplitude >= SWAY_PEAK) {
        state = SWAY_HOLD;
        frames = -1;
      }
      break;
    case SWAY_HOLD:
      if (frames == SWAY_HOLD_FRAMES) {
        state = SWAY_SHRINK;
        frames = -1;
      }
      break;
    case SWAY_SHRINK:
      if (frames == 50)
        showing = -1;
      amplitude -= SWAY_RAMP;
      if (amplitude <= SWAY_START) {
        state = SWAY_SETTLE;
        frames = -1;
      }
      break;
    case SWAY_SETTLE:
      amplitude = 0;
      if (frames == 0)
        state = SWAY_DONE;
      break;
    }

    // A count up to the picture's width and back that nothing reads.
    if (showing == 1 && shown <= width)
      shown++;
    if (showing == -1 && shown >= 0)
      shown -= 2;

    wait(1);
    frames++;
    frameCount++;
  } while (state != SWAY_DONE);

  if (target != NULL && targetSprite != NULL) {
    Animation_ApplyChildValueFar(targetSprite, 16);
    ((struct ObjectSprite *)target->sprite)->unk19 = 1;
  }
  Func_080e8c9c();
  ViewMove_RestoreNearbySprites();
  Resource_ResetEntry(tiles);
  BattleEffect_PrepareBufferInterpolation();
release:
  HeapSlot_Release(HEAP_SLOT_VIEW_MOVE);
}

#undef SWAY_PEAK
#undef SWAY_RAMP
#undef SWAY_HOLD_FRAMES

// Field Psynergy kind 19 (BattleEffect_Run's case 19).

#define VIEW_MOVE_WORK_SIZE 0x2154

// The sway's amplitude (16.16 pixels) and how it ramps.
#define SWAY_PEAK 0x60000
#define SWAY_RAMP 0x2666 // 0.15 pixels a frame
#define SWAY_HOLD_FRAMES 10
#define SWAY_SHAKE_FRAMES 50 // the hold while objects shake

// Sways the view from side to side, with a sound every 16 frames: the swing
// grows from 2 to 6 pixels, holds, shrinks back to 2 and stops. While the
// field is in control mode 2 the nearby objects' sprites shake with it; when
// there are any, the hold lasts 50 frames instead of 10. Then the view bounds
// and the sprites come back.
void Func_080e9090(void) {
  struct ViewMoveWork *work =
      HeapSlot_Alloc(HEAP_SLOT_VIEW_MOVE, VIEW_MOVE_WORK_SIZE);
  struct EffectScene__f0e8db0 *scene = Ram_HeapSlots->effectWork;
  struct EventWork *event = Ram_HeapSlots->eventWork;
  s32 frameCount;
  s32 phase;
  s32 i;
  s32 amplitude;
  s32 state;
  s32 frames;

  BattleEffect_InitializeSharedScene();
  BattleEffect_SetObjectUpdateMode__f0e8db0(scene->mainObject, 0);
  Func_080e89e4(VIEW_MOVE_X);
  ViewMove_SaveNearbySprites();
  wait(10);

  phase = 0;
  frameCount = 0;
  amplitude = SWAY_START;
  state = SWAY_GROW;
  frames = 0;
  do {
    if ((frameCount & 15) == 15)
      Sound_PlayFar(SE_SWAY);

    // Shake the gathered objects' sprites with the view.
    if (event->controlMode == 2) {
      for (i = 0; i < work->count; i++) {
        struct ObjectSprite *sprite = work->objects[i]->sprite;
        s32 offset = work->spriteOffsets[i];

        if (amplitude > 0)
          sprite->offsetX =
              offset + Iwram_MulQ16(Trig_Sin(phase * 3 << 11), amplitude / 384);
        else
          sprite->offsetX = offset;
      }
    }

    Func_080e8b44(VIEW_MOVE_X, phase++, amplitude);
    switch (state) {
    case SWAY_GROW:
      amplitude += SWAY_RAMP;
      if (amplitude >= SWAY_PEAK) {
        state = SWAY_HOLD;
        frames = -1;
      }
      break;
    case SWAY_HOLD:
      if (work->count != 0) {
        if (frames == SWAY_SHAKE_FRAMES) {
          state = SWAY_SHRINK;
          frames = -1;
        }
      } else if (frames == SWAY_HOLD_FRAMES) {
        state = SWAY_SHRINK;
        frames = -1;
      }
      break;
    case SWAY_SHRINK:
      amplitude -= SWAY_RAMP;
      if (amplitude <= SWAY_START) {
        state = SWAY_SETTLE;
        frames = -1;
      }
      break;
    case SWAY_SETTLE:
      amplitude = 0;
      if (frames == 0)
        state = SWAY_DONE;
      break;
    }
    wait(1);
    frames++;
    frameCount++;
  } while (state != SWAY_DONE);

  Func_080e8c9c();
  ViewMove_RestoreNearbySprites();
  BattleEffect_PrepareBufferInterpolation();
  HeapSlot_Release(HEAP_SLOT_VIEW_MOVE);
}


/// Moves rows 1 to rows - 2 of a buffer down one row by DMA, dropping the
/// last, then clears row 0 (stride bytes per row).
void BattleEffect_ShiftRowsDown(u8 *base, s32 stride, s32 rows) {
  u8 *source = base + (rows - 2) * stride;
  u8 *destination = base + (rows - 1) * stride;
  s32 i;

  for (i = rows - 1; i != 1; i--) {
    Dma_Set(source, destination, DMA_COPY_WORDS(stride / 4), REG_DMA3);
    source -= stride;
    destination -= stride;
  }
  Iwram_FillWords(base, stride, 0);
}
#undef SHAKE_SPRITE_COUNT
#undef PICTURE_SIZE
#undef PICTURE_MAX_PIXELS
#undef SWAY_START
#undef SWAY_PEAK
#undef SWAY_RAMP
#undef SWAY_SHAKE
#undef SWAY_HOLD_FRAMES
#undef SE_SWAY
#undef VIEW_MOVE_WORK_SIZE
#undef SWAY_SHAKE_FRAMES

// ---- games/tla/src/field/event_commands/f0e92b4.c ----
struct SplashParticle {
  s32 x;
  s32 y;
  s32 z;
  s32 velocityX;
  s32 velocityY;
  s32 velocityZ;
  s32 timer;
};

struct SplashWork {
  u8 unk0000[0x20];
  u8 picture[0x800];
  struct SplashParticle particles[64];
  struct EffectSprite sprites[64];
};

struct SplashAnimation {
  u8 unk00[16];
  u8 entry;
  u8 unk11[3];
  u8 width;
  u8 height;
  u8 unk16[18];
  void *tiles;
};

/* VIEW(f0e92b4): tag struct FxScene as struct FxScene__f0e92b4; f0e2c38.c has another definition. */
struct FxScene__f0e92b4 {
  u16 facing;
  u16 angle;
  s32 x;
  s32 y;
  s32 z;
  struct Actor *mainObject;
  struct Actor *child;
  s16 firstObjectId;
  s16 secondObjectId;
  u16 action;
  s16 fieldKind;
  s8 enabled;
};

/* VIEW(f0e92b4): tag struct PendingEvent as struct PendingEvent__f0e92b4; event_work.h has another definition. */
struct PendingEvent__f0e92b4 {
  u8 unk00[8];
  void (*callback)(s32, s32, s32);
};

extern char ResourceId_WaterSplash;

void *HeapSlot_Alloc(s32 slot, s32 size);
void HeapSlot_Release(s32 slot);
s32 Func_080ce31c(s32 kind);
/* VIEW(f0e92b4): routine Func_080ce458 as struct PendingEvent__f0e92b4 *Func_080ce458(u32 kind, s32 entry, s32 arg); f0e5d5c has void *Func_080ce458(u32 kind, s32 entry, s32 arg). */
extern struct PendingEvent__f0e92b4 *Func_080ce458__f0e92b4(u32 kind, s32 entry, s32 arg) __asm__("Func_080ce458");
void Vector_AddPolarOffset(s32 distance, s32 angle, s32 *position);
s32 Func_08020330(struct Actor *object);
void BattleEffect_InitializeSharedScene(void);
void *Resource_GetTableEntry(s32 resourceId);
s32 Resource_DecodeType01(const void *source, void *destination);
s32 Resource_FindFreeEntry(void);
s32 VramCache_Load(u32 slot, u32 size, const void *source);
s32 Resource_ResetEntry(u32 slot);
void Animation_ApplyChildValueFar(void *sprite, s32 value);
/* VIEW(f0e92b4): routine Func_080e43a4 as void Func_080e43a4(void *source, void *buffer, s32 size); the definition has void Func_080e43a4(struct PictureSource *source, void *buffer, s32 size). */
extern void Func_080e43a4__f0e92b4(void *source, void *buffer, s32 size) __asm__("Func_080e43a4");
void Func_080e89e4(s32 direction);
void Func_080e8b44(s32 direction, s32 phase, s32 amplitude);
void Func_080e8c9c(void);
void wait(s32 frames);
u32 Random16(void);
s32 BattleEffect_GetObjectPriority(struct Actor *object);
s32 BattleEffect_GetObjectDepth(struct Actor *object);
/* VIEW(f0e92b4): routine EffectSprite_DrawAtWorldPosition as void EffectSprite_DrawAtWorldPosition(struct EffectSprite *sprite, struct SplashParticle *particle); f0e4a34 has void EffectSprite_DrawAtWorldPosition(struct EffectSprite *sprite, struct GlowParticle *particle). */
extern void EffectSprite_DrawAtWorldPosition__f0e92b4(struct EffectSprite *sprite,
                                      struct SplashParticle *particle) __asm__("EffectSprite_DrawAtWorldPosition");
void BattleEffect_ShiftRowsDown(u8 *base, s32 stride, s32 rows);
void BattleEffect_PrepareBufferInterpolation(void);

void Func_080e92b4(void) {
  struct SplashWork *work = HeapSlot_Alloc(92, 0x1920);
  struct FxScene__f0e92b4 *scene = Ram_HeapSlots->effectWork;
  struct Actor *caster = scene->mainObject;
  struct Actor *child = scene->child;
  struct SplashAnimation *animation = child->sprite;
  struct EffectSprite *sprite;
  struct SplashParticle *particle;
  void *source;
  u8 *dst;
  s32 angle;
  s32 i;
  s32 entry;
  s32 tile;
  s32 phase;
  s32 timer;
  s32 shifts;
  s32 animating;
  s32 spraying;
  s32 next;
  s32 isWaterObject;
  s32 mode;
  s32 radius;
  s32 selector;
  struct PendingEvent__f0e92b4 *pending;
  u8 *picture;
  s32 counter;
  s32 amplitude;

  shifts = 0;
  radius = 0xe0000;
  animating = 0;
  spraying = 0;
  next = 0;
  mode = 0;
  selector = Func_080ce31c(scene->fieldKind);
  pending = Func_080ce458__f0e92b4(0x50000005, scene->fieldKind, selector);
  if (child != NULL) {
    if (animation != NULL) {
      if (animation->width * animation->height <= 0x800) {
      } else
        goto end;
    }
  } else {
    scene->x = caster->x;
    scene->z = caster->z + 0x80000;
    scene->y = caster->y;
    Vector_AddPolarOffset(0x200000, scene->facing, &scene->x);
  }
  isWaterObject = 0;
  if (Func_08020330(child) == 424) {
    isWaterObject = 1;
    mode = 1;
  }
  if (child == NULL)
    mode = 2;
  if (mode == 1)
    radius = 0xe0000;
  if (mode == 2)
    radius = 0x150000;

  BattleEffect_InitializeSharedScene();
  source = Resource_GetTableEntry((s32)&ResourceId_WaterSplash);
  picture = work->picture;
  Resource_DecodeType01(source, picture);
  entry = Resource_FindFreeEntry();
  tile = VramCache_Load(entry, 0x400, picture);
  particle = work->particles;
  for (i = 0; i < 64; i++) {
    sprite = &work->sprites[i];
    Func_080eaf98(sprite, 8, 8, 0x40000000, tile);
    sprite->oam.palette256 = 1;
    sprite->oam.palette = 0;
    sprite->oam.priority = 0;
    sprite->depth = 240;
    particle->timer = -1;
    particle++;
  }
  if (child != NULL && animation != NULL && isWaterObject == 1) {
    Animation_ApplyChildValueFar(animation, 0);
    Func_080e43a4__f0e92b4(animation->tiles, picture,
                  animation->width * animation->height);
  }
  Func_080e89e4(2);
  wait(1);
  Sound_PlayFar(238);
  counter = 0;
  amplitude = 0x20000;
  phase = 0;
  timer = 0;
  do {
    if (spraying) {
      if (mode == 1) {
        if (radius > 0x50000)
          radius += -0xa3d;
      } else if (radius > 0x50000) {
        radius += -0x147a;
      }
    }
    for (i = 0; i < spraying; i++) {
      angle = Random16();
      if (mode == 2)
        angle = ((Random16() * 10) >> 4) - 0x1000;
      sprite = &work->sprites[next];
      particle = &work->particles[next];
      if (child != NULL) {
        sprite->oam.priority = BattleEffect_GetObjectPriority(child);
        if (angle <= 0x7fff)
          sprite->depth = BattleEffect_GetObjectDepth(child) + 15;
        else
          sprite->depth = BattleEffect_GetObjectDepth(child) - 15;
      }
      particle->timer = 0;
      particle->x = scene->x;
      particle->y = scene->y + 0x50000;
      particle->z = scene->z;
      if (radius > 0x50000)
        Vector_AddPolarOffset(radius + Random16() * 5, angle, &particle->x);
      else
        Vector_AddPolarOffset(radius - (Random16() << 2), angle, &particle->x);
      next++;
      next %= 64;
    }
    particle = work->particles;
    for (i = 0; i < 64; i++, particle++) {
      sprite = &work->sprites[i];
      if ((u32)particle->timer <= 23) {
        if (particle->timer == 0)
          sprite->oam.index = tile;
        if (particle->timer == 18)
          sprite->oam.index = tile + 8;
        if (particle->timer == 20)
          sprite->oam.index = tile + 16;
        if (particle->timer == 22)
          sprite->oam.index = tile + 24;
        EffectSprite_DrawAtWorldPosition__f0e92b4(sprite, particle);
        particle->timer++;
      }
    }
    if (child != NULL && animation != NULL && animating) {
      s32 size = animation->width * animation->height;
      dst =
          (u8 *)0x06010000 + VramCache_Load(animation->entry, size, NULL) * 32;
      if (shifts % 3 == 0)
        BattleEffect_ShiftRowsDown(picture, animation->width,
                                   animation->height);
      shifts++;
      ((void (*)(void *, s32, s32, void *))Ram_HeapSlots->unk4c[2])(
          picture, animation->width, animation->height, dst);
    }
    Func_080e8b44(2, counter++, amplitude);
    switch (phase) {
    case 0:
      if (timer == 40 && mode != 0)
        spraying = 1;
      amplitude += 0x1999;
      if (amplitude > 0xbffff) {
        phase++;
        timer = -1;
      }
      break;
    case 1:
      if (isWaterObject == 0) {
        if (timer == 50) {
          phase = 2;
          timer = -1;
        }
        if (pending != NULL)
          pending->callback(0, timer, selector);
      } else {
        if (timer == animation->height * 3) {
          phase = 2;
          timer = -1;
        }
        animating = 1;
      }
      break;
    case 2:
      if (isWaterObject == 1)
        animating = 0;
      if (timer == 5 && mode != 0)
        spraying = 0;
      amplitude += -0x2666;
      if (amplitude <= 0x20000) {
        phase++;
        timer = -1;
      }
      break;
    case 3:
      amplitude = 0;
      if (timer == 0)
        phase = 999;
      break;
    }
    wait(1);
    timer++;
  } while (phase != 999);
  Sound_PlayFar(390);
  Func_080e8c9c();
  BattleEffect_PrepareBufferInterpolation();
  Resource_ResetEntry(entry);
end:
  HeapSlot_Release(92);
}


// Field Psynergy kind 27 (BattleEffect_Run's case 27): the view sways.

#define VIEW_MOVE_WORK_SIZE 0x1920

/// The sway's amplitude (16.16 pixels) and how it ramps.
#define SWAY_START 0x20000
#define SWAY_PEAK 0x60000
#define SWAY_RAMP 0x2666 ///< 0.15 pixels a frame
#define SWAY_HOLD_FRAMES 10

/* VIEW(f0e92b4): enumerator SWAY_GROW as SWAY_GROW__f0e92b4; f0e8db0 has enum { SWAY_GROW, SWAY_HOLD, SWAY_SHRINK, SWAY_SETTLE, SWAY_DONE = 999, }. */
/* VIEW(f0e92b4): enumerator SWAY_HOLD as SWAY_HOLD__f0e92b4; f0e8db0 has enum { SWAY_GROW, SWAY_HOLD, SWAY_SHRINK, SWAY_SETTLE, SWAY_DONE = 999, }. */
/* VIEW(f0e92b4): enumerator SWAY_SHRINK as SWAY_SHRINK__f0e92b4; f0e8db0 has enum { SWAY_GROW, SWAY_HOLD, SWAY_SHRINK, SWAY_SETTLE, SWAY_DONE = 999, }. */
/* VIEW(f0e92b4): enumerator SWAY_SETTLE as SWAY_SETTLE__f0e92b4; f0e8db0 has enum { SWAY_GROW, SWAY_HOLD, SWAY_SHRINK, SWAY_SETTLE, SWAY_DONE = 999, }. */
/* VIEW(f0e92b4): enumerator SWAY_DONE as SWAY_DONE__f0e92b4; f0e8db0 has enum { SWAY_GROW, SWAY_HOLD, SWAY_SHRINK, SWAY_SETTLE, SWAY_DONE = 999, }. */
enum {
  SWAY_GROW__f0e92b4,
  SWAY_HOLD__f0e92b4,
  SWAY_SHRINK__f0e92b4,
  SWAY_SETTLE__f0e92b4,
  SWAY_DONE__f0e92b4 = 999,
};

/* VIEW(f0e92b4): tag struct EffectScene as struct EffectScene__f0e92b4; f0e2c38.c has another definition. */
struct EffectScene__f0e92b4 {
  u8 unk00[0x10];
  void *mainObject;
};

/* VIEW(f0e92b4): routine BattleEffect_SetObjectUpdateMode as void BattleEffect_SetObjectUpdateMode(void *object, s32 mode); f0e4a34 has void BattleEffect_SetObjectUpdateMode(struct Actor *actor, s32 mode). */
extern void BattleEffect_SetObjectUpdateMode__f0e92b4(void *object, s32 mode) __asm__("BattleEffect_SetObjectUpdateMode");

/// Sways the view up and down the screen: the swing grows from 2 to 6 pixels,
/// holds for 10 frames, shrinks back to 2 and stops; then the map's view
/// bounds come back.
void BattleEffect_RunViewSway(void) {
  struct EffectScene__f0e92b4 *scene;
  s32 phase;
  s32 amplitude;
  s32 state;
  s32 frames;

  HeapSlot_Alloc(HEAP_SLOT_VIEW_MOVE, VIEW_MOVE_WORK_SIZE);
  scene = Ram_HeapSlots->effectWork;
  BattleEffect_InitializeSharedScene();
  BattleEffect_SetObjectUpdateMode__f0e92b4(scene->mainObject, 0);
  Func_080e89e4(VIEW_MOVE_Z);
  wait(10);

  phase = 0;
  amplitude = SWAY_START;
  state = SWAY_GROW__f0e92b4;
  frames = 0;
  do {
    Func_080e8b44(VIEW_MOVE_Z, phase++, amplitude);
    switch (state) {
    case SWAY_GROW__f0e92b4:
      amplitude += SWAY_RAMP;
      if (amplitude >= SWAY_PEAK) {
        state = SWAY_HOLD__f0e92b4;
        frames = -1;
      }
      break;
    case SWAY_HOLD__f0e92b4:
      if (frames == SWAY_HOLD_FRAMES) {
        state = SWAY_SHRINK__f0e92b4;
        frames = -1;
      }
      break;
    case SWAY_SHRINK__f0e92b4:
      amplitude -= SWAY_RAMP;
      if (amplitude <= SWAY_START) {
        state = SWAY_SETTLE__f0e92b4;
        frames = -1;
      }
      break;
    case SWAY_SETTLE__f0e92b4:
      amplitude = 0;
      if (frames == 0)
        state = SWAY_DONE__f0e92b4;
      break;
    }
    wait(1);
    frames++;
  } while (state != SWAY_DONE__f0e92b4);

  Func_080e8c9c();
  BattleEffect_PrepareBufferInterpolation();
  HeapSlot_Release(HEAP_SLOT_VIEW_MOVE);
}

/// A cycle of 96 colour levels (31 down to 0, 32 at 0, then 0 up to 31),
/// read a third of a turn apart for red, green and blue.
extern const u8 gData_080f0fe0[];

/// The colour of field Psynergy kind 24's glow (Func_080e9940) for a step of
/// its 24-step cycle and a brightness level (7 is neutral): three channels of
/// the wave, each raised or lowered by the level and held to 0..31, packed as
/// RGB555.
// ⚓️'s form of ☀️'s GraphicsPalette_SetTintChannelsFromCounters.
s32 Func_080e989c(s32 counter, s32 level) {
  s16 phase;
  s16 bias;
  s16 red;
  s16 green;
  s16 blue;
  s16 redLevel;
  s32 step;
  const u8 *wave;

  step = (counter + 12) % 24;
  wave = gData_080f0fe0;
  phase = step * 4;
  redLevel = wave[(s16)(phase % 96)];
  bias = level - 7;
  red = redLevel + bias;
  green = wave[(phase + 32) % 96] + bias;
  blue = wave[(phase + 64) % 96] + bias;
  if (red < 0)
    red = 0;
  if (red > 31)
    red = 31;
  if (green < 0)
    green = 0;
  if (green > 31)
    green = 31;
  if (blue < 0)
    blue = 0;
  if (blue > 31)
    blue = 31;
  return (blue << 10) | (green << 5) | red;
}


#define OBJ_PALETTE_14 ((u16 *)0x050003c0)

// The glow's strength, in frames: full at 112, and it fades out a frame at a
// time once the player leaves the place that feeds it.
#define GLOW_FULL 112
#define GLOW_SPENT_CUE 0x2099

// Kind 24's work (battle_fx_work[0]).
/* VIEW(f0e92b4): tag struct GlowWork as struct GlowWork__f0e92b4; f0e2c38.c has another definition. */
struct GlowWork__f0e92b4 {
  u8 unk0000[0x1000];
  struct EffectSprite sprite;
  s32 scale;
  u8 unk102c[2];
  u16 tile;
  u8 unk1030[2];
  u16 phase;
  s16 fading;
};

struct GlowEvent {
  u8 unk000[0x166];
  u16 cue;
  u8 unk168[0xbdc];
  s16 glow;
  s16 glowHeld;
};

s32 Party_GetLeaderActor(void);
s32 Func_080dbcc0(s32 x, s32 z, s32 layer);
void Camera_WorldToScreen(s32 *position);

// Kind 24's frame callback: keeps the glow's strength full while the player
// stands where Func_080dbcc0 allows, and runs it down otherwise (once it is
// fading, the glow shrinks with it and the event takes its cue when it is
// spent). It cycles OBJ palette 14 through Func_080e989c's colours, flickers
// between two pictures every other frame, and draws the glow under the
// player, flattened to three quarters of its height.
void Func_080e9940(void) {
  struct GlowWork__f0e92b4 *work = Ram_HeapSlots->battleFxWork[0];
  struct Actor *player = ObjectTable_Get(Party_GetLeaderActor());
  struct GlowEvent *event = Ram_HeapSlots->eventWork;
  u16 *palette;
  s32 position[3];
  s32 level;
  s32 i;
  s32 scaleX;
  s32 scaleY;
  struct EffectSprite *sprite;

  // The glow's strength.
  if (Func_080dbcc0(player->x, player->z, player->terrainId) == 0) {
    if (event->glowHeld != 0)
      event->glow--;
  } else {
    event->glow = GLOW_FULL;
  }

  if (work->fading == 1) {
    level = event->glow;
    if (level > GLOW_FULL)
      level = GLOW_FULL;
    if (level < 0)
      level = 0;
    work->scale = (level << 15) / GLOW_FULL + 0x8000;
    if (level == 0)
      event->cue = GLOW_SPENT_CUE;
  }

  // Cycle the glow's colours; the last takes the 15th's level.
  palette = OBJ_PALETTE_14;
  for (i = 0; i < 16; i++) {
    // The colour's store in a once-only block, which ends the scheduler's
    // region there; as the plain store, 2 of 194 instructions differ in
    // tla-en.
    do {
      *palette++ = Func_080e989c((((s16)work->phase >> 2) + i / 2) % 24,
                                 i == 15 ? 14 : i);
    } while (0);
  }
  work->phase++;

  // Draw it under the player.
  scaleX = work->scale;
  scaleY = scaleX * 6 / 8;
  sprite = &work->sprite;
  sprite->oam.index = work->tile + ((gFrameCount >> 1) & 1) * 32;
  sprite->oam.priority = BattleEffect_GetObjectPriority(player);
  sprite->depth = BattleEffect_GetObjectDepth(player) - 1;
  position[0] = player->x;
  position[1] = player->y;
  position[2] = player->z;
  Camera_WorldToScreen(position);
  sprite->x = position[0];
  sprite->y = position[2];
  sprite->scaleX = scaleX;
  sprite->scaleY = scaleY;
  Func_080eb01c(sprite);
}


void Func_080e3d04(void);
void Func_080e4a34(void);
void Func_080e572c(void);
void Func_080e5d5c(void);
void Func_080e68d0(void);
void Func_080e7844(void);
void Func_080e9af4(void);
void Func_080e74d8(void);

/// Runs field Psynergy animation 24 for the dispatcher; the target is unused.
void BattleEffect_CallEffect24(void) { Func_080e9af4(); }
#undef VIEW_MOVE_WORK_SIZE
#undef SWAY_START
#undef SWAY_PEAK
#undef SWAY_RAMP
#undef SWAY_HOLD_FRAMES
#undef OBJ_PALETTE_14
#undef GLOW_FULL
#undef GLOW_SPENT_CUE

// ---- games/tla/src/field/event_commands/f0e9af4.c ----
// Kind 24's work (battle_fx_work[0]), as Func_080e9940's draft has it.
/* VIEW(f0e9af4): tag struct GlowWork as struct GlowWork__f0e9af4; f0e2c38.c has another definition. */
struct GlowWork__f0e9af4 {
  u8 unk0000[0x1000];
  struct EffectSprite sprite;
  s32 scale;
  s16 resource;
  u16 tile;
  u16 unk1030;
  u16 phase;
  s16 fading;
};

struct GlowScene {
  u8 unk00[0x10];
  struct Actor *mainObject;
  u8 unk14[4];
  s16 unk18;
  s16 unk1a;
  u8 unk1c[2];
  s16 kind;
};

/* VIEW(f0e9af4): tag struct GlowEvent as struct GlowEvent__f0e9af4; f0e2c38.c has another definition. */
struct GlowEvent__f0e9af4 {
  u8 unk000[0x14];
  struct Actor *objects[80];
  u8 unk154[0xbf0];
  s16 glow;
  s16 glowHeld;
};

extern char ResourceId_RingOrb;

void wait(s32 frames);
s32 Func_080dbcc0(s32 x, s32 z, s32 layer);
s32 Func_080dbe80(s32 *a, s32 radiusA, s32 *b, s32 radiusB);
/* VIEW(f0e9af4): routine BattleEffect_SetObjectUpdateMode as void BattleEffect_SetObjectUpdateMode(struct Actor *object, s32 mode); f0e92b4 has void BattleEffect_SetObjectUpdateMode(void *object, s32 mode). */
extern void BattleEffect_SetObjectUpdateMode__f0e9af4(struct Actor *object, s32 mode) __asm__("BattleEffect_SetObjectUpdateMode");
void Func_080e9940(void);

void Func_080e9af4(void) {
  struct GlowWork__f0e9af4 *work = HeapSlot_Alloc(92, 0x1038);
  struct GlowScene *scene = Ram_HeapSlots->effectWork;
  struct GlowEvent__f0e9af4 *event = Ram_HeapSlots->eventWork;
  struct Actor *player = scene->mainObject;
  struct PendingEvent *pending;
  struct EffectSprite *sprite;
  struct Actor *other;
  s32 position[3];
  s32 found;
  s32 slot;
  struct Actor **list;
  s32 tile;
  s32 i;
  s32 n;

  pending = Func_080ce458(0x50000005, scene->kind, Func_080ce31c(scene->kind));
  Resource_DecodeType01(Resource_GetTableEntry((s32)&ResourceId_RingOrb), work);
  slot = Resource_FindFreeEntry();
  tile = VramCache_Load(slot, 0x800, work);
  work->resource = slot;
  work->tile = tile;
  work->scale = 0;
  sprite = &work->sprite;
  Func_080eaf98(sprite, 15, 15, 0x80000000, (s16)work->tile);
  sprite->oam.palette256 = 1;
  sprite->oam.palette = 0;
  Actor_SetPositionAndResetMotionFar(player, player->x, player->y, player->z);
  BattleEffect_SetObjectUpdateMode__f0e9af4(player, 0);
  Actor_SetMode(player, 5);
  Sound_PlayFar(130);
  wait(1);
  work->unk1030 = 0;
  work->phase = 0;
  work->fading = 0;
  Task_Add(Func_080e9940, TASK_PASS_VBLANK);

  found = 0;
  if (Func_080dbcc0(player->x, player->z, player->terrainId) == 0) {
    found = 1;
  } else {
    for (n = 0; n < 80; n++) {
      other = event->objects[n];
      if (other != NULL && other->script != NULL &&
          (other->collisionFlags & 1) && other != scene->mainObject) {
        position[0] = other->x;
        position[1] = other->terrainHeight;
        position[2] = other->z;
        if (Func_080dbe80(&player->x, player->radius, &other->x,
                          other->radius - 2) >= 0) {
          found = 1;
          break;
        }
      }
    }
  }

  if (found) {
    wait(5);
    Sound_PlayFar(246);
    for (i = 0; i < 32; i++) {
      work->scale = i << 11;
      wait(1);
    }
    wait(10);
    if (pending != NULL)
      Field_RunPsynergyEvent(pending, scene->unk18, scene->unk1a);
    for (i = 31; i != 0; i--) {
      work->scale = i << 11;
      wait(1);
    }
    wait(5);
    Task_Remove(Func_080e9940);
    Resource_ResetEntry(work->resource);
    HeapSlot_Release(92);
  } else {
    player->motionFlags = 0;
    player->y = player->terrainHeight;
    work->scale = 0;
    event->glowHeld = 0;
    event->glow = 100;
    for (i = 0; i < 32; i++) {
      if (i == 10)
        Sound_PlayFar(246);
      player->y += 0x10000;
      work->scale = i << 11;
      wait(1);
    }
    player->motionFlags = 4;
    player->y = player->terrainHeight + 0x248000;
    player->gravity = 0x30000;
    gMainRoutineWork.renderMode = 10;
    work->fading = 1;
    event->glowHeld = 1;
    event->glow = 112;
  }
}

// Kind 24's work (battle_fx_work[0]).

struct Actor *Actor_GetById(u32 objectId);

void Func_080e9db4(void) {
  struct GlowWork__f0e9af4 *work = (struct GlowWork__f0e9af4 *)Ram_HeapSlots->battleFxWork[0];
  struct Actor *object = Actor_GetById(Party_GetLeaderActor());
  s32 i;

  Task_Remove(Func_080e9940);
  Actor_SetPositionAndResetMotionFar(object, object->x, object->y, object->z);
  object->motionFlags = 3;
  gMainRoutineWork.renderMode = 0;
  for (i = 0; i < 60; i++) {
    if (object->y <= object->terrainHeight)
      break;
    wait(1);
  }
  Sound_PlayFar(132);
  wait(1);
  for (i = 0; i < 20; i++) {
    if (object->y <= object->terrainHeight)
      break;
    wait(1);
  }
  wait(1);
  for (i = 0; i < 20; i++) {
    if (object->y <= object->terrainHeight)
      break;
    wait(1);
  }
  wait(1);
  object->bounce = 0;
  object->velocityY = 0;
  object->y = object->terrainHeight;
  object->gravity = 0x10000;
  Resource_ResetEntry(work->resource);
  HeapSlot_Release(92);
}

#include "field_event.h"

struct SpritePictures {
  u8 unk00[5];
  u8 picture;
};

struct AnimatedSprite {
  u8 unk00[0x12];
  s16 rotation;
  u8 unk14[5];
  u8 picture;
  u8 nextPicture;
  u8 unk1b[0xd];
  struct SpritePictures *pictures;
};

/// Steps the sprite through a four-picture cycle on the actor's timer, as
/// overlay 68c's Func_02001398 does, but leaves the next picture alone on the
/// fourth step.
void Func_080e9e8c(struct Actor *object) {
  struct AnimatedSprite *sprite = (struct AnimatedSprite *)object->sprite;
  struct SpritePictures *pictures = sprite->pictures;

  switch (object->timer & 3) {
  case 0:
    pictures->picture = 7;
    sprite->picture = 1;
    sprite->nextPicture = 2;
    break;
  case 1:
    sprite->picture = 1;
    sprite->nextPicture = 0;
    break;
  case 2:
    pictures->picture = 7;
    sprite->picture = 1;
    sprite->nextPicture = 0;
    break;
  case 3:
    pictures->picture = 0;
    sprite->picture = 1;
    break;
  }
  object->timer++;
}

struct Chase {
  s32 x;
  s32 z;
  s32 unk08;
  s32 targetX;
  s32 targetZ;
};

s32 ArcTan2(s32 y, s32 x);
s32 Trig_Sin(s32 angle);
s32 Trig_Cos(s32 angle);

/// Moves toward the target by speed, stopping on it on overshoot.
void Motion_StepTowardTarget(struct Chase *chase, s32 speed) {
  s32 dx = chase->targetX - chase->x;
  s32 dz = chase->targetZ - chase->z;
  u16 angle = ArcTan2(dz, dx);

  chase->x += Iwram_MulQ16(Trig_Cos(angle), speed);
  chase->z += Iwram_MulQ16(Trig_Sin(angle), speed);
  {
    s32 nextDx = chase->targetX - chase->x;
    s32 nextDz = chase->targetZ - chase->z;

    if ((nextDx ^ dx) & 0x80000000)
      chase->x = chase->targetX;
    if ((dz ^ nextDz) & 0x80000000)
      chase->z = chase->targetZ;
  }
}

// One mote: a position chasing its target (Motion_StepTowardTarget's struct
// Chase) and a frame count, negative before it starts.
struct Mote {
  s32 x;
  s32 z;
  s32 unk08;
  s32 targetX;
  s32 targetZ;
  s32 unk14;
  s32 timer;
};

struct MoteWork {
  u8 unk0000[0x800];
  struct Mote sparks[32];
  struct EffectSprite sparkSprites[32];
  struct Mote motes[48];
  struct EffectSprite moteSprites[48];
  s32 originX;
  s32 originZ;
  s32 angle;
  s32 radius;
  s32 spacing;
  s8 state;
  s8 moving;
  u16 sparkTile;
  s32 targetX;
  s32 targetZ;
};

void Vector_AddPolarOffset(s32 radius, s32 angle, s32 *position);
/* VIEW(f0e9af4): routine EffectSprite_DrawAtWorldPosition as void EffectSprite_DrawAtWorldPosition(struct EffectSprite *sprite, struct Mote *mote); f0e92b4 has void EffectSprite_DrawAtWorldPosition(struct EffectSprite *sprite, struct SplashParticle *particle). */
extern void EffectSprite_DrawAtWorldPosition__f0e9af4(struct EffectSprite *sprite,
                                      struct Mote *mote) __asm__("EffectSprite_DrawAtWorldPosition");

void Func_080e9f68(void) {
  struct MoteWork *work = Ram_HeapSlots->battleFxWork[0];
  s32 position[3];
  struct Mote *mote;
  struct EffectSprite *sprite;
  s32 i;
  u16 *tile;

  if (work->moving != 0) {
    position[0] = work->originX;
    position[2] = work->originZ;
    Vector_AddPolarOffset(work->radius, work->angle, position);
    mote = work->motes;
    mote->targetX = position[0];
    mote->targetZ = position[2];
    work->angle += 0x600;
    for (i = 0; i < 48; i++, mote++) {
      if (mote->timer >= 0) {
        if (i != 0) {
          mote->targetX = position[0];
          mote->targetZ = position[2];
        }
        position[0] = mote->x;
        position[2] = mote->z;
        Vector_AddPolarOffset(work->spacing, 0, position);
      }
      mote->timer++;
    }
  }

  mote = work->motes;
  sprite = work->moteSprites;
  for (i = 47; i >= 0; i--, sprite++, mote++) {
    if (work->state == 0) {
      if (mote->timer >= 0) {
        Motion_StepTowardTarget(mote, 0x40000);
        sprite->x = mote->x;
        sprite->y = mote->z;
        Func_080eb01c(sprite);
      }
      mote->timer++;
    } else if (work->state == 2) {
      Motion_StepTowardTarget(mote, 0x60000);
      sprite->x = mote->x;
      sprite->y = mote->z;
      Func_080eb01c(sprite);
    } else if (work->state == 3) {
      if (mote->x != work->targetX || mote->z != work->targetZ) {
        Motion_StepTowardTarget(mote, 0x60000);
        sprite->x = mote->x;
        sprite->y = mote->z;
        Func_080eb01c(sprite);
      }
    }
  }

  mote = work->sparks;
  tile = &work->sparkTile;
  sprite = work->sparkSprites;
  for (i = 31; i >= 0; i--, sprite++, mote++) {
    if ((u32)mote->timer < 20) {
      sprite->oam.index = *tile + (mote->timer / 5 & 7) * 8;
      EffectSprite_DrawAtWorldPosition__f0e9af4(sprite, mote);
      BattleEffect_IntegrateVector3(&mote->x, 63, -0x8000);
      mote->timer++;
    }
  }
}

// ---- games/tla/src/field/map/f0ea14c.c ----

/* VIEW(f0ea14c): tag struct MoteWork as struct MoteWork__f0ea14c; f0e2c38.c has another definition. */
struct MoteWork__f0ea14c {
  u8 tiles[0x800];
  struct Mote sparks[32];
  struct EffectSprite sparkSprites[32];
  struct Mote motes[48];
  struct EffectSprite moteSprites[48];
  s32 originX;
  s32 originZ;
  s32 angle;
  s32 radius;
  s32 spacing;
  s8 state;
  s8 moving;
  u16 sparkTile;
  s32 targetX;
  s32 targetZ;
};

extern const s32 gData_080f3954[];
extern const u8 Data_080f1040[];
extern const u8 gData_080f3614[];
extern char ResourceId_Runes, ResourceId_Pebbles;

void *HeapSlot_Alloc(s32 slot, s32 size);
void HeapSlot_Release(s32 slot);
s32 Party_GetLeaderActor(void);
void Field_StartColorTransition(s32 frames, s32 color);
void Camera_WorldToScreen(s32 *position);
void *Resource_GetTableEntry(s32 resourceId);
s32 Resource_DecodeType01(const void *source, void *destination);
s32 Resource_FindFreeEntry(void);
s32 VramCache_Load(u32 slot, u32 size, const void *source);
s32 Resource_ResetEntry(u32 slot);
u32 Random16(void);
void wait(s32 frames);
void Event_Wait(s32 frames);
void Actor_StartScriptFar(struct Actor *object, const void *script);
void Actor_ArmCallback(s32 id, s32 value, s32 flags);
void Vector_AddPolarOffset(s32 distance, s32 angle, s32 *position);
void BattleEffect_PrepareBufferInterpolation(void);
/* VIEW(f0ea14c): routine Func_080e9e8c as void Func_080e9e8c(void); the definition has void Func_080e9e8c(struct Actor *object). */
extern void Func_080e9e8c__f0ea14c(void) __asm__("Func_080e9e8c");
void Func_080e9f68(void);

void Func_080ea14c(s32 targetId) {
  struct MoteWork__f0ea14c *work = HeapSlot_Alloc(92, sizeof(struct MoteWork__f0ea14c));
  s32 ownerId = Party_GetLeaderActor();
  struct Actor *owner = ObjectTable_Get(ownerId);
  struct Actor *target = ObjectTable_Get(targetId);
  s32 state;
  s32 moteEntry;
  s32 sparkEntry;
  s32 ownerX;
  s32 ownerZ;
  s32 targetX;
  s32 targetZ;
  s32 sparks = 0;
  s32 nextSpark = 0;
  s32 pos[3];
  struct EffectSprite *sprite;
  struct Mote *mote;
  s32 moteTile;
  s32 sparkTile;
  s32 timer;
  s32 wave;
  s32 delay;
  s32 angle;
  s32 i;
  s32 startX = 130 * Q16_ONE;
  s32 startZ = 30 * Q16_ONE;

  Field_StartColorTransition(8, gData_080f3954[gFrameCount & 7]);
  pos[0] = owner->x;
  pos[1] = owner->y + 24 * Q16_ONE;
  pos[2] = owner->z;
  Camera_WorldToScreen(pos);
  ownerX = pos[0];
  ownerZ = pos[2];
  work->targetX = ownerX;
  work->targetZ = ownerZ;
  pos[0] = target->x;
  pos[1] = target->y;
  pos[2] = target->z;
  Camera_WorldToScreen(pos);
  targetX = pos[0];
  targetZ = pos[2];
  Resource_DecodeType01(Resource_GetTableEntry((s32)&ResourceId_Runes), work);
  moteEntry = Resource_FindFreeEntry();
  moteTile = VramCache_Load(moteEntry, 0x800, work);
  mote = work->motes;
  i = 0;
  sprite = work->moteSprites;
  for (; i < 48; i++) {
    Func_080eaf98(sprite, 4, 4, 0, moteTile + ((Random16() << 4) >> 16) * 2);
    sprite->oam.palette256 = 1;
    sprite->oam.palette = 0;
    sprite->oam.priority = 1;
    sprite->depth = 240;
    mote->x = targetX;
    mote->z = targetZ;
    mote->targetX = ((i % 16) * 3 << 18) + 30 * Q16_ONE;
    mote->targetZ = ((i / 16) * 3 << 19) + 30 * Q16_ONE;
    mote->timer = -i;
    sprite++;
    mote++;
  }
  Resource_DecodeType01(Resource_GetTableEntry((s32)&ResourceId_Pebbles), work);
  sparkEntry = Resource_FindFreeEntry();
  sparkTile = VramCache_Load(sparkEntry, 0x800, work);
  work->sparkTile = sparkTile;
  mote = work->sparks;
  sprite = work->sparkSprites;
  for (i = 31; i >= 0; i--) {
    Func_080eaf98(sprite, 8, 8, 0x40000000, sparkTile);
    sprite->oam.palette256 = 1;
    sprite->oam.palette = 0;
    sprite->oam.priority = 0;
    sprite->depth = 240;
    mote->timer = -1;
    sprite++;
    mote++;
  }
  wait(1);
  Sound_PlayFar(139);
  Actor_SetMode(target, 2);
  Event_Wait(60);
  target->timer = 0;
  target->update = Func_080e9e8c__f0ea14c;
  Sound_PlayFar(142);
  Event_Wait(80);
  Actor_StartScriptFar(target, Data_080f1040);
  work->moving = 0;
  Task_Add(Func_080e9f68, 0x480);
  state = 0;
  timer = 0;
  do {
    switch (state) {
    case 0:
      work->state = 0;
      if (timer <= 39) {
        if ((timer & 7) == 0)
          Sound_PlayFar(133);
        sparks = 1;
      }
      if (timer == 40) {
        Sound_PlayFar(134);
        sparks = 25;
        Actor_ResetMotion(target);
        target->scaleX = 0;
      }
      if (timer == 80) {
        state++;
        timer = -1;
      }
      break;
    case 1:
      for (i = 0; i < 48; i++) {
        struct EffectSprite *rune;
        wave = i % 16 + i / 16;
        wave = timer - wave;
        rune = &work->moteSprites[i];
        if (wave == 0)
          rune->oam.index += 32;
        if (wave == 3)
          rune->oam.index += -32;
        if (wave == 8)
          rune->oam.index += 32;
        if (wave == 10)
          rune->oam.index += -32;
        if (wave == 16)
          rune->oam.index += 32;
        if (wave == 18)
          rune->oam.index += -32;
      }
      if (timer == 0)
        Sound_PlayFar(246);
      if (timer == 56) {
        state++;
        timer = -1;
      }
      break;
    case 2:
      if (timer == 0) {
        Actor_ArmCallback(ownerId, 0x4000, 0);
        break;
      }
      if (owner->script[owner->step] == 17) {
        state++;
        timer = -1;
      }
      if (timer == 60) {
        state++;
        timer = -1;
      }
      break;
    case 3:
      if (timer == 0)
        Actor_SetMode(owner, 28);
      if (timer == 10) {
        state++;
        timer = -1;
      }
      break;
    case 4:
      if (timer == 0) {
        mote = work->motes;
        delay = 0;
        for (i = 47; i >= 0; i--) {
          mote->timer = delay;
          delay -= 3;
          mote++;
        }
        work->radius = 60 * Q16_ONE;
        work->angle = 0xc000;
        work->originX = startX;
        work->originZ = startZ;
        work->moving = 1;
        work->spacing = 4 * Q16_ONE;
        work->state = 2;
      }
      if (timer == 0)
        Sound_PlayFar(187);
      if (timer == 48)
        Sound_PlayFar(187);
      if (timer == 96)
        Sound_PlayFar(187);
      if (timer == 100) {
        state++;
        timer = -1;
      }
      break;
    case 5:
      work->originX = timer * (ownerX - startX) / 30 + startX;
      work->originZ = timer * (ownerZ - startZ) / 30 + startZ;
      if (timer == 30) {
        state++;
        timer = -1;
      }
      break;
    case 6:
      if (timer == 0)
        work->spacing = 0;
      work->radius += -2 * Q16_ONE;
      if (work->radius < 0) {
        work->radius = 0;
        state++;
        timer = -1;
      }
      break;
    case 7:
      if (timer == 0)
        Actor_StartScriptFar(owner, gData_080f3614);
      if ((timer & 3) == 0)
        Sound_PlayFar(167);
      work->state = 3;
      if (timer == 60)
        state = 999;
      break;
    }
    for (i = 0; i < sparks; i++) {
      angle = Random16();
      mote = &work->sparks[nextSpark];
      mote->timer = 0;
      mote->x = target->x;
      mote->z = target->y + 12 * Q16_ONE;
      mote->unk08 = target->z;
      Vector_AddPolarOffset(Random16() * 4 + 2 * Q16_ONE, angle, &mote->x);
      mote->targetX = 0;
      mote->targetZ = Q16_ONE * 9 / 2;
      mote->unk14 = 0;
      Vector_AddPolarOffset(Random16() * 2 + 2 * Q16_ONE, angle,
                            &work->sparks[nextSpark].targetX);
      nextSpark = (nextSpark + 1) & 31;
    }
    sparks = 0;
    wait(1);
    timer++;
  } while (state != 999);
  Actor_ResetMotion(owner);
  owner->scaleX = Q16_ONE;
  owner->scaleY = Q16_ONE;
  Event_Wait(10);
  Task_Remove(Func_080e9f68);
  BattleEffect_PrepareBufferInterpolation();
  Resource_ResetEntry(moteEntry);
  Resource_ResetEntry(sparkEntry);
  HeapSlot_Release(92);
}

#include "field/field.h"

struct ActorFlag {
  u16 actor;
  s16 flag;
};

struct Actor *Actor_GetById(u32 objectId);
s32 Func_080eab70(struct Actor *object);
s32 GameFlag_Test(s32 flag);
void GameFlag_ClearBit(s32 flag);
void Func_080eaa14(struct ActorFlag *list);

void Func_080ea7c0(struct ActorFlag *actors) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  struct ActorFlag *list = actors;
  struct Actor *leader;
  s32 height;

  for (; actors->actor != 0xffff; actors++) {
    struct Actor *object = Actor_GetById(actors->actor);
    u16 flag = actors->flag;
    s32 slot = Func_080eab70(object);
    struct MapCell *cell = &map->layers[object->terrainId].cells[slot];

    object->motionFlags = 0;
    object->collisionFlags &= ~1;
    Actor_SetSingleChildFlagsFar(object, 0);
    cell->collisionCode = 0xff;
    if (GameFlag_Test(flag)) {
      Actor_SetMode(object, 0);
      GameFlag_ClearBit(flag);
    } else {
      Actor_SetMode(object, 1);
    }
  }
  Func_080eaa14(list);
  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  height = Map_GetTerrainHeightFar(leader->terrainId, leader->x, leader->z);
  if (leader->y < height)
    leader->y = leader->terrainHeight = height;
}


struct MenuFlagWork {
  u8 unk000[0x506];
  u8 busy;
};


struct DescriptorEntry {
  s16 id;
  u8 unk02[6];
};

extern void *gOverlayArea[];
extern struct DescriptorEntry gData_080eff28[];

void BattleEffect_StartTiledCanvas(s32 mode);
void BattleEffect_EndCanvasLayer(void);
void Func_0814ea58(void *effect, s32 mode);
void PartyStatus_CreateWindow(s32 flags);
s32 Func_08020310(s32 x, s32 z, s32 layer);
s32 Func_080dbde8(s32 x, s32 z, s32 layer);
u8 Func_080dbdf4(s32 x, s32 z, s32 layer, s32 value);
void Func_08108148(void);
s32 Inn_RoomPrice(s32 innId);
void Func_0810824c(void);

void Func_080ea8a8(s32 x, s32 z, s32 layer) {
  if (Func_08020310(x, z, layer) == 0)
    Func_080dbdf4(x, z, layer, Func_080dbde8(x, z, layer) | 0x80);
}

// ---- games/tla/src/field/map/f0ea8d4.c ----

struct Actor *Actor_GetById(u32 objectId);
s32 Func_080eab70(struct Actor *object);
s32 GameFlag_Test(s32 flag);
void GameFlag_ClearBit(s32 flag);
void Func_080eaa14(struct ActorFlag *list);
/* VIEW(f0ea8d4): routine Map_GetTerrainHeightFar as s32 Map_GetTerrainHeightFar(s32 layer, s32 x, s32 z); field.h has s32 Map_GetTerrainHeightFar(u8, s32, s32). */
extern s32 Map_GetTerrainHeightFar__f0ea8d4(s32 layer, s32 x, s32 z) __asm__("Map_GetTerrainHeightFar");
s32 Party_GetLeaderActor(void);
/* VIEW(f0ea8d4): routine Func_080ea8a8 as void Func_080ea8a8(s32 x, s32 z, s32 layer, s32 height); the definition has void Func_080ea8a8(s32 x, s32 z, s32 layer). */
extern void Func_080ea8a8__f0ea8d4(s32 x, s32 z, s32 layer, s32 height) __asm__("Func_080ea8a8");
/* VIEW(f0ea8d4): routine Func_08020310 as s32 Func_08020310(s32 x, s32 z, s32 layer, s32 height); f0ea14c has s32 Func_08020310(s32 x, s32 z, s32 layer). */
extern s32 Func_08020310__f0ea8d4(s32 x, s32 z, s32 layer, s32 height) __asm__("Func_08020310");

void Func_080ea8d4(struct ActorFlag *list) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  struct ActorFlag *first = list;
  struct Actor *owner;

  for (; list->actor != 0xffff; list++) {
    struct Actor *object = Actor_GetById(list->actor);
    u16 flag = list->flag;
    s32 slot = Func_080eab70(object);
    struct MapCell *cell = &map->layers[object->terrainId].cells[slot];

    object->motionFlags = 0;
    object->collisionFlags &= ~1;
    Actor_SetSingleChildFlagsFar(object, 0);
    cell->collisionCode = 0xff;
    Func_080ea8a8__f0ea8d4(object->x + 0x100000, object->z, object->terrainId,
                  object->terrainHeight);
    Func_080ea8a8__f0ea8d4(object->x - 0x100000, object->z, object->terrainId,
                  object->terrainHeight);
    Func_080ea8a8__f0ea8d4(object->x, object->z + 0x100000, object->terrainId,
                  object->terrainHeight);
    Func_080ea8a8__f0ea8d4(object->x, object->z - 0x100000, object->terrainId,
                  object->terrainHeight);
    if (GameFlag_Test(flag)) {
      Actor_SetMode(object, 0);
      GameFlag_ClearBit(flag);
    } else {
      Actor_SetMode(object, 1);
    }
  }
  Func_080eaa14(first);
  owner = Actor_GetById(Party_GetLeaderActor());
  if (Func_08020310__f0ea8d4(owner->x, owner->z, owner->terrainId, owner->terrainHeight))
    owner->terrainHeight = owner->y =
        Map_GetTerrainHeightFar__f0ea8d4(owner->terrainId, owner->x, owner->z);
}

struct SwitchCell {
  u8 unk00[2];
  u8 block;
  u8 unk03_0 : 7;
  u8 raised : 1;
};

struct RestoreLayer {
  u8 unk00[0x138 - 0x108];
  struct SwitchCell *cells;
  u8 unk34[4];
};

struct RestoreMap {
  u8 unk000[0x108];
  struct RestoreLayer layers[1];
};

extern u8 gMapShapeGrid[];

void GameFlag_SetBit(s32 flag);
u32 Func_080202f0(s32 terrain, s32 x, s32 z);
s32 Func_080eab98(s32 unused, s32 key, s32 value);

void Func_080eaa14(struct ActorFlag *list) {
  u8 *event = Ram_HeapSlots->eventWork;
  u8 *grid = gMapShapeGrid;

  *(struct Actor **)(event + 0xcbc) = NULL;
  for (; list->actor != 0xffff; list++) {
    s32 change = 0;
    struct Actor *object = Actor_GetById(list->actor);
    u8 frame = ((u8 *)object->sprite)[24];
    u16 flag = list->flag;

    if (GameFlag_Test(flag)) {
      if (frame != 0)
        change = 1;
    } else {
      if (frame != 1)
        change = 1;
    }
    if (change) {
      struct SwitchCell *cells = ((struct RestoreMap *)Ram_HeapSlots->mapWork)
                                     ->layers[object->terrainId]
                                     .cells;
      u8 *shapes;
      s32 slot;
      u32 key;
      s32 height;

      shapes = &grid[((s32)cells - (s32)Ram_MapCellBuffer) >> 2];
      slot = Func_080eab70(object);
      key = Func_080202f0(object->terrainId, object->x, object->z);
      height =
          Map_GetTerrainHeightFar__f0ea8d4(object->terrainId, object->x, object->z) >>
          19;

      *(struct Actor **)(event + 0xcbc) = object;
      if (GameFlag_Test(flag)) {
        height -= 4;
        GameFlag_ClearBit(flag);
      } else {
        height += 4;
        GameFlag_SetBit(flag);
      }
      if ((object->x | object->z) != 0) {

        cells += slot;
        shapes += slot;
        *shapes = Func_080eab98(object->terrainId, key, height);
        if (GameFlag_Test(flag)) {
          s32 bits;

          cells->block = 0;
          // The bit set in int arithmetic, where the clear below is a bitfield.
          bits = ((u8 *)cells)[3] | 0x80;
          ((u8 *)cells)[3] = bits;
        } else {
          cells->block = 0xff;
          cells->raised = 0;
        }
      }
    }
  }
}

s32 Func_080eab70(struct Actor *object) {
  return object->x / 0x100000 + (object->z / 0x100000) * 128;
}

/// Finds the tag (key, value) in the 4-byte entries at gMapCollision, or adds
/// it at the first free entry (key 0xff), and returns its index. Func_080eabd0
/// gives a raised cell's shape this index.
s32 Func_080eab98(s32 unused, s32 key, s32 value) {
  u8 *entry = Ram_MapCollision;
  s8 tagValue;
  s32 i = 0;

  tagValue = value;
  while (1) {
    if (entry[0] == key) {
      if (*(s8 *)++entry == tagValue)
        return i;
      entry--;
    } else if (entry[0] == 0xff) {
      entry[0] = key;
      entry[1] = tagValue;
      return i;
    }
    entry += 4;
    i++;
  }
}
