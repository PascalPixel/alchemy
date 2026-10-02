/* Draft, not exact: 404 of 404 bytes, score 485 (was 975), 12 rows off.
 * The frame loop is a while (1) with the A and B exits as breaks; the wrap
 * is the % operator, and the two id tables are s16 pairs read as
 * [index][1]. The B mask is a variable set to 2 before the loop: that is
 * what puts it in a register (the reference's r9) and spills redraw and the
 * portrait window as the reference does; portrait is declared before
 * redraw, which orders their two slots.
 * Remaining: the reference keeps the work pointer in fp and the mask in r9,
 * here they are swapped (the mask is a constant to the allocator, so it
 * ranks last), and with it the slot read is ldrh [fp, r1] there and an
 * add here. Setting the mask at the top of the loop body, a second set, a
 * narrower type and moving the work read do not turn it; 110,000 permuter
 * candidates reached 345. */
#include "TYPES.H"
#include "RENDER_INPUT.H"


struct GlyphWork {
    u8 unknown_00[0x12f2];
    u16 slot;
};

extern struct GlyphWork *gWindowWork;
extern volatile u32 gKeysRepeat;
extern s16 SideObject_CharacterIdMap[][2], SideObject_ActorKindIdMap[][2];
extern u8 Value_00000dd2[];

struct RenderInput *UiWindow_CreateWithSideObject(s32, s32, s32, s32);
struct RenderInput *UiWindow_Create(s32, s32, s32, s32, s32);
void RenderOutput_PrepareForRedraw(struct RenderInput *);
void UiGlyph_LoadEntryWithPalette(u32, s32, s32 *, s32 *, s32, s32);
void UiText_DrawNumberInWindow(s32, s32, struct RenderInput *, s32, s32);
void UiText_DrawCharacterAtOffset(s32, struct RenderInput *, s32, s32);
void WaitFrames(s32);
void UiWork_Finalize(struct RenderInput *, s32);

s32 DebugMenu_BrowseEntryGlyphs(void)
{
    struct GlyphWork *work;
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

    work = gWindowWork;
    redraw = 1;
    portrait = UiWindow_CreateWithSideObject(0, 0, 10, 5);
    window = UiWindow_Create(10, 10, 14, 3, 2);
    index = 0;
    for (i = 0; SideObject_CharacterIdMap[i][0] != -1; i++) {}
    count = i;
    for (i = 0; SideObject_ActorKindIdMap[i][0] != -1; i++) {}
    total = count + i;

    cancel = 2;
    while (1) {
        if (gKeysRepeat & 0x20) {
            redraw = 1;
            index--;
        }
        if (gKeysRepeat & 0x10) {
            redraw = 1;
            index++;
        }
        if (gKeysRepeat & 0x200) {
            redraw = 1;
            index -= 10;
        }
        if (gKeysRepeat & 0x100) {
            redraw = 1;
            index += 10;
        }
        if (gKeysRepeat & 1)
            break;
        if (gKeysRepeat & cancel)
            break;
        if (redraw) {
            redraw = 0;
            index = (index + total) % total;
            RenderOutput_PrepareForRedraw(window);
            if (index < count)
                glyph = SideObject_CharacterIdMap[index][1];
            else
                glyph = SideObject_ActorKindIdMap[index - count][1] + 128;
            slot = work->slot;
            UiGlyph_LoadEntryWithPalette(glyph, 0, &slot, &tile, 15, 1);
            UiText_DrawNumberInWindow(index, 2, window, 0, 0);
            UiText_DrawCharacterAtOffset(index + (s32)Value_00000dd2, window, 24, 0);
        }
        WaitFrames(1);
    }
    UiWork_Finalize(window, 2);
    UiWork_Finalize(portrait, 2);
    WaitFrames(1);
    return 0;
}
