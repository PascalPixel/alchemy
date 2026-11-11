#include "types.h"
#include "owner_state.h"

#define ACTION_MASK 0x3fff
#define PREVIEW_GAINED 0x8000
#define PREVIEW_LOST 0x4000

/// Lists the actions after a preview, marking gains, then appends the losses.
/// The two output counts distinguish gained and lost actions.
///
/// Writes every action of after (up to 32), with PREVIEW_GAINED on those
/// before lacks, then each action of before that after lacks, with
/// PREVIEW_LOST. Returns the number written, up to 64.
s32 PsynergyList_Diff(struct OwnerActionSlot *before,
                      struct OwnerActionSlot *after, u16 *actionsOut,
                      s32 *gainedCountOut, s32 *lostCountOut) {
  s32 actionIndex;
  s32 otherIndex;
  s32 actionCount;
  s32 gainedCount;
  s32 lostCount;

  actionCount = 0;
  gainedCount = 0;
  lostCount = 0;
  for (actionIndex = 0;
       actionIndex < 32 && after[actionIndex].encodedAction != 0;
       actionIndex++) {
    actionsOut[actionCount] = after[actionIndex].encodedAction & ACTION_MASK;
    actionCount++;
    for (otherIndex = 0; otherIndex < 32; otherIndex++) {
      if (((after[actionIndex].encodedAction ^
            before[otherIndex].encodedAction) &
           ACTION_MASK) == 0)
        break;
    }
    if (otherIndex == 32) {
      gainedCount++;
      actionsOut[actionCount - 1] |= PREVIEW_GAINED;
    }
  }
  for (actionIndex = 0;
       actionIndex < 32 && before[actionIndex].encodedAction != 0;
       actionIndex++) {
    for (otherIndex = 0; otherIndex < 32; otherIndex++) {
      if (((before[actionIndex].encodedAction ^
            after[otherIndex].encodedAction) &
           ACTION_MASK) == 0)
        break;
    }
    if (otherIndex == 32) {
      lostCount++;
      actionsOut[actionCount] =
          (before[actionIndex].encodedAction & ACTION_MASK) | PREVIEW_LOST;
      actionCount++;
    }
  }
  *gainedCountOut = gainedCount;
  *lostCountOut = lostCount;
  return actionCount;
}
