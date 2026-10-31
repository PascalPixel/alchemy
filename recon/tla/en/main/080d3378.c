// Near miss, differ 2 of 94 (tla-en, 2026-10-30): scheduler order only, as
// ☀️'s BattleEffect_SpawnBurstParticle. No empty asm before any one statement
// lowers it. The native stores the cleared action (strh r5) before it starts
// the priority flags' address (adds r0, r6, #0); sched2 here ranks the address
// first by one: the flags' ldrb waits on the strh by its store cost 1 (prio
// 23) and on the address by two adds (prio 24), so only a store-to-load
// latency of 2, the native's model, orders them as the native does. Also
// tried 2026-10-30: the flags through a u8 pointer or a raw byte offset
// (differ 6), the action through a u16 cast or flags = flags | 2 (differ 2).
// The rate is a variable as
// the native multiplies by its pool constant: as -0x1999 * speed it is
// synthesized from shifts (differ 26). An empty asm after the store moves the
// pool's zero ahead of it (differ 4, 8 with a memory clobber); a one-pass
// block around the store differs 28; permute (4 min) found nothing.
#include "event_work.h"
#include "actor.h"
#include "objdisp.h"
#include "system.h"

extern const s32 Data_080f085c[];
extern const s32 Data_080f0874[];

struct Actor *Func_080200c0(s32 kind, s32 x, s32 y, s32 z);
void Actor_SetPartAttribute(struct Actor *object, s32 value);
void FieldEffect_UpdateParticleLinearMotion(struct Actor *object);

struct ParticleSprite {
  u8 unk00[8];
  u16 tile : 10;
  u16 priority : 2;
  u16 palette : 4;
  u8 unk0a[16];
  u8 flags;
};

// Kicks up a dust puff (object 285) where source stands, as ☀️'s
// BattleEffect_SpawnBurstParticle.
void Func_080d3378(struct Actor *source, s32 param) {
  struct Actor *object;
  struct ParticleSprite *child;
  s32 speed;
  s32 rate = -0x1999;

  object = Func_080200c0(285, source->x, source->y, source->z);
  if (object != NULL) {
    child = object->animation;
    switch (Random16() & 1) {
    case 1:
      Actor_SetMode(object, 2);
      Actor_StartScriptFar((struct ActorView *)object,
                                   Data_080f085c);
      break;
    default:
      Actor_SetMode(object, 1);
      Actor_StartScriptFar((struct ActorView *)object,
                                   Data_080f0874);
      break;
    }

    if (param != 0)
      Actor_SetPartAttribute(object, param);

    object->motionFlags = 0;
    speed = Random16() % 10 + 5;
    object->acceleration = rate * speed;
    speed = Random16() % 15 - 7;
    speed <<= 1;
    object->speedLimit = 0x1999 * speed;
    object->action = 0;
    object->priorityFlags |= 2;
    ((u8 *)object)[0x61] = 1;
    object->update = (void (*)(void))FieldEffect_UpdateParticleLinearMotion;
    child->flags = 0;
    child->priority = ((struct ParticleSprite *)source->animation)->priority;
  }
}
