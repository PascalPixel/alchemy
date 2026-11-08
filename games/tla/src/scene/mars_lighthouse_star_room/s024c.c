#include "mars_lighthouse_star_room.h"
#include "version.h"

void Func_02004404(s32 volume, s32 frames);
void Func_02004414(s32 frames);
void Func_02000d88(void);
void Func_0200432c(s32 id, s32 x, s32 z);
void Func_02000080(s32 a);

struct EventDelay {
  u8 unk000[0x16a];
  u16 frames;
};

void Func_0200024c(s32 phase, s32 frame) {
  /* FAKEMATCH(barrier): SPLIT_CONSTANT builds each constant as the game does, a
   * shifted byte plus an addend; as plain constants, 8 of 53 instructions
   * differ in tla-en. */
  if (phase == 1) {
    Func_0200446c(SPLIT_CONSTANT(0x23e));
    Func_02004404(0x10000, 0);
    Func_02004414(60);
    Func_02000d88();
    Func_0200432c(10, 0, 0);
#if TLA_PATCH_1
    GameFlag_SetBit(0x238);
#endif
    ((struct EventDelay *)Ram_HeapSlots->eventWork)->frames = 20;
  }
  if ((u32)(frame - 97) <= 174 && frame % 30 == 0)
    Func_02000080(10);
  if (frame == 276) {
    Func_02004404(SPLIT_CONSTANT(0x7fff), 0);
    Func_02004414(20);
  }
}
