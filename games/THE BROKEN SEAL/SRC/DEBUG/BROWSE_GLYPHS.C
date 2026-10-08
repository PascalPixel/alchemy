#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "TYPES.H"
#include "IO_REG.H"
#include "RENDER_INPUT.H"
#include "TBS_EDITION.H"
#include "UIFLOW.H"
#include "text/MSG_IDS.H"

#define gWindowWork (*(struct GlyphWork * *)&gWindowWork)
#define gKeysRepeat (*(volatile u32 *)&gKeysRepeat)
#define UiWindow_Create ((struct RenderInput * (*)(s32, s32, s32, s32, s32))UiWindow_Create)
#define UiText_DrawCharacterAtOffset ((void (*)(s32, struct RenderInput *, s32, s32))UiText_DrawCharacterAtOffset)

struct GlyphWork {
    u8 unknown_00[RENDER_SLOT_VALUE_OFS + 2];
    u16 slot;
};

#if !EDITION_INTERNATIONAL || defined(TBS_EDITION_EN)
#define DEBUG_GLYPH_NAME ((s32)MsgDebugGlyphName)
#elif defined(TBS_EDITION_DE) || defined(TBS_EDITION_FR)
#define DEBUG_GLYPH_NAME ((s32)MsgEnd)
#else
#define DEBUG_GLYPH_NAME ((s32)MsgDebugSecondForm)
#endif

struct RenderInput *UiWindow_CreateWithSideObject(s32, s32, s32, s32);
void RenderOutput_PrepareForRedraw(struct RenderInput *);
void UiText_DrawNumberInWindow(s32, s32, struct RenderInput *, s32, s32);
void WaitFrames(s32);

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
    /* FAKEMATCH: the B mask's constant equivalence ranks it after work; the native mask is in r9 and work in fp. */
    register s32 cancel asm("r9");

    work = gWindowWork;
    redraw = 1;
    portrait = UiWindow_CreateWithSideObject(0, 0, 10, 5);
    window = UiWindow_Create(10, 10, 14, 3, 2);
    index = 0;
    for (i = 0; Portrait_CharacterMap[i][0] != -1; i++) {}
    count = i;
    for (i = 0; Portrait_SpriteMap[i][0] != -1; i++) {}
    total = count + i;

    cancel = KEY_B;
    while (1) {
        if (gKeysRepeat & KEY_LEFT) {
            redraw = 1;
            index--;
        }
        if (gKeysRepeat & KEY_RIGHT) {
            redraw = 1;
            index++;
        }
        if (gKeysRepeat & KEY_L) {
            redraw = 1;
            index -= 10;
        }
        if (gKeysRepeat & KEY_R) {
            redraw = 1;
            index += 10;
        }
        if (gKeysRepeat & KEY_A)
            break;
        if (gKeysRepeat & cancel)
            break;
        if (redraw) {
            redraw = 0;
            index = (index + total) % total;
            RenderOutput_PrepareForRedraw(window);
            if (index < count)
                glyph = Portrait_CharacterMap[index][1];
            else
                glyph = Portrait_SpriteMap[index - count][1] + 128;
            {
                /* FAKEMATCH: forced temporary pinned to r1; reload otherwise adds the offset to fp in r3. */
                register u32 off asm("r1") = RENDER_SLOT_VALUE_OFS + 2;
                /* FAKEMATCH: the palette stays in r2 across the slot store, as natively. */
                register s32 pal asm("r2");
                slot = *(u16 *)((u8 *)work + off);
                pal = 15;
                UiGlyph_LoadEntryWithPalette(glyph, 0, &slot, &tile, pal, 1);
            }
            UiText_DrawNumberInWindow(index, 2, window, 0, 0);
            UiText_DrawCharacterAtOffset(index + DEBUG_GLYPH_NAME, window, 24, 0);
        }
        WaitFrames(1);
    }
    UiWork_Finalize(window, 2);
    UiWork_Finalize(portrait, 2);
    WaitFrames(1);
    return 0;
}
