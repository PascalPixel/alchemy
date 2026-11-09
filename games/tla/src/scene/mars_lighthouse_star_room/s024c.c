#include "mars_lighthouse_star_room.h"
#include "version.h"

void Func_02000d88(void);
void Func_02000080(s32 a);

struct EventDelay {
  u8 unk000[0x16a];
  u16 frames;
};

void Func_0200024c(s32 phase, s32 frame) {
  if (phase == 1) {
    Event_PlaySound(574);
    PaletteFade_ApplyTarget(0x10000, 0);
    PaletteFade_Interpolate(60);
    Func_02000d88();
    Actor_SetPosition(10, 0, 0);
#if TLA_PATCH_1
    Event_SetFlag(0x238);
#endif
    ((struct EventDelay *)Ram_HeapSlots->eventWork)->frames = 20;
  }
  if ((u32)(frame - 97) <= 174 && frame % 30 == 0)
    Func_02000080(10);
  if (frame == 276) {
    PaletteFade_ApplyTarget(0x7fff, 0);
    PaletteFade_Interpolate(20);
  }
}
