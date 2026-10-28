// Draft, differ 256 of 358 (2026-10-19, written from the listing; ☀️ has no
// twin): the controlled actor spins, shrinks and vanishes in a column of 100
// rising and falling sparks (resource 0x1ef, FieldEffect_ColouredOrbs: adoption
// names it ColouredOrbs in system/resource/directory.s). Most of what
// remains is allocation: agscc hoists the spawn loop's stored 0 into r11,
// where the ROM keeps the next spark's index there and rematerializes the 0
// at each store, so the index spills and the stack frame grows by a slot; and the
// ROM builds the ~12 mask inside the first loop where agscc hoists it. The
// spin loop counts down in the frame counter's register (separate counters
// 297), and the first loop walks sprite and spark pointers (256).
#include "types.h"
#include "mem.h"
#include "resource.h"
#include "object_runtime.h"
#include "event_work.h"
#include "effect_sprite.h"
#include "battle.h"
#include "system.h"

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

extern char ResourceId_ColouredOrbs;

void WaitFrames(s32 frames);
s32 Resource_FindFreeEntry(void);
s32 Resource_ResetEntry(u32 entry);
void Runtime_ReleaseHeapBlock(s32 slot);
void ObjectMotion_ArmCallback(s32 actor, s32 angle, s32 frames);
void ObjectGroup_ApplyRandomChildValues(void);
void ObjectDispatch_SetSingleChildFlagsFar(struct ObjectRuntime *object,
                                           u32 flags);
void EffectSprite_DrawAtWorldPosition(struct EffectSprite *sprite,
                                      struct SparkParticle *particle);
void Vector_AddPolarOffset(s32 magnitude, s32 angle, s32 *position);
void Sound_PlayFar(s32 cue);

void Func_080e74d8(void) {
  struct SparkWork *work = Runtime_AllocateHeapBlock(92, sizeof(*work));
  s32 owner = EventRuntime_GetControlledOwner();
  struct ObjectRuntime *object =
      ObjectTable_Get(EventRuntime_GetControlledOwner());
  struct SparkParticle *particle;
  struct EffectSprite *sprite;
  s32 entry;
  s32 tiles;
  s32 i;
  s32 spin;
  s32 angle;
  s32 count;
  s32 state;
  s32 next;
  s32 frame;

  WaitFrames(20);
  BattleEffect_InitializeSharedScene();
  Resource_DecodeType01(Resource_GetTableEntry((s32)&ResourceId_ColouredOrbs),
                        work);
  entry = Resource_FindFreeEntry();
  tiles = VramCache_Load(entry, 0x400, work);
  particle = work->particles;
  sprite = work->sprites;
  for (i = 0; i < SPARK_COUNT; i++, sprite++, particle++) {
    Func_080eaf98(sprite, 4, 4, 0, tiles + (i & 15) * 2);
    sprite->oam.palette256 = 1;
    sprite->oam.palette = 0;
    sprite->oam.priority = 0;
    sprite->depth = 240;
    particle->age = -1;
  }
  WaitFrames(10);
  ObjectMotion_ArmCallback(owner, 0x4000, 0);
  WaitFrames(20);
  Object_SetMode(object, 28);
  object->update = ObjectGroup_ApplyRandomChildValues;
  Sound_PlayFar(131);
  WaitFrames(30);
  Sound_PlayFar(220);
  Object_SetMode(object, 3);
  for (frame = 49, spin = 0; frame >= 0; frame--, spin += 60) {
    object->angle += spin + 0x1000;
    WaitFrames(1);
  }

  count = 2;
  state = 0;
  next = 0;
  frame = 0;
  do {
    for (i = count; i != 0; i--) {
      angle = Random16();
      particle = &work->particles[next];
      particle->age = 0;
      particle->x = object->x;
      particle->y = object->y + Random16() * 20;
      particle->z = object->z;
      Vector_AddPolarOffset(Random16() << 3, angle, &particle->x);
      if (next & 1) {
        particle->velocityX = 0;
        particle->velocityY = -0x40000;
        particle->velocityZ = 0;
        Vector_AddPolarOffset((Random16() << 2) + 0x40000, angle,
                              &particle->velocityX);
      } else {
        particle->velocityX = 0;
        particle->velocityY = 0x50000;
        particle->velocityZ = 0;
        Vector_AddPolarOffset((Random16() << 2) + 0x40000, angle,
                              &particle->velocityX);
      }
      next = (next + 1) % SPARK_COUNT;
    }

    for (i = 0; i < SPARK_COUNT; i++) {
      particle = &work->particles[i];
      if (particle->age >= 0) {
        if ((u32)particle->age < 20) {
          EffectSprite_DrawAtWorldPosition(&work->sprites[i], particle);
          if (i & 1)
            BattleEffect_IntegrateVector3(&particle->x, 63, 0x8000);
          else
            BattleEffect_IntegrateVector3(&particle->x, 63, -0xa000);
        }
        if (++particle->age == 20)
          particle->age = -1;
      }
    }

    switch (state) {
    case 0:
      if (frame == 10)
        Sound_PlayFar(198);
      object->angle += 0x2000;
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
      object->angle += 0x2000;
      object->scaleX = 0x10000 - (frame << 16) / 30;
      if (object->scaleX < 0)
        object->scaleX = 0;
      object->scaleY = object->scaleX;
      ObjectDispatch_SetSingleChildFlagsFar(object, 0);
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
    WaitFrames(1);
    frame++;
  } while (state != 999);
  Resource_ResetEntry(entry);
  Runtime_ReleaseHeapBlock(92);
}
