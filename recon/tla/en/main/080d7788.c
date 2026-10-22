// Near miss, differ 6 of 82: a Djinni joins. Djinni 19 means the one recorded
// for the element in the save (none: nothing happens). With flag 366 set it
// joins the current owner by its flag (48 + element * 20 + djinni), otherwise
// the least loaded owner; if it found one, the element's join effect plays at
// the actor (none for actor -1), the Djinni is added, and the actor's event
// slot is cleared. No ☀️ twin found. Left: instruction order only, three
// single moves: the save table's pointer load, the flag number's add and the
// actor copy each sit one slot from the native. The event slots are an array
// field (through a cast pointer the slot address is formed the other way
// round: differ 14).
#include "types.h"
#include "flag_call.h"
#include "party_state.h"
#include "ram_buffer.h"

s32 Djinn_AddToLeastLoadedOwnerFar(s32 element, s32 djinni);
void EventRuntime_Start(void);
void EventRuntime_End(void);
void Func_080d82e0(s32 actor);
void Func_080d8740(s32 actor);
void Djinn_RunMarsJoinEffect(s32 actor);
void Djinn_RunJupiterJoinEffect(s32 actor);
void Func_080cb82c(void);
void Func_08038328(s32 owner, s32 element, s32 djinni);
void Func_080cb8a4(void);

extern s8 gData_02001000[];

struct EventActors {
  u8 unk00[20];
  void *actors[64];
};

void Func_080d7788(s32 actor, s32 element, s32 djinni) {
  s32 owner;

  if (djinni == 19) {
    s32 recorded = gData_02001000[element + 0x1950];

    if (recorded == 0)
      return;
    djinni = recorded - 1;
  }
  if (GameFlag_Test(366)) {
    owner = gPartyState.currentOwner;
    GameFlag_SetBit(element * 20 + djinni + 48);
  } else {
    owner = Djinn_AddToLeastLoadedOwnerFar(element, djinni);
  }
  if (owner < 0)
    return;
  EventRuntime_Start();
  if (actor != -1) {
    if (element == 0)
      Func_080d82e0(actor);
    else if (element == 1)
      Func_080d8740(actor);
    else if (element == 2)
      Djinn_RunMarsJoinEffect(actor);
    else if (element == 3)
      Djinn_RunJupiterJoinEffect(actor);
  }
  Func_080cb82c();
  Func_08038328(owner, element, djinni);
  Func_080cb8a4();
  ((struct EventActors *)Ram_HeapSlots->eventWork)->actors[actor] = NULL;
  EventRuntime_End();
}
