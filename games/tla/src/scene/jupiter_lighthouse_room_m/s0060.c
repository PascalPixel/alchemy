#include "types.h"
#include "adventure.h"
#include "field_event.h"

#define WORD_AT(base, offset) (*(s32 *)((u8 *)(base) + (offset)))

extern s32 gData_02005a3c;

struct FieldActor *Actor_GetById();
void Func_02003474();

void Func_02000060(void) {
  if (gData_02005a3c == 0) {
    Func_02003474(Actor_GetById(24));
    gData_02005a3c = 1;
  }
}

void Func_02000080(void) {
  if (gData_02005a3c == 0) {
    struct Adventure *party = &gAdventure;

    /* FAKEMATCH: ⚓️ loads the adventure's address before it builds the field
     * offset; plain field tests and a local pointer build the offset first */
    asm volatile("" : "+l"(party));
    if (party->renderMode == 10) {
      struct FieldActor *object = Actor_GetById(party->currentOwner);

      party->renderMode = 12;
      WORD_AT(object, 48) = 0;
      WORD_AT(object, 52) = 0;
      Func_02003474(Actor_GetById(25));
      gData_02005a3c = 1;
    }
  }
}
