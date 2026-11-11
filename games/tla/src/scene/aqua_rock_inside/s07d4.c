#include "../../../../../games/tla/src/scene/aqua_rock_inside/local.h"

extern const u8 gData_02003fb8[];

void Sound_WaitForBgmRestore(void);
void Func_020007a8(void);
void Func_02003ebc(void);

static __inline__ void Func_02003d44_(s32 a, s32 b, s32 c) {
  /* FAKEMATCH(wrapper): the call forwarded through an inline, which keeps each
   * call's own constants, as The Broken Seal's scene layer does; as a direct
   * call. */
  Map_SetScreenShake(a, b, c);
}

void Func_020007d4(void) {
  s32 priority;
  s32 i;
  s32 j;

  Func_02003d7c();
  FieldEvent_ResolvePendingActions(0);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, 0xc000, 0);
  Func_020007a8();
  Event_PlaySound(78);
  FieldEvent_Wait(60);
  Event_PlaySound(162);
  Func_02003d44_(0x10000, 0x10000, 0x10000);
  FieldEvent_Wait(20);
  Func_02003d44_(0x10000, 0x20000, 0x10000);
  FieldEvent_Wait(20);
  Func_02003d44_(0x10000, 0x30000, 0x10000);
  FieldEvent_Wait(40);
  priority = 0x480;
  FieldTask_AddFrame(Func_020006d0, priority);
  FieldCamera_SetSpeed(0x28000, 0x4000);
  FieldCamera_MoveTo(0x1380000, -1, 0x1f80000, 1);
  Event_PlaySound(164);
  Func_02003d24_(75, 96, 7, 13, 80, 27);
  MapGrid_CopyCellsLayered(91, 96, 7, 10, 16, 31);
  MapGrid_CopyCellsLayered(99, 96, 7, 14, 16, 91);
  MapGrid_CopyCellAttributes(43, 0, 5, 9, 17, 31);
  // Both loops are goto loops: the ROM builds the two stack arguments and the
  // table address on every pass, which for loops hoist (differ 38).
  i = 0;
row:
  j = 0;
cell:
  {
    s32 index = (i + (1 & ~j)) * 2;

    Func_02003d24_(gData_02003fb8[index], gData_02003fb8[index + 1], 7, 13, 80,
                   27);
    System_Wait(1);
  }
  j++;
  if (j <= 6)
    goto cell;
  i++;
  if (i <= 14)
    goto row;
  Func_02003e54();
  Func_02003d44_(-1, -1, 0xe666);
  Event_PlaySound(390);
  FieldEvent_Wait(30);
  Event_PlaySound(80);
  Sound_WaitForBgmRestore();
  Func_02003d84();
  Func_02003ebc();
  Event_SetFlag(0x95d);
  Event_SetFlag(0x143);
}
