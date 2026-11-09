#include "types.h"
#include "actor.h"
#include "event_inline.h"

extern char MsgWeDidIt[];
extern s32 Data_02006be8[8];

void Actor_SetSingleChildFlags(struct Actor *object, s32 flags);
void Func_0200050c(void);
void Func_02004c20(void);
void Func_02004c28(void);
void Func_02004da8(void);
void Func_02004db0(void);
void Func_02004dc0(void);

void Func_02003144(void) {
  s32 x, z, i;

  if (GameFlag_Test(0x8df) == 0)
    return;
  Func_02004c20();
  Event_ResolvePendingActions(0);
  GameFlag_SetBit(0xa78);
  Actor_MoveToAndWait(8, 0x1bf6, 0x2154);
  Func_0200050c();
  x = Actor_GetById(8)->x / 0x10000;
  z = Actor_GetById(8)->z / 0x10000;
  i = 0;
  {
    do {
      if (Actor_GetById(i) != NULL) {
        Data_02006be8[i] = i;
        Actor_GetById(i)->scaleX = 0;
        Actor_SetSingleChildFlags(Actor_GetById(i), 0);
        Actor_SetPosition(i, x << 16, z << 16);
      } else {
        Data_02006be8[i] = -1;
      }
      i++;
    } while (i <= 7);
  }
  Actor_GetById(23)->scaleX = 0;
  Actor_SetSingleChildFlags(Actor_GetById(23), 0);
  Actor_SetHorizontalPositionWithTerrain(23, x << 16, z << 16);
  Func_02004da8();
  Event_SetMessageId((s32)MsgWeDidIt);
  Sound_Play(390);
  Event_ShowMessageAndWait(2, 0);
  Func_080d47b4(8, 0x102, 40);
  Event_ShowMessageAndWait(1, 0);
  Event_ShowMessageAndWait(3, 0);
  Event_ShowMessageAndWait(0, 0);
  Event_Wait(30);
  Event_ShowMessageAndWait(5, 0);
  Event_ShowMessageAndWait(6, 0);
  Event_ShowMessageAndWait(23, 0);
  Sound_Play(78);
  Event_Wait(50);
  Event_ShowMessageAndWait(6, 0);
  Event_ShowMessageAndWait(5, 0);
  Event_Wait(30);
  Sound_Play(38);
  Event_ShowMessageAndWait(6, 0);
  Event_Wait(20);
  Event_ShowMessageAndWait(0, 0);
  Event_Wait(20);
  Event_ShowMessageAndWait(6, 0);
  Event_ShowMessageAndWait(7, 0);
  Event_Wait(20);
  Event_ShowMessageAndWait(3, 0);
  Event_Wait(30);
  Event_ShowMessageAndWait(2, 0);
  Event_Wait(20);
  Event_ShowMessageAndWait(1, 0);
  Event_ShowMessageAndWait(6, 0);
  Event_ShowMessageAndWait(1, 0);
  Event_ShowMessageAndWait(6, 0);
  Event_ShowMessageAndWait(1, 0);
  Event_ShowMessageAndWait(5, 0);
  Event_ShowMessageAndWait(3, 0);
  Event_ShowMessageAndWait(1, 0);
  Event_Wait(40);
  Event_ShowMessageAndWait(23, 0);
  Event_ShowMessageAndWait(6, 0);
  Event_ShowMessageAndWait(23, 0);
  Event_Wait(20);
  Event_ShowMessageAndWait(23, 0);
  Event_Wait(20);
  Event_ShowMessageAndWait(23, 0);
  Event_Wait(20);
  Event_ShowMessageAndWait(23, 0);
  Event_ShowMessageAndWait(0, 0);
  Event_Wait(20);
  Event_ShowMessageAndWait(23, 0);
  Event_ShowMessageAndWait(6, 0);
  Event_ShowMessageAndWait(23, 0);
  Event_Wait(60);
  Event_ShowMessageAndWait(6, 0);
  Sound_Play(78);
  Event_ShowMessageAndWait(23, 0);
  Event_Wait(40);
  Event_ShowMessageAndWait(23, 0);
  Func_02004dc0();
  Func_02004db0();
  i = 0;
  {
  restore:
    if (Data_02006be8[i] >= 0) {
      Actor_SetSingleChildFlags(Actor_GetById(Data_02006be8[i]), 1);
      Actor_GetById(Data_02006be8[i])->scaleX = 0x10000;
      Actor_SetHorizontalPositionWithTerrain(Data_02006be8[i], 0, 0);
    }
    i++;
    if (i <= 7)
      goto restore;
  }
  Actor_SetHorizontalPositionWithTerrain(23, 0, 0);
  Func_02004c28();
}
