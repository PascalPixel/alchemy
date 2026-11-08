// The debug selection row's keys (The Broken Seal's
// DebugSceneSelect_HandleRowInput): A returns -1 and B -2; Up or Down switches
// the field, Left and Right step the value (0 to 324) or the second number
// (0 to 99) by one, R and L by ten, and the row is redrawn after a step.
#include "types.h"
#include "io_reg.h"
#include "input.h"

void Menu_DrawSelectionRow(s32 arg, s32 value, s16 *sub);

s32 Menu_HandleSelectionRowInput(s32 arg, s16 value, s16 *sub, s16 *modeArg) {
  /* FAKEMATCH(pin): the field pointer in r0, where the native keeps it; as
   * the plain parameter, 32 of 165 instructions differ in tla-en (gInput's
   * address takes r0). */
  register s16 *mode asm("r0") = modeArg;

  if (gInput.repeat & KEY_A)
    return -1;
  if (gInput.repeat & KEY_B)
    return -2;
  if ((gInput.repeat & KEY_DOWN) || (gInput.repeat & KEY_UP)) {
    *mode ^= 1;
  } else if (gInput.repeat & KEY_RIGHT) {
    if (*mode == 0) {
      value++;
    } else {
      (*sub)++;
      if (*sub > 99)
        *sub = 0;
    }
    if (value > 324)
      value = 0;
    Menu_DrawSelectionRow(arg, value, sub);
  } else if (gInput.repeat & KEY_LEFT) {
    if (*mode == 0) {
      value--;
    } else {
      (*sub)--;
      if (*sub < 0)
        *sub = 99;
    }
    if (value < 0)
      value = 324;
    Menu_DrawSelectionRow(arg, value, sub);
  } else if (gInput.repeat & KEY_R) {
    if (*mode == 0) {
      *sub = 0;
      value += 10;
    } else {
      *sub += 10;
      if (*sub > 99)
        *sub -= 99;
    }
    if (value > 324)
      value = 0;
    Menu_DrawSelectionRow(arg, value, sub);
  } else if (gInput.repeat & KEY_L) {
    if (*mode == 0) {
      *sub = 0;
      value -= 10;
    } else {
      *sub -= 10;
      if (*sub < 0)
        *sub += 99;
    }
    if (value < 0)
      value = 324;
    Menu_DrawSelectionRow(arg, value, sub);
  }
  return value;
}
