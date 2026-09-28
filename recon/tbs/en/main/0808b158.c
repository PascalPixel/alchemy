/* NONMATCHING: complete 128-byte owner [0808b158,0808b1d8) with its pool;
 * 128 of 128 bytes, 21 differing halfwords (2026-09-28).
 *
 * The rule table is the 8-byte-entry list at 0809ddd8, inside the data the
 * scaffold still labels Scene_InteractionRuleTable (+0x3e8); it needs its
 * own label there before adoption. Scored here through that offset.
 *
 * Residual, all allocation: the reference keeps effect_id in r7, condition
 * in r6, the result in r8 (cleared before the group call) and the group in
 * ip; this candidate gives effect_id r6, condition r8, group r7 and the
 * result lr (cleared after the call). Declaring the result before the call
 * clears it early as the reference does, but then the result takes sl and
 * an extra register is saved (132 bytes, 63 halfwords), because global
 * allocation still orders effect_id (4 weighted refs over 39 insns) ahead
 * of condition (3 over 38): the reference needs condition allocated first.
 * Ternary, goto/continue and combined-condition spellings of the id test
 * and every order of the three initialised locals were tried.
 */
#include "TYPES.H"

struct BattleResourceCondition {
    s16 id;
    s16 condition : 15;
    u16 use_effect_id : 1;
    void *resource;
};

extern const u8 Scene_InteractionRuleTable[];

s32 BattleFx_GetResourceGroup(s32 effect_id);

void *BattleFx_FindConditionResource(s32 effect_id, s32 condition)
{
    const struct BattleResourceCondition *entry =
        (const struct BattleResourceCondition *)(Scene_InteractionRuleTable + 0x3e8);
    s32 group = BattleFx_GetResourceGroup(effect_id);
    void *resource = 0;

    while (entry->id != -1) {
        if (entry->use_effect_id) {
            if (entry->id != effect_id)
                goto next;
        } else if (entry->id != group) {
            goto next;
        }

        if (entry->condition == -1 || entry->condition == condition) {
            resource = entry->resource;
            break;
        }

next:
        entry++;
    }

    return resource;
}
