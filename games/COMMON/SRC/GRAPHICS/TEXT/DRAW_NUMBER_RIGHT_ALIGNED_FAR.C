#include "EDITION.H"
#if EDITION_INTERNATIONAL
/* Japanese menus use fixed digit counts; this localization helper is absent
 * in both Japanese editions. */
#include "TYPES.H"
#include "FIXED_MATH.H"

void UiText_DrawNumberAtOffsetFar(s32 value, s32 digits, s32 layer, s32 x, s32 y);

void UiText_DrawNumberRightAlignedFar(s32 number, s32 layer, s32 x, s32 y)
{
    s32 value = number;
    s32 digits = 1;

    while (digits <= 15) {
        value = value / 10;
        if (value <= 9) {
            break;
        }
        digits++;
    }

    digits++;
    x -= digits << 3;
    UiText_DrawNumberAtOffsetFar(number, digits, layer, x, y);
}


#endif
