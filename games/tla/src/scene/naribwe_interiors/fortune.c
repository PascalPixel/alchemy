#include "types.h"
#include "main_routine_work.h"
#include "scene_call.h"
#include "text/msg_ids.h"

void Func_020010e0(void);
void Func_02001180(s32 a0);
void Func_02001138(s32 a0);
void Func_02001148(s32 a0, s32 a1);
void Func_020010e8(void);
s32 Func_02001188(void);
void Func_02000390(void);

void Func_02000304(void) {
  u8 *message;

  Func_020010e0();
  Func_02001180(0);
  message = MsgIAmA;
  Func_02001138((s32)message);
  FieldMessage_ShowAtActor(10, 0);
  if (Event_AskYesNoWithActorAnimation(Func_02001188(), 0) == 0) {
    Func_02001138((s32)(message + 2));
    FieldMessage_ShowAtActor(10, 0);
    if (Event_AskYesNoWithActorAnimation(Func_02001188(), 0) == 0) {
      if ((u32)gMainRoutineWork.coins > 19) {
        Func_02000390();
        Func_020010e8();
      } else {
        Func_02001138((s32)(message + 4));
        Func_02001148(10, 0);
        Func_020010e8();
      }
      return;
    }
  }
  Func_02001138((s32)MsgThenStandBack);
  Func_02001148(10, 0);
  Func_020010e8();
}
