// First draft of Func_080ec1d0, differ 27 of 92 (draft 93 instructions),
// written from the listing: the controlled owner's cell is turned into two
// screen values, with flag 286 set in one quadrant. The registers match once x
// reuses the owner's variable (differ 28 with its own). Left, all instruction
// order: the native loads the event work before it copies the owner (ldr r5,
// [r3, #108] before adds r6, r0, #0), loads the else branch's literal before
// its shift is started, and the product of x and 853 stays in the constant's
// register (muls r3, r6), where this draft copies x to r0 first (853 * x and x
// * 853 alike). The one-pass block at the z read is measured (differ 4 more
// without it).
#include "types.h"
#include "ram_buffer.h"

s32 EventRuntime_GetControlledOwner(void);
void GameFlag_ClearBit(s32 flag);
void GameFlag_SetBit(s32 flag);
void *ObjectTable_Get(s32 id);

void Func_080ec1d0(s32 *screenX, s32 *screenY) {
  s32 x = EventRuntime_GetControlledOwner();
  u8 *state = Ram_HeapSlots->eventWork;
  u8 *object;
  s32 z;
  s32 shift;

  GameFlag_ClearBit(286);
  if (*(s8 *)(state + 394) == 3) {
    object = ObjectTable_Get(x);
    if (object == 0)
      return;
    x = *(s16 *)(object + 10);
    /* FAKEMATCH: the one-pass block reads z before the bound is started (differ
     * 4 without it). */
    do {
      z = *(s16 *)(object + 18);
    } while (0);
    if (z < 1832) {
      if (x < 12000) {
        GameFlag_SetBit(286);
        shift = 1966;
        x += 65;
      } else {
        shift = 6084;
        x -= 627;
      }
      z += shift;
    }
    *screenX = 853 * x / 16384 - 269;
    *screenY = z * 640 / 16384 - 112;
  } else {
    *screenX = 264;
    *screenY = 16;
  }
}
