#include "types.h"
#include "call.h"
#include "ram_buffer.h"
#include "actor.h"
#include "effect_sprite.h"

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

struct FxScene {
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

struct PendingEvent {
  u8 unk00[8];
  void (*callback)(s32, s32, s32);
};

extern char ResourceId_WaterSplash;

void *HeapSlot_Alloc(s32 slot, s32 size);
void HeapSlot_Release(s32 slot);
s32 Func_080ce31c(s32 kind);
struct PendingEvent *Func_080ce458(u32 kind, s32 entry, s32 arg);
void Vector_AddPolarOffset(s32 distance, s32 angle, s32 *position);
s32 Func_08020330(struct Actor *object);
void BattleEffect_InitializeSharedScene(void);
void *Resource_GetTableEntry(s32 resourceId);
s32 Resource_DecodeType01(const void *source, void *destination);
s32 Resource_FindFreeEntry(void);
s32 VramCache_Load(s32 slot, s32 size, const void *data);
void Resource_ResetEntry(s32 slot);
void Animation_ApplyChildValueFar(void *sprite, s32 value);
void Func_080e43a4(void *source, void *buffer, s32 size);
void Func_080e89e4(s32 direction);
void Func_080e8b44(s32 direction, s32 phase, s32 amplitude);
void Func_080e8c9c(void);
void wait(s32 frames);
void Sound_PlayFar(s32 cue);
u32 Random16(void);
s32 BattleEffect_GetObjectPriority(struct Actor *object);
s32 BattleEffect_GetObjectDepth(struct Actor *object);
void EffectSprite_DrawAtWorldPosition(struct EffectSprite *sprite,
                                      struct SplashParticle *particle);
void BattleEffect_ShiftRowsDown(u8 *base, s32 stride, s32 rows);
void BattleEffect_PrepareBufferInterpolation(void);

void Func_080e92b4(void) {
  struct SplashWork *work = HeapSlot_Alloc(92, 0x1920);
  struct FxScene *scene = Ram_HeapSlots->effectWork;
  struct Actor *caster = scene->mainObject;
  struct Actor *child = scene->child;
  struct SplashAnimation *animation = child->animation;
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
  struct PendingEvent *pending;
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
  pending = Func_080ce458(SPLIT_CONSTANT(0x50000005), scene->fieldKind, selector);
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
    Func_080e43a4(animation->tiles, picture,
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
        EffectSprite_DrawAtWorldPosition(sprite, particle);
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
          ValueLr3((s32 (*)())pending->callback, (s32)(0), (s32)(timer), (s32)(selector));
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
