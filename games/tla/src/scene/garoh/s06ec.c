#include "garoh.h"

extern u8 MsgGaroh20[];
extern u8 MsgGaroh21[];

void Actor_Stop(s32 id);
void Actor_SetModeById(u32 id, s32 mode);
void Actor_SetAngleToward(s32 id, s32 target, s32 frames);
void Actor_SetMovementTypeById(s32 id, s32 callback);
void Func_080d47b4(s32 id, s32 a, s32 b);

void Func_020006ec(void) {
  Func_020051ac();
  Event_ResolvePendingActions(0);
  Actor_Stop(13);
  Actor_SetModeById(13, 1);
  Event_Wait(10);
  Actor_SetAngleToward(13, 4, 0);
  if (GameFlag_Test(0x301)) {
    Event_SetMessageId((s32)MsgGaroh21);
  } else {
    Event_SetMessageId((s32)MsgGaroh20);
    Event_ShowMessageThenWait(13, 0, 10);
    {
      s32 a = 0x101;
      s32 b = 15;

      /* FAKEMATCH: the native loads the second and third arguments before the
       * first; the plain call loads the first earlier (one instruction moved,
       * differ 2). */
      asm volatile("" : "+l"(a), "+l"(b));
      Func_080d47b4(4, a, b);
    }
    {
      s32 a = 0x106;
      s32 b = 10;

      /* FAKEMATCH: the native loads the second and third arguments before the
       * first; the plain call loads the first earlier (one instruction moved,
       * differ 2). */
      asm volatile("" : "+l"(a), "+l"(b));
      Func_080d47b4(13, a, b);
    }
    Event_ShowMessageThenWait(13, 0, 5);
    {
      s32 a = 0x108;
      s32 b = 15;

      /* FAKEMATCH: the native loads the second and third arguments before the
       * first; the plain call loads the first earlier (one instruction moved,
       * differ 2). */
      asm volatile("" : "+l"(a), "+l"(b));
      Func_080d47b4(13, a, b);
    }
    Event_ShowMessageThenWait(13, 0, 5);
  }
  Event_ShowMessageThenWait(13, 0, 10);
  GameFlag_SetBit(0x301);
  Actor_SetMovementTypeById(13, 2);
  Func_020051b4();
}
