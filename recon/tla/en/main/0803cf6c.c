/* Near miss: differ 4, one swapped load. The listing loads the entry count
   (ldrh) before building 0xffff in r4 and loading the 0x1ff mask from the
   pool; this build builds the first byte of 0xffff ahead of the load and
   the mask after it. The (u16) cast is what gives the 0xffff add and the
   pooled mask, and the branch over the pool to the return. Two separate
   statements, one nested expression, a u32 or u16 local copy, a loop of
   two, a returned call and an s32 mask local leave 4 or more; the
   scheduling-barrier search finds nothing. It goes between
   UiText_BuildRenderEntriesMode1 (BUILD_MODE1.C) and UiText_PlotGlyphRows
   (PLOT_GLYPH.C). */
#include "TYPES.H"
#include "WINDOW.H"
#include "RAM_BUFFER.H"

s32 UiText_BuildRenderEntries(s32 message, s32 mode);

/* Steps the text entry ring back two entries and appends message there. */
void Func_0803cf6c(s32 message)
{
    struct UiRenderWork *work = (struct UiRenderWork *)Ram_HeapSlots->window_tiles;

    work->count = (u16)(work->count - 1) & (UI_TEXT_ENTRY_COUNT - 1);
    work->count = (u16)(work->count - 1) & (UI_TEXT_ENTRY_COUNT - 1);
    UiText_BuildRenderEntries(message, 1);
}
