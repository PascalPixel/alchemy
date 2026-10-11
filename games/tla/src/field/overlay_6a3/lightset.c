#include "types.h"
#include "ram_buffer.h"
#include "field_event.h"
#include "iwram_call.h"
#include "dma.h"
#include "io_reg.h"

extern s32 Data_02005a14;
extern s32 Data_02005a18;
extern u16 Data_02005a40[];
extern u16 Data_02005a60[];
extern u16 Data_02005a64[];

s32 GameFlag_Test();
void Scheduler_AddOrUpdateCallback();
void Func_02002f08(void);

void Func_02002fc4(s32 mode)
{
    Data_02005a18 = 1;
    if (mode != 2) {
        Iwram_CopyWords(Data_02005a40, (void *)0x05000180, 32);
        Iwram_CopyWords(Data_02005a60, (void *)0x050001a0, 32);
    }
    if (GameFlag_Test(0xa00) || mode == 1) {
        s32 zero = 0; /* FAKEMATCH: ⚓️ builds the zero in r2 and the counter address in r3 */

        Data_02005a14 = zero;
        Scheduler_AddOrUpdateCallback(Func_02002f08, 1152);
    } else {
        Dma_Set(Data_02005a64, (void *)0x05000184, 0x84000002, REG_DMA3);
    }
}
