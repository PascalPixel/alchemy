#include "types.h"
#include "system.h"
#include "ram_buffer.h"
#include "fixed_math.h"
#include "field_psynergy_work.h"
#include "actor.h"
#include "edition.h"

struct BurstParticleVector {
  s32 values[3];
};

extern u8 gData_080f0ee8[];

void Func_080ddbd8(void);
void Sound_PlayFar(s32 no);
void Vector_AddPolarOffset(s32 magnitude, s32 angle,
                           struct BurstParticleVector *position);
void *Actor_Spawn(s32 id, s32 x, s32 y, s32 z);
void Actor_StartScriptFar(void *object, u32 script);
void BattleEffect_PrepareBufferInterpolation(void);
// Far call to the target classifier Func_080237c8.
s32 Func_08020330(struct Actor *object);
void Actor_SetSingleChildFlagsFar(struct Actor *object,
                                           s32 flags);
void Func_080daecc(s32 objectId, s32 mode);

#define BURST_FALL_OBJECT 281 ///< the object that falls round the target

// The two kinds of target that do not sink into the ground.
#define TARGET_KIND_SCATTERS 372 ///< scatters five falling pieces instead
#define TARGET_KIND_STAYS 394

/// Animation 12 on a target: the burst of particles, then the target sinks
/// to the underfoot priority. A target of kind 372 instead throws five pieces
/// round the scene's position; one of kind 394 only plays the cue (sound 126).
void Func_080dda30(s32 unused) {
  struct BattleFxScene *scene = Ram_HeapSlots->effectWork;
  struct Actor *target = scene->child;
  s32 kind = 0;

  if (target == 0)
    return;

  Func_080ddbd8();
  if (Func_08020330(target) == TARGET_KIND_SCATTERS)
    kind = 1;
  if (Func_08020330(target) == TARGET_KIND_STAYS)
    kind = 2;

  if (kind == 0) {
    Actor_SetMode(target, 2);
    Actor_SetSingleChildFlagsFar(target, 0);
    target->collisionFlags = 0;
    /* FAKEMATCH(barrier): an empty asm after the collision store, which the
     * priority address is copied after; as plain statements, 2 of 113
     * instructions differ in tla-en. */
    asm("");
    target->priorityFlags |= 2;
    wait(10);
    Sound_PlayFar(126);
    wait(40);
  } else if (kind == 1) {
    struct BurstParticleVector position;
    struct BurstParticleVector *p = &position;
    s32 n = 4;

    do {
      struct Actor *piece;
      s32 distance;

      p->values[0] = scene->x;
      p->values[2] = scene->z;
      distance = Random16() * 6 + 0x40000;
      Vector_AddPolarOffset(distance, Random16(), p);
      p->values[1] = scene->y;
      piece = Actor_Spawn(BURST_FALL_OBJECT, p->values[0], p->values[1],
                           p->values[2]);
      if (piece != 0) {
        Actor_StartScriptFar(piece, (u32)gData_080f0ee8);
        piece->motionFlags = 2;
      }
      wait((((u32)Random16() * 2) >> 16) + 2);
      n--;
    } while (n >= 0);
    Func_080daecc(scene->secondObjectId, 1);
  } else {
    wait(10);
    Sound_PlayFar(126);
  }
  BattleEffect_PrepareBufferInterpolation();
}

void FieldEffect_RunGrowth(void) {
  u8 *state = Ram_HeapSlots->effectWork;
  struct BurstParticleVector position;
  struct BurstParticleVector *p;
  s32 entryCount;

  Func_080ddbd8();
  Sound_PlayFar(134);
  p = &position;
  entryCount = 4;
  do {
    void *object;
    s32 randomValue;

    p->values[0] = *(s32 *)(state + 4);
    p->values[2] = *(s32 *)(state + 12);
    randomValue = (Random16() * 6) + 0x40000;
    Vector_AddPolarOffset(randomValue, Random16(), p);
    p->values[1] = *(s32 *)(state + 8);
    object = Actor_Spawn(0x119, p->values[0], p->values[1], p->values[2]);
    if (object != 0) {
      Actor_StartScriptFar(object, (u32)gData_080f0ee8);
      *((u8 *)object + 0x55) = 2;
    }
    wait((((u32)Random16() * 2) >> 16) + 2);
    entryCount--;
  } while (entryCount >= 0);
  wait(30);
  BattleEffect_PrepareBufferInterpolation();
}
