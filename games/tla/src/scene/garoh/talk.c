#include "garoh.h"

extern u8 MsgGaroh02[];
extern u8 MsgGaroh04[];
extern u8 MsgGaroh06[];
extern u8 MsgGaroh17[];
extern u8 MsgGaroh18[];

void Func_02000580(s32 actor) {
  s32 msg = (s32)MsgGaroh02;

  Func_0200524c(msg);
  UiText_OpenMessageAtObject(actor, 0);
  if (Event_AskYesNoWithActorAnimation(Func_02005384(), 0) == 0) {
    Battle_WaitMode0(10);
    Func_0200524c(msg + 1);
  } else {
    Battle_WaitMode0(20);
    Func_0200524c(msg + 2);
  }
  Func_02005264(actor, 0);
}

void Func_020005cc(s32 actor) {
  s32 msg = (s32)MsgGaroh06;

  Func_0200524c(msg);
  UiText_OpenMessageAtObject(actor, 0);
  if (Event_AskYesNoWithActorAnimation(Func_02005384(), 0) == 0) {
    Battle_WaitMode0(10);
    Func_0200524c(msg + 1);
  } else {
    Battle_WaitMode0(20);
    Func_0200524c(msg + 2);
  }
  Func_02005264(actor, 0);
}

void Func_02000618(s32 actor) {
  s32 msg = (s32)MsgGaroh18;

  Func_0200524c(msg);
  UiText_OpenMessageAtObject(actor, 0);
  if (Event_AskYesNoWithActorAnimation(Func_02005384(), 0) == 0) {
    Battle_WaitMode0(10);
    Func_0200524c(msg + 1);
  } else {
    Battle_WaitMode0(20);
    Func_0200524c(msg + 2);
  }
  Func_02005264(actor, 0);
}

void Func_02000664(s32 actor) {
  Func_020051ac();
  Func_02005324(0);
  Func_0200524c((s32)MsgGaroh17);
  Func_0200525c(actor, 0, 10);
  Func_0200525c(actor, 0, 10);
  Func_020051b4();
}

void Func_02000698(s32 actor) {
  s32 flag;
  s32 msg;

  Func_020051ac();
  Func_02005324(0);
  flag = GameFlag_Test(0x300);
  msg = (s32)MsgGaroh04;
  if (flag) {
    Func_0200524c(msg);
  } else {
    Func_0200524c(msg);
    Func_0200525c(actor, 0, 10);
    Func_0200525c(actor, 0, 10);
  }
  Func_0200525c(actor, 0, 10);
  Func_020051b4();
}
