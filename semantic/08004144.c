#include "types.h"

struct Record_08004144 {
    u32 first;
    s16 key;
    s16 second;
};

void Func_08004144(void)
{
    struct Record_08004144 *record;
    s32 outer;
    s32 inner;

    for (outer = 19; outer > 1; outer--) {
        record = (struct Record_08004144 *)0x03001A20;
        for (inner = outer; inner > 0; inner--) {
            if (record[1].key > record[0].key) {
                struct Record_08004144 swapped = record[0];
                record[0] = record[1];
                record[1] = swapped;
            }
            record++;
        }
    }
}
