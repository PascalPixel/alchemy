#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "io_reg.h"
#include "iwram_call.h"

struct FxBgScroll {
  u8 unk00[6];
  u16 y;
};

extern struct FxBgScroll gBgScroll[];
extern u32 gCameraSceneParameters[];

void BattleBackground_LoadFar(s32, s32, s32);
void wait(s32 frames);
void Sound_PlayFar(s32 cue);

static __inline__ void FillWords_(void *destination, s32 size, u32 value) {
  /* FAKEMATCH(one-pass): the call kept in a one-pass block, so the entry
   * loads before the size starts; as Iwram_FillWords(destination, size,
   * value) alone, 6 of 59 instructions differ in tla-en. */
  do {
    Iwram_FillWords(destination, size, value);
  } while (0);
}

void Func_0814cca8(void) {
  u8 *battle = Ram_HeapSlots->battleWork;
  void *buffer = Ram_HeapSlots->battleFxWork[1];

  REG_DISPCNT = 1;
  gBgScroll[0].y = 32;
  BattleBackground_LoadFar(1, *(u16 *)(battle + 0x670), 24);
  /* FAKEMATCH(wrapper): both fills forwarded through FillWords_, which keeps
   * each call's own size; as direct Iwram_FillWords calls, 43 of 59
   * instructions differ in tla-en. */
  FillWords_(buffer, 0x4000, 0);
  FillWords_((void *)0x06004000, 0x4000, 0);
  wait(1);
  REG_BLDALPHA = 0x100e;
  REG_BLDCNT = 0x3f46;
  REG_DISPCNT = 0x7741;
  gCameraSceneParameters[4] = 120;
  Sound_PlayFar(390);
}
