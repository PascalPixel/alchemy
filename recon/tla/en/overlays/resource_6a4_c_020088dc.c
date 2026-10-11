/*
 * Draft of Func_020008dc (overlay 6A4, 020088dc), the twin of overlay 699's
 * Func_02000560 (games/tla/src/field/overlay_699/lightset.c).
 * With 699's asm barrier on the height it matches (differ 0). Not adopted:
 * the barrier would raise ⚓️'s asm-barrier count over the ratchet (alchemy
 * check devices). Without it, 2 instructions differ: the zero is stored at
 * 68 before the ground height is loaded, where ⚓️ loads the height first.
 * See resource_6a3_c_0200b048.c.
 */
#include "types.h"
#include "ram_buffer.h"
#include "field_event.h"

#define BYTE_AT(base, offset) (*(u8 *)((u8 *)(base) + (offset)))
#define WORD_AT(base, offset) (*(s32 *)((u8 *)(base) + (offset)))

struct FieldActor *Object_GetById();
s32 Map_GetTerrainHeight();
void Func_02002a78();
void Func_02002b58();

void Func_020008dc(s32 id)
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
        s32 bit = 128; /* FAKEMATCH: ⚓️ builds the bit in the register it ORs into */

        *flags = bit | *flags;
    }
    height = Map_GetTerrainHeight(BYTE_AT(object, 34), object->x, object->z);
    Func_02002a78(object->x, object->z, 0, (height >> 19) + 6);
    Func_02002b58(object->x, object->z, 0, 128);
}
