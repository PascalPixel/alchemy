#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "resource.h"
#include "dma.h"
#include "resource_ids.h"
#include "types.h"
#include "irq.h"
#include "scene.h"
#include "mem.h"
#include "global_cells.h"
#include "io_reg.h"

struct ResourceWork {
    u32 header[3];
    u32 cursor;
};

void Resource_LoadWorkHeader(void)
{
    const void *header;
    struct ResourceWork *work;

    header = (const void *)Resource_GetTableEntry((u32)&ResourceId_BuildStamp);
    work = (struct ResourceWork *)&Data_03007804;
    Dma_Set(header, work, 0x84000003, REG_DMA3);
    work->cursor = 0;
}

void RuntimeDispatch_NoOpHook(void)
{
}

void *Resource_GetTableEntry(s32 index)
{
    return Resource_DirectoryTable[index];
}

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
    routine = (void (*)(void *, s32))MemAlloc(size);
    Dma_Set((const void *)Resource_PatchThumbBranch, routine, 0x84000000 | (size >> 2), REG_DMA3);
    routine(destination, end);
    MemFree(routine);
}

void Runtime_IgnoreInterrupt(void)
{
}
