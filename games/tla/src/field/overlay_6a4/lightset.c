#include "types.h"
#include "ram_buffer.h"
#include "field_event.h"
#include "iwram_call.h"
#include "dma.h"
#include "io_reg.h"

extern s32 Data_02003314;
extern s32 Data_02003318;
extern u16 Data_02003340[];
extern u16 Data_02003360[];
extern u16 Data_02003364[];

s32 GameFlag_Test();
void Scheduler_AddOrUpdateCallback();
void Func_0200079c(void);

void Func_02000858(s32 mode)
{
    Data_02003318 = 1;
    if (mode != 2) {
        Iwram_CopyWords(Data_02003340, (void *)0x05000180, 32);
        Iwram_CopyWords(Data_02003360, (void *)0x050001a0, 32);
    }
    if (GameFlag_Test(0xa00) || mode == 1) {
        s32 zero = 0; /* FAKEMATCH: ⚓️ builds the zero in r2 and the counter address in r3 */

        Data_02003314 = zero;
        Scheduler_AddOrUpdateCallback(Func_0200079c, 1152);
    } else {
        Dma_Set(Data_02003364, (void *)0x05000184, 0x84000002, REG_DMA3);
    }
}
