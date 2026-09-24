/* Battle: clear the active party members' status bytes and stat modifiers.
   Returns int: its epilogue returns through r1. */
#include "TYPES.H"
#include "BATTLE_TYPES.H"

s32 Func_080b6a60(u16 *owners);
struct BattleUnit *Func_08077008(s32 owner);
void Func_08077010(s32 owner);

s32 BattleParty_ResetActiveRuntimeFields(void)
{
    u16 owners[10];
    s32 count;
    s32 i;

    count = Func_080b6a60(owners);

    i = 0;
    if (i < count) {
        struct BattleUnit *unit;
        u8 *cursor;
        s32 remaining;

        do {
            unit = Func_08077008(owners[i]);
            cursor = &unit->status_12f;
            remaining = 3;

            do {
                remaining--;
                *cursor-- = 0;
            } while (remaining >= 0);

            unit->attack_modifier_turns = 0;
            unit->attack_modifier = 0;
            unit->defense_modifier_turns = 0;
            unit->defense_modifier = 0;
            unit->res_modifier_turns = 0;
            unit->res_modifier = 0;
            unit->delusion = 0;
            unit->confusion = 0;
            unit->charm = 0;
            unit->stun = 0;
            unit->sleep = 0;
            unit->psy_seal = 0;
            unit->refrain = 0;
            unit->reflect = 0;
            unit->death_count = 0;
            unit->unknown_142[0] = 0;
            unit->unknown_142[1] = 0;
            unit->ready_pose = 0;
            unit->cannot_move = 0;
            unit->agility_modifier_turns = 0;
            unit->agility_modifier = 0;
            unit->battle_end_state = 0;

            Func_08077010(owners[i]);
            i++;
        } while (i < count);
    }
}
