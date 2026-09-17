#include "TEXT_RENDER_RUNTIME.H"
#include "TYPES.H"

struct NumberRow {
    u8 unknown_00[0x28];
    s8 values[4];
};

void UiText_DrawFourNumbersInRow(struct TextRenderWork *work, struct NumberRow *row)
{
    s32 i;

    if (work != 0) {
        for (i = 0; i < 4; i++)
            UiText_DrawNumberAtOffset(row->values[i], 2, work, i * 0x18, 0x10);
    }
}
