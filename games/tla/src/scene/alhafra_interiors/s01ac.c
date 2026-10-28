#include "alhafra_interiors.h"

void Func_020001ac(s32 actor) {
  u16 facing = Actor_GetById(gSequenceParentWork.currentOwner)->facing;

  /* FAKEMATCH: the loop note ends the scheduling block: ⚓️ loads the facing
   * before it builds the quadrant offset (2 differ without it). */
  do {
  } while (0);
  if ((u16)((facing + 0x2000) & ~0x3fff) == 0xc000) {
    Func_02000bc4(9, actor);
  } else {
    Func_02000b24((s32)MsgAlhafraInteriors14);
    Func_02000b3c(actor, 0);
  }
}

void Func_020001fc(s32 actor) {
  u16 facing = Actor_GetById(gSequenceParentWork.currentOwner)->facing;

  /* FAKEMATCH: the loop note ends the scheduling block: ⚓️ loads the facing
   * before it builds the quadrant offset (2 differ without it). */
  do {
  } while (0);
  if ((u16)((facing + 0x2000) & ~0x3fff) == 0xc000) {
    Func_02000bd4(2, actor);
  } else {
    Func_02000b24((s32)MsgAlhafraInteriors16);
    Func_02000b3c(actor, 0);
  }
}

void Func_0200024c(s32 actor) {
  u16 facing = Actor_GetById(gSequenceParentWork.currentOwner)->facing;

  /* FAKEMATCH: the loop note ends the scheduling block: ⚓️ loads the facing
   * before it builds the quadrant offset (2 differ without it). */
  do {
  } while (0);
  if ((u16)((facing + 0x2000) & ~0x3fff) == 0xc000) {
    Func_02000bcc(actor);
  } else {
    Func_02000b24((s32)MsgAlhafraInteriors49);
    Func_02000b3c(actor, 0);
  }
}
