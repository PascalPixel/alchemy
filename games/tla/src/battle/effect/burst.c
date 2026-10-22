#include "types.h"
#include "system.h"
#include "ram_buffer.h"
#include "fixed_math.h"
#include "fx_scene.h"
#include "object_runtime.h"
#include "edition.h"

struct BurstParticleVector {
  s32 values[3];
};

extern u8 gData_080f0ee8[];

void Func_080ddbd8(void);
void Audio_PlayCue(s32 no);
void Vector_AddPolarOffset(s32 magnitude, s32 angle,
                           struct BurstParticleVector *position);
void *Object_Spawn(s32 id, s32 x, s32 y, s32 z);
void ObjectDispatch_InitializeFar(void *object, u32 script);
void BattleEffect_PrepareBufferInterpolation(void);
// Far call to the target classifier Func_080237c8. English and Japanese name
// its slot Func_08020330; in the others that name is a later slot, and the
// one at 0x08020330 is Func_080237c8Far.
#if (LANGUAGE == LANGUAGE_ENGLISH) || (LANGUAGE == LANGUAGE_JAPANESE)
#define Func_080237c8Far Func_08020330
#endif
s32 Func_080237c8Far(struct ObjectRuntime *object);
void ObjectDispatch_SetSingleChildFlagsFar(struct ObjectRuntime *object,
                                           s32 flags);
void Func_080daecc(s32 objectId, s32 mode);

#define BURST_FALL_OBJECT 281 ///< the object that falls round the target

// The two kinds of target that do not sink into the ground.
#define TARGET_KIND_SCATTERS 372 ///< scatters five falling pieces instead
#define TARGET_KIND_STAYS 394

// The Spanish and Italian scaffolds still label Func_080daecc with another
// name, so those editions keep this function on the scaffold.
#if (LANGUAGE != LANGUAGE_SPANISH) && (LANGUAGE != LANGUAGE_ITALIAN)
/// Animation 12 on a target: the burst of particles, then the target sinks
/// to the underfoot priority. A target of kind 372 instead throws five pieces
/// round the scene's position; one of kind 394 only plays the cue (sound 126).
void Func_080dda30(s32 unused) {
  struct BattleFxScene *scene = Ram_HeapSlots->effectWork;
  struct ObjectRuntime *target = scene->child;
  s32 kind = 0;

  if (target == 0)
    return;

  Func_080ddbd8();
  if (Func_080237c8Far(target) == TARGET_KIND_SCATTERS)
    kind = 1;
  if (Func_080237c8Far(target) == TARGET_KIND_STAYS)
    kind = 2;

  if (kind == 0) {
    Object_SetMode(target, 2);
    ObjectDispatch_SetSingleChildFlagsFar(target, 0);
    target->collisionFlags = 0;
    /* FAKEMATCH(barrier): an empty asm after the collision store, which the
     * priority address is copied after; as plain statements, 2 of 113
     * instructions differ in tla-en. */
    asm("");
    target->priorityFlags |= 2;
    WaitFrames(10);
    Audio_PlayCue(126);
    WaitFrames(40);
  } else if (kind == 1) {
    struct BurstParticleVector position;
    struct BurstParticleVector *p = &position;
    s32 n = 4;

    do {
      struct ObjectRuntime *piece;
      s32 distance;

      p->values[0] = scene->x;
      p->values[2] = scene->z;
      distance = Random16() * 6 + 0x40000;
      Vector_AddPolarOffset(distance, Random16(), p);
      p->values[1] = scene->y;
      piece = Object_Spawn(BURST_FALL_OBJECT, p->values[0], p->values[1],
                           p->values[2]);
      if (piece != 0) {
        ObjectDispatch_InitializeFar(piece, (u32)gData_080f0ee8);
        piece->motionFlags = 2;
      }
      WaitFrames((((u32)Random16() * 2) >> 16) + 2);
      n--;
    } while (n >= 0);
    Func_080daecc(scene->secondObjectId, 1);
  } else {
    WaitFrames(10);
    Audio_PlayCue(126);
  }
  BattleEffect_PrepareBufferInterpolation();
}
#endif

void FieldFx_RunGrowth(void) {
  u8 *state = Ram_HeapSlots->effectWork;
  struct BurstParticleVector position;
  struct BurstParticleVector *p;
  s32 entryCount;

  Func_080ddbd8();
  Audio_PlayCue(134);
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
    object = Object_Spawn(0x119, p->values[0], p->values[1], p->values[2]);
    if (object != 0) {
      ObjectDispatch_InitializeFar(object, (u32)gData_080f0ee8);
      *((u8 *)object + 0x55) = 2;
    }
    WaitFrames((((u32)Random16() * 2) >> 16) + 2);
    entryCount--;
  } while (entryCount >= 0);
  WaitFrames(30);
  BattleEffect_PrepareBufferInterpolation();
}
