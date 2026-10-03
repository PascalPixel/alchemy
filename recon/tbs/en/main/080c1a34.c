/* 2026-10-03 BATTLE5 frame audit. Demoted in every edition under S1/S2/C1.
 * Party_ComputeEligibleMemberAverage is 200 bytes, literal pool included,
 * in all six owned ROMs. Its 196-byte instruction body refers to sp only
 * when reserving/releasing 28 bytes; it never reads, writes or passes an
 * address within that frame. The four-byte formation-table pool is data.
 * Native entries JA/EN/DE/ES/FR/IT: 080b8a34, 080c1a34, 080c404c,
 * 080c5a4c, 080c5a4c, 080c1a4c. The uncredited raw listing is from EN.
 *
 * The sole current caller, BattleFormation_SelectLevelMatchedCandidate,
 * uses only the returned average and discards every negative result.
 * Owner_GetRecordFar returns the constant enemy definition; GameFlag_IsSet
 * reads a flag and accepts no output buffer. Neither consumes caller-local
 * storage. The division takes the sum and count in registers.
 *
 * Retained ordinary draft: no scratch, no device, 196 bytes; score 260,
 * two deleted stack-adjustment instructions and three return branches
 * shortened by their deletion. All substantive instructions/registers
 * otherwise agree. The former unused reserve made this function exact,
 * but supplied only frame size and does not meet S2.
 *
 * Rejected meaningful source trials:
 * - One context containing the seven consumed variables (formation,
 *   member index, level sum, count, enemy definition, enemy id and count
 *   cursor): 6365, with real field loads/stores. Every field was read.
 * - The same context declared register: unchanged 6365.
 * - A two-field level-sum/count accumulator: 2102, four-byte spill frame.
 * - All seven ordinary scalars given the outer function lifetime: 260,
 *   identical to this scoped form, with no local frame.
 * No truthful consumed-state model tested explains the 28-byte reserve.
 * Keep this draft uncredited until a truthful frame model is recovered.
 * Runtime_RemoveIrqHandlerSlot2 remains C in the preceding existing
 * ACT_RUN_ZERO module; the remaining BATTLE5 functions stay together.
 * All twelve edition comparisons pass. The 200-byte function leaves C in
 * each TBS edition; moving the IRQ helper makes the preceding two-byte
 * alignment compiler-owned. Net DONE delta: -198 per edition, -1188 across
 * six; FAKEMATCH-steered C falls by 1200. No count rule changes are involved.
 */
#include "TYPES.H"
#include "BATTLE_FORMATION.H"
#include "CHARACTER.H"

struct EnemyDefinition *Owner_GetRecordFar(s32 id);
s32 GameFlag_IsSet(s32 flag);

s32 Party_ComputeEligibleMemberAverage(s32 record_id)
{
    struct BattleFormationRecord *record;
    s32 member_index;
    s32 level_sum;
    s32 eligible_count;

    eligible_count = 0;
    level_sum = 0;
    record = &BattleFormation_Records[record_id];

    member_index = 0;
    if (record->minimum_counts[0] == 0) {
        u8 *present;

        present = record->minimum_counts;
        do {
            member_index++;
            if ((u32)member_index > 4)
                break;
            present++;
        } while (*present == 0);
    }
    if (member_index == 5)
        return -1;

    member_index = 0;
    do {
        if (record->maximum_counts[member_index] != 0) {
            struct EnemyDefinition *object;
            s32 member;

            member = record->member_ids[member_index];
            object = Owner_GetRecordFar(member + 8);
            if (object != 0) {
                if (object->level <= 3 ||
                    GameFlag_IsSet(372) != 0 ||
                    GameFlag_IsSet(member + 1544) != 0) {
                    level_sum += object->level;
                    eligible_count++;
                } else {
                    return -2;
                }
            }
        }
        member_index++;
    } while ((u32)member_index <= 4);

    if (eligible_count == 0)
        return -3;
    return level_sum / eligible_count;
}
