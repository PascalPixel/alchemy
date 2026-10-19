#include "types.h"
#include "edition.h"

#if (LANGUAGE == LANGUAGE_JAPANESE)
/// The character a cell of the password grid stands for, in the Japanese
/// font's codes: kana, the voiced ones with the dakuten mark in the high byte.
void Func_02000a70(s32 cell, u16 *text) {
  /* FAKEMATCH: the dakuten mark is pinned to r2 as a halfword: the native loads
   * it before it copies the cell for the add, where plainly the copy comes
   * first (differ 4). */
  register u16 mark asm("r2");

  text[1] = 0;
  text[2] = 0;
  if (cell <= 14)
    text[0] = cell + 145;
  else if (cell <= 43)
    text[0] = cell + 209;
  else if (cell == 45)
    text[0] = 0x86;
  else if (cell == 44)
    text[0] = 0xfd;
  else if (cell <= 55) {
    mark = 0x4000;
    text[0] = (cell + 104) | mark;
  } else if (cell == 56)
    text[0] = 0xe0 | 0x4000;
  else if (cell == 57)
    text[0] = 0xe3 | 0x4000;
  else if (cell == 58)
    text[0] = 0xe4 | 0x4000;
  else {
    mark = 0x4000;
    text[0] = (cell + 175) | mark;
  }
}
#else
/// The character a cell of the password grid stands for, in the font's
/// codes: capitals without I and O, the digits 2 to 9, small letters without
/// l and o, then symbols.
void Func_02000a70(s32 cell, u16 *text) {
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
