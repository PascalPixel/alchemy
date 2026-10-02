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

#if EDITION_INTERNATIONAL
/* Opens the current message beside the object the low twelve bits of arg
   name, with the speaker's side window; the top four bits place the
   windows. The Japanese edition measures the message differently and is not
   C yet. */
s32 UiText_OpenMessageAtObject(s32 arg)
{
    struct MessageWindowWork *win;
    struct EventRuntime *work;
    s32 speaker;
    s32 extra;
    s32 tail;
    s32 margin;
    s32 height;
    s32 width;
    s32 top;
    s32 left;
    s32 pos[3];
    s32 handle;
    s32 side;
    s32 flags;
    s32 message;
    u8 *object;
    s32 x;
    s32 y;
    s32 ret;
    s32 column;
    s32 none;

    win = (struct MessageWindowWork *)gWindowWork.window;
    work = gWindowWork.event;
    handle = 0;
    side = 0;
    speaker = ObjectTable_ReadActiveValue(arg);
    extra = 0;
    tail = 0;
    margin = 4;
    flags = arg & 0xf000;
    message = work->message;
    arg &= 0xfff;
    x = 0;
    object = ObjectTable_Get(arg);
    work->speaker = arg;
    y = 0;
    ret = 0;
    if (work->message_busy == 0) {
        if (object != 0) {
            if (work->mode_19e == 3) {
                Render_ProjectPoint((s32 *)(object + 8), pos);
                x = pos[0] >> 3;
                y = pos[1] >> 3;
                ret = 1;
                y -= 2;
            } else {
                if (Object_GetScreenPosition(arg, pos) != -1)
                    ret = 1;
                else
                    ret = 0;
                x = pos[0] >> 3;
                y = pos[1] >> 3;
            }
        } else if (arg <= 7) {
            speaker = arg;
            object = ObjectTable_Get(gGameState.selected_actor);
            if (work->mode_19e == 3) {
                Render_ProjectPoint((s32 *)(object + 8), pos);
                x = pos[0] >> 3;
                y = pos[1] >> 3;
                ret = 1;
            } else {
                if (Object_GetScreenPosition(gGameState.selected_actor, pos) != -1)
                    ret = 1;
                else
                    ret = 0;
                x = pos[0] >> 3;
                y = pos[1] >> 3;
            }
        }
        if (!ret) {
            left = 15;
            top = 10;
        } else {
            left = 0;
            top = 0;
            UiText_GetResourceDimensionsAltFar(message, &left, &top, &width, &height);
            left = x - width / 2;
            if (flags & 0x4000)
                top = y - height - 1;
            else if (!(flags & 0x8000) && y <= 8)
                top = y - height - 1;
            else
                top = y + 4;
        }
        if (win->plain != 0)
            margin = 5;
        column = x;
        if (flags & 0x1000) {
            column = x - margin - 2;
            if (column < 0)
                column = 0;
        } else if (flags & 0x2000) {
            column += 2;
            if (column + margin > 29)
                column = 29 - margin;
        } else if (column <= 15) {
            column = column - margin - 2;
            if (column < 0)
                column = x + 2;
        } else {
            column += 2;
            if (column + margin > 29)
                column = x - margin - 2;
        }
        ret = Localization_LookupEntryIdFar(speaker);
        /* FAKEMATCH: the ROM keeps -1 in a register of its own across the
           measuring call and loads it again in the block after; a variable
           holding it gives both. */
        none = -1;
        if (ret != none) {
            UiText_GetResourceDimensionsAltFar(message, &left, &top, &width, &height);
            message = none;
            tail = top - 5;
            if (top <= y)
                tail = top + height;
            if (tail < 0) {
                tail = top + height;
            } else if (tail + 5 > 19) {
                /* FAKEMATCH: written as top - 5 the compiler shares the
                   first top - 5 through a register; the ROM subtracts
                   again. */
                tail = top;
                tail -= 5;
            }
            if (top < tail) {
                s32 lines = height;

                UiText_GetResourceDimensionsFar(none, &left, &top, &width, &height);
                message = none;
                extra = lines - height + 1;
            }
        } else if (top < y) {
            s32 lines = height;

            UiText_GetResourceDimensionsFar(message, &left, &top, &width, &height);
            extra = lines - height + 1;
            message = ret;
        }
        if (column < 0)
            column = 0;
        else if (column + margin > 29)
            column = 29 - margin;
        if (win->plain != 0) {
            WaitFrames(8);
            if (extra != 0)
                handle = UiText_OpenMessageWindowFar(message, left, top + extra - 1, 18);
            else
                handle = UiText_OpenMessageWindowFar(message, left, top, 2);
        } else {
            s32 icon = BattleFx_GetResourceId(speaker);

            if (extra != 0)
                handle = UiText_OpenMessageWindowFar(message, left, top + extra - 1,
                                                     (icon << 16) | 17);
            else
                handle = UiText_OpenMessageWindowFar(message, left, top, (icon << 16) | 1);
        }
        /* Both arms make the same call; the ROM still reads the flag. */
        if (win->plain != 0)
            side = UiWindow_CreateWithSideObjectFar(speaker, 0, column, tail);
        else
            side = UiWindow_CreateWithSideObjectFar(speaker, 0, column, tail);
        while (!UiWork_IsCompleteFar())
            WaitFrames(1);
    }
    work->message_window = handle;
    work->side_window = side;
    work->message++;
    return handle;
}
#endif
