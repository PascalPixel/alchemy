/* Near miss: listing 45 instructions, draft 45, common 39, differ 12; the rest differs in instruction order. */
#include "types.h"
#include "ram_buffer.h"

void Runtime_SetWorkTripleIfNonNegativeFar(s32 a, s32 b, s32 c);
void Object_AttachWorkTargetToObject(s32 id, s32 mode);

void Func_080e8c9c(void)
{
    u8 *map = (u8 *)Ram_HeapSlots->map_work;
    u8 *effect = Ram_HeapSlots->effect_work;
    u8 *fx = Ram_HeapSlots->battle_fx_work[0];
    u8 *event = Ram_HeapSlots->event_work;

    if (*(s32 *)(map + 252) != 0 || *(s8 *)(event + 394) == 3) {
        Runtime_SetWorkTripleIfNonNegativeFar(0, 0, 0);
    } else {
        *(s32 *)(map + 236) = *(s32 *)(fx + 12);
        *(s32 *)(map + 244) = *(s32 *)(fx + 16);
        *(s32 *)(map + 240) = *(s32 *)(fx + 20);
        *(s32 *)(map + 248) = *(s32 *)(fx + 24);
        Object_AttachWorkTargetToObject(*(s16 *)(effect + 24), 1);
    }
}
