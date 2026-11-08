#include "types.h"
#include "call.h"
#include "ram_buffer.h"
#include "actor.h"
#include "effect_sprite.h"
#include "canvas_sprite.h"

// One moving point: a spark on the canvas (x and y only, counting delay) or
// a sprite's place in the world.
struct BowMotion {
  s32 x;
  s32 y;
  s32 z;
  s32 velocityX;
  s32 velocityY;
  s32 velocityZ;
  s32 delay;
};

struct BowWork {
  u8 *tiles;
  struct BowMotion sparks[32];
  struct BowMotion arrowMotion;
  struct EffectSprite arrow;
  struct BowMotion bowMotion;
  struct EffectSprite bow;
  s32 start[3];
  s32 target[3];
  s16 phase;
  s16 timer;
  u16 bowTile;
  s16 frame;
  s8 bowGrowing;
  s8 bowShrinking;
  s8 sparking;
  s8 sparksRepeat;
  s16 result;
  s8 done;
  s8 arrowMoving;
};

struct BowScene {
  u16 angleFraction;
  u16 angle;
  s32 x;
  s32 y;
  s32 z;
  struct Actor *mainObject;
};

struct BowEvent {
  u8 unk00[20];
  struct Actor *objects[80];
};

extern const s32 Data_080f0f34[8];
extern const s32 Data_080f0f54[4];
extern const u8 Data_080eda0c[];

void Vector_AddPolarOffset(s32 distance, s32 angle, s32 *position);
void Camera_WorldToScreen(s32 *position);
s32 Func_08020210(struct Actor *object, s32 *position);
s32 Func_080dbe80(s32 *a, s32 radiusA, s32 *b, s32 radiusB);
void BattleEffect_IntegrateVector2(s32 *vector, s32 damping, s32 gravity);
void BattleEffect_IntegrateVector3(s32 *vector, s32 damping, s32 gravity);
void BattleEffect_DrawCanvasBlock(const u8 *src, s32 x, s32 y, s32 width,
                                  s32 height);
u32 Random16(void);
s32 Trig_Sin(s32 angle);
s32 Trig_Cos(s32 angle);

#define BOW_NEXT_PHASE(work) ((work)->phase++, (work)->timer = -1)

void Func_080e1678(void) {
  struct BowScene *scene = Ram_HeapSlots->effectWork;
  struct BowWork *work = (struct BowWork *)Ram_HeapSlots->battleFxWork[0];
  struct CanvasSpriteWork *canvas = Ram_HeapSlots->unkec[1];
  struct BowEvent *event = Ram_HeapSlots->eventWork;
  struct Actor *object;
  struct BowMotion *motion;
  struct EffectSprite *sprite;
  s32 pos[3];
  s32 objectIndex;
  s32 i;
  s32 dx;
  s32 dz;

again:
  switch (work->phase) {
  hit:
    work->done = 1;
    work->result = objectIndex;
    BOW_NEXT_PHASE(work);
    goto again;
  case 0:
    if (work->timer == 0) {
      work->sparking = 0;
      work->sparksRepeat = 1;
      work->arrowMoving = 0;
      work->bowGrowing = 1;
      work->frame = 999;
    } else if (work->bowGrowing == 0) {
      work->frame = 0;
      BOW_NEXT_PHASE(work);
    }
    break;
  case 1:
    if (work->timer == 0) {
      sprite = &work->arrow;
      sprite->scaleX = sprite->scaleY = 0x10000;
      motion = &work->arrowMotion;
      pos[0] = pos[1] = pos[2] = 0;
      Vector_AddPolarOffset(SPLIT_CONSTANT(0x2666), scene->angle + 0x8000, pos);
      motion->velocityX = pos[0];
      motion->velocityY = pos[1];
      motion->velocityZ = pos[2];
      work->arrowMoving = 1;
    }
    if (work->timer == 20)
      work->sparking = 1;
    if (work->timer == 56) {
      motion = &work->arrowMotion;
      pos[0] = pos[1] = pos[2] = 0;
      Vector_AddPolarOffset(0x50000, scene->angle, pos);
      motion->velocityX = pos[0];
      motion->velocityY = pos[1];
      motion->velocityZ = pos[2];
      BOW_NEXT_PHASE(work);
    }
    break;
  case 2:
    motion = &work->arrowMotion;
    if (work->timer == 90) {
      BOW_NEXT_PHASE(work);
      break;
    }
    dx = work->target[0] - work->start[0];
    if (dx < 0)
      dx = work->start[0] - work->target[0];
    dz = motion->x - work->start[0];
    if (dz < 0)
      dz = work->start[0] - motion->x;
    if (dx < dz) {
      BOW_NEXT_PHASE(work);
      goto again;
    }
    dx = work->target[2] - work->start[2];
    if (dx < 0)
      dx = work->start[2] - work->target[2];
    dz = motion->z - work->start[2];
    if (dz < 0)
      dz = work->start[2] - motion->z;
    if (dx < dz) {
      BOW_NEXT_PHASE(work);
      goto again;
    }
    pos[0] = motion->x;
    pos[1] = motion->y;
    pos[2] = motion->z;
    if (Func_08020210(scene->mainObject, pos) > 0) {
      work->done = 1;
      work->result = -1;
      work->phase++;
      work->timer = -1;
      goto again;
    }
    for (objectIndex = 0; objectIndex < 80; objectIndex++) {
      object = event->objects[objectIndex];
      if (object != NULL && object->script != NULL &&
          (object->collisionFlags & 1) && object != scene->mainObject &&
          Func_080dbe80(&motion->x, 2, &object->x, object->radius - 2) >= 0)
        goto hit;
    }
    break;
  case 3:
    if (work->done != 0) {
      work->arrowMoving = 0;
    } else {
      work->sparksRepeat = 0;
      work->bowShrinking = 1;
    }
    motion = &work->arrowMotion;
    pos[0] = pos[1] = pos[2] = 0;
    Vector_AddPolarOffset(0x20000, scene->angle, pos);
    motion->velocityX = pos[0];
    motion->velocityY = pos[1];
    motion->velocityZ = pos[2];
    BOW_NEXT_PHASE(work);
    break;
  case 4:
    sprite = &work->arrow;
    if (work->done != 0) {
      if (work->timer < 32) {
        s32 wobble[8];
        s32 fade[4];
        s32 w;

        memcpy(wobble, Data_080f0f34, sizeof(wobble));
        memcpy(fade, Data_080f0f54, sizeof(fade));
        w = wobble[work->timer & 7];
        sprite->angle = scene->angle + w * fade[work->timer / 8] / 0x10000;
      }
      if (work->timer == 32)
        work->bowShrinking = 1;
      if (work->timer == 50) {
        sprite->scaleX = sprite->scaleY = 0;
        work->arrowMoving = 1;
        work->sparksRepeat = 0;
        BOW_NEXT_PHASE(work);
      }
    } else {
      sprite->scaleX += -0x3000;
      sprite->scaleY = sprite->scaleX;
      if (sprite->scaleX < 0) {
        sprite->scaleX = sprite->scaleY = 0;
        BOW_NEXT_PHASE(work);
      }
    }
    break;
  case 5:
    if (work->bowShrinking == 0)
      work->phase = 999;
    break;
  }

  if (work->bowGrowing != 0) {
    sprite = &work->bow;
    sprite->scaleX += 0x400;
    sprite->scaleY = sprite->scaleX;
    sprite->angle += 0x2000;
    if (sprite->scaleX >= 0x10000) {
      sprite->scaleX = sprite->scaleY = 0x10000;
      sprite->angle = scene->angle;
      work->bowGrowing = 0;
    }
  }
  if (work->bowShrinking != 0) {
    sprite = &work->bow;
    sprite->scaleX += -0x400;
    sprite->scaleY = sprite->scaleX;
    sprite->angle += 0x2000;
    if (sprite->scaleX <= 0x6665) {
      sprite->scaleX = sprite->scaleY = 0;
      work->bowShrinking = 0;
    }
  }

  // The bow's drawing pose, by frame: drawn back, then loosed and recovered.
  if (work->frame != 999) {
    work->frame++;
    sprite = &work->bow;
    switch (work->frame) {
    case 10:
      sprite->oam.index = work->bowTile + 32;
      break;
    case 20:
      sprite->oam.index = work->bowTile + 64;
      break;
    case 54:
      sprite->oam.index = work->bowTile + 32;
      break;
    case 62:
      sprite->oam.index = work->bowTile + 96;
      break;
    case 65:
      sprite->oam.index = work->bowTile + 128;
      break;
    case 68:
      sprite->oam.index = work->bowTile + 160;
      break;
    case 58:
    case 71:
      sprite->oam.index = work->bowTile;
      break;
    case 72:
      work->frame = SPLIT_CONSTANT(999);
      break;
    }
  }

  motion = &work->bowMotion;
  pos[0] = motion->x;
  pos[1] = motion->y;
  pos[2] = motion->z;
  sprite = &work->bow;
  Camera_WorldToScreen(pos);
  sprite->x = pos[0];
  sprite->y = pos[2];
  Func_080eb01c(sprite);
  motion = &work->arrowMotion;
  pos[0] = motion->x;
  pos[1] = motion->y;
  pos[2] = motion->z;
  Camera_WorldToScreen(pos);
  sprite = &work->arrow;
  sprite->x = pos[0];
  sprite->y = pos[2];
  Func_080eb01c(sprite);
  if (work->arrowMoving != 0)
    BattleEffect_IntegrateVector3(&motion->x, 64, 0);

  if (work->sparking != 0) {
    motion = work->sparks;
    for (i = 9; i >= 0; i--) {
      if (motion->delay == 0) {
        s32 speed;
        s32 angle;

        speed = (Random16() & 255) + 256;
        angle = (Random16() & 0x1fff) + 0x7000;
        motion->x = 0x3c0000;
        motion->y = 0x1c0000;
        motion->velocityX = Trig_Cos(angle) * speed >> 6;
        motion->velocityY = Trig_Sin(angle) * speed >> 6;
      }
      if (motion->delay > 0) {
        s32 x = (s16)(motion->x >> 16);

        BattleEffect_DrawCanvasBlock(Data_080eda0c + (motion->delay & 1) * 64,
                                     x, (s16)(motion->y >> 16), 8, 8);
        BattleEffect_IntegrateVector2(&motion->x, 60, 0);
/* FAKEMATCH(barrier): an empty asm after `BattleEffect_IntegrateVector2(&motion->x, 60, 0);`; as plain statements, 2 of 923 instructions differ in tla-en. */
asm("");
      }
      motion->delay++;
      if (work->sparksRepeat == 1 && motion->delay == 10)
        motion->delay = 0;
      motion++;
    }
  }

  motion = &work->arrowMotion;
  pos[0] = motion->x;
  pos[1] = motion->y;
  pos[2] = motion->z;
  Vector_AddPolarOffset(-0x240000, scene->angle, pos);
  Camera_WorldToScreen(pos);
  canvas->x = (s16)(pos[0] >> 16);
  canvas->y = (s16)(pos[2] >> 16);
  canvas->command = CANVAS_UPLOAD_AND_FADE3;
  work->timer++;
}
