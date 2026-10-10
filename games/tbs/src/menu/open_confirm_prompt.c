#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "character_menu.h"
#include "psynergy_menu.h"
#include "heap_state.h"
#include "types.h"
#include "mem.h"
#include "callback_scheduler.h"
#include "system.h"
#include "ui.h"
#include "tbs_edition.h"
#include "iwram_call.h"
#include "btlsys.h"
#include "far_runtime.h"
#include "btlcalc.h"
#include "eventwrk.h"
#include "text/msg_ids.h"
void UnitCalcFar(s32 unit_id);

#define RenderOutput_ClearListFar ((void (*)(s32))RenderOutput_ClearListFar)

s32 Party_ListActiveOwnersFar(u16 *ids);
void ItemMenu_Init(s32, s32, s32, s32);
void ItemMenu_Close(void);
void Event_ClearInvalidPackedValuesFar(void);

#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_ES) || \
    defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
#define PROMPT_SAVES_TILES 1
#define PROMPT_TILES ((void *)0x06004000)
#define PROMPT_TILES_SIZE 0x2000

typedef s32 (*WordCopyFn)(void *dst, const void *src, s32 size);
typedef s32 (*WordFillFn)(void *dst, s32 size, u32 value);

static __inline__ s32 CopyWords(WordCopyFn copy, void *dst, const void *src, s32 size)
{
    /* FAKEMATCH: direct calls move the saved tile size from r9 to fp in the European editions. */
    return copy(dst, src, size);
}

static __inline__ s32 FillWords(WordFillFn fill, void *dst, s32 size, u32 value)
{
    /* FAKEMATCH: a direct fill call changes the preceding copy's fp call register and adds moves. */
    return fill(dst, size, value);
}
#endif

s32 Menu_OpenConfirmPrompt(void)
{
    /* FAKEMATCH: the heap slice at slot 6 preserves renderer/event load order; a whole-bank base adds a literal word. */
#if defined(PROMPT_SAVES_TILES)
    void *saved = Runtime_BumpAllocateAlternatePool(PROMPT_TILES_SIZE);
#endif
    struct PsynergyMenuState *state = (struct PsynergyMenuState *)Runtime_AllocateHeapBlock(HEAP_SLOT_MENU, 0xa70);
    s32 high;
    s32 unused;
    s32 low;
    s32 result;

    gMenuCtrlWork->suspended = 1;
    UiWindow_DrawFrameFar(0, 0, 30, 20);
    WaitFrames(1);
    UiWindow_InitializeWork(0);
    state->owner_count = Party_ListActiveOwnersFar(state->owner_table);
    ItemMenu_Init(0, 3, 0, 7);
    state->message_window = UiWindow_CreateFar(13, 0, 17, 3, 2);
    Palette_LightenBankHighlight(14);
    Link_DrawShiftedTilePairFar(0x06002500);
#if defined(PROMPT_SAVES_TILES)
    Scheduler_EnableOverlayCallbacksWithFlags();
    CopyWords(Iwram_CopyWords, saved, PROMPT_TILES, PROMPT_TILES_SIZE);
    FillWords(Iwram_FillWords, PROMPT_TILES, PROMPT_TILES_SIZE, 0x33333333);
    UiWork_SetAltFlagAndClearTableFar(1);
#endif
    Menu_CancelSoundReset();
    result = Menu_ResolveSelectedAction(
        &high, &unused, &low);
    Menu_EnsureCancelSound();
    if (result == 1) {
        struct EventWork *target = ((struct EventWork **)&gMenuCtrlWork)[HEAP_SLOT_EVENT - HEAP_SLOT_MENU_CONTROL];
        u16 flags;
        BtlActGet(0x3fff & state->pane_action[0]);
        flags = low | (high << 10);
        target->psynergy_request = flags;
    }
    RenderOutput_ClearListFar(state->status_window);
    ((struct UiRenderWork *)((void **)&gMenuCtrlWork)[HEAP_SLOT_WINDOW - HEAP_SLOT_MENU_CONTROL])->menu_busy = 1;
    ItemMenu_Close();
    UiWindow_DrawFrameFar(0, 0, 30, 20);
    Runtime_ReleaseHeapBlock(0x37);
    gMenuCtrlWork->suspended = 0;
#if defined(PROMPT_SAVES_TILES)
    UiWindow_MarkVisibleTileAttributesFar();
    UiWork_SetAltFlagAndClearTableFar(0);
    CopyWords(Iwram_CopyWords, PROMPT_TILES, saved, PROMPT_TILES_SIZE);
    ((struct UiRenderWork *)((void **)&gMenuCtrlWork)[HEAP_SLOT_WINDOW - HEAP_SLOT_MENU_CONTROL])->menu_busy = 0;
    MemFree(saved);
    WaitFrames(1);
    Scheduler_DisableOverlayCallbacksWithFlags();
#endif
    WaitFrames(1);
    UiWindow_EraseBorderRectFar(0, 0, 30, 20);
    ((struct UiRenderWork *)((void **)&gMenuCtrlWork)[HEAP_SLOT_WINDOW - HEAP_SLOT_MENU_CONTROL])->menu_busy = 0;
    Event_ClearInvalidPackedValuesFar();
    return result;
}


void ItemMenu_DrawMsg(s32 unused, s32 message);
void AudioCommand_PlayFar(s32 cue);

s32 Menu_ResolveSelectedAction(s32 *out_owner, s32 *unused, s32 *out_action)
{
    struct PsynergyMenuState *work;
    s32 result;
    s32 state;
    s32 done;
    s32 selection;
    s32 classification;
    s32 raw;
    s32 self_flag;
    struct BtlAct *action;

    state = 0;
    done = 0;
    work = gMenuWork;
    result = 0;

    while (done == 0 && GameFlag_TestFar(0x150) == 0) {
        switch (state) {
        case 0:
        {
            s32 cancelled;
            s32 chosen;

            work->pane_row[0] = 0;
            ItemMenu_DrawMsg(0, (s32)MsgPsynergyChooseOwner);
            chosen = PsynergyMenu_SelectPartySlot(0);
            cancelled = -1;
            /* FAKEMATCH: separate the cancellation definition from the call result; without this boundary, pane-row stores exchange r2/r3 (12 instructions differ). XOR equality preserves the cancellation copy; direct equality copies r0 instead of r3 (2 instructions differ). */
            asm("" : "+r"(cancelled));
            if (((u32)chosen ^ (u32)cancelled) == 0) {
                done = 1;
                result = cancelled;
            }
            RenderOutput_RedrawSavedRectFar(work->info_window);
            state = 1;
            break;
        }

        case 1:
            WaitFrames(1);
            UnitGetFar(work->owner_ids[0]);
            state = 0;
            if (work->psynergy_count != 0) {
                /* FAKEMATCH: clobbering r2 hides the 0x218 it holds from reload_cse_move2add, so the 0x268 mode offset is built from its own constant (movs/lsls) instead of adds r2, #80. */
                asm("" : : : "r2");
                switch (work->mode) {
                case 0:
                    ItemMenu_DrawMsg(0, (s32)MsgPsynergyChooseAbility);
                    break;
                case 1:
                    ItemMenu_DrawMsg(0, (s32)MsgPsynergyChooseForL);
                    break;
                case 2:
                    ItemMenu_DrawMsg(0, (s32)MsgPsynergyChooseForR);
                    break;
                }
                ItemMenu_PosCategory();
                Menu_DrawOwnerStatusPanel(work->status_window, work->owner_ids[0], 0, 0);
                selection = PsynergyMenu_RunList(0);
                state = 0;
                if (selection != -1) {
                    state = 2;
                    if (work->mode != 0) {
                        if (work->mode == 1) {
                            PsynergyMenu_SetShortcut(
                                work->owner_ids[0], selection, 0);
                            RenderOutput_ClearListFar((s32)work->info_window);
                            InventoryMenu_ShowModalMessage(
                                (s32)MsgShortcutSetL, -1, -1);
#if !EDITION_INTERNATIONAL
                            RenderOutput_RedrawSavedRectFar(work->info_window);
#endif
                        } else {
                            PsynergyMenu_SetShortcut(
                                work->owner_ids[0], selection, 1);
                            RenderOutput_ClearListFar((s32)work->info_window);
                            InventoryMenu_ShowModalMessage(
                                (s32)MsgShortcutSetR, -1, -1);
#if !EDITION_INTERNATIONAL
                            RenderOutput_RedrawSavedRectFar(work->info_window);
#endif
                        }
                        state = 0;
                    }
                }
            }
            break;

        case 3:
            ItemMenu_DrawMsg(0, (s32)MsgPsynergyChooseTarget);
            self_flag = PsynergyMenu_SelectTarget(0);
            state = 4;
            if (self_flag == -1) {
                work->flags |= 1;
                state = 1;
            }
            break;

        case 2:
            classification = PsynergyMenu_ClassifySelectedPsynergy();
            if (classification == 1) {
                state = 3;
                break;
            }
            if (classification == 2) {
                work->owner_ids[1] = 9;
                state = 4;
                break;
            }
            done = 1;
            result = 1;
            *out_owner = work->owner_ids[0];
            *out_action = work->pane_action[0] & 0x3fff;
            break;

        case 4:
            self_flag = 0;
            raw = work->pane_action[0];
            result = BattleEffect_ApplyToTargets(
                raw, work->owner_ids[0], work->owner_ids[1], 0);
            if (work->owner_ids[1] == 9) {
                work->owner_ids[1] = work->owner_ids[0];
                self_flag = 9;
            }
            if (result != -1) {
                action = BtlActGet(work->pane_action[0] & 0x3fff);
                UnitEpAddFar(work->owner_ids[0], -action->pp_cost);
            }
            UnitCalcFar(work->owner_ids[0]);
            if (result != -1) {
                Menu_DrawOwnerStatusPanel(work->status_window, work->owner_ids[1], 0, 0);
                Ability_PlayUseAnimation(work->pane_action[0] & 0x3fff);
                RenderOutput_ClearListFar((s32)work->info_window);
                InventoryMenu_ShowModalMessage(
                    work->use_result + (s32)MsgItemUseResult, 0, -1);
            } else {
                AudioCommand_PlayFar(114);
                RenderOutput_ClearListFar((s32)work->info_window);
                InventoryMenu_ShowModalMessage(
                    work->use_result + (s32)MsgItemUseResult,
                    result,
                    result);
            }
            if (result != -1) {
                result = 1;
                work->flags |= 1;
                state = 1;
            } else {
                work->skip_slide = 1;
                if (self_flag == 9) {
                    work->flags |= 1;
                    state = 1;
                } else {
                    state = 3;
                }
            }
            break;

        default:
            done = 1;
            break;
        }
    }

    if (GameFlag_TestFar(0x150) != 0) {
        result = -1;
    }
    return result;
}
