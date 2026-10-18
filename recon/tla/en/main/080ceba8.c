// Draft, differ 32 of 57: finds the current scene's entry in the table at
// Data_080f2204, pairs of a scene key and a flag, each header followed by its
// rows; the first header for the scene whose flag is -1 or clear wins, and the
// rows after it are returned (NULL when none). No ☀️ twin found. Left: register
// choice only: the native carries the next key in r0 (ldrh r0) and tests the
// signed copy in r2, this draft carries it in r2 and tests r3, and the 0x3fff
// and -1 constants trade r1 and r2. A s16 scene, s32 key and flag, or the skip
// written as a do-while behind an if change nothing.
#include "types.h"
#include "flag_call.h"
#include "party_state.h"

extern s16 Data_080f2204[];

s16 *Func_080ceba8(void) {
  s32 scene = gPartyState.scene;
  s16 *entry = Data_080f2204;
  s16 *found = NULL;

  for (;;) {
    s16 key = *entry++;
    s16 flag = *entry++;

    if (key == 0)
      break;
    if ((key & 0x3fff) == scene && (flag == -1 || !GameFlag_Test(flag))) {
      found = entry;
      break;
    }
    while (*entry != 0 && !(*entry & 0x4000))
      entry += 2;
  }
  return found;
}
