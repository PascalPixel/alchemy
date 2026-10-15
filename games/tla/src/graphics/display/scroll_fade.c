#include "types.h"

struct BucketEntry {
    void *next;
    u8 unknown_04;
    u8 unknown_05[7];
};

extern struct BucketEntry *Data_0200752c;

void Func_081a18f8(s32 enable)
{
    struct BucketEntry *entry = Data_0200752c;
    u32 i;

    if (!enable) {
        for (i = 0; i < 8; i++) {
            entry->unknown_04 = 0;
            entry++;
        }
    } else {
        for (i = 0; i < 8; i++) {
            entry->unknown_04 = 120;
            entry++;
        }
    }
}
