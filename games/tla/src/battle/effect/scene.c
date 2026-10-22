#include "types.h"
#include "ram_buffer.h"
#include "effect_slot.h"

void Field_StartPaletteTransition(s32 frames);

/// Fades the palettes back to the saved ones over the given frames.
void BattleEffect_FadeToSavedPalettes(s32 frames) { Field_StartPaletteTransition(frames); }

void BattleEffect_PrepareBufferInterpolation(void) {
  Field_StartPaletteTransition(8);
}

struct EffectVector {
  s32 x;
  s32 y;
  s32 z;
};

s32 Render_ProjectPoint(const void *position, s32 *result);

void Camera_WorldToScreen(struct EffectVector *position) {
  struct HeapSlots *slots = Ram_HeapSlots;

  if (*(s8 *)((u8 *)slots->eventWork + 0x18a) == 3) {
    s32 result[3];

    Render_ProjectPoint(position, result);
    position->x = result[0] << 16;
    position->z = result[1] << 16;
    position->y = 0;
  } else {
    u8 *state = (u8 *)slots->mapWork;
    s32 x = *(s32 *)(state + 228) & 0xffff0000;
    s32 z = *(s32 *)(state + 232) & 0xffff0000;

    position->x -= x;
    position->z -= position->y;
    position->z -= z;
    position->y = 0;
  }
}

struct BattleFxScene {
  u8 unk00[0x50];
  struct EffectSlot slots[24];
  u8 unk710[0xe4];
};

/// Scheduler callback: updates the scene's 24 effect slots.
void BattleEffect_UpdateAllEffectSlots(void) {
  struct BattleFxScene *scene;
  struct EffectSlot *slot;
  s32 remainingSlots;

  scene = Ram_HeapSlots->effectWork;
  remainingSlots = 0x17;
  slot = scene->slots;
  do {
    remainingSlots -= 1;
    EffectSlot_Update(slot);
    slot++;
  } while (remainingSlots >= 0);
}
