#ifndef GUARD_SAVE_STATE_H
#define GUARD_SAVE_STATE_H

#include "TYPES.H"
#include "LAYOUT_GUARD.H"
#include "DMA.H"
#include "GLOBAL_CELLS.H"
#include "FIXED_MATH.H"
#include "FLASH.H"
#include "GLOBAL_CELLS.H"


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

void *Func_080048f4(s32 kind, s32 size);
void WaitFrames(s32 frames);
s32 _call_via_r3(s32, s32, s32, s32);
s32 Flash_VerifySector(u16 sector, s32 source);
s32 Func_080058ac(s32 index);
u32 Func_08005810(s32 record_id);
s32 Func_08005920(s32 record_id, void *source);
u32 Func_08005a78(s32 record_id, void *destination);
u32 Func_08005ac0(s32 record_id);
s32 Func_08005ae0(void);
u32 Func_08005b24(s32 record_id);
s32 Func_08005b64(s32 index);
s32 Func_08005c08(u8 *left, u8 *right, s32 count);
u32 Func_08005c2c(s32 record_id);
s32 Func_08005c68(void);

#endif
