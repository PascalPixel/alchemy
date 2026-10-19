#include "types.h"
#include "field_event.h"

struct FieldActor *Object_GetById(s32 id);
void WaitFrames(s32 frames);
void Object_SetPartAttribute(struct FieldActor *object, s32 value);
void Func_02002a00(void);
void Func_02002a08(void);
void Func_02002a10(void);
void Func_02002a18(struct FieldActor *object, s32 value);
void Func_02002a40(s32 cue);

void Func_02000810(s32 actor, s32 target) {
  s32 i;

  Func_02002a00();
  Func_02002a18(Object_GetById(actor), 2);
  Func_02002a40(201);
  WaitFrames(50);
  Func_02002a40(82);
  for (i = 0; i < 120; i++) {
    {
      struct FieldActor *object = Object_GetById(target);
      u32 facing = object->facing;

      /* FAKEMATCH: a forced temporary and the loop note after it read the
       * facing before the step 0x1000 starts, as native; as facing += 0x1000,
       * 2 instructions differ in tla-en. */
      do {
      } while (0);
      object->facing = facing + 0x1000;
    }
    Object_SetPartAttribute(Object_GetById(target), (i & 4) ? 7 : 0);
    WaitFrames(1);
  }
  Object_GetById(target)->facing = 0xc000;
  Object_SetPartAttribute(Object_GetById(target), 0);
  Func_02002a18(Object_GetById(actor), 0);
  Func_02002a10();
  Func_02002a08();
  Object_SetModeById(target, 1);
}
