#include "types.h"
#include "ram_buffer.h"
#include "field_event.h"
#include "iwram_call.h"
#include "dma.h"
#include "io_reg.h"

#define BYTE_AT(base, offset) (*(u8 *)((u8 *)(base) + (offset)))
#define WORD_AT(base, offset) (*(s32 *)((u8 *)(base) + (offset)))

extern s32 Data_02002194;
extern s32 Data_02002198;
extern u16 Data_020021c0[];
extern u16 Data_020021e0[];
extern u16 Data_020021e4[];

s32 GameFlag_Test();
void Scheduler_AddOrUpdateCallback();
struct FieldActor *Object_GetById();
s32 Map_GetTerrainHeight();
void Func_02001a6c();
void Func_02001b34();
void Func_02000238(void);

void Func_020002f4(s32 mode)
{
    Data_02002198 = 1;
    if (mode != 2) {
        Iwram_CopyWords(Data_020021c0, (void *)0x05000180, 32);
        Iwram_CopyWords(Data_020021e0, (void *)0x050001a0, 32);
    }
    if (GameFlag_Test(0xa00) || mode == 1) {
        s32 zero = 0; /* FAKEMATCH: ⚓️ builds the zero in r2 and the counter address in r3 */

        Data_02002194 = zero;
        Scheduler_AddOrUpdateCallback(Func_02000238, 1152);
    } else {
        Dma_Set(Data_020021e4, (void *)0x05000184, 0x84000002, REG_DMA3);
    }
}

void Func_02000378(s32 id)
{
    struct FieldActor *object;
    s32 height;

    /* FAKEMATCH: the one-pass block loads the ground height before the word is cleared; as plain statements the ARM9TDMI schedule clears it first (differ 2). */
    do {
        object = Object_GetById(id);
        object->motionFlags = 4;
        height = object->terrainHeight;
        WORD_AT(object, 68) = 0;
        object->y = height + 0x200000;
    } while (0);
    {
        u8 *flags = &object->priorityFlags;
        s32 bit = 128; /* FAKEMATCH: ⚓️ builds the bit in the register it ORs into */

        *flags = bit | *flags;
    }
    height = Map_GetTerrainHeight(BYTE_AT(object, 34), object->x, object->z);
    Func_02001a6c(object->x, object->z, 0, (height >> 19) + 6);
    Func_02001b34(object->x, object->z, 0, 128);
}
