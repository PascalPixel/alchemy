#include "types.h"
#include "scene.h"
#include "abi/ui/text/draw/draw_number_right_aligned_far.h"

s32 FixedPoint_Ratio(s32 value, s32 divisor);

void UiText_DrawNumberRightAlignedFar(s32 number, s32 layer, s32 x, s32 y)
{
    s32 value = number;
    s32 digits = 1;

    while (digits <= 15) {
        value = FixedPoint_Ratio(value, 10);
        if (value <= 9) {
            break;
        }
        digits++;
    }

    digits++;
    x -= digits << 3;
    Ui_SetRange(number, digits, layer, x, y);
}
