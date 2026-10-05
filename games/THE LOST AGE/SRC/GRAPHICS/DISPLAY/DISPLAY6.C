#include "TYPES.H"

/* The intro crawl's scroll position, the line still being drawn and the
   position its last line was drawn at. */
extern u16 DisplayScroll_Position;
extern s16 DisplayScroll_Busy;
extern s16 DisplayScroll_LastPosition;
/* The first line of the intro crawl's text, two lines to a message. */
extern u8 MsgIntroCrawl[];

s32 DisplayScroll_DrawMessageLine(s32 message, s32 slot, s32 align);

/* When no line is still being drawn and the scroll position has entered a
   new sixteen-pixel line, remember the position and draw that line's text
   into its slot of the 32-row ring (24 tiles per row, sixteen ahead). */
void DisplayScroll_RenderEnteringLine(void)
{
    s32 first = (s32)MsgIntroCrawl;
    u32 current;
    s32 line;

    if (DisplayScroll_Busy == 0) {
        current = DisplayScroll_Position;
        if ((s16)DisplayScroll_Position / 16 != DisplayScroll_LastPosition / 16) {
            DisplayScroll_LastPosition = current;
            line = (s16)((s16)DisplayScroll_Position / 8);
            DisplayScroll_Busy = DisplayScroll_DrawMessageLine(line / 2 + first - 1, ((line + 16) & 31) * 24, 1);
        }
    }
}
