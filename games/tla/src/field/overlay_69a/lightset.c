#include "types.h"
#include "ram_buffer.h"
#include "iwram_call.h"
#include "dma.h"
#include "io_reg.h"

extern s32 Data_0200222c;
extern s32 Data_02002230;
extern u16 Data_02002258[];
extern u16 Data_02002278[];
extern u16 Data_0200227c[];

s32 GameFlag_Test();
void Scheduler_AddOrUpdateCallback();
void Func_020002c8(void);

void Func_02000384(s32 mode)
{
    Data_02002230 = 1;
    if (mode != 2) {
        Iwram_CopyWords(Data_02002258, (void *)0x05000180, 32);
        Iwram_CopyWords(Data_02002278, (void *)0x050001a0, 32);
    }
    if (GameFlag_Test(0xa00) || mode == 1) {
        s32 zero = 0; /* FAKEMATCH: ⚓️ builds the zero in r2 and the counter address in r3 */

        Data_0200222c = zero;
        Scheduler_AddOrUpdateCallback(Func_020002c8, 1152);
    } else {
        Dma_Set(Data_0200227c, (void *)0x05000184, 0x84000002, REG_DMA3);
    }
}
