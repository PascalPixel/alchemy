/* First draft of Func_08041068, differ 27 of 65 (draft 64 instructions), written from the
 * listing; the calls and the loop's shape match, the registers do not. The native keeps the
 * line pointer in r7, the text y in r10, the count in r8 (mov r3, r8; cmp r3, #8) and the
 * divider row in r5; here the count takes r7 and the line pointer r10. Measured: an indexed
 * for loop with the divider skipped on the first pass is differ 59. */
#include "types.h"
#include "ram_buffer.h"

struct RenderInput;

struct ScreenWork
{
	u8	  unknown_000[0x5a4];
	void *cursor;
};

struct ScreenLine
{
	const u8 *text;
	s32		  unknown_4;
};

extern struct ScreenLine Data_080aa128[];

struct RenderInput *UiWindow_Create(s32 x, s32 y, s32 width, s32 height, s32 style);
void				UiWindow_DrawDividerLine(struct RenderInput *window, s32 x, s32 y, s32 right, s32 bottom);
void				UiText_DrawStringInWindow(const u8 *text, struct RenderInput *window, s32 x, s32 y);
s32					Resource_LoadFixedBlockBIntoFreeSlot(void);
void			   *RenderOutput_Create(s32 slot, s32 attributes, struct RenderInput *window, s32 mode);

struct RenderInput *Func_08041068(void)
{
	struct ScreenWork  *work = Ram_HeapSlots->unknown_d0[0];
	struct RenderInput *window;
	struct ScreenLine  *line;
	s32					i;
	s32					row;
	s32					y;

	window = UiWindow_Create(0, 0, 30, 19, 2);
	line = Data_080aa128;
	y = 4;
	i = 0;
	row = 2;
	for (;;)
	{
		UiText_DrawStringInWindow(line->text, window, 8, y);
		y += 16;
		line++;
		if (i == 8)
			break;
		UiWindow_DrawDividerLine(window, 0, row, 29, row);
		row += 2;
		i++;
	}
	work->cursor = RenderOutput_Create(Resource_LoadFixedBlockBIntoFreeSlot(), 0x40000000, window, 0);
	return window;
}
