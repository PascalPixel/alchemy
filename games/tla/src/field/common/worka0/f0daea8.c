#include "worka0.h"

struct WorkA4Item *Func_080daea8(void)
{
    struct WorkA4 *work = WORK_A4;
    struct WorkA4Item *item = work->items;
    s32 index;

    for (index = 0; index <= 127; index++) {
        if (item->used == 0)
            return item;
        item++;
    }
    return NULL;
}
