#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "sequence_parent_work.h"

struct Actor *Actor_GetById(s32 id);
void Actor_TurnToFaceEachOther(s32 id, s32 other, s32 mode);
void Actor_SetModeById(u32 id, s32 mode);
void wait(s32 frames);
void Func_02001bcc(void);
void Func_02001bd4(void);
void Func_02001c6c(s32 message);
void Func_02001c7c(s32 id, s32 a, s32 b);
void Func_02001d04(s32 a);

extern u8 MsgItHasBeen;
extern u8 MsgEveryoneSpoilsMe;
extern u8 MsgWeAreThe;
extern u8 MsgAreThereOther;

/// Actor 18's talk: one message, and Actor_TurnToFaceEachOther with the party leader,
/// while the event work's halfword at 0x166 is 0, another once it is set; the
/// actor's angle is restored after.
void Func_02000308(void) {
  u8 *work = Ram_HeapSlots->eventWork;
  struct Actor *object = Actor_GetById(18);
  s16 angle = object->angle;

  /* FAKEMATCH: the action's address is built after the angle is loaded (differ
   * 4 without it). */
  __asm__("" : "+l"(object));
  object->action = 1;
  Func_02001bcc();
  Func_02001d04(0);
  if (*(s16 *)(work + 0x166) == 0) {
    Func_02001c6c((s32)&MsgItHasBeen);
    Actor_TurnToFaceEachOther(18, gSequenceParentWork.currentOwner, 2);
  } else {
    Func_02001c6c((s32)&MsgEveryoneSpoilsMe);
  }
  Actor_SetModeById(18, 0);
  Func_02001c7c(18, 0, 10);
  Func_02001bd4();
  object->action = 0;
  object->angle = angle;
  wait(1);
}

/// The same for actor 19.
void Func_02000398(void) {
  u8 *work = Ram_HeapSlots->eventWork;
  struct Actor *object = Actor_GetById(19);
  s16 angle = object->angle;

  /* FAKEMATCH: the action's address is built after the angle is loaded, as in
   * Func_02000308. */
  __asm__("" : "+l"(object));
  object->action = 1;
  Func_02001bcc();
  Func_02001d04(0);
  if (*(s16 *)(work + 0x166) == 0) {
    Func_02001c6c((s32)&MsgWeAreThe);
    Actor_TurnToFaceEachOther(19, gSequenceParentWork.currentOwner, 2);
  } else {
    Func_02001c6c((s32)&MsgAreThereOther);
  }
  Actor_SetModeById(19, 0);
  Func_02001c7c(19, 0, 10);
  Func_02001bd4();
  object->action = 0;
  object->angle = angle;
  wait(1);
}
