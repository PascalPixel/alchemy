#include "madra_interiors.h"
#include "edition.h"

const u8 *Func_0200004c(void) {
  if (gAdventure.scene == (s32)&SceneId_Ov659Room2)
    return gData_02002828;
  if (gAdventure.entrance == 20)
    return gData_02002750;
  if (FlagCheck(0x8ff))
    return gData_02002480;
  return gData_02002240;
}

void Func_020000a4(void) {
  s32 msg;

  Func_02001d50();
  Func_02001eb8(0);
  msg = (s32)MsgMadraInteriors06;
  Func_02001e18(msg);
  FieldMessage_ShowAtActor(9, 0);
  if (Event_AskYesNoWithActorAnimation(Func_02001ee8(), 0) == 0) {
    FieldEvent_Wait(10);
    Func_02001e18(msg + 1);
  } else {
    FieldEvent_Wait(20);
    Func_02001e18(msg + 2);
  }
  Func_02001e30(9, 0);
  Func_02001d58();
}

void Func_020000fc(void) {
  u16 facing = Actor_GetById(gAdventure.currentOwner)->angle;

  /* FAKEMATCH: the one-pass block keeps the quadrant offset after the facing
   * load. */
  do {
    if ((u16)((facing + 0x2000) & ~0x3fff) == 0xc000) {
      Func_02001f08(1, 22);
    } else {
#if (LANGUAGE == LANGUAGE_JAPANESE)
      // The Japanese edition has no choice prompt.
      if (FlagCheck(0x8ff))
        Func_02001e18((s32)MsgMadraInteriors155);
      else if (FlagCheck(0x8ab))
        Func_02001e18((s32)MsgMadraInteriors87);
      else
        Func_02001e18((s32)MsgMadraInteriors39);
      Func_02001e30(22, 0);
#else
      if (FlagCheck(0x8ff)) {
        s32 msg = (s32)MsgMadraInteriors155;

        Func_02001e18(msg);
        FieldMessage_ShowAtActor(22, 0);
        if (Event_AskYesNoWithActorAnimation(Func_02001ee8(), 0) == 0) {
          FieldEvent_Wait(10);
          Func_02001e18(msg + 1);
        } else {
          FieldEvent_Wait(20);
          Func_02001e18(msg + 2);
        }
        Func_02001e30(22, 0);
      } else {
        if (FlagCheck(0x8ab))
          Func_02001e18((s32)MsgMadraInteriors87);
        else
          Func_02001e18((s32)MsgMadraInteriors39);
        Func_02001e30(22, 0);
      }
#endif
    }
  } while (0);
}
