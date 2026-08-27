#include "types.h"

#define runtime_owner_285 Func_0808b090

extern s16 Data_02000240[];

struct SceneInteractionEntry {
    s16 id;
    s16 value : 15;
    u16 alt_source : 1;
    s16 condition;
    s16 result;
};

extern const struct SceneInteractionEntry Data_0809d9f0[];

s32 GameFlag_IsSet(s32 flag);

void runtime_owner_285(void)
{
    s16 progress = Data_02000240[224];
    s16 sub = Data_02000240[225];
    const struct SceneInteractionEntry *entry = Data_0809d9f0;
    s16 alt = Data_02000240[230];
    s16 result = 18;

    for (; entry->id != -1; entry++) {
        if (entry->alt_source) {
            if (entry->id != progress) {
                continue;
            }
        } else {
            if (entry->id != alt) {
                continue;
            }
        }
        if (entry->value != -1 && entry->value != sub) {
            continue;
        }
        if (entry->condition != -1 && GameFlag_IsSet(entry->condition) == 0) {
            continue;
        }
        result = entry->result;
        break;
    }

    Data_02000240[248] = result;
}
