// Draft, differ 80 of 69: the native rebuilds every loop constant each pass
// (the 0x10000 step, 352 and the BLDALPHA address), keeps i in r4 and builds
// blend + 1 in r1, storing it back at the end of the branch; this for loop
// hoists the constants into r4, ip and lr (16 from the pool), keeps i in r1
// and the address in r8. A while or do/while loop measures the same 80. A
// goto loop, which loop.c leaves alone, measures 43 (i in r0, the register
// address in r4, 16 pooled) and is not kept: no new goto. Tried: the step as
// a local, as ++blend inside the expression, the operands of | swapped.
#include "../../../../games/tla/src/scene/gabomba_statue_chamber/gabomba_statue_chamber.h"
#include "../../../../games/tla/include/system/io_reg.h"

struct GabombaStatueChamberLayer {
  u8 unk00[12];
  s32 y;
  u8 unk10[0x28];
};

struct GabombaStatueChamberMapWork {
  u8 unk000[0x108];
  struct GabombaStatueChamberLayer layers[3];
};

void Func_02001d40(void);
void Func_02001d60(void);
void Func_02000be8(void);

void Func_02001ba8(void) {
  if (GameFlag_Test(265)) {
    if (GameFlag_Test(519)) {
      struct GabombaStatueChamberMapWork *map =
          (struct GabombaStatueChamberMapWork *)Ram_HeapSlots->mapWork;
      struct GabombaStatueChamberLayer *front = &map->layers[0];
      struct GabombaStatueChamberLayer *back = &map->layers[2];
      s32 blend = 1;
      s32 i;

      Func_02001d60();
      for (i = 0; i < 352; i++) {
        if (i >= 17 && i <= 79) {
          blend++;
          REG_BLDALPHA = ((blend / 4) << 8) | (16 - blend / 4);
        }
        front->y += 0x10000;
        back->y += 0x10000;
      }
      Func_02001d40();
      wait(1);
    }
  } else if (gAdventure.entrance == 2) {
    Func_02000be8();
  }
}
