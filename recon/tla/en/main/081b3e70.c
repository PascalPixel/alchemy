/* Near miss: score 220. ☀️'s, reading the track table through ⚓️'s
   Flash_Handler3 cell. ⚓️ scales the index (lsls r1, r0, #1) before loading
   the table pointer; this draft after, and moving the statement reallocates
   every register. */
#include "TRACKBUF.H"

void AudioTrack_InsertSlotNode(s32 index)
{
    /* FAKEMATCH: retain the existing address-word link stores and reloads.
       A direct typed-node insertion shrinks 68 to 60 bytes and changes
       the back-link store order. All offsets derive from the real owners. */
    s32 base;
    s32 node_off;
    s32 tbl_off;
    s32 bucket;
    s32 bucket_off;
    s32 link_off;
    void **node;
    void *next;

    base = (s32)Flash_Handler3;
    node_off = index * sizeof(struct AudioSlotNode);
    tbl_off = index * sizeof(s32) + (u32)&((struct AudioTrackSlotWork *)0)->bucket_by_slot;
    bucket = *(s32 *)(base + tbl_off) * 4;
    link_off = node_off + (u32)&((struct AudioSlotNode *)0)->back;
    *(s32 *)(base + link_off) = base + bucket + (u32)&((struct AudioTrackSlotWork *)0)->bucket;
    bucket_off = bucket + (u32)&((struct AudioTrackSlotWork *)0)->bucket;
    *(s32 *)(base + node_off) = *(s32 *)(base + bucket_off);
    node = (void **)(base + node_off);
    *(void **)(base + bucket_off) = node;
    next = *node;
    if (next != 0)
        ((struct AudioSlotNode *)next)->back = (struct AudioSlotNode **)node;
}
