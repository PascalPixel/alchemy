#include "types.h"
#include "scene.h"

/* battle/effects/data/lookup_result.c */
typedef struct Entry {
    s16 first;
    s16 second;
    s16 third;
    s16 result;
} Entry;

extern Entry gRom[];

s32 GameFlag_IsSet(s32 flag);
void BattleFx_SelectResultPointer(s32 arg0);

s32 BattleFx_LookupResult(void *arg0)
{
    s32 value;
    Entry *entry = gRom;
    s32 key = SceneData_Apply(arg0, &value);
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
    BattleFx_SelectResultPointer(key);
    return result;
}
