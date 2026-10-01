/* Draft: retaining the English name-entry grid produces 142 bytes;
 * the Japanese 90-byte function instead selects its own glyph ranges
 * and appends dakuten to the final grid entries. */
#include "TYPES.H"

void Clear_NameEntryCharacter(s32 cell, u8 *text)
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
