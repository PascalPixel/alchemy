#ifndef TLA_SAVE_STATE_H
#define TLA_SAVE_STATE_H

#include "TYPES.H"
#include "LAYOUT_GUARD.H"

/* A save file's 64-byte summary, as the file select screen shows it; the
   four bytes at 0x38 come from the file's second flash sector. */
struct SaveSummary {
    u8 unknown_00[0x38];
    u8 unknown_38[4];
    u32 checksum;
};

/* The save workspace (heap slot serial_work while saving): the fifteen
   flash sectors' occupancy, record and sequence, one sector's buffer, the
   three files' summaries and the summary of record 3. */
struct SaveWorkspace {
    u8 occupied[15];
    u8 record_id[15];
    u16 sequence[15];
    u8 slot[0x3000];
    struct SaveSummary summary[3];
    struct SaveSummary extra;
};

LAYOUT_SIZE_GUARD(SaveSummary_Size, struct SaveSummary, 64);
LAYOUT_OFFSET_GUARD(SaveWorkspace_Sequences, struct SaveWorkspace, sequence, 0x1e);
LAYOUT_OFFSET_GUARD(SaveWorkspace_Summary, struct SaveWorkspace, summary, 0x303c);
LAYOUT_OFFSET_GUARD(SaveWorkspace_Extra, struct SaveWorkspace, extra, 0x30fc);

u32 SaveState_FindLatestSlot(s32 record_id);

#endif
