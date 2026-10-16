// Draft of Func_020000f4 of overlay 68D, a palette rotation, differ 16 of 35 (8
// register): goto loops as overlay 689's Func_020002f8, and the frame count's
// modulo through an inline (alchemy permute; plain, the address of gFrameCount
// is kept in a callee-saved register, differ 32). Left: the native gives the
// loop index r0 and the saved colour r1 (this draft the other way round), and
// loads colour 15's entry into r1 (here r2). Tried: the index set before the
// saved colour, the declarations swapped, an s32 or u32 index, a table pointer.
#include "types.h"
#include "frame.h"

extern s16 gData_02002608[];

static __inline__ u32 Frame_Mod6(u32 frame) { return frame % 6; }

// Every sixth frame rotates colours 1-14 of background palette 12 by one place;
// every frame sets its colour 15 from a twelve-step cycle.
void Func_020000f4(void) {
  s32 color;

  if (Frame_Mod6(gFrameCount) == 0) {
    volatile u16 *palette;
    s16 saved;
    s32 index;

    saved = *(volatile u16 *)0x0500019c;
    index = 14;
  loop:
    palette = (volatile u16 *)0x05000180;
    palette[index] = (palette - 1)[index];
    index--;
    if (index != 1)
      goto loop;
    *(volatile u16 *)0x05000182 = saved;
  }
  color = gData_02002608[(gFrameCount >> 2) % 12];
  *(volatile u16 *)0x0500019e = color;
}
