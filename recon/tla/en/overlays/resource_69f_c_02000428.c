// near miss: differ 2 (listing 38 instructions, draft 38); overlay 699's twin (Func_02000560) holds this order with an asm barrier; with an empty loop the height load still precedes the zero
#include "types.h"
#include "ram_buffer.h"
#include "field_event.h"
#include "iwram_call.h"
#include "dma.h"
#include "io_reg.h"

#define BYTE_AT(base, offset) (*(u8 *)((u8 *)(base) + (offset)))
#define WORD_AT(base, offset) (*(s32 *)((u8 *)(base) + (offset)))

extern s32 Data_020023bc;
extern s32 Data_020023c0;
extern u16 Data_020023e8[];
extern u16 Data_02002408[];
extern u16 Data_0200240c[];

s32 GameFlag_Test();
void Scheduler_AddOrUpdateCallback();
struct FieldActor *Object_GetById();
s32 Map_GetTerrainHeight();
void Func_02001b34();
void Func_02001be4();
void Func_020002e8(void);

void Func_02000428(s32 id)
{
    struct FieldActor *object;
    s32 height;

    object = Object_GetById(id);
    object->motion_flags = 4;
    WORD_AT(object, 68) = 0;
    height = object->terrain_height;
    do { } while (0); /* FAKEMATCH: the loop note ends the scheduling block: ⚓️ loads the ground height before it builds the offset (overlay 699's twin holds the same order with an asm barrier). */
    object->y = height + 0x200000;
    {
        u8 *flags = &object->priority_flags;
        s32 bit = 128; /* FAKEMATCH: ⚓️ builds the bit in the register it ORs into */

        *flags = bit | *flags;
    }
    height = Map_GetTerrainHeight(BYTE_AT(object, 34), object->x, object->z);
    Func_02001b34(object->x, object->z, 0, (height >> 19) + 6);
    Func_02001be4(object->x, object->z, 0, 128);
}
