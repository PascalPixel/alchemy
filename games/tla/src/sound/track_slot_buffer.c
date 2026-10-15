#include "trackbuf.h"

void AudioTrack_InsertSlotNode(s32 index)
{
    struct AudioTrackSlotWork *work = Flash_Handler3;

    work->nodes[index].prev = (struct AudioSlotNode *)&work->bucket[work->bucketBySlot[index]];
    work->nodes[index].next = work->bucket[work->bucketBySlot[index]];
    work->bucket[work->bucketBySlot[index]] = &work->nodes[index];
    if (work->nodes[index].next != NULL)
        work->nodes[index].next->prev = &work->nodes[index];
}

void AudioTrack_RemoveSlotNode(s32 slot)
{
    if (Flash_Handler3->nodes[slot].prev != NULL) {
        if (Flash_Handler3->nodes[slot].next != NULL)
            Flash_Handler3->nodes[slot].next->prev = Flash_Handler3->nodes[slot].prev;
        Flash_Handler3->nodes[slot].prev->next = Flash_Handler3->nodes[slot].next;
    }
}
