/* Draft: Func_080dda30, TLA English main image, 0x080dda30.
 * Plain source: 113 native instructions, 113 built, 112 common, differ 2
 * (tla-en, 2026-10-16). The priority-address copy is scheduled before the
 * collisionFlags store; the native copies it after that store.
 * An empty asm barrier between those operations matched in the proposed
 * adoption, but would increase the barrier count forbidden by S2. Keep the
 * adoption unlinked until a plain source form matches.
 * Tried: use the just-written collisionFlags in the priority OR (differ 2);
 * traverse both flag bytes through one u8 pointer (118 built, differ 45).
 */
#include "types.h"
#include "system.h"
#include "ram_buffer.h"
#include "fixed_math.h"
#include "fx_scene.h"
#include "object_runtime.h"

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
void BattleFx_PrepareBufferInterpolation(void);
s32 Func_08020330(struct ObjectRuntime *object);
void ObjectDispatch_SetSingleChildFlagsFar(struct ObjectRuntime *object,
                                           s32 flags);
void Func_080daecc(s32 objectId, s32 mode);

#define SE_RECOVERY 126
#define BURST_FALL_OBJECT 281 // the object that falls round the target

// The two kinds of target that do not sink into the ground.
#define TARGET_KIND_SCATTERS 372 // scatters five falling pieces instead
#define TARGET_KIND_STAYS 394

/* The Spanish and Italian scaffolds name two of its callees differently
 * (Func_080daecc in both, Func_08020330 in the Italian one); those two
 * editions keep it on their scaffold. */
#if !defined(TLA_EDITION_ES) && !defined(TLA_EDITION_IT)
// Animation 12 on a target: the burst of particles, then the target sinks
// to the underfoot priority. A target of kind 372 instead throws five pieces
// round the scene's position; one of kind 394 only plays the cue.
void Func_080dda30(s32 unused) {
  struct BattleFxScene *scene = Ram_HeapSlots->effectWork;
  struct ObjectRuntime *target = scene->child;
  s32 kind = 0;

  if (target == 0)
    return;

  Func_080ddbd8();
  if (Func_08020330(target) == TARGET_KIND_SCATTERS)
    kind = 1;
  if (Func_08020330(target) == TARGET_KIND_STAYS)
    kind = 2;

  if (kind == 0) {
    Object_SetMode(target, 2);
    ObjectDispatch_SetSingleChildFlagsFar(target, 0);
    target->collisionFlags = 0;
    target->priorityFlags |= 2;
    WaitFrames(10);
    Audio_PlayCue(SE_RECOVERY);
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
    Audio_PlayCue(SE_RECOVERY);
  }
  BattleFx_PrepareBufferInterpolation();
}
#endif
