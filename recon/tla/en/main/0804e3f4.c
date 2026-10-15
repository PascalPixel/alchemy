/* Near miss, differ 2 of 180 (one reordered pair), 2026-10-15: The Broken
 * Seal's DebugMenu_BrowseEntryGlyphs with its two devices (the count-derived
 * cancel mask, the slot offset before the loop and the loaded temporary).
 * The name list starts at message 4930 in English ("Master Poi"; 4916 in
 * Japanese, 5433 German, 5397 Spanish, 5390 French and Italian), a pool
 * symbol that the catalogs must name before adoption (MsgTestName here).
 * Remaining: the native loads gInput's address (ldr r6) before it copies the
 * total to sl; this copies first (equal priority, the copy has a dependent).
 * Five permuter minutes and operand-tied empty asms found nothing lower.
 */
#include "types.h"
#include "ram_buffer.h"
#include "window.h"
#include "input.h"
#include "system.h"
#include "uiflow.h"
#include "text_render_runtime.h"

extern s16 gData_0805eb58[];
extern s16 gData_0805eb7c[];

struct RenderInput *Func_0803d3c0(s32 x, s32 y, s32 width, s32 height);
struct RenderInput *UiWindow_Create(s32 x, s32 y, s32 width, s32 height, s32 style);
void RenderOutput_PrepareForRedraw(struct RenderInput *window);
void UiGlyph_LoadEntryWithPalette(u32 icon, s32 unused, s32 *slot, s32 *tile, s32 palette, s32 reuse);
void UiText_DrawNumberInWindow(s32 value, s32 width, struct RenderInput *output, s32 x, s32 y);

extern u8 MsgTestName[];

s32 Func_0804e3f4(void)
{
    struct UiRenderWork *work;
    struct RenderInput *portrait;
    s32 redraw;
    struct RenderInput *window;
    s32 count;
    s32 total;
    s32 index;
    s32 i;
    s32 glyph;
    s32 tile;
    s32 slot;
    s32 cancel;
    u32 slotOffset;

    work = (struct UiRenderWork *)Ram_HeapSlots->windowTiles;
    redraw = 1;
    portrait = Func_0803d3c0(0, 0, 10, 5);
    window = UiWindow_Create(10, 10, 14, 3, 2);
    index = 0;
    for (i = 0; gData_0805eb58[i * 2] != -1; i++) {
    }
    count = i;
    for (i = 0; gData_0805eb7c[i * 2] != -1; i++) {
    }
    total = count + i;
    cancel = ((u32)(u16)count >> 16) | 2;
    slotOffset = 0x1382;
    while (1) {
        if (gInput.repeat & 0x20) {
            redraw = 1;
            index--;
        }
        if (gInput.repeat & 0x10) {
            redraw = 1;
            index++;
        }
        if (gInput.repeat & 0x200) {
            redraw = 1;
            index -= 10;
        }
        if (gInput.repeat & 0x100) {
            redraw = 1;
            index += 10;
        }
        if (gInput.repeat & 1)
            break;
        if (gInput.repeat & cancel)
            break;
        if (redraw) {
            redraw = 0;
            index = (index + total) % total;
            RenderOutput_PrepareForRedraw(window);
            if (index < count)
                glyph = gData_0805eb58[index * 2 + 1];
            else
                glyph = gData_0805eb7c[(index - count) * 2 + 1] + 128;
            {
                s32 pal;
                s32 loaded = *(u16 *)((u8 *)work + slotOffset);
                pal = 15;
                slot = loaded;
                UiGlyph_LoadEntryWithPalette(glyph, 0, &slot, &tile, pal, 1);
            }
            UiText_DrawNumberInWindow(index, 2, window, 0, 0);
            UiText_DrawCharacterAtOffset(index + (s32)MsgTestName, window, 24, 0);
        }
        WaitFrames(1);
    }
    UiWork_Finalize((struct UiWindow *)window, 2);
    UiWork_Finalize((struct UiWindow *)portrait, 2);
    WaitFrames(1);
    return 0;
}
