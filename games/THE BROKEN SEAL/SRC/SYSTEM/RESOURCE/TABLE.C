#include "DMA.H"

u32 Resource_GetTableEntry(u32 index);
extern u8 Value_00000002;

struct ResourceWork {
    u32 header[3];
    u32 cursor;
};

void Resource_LoadWorkHeader(void)
{
    const void *header;
    struct ResourceWork *work;

    header = (const void *)Resource_GetTableEntry((u32)&Value_00000002);
    work = (struct ResourceWork *)0x03007804;
    Dma_Set(header, work, 0x84000003, (volatile u32 *)0x040000d4);
    work->cursor = 0;
}

#include "TYPES.H"

extern u32 Data_08320000[];

void RuntimeDispatch_NoOpHook(void)
{
}

u32 Resource_GetTableEntry(u32 index)
{
    return Data_08320000[index];
}

#include "TYPES.H"
#include "SCENE.H"

void PackedTable_AdjustMarkedOffsets(u16 *entries, s32 byte_length)
{
    s32 entry_count;
    s32 entry_index;

    entry_count = byte_length >> 1;
    entry_index = 1;
    entries++;
    if (entry_index < entry_count) {
        u32 next_marker_mask;
        u32 current_marker_mask;
        u32 payload_mask = 0x7ff;
        next_marker_mask = 0xf800;
        current_marker_mask = 0xf000;
        entries -= 2;
        do {
            u16 current_entry;
            u16 next_entry;
            u32 next_marker_bits;
            u32 current_marker_bits;

            next_entry = entries[2];
            next_marker_bits = next_entry & next_marker_mask;
            entries++;
            if (next_marker_bits == next_marker_mask) {
                current_entry = entries[0];
                current_marker_bits = current_entry & next_marker_bits;
                if (current_marker_bits == current_marker_mask) {
                    u32 packed_offset;
                    u32 adjusted_current;
                    u32 adjusted_next;

                    packed_offset = (((current_entry & payload_mask) << 12)
                        | ((next_entry & payload_mask) * 2)) - (entry_index * 2);
                    adjusted_current = ((packed_offset >> 12) & payload_mask)
                        | current_marker_bits;
                    adjusted_next = ((packed_offset >> 1) & payload_mask)
                        | next_marker_bits;
                    entries[0] = adjusted_current;
                    entries[1] = adjusted_next;
                }
            }
            entry_index++;
        } while (entry_index < entry_count);
    }
}

#include "DMA.H"
#include "RUNTIME_MEM.H"

u32 Resource_GetTableEntry(u32 index);
s32 Resource_DecodeType01(const void *source, void *destination);

/* The bl-pair fixer (PATCH_THUMB_BRANCH.S) is ARM code that runs from a heap
   copy of itself; the copy length is a link-time symbol. */
extern u8 Resource_PatchThumbBranchCodeSize[];

/* Decodes a code resource into place, then adjusts its Thumb calls for the
   address it now runs from. */
void Resource_LoadCode(u32 index, void *destination)
{
    void (*routine)(void *, s32);
    s32 end;
    u32 size;

    end = Resource_DecodeType01((const void *)Resource_GetTableEntry(index), destination);
    /* FAKEMATCH: the wrapper keeps the size load after the decode. */
    do {
        size = (u32)Resource_PatchThumbBranchCodeSize;
    } while (0);
    routine = (void (*)(void *, s32))Runtime_BumpAllocate(size);
    Dma_Set((const void *)0x8002d5c, routine, 0x84000000 | (size >> 2), (volatile u32 *)0x040000d4);
    routine(destination, end);
    Sys_Free(routine);
}
