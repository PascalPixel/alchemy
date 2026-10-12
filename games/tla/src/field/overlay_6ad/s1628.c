#include "types.h"
#include "ram_buffer.h"
#include "field_event.h"
#include "iwram_call.h"
#include "dma.h"
#include "io_reg.h"

#define BYTE_AT(base, offset) (*(u8 *)((u8 *)(base) + (offset)))
#define WORD_AT(base, offset) (*(s32 *)((u8 *)(base) + (offset)))

extern s32 Data_0200703c;
extern s32 Data_02007040;
extern u16 Data_02007068[];
extern u16 Data_02007088[];
extern u16 Data_0200708c[];

s32 GameFlag_Test();
void Scheduler_AddOrUpdateCallback();
struct FieldActor *Object_GetById();
s32 Map_GetTerrainHeight();
void Func_02005540();
void Func_020056a8();
void Func_020014e8(void);

void Func_02001628(s32 id)
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
    Func_02005540(object->x, object->z, 0, (height >> 19) + 6);
    Func_020056a8(object->x, object->z, 0, 128);
}
