#include "layout_guard.h"
#include "types.h"

typedef struct ActorState_080b1470 {
    u8 padding00[0xd8];
    u16 slots[15];
} ActorState_080b1470;

typedef struct DisplayObject_080b1470 {
    u8 padding00[0x0f];
    u8 style;
} DisplayObject_080b1470;

LAYOUT_OFFSET_GUARD(
    ActorState080b1470_Slots,
    ActorState_080b1470,
    slots,
    0xd8);
LAYOUT_OFFSET_GUARD(
    DisplayObject080b1470_Style,
    DisplayObject_080b1470,
    style,
    0x0f);

void Func_08015060(s32);
void Func_08015080(s32, s32, s32, s32);
void Func_08015120(s32, s32);
DisplayObject_080b1470 *Func_080152d0(
    u16,
    s32,
    s32,
    s32,
    s32);
ActorState_080b1470 *Func_08077008(s32);
s32 Func_08077038(s32, s32);

/*
 * Render an actor's contiguous inventory as three rows of five entries and
 * show whether the requested item already occupies one of those slots.
 */
void Func_080b1470(s32 window, s32 actor, s32 item_id)
{
    ActorState_080b1470 *actor_state = Func_08077008(actor);
    s32 x = 8;
    s32 y = 8;
    s32 slot;
    s32 i;

    if (window == 0)
        return;

    Func_08015060(window);
    slot = Func_08077038(actor, item_id);
    if (slot != -1) {
        Func_08015120((actor_state->slots[slot] >> 11) + 1, 5);
        Func_08015080(0x0c90, window, 0, 0);
    } else {
        Func_08015080(0x0c8f, window, 0, 0);
    }

    for (i = 0; i < 15 && actor_state->slots[i] != 0; i++) {
        DisplayObject_080b1470 *object = Func_080152d0(
            actor_state->slots[i],
            27,
            window,
            x,
            y);
        object->style = 0xfc;
        x += 16;
        if (i == 4) {
            x = 8;
            y += 16;
        }
        if (i == 9) {
            x = 8;
            y += 16;
        }
    }
}
