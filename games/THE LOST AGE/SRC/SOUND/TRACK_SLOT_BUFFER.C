#include "TRACKBUF.H"

void AudioTrack_RemoveSlotNode(s32 slot)
{
    struct AudioSlotNode *node;
    struct AudioSlotNode **link;
    struct AudioSlotNode *next;

    node = &Flash_Handler3->nodes[slot];
    link = node->back;
    if (link != NULL) {
        next = node->prev;
        if (next != NULL)
            next->back = link;
        *link = node->prev;
    }
}
