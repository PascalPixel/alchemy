/* Draft of overlay 69a Func_02000408, the twin of overlay 699's
 * Func_02000560 (lightset.c). differ 2 of 38: the listing loads the ground
 * height ([r5, #20]) before it stores the zero into [r5, #68]; this draft
 * stores first. The 699 twin orders them with an asm barrier; plain
 * statement order and a pointer-typed field did not move the load. */
#include "types.h"
#include "field_event.h"

#define BYTE_AT(base, offset) (*(u8 *)((u8 *)(base) + (offset)))
#define WORD_AT(base, offset) (*(s32 *)((u8 *)(base) + (offset)))

struct FieldActor *Object_GetById();
s32 Map_GetTerrainHeight();
void Func_02001b14();
void Func_02001bc4();

void Func_02000408(s32 id)
{
    struct FieldActor *object;
    s32 height;

    object = Object_GetById(id);
    object->motion_flags = 4;
    height = object->terrain_height;
    WORD_AT(object, 68) = 0;
    object->y = height + 0x200000;
    {
        u8 *flags = &object->priority_flags;
        s32 bit = 128;

        *flags = bit | *flags;
    }
    height = Map_GetTerrainHeight(BYTE_AT(object, 34), object->x, object->z);
    Func_02001b14(object->x, object->z, 0, (height >> 19) + 6);
    Func_02001bc4(object->x, object->z, 0, 128);
}
