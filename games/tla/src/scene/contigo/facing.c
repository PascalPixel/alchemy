#include "types.h"
#include "actor.h"
#include "adventure.h"
#include "text/msg_ids.h"

struct Actor *Actor_GetById();
s32 GameFlag_Test(s32 flag);
void Func_0200406c(void);
void Func_02004074(void);
void Func_0200412c(s32 message);
void Func_02004144(s32 speaker, s32 flags);
void Func_02004204(s32 skip);
void Func_02004294(s32 a, s32 b);
void Func_0200429c(s32 a);
void Func_020042a4(s32 a, s32 b);

s32 Func_02000cc4(void) {
  u16 turn;
  s32 facing;

  turn = Actor_GetById(gAdventure.currentOwner)->angle + 0x5fff;
  facing = 1;
  if (turn >= 0x3fff)
    facing = 0;
  return facing;
}

void Func_02000cf4(void) {
  if (Func_02000cc4()) {
    Func_02004294(28, 13);
  } else {
    Func_0200406c();
    Func_02004204(0);
    if (GameFlag_Test(0x9b2))
      Func_0200412c((s32)MsgContigo176);
    else
      Func_0200412c((s32)MsgContigo74);
    Func_02004144(13, 0);
    Func_02004074();
  }
}

void Func_02000d44(void) {
  if (Func_02000cc4()) {
    Func_02004294(26, 18);
  } else {
    Func_0200406c();
    Func_02004204(0);
    if (GameFlag_Test(0x9b2))
      Func_0200412c((s32)MsgContigo172);
    else
      Func_0200412c((s32)MsgContigo70);
    Func_02004144(18, 0);
    Func_02004074();
  }
}

void Func_02000d94(void) {
  if (Func_02000cc4()) {
    Func_02004294(27, 19);
  } else {
    Func_0200406c();
    Func_02004204(0);
    if (GameFlag_Test(0x9b2))
      Func_0200412c((s32)MsgContigo174);
    else
      Func_0200412c((s32)MsgContigo72);
    Func_02004144(19, 0);
    Func_02004074();
  }
}

void Func_02000de4(void) {
  if (Func_02000cc4()) {
    Func_020042a4(12, 20);
  } else {
    Func_0200406c();
    Func_02004204(0);
    if (GameFlag_Test(0x9b2))
      Func_0200412c((s32)MsgContigo178);
    else if (GameFlag_Test(0xa21))
      Func_0200412c((s32)MsgContigo128);
    else
      Func_0200412c((s32)MsgContigo76);
    Func_02004144(20, 0);
    Func_02004074();
  }
}

void Func_02000e50(void) {
  if (Func_02000cc4()) {
    Func_0200429c(28);
  } else {
    Func_0200406c();
    Func_02004204(0);
    if (GameFlag_Test(0x9b2))
      Func_0200412c((s32)MsgContigo186);
    else if (GameFlag_Test(0xa21))
      Func_0200412c((s32)MsgContigo136);
    else
      Func_0200412c((s32)MsgContigo88);
    Func_02004144(28, 0);
    Func_02004074();
  }
}
