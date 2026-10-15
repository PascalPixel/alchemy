#include "types.h"
#include "ram_buffer.h"
#include "field_event.h"
#include "iwram_call.h"
#include "dma.h"
#include "io_reg.h"

#define BYTE_AT(base, offset) (*(u8 *)((u8 *)(base) + (offset)))
#define WORD_AT(base, offset) (*(s32 *)((u8 *)(base) + (offset)))

extern s32 gData_0200703c;
extern s32 gData_02007040;
extern u16 gData_02007068[];
extern u16 gData_02007088[];
extern u16 gData_0200708c[];

s32                GameFlag_Test();
void               Scheduler_AddOrUpdateCallback();
struct FieldActor *Object_GetById();
s32                Map_GetTerrainHeight();
void               Func_02005540();
void               Func_020056a8();
void               Func_020014e8(void);

void Func_020015a4(s32 mode)
{
    gData_02007040 = 1;
    if (mode != 2)
    {
        Iwram_CopyWords(gData_02007068, (void *)0x05000180, 32);
        Iwram_CopyWords(gData_02007088, (void *)0x050001a0, 32);
    }
    if (GameFlag_Test(0xa00) || mode == 1)
    {
        s32 zero = 0; /* FAKEMATCH: ⚓️ builds the zero in r2 and the counter address in r3 */

        gData_0200703c = zero;
        Scheduler_AddOrUpdateCallback(Func_020014e8, 1152);
    }
    else
    {
        Dma_Set(gData_0200708c, (void *)0x05000184, 0x84000002, REG_DMA3);
    }
}
