#include "types.h"
#include "ram_buffer.h"
#include "field_event.h"
#include "iwram_call.h"
#include "dma.h"
#include "io_reg.h"

#define BYTE_AT(base, offset) (*(u8 *)((u8 *)(base) + (offset)))
#define WORD_AT(base, offset) (*(s32 *)((u8 *)(base) + (offset)))

extern s32 Data_02002cc0;
extern s32 Data_02002cc4;
extern u16 Data_02002cec[];
extern u16 Data_02002d0c[];
extern u16 Data_02002d10[];

s32 GameFlag_Test();
void Scheduler_AddOrUpdateCallback();
struct FieldActor *Object_GetById();
s32 Map_GetTerrainHeight();
void Func_02002368();
void Func_02002460();
void Func_02000404(void);

void Func_020004c0(s32 mode)
{
    Data_02002cc4 = 1;
    if (mode != 2) {
        Iwram_CopyWords(Data_02002cec, (void *)0x05000180, 32);
        Iwram_CopyWords(Data_02002d0c, (void *)0x050001a0, 32);
    }
    if (GameFlag_Test(0xa00) || mode == 1) {
        s32 zero = 0; /* FAKEMATCH: ⚓️ builds the zero in r2 and the counter address in r3 */

        Data_02002cc0 = zero;
        Scheduler_AddOrUpdateCallback(Func_02000404, 1152);
    } else {
        Dma_Set(Data_02002d10, (void *)0x05000184, 0x84000002, REG_DMA3);
    }
}

void Func_02000544(s32 id)
{
    struct FieldActor *object;
    s32 height;

    object = Object_GetById(id);
    object->motion_flags = 4;
    height = object->terrain_height;
    asm volatile("" : "+l"(height)); /* FAKEMATCH: ⚓️ loads the ground height before it clears the word */
    WORD_AT(object, 68) = 0;
    object->y = height + 0x200000;
    {
        u8 *flags = &object->priority_flags;
        s32 bit = 128; /* FAKEMATCH: ⚓️ builds the bit in the register it ORs into */

        *flags = bit | *flags;
    }
    height = Map_GetTerrainHeight(BYTE_AT(object, 34), object->x, object->z);
    Func_02002368(object->x, object->z, 0, (height >> 19) + 6);
    Func_02002460(object->x, object->z, 0, 128);
}
