/*
 * Draft of Func_02001dbc (overlay 6A4, 02009dbc), the twin of overlay 699's
 * Func_02000fd8 (games/tla/src/field/overlay_699/lightcue.c).
 * With 699's empty asm barrier after the timer store it matches (differ 0).
 * Not adopted: the barrier would raise ⚓️'s asm-barrier count over the
 * ratchet (alchemy check devices). Without it, 6 instructions differ: the
 * count's address is loaded before the timer store and the list's address
 * after the count is read. See resource_6a3_c_0200bac0.c.
 */
#include "types.h"
#include "field_event.h"

extern s16 Data_02003382;
extern struct FieldActor *Data_02003384[];

struct FieldActor *Object_GetById(s32 id);

void Func_02001dbc(s32 id, s32 value)
{
    struct FieldActor *object = Object_GetById(id);

    if (object != NULL) {
        object->timer = value;
        Data_02003384[Data_02003382] = object;
        Data_02003382++;
    }
}
