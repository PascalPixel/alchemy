#include "EDITION.H"
#include "EVENT_RUNTIME.H"
#include "GAME_STATE.H"
#include "TYPES.H"

extern struct EventRuntime *Data_03001ebc;
extern struct WorkPointers gWindowWork;

/* The window work's flag for a message shown without the speaker's icon. */
struct MessageWindowWork {
    u8 unknown_000[0xea4];
    u8 plain;
};

u8 *ObjectTable_Get(s32 id);
s32 Render_ProjectPoint(const s32 *point, s32 *screen);
s32 Object_GetScreenPosition(s32 id, s32 *screen);
void UiText_GetResourceDimensionsAltFar(s32 message, s32 *x, s32 *y, s32 *width, s32 *height);
void UiText_GetResourceDimensionsFar(s32 message, s32 *x, s32 *y, s32 *width, s32 *height);
s32 Localization_LookupEntryIdFar(s32 speaker);
void WaitFrames(s32 frames);
s32 BattleFx_GetResourceId(s32 speaker);
s32 UiText_OpenMessageWindowFar(s32 message, s32 x, s32 y, s32 style);
s32 UiWindow_CreateWithSideObjectFar(s32 speaker, s32 mode, s32 x, s32 y);
s32 UiWork_IsCompleteFar(void);

struct EventRuntime1d8 {
    u8 unknown_000[0x1d8];
    s16 value;
};

struct ObjectValueSource {
    u8 unknown_00[0x28];
    const s16 *value;
};

struct ObjectValueEntry {
    u8 unknown_00[0x50];
    struct ObjectValueSource *value_source;
    u8 active;
};

struct ObjectValueTable {
    u8 unknown_00[0x14];
    struct ObjectValueEntry *objects[4096];
};

extern struct ObjectValueTable *gEventWork;

void Event_SetValue1d8(s16 value)
{
    ((struct EventRuntime1d8 *)Data_03001ebc)->value = value;
}

s32 ObjectTable_ReadActiveValue(s32 key)
{
    s32 result = -1;
    struct ObjectValueEntry *entry =
        gEventWork->objects[(u32)key & 0x0fff];

    if (entry != 0 && entry->active == 1)
        result = *entry->value_source->value;
    return result;
}

s32 ObjectTable_FindActiveByValue(s32 value)
{
    struct ObjectValueTable *state = gEventWork;
    s32 result = -1;
    s32 index = 8;
    struct ObjectValueEntry *object = state->objects[index];

    if (object != 0 && object->active == 1 && *object->value_source->value == value) {
        result = index;
    } else {
    next:
        index++;
        if (index <= 65) {
            object = state->objects[index];
            if (object == 0 || object->active != 1 ||
                *object->value_source->value != value)
                goto next;
            result = index;
        }
    }
    return result;
}
