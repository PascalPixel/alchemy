/* NONMATCHING: whole 276-byte owner including its six-word pool;
 * candidate 280 bytes, 110 differing halfwords / 31 aligned edits.
 * The shared null window recovers the initial zero's cross-call lifetime.
 * UiText_QueueRenderEntries copies four halfwords from its fifth argument:
 * bounds is therefore eight bytes, not the old four-byte scalar. This
 * restores the 36-byte frame. Reusing packed after extracting the cursor
 * restores r5 options, r8 work, sl entry and the literal-pool contents.
 * UiWindow_FitOnScreen is void, with unsigned width/height and an integer
 * mode carrying the bounds pointer; auditing its prototype changed no bytes.
 * Remaining: x's spill is late, the bounds address is formed early with an
 * extra move, and flags/window use r6/r7 instead of r7/r6. Three bounded
 * lifetime/layout hypotheses used (2026-09-26); no adoption or credit. */
#include "RENDER_INPUT.H"

struct MessageWindowWork {
    u8 unknown_000[0xeb0];
    u16 entries[(0x12f4 - 0xeb0) / 2];
    u16 cursor;
    u16 scroll;
    u16 unknown_12f8;
    u8 busy;
    u8 pending;
};

extern struct MessageWindowWork *Data_03001e8c;
s32 UiText_BuildRenderEntries(s32 message, s32 mode);
void UiWindow_FitOnScreen(s32, s32 *, s32 *, u32 *, u32 *, s32, u32);
struct RenderInput *UiWindow_Create(s32, s32, s32, s32, s32);
s32 UiText_QueueRenderEntries(struct RenderInput *, s32, s32, s32, const u16 *, s32);
void UiWork_Finalize(struct RenderInput *, s32);

struct RenderInput *Func_08017658(s32 message, s32 x, s32 y, u32 packed)
{
    s32 entry;
    u32 height;
    u32 width;
    u16 bounds[4];
    s32 flags;
    u16 *out;
    struct RenderInput *window;
    struct MessageWindowWork *work;

    work = Data_03001e8c;
    work->cursor = (packed << 4) >> 20;
    /* FAKEMATCH: retain the null window as the scroll and layout zero. */
    window = NULL;
    work->scroll = (u32)window;
    packed &= 0xffff;
    entry = UiText_BuildRenderEntries(message, 1);
    flags = 0;
    if (work->entries[entry] == 0)
        return NULL;
    out = bounds;
    UiWindow_FitOnScreen(entry, &x, &y, &width, &height, (s32)out, (u32)window);
    if (width == 0 && height == 0)
        return NULL;
    if (!(packed & 1))
        flags |= 2;
    if (packed & 8)
        flags |= 8;
    if (packed & 16)
        flags |= 128;
    if (packed & 32)
        flags |= 256;
    window = UiWindow_Create(x, y, width, height, flags);
    if (window == NULL)
        return NULL;
    if (UiText_QueueRenderEntries(window, entry, 0, 0, out, 0) == 0) {
        UiWork_Finalize(window, 1);
        return NULL;
    }
    work->busy = 0;
    work->pending = 0;
    return window;
}
