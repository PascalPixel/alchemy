#include "worka0.h"
#include "types.h"
#include "input.h"
#include "object_runtime.h"

extern s32 gData_080f3950;

void Func_080da060(s32 a0, s32 a1, s32 a2, s32 a3);

void Func_080da908(s32 index)
{
    struct WorkA0 *work = WORK_A0;
    struct WorkA0Node *node;

    for (node = work->slots[index].head; node != NULL; node = node->next)
        node->used = 0;
    work->slots[index].head = NULL;
}

void Func_080da938(s32 index)
{
    struct WorkA0 *work = WORK_A0;
    struct WorkA0Node *node;

    for (node = work->slots[index].head; node != NULL; node = node->next)
        node->used = 2;
    work->slots[index].tail = NULL;
}
