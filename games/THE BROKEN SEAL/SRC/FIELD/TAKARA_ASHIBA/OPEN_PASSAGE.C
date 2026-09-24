#include "TYPES.H"
#include "FIELD_EVENT.H"

void Main_080091b8(s32 src_x, s32 src_y, s32 width, s32 height, s32 dest_x, s32 dest_y);

/* Passage n (low byte of arg, four cells apart): with bit 8 set, play cue 157, set the work values and show the 1x3 tiles from (79, 29) for 40 frames first; then draw the tiles from (80, 29) and copy the row-40 cell attributes down to rows 41 and 42. */
void TakaraAshiba_OpenPassage(s32 arg)
{
    s32 step = (arg & 0xff) * 4;
    s32 dest = step + 77;
    s32 column = step + 13;

    if (arg & 0x100) {
        Engine_AudioPlayCue(157);
        Work_SetValuesIfNonNegative(0x20000, 0x20000, 0x10000);
        Work_SetValuesIfNonNegative(-1, -1, 0xe666);
        Main_080091b8(79, 29, 1, 3, dest, 40);
        Engine_TaskWait(40);
    }
    Main_080091b8(80, 29, 1, 3, dest, 40);
    Engine_MapCopyCellAttributes(column, 40, 1, 1, column, 41);
    Engine_MapCopyCellAttributes(column, 40, 1, 1, column, 42);
}
