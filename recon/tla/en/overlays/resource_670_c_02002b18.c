// Near miss, differ 6 of 63 (was 22): brightens palette colour `index` from
// entry `entry` by delta (green by half and blue by a quarter of it) through
// Func_02002b08. Taking the destination as a pointer before the channels are
// joined builds index * 2 in r10 as the native does. Left: the native loads
// the pool constant 0x1f one insn earlier, and places the literal pool before
// the third call where arm_reorg here places it after (the size estimate is 4
// bytes short there; an empty asm before that call moves the pool but
// disturbs the allocation, differ 40).
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

void Func_02002b18(s32 index, s32 entry, s32 delta) {
  u16 color = gData_02007948[entry];
  s16 r = color & 0x1f;
  s16 g = (color >> 5) & 0x1f;
  s16 b = (color >> 10) & 0x1f;

  r += delta;
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
