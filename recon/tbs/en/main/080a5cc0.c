/* Pin-free Menu_ResolveSelectedAction trial: international 316 native/built,
 * 315 common; Japanese 320 native/built, 319 common; differ 2 in all six.
 * A generic output barrier after cancelled = -1 restores the pane-row
 * store registers. The cancel result still copies r0 instead of r3.
 * Generic, early-clobber and chosen-input constraints retain 2, as does
 * reversing the comparison. Moving the barrier inside the taken branch
 * restores 12. Assigning literal -1 adds two instructions and differs 16.
 * Earlier ordinary-variable trials differed 12. An unsigned
 * cancelled and a one-pass result copy left that same residual.
 * A 3-minute search (seed 149, exact stack/branches) improved weighted
 * 45 to 35 by a message-id temporary, still differ 12. The trial also
 * differs 12 when cancelled uses a value-preserving narrow
 * shift minus one (chosen or state), or the complement of that known zero.
 * Linked source retains its binding. */
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
            /* FAKEMATCH: keep the cancellation value's definition separate from the call result; without this boundary, pane-row stores exchange r2/r3 (12 instructions differ). */
            asm("" : "+r"(cancelled));
            if (chosen == cancelled) {
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
