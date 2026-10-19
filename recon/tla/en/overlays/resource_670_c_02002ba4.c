// Near miss, differ 2 of 65 (was 36): as Func_02002b18, with red raised by 4
// more and green and blue by 4 less, written in its draft's form (a halfword
// colour, the channels extracted first, the destination as a pointer). Left:
// the native loads the pool constant 0x1f one insn earlier (before adds r0,
// r0, r2); an empty asm before the red sum moves the masks instead (differ
// 16).
#include "../../../../games/tla/src/scene/kibombo/kibombo.h"

extern u8 gData_0200635c[];
extern s16 gData_02007948[];
extern struct KibomboObject *gData_02007950;

void Func_02001318(void);
s32 Func_02002b08(s32 value);
void Func_02002b18(s32 index, s32 entry, s32 delta);
void Func_02002ba4(s32 index, s32 entry, s32 delta);

struct KibomboWork90 {
  u8 unk00[0x18];
  struct KibomboObject *leader;
};

void Func_02002ba4(s32 index, s32 entry, s32 delta) {
  u16 color = gData_02007948[entry];
  s16 r = color & 0x1f;
  s16 g = (color >> 5) & 0x1f;
  s16 b = (color >> 10) & 0x1f;

  r += delta + 4;
  delta -= 4;
  g += delta >> 1;
  b += delta >> 2;

  r = Func_02002b08(r);
  g = Func_02002b08(g);
  b = Func_02002b08(b);
  {
    u16 *dst = &((u16 *)0x05000000)[index];

    r |= (b << 10) | (g << 5);
    *dst = r;
  }
}

