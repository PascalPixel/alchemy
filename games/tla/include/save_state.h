#ifndef TLA_SAVE_STATE_H
#define TLA_SAVE_STATE_H

#include "types.h"
#include "layout_guard.h"

struct SaveSummary {
    u8 unknown_00[0x38];
    u8 unknown_38[4];
    u32 checksum;
};

struct SaveWorkspace {
    u8 occupied[15];
    u8 record_id[15];
    u16 sequence[15];
    u8 slot[0x3000];
    struct SaveSummary summary[3];
    struct SaveSummary extra;
    void (*progress)(s32 step);
    u16 sector_count;
    u16 sector_total;
};

struct SaveSlotHeader {
    u8 signature[7];
    u8 record_id;
    u16 checksum;
    u16 sequence;
    u8 reserved[4];
};

LAYOUT_SIZE_GUARD(SaveSummary_Size, struct SaveSummary, 64);
LAYOUT_OFFSET_GUARD(SaveWorkspace_Sequences, struct SaveWorkspace, sequence, 0x1e);
LAYOUT_OFFSET_GUARD(SaveWorkspace_Summary, struct SaveWorkspace, summary, 0x303c);
LAYOUT_OFFSET_GUARD(SaveWorkspace_Extra, struct SaveWorkspace, extra, 0x30fc);
LAYOUT_SIZE_GUARD(SaveWorkspace_Size, struct SaveWorkspace, 0x3144);
LAYOUT_SIZE_GUARD(SaveSlotHeader_Size, struct SaveSlotHeader, 0x10);

u32 SaveState_FindLatestSlot(s32 record_id);

#endif
