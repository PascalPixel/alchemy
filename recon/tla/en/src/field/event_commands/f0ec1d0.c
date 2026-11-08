#include "types.h"
#include "call.h"
#include "ram_buffer.h"

s32 Party_GetLeaderActor(void);
void GameFlag_ClearBit(s32 flag);
void GameFlag_SetBit(s32 flag);
void *ObjectTable_Get(s32 id);

void Func_080ec1d0(s32 *screenX, s32 *screenY) {
  /* FAKEMATCH(pin): x held in r6; as a plain local, 23 of 92 instructions
   * differ in tla-en. */
  register s32 x asm("r6") = Party_GetLeaderActor();
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
    z = *(s16 *)(object + 18);
    if (z < 1832) {
      if (x < 12000) {
        GameFlag_SetBit(286);
        shift = SPLIT_CONSTANT(1966);
        x += 65;
      } else {
        shift = SPLIT_CONSTANT(6084);
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
