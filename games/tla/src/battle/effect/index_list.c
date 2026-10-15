#include "types.h"

typedef struct {
  s32 reserved00[3];
  s32 currentValue;
  s32 indexRadius;
  s32 entryLimit;
  s32 reserved18[3];
  s16 values[1];
} BattleEffectIndexWindow;

s32 BattleFx_BuildCenteredIndexList(BattleEffectIndexWindow *window,
                                    s16 *output) {
  s32 length = window->indexRadius * 2 + 1;
  s32 entryCount = 0;
  s32 center = 0;
  s32 entryIndex;

  for (entryIndex = 0; entryIndex != length; entryIndex++) {
    if (window->currentValue == window->values[entryIndex]) {
      center = entryIndex;
      break;
    }
  }
  for (entryIndex = 0; entryIndex != length; entryIndex++) {
    s32 battleValue = center + entryIndex - window->indexRadius;

    if (battleValue >= 0 && battleValue < window->entryLimit) {
      output[entryCount++] = battleValue;
    }
  }
  return entryCount;
}
