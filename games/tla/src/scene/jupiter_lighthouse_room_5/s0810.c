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
  Func_02002a18(((struct FieldActor *)Actor_Get(actor)), 2);
  Event_PlaySound(201);
  System_Wait(50);
  Event_PlaySound(82);
  for (i = 0; i < 120; i++) {
    {
      struct FieldActor *object = ((struct FieldActor *)Actor_Get(target));
      u32 facing = object->facing;


      object->facing = facing + 0x1000;
    }
    Actor_SetSpriteColor(((struct FieldActor *)Actor_Get(target)), (i & 4) ? 7 : 0);
    System_Wait(1);
  }
  ((struct FieldActor *)Actor_Get(target))->facing = 0xc000;
  Actor_SetSpriteColor(((struct FieldActor *)Actor_Get(target)), 0);
  Func_02002a18(((struct FieldActor *)Actor_Get(actor)), 0);
  Func_02002a10();
  Func_02002a08();
  Actor_SetAnimation(target, 1);
}
