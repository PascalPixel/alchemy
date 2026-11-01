// Matches: differ 0 (tla-en, 2026-11-01)
// The controlled actor spins, shrinks and vanishes in a column of 100 rising
// and falling sparks (resource 0x1ef, FieldEffect_ColouredOrbs: adoption names
// it ColouredOrbs in gamepak/resource_directory.s). No FAKEMATCH device.
// What closed it: zero is one constant local that gets no register, so reload
// rebuilds its 0 at each use and rotates the spill registers (the else arm's
// literal 0 stays, cse uses the test's own zero there); next++ and next %= 100
// are two statements, which gives next its register; the setup loop steps
// particle and indexes sprite, or the -13 mask hoists; the draw loop sets i
// first and steps both pointers; age is reread in both arms, which is what
// cross-jumping merges down to the one shared bl. spin keeps its initializer
// first (the other order differs by 2).
#include "types.h"
#include "heap_release.h"
#include "resource.h"
#include "actor.h"
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

void wait(s32 frames);
s32 Resource_FindFreeEntry(void);
s32 Resource_ResetEntry(u32 entry);
void HeapSlot_Release(s32 slot);
void Actor_ArmCallback(s32 actor, s32 angle, s32 frames);
void Actor_ApplyRandomChildValues(void);
void Actor_SetSingleChildFlagsFar(struct Actor *object,
                                           u32 flags);
void EffectSprite_DrawAtWorldPosition(struct EffectSprite *sprite,
                                      struct SparkParticle *particle);
void Vector_AddPolarOffset(s32 magnitude, s32 angle, s32 *position);
void Sound_PlayFar(s32 cue);

void Func_080e74d8(void) {
  struct SparkWork *work = HeapSlot_Alloc(92, sizeof(*work));
  s32 owner = Party_GetLeaderActor();
  struct Actor *object =
      ObjectTable_Get(Party_GetLeaderActor());
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
    object->angle += spin + 0x1000;
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
          EffectSprite_DrawAtWorldPosition(sprite, particle);
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
