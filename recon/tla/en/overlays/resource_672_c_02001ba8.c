// Matches: differ 0 (tla-en, 2026-11-08); also differ 0 in tla-ja, tla-de,
// tla-es, tla-fr and tla-it, plain C with no device.
// What closed it: the quotient written twice, ((blend + 1) / 4) << 8 and then
// 16 - (blend + 1) / 4, with blend++ after the store. The second division
// keeps its own compare and branch through local allocation (they are gone
// from the final code), so the quotient and the shifted value span blocks and
// become global pseudos: blend + 1 takes r1, the division's copy r3 and the
// quotient r0, as the listing has them. With the quotient in a named local
// (next = blend + 1; fade = next / 4) it is local to its block, takes r1
// first, the division's copy ties to it and next falls to r0 (differ 16 under
// every reshaping and a 10-minute permuter run). Func_02000be8's rise loop and
// Func_02000d3c stop on that same tie; this form is the one to try there.
// The loop is a label and goto: a for or while hoists its constants and
// compares with 351 where the listing rebuilds them every pass and compares
// with 352.
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
