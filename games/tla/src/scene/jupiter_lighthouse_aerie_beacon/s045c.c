#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

#include "event_inline.h"
s32 Func_02005814();
s32 Func_0200581c();
extern u8 MsgYouReLate[];

void Func_0200045c(void) {
  s32 t0;
  s32 t2;

  t0 = Event_GetFlag(0xa23);
  if (t0 != 0) {
    Event_SetFlag(0x200);
    t2 = Event_GetFlag(0xa21);
    if (t2 == 0) {
      Func_02005814();
      FieldEvent_ResolvePendingActions(0);
      FieldEvent_SetMessageId((s32)MsgYouReLate);
      Actor_ShowEmote(0x1c, 0x103, 0x1e);
      Event_ShowMessage(0x1c, 0);
      Actor_SetAnimation(27, 3);
      /* FAKEMATCH: the one-pass block ends the scheduling block after the mode
       * call: ⚓️ sets its id (r0) before its mode (r1); without it the mode is
       * set first (differ 2). */
      do {
      } while (0);
      FieldEvent_Wait(20);
      Event_ShowMessage(0x1b, 0);
      Func_0200581c();
    }
  }
}
