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
    /* FAKEMATCH(barrier): a and b held at an empty asm, so 0x103 and the frame
     * count are built before the id, and again for the second call (with no
     * asm the calls share 0x103 in r5); as no asm, 14 of 38 instructions
     * differ in tla-en. */
    __asm__ volatile("" : "+r"(a), "+r"(b));
    Func_080d47b4(33, a, b);
    FieldMessage_ShowAtActor(33, 0);
    if (Event_AskYesNoWithActorAnimation(4, 0) == 0) {
      a = 0x103;
      b = 30;
      Func_080d47b4(33, a, b);
      Event_ShowMessageThenWait(33, 0, 10);
    } else {
      Func_02000294();
    }
  } else {
    Func_02000294();
  }
}
