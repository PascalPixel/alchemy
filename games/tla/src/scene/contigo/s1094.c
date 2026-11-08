#include "types.h"
#include "ram_buffer.h"
#include "event_work.h"
#include "main_routine_work.h"
#include "text/msg_ids.h"

/// The bytes of an actor this handler uses. unk63 is the last of the four
/// scene-owned bytes the Broken Seal's actors keep at this place.
struct TalkActor {
  u8 unk00[6];
  u16 angle; ///< 1/65536 turns
  u8 unk08[0x5b];
  u8 unk63; ///< Func_020006d0 reads it
};

struct TalkActor *Actor_GetById(s32 id);
s32 GameFlag_Test(s32 flag);
void Actor_SetAngleToward(s32 actor, s32 target, s32 mode);
void Func_0200406c(void);
void Func_02004074(void);
void Func_0200412c(s32 message);
void Func_02004144(s32 speaker, s32 flags);
void Func_02004204(s32 skip);

void Func_02001094(void) {
  struct EventWork *work = Ram_HeapSlots->eventWork;
  struct TalkActor *object = Actor_GetById(14);
  s16 angle = object->angle;

  Func_0200406c();
  Func_02004204(0);
  object->unk63 = 1;
  if (work->unk166 == 0) {
    Actor_SetAngleToward(14, gMainRoutineWork.currentOwner, 0);
    if (GameFlag_Test(0x9b2) != 0)
      Func_0200412c((s32)MsgContigo203);
    else if (GameFlag_Test(0xa21) != 0)
      Func_0200412c((s32)MsgContigo199);
    else
      Func_0200412c((s32)MsgContigo41);
  } else {
    if (GameFlag_Test(0x9b2) != 0)
      Func_0200412c((s32)MsgContigo205);
    else if (GameFlag_Test(0xa21) != 0)
      Func_0200412c((s32)MsgContigo201);
    else
      Func_0200412c((s32)MsgContigo49);
  }
  Func_02004144(14, 0);
  if (work->unk166 == 0)
    object->angle = angle;
  object->unk63 = 0;
  Func_02004074();
}
