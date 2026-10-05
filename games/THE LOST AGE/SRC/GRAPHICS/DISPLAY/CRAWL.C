#include "TYPES.H"
#include "text/MSG_IDS.H"

extern u16 DisplayScroll_Position;
extern s16 DisplayScroll_Busy;
extern s16 DisplayScroll_LastPosition;

s32 DisplayScroll_DrawMessageLine(s32 message, s32 slot, s32 align);

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
