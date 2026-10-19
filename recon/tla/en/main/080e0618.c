// Draft, differ 99 of 394 (2026-10-18): Func_080e0618, the frame callback
// Func_080e09c0 installs: it sprays particles round the effect's object and
// moves the object through five phases. Left: in the sprite loop the native
// holds the 3 of (timer & 3) in r11 and loads 0x3ff from the pool at its one
// use, where this holds 0x3ff and rebuilds 3 (the two tile stores are
// cross-jumped into one tail); one tile through a local ends at 115-118. The
// rest is register choice in the spawn loop and the phase switch.
#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "fx_scene.h"
#include "effect_sprite.h"

struct ScatterParticle {
  s32 x;
  s32 y;
  s32 z;
  s32 velocityX;
  s32 velocityY;
  s32 velocityZ;
  s32 timer;
};

struct ScatterWork {
  u8 tiles[0x1000];
  struct EffectSprite sprites[64];
  struct ScatterParticle particles[64];
  struct ObjectRuntime *object;
  s16 phase;
  s16 timer;
  s16 spawnCount;
  s16 next;
  u16 tileA;
  u16 tileB;
  s16 flash;
  s16 hit;
};

struct ScatterEvent {
  u8 unk000[0xd31];
  s8 target;
  s8 targetY;
  s8 targetZ;
};

struct ScatterVector {
  s32 x;
  s32 y;
  s32 z;
};

u32 Random16(void);
void Vector_AddPolarOffset(s32 magnitude, s32 angle, void *position);
void EffectSprite_DrawAtWorldPosition(struct EffectSprite *sprite, struct ScatterParticle *p);
void BattleFx_IntegrateVector3(struct ScatterParticle *particle, s32 frames,
                               s32 flags);
void Object_SetPositionAndResetMotionFar(struct ObjectRuntime *object, s32 x,
                                         s32 y, s32 z);
void Func_080d92a4(s32 target, struct ScatterVector *position);

void Func_080e0618(void) {
  struct BattleFxScene *scene = Ram_HeapSlots->effectWork;
  struct ScatterWork *work =
      (struct ScatterWork *)Ram_HeapSlots->battleFxWork[0];
  struct ScatterEvent *event = Ram_HeapSlots->eventWork;
  struct ObjectRuntime *object = work->object;
  struct ScatterVector position;
  struct EffectSprite *sprite;
  struct ScatterParticle *particle;
  s32 i;

  work->flash = 0;
  for (i = 0; i < work->spawnCount; i++) {
    s32 distance;
    s32 speed;

    particle = &work->particles[work->next];
    particle->x = object->x;
    particle->y = object->y;
    particle->z = object->z;
    distance = Random16() * 12 + 0x40000;
    Vector_AddPolarOffset(distance, Random16(), particle);
    particle->velocityX = 0;
    particle->velocityY = 0x20000 - Random16() * 2;
    particle->velocityZ = 0;
    speed = Random16() + 0x18000;
    Vector_AddPolarOffset(speed, Random16(), &particle->velocityX);
    particle->timer = 0;
    work->next = (work->next + 1) & 63;
  }

  particle = work->particles;
  sprite = work->sprites;
  for (i = 0; i <= 63; i++, sprite++, particle++) {
    s32 timer = particle->timer;

    if (timer >= 0) {
      if (i & 1)
        sprite->oam.index = work->tileA + (timer & 3) * 2;
      else
        sprite->oam.index = work->tileB + (timer & 3) * 2;
      EffectSprite_DrawAtWorldPosition(sprite, particle);
      BattleFx_IntegrateVector3(particle, 63, -0x1400);
      if (++particle->timer == 32)
        particle->timer = -1;
    }
  }

  switch (work->phase) {
  case 0: {
    s32 a = Random16();
    s32 b = Random16();
    s32 c, d;

    position.x = scene->x + a * 3 - b * 3;
    c = Random16();
    d = Random16();
    position.y = scene->y + c * 3 - d * 3;
    position.z = scene->z;
    Object_SetPositionAndResetMotionFar(object, position.x, position.y,
                                        position.z);
    if (work->timer == 20) {
      work->phase++;
      work->timer = -1;
    }
    break;
  }
  case 1:
    Object_SetPositionAndResetMotionFar(object, scene->x, scene->y, scene->z);
    if (work->timer == 6) {
      work->phase++;
      work->timer = -1;
    }
    break;
  case 2:
    if (work->timer == 0)
      work->flash = 1;
    Func_080d92a4(event->target, &position);
    object->x = position.x;
    object->y = position.y - 0x40000;
    object->z = position.z;
    if (work->hit != 0) {
      work->phase++;
      work->timer = -1;
    }
    if (work->timer == 90) {
      work->phase++;
      work->timer = -1;
    }
    break;
  case 3:
    Object_SetMode(object, 3);
    work->spawnCount = 3;
    object->y += work->timer << 17;
    if (work->timer == 10) {
      work->phase++;
      work->timer = -1;
    }
    break;
  case 4:
    object->scaleX = 0;
    work->spawnCount = 0;
    if (work->timer == 30)
      work->phase = 999;
    break;
  }
  work->timer++;
}
