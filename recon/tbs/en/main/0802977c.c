/* Pin-free DebugMenu_BrowseEntryGlyphs attempt: 179 native instructions,
   179 built, 176 common, differ 6. The slot-offset literal loads into r3
   instead of r1, and the following number draw copies index three rows late.
   An offset input barrier differs 14. Ordinary/signed/register/volatile offset
   types, a volatile halfword read, slot-member access and five barrier forms
   did not improve the baseline. A one-pass slot-store/load-call block differs
   10; a repeated offset assignment, redundant equal-store branch, cancelled
   glyph-derived offset and typed inline slot reader each still differ 6.
   The linked source retains its pin until this residual is closed. */
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "io_reg.h"
#include "btlsys.h"
#include "scene.h"
#include "save_state.h"
#include "resource.h"
#include "render_input.h"
#include "uiflow.h"
#include "text/msg_ids.h"
#include "tbs_edition.h"

#define gKeysRepeat (*(volatile u32 *)&gKeysRepeat)
#define gKeyState (*(volatile u32 *)&gKeyState)

#define Text_FormatHex ((void (*)(s32,s32,s32))Text_FormatHex)
#define gWindowWork (*(struct GlyphWork * *)&gWindowWork)
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

s32 DebugMenu_BrowseEntryGlyphs(void)
{
	void RenderOutput_PrepareForRedraw(struct RenderInput *);
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
    for (i = 0; Portrait_CharacterMap[i * 2] != -1; i++) {}
    count = i;
    for (i = 0; Portrait_SpriteMap[i * 2] != -1; i++) {}
    total = count + i;

    /* FAKEMATCH: the count-derived mask is needed to match the native mask
     * in r9 and work in fp; a direct constant swaps them (four instructions). */
    cancel = ((u32)(u16)count >> 16) | KEY_B;
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
                glyph = Portrait_CharacterMap[index * 2 + 1];
            else
                glyph = Portrait_SpriteMap[(index - count) * 2 + 1] + 128;
            {
                u32 off = RENDER_SLOT_VALUE_OFS + 2;
                /* FAKEMATCH: the word temporary is needed to match palette's
                 * r2 through the slot store; direct assignment chooses r3. */
                s32 pal;
                s32 loaded = *(u16 *)((u8 *)work + off);
                pal = 15;
                slot = loaded;
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
