#include "types.h"

struct BattleResourceCondition {
    s16 id;
    s16 condition : 15;
    u16 use_effect_id : 1;
    void *resource;
};

s32 Func_0808a8d0(s32 effect_id);

void *Func_0808b158(s32 effect_id, s32 condition)
{
    const struct BattleResourceCondition *entry =
        (const struct BattleResourceCondition *)0x0809ddd8;
    s32 resource_group = Func_0808a8d0(effect_id);
    void *resource = 0;

    while (entry->id != -1) {
        if (entry->use_effect_id) {
            if (entry->id != effect_id)
                goto next;
        } else if (entry->id != resource_group) {
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
