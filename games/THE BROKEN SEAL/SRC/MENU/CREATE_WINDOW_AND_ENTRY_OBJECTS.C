#include "A8_STATE.H"
#include "TYPES.H"
#include "GLOBAL_CELLS.H"

#define FIELD_AT_OFFSET(base, type, offset) (*(type)((u8 *)(base) + (offset)))

typedef struct {
    u8 padding[15];
    u8 field_0f;
} Object0f;

void *SideObject_CreateFar(s32, s32, s32, s32, s32, s32);
void CharacterMenu_DrawStatusAilments(s32, s32, s32);
s32 Menu_CreateEightEntryObjects(s32 resource);

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
        created = UiWindow_UpdateOrCreate(&state->handle, 0, 5, 0x1E, 0xF, 2);
        handle = state->handle;
    }
    if (created != 0) {
        object = SideObject_CreateFar(resource, 0, 0, handle, 0, 0);
        state->object = object;
        if ((((Object0f *)object)->field_0f = 0xF0, state->mode) == 3) {
            Menu_SpawnIconEntries(state, handle);
        }
        Menu_CreateEightEntryObjects(handle);
        CharacterMenu_DrawStatusAilments(handle, resource, 0x100);
        return;
    }
    CharacterMenu_DrawStatusAilments(handle, resource, 0);
}
