/* Draft, differ 34 of 93: the logic is right. Left: the native picks object 15 or 14 with a branch
 * (movs r0, #15; b; movs r0, #14) where this form computes it without one; it reloads the party
 * state's address from the pool for the last Object_GetById where this form reuses r7 (the
 * forwarding inline does not separate it); and three rows around split constants are scheduled the
 * other way round. */
#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "party_state.h"

#define BYTE_AT(base, offset) (*(u8 *)((u8 *)(base) + (offset)))

struct ObjectRuntime *Object_GetById(s32 id);
s32 Func_02000620(s32 *a, s32 *b);
s32 Func_02000658(struct ObjectRuntime *object, struct ObjectRuntime *other, s32 range);
void Func_02003fdc(struct ObjectRuntime *object, s32 mode);
void Func_0200402c(struct ObjectRuntime *object, s32 value);
s32 Func_02004254(struct ObjectRuntime *object);

static __inline__ struct ObjectRuntime *Object_GetById_(s32 id)
{
    /* FAKEMATCH: forwarding through the inline keeps this call's own load of the party state, as ☀️'s scene layer does. */
    return Object_GetById(id);
}

s32 Func_020006d0(struct ObjectRuntime *object)
{
    u8 *event = Ram_HeapSlots->event_work;
    u8 *seen;
    u32 mode;

    Func_0200402c(object, Func_02004254(Object_GetById(gPartyState.current_owner)));
    mode = *(u16 *)(event + 358) & 0x3ff;
    seen = &BYTE_AT(object, 91);
    if ((mode != 141 || *(s8 *)((u8 *)Ram_HeapSlots->effect_work + 0x713) == 0)
        && BYTE_AT(object, 99) != 1) {
        if (Func_02000620(&object->x, &Object_GetById(gPartyState.current_owner)->x) > 11) {
            s32 id;

            if (BYTE_AT(object, 98) == 1)
                id = 15;
            else
                id = 14;
            if (Func_02000658(object, Object_GetById(id), 18) != 0)
                goto on;
            if (Func_02000658(object, Object_GetById_(gPartyState.current_owner), 26) != 0)
                goto on;
        }
        *seen = 0;
        Func_02003fdc(object, 2);
        return 0;
    }
on:
    *seen = 1;
    Func_02003fdc(object, 1);
    return 0;
}
