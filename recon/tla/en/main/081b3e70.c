/* Near miss: score 220. ☀️'s, reading the track table through ⚓️'s
   Flash_Handler3 cell. ⚓️ scales the index (lsls r1, r0, #1) before loading
   the table pointer; this draft after, and moving the statement reallocates
   every register. */
#include "TYPES.H"
extern u8 Flash_Handler3[];

struct AudioTrackSlotWork {
    u8 unknown0000[0x3404];
    s32 bucket_by_slot[0x400];
    u8 unknown4404[0x34];
    u32 input_cursor;
    s32 unknown443c;
    u32 input_limit;
};


void AudioTrack_InsertSlotNode(s32 index)
{
    s32 base;
    s32 node_off;
    s32 tbl_off;
    s32 bucket;
    s32 bucket_off;
    s32 link_off;
    void **node;
    void *next;

    base = *(s32 *)Flash_Handler3;
    node_off = index * 12;
    tbl_off = index * 4 + 0x3404;
    bucket = *(s32 *)(base + tbl_off) * 4;
    link_off = node_off + 4;
    *(s32 *)(base + link_off) = base + bucket + 0x3000;
    bucket_off = bucket + 0x3000;
    *(s32 *)(base + node_off) = *(s32 *)(base + bucket_off);
    node = (void **)(base + node_off);
    *(void **)(base + bucket_off) = node;
    next = *node;
    if (next != 0)
        ((void **)next)[1] = node;
}
