#include "types.h"

// The Japanese edition's form of this function (kana, with the dakuten mark
// in the high byte) stays on its listing in recon/tla/raw/overlays (S4).
#if !defined(TLA_EDITION_JA)
// The character a cell of the password grid stands for, in the font's
// codes: capitals without I and O, the digits 2 to 9, small letters without
// l and o, then symbols.
void Func_02000a70(s32 cell, u16 *text)
{
    text[1] = 0;
    text[2] = 0;
    if (cell <= 7)
        text[0] = cell + 'A';
    else if (cell <= 12)
        text[0] = cell + 'A' + 1;
    else if (cell <= 23)
        text[0] = cell + 'A' + 2;
    else if (cell <= 31)
        text[0] = cell + 26;
    else if (cell <= 42)
        text[0] = cell + 'A';
    else if (cell <= 44)
        text[0] = cell + 'A' + 1;
    else if (cell <= 55)
        text[0] = cell + 'A' + 2;
    else if (cell == 56)
        text[0] = '!';
    else if (cell == 57)
        text[0] = '?';
    else if (cell == 58)
        text[0] = '#';
    else if (cell == 59)
        text[0] = '&';
    else if (cell == 60)
        text[0] = '$';
    else if (cell == 61)
        text[0] = '%';
    else if (cell == 62)
        text[0] = '+';
    else
        text[0] = '=';
}
#endif
