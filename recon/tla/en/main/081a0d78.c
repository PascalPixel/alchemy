/* First draft of Func_081a0d78, differ 47 of 62 (draft 61 instructions), written from the
 * listing; the control flow and every call match, the registers do not. The native keeps the
 * signed position in r2 beside its line in r4 (adds r3, r2, #0; cmp r2, #0), reads the busy
 * word twice before the test (ldrsh r3, then ldrh r1 for the increment) with the zero index
 * in r6, and shares one DrawLine tail between the busy branch and the end-mark branch. Here
 * the line takes r1 and nothing needs r6. Measured: the line read before the unsigned position
 * is differ 51; an early return on the unchanged line is differ 52. */
#include "types.h"

extern u16 DisplayScroll_Position;
extern s16 DisplayScroll_LastPosition;
extern s16 DisplayScroll_Busy;
extern const u8 *DisplayScroll_LineTable[];

s32 DisplayScroll_DrawLine(const u8 *text, s32 slot, s32 align);

// スタッフロール：入ってくる行を描く。表の 255 は終わりのしるしで、あとは空行をくりかえす
void Func_081a0d78(void)
{
	u32		  current;
	s32		  line;
	const u8 *text;

	current = DisplayScroll_Position;
	line = (s16)DisplayScroll_Position / 8;
	if (DisplayScroll_Busy != 0)
	{
		DisplayScroll_Busy++;
		DisplayScroll_DrawLine(DisplayScroll_LineTable[0], ((line + 16) & 31) * 24, 1);
	}
	else if (line != DisplayScroll_LastPosition / 8)
	{
		DisplayScroll_LastPosition = current;
		text = DisplayScroll_LineTable[line];
		if (text == (const u8 *)255)
		{
			DisplayScroll_Busy = 1;
			DisplayScroll_DrawLine(DisplayScroll_LineTable[0], ((line + 16) & 31) * 24, 1);
		}
		else
		{
			DisplayScroll_Busy = DisplayScroll_DrawLine(text, ((line + 16) & 31) * 24, 1);
		}
	}
}
