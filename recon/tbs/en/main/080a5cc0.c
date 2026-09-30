/* NONMATCHING, 2026-09-30 (helper hL): 800/800 bytes with the padding; 6
 * differing halfwords. Its seven messages now carry names in all six
 * catalogs (MsgPsynergyChooseOwner and the rest). An r2 clobber before the mode
 * switch (FAKEMATCH below) stops reload_cse_move2add turning the 0x268
 * constant into adds r2, #80, which fixed the 6-byte shift. Left: (1) the
 * 0x174 clear allocates address r3 / zero r2 where the ROM has r2 / r3:
 * block 3 has exactly three local qtys (address, zero, message) and
 * local-alloc's three-qty ordering compares fixed qty numbers, so the
 * address (born first) is allocated first; the ROM needs the zero born
 * first or a fourth local qty. (2) case 0 sets result from the call value
 * (mov fp, r0) where the ROM uses the -1 register (mov fp, r3): CSE's jump
 * equivalence makes the call copy canonical; tried statement order,
 * switch, inverted if, constant-first, a slot local, a one-pass loop and
 * an asm-hidden -1, none changes it.
 *
 * 2026-09-29: the two do-while(0) FAKEMATCH wrappers are no longer needed
 * (1855 -> 1775 without them) and callees carry the build's names; alchemy
 * permute scores 1575. Eight minutes of permutation from there found 650
 * with two natural changes: case 4 clears self_flag before reading the
 * action, and its final test puts the success branch first. Message 0xbef
 * is the text build's MsgItemUseResult: 610. Seven more message numbers
 * (0xae2..0xaf1) are still Value_ symbols and cost 140 of that. The rest:
 * the 0x174 halfword clear swaps r2/r3, and the reference forms r7 + 0x268
 * from its own constant where this adds 80 to a neighbouring offset
 * (move2add reuses the 0x218 already in r2; reading the mode through a
 * local or reordering the case does not stop it). A second 8-minute run
 * from 610 with another seed found nothing lower. */
/* NONMATCHING, 2026-09-26: 794/800 bytes, 323 differing halfwords, 98
 * aligned edits (previously 800/800, 239/120). Shared return types audited.
 * ROM corrections: case 0 clears the halfword at 0x174, not selected_action
 * at 0x178; case 4 calls ApplyTargets before replacing target 9 with the
 * acting owner; the final flag test uses an immediate 0x150, not a pool
 * value. Initialising state/done before loading work matches the opening
 * stores. Reusing self_flag for the case-3 response reproduces the sl
 * carrier and puts the returned result in fp, as in the reference.
 * Still missing six bytes, with block scheduling and register differences.
 * The first body difference is the r2/r3 pair in the 0x174 zero store;
 * case 0 also copies the known -1 from r0 where the ROM copies r3.
 * These are separate residuals, not reasons to restore the incorrect
 * field or call order. Remaining FAKEMATCH wrappers only move scheduling. */
#include "TYPES.H"
#include "FAR_RUNTIME.H"
#include "BATTLE_RUNTIME.H"
#include "BATTLE_CALC.H"

/*
 * gMenuWork is the polymorphic menu-runtime cell (see item_menu.h /
 * psynergy_menu.h). This owner reads and writes fields shared by both the
 * Inventory and Psynergy menu views (item_owner/target_owner at 0x21a/0x21b,
 * info_window at 0x2c, the selected id at 0x178, entry_count at 0x218), plus
 * two fields not yet named in either shared header (a byte "mode" at 0x268
 * that selects between three confirmation messages, and a u16 flags word at
 * 0x220). A local view is used here instead of extending the shared structs,
 * matching the project's convention for an owner-specific field range
 * (compare games/THE BROKEN SEAL/SRC/GAME/ITEM/USE.C's local ItemUseWork).
 */
struct MenuActionWork {
    u8 unknown_000[0x24];
    s32 field_024;             /* 0x024 */
    u8 unknown_028[4];
    s32 info_window;           /* 0x02c */
    u8 unknown_030[0x144];
    u16 field_174;             /* 0x174, cleared before party selection */
    u8 unknown_176[2];
    u16 selected_action;       /* 0x178 */
    u8 unknown_17a[0x9e];
    u8 entry_count;            /* 0x218 */
    u8 unknown_219;
    u8 item_owner;             /* 0x21a */
    u8 target_owner;           /* 0x21b */
    u8 unknown_21c[4];
    u16 flags_220;             /* 0x220 */
    s16 completion_flag;       /* 0x222 */
    u8 unknown_224[0x36];
    s16 message_offset;        /* 0x25a */
    u8 unknown_25c[0x0c];
    u8 mode;                   /* 0x268 */
};

extern struct MenuActionWork *gMenuWork;

extern char MsgShortcutSetL;
extern char MsgShortcutSetR;
extern char MsgPsynergyChooseOwner;
extern char MsgPsynergyChooseAbility;
extern char MsgPsynergyChooseTarget;
extern char MsgPsynergyChooseForR;
extern char MsgPsynergyChooseForL;
extern char MsgItemUseResult;

void WaitFrames(s32 frames);
void ItemMenu_DrawMsg(s32 unused, s32 message);
s32 PsynergyMenu_SelectPartySlot(s32 unused);
void ItemMenu_PosCategory(void);
void Menu_DrawOwnerStatusPanel(s32 window, s32 owner, s32 unused0, s32 unused1);
s32 PsynergyMenu_RunList(s32 unused);
s32 PsynergyMenu_SetShortcut(s32 owner, s32 psynergy, s32 shortcut);
s32 RenderOutput_ClearListFar(s32 window);
s32 InventoryMenu_ShowModalMessage(s32 message, s32 arg1, s32 arg2);
s32 PsynergyMenu_SelectTarget(s32 unused);
s32 PsynergyMenu_ClassifySelectedPsynergy(void);
s32 BattleEffect_ApplyToTargets(s32 action, s32 owner, s32 target, s32 flags);
void Ability_PlayUseAnimation();
void Audio_PlayCue(s32 cue);

/*
 * State machine that resolves the currently selected item/Psynergy command:
 * state 0 primes the category window and waits for the underlying selection
 * loop to finish; state 1 waits for a target/shortcut pick; state 2
 * classifies the selected Psynergy (immediate vs needs-target vs
 * needs-confirm); state 3 asks for a final confirmation; state 4 actually
 * applies the action, pays its PP cost, and reports success or failure. On
 * a clean finish (state 2's default) the acting owner and the selected
 * action id are written back through the two out-parameters.
 */
s32 Menu_ResolveSelectedAction(s32 *out_owner, s32 unused, s32 *out_action)
{
    struct MenuActionWork *work;
    s32 result;
    s32 state;
    s32 done;
    s32 selection;
    s32 classification;
    s32 raw;
    s32 self_flag;
    struct BattleAction *action;

    state = 0;
    done = 0;
    work = gMenuWork;
    result = 0;

    while (done == 0 && GameFlag_TestFar(0x150) == 0) {
        switch (state) {
        case 0:
            work->field_174 = 0;
            ItemMenu_DrawMsg(0, (s32)&MsgPsynergyChooseOwner);
            if (PsynergyMenu_SelectPartySlot(0) == -1) {
                done = 1;
                result = -1;
            }
            RenderOutput_RedrawSavedRectFar(work->info_window);
            state = 1;
            break;

        case 1:
            WaitFrames(1);
            Owner_GetStateFar(work->item_owner);
            state = 0;
            if (work->entry_count != 0) {
                /* FAKEMATCH: clobbering r2 hides the 0x218 it holds from reload_cse_move2add, so the 0x268 mode offset is built from its own constant (movs/lsls) instead of adds r2, #80. */
                asm("" : : : "r2");
                switch (work->mode) {
                case 0:
                    ItemMenu_DrawMsg(0, (s32)&MsgPsynergyChooseAbility);
                    break;
                case 1:
                    ItemMenu_DrawMsg(0, (s32)&MsgPsynergyChooseForL);
                    break;
                case 2:
                    ItemMenu_DrawMsg(0, (s32)&MsgPsynergyChooseForR);
                    break;
                }
                ItemMenu_PosCategory();
                Menu_DrawOwnerStatusPanel(work->field_024, work->item_owner, 0, 0);
                selection = PsynergyMenu_RunList(0);
                state = 0;
                if (selection != -1) {
                    state = 2;
                    if (work->mode != 0) {
                        if (work->mode == 1) {
                            PsynergyMenu_SetShortcut(
                                work->item_owner, selection, 0);
                            RenderOutput_ClearListFar(work->info_window);
                            InventoryMenu_ShowModalMessage(
                                (s32)&MsgShortcutSetL, -1, -1);
                        } else {
                            PsynergyMenu_SetShortcut(
                                work->item_owner, selection, 1);
                            RenderOutput_ClearListFar(work->info_window);
                            InventoryMenu_ShowModalMessage(
                                (s32)&MsgShortcutSetR, -1, -1);
                        }
                        state = 0;
                    }
                }
            }
            break;

        case 3:
            ItemMenu_DrawMsg(0, (s32)&MsgPsynergyChooseTarget);
            self_flag = PsynergyMenu_SelectTarget(0);
            state = 4;
            if (self_flag == -1) {
                work->flags_220 |= 1;
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
                work->target_owner = 9;
                state = 4;
                break;
            }
            done = 1;
            result = 1;
            *out_owner = work->item_owner;
            *out_action = work->selected_action & 0x3fff;
            break;

        case 4:
            self_flag = 0;
            raw = work->selected_action;
            result = BattleEffect_ApplyToTargets(
                raw, work->item_owner, work->target_owner, 0);
            if (work->target_owner == 9) {
                work->target_owner = work->item_owner;
                self_flag = 9;
            }
            if (result != -1) {
                action = BattleAction_Get(work->selected_action & 0x3fff);
                Owner_AdjustSecondValueFar(work->item_owner, -action->pp_cost);
            }
            BattleUnit_Recalculate(work->item_owner);
            if (result != -1) {
                Menu_DrawOwnerStatusPanel(work->field_024, work->target_owner, 0, 0);
                Ability_PlayUseAnimation(work->selected_action & 0x3fff);
                RenderOutput_ClearListFar(work->info_window);
                InventoryMenu_ShowModalMessage(
                    work->message_offset + (s32)&MsgItemUseResult, 0, -1);
            } else {
                Audio_PlayCue(114);
                RenderOutput_ClearListFar(work->info_window);
                InventoryMenu_ShowModalMessage(
                    work->message_offset + (s32)&MsgItemUseResult,
                    result,
                    result);
            }
            if (result != -1) {
                result = 1;
                work->flags_220 |= 1;
                state = 1;
            } else {
                work->completion_flag = 1;
                if (self_flag == 9) {
                    work->flags_220 |= 1;
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
