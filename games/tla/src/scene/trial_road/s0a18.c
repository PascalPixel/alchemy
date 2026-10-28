#include "trial_road.h"

extern const s8 gData_02005a88[4];

void Func_02000924(s32 id);
void Func_0200095c(s32 id);

void Func_02000a18(s32 id, s32 mode, s32 frame) {
  s32 other = gData_02005a88[(id - 18) & 3];
  struct Actor *object = Actor_GetById(id);
  struct Actor *partner = Actor_GetById(other);
  struct Actor *leader = Actor_GetById(gAdventure.currentOwner);

  if (frame <= 10)
    object->scaleY = 0x10000 - (frame << 12);
  else
    object->scaleY = 0x10000;
  if (mode == 1) {
    Func_02000924(id);
    Func_0200095c(other);
    switch (id) {
    case 18:
      GameFlag_SetBit(821);
      break;
    case 19:
      GameFlag_ClearBit(821);
      break;
    case 20:
      GameFlag_SetBit(823);
      break;
    case 21:
      GameFlag_ClearBit(823);
      break;
    }
    if (partner->x >> 20 == leader->x >> 20 &&
        partner->z >> 20 == leader->z >> 20) {
      leader->velocityY = 0x80000;
      leader->x = (leader->x & 0xfff00000) + 0x80000;
      leader->z = (leader->z & 0xfff00000) + 0x80000;
    }
  }
  if (frame == 9)
    partner->scaleY = 0x13333;
  else if (frame == 11)
    partner->scaleY = 0x11999;
  else if (frame == 13)
    partner->scaleY = 0x10ccc;
  else
    partner->scaleY = 0x10000;
}
