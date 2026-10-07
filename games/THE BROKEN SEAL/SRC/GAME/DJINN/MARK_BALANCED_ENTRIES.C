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
#include "SCENE.H"
#include "GLOBAL_CELLS.H"
#include "DJINN_MENU.H"
#include "BATTLE_UNIT.H"
#include "BATTLE_RUNTIME.H"
#include "RUNTIME_MEM.H"
#include "IWRAM_CALL.H"
#include "RENDER_INPUT.H"
#include "CALLBACK_SCHEDULER.H"
#include "IO_REG.H"
#include "UIWINDOW.H"
#include "WINDOW.H"
extern struct DjinnMenuWork *gMenuWork;

extern u8 MsgCurrentStatus[];
void UiWindow_CloseIfOpen(struct UiWindow **window, s32 release);
void UiText_DrawCharacterAtOffsetFar(s32 message,
    struct UiWindow *window, s32 x, s32 y);
void UiWindow_SetTilemapEntryFar(struct UiWindow *window,
    s32 tile, s32 x, s32 y, s32 palette);

s32 DjinnMenu_ShowOwnerStatus(void)
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
    struct DjinnMenuWork *menu;
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
    phase = 0;
    /* FAKEMATCH: keep this consumed zero separate from frame; initialization reshapes differ 27-274. */
    __asm__("" : "+r" (phase));
    pending = 1;
    frame = 0;
    menu->cursor->active = 13;
    result = 0;
    /* FAKEMATCH: the cursor initialization uses a fresh zero instead of a spilled scalar initializer. */
    __asm__("" : "+r" (result));
    cursor[1] = cursor[0] = result;
    for (side = 3; side >= 0; side--)
        menu->owner_y[side] = 200;
    RenderOutput_ClearListFar((struct UiWindow *)menu->list_window);
    WaitFrames(1);
    page = pages;
    page[1] = page[0] = 1;
    actions = Runtime_BumpAllocateAlternatePool(96);
    saved = Runtime_BumpAllocateAlternatePool(332);
    owner = Owner_GetStateFar(menu->shown_owner[0]);
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
    left = (struct UiWindow **)&menu->window;
    UiWindow_UpdateOrCreate(left, 0, 5, 15, 15, 2);
    right = (struct UiWindow **)&menu->second_window;
    UiWindow_UpdateOrCreate(right, 15, 5, 15, 15, 2);
    RenderOutput_RedrawSavedRectFar((struct UiWindow *)menu->help_window);
    RenderOutput_RedrawSavedRectFar((struct UiWindow *)menu->status_window);
    UiText_DrawCharacterAtOffsetFar((s32)MsgCurrentStatus, (struct UiWindow *)menu->status_window, 0, 0);
    UiText_DrawCharacterAtOffsetFar((s32)MsgCurrentStatus + 2, (struct UiWindow *)menu->status_window, 0, 16);
    side = 0;
    while (1) {
        render = (struct UiRenderWork *)gWindowWork[0];
        keys = gKeyState;
        repeat = gKeysRepeat;
        if (pending) {
            render->menu_busy = 1;
            RenderOutput_PrepareForRedrawFar((struct UiWindow *)menu->window);
            RenderOutput_PrepareForRedrawFar((struct UiWindow *)menu->second_window);
            DjinnMenu_DrawStatPreview((struct UiWindow *)menu->window, 0, 0, menu->pair_owner[0], 0, 0, 3, 0, 1);
            DjinnMenu_DrawStatPreview((struct UiWindow *)menu->second_window, 0, 0, menu->pair_owner[0], 0, 0, 3, cursor[0] + 1, 1);
            render->menu_busy = 0;
        }
        if (page[side] > 1) {
            window = (struct UiWindow *)menu->second_window;
            for (i = 0; i < page[side]; i++) {
                tile = 0xf031 + i;
                if (i > 9)
                    tile = 0xf030;
                if (i == cursor[side])
                    tile -= 0x1000;
                UiWindow_SetTilemapEntryFar(window, tile, window->width - page[side] + i - 2, -1, 0);
            }
            UiWindow_SetTilemapEntryFar(window, 0xf128, window->width - page[side] - 3, -1, 0);
            UiWindow_SetTilemapEntryFar(window, 0xf129, window->width - 2, -1, 0);
            /* FAKEMATCH: without this scheduling barrier, the page-cap arguments and dirty offset differ by six instruction swaps. */
            __asm__("");
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
            cursor[side]--;
            cursor[side] = Menu_GetModuloOfSum(cursor[side], page[side]);
            AudioCommand_PlayFar(111);
            Runtime_SetMainState19();
            pending = 1;
        } else if (repeat & KEY_RIGHT) {
            cursor[side]++;
            AudioCommand_PlayFar(111);
            Runtime_SetMainState19();
            pending = 1;
            cursor[side] = Menu_GetModuloOfSum(cursor[side], page[side]);
        }
        WaitFrames(1);
    }
    Scheduler_AddOrUpdateCallback((s32)Menu_UpdateEntryObjectTransforms, 0xc80);
    ((struct UiRenderWork *)gWindowWork[0])->menu_busy = 1;
    UiWindow_CloseIfOpen((struct UiWindow **)&menu->help_window, 1);
    WaitFrames(1);
    UiWindow_UpdateOrCreate((struct UiWindow **)&menu->help_window, 13, 0, 17, 5, 2);
    UiWindow_CloseIfOpen(left, 1);
    UiWindow_CloseIfOpen(right, 1);
    RenderOutput_RedrawSavedRectFar((struct UiWindow *)menu->list_window);
    RenderOutput_RedrawSavedRectFar((struct UiWindow *)menu->unknown_window_028);
    RenderOutput_RedrawSavedRectFar((struct UiWindow *)menu->status_window);
    ((struct UiRenderWork *)gWindowWork[0])->menu_busy = 0;
    WaitFrames(1);
    return result;
}

s32 Djinn_MarkBalancedEntries(s8 *tbl, s32 self)
{
    s32 sp0;
    s32 cnt;
    s32 i;
    s8 *p;
    struct DjinnMenuWork *state;

    state = gMenuWork;
    cnt = 0;
    i = 0;
    if (cnt < state->owner_count) {
        p = tbl;
        do {
            *p = 0;
            if (i != self) {
                sp0 = cnt;
                if (Djinn_CheckTurnBalance(self, i) == 0) {
                    *p = 1;
                    cnt += 1;
                }
            }
            i += 1;
            p += 1;
        } while (i < state->owner_count);
    }
    return cnt;
}

s32 Djinn_CheckTurnBalance(s32 from, s32 to)
{
    struct DjinnMenuWork *work;
    s8 counts[16];
    u8 i;
    u8 j;
    s32 difference;
    s32 balanced;

    work = gMenuWork;
    Djinn_CountTurns(counts);
    counts[from] -= 1;
    counts[to] += 1;
    balanced = 1;
    for (i = 0; i < work->owner_count; i++) {
        j = i;
        if (i < work->owner_count) {
            while (1) {
                j++;
                if (j >= work->owner_count)
                    break;
                difference = counts[i] - counts[j];
                if (difference < -1 || difference > 1) {
                    balanced = 0;
                    break;
                }
            }
        }
    }
    return balanced;
}

void Djinn_CountTurns(u8 *counts)
{
    struct DjinnMenuWork *work;
    struct BattleUnit *unit;
    s32 owner_index;
    s32 element;
    s32 bit;
    s32 count;

    work = gMenuWork;
    for (owner_index = 0; owner_index < work->owner_count; owner_index++) {
        unit = Owner_GetStateFar(work->owners[owner_index]);
        count = 0;
        for (element = 0; element <= 3; element++) {
            for (bit = 0; bit <= 19; bit++) {
                if (unit->djinn_active[element] & (1 << bit))
                    count++;
                else if (unit->djinn_available[element] & (1 << bit))
                    count++;
            }
        }
        counts[owner_index] = count;
    }
}
