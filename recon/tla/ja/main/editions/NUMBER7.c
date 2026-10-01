/* Draft: Japanese number spacing trial 7. The complete instruction extent
 * finds no Japanese ROM counterpart under the approved TLA flags; keep the
 * maintained eight-pixel source until the Japanese structure is understood. */
#if !defined(TBS_EDITION_JA)
/* These localization routines have no counterpart in Japanese TBS. */
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
    x -= digits * 7;
    UiText_DrawNumberAtOffsetFar(number, digits, layer, x, y);
}


#endif
