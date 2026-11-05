// Measured under -Os, 5 November 2026: differ 134 (tla-en); notes below may predate it.
// Draft, differ 193 of 272, skeleton gap 35 (tla-en, 2026-11-01): 19 removed,
// 16 added, nearly all order and allocation. Calls, branches and both
// palette loops are the listing's.
// Found: a u16 local is promoted signed, so assignment sign-extends and each
// read zero-extends; that is the listing's lsls/asrs and lsls/lsrs pairs. The
// palette loops are the scenes' goto idiom (palette set inside the loop, as in
// champa/s0a60.c); the counter falls between the palette and table copies.
// Left: (1) line and layer swap r10 and r8: global.c ranks line first. (2) In
// the first loop rotation should be a copy of the pool register in r6 and color
// r4; here rotation takes r4, so reload derives 0x0500005e from 0x05000060 and
// color lands in ip. from, to and rotation stand for what looks like the loop
// pass's work: a real do-while gets the hoisted copy and offsets but reduces
// the palette too (gap 55). (3) gFrameCount's pool load is shared by PRE across
// the first block; the listing reloads it. (4) The extra copy of red in the
// else arm: red and next want one register, r7.
// redBits, greenBits, blueBits and next are forced temporaries, and the first
// three are a guess at a hoist: the listing shifts all three after
// GameFlag_Test, before the branch, and PRE alone will not move them (the store
// written in both arms does not, nor a plain u16 red). Writing red as one
// variable lets PRE push red << 16 into the arms (18 becomes 144 << 13), so
// next is separate; next = red before the branch makes red block-local and
// caller-saved. A one-pass block would explain the hoist but is a device; if
// the temporaries stay they need FAKEMATCH(temporary) tags before adoption.
// gData_0200257c needs its size: as [] each store becomes load, IOR and store.
#include "../../../../games/tla/src/scene/sea_of_time/sea_of_time.h"
#include "map.h"
#include "io_reg.h"

/// The event work's fields this function reads that SeaOfTimeEventWork lacks.
struct EventWork_02001650 {
  u8 unk000[0x166];
  s16 unk166;
  u8 unk168[0x4a];
  u16 unk1b2;
};

extern u16 Data_02002580[160];
extern u16 gData_020026c0[14];
extern s16 gData_0200257c[1];

void Func_0200201c(s32 colors, s32 mode);

void Func_02001650(void) {
  u16 *line = Data_02002580;
  struct MapLayer *layer = &Ram_HeapSlots->mapWork->layers[2];
  struct EventWork_02001650 *event = Ram_HeapSlots->eventWork;
  u8 *block = Ram_HeapSlots->block32;
  s32 i;
  volatile u16 *palette;

  if (event->unk166 != 0)
    return;
  if (*(s8 *)(block + 0x2a01) != 0)
    return;
  if (event->unk1b2 == 0)
    return;

  if (gFrameCount - gData_020022c8 > gData_020022cc) {
    s16 color = BG_PLTT[62];
    s16 target = gData_020026c0[13];
    u8 *rotation = (u8 *)gData_020026c0;
    s32 from = 24;
    s32 to = 26;

    i = 14;
  rotate:
    palette = (volatile u16 *)0x05000060;
    palette[i] = (palette - 1)[i];
    i--;
    *(u16 *)(rotation + to) = *(u16 *)(rotation + from);
    from -= 2;
    to -= 2;
    if (i != 0)
      goto rotate;
    BG_PLTT[49] = color;
    gData_020026c0[0] = target;
    gData_020022c8 = gFrameCount;
  }

  if ((gFrameCount & 1) == 0) {
    s16 color = BG_PLTT[78];

    i = 14;
  rotate2:
    palette = (volatile u16 *)0x05000080;
    palette[i] = (palette - 1)[i];
    i--;
    if (i != 0)
      goto rotate2;
    BG_PLTT[65] = color;

    if ((gFrameCount & 3) == 0) {
      u16 color = BG_PLTT[15];
      s32 red = color & 31;
      s32 green = (color >> 5) & 31;
      s32 blue = (color >> 10) & 31;
      s32 flag = GameFlag_Test(0x200);
      u32 greenBits = green << 16;
      u32 redBits = red << 16;
      u32 blueBits = blue << 16;
      s32 next;

      if (flag == 0) {
        u32 sum = (u16)(redBits >> 16) + (u16)gData_0200257c[0];
        next = (s16)sum;
        if ((u16)sum < 18) {
          next = 18;
          gData_0200257c[0] = 1;
        } else if ((u16)sum > 31) {
          next = 31;
          gData_0200257c[0] = -1;
        }
      } else {
        next = red;
        if (((struct SeaOfTimeEventWork *)Ram_HeapSlots->eventWork)->alarm ==
            200) {
          u16 current = redBits >> 16;
          if (current != 0)
            next = (s16)(u16)(current - 1);
        } else if (((struct SeaOfTimeEventWork *)Ram_HeapSlots->eventWork)
                       ->alarm == 201) {
          u16 current = redBits >> 16;
          if (current < 30)
            next = current + 1;
        }
      }
      {
        u16 last = next;
        BG_PLTT[15] = (blueBits >> 6) | (greenBits >> 11) | (last & 31);
      }
    }
  }

  Dma_Set((const void *)BG_PLTT, Ram_HeapSlots->block32, DMA_COPY_WORDS(112),
          REG_DMA3);
  Func_0200201c(0x10000, 0);
  *(s32 *)((u8 *)layer + 0x1c) = 0;
  for (i = 0; i != 160; i++)
    *line++ =
        (Math_Sine((i + *(u8 *)&gFrameCount + (s16)(layer->y >> 16)) << 9) >> 15) +
        (s16)(layer->x >> 16);
  line = Data_02002580;
  REG_BG1HOFS = *line;
  Dma_Set(line + 1, (void *)&REG_BG1HOFS, 0xa2600001, (volatile u32 *)REG_DMA0);
}
