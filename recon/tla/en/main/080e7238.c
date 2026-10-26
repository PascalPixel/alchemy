// Draft, differ 103 (245 before statements reordered 2026-10-26).
// Draft, differ 247 of 306 (2026-10-19, written from the listing; ☀️ has no
// twin): the controlled actor grows out of a whirl of 100 sparks that
// spiral in to it (resource 0x1ef, FieldEffect_ColouredOrbs, as Func_080e74d8
// uses). What remains is allocation: the ROM keeps the sprite in r5 and 15
// in r7, builds the ~12 mask inside the first loop and keeps the next index
// in r11 with no hoisted 0, and the stack slots follow from that.
#include "types.h"
#include "mem.h"
#include "resource.h"
#include "object_runtime.h"
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

void WaitFrames(s32 frames);
s32 Resource_FindFreeEntry(void);
s32 Resource_ResetEntry(u32 entry);
void Runtime_ReleaseHeapBlock(s32 slot);
void ObjectDispatch_SetSingleChildFlagsFar(struct ObjectRuntime *object,
                                           u32 flags);
void Vector_AddPolarOffset(s32 magnitude, s32 angle, s32 *position);
void Camera_WorldToScreen(s32 *position);
void Audio_PlayCue(s32 cue);
void Func_080dc954(void);

void Func_080e7238(void) {
  struct WhirlWork *work = Runtime_AllocateHeapBlock(92, sizeof(*work));
  struct ObjectRuntime *object =
      ObjectTable_Get(EventRuntime_GetControlledOwner());
  struct WhirlSpark *spark;
  struct EffectSprite *sprite;
  s32 position[3];
  s32 next;
  s32 entry;
  s32 tiles;
  s32 i;
  s32 angle;
  s32 count;
  s32 state;
  s32 frame;

  Resource_DecodeType01(Resource_GetTableEntry((s32)&ResourceId_ColouredOrbs),
                        work);
  spark = work->sparks;
  entry = Resource_FindFreeEntry();
  tiles = VramCache_Load(entry, 0x400, work);
  sprite = work->sprites;
  for (i = 0; i < WHIRL_COUNT; i++, sprite++, spark++) {
    Func_080eaf98(sprite, 4, 4, 0, tiles + (i & 15) * 2);
    sprite->oam.palette256 = 1;
    sprite->oam.palette = 0;
    sprite->oam.priority = 0;
    sprite->depth = 240;
    spark->age = -1;
  }
  Audio_PlayCue(200);
  WaitFrames(20);
  object->scaleX = 0;
  object->scaleY = 0;
  ObjectDispatch_SetSingleChildFlagsFar(object, 0);

  count = 2;
  next = 0;
  state = 0;
  frame = 0;
  do {
    for (i = count; i != 0; i--) {
      angle = Random16();
      spark = &work->sparks[next];
      spark->age = 0;
      spark->x = object->x;
      spark->y = object->y + Random16() * 20;
      next = (next + 1) % WHIRL_COUNT;
      spark->z = object->z;
      Vector_AddPolarOffset(Random16() << 3, angle, &spark->x);
      spark->angle = angle;
      spark->radius = 0x500000;
    }

    spark = work->sparks;
    sprite = work->sprites;
    for (i = WHIRL_COUNT - 1; i >= 0; i--, sprite++, spark++) {
      if (spark->age >= 0) {
        if ((u32)spark->age < 20) {
          position[0] = spark->x;
          position[1] = spark->y;
          position[2] = spark->z;
          Vector_AddPolarOffset(spark->radius, spark->angle, position);
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
        ObjectDispatch_SetSingleChildFlagsFar(object, 1);
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
    WaitFrames(1);
    frame++;
  } while (state != 999);
  Resource_ResetEntry(entry);
  Runtime_ReleaseHeapBlock(92);
  Func_080dc954();
}
