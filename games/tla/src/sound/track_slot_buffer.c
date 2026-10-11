#include "trackbuf.h"

void AudioTrack_RemoveSlotNode(s32 slot)
{
    if (Flash_Handler3->nodes[slot].prev != NULL) {
        if (Flash_Handler3->nodes[slot].next != NULL)
            Flash_Handler3->nodes[slot].next->prev = Flash_Handler3->nodes[slot].prev;
        Flash_Handler3->nodes[slot].prev->next = Flash_Handler3->nodes[slot].next;
    }
}
