// differ 2: at the second message the native loads MsgNextILl before it
// multiplies the flag (the multiply has the higher priority here). A volatile
// "+l" asm on the address gives that order but cuts the later calls from the
// launch arguments, which then load r1 before r2 (differ 2 again).
#include "types.h"
#include "text/msg_ids.h"
#include "flag_call.h"

void GameFlag_SetBit(s32 flag);
void GameFlag_ClearBit(s32 flag);
void FieldEvent_Wait(s32 frames);
void Motion_SetVarCbAndRefresh(s32 actor, s32 variant);
void Menu_AnimateSelectionToEntry(s32 entry, s32 mode);
void Actor_Jump(s32 actor, s32 speed, s32 frames);
void Func_02004db0(void);
void Func_02004df8(s32 value, s32 mode);
void Func_02004e00(void);
void Func_02004da8(void);
void Func_02004cd8(s32 message);
void Func_02004cf0(s32 actor, s32 flags);
void Func_02004e30(s32 cue);
void Func_02004e28(void);
void Func_02004d48(s32 actor, s32 value);

void Func_020011fc(void) {
  Func_02004db0();
  Func_02004df8(0x10000, 6);
  Func_02004e00();
  Func_02004da8();
  Motion_SetVarCbAndRefresh(9, 2);
  Func_02004cd8(Flag_IsSet(0x175) *
                    (MsgDjinnGiveTutorialB - MsgDjinnGiveTutorialA) +
                (s32)MsgAfterBringingA);
  Func_02004cf0(9, 0);
  FieldEvent_Wait(30);
  Func_02004e30(111);
  Menu_AnimateSelectionToEntry(0, 2);
  Flag_Clear(0x16f);
  Flag_Clear(0x171);
  Func_02004e28();
  Actor_Jump(9, 4, 30);
  Func_02004cd8(Flag_IsSet(0x175) *
                    (MsgDjinnGiveTutorialB - MsgDjinnGiveTutorialA) +
                (s32)MsgNextILl);
  Func_02004cf0(9, 0);
  Flag_Clear(0x16f);
  Flag_Set(0x171);
  Func_02004e28();
  FieldEvent_Wait(30);
  Func_02004d48(9, 5);
}
