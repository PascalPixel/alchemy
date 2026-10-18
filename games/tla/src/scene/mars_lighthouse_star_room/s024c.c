#include "mars_lighthouse_star_room.h"
#include "edition.h"

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
  if (phase == 1) {
    Func_0200446c(574);
    Func_02004404(0x10000, 0);
    Func_02004414(60);
    Func_02000d88();
    Func_0200432c(10, 0, 0);
#if (LANGUAGE != LANGUAGE_JAPANESE)
    GameFlag_SetBit(0x238);
#endif
    ((struct EventDelay *)Ram_HeapSlots->eventWork)->frames = 20;
  }
  if ((u32)(frame - 97) <= 174 && frame % 30 == 0)
    Func_02000080(10);
  if (frame == 276) {
    Func_02004404(0x7fff, 0);
    Func_02004414(20);
  }
}
