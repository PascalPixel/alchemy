/*
 * Draft of Func_02003ac0 (overlay 6A3, 0200bac0), the twin of overlay 699's
 * Func_02000fd8 (games/tla/src/field/overlay_699/lightcue.c).
 * With 699's empty asm barrier after the timer store it matches (differ 0).
 * Not adopted: the barrier would raise ⚓️'s asm-barrier count over the
 * ratchet (alchemy check devices). Without it, 6 instructions differ: the
 * count's address is loaded before the timer store and the list's address
 * after the count is read. The honest way in is one shared module for the
 * five overlays that carry this light library (699, 69D, 69E, 6A3, 6A4),
 * which needs their data labels given one name each.
 */
#include "types.h"
#include "field_event.h"

extern s16 Data_02005a82;
extern struct FieldActor *Data_02005a84[];

struct FieldActor *Object_GetById(s32 id);

void Func_02003ac0(s32 id, s32 value)
{
    struct FieldActor *object = Object_GetById(id);

    if (object != NULL) {
        object->timer = value;
        Data_02005a84[Data_02005a82] = object;
        Data_02005a82++;
    }
}
