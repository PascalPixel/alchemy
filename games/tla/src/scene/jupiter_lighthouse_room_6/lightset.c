#include "types.h"
#include "ram_buffer.h"
#include "field_event.h"
#include "iwram_call.h"
#include "dma.h"
#include "io_reg.h"

#define BYTE_AT(base, offset) (*(u8 *)((u8 *)(base) + (offset)))
#define WORD_AT(base, offset) (*(s32 *)((u8 *)(base) + (offset)))

extern s32 gData_020023bc;
extern s32 gData_020023c0;
extern u16 gData_020023e8[];
extern u16 gData_02002408[];
extern u16 gData_0200240c[];

s32 GameFlag_Test();
void Task_Add();
void Func_020002e8(void);

void Func_020003a4(s32 mode) {
  gData_020023c0 = 1;
  if (mode != 2) {
    Iwram_CopyWords(gData_020023e8, (void *)0x05000180, 32);
    Iwram_CopyWords(gData_02002408, (void *)0x050001a0, 32);
  }
  if (GameFlag_Test(0xa00) || mode == 1) {
    s32 zero = 0; /* FAKEMATCH: ⚓️ builds the zero in r2 and the counter address
                     in r3 */

    gData_020023bc = zero;
    Task_Add(Func_020002e8, 1152);
  } else {
    Dma_Set(gData_0200240c, (void *)0x05000184, 0x84000002, REG_DMA3);
  }
}
