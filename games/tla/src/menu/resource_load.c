#include "types.h"
#include "resource.h"

void Ui_BuildPairedPatternsToSlot(s32 arg0, s32 arg1, s32 *arg2, s32 *arg3,
                                  s32 arg4);
void Ability_LoadGlyph(s32 action, s32 with_base, s32 *slot, s32 *tile,
                       s32 reuse);
s32 UiIcon_CopyResourceToSlot(s32 arg0, s32 arg1, s32 arg2);
s32 Resource_FindFreeEntry(void);

s32 Resource_LoadByMode(u32 mode, s32 value) {
  s32 output;
  s32 result = -1;

  switch (mode) {
  case 1:
  case 6:
    Ui_BuildPairedPatternsToSlot(value, 0, &result, &output, 0);
    break;
  case 2:
    result = Resource_FindFreeEntry();
    if (result == 96)
      return -1;
    UiIcon_CopyResourceToSlot(value, 0x1a, result);
    break;
  case 4:
    Ability_LoadGlyph(value, 1, &result, &output, 0);
    break;
  }
  return result;
}

s32 Resource_LoadByModeIntoSlot(s32 mode, s32 value, s32 result, s32 option) {
  s32 output;
  s32 original = result;

  if (result == -1) {
    result = Resource_FindFreeEntry();
    if (result == 0x60)
      return original;
  }

  switch (mode) {
  case 1:
  case 6:
    Ui_BuildPairedPatternsToSlot(value, option, &result, &output, 1);
    break;
  case 2:
    UiIcon_CopyResourceToSlot(value, 58, result);
    break;
  case 7:
    UiIcon_CopyResourceToSlot(value, 42, result);
    break;
  case 4:
    Ability_LoadGlyph(value, option, &result, &output, 1);
    break;
  case 8:
    Ui_BuildPatternToSlot(value, 0, result);
    break;
  }

  return result;
}
