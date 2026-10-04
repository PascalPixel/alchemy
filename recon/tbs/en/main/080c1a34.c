/* 2026-10-04 (main-5): score 260, frame only. Plain for loops with an
 * unsigned member index and count/sum cleared before the record address
 * now give every instruction and register of the ROM; the only difference
 * is the ROM's unused 28-byte reserve (sub/add sp, #28), which shifts the
 * three return branches. 28 bytes is one word per call-crossing pseudo
 * (seven here); no ordinary source tried reproduces it without an unread
 * local, which S2 does not admit for a frame-size difference.
 *
 * 2026-10-03 BATTLE5 frame audit. Demoted in every edition under S1/S2/C1.
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
 * Rejected meaningful source trials:
 * - One context containing the seven consumed variables (formation,
 *   member index, level sum, count, enemy definition, enemy id and count
 *   cursor): 6365, with real field loads/stores. Every field was read.
 * - The same context declared register: unchanged 6365.
 * - A two-field level-sum/count accumulator: 2102, four-byte spill frame.
 * - All seven ordinary scalars given the outer function lifetime: 260.
 * - Signed member index: 270 (signed loop tests); record address set
 *   before count/sum: 270 (r9/r10 initialisation order).
 * The former unused reserve made this function exact, but supplied only
 * frame size and does not meet S2. Keep this draft uncredited until a
 * truthful frame model is recovered.
 */
#include "TYPES.H"
#include "BATTLE_FORMATION.H"
#include "CHARACTER.H"

struct EnemyDefinition *Owner_GetRecordFar(s32 id);
s32 GameFlag_IsSet(s32 flag);

s32 Party_ComputeEligibleMemberAverage(s32 record_id)
{
    struct BattleFormationRecord *record;
    struct EnemyDefinition *enemy;
    u32 i;
    s32 id;
    s32 sum;
    s32 count;

    count = 0;
    sum = 0;
    record = &BattleFormation_Records[record_id];
    for (i = 0; i < 5; i++) {
        if (record->minimum_counts[i] != 0)
            break;
    }
    if (i == 5)
        return -1;
    for (i = 0; i < 5; i++) {
        if (record->maximum_counts[i] == 0)
            continue;
        id = record->member_ids[i];
        enemy = Owner_GetRecordFar(id + 8);
        if (enemy == 0)
            continue;
        if (enemy->level > 3 && !GameFlag_IsSet(372) && !GameFlag_IsSet(id + 1544))
            return -2;
        sum += enemy->level;
        count++;
    }
    if (count == 0)
        return -3;
    return sum / count;
}
