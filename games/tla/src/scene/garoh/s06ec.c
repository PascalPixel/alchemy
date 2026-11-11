#include "garoh.h"
#include "event_inline.h"

extern u8 MsgGaroh20[];
extern u8 MsgGaroh21[];


void Func_020006ec(void) {
  Func_020051ac();
  FieldEvent_ResolvePendingActions(0);
  FieldActor_Stop(13);
  Actor_SetAnimation(13, 1);
  FieldEvent_Wait(10);
  Actor_FaceActor(13, 4, 0);
  if (Event_GetFlag(0x301)) {
    FieldEvent_SetMessageId((s32)MsgGaroh21);
  } else {
    FieldEvent_SetMessageId((s32)MsgGaroh20);
    FieldEvent_ShowMessageThenWait(13, 0, 10);
    {
      s32 a = 0x101;
      s32 b = 15;

      /* FAKEMATCH(barrier): an empty asm on a and b, so the second and third
       * arguments are loaded before the first; as the plain call, the first is
       * loaded earlier and 2 instructions differ. */
      asm volatile("" : "+l"(a), "+l"(b));
      FieldActor_ShowEmote(4, a, b);
    }
    {
      s32 a = 0x106;
      s32 b = 10;

      FieldActor_ShowEmote(13, a, b);
    }
    FieldEvent_ShowMessageThenWait(13, 0, 5);
    {
      s32 a = 0x108;
      s32 b = 15;

      FieldActor_ShowEmote(13, a, b);
    }
    FieldEvent_ShowMessageThenWait(13, 0, 5);
  }
  FieldEvent_ShowMessageThenWait(13, 0, 10);
  Event_SetFlag(0x301);
  Actor_SetScript(13, 2);
  Func_020051b4();
}
