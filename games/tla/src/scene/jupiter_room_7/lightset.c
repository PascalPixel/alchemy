#include "types.h"
#include "ram_buffer.h"
#include "field_event.h"
#include "iwram_call.h"
#include "dma.h"
#include "io_reg.h"

#define BYTE_AT(base, offset) (*(u8 *)((u8 *)(base) + (offset)))
#define WORD_AT(base, offset) (*(s32 *)((u8 *)(base) + (offset)))

extern s32 gData_02003590;
extern s32 gData_02003594;
extern u16 gData_020035bc[];
extern u16 gData_020035dc[];
extern u16 gData_020035e0[];

s32                GameFlag_Test();
void               Scheduler_AddOrUpdateCallback();
struct FieldActor *Object_GetById();
s32                Map_GetTerrainHeight();
void               Func_02002b54();
void               Func_02002c44();
void               Func_02001168(void);

void Func_02001224(s32 mode)
{
    gData_02003594 = 1;
    if (mode != 2)
    {
        Iwram_CopyWords(gData_020035bc, (void *)0x05000180, 32);
        Iwram_CopyWords(gData_020035dc, (void *)0x050001a0, 32);
    }
    if (GameFlag_Test(0xa00) || mode == 1)
    {
        s32 zero = 0; /* FAKEMATCH: ⚓️ builds the zero in r2 and the counter address in r3 */

        gData_02003590 = zero;
        Scheduler_AddOrUpdateCallback(Func_02001168, 1152);
    }
    else
    {
        Dma_Set(gData_020035e0, (void *)0x05000184, 0x84000002, REG_DMA3);
    }
}
