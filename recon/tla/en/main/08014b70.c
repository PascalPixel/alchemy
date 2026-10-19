// Draft, differ 2 of 22: ☀️'s DebugText_Clear (fill screen block 4 with
// blank cells, home the cursor, point BG0 at it). The native loads the fill
// word 0xf000f000 from the pool before the prologue's sub sp, #4; here the
// stack adjusts first, the same prologue order as 08015ad0 and 08015ca4,
// and no statement can be placed before the prologue. Measured: ☀️'s form
// with Dma_Fill32 and Io_Write16 (also 2); four minutes of permuter found
// nothing better. Move it to games/tla/src/system/memory/debugnum.c, its
// linker neighbour, when it matches.
#include "types.h"
#include "dma.h"
#include "ram_buffer.h"
#include "iwram_call.h"
#include "mem.h"

extern u16 *gData_030011c4;
extern const u8 gData_08017b10[];
extern const u16 gUi_WindowPalette[];
extern u8 gIwramHeap[];
extern u8 gEwramHeap[];
extern u8 IwramSoundMixWorkspace[];
extern char ResourceId_WindowTiles;
void *Resource_GetTableEntry(s32);

void DebugText_Clear(void) {
  volatile u32 fill;

  fill = 0xf000f000;
  Dma_Set((const void *)&fill, (void *)0x06002000, 0x85000140,
          (volatile u32 *)0x040000d4);
  /* FAKEMATCH: the cursor store sits in a do-while(0) to hold the BG0CNT
   * constant load order */
  do {
    gData_030011c4 = (u16 *)0x06002000;
  } while (0);
  {
    s32 control = 0x400;

    *(volatile u16 *)0x04000008 = control;
  }
}
