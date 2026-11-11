#include "types.h"
#include "ram_buffer.h"
#include "iwram_call.h"
#include "dma.h"
#include "io_reg.h"
#include "event_inline.h"

extern s32 gData_02002b34;
extern s32 gData_02002b38;
extern u16 gData_02002b98[];
extern u16 gData_02002bb8[];
extern u16 gData_02002bbc[];

void Func_020009e8(void);

void Func_02000aa4(s32 mode) {
  gData_02002b38 = 1;
  if (mode != 2) {
    Iwram_CopyWords(gData_02002b98, (void *)&BG_PLTT_COLOR(12, 0), 32);
    Iwram_CopyWords(gData_02002bb8, (void *)&BG_PLTT_COLOR(13, 0), 32);
  }
  if (Event_GetFlag(0xa00) || mode == 1) {
    /* FAKEMATCH(temporary): a local holding 0, so the zero is built in r2
     * and the counter's address in r3; as the 0 stored directly, 6 of 44
     * instructions differ in tla-en. */
    s32 zero = 0;

    gData_02002b34 = zero;
    Task_Add(Func_020009e8, 1152);
  } else {
    Dma_Set(gData_02002bbc, (void *)&BG_PLTT_COLOR(12, 2), DMA_COPY_WORDS(2),
            REG_DMA3);
  }
}
