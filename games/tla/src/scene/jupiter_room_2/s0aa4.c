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
void Scheduler_AddOrUpdateCallback();
void Func_020009e8(void);

void Func_02000aa4(s32 mode)
{
    gData_02002b38 = 1;
    if (mode != 2) {
        Iwram_CopyWords(gData_02002b98, (void *)0x05000180, 32);
        Iwram_CopyWords(gData_02002bb8, (void *)0x050001a0, 32);
    }
    if (GameFlag_Test(0xa00) || mode == 1) {
        s32 zero = 0; /* FAKEMATCH: ⚓️ builds the zero in r2 and the counter address in r3 */

        gData_02002b34 = zero;
        Scheduler_AddOrUpdateCallback(Func_020009e8, 1152);
    } else {
        Dma_Set(gData_02002bbc, (void *)0x05000184, 0x84000002, REG_DMA3);
    }
}
