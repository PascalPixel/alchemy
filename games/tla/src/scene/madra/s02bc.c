#include "types.h"
#include "event_inline.h"

extern const u8 MsgHaveYouBy[];

void Func_02000294(void);

void Func_020002bc(void) {
  s32 a, b;

  Event_SetMessageId(MsgHaveYouBy);
  FieldMessage_ShowAtActor(33, 0);
  if (Event_AskYesNoWithActorAnimation(4, 0) == 0) {
    a = 0x103;
    b = 30;
    /* FAKEMATCH: the native builds 0x103 and the frame count before the id, and
     * again for the second call; direct calls share it in r5 and a forwarding
     * inline loads the id inside it. */
    __asm__ volatile("" : "+r"(a), "+r"(b));
    Func_080d47b4(33, a, b);
    FieldMessage_ShowAtActor(33, 0);
    if (Event_AskYesNoWithActorAnimation(4, 0) == 0) {
      a = 0x103;
      b = 30;
      /* FAKEMATCH: as above. */
      __asm__ volatile("" : "+r"(a), "+r"(b));
      Func_080d47b4(33, a, b);
      Event_ShowMessageThenWait(33, 0, 10);
    } else {
      Func_02000294();
    }
  } else {
    Func_02000294();
  }
}
