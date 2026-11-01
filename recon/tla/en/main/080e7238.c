// Matches: differ 0 (tla-en, 2026-11-01)
// The controlled actor grows out of a whirl of 100 sparks that spiral in to
// it (resource 0x1ef, FieldEffect_ColouredOrbs, as Func_080e74d8 uses); The
// Broken Seal has no twin. Forms the match depends on, no devices: the first
// loop steps spark by hand and rebuilds sprite from i, which keeps the -13
// mask in the loop; the spawn loop is if plus do/while with radius set before
// i, so the constant stays above i's copy; the update loop counts up and
// loop.c reverses it; the block locals in that loop load the call's arguments
// ahead of the position copy; next++ and next %= stay separate statements.
#include "types.h"
#include "heap_release.h"
#include "resource.h"
#include "actor.h"
#include "event_work.h"
#include "effect_sprite.h"
#include "system.h"

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
struct WhirlWork {
  u8 picture[0x400];
  struct WhirlSpark sparks[WHIRL_COUNT];
  struct EffectSprite sprites[WHIRL_COUNT];
};

extern char ResourceId_ColouredOrbs;

void wait(s32 frames);
s32 Resource_FindFreeEntry(void);
s32 Resource_ResetEntry(u32 entry);
void HeapSlot_Release(s32 slot);
void Actor_SetSingleChildFlagsFar(struct Actor *object, u32 flags);
void Vector_AddPolarOffset(s32 magnitude, s32 angle, s32 *position);
void Camera_WorldToScreen(s32 *position);
void Sound_PlayFar(s32 cue);
void Func_080dc954(void);

void Func_080e7238(void) {
  s32 position[3];
  struct WhirlWork *work = HeapSlot_Alloc(92, sizeof(*work));
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
      object->angle += 0x2000;
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
        object->angle = 0x4000;
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
