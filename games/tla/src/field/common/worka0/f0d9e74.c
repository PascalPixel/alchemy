#include "worka0.h"

struct WorkA0Node *Func_080d9e74(void)
{
    struct WorkA0 *work = WORK_A0;
    struct WorkA0Node *item = work->items;
    s32 index;

    for (index = 0; index <= 127; index++) {
        if (item->used == 0)
            return item;
        item++;
    }
    return NULL;
}
