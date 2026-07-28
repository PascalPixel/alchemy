#include "types.h"

struct SubQueueItem_08011b00 {
    u32 unknown_00;
    u16 unknown_04;
    u16 unknown_06;
    u16 unknown_08;
    u16 unknown_0a;
    u16 unknown_0c[16];
};

struct Queue_08011b00 {
    struct SubQueueItem_08011b00 slots[4];
    u16 count;
};

void *Func_080048f4(s32 arg0, s32 arg1);

void Func_08011b00(void)
{
    struct Queue_08011b00 *queue;
    struct SubQueueItem_08011b00 *entry;
    u16 i;
    u16 j;

    queue = (struct Queue_08011b00 *)Func_080048f4(28, sizeof(struct Queue_08011b00));
    entry = queue->slots;
    for (i = 0; i != 4; i++) {
        entry->unknown_00 = 0;
        entry->unknown_04 = 0;
        entry->unknown_06 = 0;
        entry->unknown_08 = 0;
        entry->unknown_0a = 0;
        for (j = 0; j != 16; j++) {
            entry->unknown_0c[j] = 0;
        }
        entry++;
    }
    queue->count = 0;
}
