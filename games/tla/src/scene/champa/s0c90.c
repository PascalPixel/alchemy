#include "types.h"
#include "field_event.h"
#include "event_inline.h"

extern const u8 gData_02003620[];

void Func_020029dc(struct FieldActor *object, s32 mode);
void Func_020029e4(struct FieldActor *object, const void *script);

void Func_02000c40(s32 x, s32 y, s32 z, s32 angle, s32 frames);

void Func_02000c90(s32 firstId, s32 secondId) {
  struct FieldActor *first = ((struct FieldActor *)Actor_GetById(firstId));
  struct FieldActor *second = ((struct FieldActor *)Actor_GetById(secondId));
  s32 rise = 0x1c0000;
  s32 y;

  y = first->y;
  Func_02000c40(first->x, y + rise, first->z, 0x1000, 15);
  Sound_Play(151);
  Event_Wait(15);
  y = second->y;
  Func_02000c40(second->x, y + rise, second->z, 0xf000, 15);
  Sound_Play(151);
  Event_Wait(15);
  Func_02000c40(first->x, first->y + rise, first->z, 0x1000, 15);
  Sound_Play(151);
  Event_Wait(15);
  Func_02000c40(second->x, second->y + rise, second->z, 0xf000, 15);
  Sound_Play(151);
  Event_Wait(15);
}
