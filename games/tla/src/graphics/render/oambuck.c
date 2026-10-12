#include "types.h"

extern u8 gOamBucketMasks[];
extern void *gOamBuckets[];

// 優先度 0〜239 のバケツの先頭に entry をつなぐ
void Func_080140d8(void *entry, s32 slot)
{
    u8 *mask;
    void **buckets;
    s32 bit;

    if ((u32)slot > 239) {
        if (slot < 0)
            slot = 0;
        else
            slot = 239;
    }
    mask = &gOamBucketMasks[(u32)slot >> 3];
    bit = slot & 7;
    buckets = gOamBuckets;
    if (((u32)*mask >> bit) & 1) {
        void **head = &buckets[slot];
        void *previous = *head;

        *head = entry;
        *(void **)entry = previous;
    } else {
        *mask |= 1 << bit;
        buckets[slot] = entry;
        *(void **)entry = NULL;
    }
}

// 優先度ごとのバケツの先頭に entry をつなぐ
void Runtime_PushSlotEntry(void *entry, s32 slot)
{
    u8 *mask;
    void **head;
    s32 bit;

    if (slot > 255)
        slot = 255;
    if (slot < 0)
        slot = 0;
    mask = &gOamBucketMasks[(u32)slot >> 3];
    head = &gOamBuckets[slot];
    bit = slot & 7;
    if (((u32)*mask >> bit) & 1) {
        void *previous = *head;

        *head = entry;
        *(void **)entry = previous;
    } else {
        *mask |= 1 << bit;
        *head = entry;
        *(void **)entry = NULL;
    }
}
