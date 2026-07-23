typedef unsigned char u8;
typedef signed int s32;
typedef unsigned int u32;

#define FIELD(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

extern void *Data_02004c00;

void Func_080f7df0(s32 index)
{
    s32 base;
    s32 nodeOffset;
    s32 tableOffset;
    s32 scaledBucket;
    s32 bucketOffset;
    s32 linkOffset;
    void **node;
    void *next;

    base = (s32)Data_02004c00;
    nodeOffset = index * 12;
    tableOffset = index * 4 + 0x3404;
    scaledBucket = *(s32 *)(base + tableOffset) * 4;
    linkOffset = nodeOffset + 4;
    *(s32 *)(base + linkOffset) = base + scaledBucket + 0x3000;
    bucketOffset = scaledBucket + 0x3000;
    *(s32 *)(base + nodeOffset) = *(s32 *)(base + bucketOffset);
    node = (void **)(base + nodeOffset);
    *(void **)(base + bucketOffset) = node;
    next = *node;
    if (next != 0)
        FIELD(next, void *, 4) = node;
}
