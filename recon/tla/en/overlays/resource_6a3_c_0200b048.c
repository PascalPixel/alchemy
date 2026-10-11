/*
 * Draft of Func_02003048 (overlay 6A3, 0200b048), the twin of overlay 699's
 * Func_02000560 (games/tla/src/field/overlay_699/lightset.c).
 * With 699's asm barrier on the height it matches (differ 0). Not adopted:
 * the barrier would raise ⚓️'s asm-barrier count over the ratchet (alchemy
 * check devices). Without it, 2 instructions differ: the zero is stored at
 * 68 before the ground height is loaded, where ⚓️ loads the height first.
 * Storing the zero after the sum is worse (13 differ).
 */
#include "types.h"
#include "ram_buffer.h"
#include "field_event.h"

#define BYTE_AT(base, offset) (*(u8 *)((u8 *)(base) + (offset)))
#define WORD_AT(base, offset) (*(s32 *)((u8 *)(base) + (offset)))

struct FieldActor *Object_GetById();
s32 Map_GetTerrainHeight();
void Func_02004774();
void Func_020049b4();

void Func_02003048(s32 id)
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
    Func_02004774(object->x, object->z, 0, (height >> 19) + 6);
    Func_020049b4(object->x, object->z, 0, 128);
}
