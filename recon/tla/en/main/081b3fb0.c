/* Near miss: differ 2 of 31, The Broken Seal's AudioTrack_CopyBufferedBytes. In the loop the native steps the source pointer (adds r1, #1) before it reloads outTotal after the byte store; sched2 here reloads first, as the store-to-load dependence costs it one cycle. One-pass blocks or empty asm around either statement (16 combinations), an explicit source pointer (differ 21), the index stepped before the store with a barrier after it (differ 4) and three permuter minutes found nothing better. */
#include "trackbuf.h"

void AudioTrack_CopyBufferedBytes(u8 *destination)
{
    struct AudioTrackSlotWork *work = gFlash_Handler3;
    s32 index;

    for (index = 0; index != work->outCnt; index++) {
        destination[work->outTotal] = work->outBuf[index];
        work->outTotal = work->outTotal + 1;
    }
}
