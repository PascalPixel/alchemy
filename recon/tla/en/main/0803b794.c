/* Draft, differ 2 of 104: The Broken Seal's UiWindow_FitOnScreen as its
 * international editions build it (no work mode check), calling
 * Func_0803bb58 for flag 1. The native loads the flags argument from the
 * stack (ldr r2, [sp, #40]) one slot before it copies py for the y load
 * (mov r3, r9); sched2 takes the copy first, the same one-slot load order
 * as this lane's other near misses. Statement orders of x, y and limit
 * measured differ 2 to 42. ⚓️'s Japanese edition keeps the work mode
 * check (0xf8 bytes). */
#include "types.h"

void Func_0803bb58(s32 no, u32 *pw, u32 *ph, u16 *spacing);
void UiText_MeasureEntryDimensions(s32 no, u32 *pw, u32 *ph, u16 *spacing);

void UiWindow_FitOnScreen(s32 no, s32 *px, s32 *py, u32 *pw, u32 *ph, u16 *spacing, u32 flags)
{
	s32 x;
	s32 y;
	s32 limit;
	s32 right;
	s32 bottom;
	s32 over;
	s32 pos;

	x = *px;
	y = *py;
	limit = 30;

	if (!(flags & 2))
	{
		if (flags & 1)
			Func_0803bb58(no, pw, ph, spacing);
		else
			UiText_MeasureEntryDimensions(no, pw, ph, spacing);
	}

	if (*pw == 0 && *ph == 0)
		return;

	if (!(flags & 2))
	{
		*pw = (*pw + 19) >> 3;
		*ph = (*ph + 15) >> 3;
	}

	right = x + *pw;
	if (right > limit)
	{
		over = right - limit;
		pos = x - over;
		if (pos >= 0)
			x = pos;
		else
			x = 0;
	}
	bottom = y + *ph;
	if (bottom > 20)
	{
		over = bottom - 20;
		pos = y - over;
		if (pos >= 0)
			y = pos;
		else
			y = 0;
	}
	if (x < 0)
		x = 0;
	if (y < 0)
		y = 0;
	if (x > limit - *pw)
		x = limit - *pw;
	if (y > 20 - *ph)
		y = 20 - *ph;
	*px = x;
	*py = y;
}
