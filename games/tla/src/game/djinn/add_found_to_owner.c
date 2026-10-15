#include "owner_state.h"

void GameFlag_SetBit(s32 flag);

void Djinn_AddFoundToOwner(s32 owner, s32 element, s32 djinn) {
  s32 flag = element * 20 + djinn;

  Djinn_AddToOwner(owner, element, djinn);
  flag += 0x30;
  DjinnRecovery_AddStandby(owner, element, djinn);
  GameFlag_SetBit(flag);
}
