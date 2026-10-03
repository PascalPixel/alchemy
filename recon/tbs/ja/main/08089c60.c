/* DRAFT: Japanese UiText_OpenMessageAtObject at 08089c60 (780 bytes
   including pools); the five international editions remain drafts.
   2026-10-02 trial used RENDER_MODE_OFS for the Japanese work byte (0xf34).
   Prior trial with named call targets: the plain source
   scored 560 (7 register-only, 2 reordered, 2 inserted, 2 deleted).
   The prior tagged r1 top copy scored 340 (3 register-only, 2 reordered,
   1 inserted, 1 deleted): it loads top directly into r1 then reloads r2,
   where the ROM loads r2 and copies r1. Two dimension arguments and the
   final -1 are prepared in another order. Pinning the initial top to r2
   scored 625; exposing top before the copy scored 445. No exact match.
   Canonical-owner/pointer ordinary trial: 776 bytes for JA and 836 under
   international defines; JA weighted diagnostic 2366 (83 rows) against the
   native 780 bytes, with no unresolved calls. The identical side-window
   branch was removed instead of forcing a mode reload. Measured 64-slot
   window/event bank trial: 772 bytes against native 780; weighted diagnostic
   2165 (53 rows: 27 register-only, 3 operand, 9 reordered, 5 inserted,
   9 deleted), with no unresolved calls. Deleted mode/stack reloads and
   register/argument ordering remain. No exact match. */
#include "EDITION.H"
#include "EVENT_RUNTIME.H"
#include "GAME_STATE.H"
#include "HEAP_STATE.H"
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
    void **slots;
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
    s32 face;
    s32 column;
    s32 none;
    /* FAKEMATCH: the tail clamp retains the loaded top in r1; CSE otherwise shares top - 5 through ip. */
    register s32 base asm("r1");

    slots = &gWorkSlot[HEAP_SLOT_WINDOW];
    win = slots[0];
    work = slots[HEAP_SLOT_EVENT - HEAP_SLOT_WINDOW];
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
    face = 0;
    if (work->message_busy == 0) {
        if (object != 0) {
            if (work->mode_19e == 3) {
                Render_ProjectPoint(&object->x, pos);
                x = pos[0] >> 3;
                y = pos[1] >> 3;
                face = 1;
                y -= 2;
            } else {
                if (Object_GetScreenPosition(arg, pos) != -1) face = 1; else face = 0;
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
                face = 1;
            } else {
                if (Object_GetScreenPosition(gGameState.selected_actor, pos) != -1) face = 1; else face = 0;
                x = pos[0] >> 3;
                y = pos[1] >> 3;
            }
        }
        if (!face) {
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
#if !EDITION_INTERNATIONAL
        none = Localization_LookupEntryIdFar(speaker);
        face = -1;
        if (none != face) {
            UiText_GetResourceDimensionsAltFar(message, &left, &top, (u32 *)&width, (u32 *)&height);
            message = face;
            tail = top - 5;
            base = top;
            /* FAKEMATCH: keep the reference top copy in the register the clamp uses. */
            asm ("" : : "r" (base));
            if (top <= y)
                tail = top + height;
            if (tail < 0)
                tail = base + height;
            else if (tail + 5 > 19)
                tail = base - 5;
        }
        if (top < y) {
            s32 lines = height;
            UiText_GetResourceDimensionsFar(message, &left, &top, (u32 *)&width, (u32 *)&height);
            extra = lines - height + 1;
            message = -1;
        }
#else
        face = Localization_LookupEntryIdFar(speaker);
        none = -1;
        if (face != none) {
            UiText_GetResourceDimensionsAltFar(message, &left, &top, (u32 *)&width, (u32 *)&height);
            message = none;
            tail = top - 5;
            if (top <= y)
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
            message = face;
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
                handle = UiText_OpenMessageWindowFar(message, left, top + extra - 1, (icon << 16) | 17);
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
