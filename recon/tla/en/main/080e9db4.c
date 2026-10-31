// Near miss, differ 4 of 90 (tla-en, 2026-10-31): scheduler order only, no
// device. The first wait loop is the same plain for loop as the other two
// (the goto into a do-while gave the same 4); the one-pass block from the
// render mode store to the gravity store is gone (4 with it, the render mode's
// 0 then set too early; 4 without it, the opening matching). What remains:
// the native finishes the gravity's 0x10000 and its store before the work
// offset 0x102c goes on (lsls r2, adds r2); agscc interleaves them. In sched2
// the offset's lsls and the gravity's movs both rank 69, and the movs is
// anti-dependent on the y store just scheduled, so the lsls goes first; the
// native's order needs the gravity chain two above the offset chain. The
// other 23 orders of the four stores measure 6 to 12; reading the resource
// before the stores 12; through GlowWork (as Func_080e9af4's draft) or
// *(s16 *)(work + 0x102c) the same 4. alchemy-devtools permute (4 minutes)
// found only a one-pass block from the first wait loop to the gravity store:
// 2, the offset's movs r2, #128 then after the gravity store, not before the
// y store; taking &work->resource inside that block is 4 or 10.
// Near miss of Func_080e9db4 (first written 2026-10-12): differ 4 of 90 since
// 2026-10-19 (6 on 2026-10-18).
#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "task.h"
#include "effect_sprite.h"

// Kind 24's work (battle_fx_work[0]), as Func_080e9af4's draft has it.
struct GlowWork {
  u8 unk0000[0x1000];
  struct EffectSprite sprite;
  s32 scale;
  s16 resource;
  u16 tile;
  u16 unk1030;
  u16 phase;
  s16 fading;
};

s32 Party_GetLeaderActor(void);
struct Actor *Actor_GetById(s32 id);
void Actor_SetPositionAndResetMotionFar(struct Actor *object, s32 x,
                                         s32 y, s32 z);
void wait(s32 frames);
s32 Resource_ResetEntry(u32 entry);
void HeapSlot_Release(s32 slot);
void Func_080e9940(void);

void Func_080e9db4(void) {
  struct GlowWork *work = (struct GlowWork *)Ram_HeapSlots->battleFxWork[0];
  struct Actor *object = Actor_GetById(Party_GetLeaderActor());
  s32 i;

  Task_Remove(Func_080e9940);
  Actor_SetPositionAndResetMotionFar(object, object->x, object->y, object->z);
  object->motionFlags = 3;
  gMainRoutineWork.renderMode = 0;
  for (i = 0; i < 60; i++) {
    if (object->y <= object->terrainHeight)
      break;
    wait(1);
  }
  Sound_PlayFar(132);
  wait(1);
  for (i = 0; i < 20; i++) {
    if (object->y <= object->terrainHeight)
      break;
    wait(1);
  }
  wait(1);
  for (i = 0; i < 20; i++) {
    if (object->y <= object->terrainHeight)
      break;
    wait(1);
  }
  wait(1);
  object->bounce = 0;
  object->velocityY = 0;
  object->y = object->terrainHeight;
  object->gravity = 0x10000;
  Resource_ResetEntry(work->resource);
  HeapSlot_Release(92);
}
