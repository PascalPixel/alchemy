#include "shrine_of_the_sea_god.h"

void Func_02000280(void) {
  struct ShrineOfTheSeaGodObject *leader =
      Object_GetById(gPartyState.currentOwner);
  u8 *state = &Object_GetById(14)->state;
  s32 y;

  if ((u8)(*state - 4) > 1)
    return;
  y = leader->y;
  /* FAKEMATCH: loads the leader height before it builds the threshold. */
  __asm__("");
  if (y > 0xc0000) {
    GameFlag_SetBit(0x340);
    if (*state == 4)
      GameFlag_SetBit(0x341);
    else if (*state == 5)
      GameFlag_SetBit(0x342);
    Func_02003ae0(14, 67);
  }
}
