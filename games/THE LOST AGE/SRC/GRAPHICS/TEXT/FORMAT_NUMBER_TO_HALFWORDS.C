#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "LIB.H"
#include "MENU.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "TYPES.H"


s32 UiText_FormatNumberToHalfwords(s16 *out, s32 value)
{
    s16 *dst;
    s32 n;
    u8 *src;

    dst = out;
    Text_FormatSignedDecimalToWork(value);
    src = gNumberTextBuffer;
    n = 13;
    do {
        n -= 1;
        *dst = *src;
        src += 1;
        dst += 1;
    } while (n >= 0);
}
