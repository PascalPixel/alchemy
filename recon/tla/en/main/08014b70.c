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
