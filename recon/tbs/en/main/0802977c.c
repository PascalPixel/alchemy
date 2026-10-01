/* NONMATCHING: main [0802977c,08029910), 404 bytes with its pool.
 * 2026-10-01 (☀️ matcher 1): 404 of 404 bytes, permuter score 975 (the
 * goto-loop draft scored 2678). The frame loop is a while (1) with the A
 * and B exits as breaks, which keeps the reference's block order and lets
 * loop.c hoist &gKeysRepeat; the wrap is the % operator (__modsi3), and the
 * two id tables are s16 pairs read as [index][1], which gives the
 * reference's index * 4 + 2 offset with a register base.
 * Remaining: the reference also hoists the B mask 2 into r9, which pushes
 * work into fp and spills redraw and the portrait window (24-byte frame);
 * here 2 stays a movs inside the loop (loop.c: "not desirable"), so work
 * takes r9, the portrait fp, and the frame is 16 + 4. or-ing the A/B tests,
 * while (!0) and a (u16) mask do not change it. */
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
    s32 redraw;
    struct RenderInput *portrait;
    struct RenderInput *window;
    s32 count;
    s32 total;
    s32 index;
    s32 i;
    s32 glyph;
    s32 tile;
    s32 slot;

    work = gWindowWork;
    redraw = 1;
    portrait = UiWindow_CreateWithSideObject(0, 0, 10, 5);
    window = UiWindow_Create(10, 10, 14, 3, 2);
    index = 0;
    for (i = 0; SideObject_CharacterIdMap[i][0] != -1; i++) {}
    count = i;
    for (i = 0; SideObject_ActorKindIdMap[i][0] != -1; i++) {}
    total = count + i;

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
        if (gKeysRepeat & 2)
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
