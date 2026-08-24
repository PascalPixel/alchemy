#include "types.h"

typedef struct Entry {
    s16 first;
    s16 second;
    s16 third;
    s16 result;
} Entry;

extern Entry Data_0809d7a8[];

s32 Func_08009268(void *, s32 *);
s32 GameFlag_IsSet(s32);
void Func_0808b2b0(s32 arg0);

s32 BattleEffect_LookupResult(void *arg0)
{
    s32 value;
    Entry *entry = Data_0809d7a8;
    s32 key = Func_08009268(arg0, &value);
    s32 result = 0;

    while (entry->first != -1) {
        if (entry->first == value &&
            (entry->second == -1 || entry->second == key) &&
            (entry->third == -1 || GameFlag_IsSet(entry->third) == 0)) {
            result = entry->result;
            break;
        }
        entry++;
    }
    Func_0808b2b0(key);
    return result;
}
