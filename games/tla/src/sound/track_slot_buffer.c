#include "trackbuf.h"

void AudioTrack_InsertSlotNode(s32 index) {
  struct AudioTrackSlotWork *work = gFlash_Handler3;

  work->nodes[index].prev =
      (struct AudioSlotNode *)&work->bucket[work->bucketBySlot[index]];
  work->nodes[index].next = work->bucket[work->bucketBySlot[index]];
  work->bucket[work->bucketBySlot[index]] = &work->nodes[index];
  if (work->nodes[index].next != NULL)
    work->nodes[index].next->prev = &work->nodes[index];
}

void AudioTrack_RemoveSlotNode(s32 slot) {
  if (gFlash_Handler3->nodes[slot].prev != NULL) {
    if (gFlash_Handler3->nodes[slot].next != NULL)
      gFlash_Handler3->nodes[slot].next->prev =
          gFlash_Handler3->nodes[slot].prev;
    gFlash_Handler3->nodes[slot].prev->next = gFlash_Handler3->nodes[slot].next;
  }
}
