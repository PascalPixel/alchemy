#include "types.h"
#include "ram_buffer.h"
#include "event_work.h"
#include "sequence_parent_work.h"
#include "edition.h"

void Event_NoOp(void) {}

#if (LANGUAGE != LANGUAGE_SPANISH) && (LANGUAGE != LANGUAGE_ITALIAN)
void Sound_PlayFar(s32 cue);

void Func_080d2c9c(void) {
  u8 *work = Ram_HeapSlots->eventWork;

  if (work != 0) {
    s16 cue = *(s16 *)(work + 0xcb8);

    if (cue != -1)
      Sound_PlayFar(cue);
  }
}

void FieldMessage_ShowFar(s32 message, s32 mode);

void GameFlag_ClearBit(s32 flag);

s32 Party_FindOwnerWithActionFar(s32 action);

struct EventWork15a {
  u8 unk000[0x15a];
  u16 flag;
};

void Func_080d2cc4(s32 message) {
  Event_Start();
  Event_ResolvePendingActions(0);
  FieldMessage_ShowFar(message, 1);
  GameFlag_ClearBit(0x142);
  Event_End();
  if (Party_FindOwnerWithActionFar(136) != -1)
    ((struct EventWork15a *)Ram_HeapSlots->eventWork)->flag = 1;
}

struct CueObject {
  u8 unk00[8];
  s32 x;
  s32 base;
  s32 top;
};

struct EventWork1cc {
  u8 unk000[0x1cc];
  struct CueObject *focus;
};

#define ABS(x) ((x) >= 0 ? (x) : -(x))

void Func_080d2d08(struct CueObject *object, s32 cue) {
  struct CueObject *focus =
      ((struct EventWork1cc *)Ram_HeapSlots->eventWork)->focus;

  if (object == 0 || focus == 0)
    return;
  if (ABS(focus->x - object->x) > 0x980000)
    return;
  if (ABS((focus->top - focus->base) - (object->top - object->base)) > 0x700000)
    return;
  Sound_PlayFar(cue);
}

void Func_080d2d70(void) { gSequenceParentWork.counter274++; }
#endif
