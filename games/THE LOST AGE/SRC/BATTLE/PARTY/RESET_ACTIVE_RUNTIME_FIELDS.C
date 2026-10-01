/* Battle: clear the active and reserve party members' status bytes and stat
   modifiers; ⚓️ adds the reserve members ☀️ does not have.
   Returns int: its epilogue returns through r1. */
#include "TYPES.H"
#include "BATTLE_TYPES.H"

s32 BattleParty_PrepareActiveOwners(u16 *owners);
s32 BattleParty_PrepareReserveOwners(u16 *owners);
struct BattleUnit *Owner_GetState(s32 owner);
void BattleUnit_Recalculate(s32 owner);

s32 BattleParty_ResetActiveRuntimeFields(void)
{
    u16 owners[10];
    s32 count;
    s32 i;

    count = BattleParty_PrepareActiveOwners(owners);
    count += BattleParty_PrepareReserveOwners(owners + count);

    i = 0;
    if (i < count) {
        struct BattleUnit *unit;
        u8 *cursor;
        s32 remaining;

        do {
            unit = Owner_GetState(owners[i]);
            remaining = 3;
            cursor = &unit->status_12f;

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

            BattleUnit_Recalculate(owners[i]);
            i++;
        } while (i < count);
    }
}
