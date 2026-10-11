#ifndef GUARD_SAVE_STATE_H
#define GUARD_SAVE_STATE_H

#include "unit/system/save/savedata.h"
#include "unit/graphics/window/measure.h"
#include "unit/system/scheduler/callback.h"
#include "types.h"
#include "layout_guard.h"
#include "dma.h"
#include "global_cells.h"
#include "fixed_math.h"
#include "flash.h"

struct SaveSlotHeader
{
	u8	signature[7];
	u8	record_id;
	u16 checksum;
	u16 sequence;
	u8	reserved[4];
};

struct SaveSummary
{
	u8	slot_header[16];
	u8	name[12];
	u8	level;
	u8	class_id;
	u16 area;
	u32 play_time;
	u32 coins;
	u8	djinn[4];
	s8	party[5];
	s8	send_flag;
	s8	flag_count;
	u8	has_flag_20;
	u16 palette_glow : 8;
	u16 palette_glow_sub : 8;
	u16 frames;
	u8	unknown_38[4];
	u32 checksum;
};

struct SaveWorkspace
{
	u8	occupied[16];
	u8	record_id[16];
	u16 sequence[16];
	union
	{
		struct
		{
			struct SaveSlotHeader header;
			u8					  payload[0xff0];
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

#define START_DMA(source_, destination_, control_) \
	Dma_Set((const void *)(source_), (void *)(destination_), (control_), (volatile u32 *)0x040000d4)

static __inline__ void WaitDma(struct DmaRegisters volatile *channel)
{
	while ((channel->control & DMA3_BUSY) != 0)
	{
	}
}

#define WAIT_DMA() WaitDma(DMA3)

extern s16 gTitleSendOptionEnabled;

#endif
