#include "layout_guard.h"
#include "types.h"

typedef struct DisplayObject_080a2324 {
    u8 padding00[5];
    s8 state;
    s16 x;
    s16 y;
} DisplayObject_080a2324;

typedef struct MenuState_080a2324 {
    u8 padding000[0x48];
    DisplayObject_080a2324 *objects[32];
    u8 padding0c8[0x150];
    u8 entry_count;
} MenuState_080a2324;

LAYOUT_OFFSET_GUARD(
    DisplayObject080a2324_State,
    DisplayObject_080a2324,
    state,
    5);
LAYOUT_OFFSET_GUARD(
    DisplayObject080a2324_X,
    DisplayObject_080a2324,
    x,
    6);
LAYOUT_OFFSET_GUARD(
    MenuState080a2324_Objects,
    MenuState_080a2324,
    objects,
    0x48);
LAYOUT_OFFSET_GUARD(
    MenuState080a2324_EntryCount,
    MenuState_080a2324,
    entry_count,
    0x218);

extern MenuState_080a2324 *Data_03001f2c;

void Func_080a17c4(DisplayObject_080a2324 *object);

/* Hide all row objects, then position the visible slice of one menu page. */
void Func_080a2324(
    s32 page_size,
    s32 first_entry,
    s32 unused_window,
    s32 x,
    s32 y)
{
    MenuState_080a2324 *state = Data_03001f2c;
    s32 end = first_entry + page_size;
    s32 index;

    (void)unused_window;

    for (index = 0; index < 32; index++) {
        if (state->objects[index] != 0)
            state->objects[index]->state = 13;
    }

    for (index = first_entry;
         index < end && index <= state->entry_count - 1;
         index++, y += 16) {
        DisplayObject_080a2324 *object = state->objects[index];

        if (object == 0)
            break;

        object->x = (s16)x;
        object->y = (s16)y;
        Func_080a17c4(object);
        object->state = 1;
    }
}
