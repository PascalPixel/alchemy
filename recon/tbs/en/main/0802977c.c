/* Complete owner [0802977c, 08029910), 404 bytes including pool.
 * Corrects the old lift's missing Math_Mod divisor, swapped glyph argument,
 * cached key reads and missing zero return. Tables are signed entry pairs.
 * Candidate 404/404 bytes, 141 differing halfwords / 74 aligned edits.
 * Three bounded hypotheses: typed recovery; explicit frame-loop goto; joined
 * A/B exit test. Goto preserves the reference block order; natural loops
 * rotate the key tests. Remaining: 16-byte frame instead of 24, redraw and
 * portrait kept in registers, no hoisted B mask, and pair-field address adds.
 * FAKEMATCH: explicit frame-loop labels preserve the reference block layout. */
#include "TYPES.H"
#include "RENDER_INPUT.H"

struct DebugEntry {
    s16 type;
    s16 glyph;
};

struct GlyphWork {
    u8 unknown_00[0x12f2];
    u16 slot;
};

extern struct GlyphWork *gWindowWork;
extern volatile u32 gKeysRepeat;
extern struct DebugEntry SideObject_CharacterIdMap[], SideObject_ActorKindIdMap[];
extern u8 Value_00000dd2[];

struct RenderInput *UiWindow_CreateWithSideObject(s32, s32, s32, s32);
struct RenderInput *UiWindow_Create(s32, s32, s32, s32, s32);
s32 Math_Mod(s32, s32);
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
    for (i = 0; SideObject_CharacterIdMap[i].type != -1; i++) {}
    count = i;
    for (i = 0; SideObject_ActorKindIdMap[i].type != -1; i++) {}
    total = count + i;

next_frame:
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
            goto close;
        if (gKeysRepeat & 2)
            goto close;
        if (redraw) {
            redraw = 0;
            index = Math_Mod(index + total, total);
            RenderOutput_PrepareForRedraw(window);
            if (index < count)
                glyph = SideObject_CharacterIdMap[index].glyph;
            else
                glyph = SideObject_ActorKindIdMap[index - count].glyph + 128;
            slot = work->slot;
            UiGlyph_LoadEntryWithPalette(glyph, 0, &slot, &tile, 15, 1);
            UiText_DrawNumberInWindow(index, 2, window, 0, 0);
            UiText_DrawCharacterAtOffset(index + (s32)Value_00000dd2, window, 24, 0);
        }
        WaitFrames(1);
        goto next_frame;
close:
    UiWork_Finalize(window, 2);
    UiWork_Finalize(portrait, 2);
    WaitFrames(1);
    return 0;
}
