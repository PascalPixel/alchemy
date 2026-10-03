#ifndef GUARD_SAVE_STATE_H
#define GUARD_SAVE_STATE_H

#include "TYPES.H"
#include "LAYOUT_GUARD.H"
#include "DMA.H"
#include "GLOBAL_CELLS.H"
#include "FIXED_MATH.H"
#include "FLASH.H"

struct DmaRegisters {
    u32 source;
    u32 destination;
    u32 control;
};

struct SaveSlotHeader {
    u8 signature[7];
    u8 record_id;
    u16 checksum;
    u16 sequence;
    u8 reserved[4];
};

struct SaveSummary {
    u8 slot_header[16];
    u8 name[12];
    u8 level;
    u8 class_id;
    u16 area;
    u32 play_time;
    u32 coins;
    u8 djinn[4];
    s8 party[5];
    u8 send_flag;
    u8 flag_count;
    u8 has_flag_20;
    u8 palette_glow[2];
    u16 frames;
    u8 unknown_38[4];
    u32 checksum;
};

struct SaveWorkspace {
    u8 occupied[16];
    u8 record_id[16];
    u16 sequence[16];
    union {
        struct {
            struct SaveSlotHeader header;
            u8 payload[0xff0];
        } record;
        u8 bytes[0x1000];
    } slot;
    struct SaveSummary summary[3];
};

LAYOUT_SIZE_GUARD(SaveSummary_Size, struct SaveSummary, 64);
LAYOUT_OFFSET_GUARD(SaveSummary_Level, struct SaveSummary, level, 0x1c);
LAYOUT_OFFSET_GUARD(SaveSummary_SendFlag, struct SaveSummary, send_flag, 0x31);
LAYOUT_OFFSET_GUARD(SaveSummary_Checksum, struct SaveSummary, checksum, 0x3c);
LAYOUT_OFFSET_GUARD(SaveWorkspace_Slot, struct SaveWorkspace, slot, 0x40);
LAYOUT_OFFSET_GUARD(SaveWorkspace_Summary, struct SaveWorkspace, summary, 0x1040);

LAYOUT_SIZE_GUARD(SaveSlotHeader_Size, struct SaveSlotHeader, 0x10);
LAYOUT_SIZE_GUARD(SaveWorkspace_Size, struct SaveWorkspace, 0x1100);
LAYOUT_OFFSET_GUARD(SaveWorkspace_Records, struct SaveWorkspace, record_id, 0x10);
LAYOUT_OFFSET_GUARD(SaveWorkspace_Sequences, struct SaveWorkspace, sequence, 0x20);
LAYOUT_OFFSET_GUARD(SaveSummary_FlagCount, struct SaveSummary, flag_count, 0x32);
LAYOUT_OFFSET_GUARD(SaveSummary_Unknown38, struct SaveSummary, unknown_38, 0x38);

extern struct SaveWorkspace *gSaveWorkspace;

#define DMA3 ((volatile struct DmaRegisters *)0x040000d4)
#define DMA3_BUSY 0x80000000

/* The signature a save slot's header starts with, and the eight bytes of
   signature and record byte that each written slot's header starts from. */
extern const u8 Save_Signature[];
extern const u8 Save_HeaderTemplate[];

#define START_DMA(source_, destination_, control_) \
    Dma_Set((const void *)(source_), (void *)(destination_), (control_), (volatile u32 *)0x040000d4)

static __inline__ void WaitDma(struct DmaRegisters volatile *channel)
{
    while ((channel->control & DMA3_BUSY) != 0) {}
}

#define WAIT_DMA() WaitDma(DMA3)

/* Set by the save scan when a slot's summary marks it for sending and R
   and Left are held; the title menu then offers Send. */
extern s16 gTitleSendOptionEnabled;

/* Save records share sixteen flash sectors and three visible summaries. */
s32 SaveState_InitializeWorkspace(void);
void SaveState_ReleaseWorkspace(void);
u32 SaveState_SelectWriteSlot(s32 record_id);
u32 SaveState_WriteWorkspaceSlot(u16 sector);
s32 SaveState_ReadSlotAndCheckChecksum(s32 sector);
u16 SaveState_EraseSlotSector(u16 sector);
s32 SaveState_WriteRecord(s32 record_id, void *source);
u32 SaveState_ReadRecordPayload(s32 record_id, void *destination);
u32 SaveState_DeleteRecord(s32 record_id);
s32 SaveState_ChecksumWorkspace(void);
u32 SaveState_FindLatestSlot(s32 record_id);
s32 SaveState_InvalidateSlot(s32 sector);
s32 SaveState_CompareBytes(u8 *left, u8 *right, s32 count);
u32 SaveState_GetLatestSequence(s32 record_id);
s32 SaveState_LoadSummaryRecords(void);
u32 SaveState_FindFreeSummarySlot(void);
s32 SaveState_CountRecordsExcludingFlagged(s32 exclude_flagged);
s32 SaveState_ScanRecordFlags(void);
u32 SaveState_BuildSummaryHeader(void);
s16 SaveState_WriteCurrentSlotPair(void);
s32 SaveState_WriteSlotPair(s32 slot);
s32 SaveState_ProcessSelectedSlot(void);
s32 SaveState_ConfirmAndProcessSelectedSlot(void);
s32 SaveState_CopySlotPair(void);
s32 SaveState_DeleteSelectedSlot(void);
s32 SaveState_LoadRecordIntoWork(s32 mode);
s32 Save_WriteSelectedSlot(void);

#endif
