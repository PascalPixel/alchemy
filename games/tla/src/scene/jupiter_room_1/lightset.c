#include "types.h"
#include "ram_buffer.h"
#include "field_event.h"
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
struct FieldActor *Object_GetById();
s32 Map_GetTerrainHeight();
void Func_02001b14();
void Func_02001bc4();

#define BYTE_AT(base, offset) (*(u8 *)((u8 *)(base) + (offset)))
#define WORD_AT(base, offset) (*(s32 *)((u8 *)(base) + (offset)))

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

void Func_02000408(s32 id)
{
    struct FieldActor *object;
    s32 height;

    object = Object_GetById(id);
    object->motionFlags = 4;
    height = object->terrainHeight;
    asm volatile("" : "+l"(height)); /* FAKEMATCH: ⚓️ loads the ground height before it clears the word; without it the zero is stored first (2 differ); plain statement order and a pointer-typed field did not move the load */
    WORD_AT(object, 68) = 0;
    object->y = height + 0x200000;
    {
        u8 *flags = &object->priorityFlags;
        s32 bit = 128; /* FAKEMATCH: ⚓️ builds the bit in the register it ORs into */

        *flags = bit | *flags;
    }
    height = Map_GetTerrainHeight(BYTE_AT(object, 34), object->x, object->z);
    Func_02001b14(object->x, object->z, 0, (height >> 19) + 6);
    Func_02001bc4(object->x, object->z, 0, 128);
}
