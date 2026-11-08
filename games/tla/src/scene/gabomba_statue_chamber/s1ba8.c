#include "gabomba_statue_chamber.h"
#include "../../../include/system/io_reg.h"

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
      i = 0;
    loop:
      if (i >= 17 && i <= 79) {
        u32 alpha = ((blend + 1) / 4) << 8;

        alpha |= 16 - (blend + 1) / 4;
        REG_BLDALPHA = alpha;
        blend++;
      }
      front->y += 0x10000;
      back->y += 0x10000;
      i++;
      if (i < 352)
        goto loop;
      Func_02001d40();
      wait(1);
    }
  } else if (gMainRoutineWork.entrance == 2) {
    Func_02000be8();
  }
}
