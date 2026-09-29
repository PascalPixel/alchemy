/* Complete debug icon browser [08029554, 0802977c), 552 bytes with pool.
 * The adjacent entry-glyph browser now has its own listing at 0802977c.
 * ROM labels distinguish ITEM, ENERGY and STATUS pages.
 * Typed reconstruction restores slot=-1 each iteration and the third slot
 * argument to Ui_BuildPatternToSlot, both omitted by the old lifted draft.
 * Candidate 540/552 bytes, 258 differing halfwords / 98 aligned edits
 * (old draft 524 bytes / 119 edits). Three bounded hypotheses stopped.
 * Separate label calls recover the two reference call tails. Named versus
 * literal key-state address produced identical code here. Remaining:
 * the key base is shared across blocks instead of reloaded, zero-copy and
 * argument scheduling differ, and 8 lives across number-rendering calls.
 * The complete 16-byte frame, row loop and both icon-loader tails match.
 * 2026-09-29 alchemy permute (seed 1, 3 jobs, 10 minutes): 30,830
 * candidates; the best scored 1481 against 2305 (21 register-only, 1
 * stack-only, 14 operand, 8 reordered, 6 deleted) after 31 rewrites
 * (reorder independent statements, introduce a temporary, add a same-width
 * cast, swap commutative operands), none of them kept. Its gains are
 * statement moves and temporaries around the key and label calls; the five
 * label tables (Data_08037440 to Data_08037460) still need ROM labels.
 */
#include "TYPES.H"
#include "RENDER_INPUT.H"

struct DebugMenuState {
    u8 unknown_00[4];
    u16 active;
};

extern struct DebugMenuState *gMenuCtrlWork;
#define KEYS_PRESSED (*(volatile u32 *)0x03001b04)
extern u8 Data_08037440[], Data_08037448[], Data_08037450[];
extern u8 Data_08037458[], Data_08037460[];

void WaitFrames(s32);
s32 Math_Mod(s32, s32);
void UiWork_Finalize(struct RenderInput *, s32);
struct RenderInput *UiWindow_Create(s32, s32, s32, s32, s32);
void UiText_DrawStringInWindow(const u8 *, struct RenderInput *, s32, s32);
void UiText_DrawNumberInWindow(s32, s32, struct RenderInput *, s32, s32);
void UiIcon_BuildItemIconTiles(u32, s32, s32 *, s32 *, s32);
void UiIcon_BuildAbilityIconTiles(u32, s32, s32 *, s32 *, s32);
s32 Resource_FindFreeEntry(void);
s32 Ui_BuildPatternToSlot(s32, s32, s32);

s32 DebugMenu_BrowseIcons(void)
{
    s32 redraw;
    s32 tile;
    s32 page;
    s32 mode;
    struct RenderInput *window;
    s32 slot;
    s32 base;
    s32 i;
    s32 row;
    s32 x;
    s32 y;

    redraw = 1;
    window = NULL;
    page = 0;
    mode = 0;
    gMenuCtrlWork->active = redraw;
    WaitFrames(1);

next_frame:
    if (KEYS_PRESSED & 0x20) {
        redraw = 1;
        page--;
    }
    if (KEYS_PRESSED & 0x10) {
        redraw = 1;
        page++;
    }
    if (KEYS_PRESSED & 0x200) {
        redraw = 1;
        mode--;
    }
    if (KEYS_PRESSED & 0x100) {
        redraw = 1;
        mode++;
    }
    if (KEYS_PRESSED & 1)
        goto close;
    if (KEYS_PRESSED & 2)
        goto close;
    if (redraw != 0) {
        redraw = 0;
        page = (page + 8) % 8;
        mode = Math_Mod(mode + 3, 3);
        UiWork_Finalize(window, 2);
        window = UiWindow_Create(10, 0, 18, 12, 2);
        if (mode == 0)
            UiText_DrawStringInWindow(Data_08037440, window, 0, 0);
        else if (mode == 1)
            UiText_DrawStringInWindow(Data_08037448, window, 0, 0);
        else
            UiText_DrawStringInWindow(Data_08037450, window, 0, 0);
        UiText_DrawStringInWindow(Data_08037458, window, 0, 8);
        UiText_DrawNumberInWindow(page, 0, window, 40, 8);
        base = page << 5;
        UiText_DrawNumberInWindow(base, 3, window, 64, 8);
        UiText_DrawStringInWindow(Data_08037460, window, 88, 8);
        UiText_DrawNumberInWindow(base + 31, 3, window, 96, 8);
        for (i = 0; i <= 31; i++) {
            slot = -1;
            row = i / 8;
            x = (i - row * 8) * 16;
            y = row * 16 + 16;
            if (mode == 0) {
                UiIcon_BuildItemIconTiles(base + i, 1, &slot, &tile, mode);
                RenderOutput_Create(slot, 0x40000000, window, x, y);
            } else if (mode == 1) {
                UiIcon_BuildAbilityIconTiles(base + i, 1, &slot, &tile, 0);
                RenderOutput_Create(slot, 0x40000000, window, x, y);
            } else {
                slot = Resource_FindFreeEntry();
                Ui_BuildPatternToSlot(i, 0, slot);
                RenderOutput_Create(slot, 0x40000000, window, x, y);
            }
        }
    }
    WaitFrames(1);
    goto next_frame;
close:
    UiWork_Finalize(window, 2);
    gMenuCtrlWork->active = 0;
    return 0;
}
