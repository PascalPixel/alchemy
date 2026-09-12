#include "types.h"

#define FIELD(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

extern void *Data_02004c00;

void AudioTrack_InsertSlotNode(s32 index)
{
    s32 base;
    s32 node_off;
    s32 tbl_off;
    s32 bucket;
    s32 bucket_off;
    s32 link_off;
    void **node;
    void *next;

    base = (s32)Data_02004c00;
    node_off = index * 12;
    tbl_off = index * 4 + 0x3404;
    bucket = *(s32 *)(base + tbl_off) * 4;
    link_off = node_off + 4;
    *(s32 *)(base + link_off) = base + bucket + 0x3000;
    bucket_off = bucket + 0x3000;
    *(s32 *)(base + node_off) = *(s32 *)(base + bucket_off);
    node = (void **)(base + node_off);
    *(void **)(base + bucket_off) = node;
    next = *node;
    if (next != 0)
        FIELD(next, void *, 4) = node;
}
