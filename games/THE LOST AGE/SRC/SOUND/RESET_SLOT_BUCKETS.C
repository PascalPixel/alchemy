#include "TRACKBUF.H"

void AudioTrack_ResetSlotBuckets(void)
{
    struct AudioSlotNode *node;
    struct AudioSlotNode **bucket;
    s32 index;

    node = Flash_Handler3->nodes;
    index = 0;
    do {
        node->slot = index;
        index++;
        node->back = NULL;
        node++;
    } while (index <= 0x3ff);
    bucket = Flash_Handler3->bucket;
    for (index = 0xff; index >= 0; index--)
        *bucket++ = NULL;
}
