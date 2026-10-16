/* First draft of Func_081a0d78, differ 47 of 62 (draft 61 instructions), written from the
 * listing; the control flow and every call match, the registers do not. The native keeps the
 * signed position in r2 beside its line in r4 (adds r3, r2, #0; cmp r2, #0), reads the busy
 * word twice before the test (ldrsh r3, then ldrh r1 for the increment) with the zero index
 * in r6, and shares one DrawLine tail between the busy branch and the end-mark branch. Here
 * the line takes r1 and nothing needs r6. Measured: the line read before the unsigned position
 * is differ 51; an early return on the unchanged line is differ 52. */
#include "types.h"

extern u16 gDisplayScroll_Position;
extern s16 gDisplayScroll_LastPosition;
extern s16 gDisplayScroll_Busy;
extern const u8 *gDisplayScroll_LineTable[];

s32 DisplayScroll_DrawLine(const u8 *text, s32 slot, s32 align);

// Credits: draw incoming rows. Table value 255 ends the text; blank rows
// follow.
void Func_081a0d78(void)
{
	u32		  current;
	s32		  line;
	const u8 *text;

	current = gDisplayScroll_Position;
	line = (s16)gDisplayScroll_Position / 8;
	if (gDisplayScroll_Busy != 0)
	{
		gDisplayScroll_Busy++;
		DisplayScroll_DrawLine(gDisplayScroll_LineTable[0], ((line + 16) & 31) * 24, 1);
	}
	else if (line != gDisplayScroll_LastPosition / 8)
	{
		gDisplayScroll_LastPosition = current;
		text = gDisplayScroll_LineTable[line];
		if (text == (const u8 *)255)
		{
			gDisplayScroll_Busy = 1;
			DisplayScroll_DrawLine(gDisplayScroll_LineTable[0], ((line + 16) & 31) * 24, 1);
		}
		else
		{
			gDisplayScroll_Busy = DisplayScroll_DrawLine(text, ((line + 16) & 31) * 24, 1);
		}
	}
}
