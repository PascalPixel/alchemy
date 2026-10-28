#include "types.h"
#include "ram_buffer.h"
#include "sequence_parent_work.h"
#include "text/msg_ids.h"

void Func_020002b8(void);
void Func_020002c0(void);
void Func_02000300(s32 message);
void Func_02000310(s32 actor, s32 a, s32 b);
void Func_02000318(s32 actor, s32 a);
void Func_02000338(void);
void Func_02000340(s32 skip);
s32 FieldMessage_ShowAtActor(s32 speaker, s32 flags);
s32 Event_AskYesNoWithActorAnimation(s32 actor, s32 flags);
void Motion_SetModeAndWaitAnimation(s32 actor, s32 mode);
void Camera_MoveWithinMapBounds(s32 x, s32 y, s32 z, s32 mode);
void GameFlag_SetBit(s32 flag);

void Func_02000224(void) {
  Func_020002b8();
  Func_02000340(0);
  Func_02000300((s32)MsgMadraHeardAboutYou);
  Camera_MoveWithinMapBounds(0xa40000, -1, 0x1040000, 1);
  Func_02000338();
  Motion_SetModeAndWaitAnimation(8, 3);
  Func_02000318(8, 0);
  Motion_SetModeAndWaitAnimation(9, 3);
  Func_02000318(9, 0);
  GameFlag_SetBit(0x916);
  Func_020002c0();
}
