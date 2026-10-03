/* UiText_OpenMessageAtObject: plain international near miss for raw08092c40.
   Native complete836 includes its16-byte pool. Prior ordinary EN body828
   has120 complete differing positions, including eight missing bytes; calls
   and pools are symbolically linked, without address substitutions.
   Trial: the identical-call branch matched836 but was rejected under S2
   because it forced an unused four-instruction flag reload. Removing it
   removes that reload and exchanges flags fp/r9 and side-handle r9/fp.
   Two existing measuring/subtract steering devices remain tagged below.
   Other editions were not newly proved by that English trial.
   Ordinary canonical-owner/pointer attempt: not yet measured. */
#include "EDITION.H"
#include "EVENT_RUNTIME.H"
#include "GAME_STATE.H"
#include "OBJECT_RUNTIME.H"
#include "WINDOW.H"
#include "TYPES.H"

s32 ObjectTable_ReadActiveValue(s32 key);
s32 Render_ProjectPoint(s32 *point, s32 *screen);
s32 Object_GetScreenPosition(s32 id, s32 *screen);
s32 UiText_GetResourceDimensionsAltFar(s32 message, s32 *x, s32 *y, u32 *width, u32 *height);
s32 UiText_GetResourceDimensionsFar(s32 message, s32 *x, s32 *y, u32 *width, u32 *height);
s32 Localization_LookupEntryIdFar(u32 speaker);
void WaitFrames(s32 frames);
s32 BattleFx_GetResourceId(s32 speaker);
struct UiWindow *UiText_OpenMessageWindowFar(s32 message, s32 x, s32 y, u32 style);
s32 UiWindow_CreateWithSideObjectFar(s32 speaker, s32 mode, s32 x, s32 y);
s32 UiWork_IsCompleteFar(void);

/* Low twelve bits name the actor; high bits place its message and portrait. */
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
    work = gWork;
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
        ret = Localization_LookupEntryIdFar(speaker);
        /* FAKEMATCH: the ROM keeps -1 in a register of its own across the
           measuring call and loads it again in the block after; a variable
           holding it gives both. */
        none = -1;
        if (ret != none) {
            UiText_GetResourceDimensionsAltFar(message, &left, &top, (u32 *)&width, (u32 *)&height);
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
        side = UiWindow_CreateWithSideObjectFar(speaker, 0, column, tail);
        while (!UiWork_IsCompleteFar())
            WaitFrames(1);
    }
    work->message_window = (s32)handle;
    work->side_window = side;
    work->message++;
    return handle;
}
