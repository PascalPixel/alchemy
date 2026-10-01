/*
 * Draft: BattleCommand_SelectTargets is EXACT (492 bytes, 2026-10-02, wave 1b
 * slice 5) but cannot be adopted alone. It is a function nested in
 * BattleCommand_BuildPlan (listing 080be378), reaching the parent's plan,
 * target, battle, command and actor through the static chain in that order
 * (chain - 4 to chain - 20), so it only exists as part of the parent's C.
 * The parent below is a stand-in that declares those five and calls it;
 * the real one is the 080be378 draft, which is far from matching.
 *
 * alchemy drafts cannot parse a nested function definition, so this file is
 * reported as unscored; the bytes were compared by compiling it with the
 * build's command and comparing the nested function with the listing.
 *
 * What the listing fixed: both counting loops run in the index variable; the
 * range is read once and the first and last positions are locals of a block
 * opened after the centre is read (declared with the others, the last
 * position takes the address temporary's stack slot); the single-target
 * cases store offset, count, id, adjustment in that order.
 */
#include "TYPES.H"
#include "BATTLE_UNIT.H"
#include "BATTLE_WORK.H"
#include "BATTLE_RUNTIME.H"

/* A queued command: the 16-byte action record as the plan builder reads it. */
struct BattleCommand {
    s16 unit_id;                    /* 0x00 */
    s16 unknown_02;
    s16 unknown_04;
    s16 command;                    /* 0x06 */
    s16 parameter;                  /* 0x08 */
    u16 target;                     /* 0x0a: position in bits 0-3, bit 7 the enemy side */
    s16 range;                      /* 0x0c */
    s16 unknown_0e;
};

struct BattleAction *Ability_GetData(s32 action);
s32 Battle_GetTaggedSlotValue(s32 target);
void UiText_DrawQuantity(s32 value, s32 slot);
void UiText_ShowMessageAndWaitCoreFar(s32 message);

s32 BattleCommand_BuildPlan(struct BattleCommand *command, struct BattlePlan *plan)
{
    struct BattleUnit *actor;
    struct BattleSession *battle;
    s32 target;

    /* Fill the plan's targets for an action: the chosen unit alone, or every
     * listed unit within the command's range of its position on the chosen
     * side. Defeated units count only for the reviving effects. Returns -1,
     * after telling the player, when nobody is in range. */
    s32 BattleCommand_SelectTargets(s32 action_id)
    {
        struct BattleAction *action = Ability_GetData(action_id);
        s32 mode = action->target_mode;
        s32 allow_defeated = 0;
        s32 count;
        s32 allies;
        s32 enemies;
        s32 center;
        s32 range;
        s32 index;
        s32 unit;

        switch (action->effect) {
        case 5:
        case 56:
        case 57:
            allow_defeated = 1;
            break;
        }
        switch (mode) {
        case 0:
            plan->target_offsets[0] = mode;
            plan->target_count = 1;
            plan->target_ids[0] = target;
            plan->target_adjustments[0] = 1;
            break;
        case 4:
            plan->target_offsets[0] = 0;
            plan->target_count = 1;
            plan->target_ids[0] = target;
            plan->target_adjustments[0] = 1;
            break;
        default:
            count = 0;
            for (index = 0; battle->party_units[index] != 255; index++)
                ;
            allies = index;
            for (index = 0; battle->enemy_units[index] != 255; index++)
                ;
            enemies = index;
            center = command->target & 15;
            range = command->range;
            {
                s32 first = center - range + 1;
                s32 last = center + range - 1;

                for (index = first; index <= last; index++) {
                    if (index < 0)
                        continue;
                    if (command->target & 128) {
                        if (index >= enemies)
                            continue;
                        unit = battle->enemy_units[index];
                        if (unit == 254)
                            continue;
                        if (!allow_defeated && Owner_GetStateFar(unit)->hp == 0)
                            continue;
                        plan->target_adjustments[count] = 1;
                        plan->target_offsets[count] = index - center;
                        plan->target_ids[count] = unit;
                        count++;
                    } else {
                        if (index >= allies)
                            continue;
                        unit = battle->party_units[index];
                        if (unit == 254)
                            continue;
                        if (!allow_defeated && Owner_GetStateFar(unit)->hp == 0)
                            continue;
                        plan->target_adjustments[count] = 1;
                        plan->target_offsets[count] = index - center;
                        plan->target_ids[count] = unit;
                        count++;
                    }
                }
            }
            plan->target_count = count;
            if (count <= 0) {
                UiText_DrawQuantity(command->unit_id, 1);
                UiText_ShowMessageAndWaitCoreFar(0x816);
                if (actor->guard_level == 0)
                    actor->guard_level = 1;
                return -1;
            }
        }
    }

    actor = Owner_GetStateFar(command->unit_id);
    battle = gBattleWork;
    target = Battle_GetTaggedSlotValue(command->target);
    return BattleCommand_SelectTargets(command->parameter);
}
