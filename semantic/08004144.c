#include "types.h"

struct Record_08004144 {
    u32 first;
    s16 key;
    s16 second;
};

static inline void SortPass_08004144(
    struct Record_08004144 *record,
    s32 count)
{
    s32 inner;
    struct Record_08004144 swapped;

    if (count > 0) {
        inner = count;
        do {
            if (record[1].key > record[0].key) {
                __builtin_memcpy(&swapped, record, sizeof(swapped));
                __builtin_memcpy(record, record + 1, sizeof(swapped));
                record++;
                __builtin_memcpy(record, &swapped, sizeof(swapped));
            } else {
                record++;
            }
            inner--;
        } while (inner != 0);
    }
}

void Func_08004144(void)
{
    struct Record_08004144 *record;
    s32 outer;

    record = (struct Record_08004144 *)0x03001A20;
    outer = 19;
    goto check_outer;

next_outer:
    record = (struct Record_08004144 *)0x03001A20;
check_outer:
    SortPass_08004144(record, outer);

    outer--;
    if (outer > 1) {
        goto next_outer;
    }
}
