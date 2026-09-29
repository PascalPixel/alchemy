#include "TYPES.H"

s32 Func_080f07f0(const void *text, s32 tile, s32 mode);

extern u16 Data_02004c00;
extern s16 Data_02004c04;
extern s16 Data_02004c08;
extern const void *DisplayScroll_LineTable[];

/* When no line is still being drawn and the scroll position has entered a
   new eight-pixel line, remember the position and draw that line's text
   into its slot of the 32-line ring (24 tiles per line, eight ahead). */
void DisplayScroll_RenderEnteringLine(void)
{
    u32 current;
    s32 line;

    if (Data_02004c04 == 0) {
        current = Data_02004c00;
        if ((s16)Data_02004c00 / 8 != Data_02004c08 / 8) {
            line = (s16)Data_02004c00 / 8;
            Data_02004c08 = current;
            Data_02004c04 = Func_080f07f0(DisplayScroll_LineTable[line], ((line + 16) & 31) * 24, 1);
        }
    }
}
