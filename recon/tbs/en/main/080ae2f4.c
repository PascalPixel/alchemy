/* Draft, main:080ae2f4, complete extent 0x080ae2f4..0x080ae714 (1056 B).
 * Djinn "Current status" preview: two stat previews side by side, a page
 * tab row on the right window, left/right turn the page, START leaves with
 * -2 and A/B/L/R with -1.
 *
 * 2026-10-05 sun deep finisher 23: 229 to 61 (alchemy diff), no S2 devices.
 * - row_y[i] = 200 as a plain constant: a u16 store of an 8-bit constant
 *   matches *thumb_movhi_insn's "mn" alternative, so the minipool fix puts
 *   0xc8 in an early 64-byte-range pool with a branch around it, exactly
 *   the native's first pool; the Data_000000c8 symbol is gone: 216.
 * - The native's dead "frame % 60 - 5" is the sibling 080ad6d4's clamp left
 *   without a use: blink = frame % 60 - 5; if (blink < 0) blink = 0;
 *   if (blink > 29) blink = 29; flow deletes the clamp sets, jump the
 *   branches, and the subtract survives: 182.
 * - The zero arguments and menu_busy = 0 are literal 0s (loop.c combines
 *   them into one hoisted zero, the native's r8) and the page tab and the
 *   left/right code share one index, side: the high registers now match
 *   (r8 zero, sl cursor copy, fp pages, r9 menu): 148.
 * - frame = 0; pending = 1; phase = 0 in that order after menu = gMenuWork:
 *   63. Every order of those seven opening statements (1,260, and 10,080
 *   with side = 0 among them, with sel[] = 0 or = side) scores 61 to 64.
 * - gWindowWork as WINDOW.H's struct UiRenderWork and the menu windows as
 *   struct UiWindow * (or s32): UiRenderWork's alias set has the window
 *   pointer and int as children, so the menu_busy = 1 store stays ahead of
 *   the menu->left_window load as in the native: 61. With the private
 *   MenuRenderWork (and with a bitfield or a union there) sched2 hoists the
 *   load; *(u8 *)&render->menu_busy (alias set 0) keeps the order but
 *   makes the constant 1 QImode, not hoisted, and scores 97.
 * Remaining (61), three things:
 * 1. Opening reloads: the native loads 1, 0, 0 into r1, r2, r0 (pending,
 *    frame, phase, no inheritance between the zeros) and the cursor zero
 *    separately into r7; here the zeros share r0 and the cursor stores
 *    reload frame from its slot.
 * 2. The -1 shared in r5 by the two calls after the tab loop: loop.c's
 *    second pass hoists it (life 14 * threshold 14 = 196 >= 188 real insns
 *    in the frame loop); the native's loop must have had at least 197 real
 *    insns then, or a shorter life. The else-if form of the clamp keeps the
 *    -1 but loses the dead subtract (92); extra dead code after the clamp
 *    (blink %= 30, blink * 0x444, a fold at 15) changes nothing or worse.
 * 3. Reload register rotation at the loop preheader (preview, sel copy,
 *    zero in r2, r1, r0 natively), which follows from 1.
 * 0xbaa is message 2986 "Current status" (MsgCurrentStatus, unnamed in the
 * catalogue), the second title being MsgCurrentStatus + 2.
 */
#include "TYPES.H"
#include "SYSTEM.H"
#include "RUNTIME_MEM.H"
#include "IWRAM_CALL.H"
#include "BATTLE_UNIT.H"
#include "RENDER_INPUT.H"
#include "CALLBACK_SCHEDULER.H"
#include "IO_REG.H"
#include "UIWINDOW.H"
#include "WINDOW.H"

struct MenuCursor {
    u8 unknown00[5];
    u8 state;
};

struct MenuActionWork {
    u8 unknown000[0x10];
    struct UiWindow *title_window;
    struct MenuCursor *cursor;
    u8 unknown018[0x0c];
    struct UiWindow *left_window;
    struct UiWindow *saved_window;
    u8 unknown02c[4];
    struct UiWindow *list_window;
    struct UiWindow *right_window;
    u8 unknown038[0xd4];
    struct UiWindow *message_window;
    u8 unknown110[0x34];
    u16 row_y[4];
    u8 unknown14c[0xce];
    u8 owner;
    u8 unknown21b[0x3d];
    u8 preview_owner;
};

extern struct MenuActionWork *gMenuWork;
extern volatile u32 gKeyState;
extern volatile u32 gKeysRepeat;
extern u8 MsgCurrentStatus[];
extern const u8 Menu_BackdropFrameTile[];

void Runtime_SetMainState19(void);
struct BattleUnit *Owner_GetStateFar(s32 owner);
s32 OwnerAction_DiffSlots(struct OwnerActionSlot *first,
    struct OwnerActionSlot *second, u16 *out, s32 *first_count, s32 *second_count);
s32 UiWindow_UpdateOrCreate(struct UiWindow **window,
    s32 x, s32 y, s32 width, s32 height, s32 flags);
void UiWindow_CloseIfOpen(struct UiWindow **window, s32 release);
void RenderOutput_ClearListFar(struct UiWindow *window);
void RenderOutput_RedrawSavedRectFar(struct UiWindow *window);
void RenderOutput_PrepareForRedrawFar(struct UiWindow *window);
void UiText_DrawCharacterAtOffsetFar(s32 message,
    struct UiWindow *window, s32 x, s32 y);
void UiWindow_SetTilemapEntryFar(struct UiWindow *window,
    s32 tile, s32 x, s32 y, s32 palette);
s32 DjinnMenu_DrawStatPreview(struct UiWindow *window,
    s32 x, s32 y, s32 owner, s32 give, s32 take, s32 mode, s32 page, s32 side);
s32 FourObjectMotion_SetSlotPosition(s32 slot, s32 x, s32 y, s32 hidden);
s32 Menu_GetModuloOfSum(s32 value, s32 modulus);
void Menu_UpdateEntryObjectTransforms(void);
void AudioCommand_PlayFar(s32 cue);

s32 Unnamed_080ae2f4(void)
{
    s32 cursor[2];
    s32 pages[2];
    s32 gained;
    s32 lost;
    s32 phase;
    s32 pending;
    s32 frame;
    struct UiWindow **left;
    struct UiWindow **right;
    struct UiRenderWork *render;
    u32 keys;
    u32 repeat;
    u8 *preview;
    s32 *sel;
    struct MenuActionWork *menu;
    struct BattleUnit *owner;
    struct UiWindow *window;
    u16 *actions;
    void *saved;
    s32 result;
    s32 tile;
    s32 i;
    s32 side;
    s32 *page;
    s32 blink;

    menu = gMenuWork;
    frame = 0;
    pending = 1;
    phase = 0;
    menu->cursor->state = 13;
    sel = cursor;
    sel[1] = sel[0] = 0;
    for (i = 3; i >= 0; i--)
        menu->row_y[i] = 200;
    RenderOutput_ClearListFar(menu->list_window);
    WaitFrames(1);
    page = pages;
    page[1] = page[0] = 1;
    actions = Runtime_BumpAllocateAlternatePool(96);
    saved = Runtime_BumpAllocateAlternatePool(332);
    owner = Owner_GetStateFar(menu->owner);
    page[1] = page[0] = OwnerAction_DiffSlots(owner->action_slots, owner->action_slots,
        actions, &gained, &lost);
    Runtime_BumpFree(saved);
    Runtime_BumpFree(actions);
    page[0] = (page[0] - 1) / 6 + 1;
    if (page[0] == 0)
        page[0] = 1;
    page[1] = (page[1] - 1) / 6 + 1;
    if (page[1] == 0)
        page[1] = 1;
    left = &menu->left_window;
    UiWindow_UpdateOrCreate(left, 0, 5, 15, 15, 2);
    right = &menu->right_window;
    UiWindow_UpdateOrCreate(right, 15, 5, 15, 15, 2);
    RenderOutput_RedrawSavedRectFar(menu->message_window);
    RenderOutput_RedrawSavedRectFar(menu->title_window);
    UiText_DrawCharacterAtOffsetFar((s32)MsgCurrentStatus, menu->title_window, 0, 0);
    UiText_DrawCharacterAtOffsetFar((s32)MsgCurrentStatus + 2, menu->title_window, 0, 16);
    preview = &menu->preview_owner;
    side = 0;
    while (1) {
        render = (struct UiRenderWork *)gWindowWork[0];
        keys = gKeyState;
        repeat = gKeysRepeat;
        if (pending) {
            render->menu_busy = 1;
            RenderOutput_PrepareForRedrawFar(menu->left_window);
            RenderOutput_PrepareForRedrawFar(menu->right_window);
            DjinnMenu_DrawStatPreview(menu->left_window, 0, 0, *preview, 0, 0, 3, 0, 1);
            DjinnMenu_DrawStatPreview(menu->right_window, 0, 0, *preview, 0, 0, 3, cursor[0] + 1, 1);
            render->menu_busy = 0;
        }
        if (page[side] > 1) {
            window = menu->right_window;
            for (i = 0; i < page[side]; i++) {
                tile = 0xf031 + i;
                if (i > 9)
                    tile = 0xf030;
                if (i == sel[side])
                    tile -= 0x1000;
                UiWindow_SetTilemapEntryFar(window, tile, window->width - page[side] + i - 2, -1, 0);
            }
            UiWindow_SetTilemapEntryFar(window, 0xf128, window->width - page[side] - 3, -1, 0);
            UiWindow_SetTilemapEntryFar(window, 0xf129, window->width - 2, -1, 0);
            render->dirty |= 2 << (window->y >> 2);
        }
        frame++;
        blink = frame % 60 - 5;
        if (blink < 0)
            blink = 0;
        if (blink > 29)
            blink = 29;
        FourObjectMotion_SetSlotPosition(0, 32, 200, 0);
        if (pending) {
            pending = 0;
            phase = Menu_GetModuloOfSum(phase, 2);
        }
        if ((frame & 3) == 0) {
            if (frame & 4)
                Iwram_CopyWords((void *)0x060052c0, Menu_BackdropFrameTile, 32);
            else
                Iwram_FillWords((void *)0x060052c0, 32, 0x44444444);
        }
        if (keys & KEY_START) {
            AudioCommand_PlayFar(113);
            result = -2;
            break;
        }
        if (keys & (KEY_A | KEY_B | KEY_R | KEY_L)) {
            AudioCommand_PlayFar(113);
            result = -1;
            break;
        }
        if (repeat & KEY_LEFT) {
            sel[side]--;
            sel[side] = Menu_GetModuloOfSum(sel[side], page[side]);
            AudioCommand_PlayFar(111);
            Runtime_SetMainState19();
            pending = 1;
        } else if (repeat & KEY_RIGHT) {
            sel[side]++;
            AudioCommand_PlayFar(111);
            Runtime_SetMainState19();
            pending = 1;
            sel[side] = Menu_GetModuloOfSum(sel[side], page[side]);
        }
        WaitFrames(1);
    }
    Scheduler_AddOrUpdateCallback((s32)Menu_UpdateEntryObjectTransforms, 0xc80);
    ((struct UiRenderWork *)gWindowWork[0])->menu_busy = 1;
    UiWindow_CloseIfOpen(&menu->message_window, 1);
    WaitFrames(1);
    UiWindow_UpdateOrCreate(&menu->message_window, 13, 0, 17, 5, 2);
    UiWindow_CloseIfOpen(left, 1);
    UiWindow_CloseIfOpen(right, 1);
    RenderOutput_RedrawSavedRectFar(menu->list_window);
    RenderOutput_RedrawSavedRectFar(menu->saved_window);
    RenderOutput_RedrawSavedRectFar(menu->title_window);
    ((struct UiRenderWork *)gWindowWork[0])->menu_busy = 0;
    WaitFrames(1);
    return result;
}
