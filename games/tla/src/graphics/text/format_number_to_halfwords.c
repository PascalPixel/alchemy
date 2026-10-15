#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"

s32 UiText_FormatNumberToHalfwords(s16 *out, s32 value)
{
    s16 *dst;
    s32  n;
    u8  *src;

    dst = out;
    Text_FormatSignedDecimalToWork(value);
    src = gNumberTextBuffer;
    n = 0xD;
    do
    {
        n -= 1;
        *dst = *src;
        src += 1;
        dst += 1;
    } while (n >= 0);
}
