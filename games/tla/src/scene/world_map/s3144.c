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
  FieldEvent_ResolvePendingActions(0);
  Event_SetFlag(0xa78);
  Actor_MoveToAndWait(8, 0x1bf6, 0x2154);
  Func_0200050c();
  x = Actor_Get(8)->x / 0x10000;
  z = Actor_Get(8)->z / 0x10000;
  i = 0;
  {
    do {
      if (Actor_Get(i) != NULL) {
        Data_02006be8[i] = i;
        Actor_Get(i)->scaleX = 0;
        Actor_SetSingleChildFlags(Actor_Get(i), 0);
        Actor_SetPosition(i, x << 16, z << 16);
      } else {
        Data_02006be8[i] = -1;
      }
      i++;
    } while (i <= 7);
  }
  Actor_Get(23)->scaleX = 0;
  Actor_SetSingleChildFlags(Actor_Get(23), 0);
  Actor_SetPosition(23, x << 16, z << 16);
  Func_02004da8();
  FieldEvent_SetMessageId((s32)MsgWeDidIt);
  Event_PlaySound(390);
  Event_ShowMessage(2, 0);
  Actor_ShowEmote(8, 0x102, 40);
  Event_ShowMessage(1, 0);
  Event_ShowMessage(3, 0);
  Event_ShowMessage(0, 0);
  FieldEvent_Wait(30);
  Event_ShowMessage(5, 0);
  Event_ShowMessage(6, 0);
  Event_ShowMessage(23, 0);
  Event_PlaySound(78);
  FieldEvent_Wait(50);
  Event_ShowMessage(6, 0);
  Event_ShowMessage(5, 0);
  FieldEvent_Wait(30);
  Event_PlaySound(38);
  Event_ShowMessage(6, 0);
  FieldEvent_Wait(20);
  Event_ShowMessage(0, 0);
  FieldEvent_Wait(20);
  Event_ShowMessage(6, 0);
  Event_ShowMessage(7, 0);
  FieldEvent_Wait(20);
  Event_ShowMessage(3, 0);
  FieldEvent_Wait(30);
  Event_ShowMessage(2, 0);
  FieldEvent_Wait(20);
  Event_ShowMessage(1, 0);
  Event_ShowMessage(6, 0);
  Event_ShowMessage(1, 0);
  Event_ShowMessage(6, 0);
  Event_ShowMessage(1, 0);
  Event_ShowMessage(5, 0);
  Event_ShowMessage(3, 0);
  Event_ShowMessage(1, 0);
  FieldEvent_Wait(40);
  Event_ShowMessage(23, 0);
  Event_ShowMessage(6, 0);
  Event_ShowMessage(23, 0);
  FieldEvent_Wait(20);
  Event_ShowMessage(23, 0);
  FieldEvent_Wait(20);
  Event_ShowMessage(23, 0);
  FieldEvent_Wait(20);
  Event_ShowMessage(23, 0);
  Event_ShowMessage(0, 0);
  FieldEvent_Wait(20);
  Event_ShowMessage(23, 0);
  Event_ShowMessage(6, 0);
  Event_ShowMessage(23, 0);
  FieldEvent_Wait(60);
  Event_ShowMessage(6, 0);
  Event_PlaySound(78);
  Event_ShowMessage(23, 0);
  FieldEvent_Wait(40);
  Event_ShowMessage(23, 0);
  Func_02004dc0();
  Func_02004db0();
  i = 0;
  {
  restore:
    if (Data_02006be8[i] >= 0) {
      Actor_SetSingleChildFlags(Actor_Get(Data_02006be8[i]), 1);
      Actor_Get(Data_02006be8[i])->scaleX = 0x10000;
      Actor_SetPosition(Data_02006be8[i], 0, 0);
    }
    i++;
    if (i <= 7)
      goto restore;
  }
  Actor_SetPosition(23, 0, 0);
  Func_02004c28();
}
