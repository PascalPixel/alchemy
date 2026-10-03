/* Near miss: score 220. ☀️'s, reading the track table through ⚓️'s
   Flash_Handler3 cell. ⚓️ scales the index (lsls r1, r0, #1) before loading
   the table pointer; this draft after, and moving the statement reallocates
   every register. */
#include "TRACKBUF.H"

void AudioTrack_InsertSlotNode(s32 slot)
{
    struct AudioTrackSlotWork *work;
    struct AudioSlotNode *node;
    struct AudioSlotNode **bucket;
    struct AudioSlotNode *next;

    work = Flash_Handler3;
    node = &work->nodes[slot];
    bucket = &work->bucket[work->bucket_by_slot[slot]];
    node->back = bucket;
    node->prev = *bucket;
    *bucket = node;
    next = node->prev;
    if (next != NULL)
        next->back = &node->prev;
}
