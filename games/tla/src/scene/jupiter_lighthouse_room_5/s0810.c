#include "types.h"
#include "field_event.h"
#include "event_inline.h"

void Func_02002a00(void);
void Func_02002a08(void);
void Func_02002a10(void);
void Func_02002a18(struct FieldActor *object, s32 value);

void Func_02000810(s32 actor, s32 target) {
  s32 i;

  Func_02002a00();
  Func_02002a18(((struct FieldActor *)Actor_GetById(actor)), 2);
  Sound_Play(201);
  wait(50);
  Sound_Play(82);
  for (i = 0; i < 120; i++) {
    {
      struct FieldActor *object = ((struct FieldActor *)Actor_GetById(target));
      u32 facing = object->facing;


      object->facing = facing + 0x1000;
    }
    Actor_SetPartAttribute(((struct FieldActor *)Actor_GetById(target)), (i & 4) ? 7 : 0);
    wait(1);
  }
  ((struct FieldActor *)Actor_GetById(target))->facing = 0xc000;
  Actor_SetPartAttribute(((struct FieldActor *)Actor_GetById(target)), 0);
  Func_02002a18(((struct FieldActor *)Actor_GetById(actor)), 0);
  Func_02002a10();
  Func_02002a08();
  Actor_SetModeById(target, 1);
}
