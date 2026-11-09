#include "types.h"
#include "main_routine_work.h"
#include "field_event.h"
#include "event_inline.h"

#define WORD_AT(base, offset) (*(s32 *)((u8 *)(base) + (offset)))

extern s32 gData_02005a3c;

void Func_02003474();

void Func_02000060(void) {
  if (gData_02005a3c == 0) {
    Func_02003474(((struct FieldActor *)Actor_Get(24)));
    gData_02005a3c = 1;
  }
}

void Func_02000080(void) {
  if (gData_02005a3c == 0) {
    struct MainRoutineWork *party = &gMainRoutineWork;

    if (party->renderMode == 10) {
      struct FieldActor *object =
          ((struct FieldActor *)Actor_Get(party->currentOwner));

      party->renderMode = 12;
      WORD_AT(object, 48) = 0;
      WORD_AT(object, 52) = 0;
      Func_02003474(((struct FieldActor *)Actor_Get(25)));
      gData_02005a3c = 1;
    }
  }
}
