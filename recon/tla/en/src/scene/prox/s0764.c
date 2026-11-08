#include "types.h"
#include "ram_buffer.h"
#include "event_work.h"
#include "main_routine_work.h"

/// The bytes of an actor these handlers use. unk63 is the last of the four
/// scene-owned bytes the Broken Seal's actors keep at this place.
struct TalkActor {
  u8 unk00[6];
  u16 angle; ///< 1/65536 turns
  u8 unk08[0x5b];
  u8 unk63; ///< Func_020002d8 reads it
};

struct TalkActor *Actor_GetById(s32 id);
void Actor_SetAngleToward(s32 id, s32 other, s32 flags);
void Func_02004464(void);
void Func_0200446c(void);
void Func_0200451c(s32 message);
void Func_02004534(s32 id, s32 flags);
void Func_020045ec(s32 value);

extern u8 MsgProx113[];
extern u8 MsgProx114[];
extern u8 MsgProx115[];
extern u8 MsgProx117[];
extern u8 MsgProx118[];
extern u8 MsgProx119[];

void Func_02000764(void) {
  struct EventWork *work = Ram_HeapSlots->eventWork;
  struct TalkActor *object = Actor_GetById(16);
  s16 angle = object->angle;

  Func_02004464();
  Func_020045ec(0);
  object->unk63 = 1;
  if (work->unk166 == 0) {
    Actor_SetAngleToward(16, gMainRoutineWork.currentOwner, 0);
    Func_0200451c((s32)MsgProx113);
  } else {
    Func_0200451c((s32)MsgProx117);
  }
  Func_02004534(16, 0);
  if (work->unk166 == 0)
    object->angle = angle;
  object->unk63 = 0;
  Func_0200446c();
}

void Func_020007f4(void) {
  struct EventWork *work = Ram_HeapSlots->eventWork;
  struct TalkActor *object = Actor_GetById(17);
  s16 angle = object->angle;

  Func_02004464();
  Func_020045ec(0);
  object->unk63 = 1;
  if (work->unk166 == 0) {
    Actor_SetAngleToward(17, gMainRoutineWork.currentOwner, 0);
    Func_0200451c((s32)MsgProx114);
  } else {
    Func_0200451c((s32)MsgProx118);
  }
  Func_02004534(17, 0);
  if (work->unk166 == 0)
    object->angle = angle;
  object->unk63 = 0;
  Func_0200446c();
}

void Func_02000884(void) {
  struct EventWork *work = Ram_HeapSlots->eventWork;
  struct TalkActor *object = Actor_GetById(18);
  s16 angle = object->angle;

  Func_02004464();
  Func_020045ec(0);
  object->unk63 = 1;
  if (work->unk166 == 0) {
    Actor_SetAngleToward(18, gMainRoutineWork.currentOwner, 0);
    Func_0200451c((s32)MsgProx115);
  } else {
    Func_0200451c((s32)MsgProx119);
  }
  Func_02004534(18, 0);
  if (work->unk166 == 0)
    object->angle = angle;
  object->unk63 = 0;
  Func_0200446c();
}
