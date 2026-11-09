#include "types.h"
#include "scene_tables.h"
#include "flag_call.h"
#include "text/msg_ids.h"
#include "event_inline.h"

void Menu_AnimateSelectionToEntry(s32 a, s32 b);
void Func_02004d48(s32 a, s32 b);
void Func_02004da8(void);
void Func_02004db0(void);
void Func_02004e00(void);
void Func_02004e28(void);

s32 Scene_PrepareMap(void) { return 0; }

/// The djinn tutorial. With flag 0x175 set each message is offset by
/// MsgDjinnGiveTutorialB less A.
// As The Broken Seal's WorldMap_ShowDjinnTutorial.
void Func_020011fc(void) {
  Func_02004db0();
  Func_080d46a4(0x10000, 6);
  Func_02004e00();
  Func_02004da8();
  Motion_SetVarCbAndRefresh(9, 2);
  Event_SetMessageId(Flag_IsSet(0x175) * ((s32)MsgDjinnGiveTutorialB -
                                          (s32)MsgDjinnGiveTutorialA) +
                     (s32)MsgAfterBringingA);
  Event_ShowMessageAndWait(9, 0);
  Event_Wait(30);
  Sound_Play(111);
  Menu_AnimateSelectionToEntry(0, 2);
  Flag_Clear(0x16f);
  Flag_Clear(0x171);
  Func_02004e28();
  Actor_Jump(9, 4, 30);
  {
    s32 flag = Flag_IsSet(0x175);
    s32 base = (s32)MsgNextILl;

    /* FAKEMATCH: the empty asm ties the flag to the message base, so the base
     * loads before the multiply; as plain C, 2 instructions differ in tla-en.
     */
    asm("" : "+r"(flag) : "r"(base));
    Event_SetMessageId(
        flag * ((s32)MsgDjinnGiveTutorialB - (s32)MsgDjinnGiveTutorialA) +
        base);
  }
  Event_ShowMessageAndWait(9, 0);
  Flag_Clear(0x16f);
  Flag_Set(0x171);
  Func_02004e28();
  Event_Wait(30);
  Func_02004d48(9, 5);
}
