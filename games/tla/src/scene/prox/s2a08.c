#include "types.h"
#include "main_routine_work.h"
#include "field_event.h"
#include "field_sprite.h"
#include "event_inline.h"

void Func_02002a08(void) {
  s32 variant =
      ((struct FieldActor *)Actor_GetById(gMainRoutineWork.currentOwner))
          ->sprite->priority;

  Actor_SetSpritePriority(8, variant);
  Actor_SetSpritePriority(10, variant);
}
