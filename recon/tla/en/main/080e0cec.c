// Under -mthumb-slow-loads (2026-10-26): differ 2 of 105 in tla-en, instruction
// order only. After a store the native builds a constant's half before the next
// load, where alchemy-gcc issues the load at once (AGENTS.md's recurring row);
// no single empty asm lowers it.
// Near miss, differ 2 of 105 (2026-10-18; plainly 4): the twin of The Broken
// Seal's BattleEffect_UpdateRadialLaunch; this game stores x and z to the slot's
// origin and position together and does not write them back to pos. After a
// store, the listing puts one other instruction before the next load through
// another base: it copies sp to r0 for the camera call before it loads y, and
// shifts the 8-pixel constant before it loads pos.z. The camera argument
// through an empty asm with a memory clobber fixes the first (tagged). Left:
// the shift. Measured for it: the 8 << 16 forced through an empty asm after the
// x stores, with a memory clobber, orders it but moves 0xc000 later to r2
// (differ 6; pinned to r3 behind its own asm, differ 2 in the subtract); a
// volatile barrier or a one-pass block there, differ 4; operand and store
// order, 2. The same row is in 0811bd50.
#include "types.h"
#include "ram_buffer.h"
#include "effect_slot.h"

struct LaunchVector {
  s32 x;
  s32 y;
  s32 z;
};

struct LaunchObject {
  u8 unk00[8];
  s32 x;
  s32 y;
  s32 z;
};

struct LaunchScene {
  u8 unk00[0x10];
  struct LaunchObject *mainObject;
};

extern u32 gFrameCount;

void Camera_WorldToScreen(struct LaunchVector *position);
void Vector_AddPolarOffset(s32 distance, s32 angle, void *position);
u32 Random16(void);
void Audio_PlayCue(s32 cue);
void BattleEffect_ClearOwnedSlot(struct EffectSlot *slot);

void Func_080e0cec(struct EffectSlot *slot) {
  struct LaunchVector pos;
  struct LaunchObject *src;
  s32 state;
  u32 r;
  struct LaunchVector *arg;
  s32 lift;

  src = ((struct LaunchScene *)Ram_HeapSlots->effectWork)->mainObject;
  state = slot->state;

  if (state == 0) {
    pos.x = src->x;
    arg = &pos;
    pos.y = src->y;
    pos.z = src->z;
    Camera_WorldToScreen(arg);

    slot->x = slot->originX = pos.x;
    slot->z = slot->originZ = pos.z + (8 << 16);

    r = Random16();
    Vector_AddPolarOffset(
        120 << 16, ((r << 13) >> 16) - ((Random16() << 13) >> 16) + 0xc000,
        &pos);

    slot->targetX = pos.x;
    slot->targetZ = pos.z;
    slot->acceleration = 5 << 16;
    slot->maxSpeed = 5 << 16;
    slot->limitTurnStep = state;
    slot->state++;

    if ((gFrameCount & 2) != 0)
      Audio_PlayCue(246);
  } else if (state == 1) {
    if (EffectSlot_HasPendingTarget(slot) == 0)
      slot->state--;
  } else if (state == 2) {
    if (EffectSlot_HasPendingTarget(slot) == 0)
      BattleEffect_ClearOwnedSlot(slot);
  }
}
