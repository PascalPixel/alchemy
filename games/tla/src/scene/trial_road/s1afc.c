#include "trial_road.h"

extern const u8 gData_02007734[];

void Func_02000924(s32 id);
void Func_0200095c(s32 id);
void Func_020042f0(const void *data);
void Func_02000d70(void);

void Func_02001afc(void) {
  OBJECT_BYTE(Actor_GetById(18), 98) = 1;
  OBJECT_BYTE(Actor_GetById(19), 98) = 1;
  OBJECT_BYTE(Actor_GetById(20), 98) = 1;
  OBJECT_BYTE(Actor_GetById(21), 98) = 1;
  if (!GameFlag_Test(0x339))
    OBJECT_BYTE(Actor_GetById(16), 98) = 1;
  else
    Func_020058e0(22, 0, 0);
  Func_020006b4(15);
  Func_020006b4(16);
  Func_020006b4(17);
  Func_020006b4(18);
  Func_020006b4(19);
  Func_020006b4(20);
  Func_020006b4(21);
  Func_0200062c(15);
  Func_0200062c(16);
  Func_0200062c(17);
  if (!GameFlag_Test(0x335)) {
    Func_0200095c(18);
    Func_02000924(19);
  } else {
    Func_0200095c(19);
    Func_02000924(18);
  }
  if (!GameFlag_Test(0x337)) {
    Func_0200095c(20);
    Func_02000924(21);
  } else {
    Func_0200095c(21);
    Func_02000924(20);
  }
}

void Func_02001bdc(void) { Func_020042f0(gData_02007734); }

/// Copies two blocks of map cells, sets byte 98 of objects 16 to 18 and clears
/// their collision flags; then runs Func_02000d70, and Func_020006b4 and
/// Func_0200062c for object 15.
void Func_02001bec(void) {
  Map_CopyCells(11, 22, 11, 8, 11, 64);
  Map_CopyCells(79, 22, 11, 8, 79, 64);
  OBJECT_BYTE(Actor_GetById(16), 98) = 1;
  OBJECT_BYTE(Actor_GetById(17), 98) = 1;
  OBJECT_BYTE(Actor_GetById(18), 98) = 1;
  {
    /* FAKEMATCH: the zero pinned to r6, where ⚓️ builds it; unpinned it reuses
     * r5, which held the 1 (differ 12). */
    register s32 clear asm("r6") = 0;

    Actor_GetById(16)->collisionFlags = clear;
    Actor_GetById(17)->collisionFlags = clear;
    Actor_GetById(18)->collisionFlags = clear;
  }
  Func_02000d70();
  Func_020006b4(15);
  Func_0200062c(15);
}
