#include "types.h"
#include "ram_buffer.h"

u32 Random16(void);
void EffectSlot_UpdateFar(void *slot);
void Animation_ApplyChildValuesToRecordFar(void *child, u32 value);

struct ShopFxWork {
  u8 unk000[248];
  void *children[0x104];
  s8 selected;
  u8 unk509[3];
  u8 slots[24][72];
};

void Func_0810b168(void) {
  struct ShopFxWork *work = Ram_HeapSlots->menuRuntime;
  s32 i;
  s32 selected;

  /* FAKEMATCH: ⚓️ loads the work before it builds the slot offset */
  asm volatile("" : : : "memory");
  for (i = 0; i < 24; i++)
    EffectSlot_UpdateFar(work->slots[i]);
  selected = work->selected;
  if (selected != -1)
    Animation_ApplyChildValuesToRecordFar(work->children[selected],
                                          (Random16() * 7) >> 16);
}
