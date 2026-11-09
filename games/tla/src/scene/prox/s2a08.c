#include "types.h"
#include "main_routine_work.h"
#include "field_event.h"
#include "field_sprite.h"
#include "event_inline.h"

void Func_02002a08(void) {
  s32 variant =
      ((struct FieldActor *)Actor_Get(gMainRoutineWork.currentOwner))
          ->sprite->priority;

  FieldActor_SetSpritePriority(8, variant);
  FieldActor_SetSpritePriority(10, variant);
}
