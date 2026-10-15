#include "types.h"

struct BucketEntry {
    void *next;
    u8 unk04;
    u8 unk05[7];
};

extern struct BucketEntry *gData_0200752c;

void Func_081a18f8(s32 enable)
{
    struct BucketEntry *entry = gData_0200752c;
    u32 i;

    if (!enable) {
        for (i = 0; i < 8; i++) {
            entry->unk04 = 0;
            entry++;
        }
    } else {
        for (i = 0; i < 8; i++) {
            entry->unk04 = 120;
            entry++;
        }
    }
}
