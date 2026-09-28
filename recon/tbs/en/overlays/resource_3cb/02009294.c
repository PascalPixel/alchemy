/* Draft of Scene_DrawThreeDigitValue, resource_3cb at 0x02009294, built with
 * games/THE BROKEN SEAL/SRC/MENU/LINK_LOBBY/LOBBY.H.
 * Remaining difference: four bytes; the ROM sets r0 before r3 for the cell
 * copy, GCC the other way round. The listing keeps these rows. */
#include "LOBBY.H"

s32 Scene_DrawThreeDigitValue(s32 value)
{
    s32 col;

    if (value > 999) {
        value = 999;
    }

    for (col = 0; col <= 2; col++) {
        s32 digit = Engine_MathRemainder(value, 10);

        Engine_MapCopyCellsTo(27, digit, 16 - col, 8, 1, 1);
        value = Engine_MathDivide(value, 10);
    }

    return Engine_MapRedraw();
}
