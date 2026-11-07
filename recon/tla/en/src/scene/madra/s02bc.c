#include "types.h"
#include "call.h"

extern const u8 MsgHaveYouBy[];

s32 FieldMessage_ShowAtActor(s32 id, s32 flags);
s32 Event_AskYesNoWithActorAnimation(s32 id, s32 flags);
void Func_02000294(void);
void Func_02002bd0(const u8 *message);
void Func_02002be0(s32 id, s32 a, s32 b);
void Func_02002c08(s32 id, s32 a, s32 b);

void Func_020002bc(void) {
  s32 a, b;

  Func_02002bd0(MsgHaveYouBy);
  FieldMessage_ShowAtActor(33, 0);
  if (Event_AskYesNoWithActorAnimation(4, 0) == 0) {
    a = 0x103;
    b = 30;
    /* FAKEMATCH: the native builds 0x103 and the frame count before the id, and
     * again for the second call; direct calls share it in r5 and a forwarding
     * inline loads the id inside it. */
    __asm__ volatile("" : "+r"(a), "+r"(b));
    Func_02002c08(33, a, b);
    FieldMessage_ShowAtActor(33, 0);
    if (Event_AskYesNoWithActorAnimation(4, 0) == 0) {
      a = 0x103;
      b = 30;
      /* FAKEMATCH: as above. */
      __asm__ volatile("" : "+r"(a), "+r"(b));
      Func_02002c08(33, a, b);
      Func_02002be0(33, 0, 10);
    } else {
      Func_02000294();
    }
  } else {
    Func_02000294();
  }
}
