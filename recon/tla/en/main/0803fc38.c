/* Near miss, differ 10 of 101 (five swapped pairs, all else identical): The Broken Seal's
 * OptionMenu_InitializeWork on this game's party state. For each option the native reads the
 * saved byte one step earlier than here: movs r0, #160; ldrb r2, [r3]; lsls r0, r0, #3, where
 * this draft finishes the shift before the load (and ldr r1, =gPartyState before movs r0, #147
 * at the top). A one-pass block cannot give that order: it would pull the load above the
 * movs that starts the next offset. The party state fields are used by their header names. */
#include "types.h"
#include "mem.h"
#include "dma.h"
#include "io_reg.h"
#include "callback_scheduler.h"
#include "party_state.h"

struct OptionMarkerOutput
{
	u8 unknown_00[14];
	u8 index;
};

union OptionCursor
{
	struct OptionMarkerOutput *output;
	u8						   data[0x10];
};

struct OptionWork
{
	u8				   unknown_000[0x574];
	u16				   page;
	u16				   tint[3];
	u16				   cursor_frame;
	u16				   preset;
	u8				   unknown_580[0x14];
	s8				   option[5];
	s8				   option_count[5];
	u8				   unknown_59e[6];
	union OptionCursor cursor;
	union OptionCursor marker[2];
	u8				   unknown_5d4[0x18];
	void			  *frame[3][3];
};

void GraphicsPalette_LoadSelectionResourcesAndAdvance(void);

void OptionMenu_InitializeWork(void)
{
	struct OptionWork *work;
	volatile u32	   zero;

	work = (struct OptionWork *)Runtime_AllocateBlock(208, 0x628);
	zero = 0;
	Dma_Set((const void *)&zero, work, 0x8500018a, REG_DMA3);
	work->option[0] = gPartyState.unknown_220[5];
	work->option_count[0] = 24;
	work->option[1] = gPartyState.unknown_220[6];
	work->option_count[1] = 15;
	work->option[2] = gPartyState.event_mode;
	work->option_count[2] = 3;
	work->option[3] = gPartyState.battle_fx_enabled;
	work->option_count[3] = 2;
	work->option[4] = gPartyState.unknown_244[6];
	work->option_count[4] = 2;
	Scheduler_AddOrUpdateCallback(GraphicsPalette_LoadSelectionResourcesAndAdvance, 0x480);
}
