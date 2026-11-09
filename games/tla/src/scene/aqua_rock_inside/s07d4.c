#include "../../../../../games/tla/src/scene/aqua_rock_inside/local.h"

extern const u8 gData_02003fb8[];

void Sound_WaitForBgmRestore(void);
void Func_020007a8(void);
void Map_SetScreenShake(s32 a, s32 b, s32 c);
void Func_02003ebc(void);

static __inline__ void Func_02003d44_(s32 a, s32 b, s32 c) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does. */
  Map_SetScreenShake(a, b, c);
}

void Func_020007d4(void) {
  s32 priority;
  s32 i;
  s32 j;

  Func_02003d7c();
  Event_ResolvePendingActions(0);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, 0xc000, 0);
  Func_020007a8();
  Sound_Play(78);
  Event_Wait(60);
  Sound_Play(162);
  Func_02003d44_(0x10000, 0x10000, 0x10000);
  Event_Wait(20);
  Func_02003d44_(0x10000, 0x20000, 0x10000);
  Event_Wait(20);
  Func_02003d44_(0x10000, 0x30000, 0x10000);
  Event_Wait(40);
  priority = 0x480;
  Task_Add(Func_020006d0, priority);
  Camera_SetSpeedParameters(0x28000, 0x4000);
  Camera_MoveWithinMapBounds(0x1380000, -1, 0x1f80000, 1);
  Sound_Play(164);
  Func_02003d24_(75, 96, 7, 13, 80, 27);
  Func_0802b450(91, 96, 7, 10, 16, 31);
  Func_0802b450(99, 96, 7, 14, 16, 91);
  Map_CopyCellAttributeRect(43, 0, 5, 9, 17, 31);
  /* FAKEMATCH: both loops are written with goto so that they carry no loop
   * notes: the bytes build the two stack arguments and the table address on
   * every pass, and for loops hoist them (differ 38). */
  i = 0;
row:
  j = 0;
cell:
  {
    s32 index = (i + (1 & ~j)) * 2;

    Func_02003d24_(gData_02003fb8[index], gData_02003fb8[index + 1], 7, 13, 80,
                   27);
    wait(1);
  }
  j++;
  if (j <= 6)
    goto cell;
  i++;
  if (i <= 14)
    goto row;
  Func_02003e54();
  Func_02003d44_(-1, -1, 0xe666);
  Sound_Play(390);
  Event_Wait(30);
  Sound_Play(80);
  Sound_WaitForBgmRestore();
  Func_02003d84();
  Func_02003ebc();
  GameFlag_SetBit(0x95d);
  GameFlag_SetBit(0x143);
}
