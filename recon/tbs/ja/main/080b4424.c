/* DRAFT: Automatic command selection and its original nested weighted
 * selector. Current EN score 140: natural nested target is unresolved to the
 * scorer (20); one conditional edge differs (20); one redundant unconditional
 * branch is absent (100). No instruction/register/order differences remain
 * elsewhere. The compiler emits 62-byte helper plus 2-byte alignment and a
 * complete 896-byte outer function, including both literal pools. Native
 * branch near its non-retry command check is replaced by alignment padding
 * before a later pool; this is not an exact match and earns no new credit.
 * Ordinary control-flow shapes all collapse that branch; no barrier, dummy
 * storage or inline instruction is introduced. The r6 choice below is the
 * only new FAKEMATCH. Before adoption, reconcile the local automatic-state
 * view with its producer and replace the standalone helper naturally.
 * Integer return declaration follows the native r0-preserving epilogue;
 * every current caller discards the unspecified return value.
 * T0: canonical owners/nested helper score 10669 against EN; table labels
 * resolve, nested call remains a scorer-only unresolved local symbol.
 * T1: later selection init/direct retry scored 10797.
 * T2: target call results are full words in the native branches, and each
 * weighted case owns a nested call; remove the artificial common goto.
 * T2 score 8104. T3: retain loaded action IDs as full words and update
 * the saved selection through its already-established byte view.
 * T3 score 5166. T4: keep target flags promoted, and give each effect
 * and target case its complete source action instead of cross-case gotos.
 * T4 score 4399. T5: represent the packed initialized/selection bits
 * as a named subrecord view, reflecting both native reads and writes.
 * T5 score 3432. T6: separate candidate item flag from retry-item policy
 * after selecting its action, following the state used by the next branch.
 * T6 score 3318. T7: express item acceptance as ordinary if statements
 * instead of a comma-expression chain with short-circuit side effects.
 * T7 unchanged 3318. T8: simplify legality checks, eliminating manual
 * duplicate halfword/shift temporaries and cross-branch target labels.
 * T8 score 1528. T9: reverse the six spilled-local declarations; GCC
 * allocates these in reverse order, which explains the native frame slots.
 * T9 score 1507: all live frame slots now match (two aligned stores only
 * reflect different initialization scheduling). T10 shared-flag init unchanged.
 * T11: group item storage before its availability flag to test the opposite
 * saved-register lifetimes observed for the lookup and decision; unchanged.
 * T12: native return uses r1 to preserve r0, consistent with an integer
 * result whose value all known callers discard; score 1617.
 * T14 explicit discarded return unchanged. T15 pointer-order trials
 * (strategy,availability,state): 012:1330; 021:1130; 102:1315; 120:1166; 201:940; 210:982.
 * T16 removes redundant masks: score 635. T17: load candidate action
 * before policy scored 1301; restored. T18: test register hint for the
 * candidate flag after ordinary local ordering failed to allocate native r6;
 * unchanged 635. T19 pinned flag: score 570. T20: branch positively
 * on supported item target flags, rejoining their shared test; score 870
 * due interval folding. T21 separate branches restore score 570.
 * T22 enum-like switch: score 350. T23: apply item policy before
 * fetching its candidate action: score 200. T24: capture item action before
 * writing the command/enumerating non-retry commands: score 140.
 * T25 equivalent non-retry branch forms (conjunction, nested, early goto,
 * split switch, nested switch, common goto): 0:140; 1:140; 2:140; 3:140; 4:140; 5:140.
 * T26: capture command for the initial legality tests, then reload after
 * the action call scored 955, so restore direct reads.
 * T13 init-order trials (attempt,flags,selection): 012:1617; 021:1696; 102:1330; 120:1810; 201:1480; 210:1700.
 */
#include "TYPES.H"
#include "BATTLE_COMMAND.H"
#include "BATTLE_RUNTIME.H"
#include "BATTLE_TYPES.H"
#include "CHARACTER.H"

struct EnemyDefinition *Owner_GetRecordFar(s32 class_id);
struct BattleAction *Ability_GetData(s32 action_id);
s32 Battle_FindTaggedSlotByValue(s32 actor_id);
s32 BattleTarget_SelectForAction(s32 actor_id, struct BattleAction *action);
s32 BattleTarget_SelectRandomPosition(s32 side);
extern u8 BattleCommand_SelectionWeights[3][8];

/* Low bits of the combatant word copied from its enemy definition. */
struct BattleAutomaticState {
    unsigned initialized : 1;
    unsigned selection : 3;
    unsigned remaining : 28;
};

/*
 * Choose a legal automatic command for one combatant.  The class profile at
 * Owner_GetRecordFar supplies a strategy byte at +0x36, availability bits at
 * +0x37, and the candidate action IDs beginning at +0x38.
 */
s32 BattleCommand_SelectAutomatic(
    struct BattleActionRecord *request,
    s32 retry)
{
    register u32 selected;
    struct BattleAutomaticState *auto_state;
    struct EnemyDefinition *profile;
    s32 allow_item;
    s32 attempt;
    s32 keep_going;
    s8 *selection_mode;
    u8 *availability;
    s16 command;
    s32 target;
    s32 alternate_target;
    s32 side;
    s32 alternate_side;
    struct ItemDefinition *item;
    /* Plain locals, declaration grouping and a register hint keep
     * the candidate flag in r5 and item pointer in r6. Native uses the opposite
     * saved registers throughout item validation; this pin changes only that
     * allocation and the dependent scratch-register schedule. */
    /* FAKEMATCH: Native flag in r6 after plain shapes failed; score 635 -> 570. */
    register s32 item_available asm("r6");
    s8 mode;
    struct BattleAction *item_action;
    struct BattleAction *action;
    struct BattleUnit *actor;
    s32 action_id;
    s32 target_flags;
    u8 effect;
    u8 target_mode;
    s32 Battle_SelectWeightedIndex(u8 *weights)
    {
        s32 value;
        s32 total;
        s32 result;
        s32 index;

        value = BattleRandom16Far() & 0xff;
        total = weights[0];
        result = 0;
        index = 0;
        if (value >= total) {
        loop:
            index++;
            if (index <= 7) {
                total += weights[index];
                if (value < total)
                    result = index;
                else
                    goto loop;
            }
        }
        return result;
    }

    actor = Owner_GetStateFar(request->unit_id);
    keep_going = allow_item = 1;
    attempt = 0;
    selected = -1U;
    if (actor->class_index != 0)
        return;
    if (retry != 0 && request->command != 4)
        return;

    profile = Owner_GetRecordFar(actor->class_id);
    auto_state = (struct BattleAutomaticState *)&actor->rewards;
    selection_mode = &profile->action_strategy;
    availability = &profile->item_actions;

select_again:
    mode = *selection_mode;
    switch (mode) {
    case 0:
        selected = Battle_SelectWeightedIndex(BattleCommand_SelectionWeights[0]);
        break;
    case 1:
        selected = Battle_SelectWeightedIndex(BattleCommand_SelectionWeights[1]);
        break;
    case 2:
        selected = Battle_SelectWeightedIndex(BattleCommand_SelectionWeights[2]);
        break;
    case 3:
        if (auto_state->initialized == 0) {
            auto_state->selection = BattleRandom16Far();
            auto_state->initialized = 1;
        }
read_saved_selection:
        selected = auto_state->selection;
        if (retry != 0) {
            auto_state->selection = selected + 1;
        }
        break;
    case 4:
        goto read_saved_selection;
    case 5:
        selected++;
        break;
    case 6:
        break;
    }

    item_available = ((s32)*availability >> selected) & 1;
    item_available &= allow_item;
    action_id = profile->actions[selected];
    request->command = 4;
    if (item_available != 0 && retry != 0) {
        if (!(0x1ff & actor->inventory[0])) {
            item_available = 0;
            if (profile->target_strategy == 0) {
                request->command = 2;
                request->parameter = 0x1fd;
                return;
            }
        }
        if (item_available != 0) {
            item = Item_Get(actor->inventory[0]);
            if (item->use_type == 1) {
                item_action = Ability_GetData(item->action_id);
                action_id = item->action_id;
                request->command = 2;
                target_flags = item_action->target_flags;
                request->parameter = 0;
                switch (target_flags) {
                case 1:
                case 2:
                    goto item_checked;
                }
            }
            item_available = 0;
item_checked:
            if (item_available == 0)
                goto disable_item;
        } else {
disable_item:
            allow_item = 0;
        }
    }

    if (keep_going == 0)
        goto finish_target;

    action = Ability_GetData(action_id);
    effect = action->effect;
    switch (effect) {
    case 46:
        request->command = 3;
        request->target = Battle_FindTaggedSlotByValue(request->unit_id);
        break;
    case 47:
        request->command = 7;
        request->target = Battle_FindTaggedSlotByValue(request->unit_id);
        break;
    case 49:
        request->command = 99;
        request->target = Battle_FindTaggedSlotByValue(request->unit_id);
        break;
    }

    if (retry == 0) {
        switch (request->command) {
        case 3:
        case 7:
            break;
        default:
            return;
        }
    }

    if (request->command != 2 && Ability_CheckStatusOrSpecialId(action_id) != 0) {
        request->command = 1;
        request->parameter = action_id;
        if (action->pp_cost > actor->pp && profile->target_strategy != 0)
            goto next_candidate;
        if (actor->psy_seal != 0 && profile->target_strategy == 2)
            goto next_candidate;
    }
    command = request->command;
    if (command == 99 && (u8)actor->battle_end_state != 0)
        goto next_candidate;
    if (retry != 0 && (command == 3 || command == 7))
        goto next_candidate;

    if (command == 4) {
        request->parameter = action_id;
        if (action_id == 1)
            request->command = 0;
    }

    request->range = action->range;
    target_mode = action->target_mode;
    switch (target_mode) {
    case 2:
    case 4:
        target = BattleTarget_SelectForAction(request->unit_id, action);
        if (target == -2) {
            side = 0;
            if ((u32)(u16)request->unit_id <= 7U)
                side = 1;
            target = BattleTarget_SelectRandomPosition(side);
        }
        if (target != -1) {
            request->target = target;
            keep_going = 0;
        }
        break;
    case 1:
        alternate_target =
            BattleTarget_SelectForAction(request->unit_id, action);
        if (alternate_target == -2) {
            alternate_side = 0;
            if ((u32)(u16)request->unit_id <= 7U)
                alternate_side = 1;
            alternate_target = BattleTarget_SelectRandomPosition(alternate_side);
        }
        if (alternate_target != -1) {
            request->target = alternate_target;
            keep_going = 0;
        }
        break;
    case 3:
        request->target = Battle_FindTaggedSlotByValue(request->unit_id);
        break;
    default:
        alternate_target = Battle_FindTaggedSlotByValue(request->unit_id);
        request->target = alternate_target;
        keep_going = 0;
        break;
    }

finish_target:
    if (retry == 0)
        keep_going = 0;
next_candidate:
    if (keep_going != 0 && attempt > 16) {
        request->command = 3;
        keep_going = 0;
    }
    attempt++;
    if (keep_going != 0)
        goto select_again;
    return;
}
