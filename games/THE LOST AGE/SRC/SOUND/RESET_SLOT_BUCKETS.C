#include "TRACKBUF.H"

void AudioTrack_ResetSlotBuckets(void)
{
    /* FAKEMATCH: retain the original signed word stores at the node back-link
       and slot lanes. Direct node fields change register allocation at the
       same 60-byte TBS extent and shorten the TLA body from 60 to 56. */
    s32 index;
    s32 limit;
    s32 zero;
    u8 *record;
    s32 *slot;

    index = 0;
    limit = 0x3FF;
    zero = 0;
    record = (u8 *)&Flash_Handler3->nodes[0].back;
    do {
        *(s32 *)(record + sizeof(struct AudioSlotNode *)) = index;
        index++;
        *(s32 *)record = zero;
        record += sizeof(struct AudioSlotNode);
    } while (index <= limit);
    slot = (s32 *)Flash_Handler3->bucket;
    {
        s32 zero2 = 0;
        for (index = 0xFF; index >= 0; index--) {
            *slot++ = zero2;
        }
    }
}
