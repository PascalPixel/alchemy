#include "types.h"
#include "ram_buffer.h"
#include "iwram_call.h"
#include "dma.h"
#include "io_reg.h"

extern s32 gData_02002b34;
extern s32 gData_02002b38;
extern u16 gData_02002b98[];
extern u16 gData_02002bb8[];
extern u16 gData_02002bbc[];

s32 GameFlag_Test();
void Task_Add();
void Func_020009e8(void);

void Func_02000aa4(s32 mode) {
  gData_02002b38 = 1;
  if (mode != 2) {
    Iwram_CopyWords(gData_02002b98, (void *)&BG_PLTT_COLOR(12, 0), 32);
    Iwram_CopyWords(gData_02002bb8, (void *)&BG_PLTT_COLOR(13, 0), 32);
  }
  if (GameFlag_Test(0xa00) || mode == 1) {
    s32 zero = 0; /* FAKEMATCH: ⚓️ builds the zero in r2 and the counter address
                     in r3 */

    gData_02002b34 = zero;
    Task_Add(Func_020009e8, 1152);
  } else {
    Dma_Set(gData_02002bbc, (void *)&BG_PLTT_COLOR(12, 2), DMA_COPY_WORDS(2),
            REG_DMA3);
  }
}
