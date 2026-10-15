#include "types.h"
#include "ram_buffer.h"
#include "resource.h"

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

extern char ResourceId_ShopCursorFrames;

void Func_08108030(union OptionCursor *cursor);
void Func_08108038(union OptionCursor *cursor);
void Func_080450fc(void *object);

void GraphicsPalette_LoadSelectionResourcesAndAdvance(void)
{
	s32				   src0;
	s32				   src1;
	s32				   selection;
	struct OptionWork *work;

	/* FAKEMATCH: the one-pass block loads the option work before the page and cursor offsets are started; assigned directly both offsets are started first (differ 2). */
	do
	{
		work = Ram_HeapSlots->unknown_d0[0];
	} while (0);
	selection = work->page;
	Func_08108030(&work->cursor);
	Func_08108038(&work->marker[0]);
	Func_08108038(&work->marker[1]);

	if (selection == 0)
	{
		src0 = (work->cursor_frame & 7) + (s32)&ResourceId_ShopCursorFrames;
	}
	else
	{
		src0 = (s32)&ResourceId_ShopCursorFrames;
	}
	VramBlock_LoadCached((u8)work->marker[0].output->index, 0x100, Resource_GetTableEntry(src0));

	if (selection == 1)
	{
		src1 = (work->cursor_frame & 7) + (s32)&ResourceId_ShopCursorFrames;
	}
	else
	{
		src1 = (s32)&ResourceId_ShopCursorFrames;
	}
	VramBlock_LoadCached((u8)work->marker[1].output->index, 0x100, Resource_GetTableEntry(src1));

	if (selection > 1)
		Func_080450fc(work->frame[selection - 2][work->option[selection]]);
	work->cursor_frame++;
}
