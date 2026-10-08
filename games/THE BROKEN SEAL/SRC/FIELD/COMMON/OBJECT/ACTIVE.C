#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "EDITION.H"
#include "EVENT_RUNTIME.H"
#include "OBJECT_RUNTIME.H"
#include "TYPES.H"
#include "ANIMSPR.H"
#include "FIELDRUN.H"
#include "GAME_STATE.H"
#include "WINDOW.H"
#include "UI.H"

#define BattleFx_GetResourceId ((s32 (*)(s32))BattleFx_GetResourceId)

extern struct FieldStepWork *gEventWork;

void Event_SetMessageId(s16 value)
{
    gWork->message = value;
}

s32 ObjectTable_ReadActiveValue(s32 key)
{
    struct FieldStepWork *state = gEventWork;
    s32 result = -1;
    struct ObjectRuntime *entry = state->actors[key & 0x0fff];

    if (entry != 0 && entry->animation_kind == 1)
        result = ((struct AnimationObject *)entry->animation)->entries[0]->anim_id;
    return result;
}

s32 ObjectTable_FindActiveByValue(s32 value)
{
    struct FieldStepWork *state = gEventWork;
    s32 result = -1;
    s32 index = 8;
    struct ObjectRuntime *object = state->actors[index];

    if (object != 0 && object->animation_kind == 1 && ((struct AnimationObject *)object->animation)->entries[0]->anim_id == value) {
        result = index;
    } else {
    next:
        index++;
        if (index <= 65) {
            object = state->actors[index];
            if (object == 0 || object->animation_kind != 1 ||
                ((struct AnimationObject *)object->animation)->entries[0]->anim_id != value)
                goto next;
            result = index;
        }
    }
    return result;
}

void WaitFrames(s32 frames);

struct UiWindow *UiText_OpenMessageAtObject(s32 arg)
{
    struct UiRenderWork *win;
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
    struct UiWindow *handle;
    s32 side;
    s32 flags;
    s32 message;
    struct ObjectRuntime *object;
    s32 x;
    s32 y;
    s32 ret;
    s32 column;
    s32 none;

    win = (struct UiRenderWork *)gWindowWork[0];
    work = ((struct WorkPointers *)gWindowWork)->event;
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
                Render_ProjectPoint(&object->x, pos);
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
                Render_ProjectPoint(&object->x, pos);
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
            UiText_GetResourceDimensionsAltFar(message, &left, &top, (u32 *)&width, (u32 *)&height);
            left = x - width / 2;
            if (flags & 0x4000)
                top = y - height - 1;
            else if (!(flags & 0x8000) && y <= 8)
                top = y - height - 1;
            else
                top = y + 4;
        }
        if (win->mode != 0)
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
#if EDITION_INTERNATIONAL
        ret = Localization_LookupEntryIdFar(speaker);
        /* FAKEMATCH: needed to match: the -1 local preserves its register across measurement and reloads it afterward. */
        none = -1;
        if (ret != none) {
            UiText_GetResourceDimensionsAltFar(message, &left, &top, (u32 *)&width, (u32 *)&height);
            message = none;
            if (top > y)
                tail = top - 5;
            else
                tail = top + height;
            if (tail < 0)
                tail = top + height;
            else if (tail + 5 > 19)
                tail = top - 5;
            if (top < tail) {
                s32 lines = height;

                UiText_GetResourceDimensionsFar(none, &left, &top, (u32 *)&width, (u32 *)&height);
                message = none;
                extra = lines - height + 1;
            }
        } else if (top < y) {
            s32 lines = height;

            UiText_GetResourceDimensionsFar(message, &left, &top, (u32 *)&width, (u32 *)&height);
            extra = lines - height + 1;
            message = ret;
        }
#else
        none = Localization_LookupEntryIdFar(speaker);
        ret = -1;
        if (none != ret) {
            UiText_GetResourceDimensionsAltFar(message, &left, &top, (u32 *)&width, (u32 *)&height);
            message = ret;
            if (top > y)
                tail = top - 5;
            else
                tail = top + height;
            if (tail < 0)
                tail = top + height;
            else if (tail + 5 > 19)
                tail = top - 5;
        }
        if (top < y) {
            s32 lines = height;

            UiText_GetResourceDimensionsFar(message, &left, &top, (u32 *)&width, (u32 *)&height);
            message = -1;
            extra = lines - height + 1;
        }
#endif
        if (column < 0)
            column = 0;
        else if (column + margin > 29)
            column = 29 - margin;
        if (win->mode != 0) {
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
        if (win->mode != 0)
            side = UiWindow_CreateWithSideObjectFar(speaker, 0, column, tail);
        else
            side = UiWindow_CreateWithSideObjectFar(speaker, 0, column, tail);
        while (!UiWork_IsCompleteFar())
            WaitFrames(1);
    }
    work->message_window = (s32)handle;
    work->side_window = side;
    work->message++;
    return handle;
}
