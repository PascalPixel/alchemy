// Draft, differ 335 of 506: the frame callback of Func_080e3d04's spray: every
// fourth frame while it is emitting, a free one of sixteen drops starts at
// the origin with the mode's speed (mode 3 faster, longer and lighter) along
// the spray's angle; each live drop grows, cycles its four tiles, is drawn at
// its position, and dies after the mode's lifetime, on reaching an object of
// the event work's list (whose index is kept and which stops the spray for
// the frame) or on something solid; an odd drop that dies leaves one of
// sixteen puffs of smoke that rise for 32 frames. The angle then turns toward
// the scene's facing by at most 0x400 a frame.
// Same calls, offsets and control flow; left: register allocation (the
// native counts the drops in r11 and keeps the sprite pointer at sp 4 in the
// second loop) and the order it cascades into.
#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "effect_sprite.h"

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
  struct ObjectRuntime *mainObject;
  struct ObjectRuntime *otherObject;
};

struct SprayEvent {
  u8 unk00[20];
  struct ObjectRuntime *objects[80];
};

extern u32 gFrameCount;

s32 BattleEffect_GetObjectDepth(struct ObjectRuntime *object);
void EffectSprite_DrawAtWorldPosition(struct EffectSprite *sprite,
                                      struct SprayParticle *particle);
void Vector_AddPolarOffset(s32 distance, s32 angle, s32 *position);
void Camera_WorldToScreen(s32 *position);
s32 Func_08020210(struct ObjectRuntime *object, s32 *position);
s32 Func_080dbe80(s32 *a, s32 radiusA, s32 *b, s32 radiusB);
void BattleEffect_IntegrateVector3(s32 *vector, s32 damping, s32 gravity);
u32 Random16(void);

void Func_080e38c8(void) {
  struct SprayWork *work = (struct SprayWork *)Ram_HeapSlots->battleFxWork[0];
  struct SprayScene *scene = Ram_HeapSlots->effectWork;
  struct SprayEvent *event = Ram_HeapSlots->eventWork;
  struct ObjectRuntime *object;
  struct EffectSprite *sprite;
  struct SprayParticle *particle;
  struct SprayParticle *puff;
  s32 pos[3];
  s32 i;
  s32 j;
  u16 target;
  s32 turn;

  if (work->emitting != 0 && (gFrameCount & 3) == 0) {
    sprite = work->drops;
    particle = work->dropMotion;
    for (i = 0; i <= 15; i++) {
      if (particle->timer == -1) {
        object = scene->otherObject;
        if (object == NULL)
          object = scene->mainObject;
        sprite->depth = BattleEffect_GetObjectDepth(object);
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
        particle->velocityZ = 0;
        particle->z = work->origin[2];
        particle->velocityX = 0;
        particle->velocityY = -0x13333;
        Vector_AddPolarOffset(work->speed, work->angle, &particle->velocityX);
        particle->timer = 0;
        break;
      }
      sprite++;
      particle++;
    }
  }

  work->hit = 0;
  sprite = work->drops;
  particle = work->dropMotion;
  puff = work->puffMotion;
  for (i = 0; i <= 15; i++, sprite++, particle++, puff++) {
    if (particle->timer == -1)
      continue;
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
      object = event->objects[j];
      if (object != NULL && object->script != NULL &&
          (object->collisionFlags & 1) && object != scene->mainObject &&
          object != scene->otherObject &&
          Func_080dbe80(&particle->x, 2, &object->x, object->radius - 2) >= 0) {
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
      puff->x = pos[0];
      puff->y = pos[1];
      puff->z = pos[2];
      puff->timer = 0;
    }
    if (work->hit != 0)
      break;
    BattleEffect_IntegrateVector3(&particle->x, 64, work->gravity);
  }

  sprite = work->puffs;
  puff = work->puffMotion;
  for (i = 15; i >= 0; i--) {
    if (puff->timer >= 0) {
      sprite->oam.index = work->puffTile + ((puff->timer >> 2) & 7) * 8;
      pos[0] = puff->x;
      pos[1] = puff->y;
      pos[2] = puff->z;
      Camera_WorldToScreen(pos);
      sprite->x = pos[0];
      sprite->y = pos[2];
      Func_080eb01c(sprite);
      puff->y += 0xcccc;
      if (++puff->timer == 32)
        puff->timer = -1;
    }
    sprite++;
    puff++;
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
