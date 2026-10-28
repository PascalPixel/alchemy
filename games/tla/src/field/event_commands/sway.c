#include "types.h"
#include "iwram_call.h"
#include "objdisp.h"
#include "object_effect_scripts.h"
#include "actor.h"
#include "fx_scene.h"
#include "ram_buffer.h"
#include "macros.h"

struct SwayObject {
  u8 unk00[8];
  s32 y;
  u8 unk0c[44];
  s32 baseY;
  u8 unk3c[40];
  s16 phase;
};

struct OrbitVector {
  s32 x;
  s32 y;
  s32 z;
};

s32 Trig_Sin(s32 angle);
void Vector_AddPolarOffset(s32 magnitude, s32 angle,
                           struct OrbitVector *position);

/// Per-frame update: sways 4 pixels either side of the anchor, once every
/// 128 frames.
void BattleEffect_SwayParticle(struct SwayObject *object) {
  s16 *phase = &object->phase;
  s32 amplitude = 0x40000;

  object->y = object->baseY + Iwram_MulQ16(amplitude, Trig_Sin(*phase << 9));
  (*phase)++;
  *phase = (*phase + 128) % 128;
}

/// The particle circles 10 pixels above the scene's position, its radius in
/// pixels the frames it has left; at 0 it runs the shared particle script.
void BattleEffect_UpdateShrinkingOrbitObject(struct Actor *object) {
  struct BattleFxScene *scene = Ram_HeapSlots->effectWork;
  struct OrbitVector pos;
  s16 life;
  s32 count;
  s32 radius;
  /* FAKEMATCH: the lift's first byte is pinned to the r0 the native builds it
   * in; as a free temporary local-alloc gives it r2 and moves life to r4
   * (differ 18). */
  register s32 lift asm("r0");

  if (object != NULL) {
    count = (u16)object->action - 1;
    object->action = count;
    life = count;
    if (life != 0) {
      pos.x = scene->x;
      lift = 160;
      /* FAKEMATCH: the native starts the 10-pixel lift (160 << 12) between the
       * x store and the y load, and shifts it after the load; as one constant
       * both halves come after the load (differ 2). */
      asm volatile("" : "+l"(lift));
      pos.y = scene->y + (lift << 12);
      radius = life << 16;
      /* FAKEMATCH: the loop note ends the scheduling block: the native starts
       * the radius between the y store and the z load; without it the z load
       * comes first (differ 2). */
      do {
      } while (0);
      pos.z = scene->z;
      Vector_AddPolarOffset(radius, (s16)object->unk66 + (life << 11), &pos);
      SET_POSITION(object, pos.x, pos.y, pos.z);
    } else {
      Actor_StartScriptFar((struct ActorView *)object,
                                   gBattleEffect_SharedParticleScript);
    }
  }
}
