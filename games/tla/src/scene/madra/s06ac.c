#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "event_work.h"
#include "field_sprite.h"
#include "event_inline.h"

void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);
void Func_0200068c(void);
void Func_02000f80(void);
void Func_020019d8(void);

/// Madra's prepare step: sets up actor 26 (larger, its sprite flags set)
/// and places actors 26 to 28, 33 and 66 by the story flags, then runs
/// Func_02000f80 on entrance 9 and Func_020019d8 once on entrance 7.
s32 Func_020006ac(void) {
  struct Actor *object;

  ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x100;
  if (Event_GetFlag(0x8ff)) {
    Func_0200068c();
  } else {
    Actor_SetSingleChildFlags(Actor_GetById(26), 0);
    object = Actor_GetById(26);
    object->radius = Actor_GetById(26)->radius * 4 / 3;
    ((struct FieldSprite *)Actor_GetById(26)->animation)->nextFrame |= 8;
    {
      struct FieldSprite *sprite = Actor_GetById(26)->animation;

      sprite->unk11 = (sprite->unk11 & 3) | 32;
    }
    ((struct FieldSprite *)Actor_GetById(26)->animation)->unk19 = 1;
    if (Event_GetFlag(0x8ff) == 0 && Event_GetFlag(0x8ab) != 0) {
      Actor_SetHorizontalPositionWithTerrain(27, 0, 0);
      Actor_SetHorizontalPositionWithTerrain(28, 0, 0);
      if (Event_GetFlag(0x280) == 0)
        Actor_SetHorizontalPositionWithTerrain(26, 0, 0);
    } else if (Event_GetFlag(0x8ff) != 0 || Event_GetFlag(0x910) != 0 ||
               Event_GetFlag(0x280) == 0) {
      Actor_SetHorizontalPositionWithTerrain(26, 0, 0);
    }
  }
  if (gMainRoutineWork.entrance == 9 && Event_GetFlag(0x913) == 0)
    Func_02000f80();
  if (Event_GetFlag(0x8ff) != 0 && Event_GetFlag(0x989) != 0) {
    if (gMainRoutineWork.entrance == 7 && Event_GetFlag(0x98e) == 0) {
      Func_020019d8();
      Event_SetFlag(0x98e);
    }
    if (Event_GetFlag(0x98e))
      Actor_SetHorizontalPositionWithTerrain(33, 0, 0);
  } else {
    Actor_SetHorizontalPositionWithTerrain(66, 0, 0);
  }
  return 0;
}
