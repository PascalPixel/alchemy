#include "a8_state.h"
#include "types.h"
#include "scene.h"
#include "abi/menu/entry/create_window_and_entry_objects.h"
#include "global_cells.h"


typedef struct {
    u8 padding[15];
    u8 field_0f;
} Object0f;

void *Menu_Run(s32, s32, s32, s32, s32, s32);

void Menu_CreateWindowAndEntryObjects(s32 resource)
{
    s32 created;
    s32 handle;
    void *object;
    struct State080a8088 *state;

    state = *(struct State080a8088 **)ADDR_03001F2C;
    created = 0;
    handle = state->handle;
    if (handle == 0) {
        created = Menu_Run2(&state->handle, 0, 5, 0x1E, 0xF, 2);
        handle = state->handle;
    }
    if (created != 0) {
        object = Menu_Run(resource, 0, 0, handle, 0, 0);
        state->object = object;
        if ((((Object0f *)object)->field_0f = 0xF0, state->mode) == 3) {
            Menu_Run3(state, handle);
        }
        Menu_Check(handle);
        Menu_Place(handle, resource, 0x100);
        return;
    }
    Menu_Place(handle, resource, 0);
}
