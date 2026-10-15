#include "types.h"
#include "ram_buffer.h"
#include "field_event.h"

#define BYTE_AT(base, offset) (*(u8 *)((u8 *)(base) + (offset)))
#define WORD_AT(base, offset) (*(s32 *)((u8 *)(base) + (offset)))

struct FieldActor *Object_GetById();
s32                Map_GetTerrainHeight();
void               Func_02002b54();
void               Func_02002c44();

void Func_020012a8(s32 id)
{
    struct FieldActor *object;
    s32                height;

    object = Object_GetById(id);
    object->motionFlags = 4;
    height = object->terrainHeight;
    /* FAKEMATCH: ⚓️ loads the ground height before it clears the word */
    asm volatile("" : "+l"(height));
    WORD_AT(object, 68) = 0;
    object->y = height + 0x200000;
    {
        u8 *flags = &object->priorityFlags;
        s32 bit = 128; /* FAKEMATCH: ⚓️ builds the bit in the register it ORs into */

        *flags = bit | *flags;
    }
    height = Map_GetTerrainHeight(BYTE_AT(object, 34), object->x, object->z);
    Func_02002b54(object->x, object->z, 0, (height >> 19) + 6);
    Func_02002c44(object->x, object->z, 0, 128);
}
