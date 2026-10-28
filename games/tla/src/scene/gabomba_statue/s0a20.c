#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"

void Func_020056b4(struct Actor *object);
void Func_0200580c(s32 value);
void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);
void Func_02005794(s32 id, s32 value);
void FieldEvent_Wait(s32 frames);
void wait(s32 frames);
void Func_02005854(s32 sound);
void Event_FinishScreenTransition(void);

static __inline__ void Actor_Cue(s32 id, s32 value) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02005794(id, value);
}

void Func_02000a20(struct Actor *object) {
  u32 i;
  s32 delta;
  struct MainRoutineWork *party;

  Func_020056b4(object);
  Func_0200580c(0);
  Actor_SetSingleChildFlags(object, 0);
  object->motionFlags = 0;
  object->y = 0x60000;
  object->z = 0xf741cf;
  object->angle = 0x4000;
  party = &gMainRoutineWork;
  /* FAKEMATCH: the native loads gMainRoutineWork's address among the stores
   * above and builds the owner's offset after them; the plain order builds the
   * offset before the angle store (differ 4). */
  asm volatile("" : "+l"(party));
  Actor_Cue(party->currentOwner, 258);
  Actor_SetModeById(party->currentOwner, 27);
  FieldEvent_Wait(30);
  Actor_SetModeById(party->currentOwner, 28);
  FieldEvent_Wait(10);
  for (i = 0; i <= 19; i++) {
    /* FAKEMATCH: one variable set twice in the loop keeps both constants inside
     * it, as the native loads them on every pass; as plain constants loop.c
     * moves -0x300 above the loop (life 4, differ 20). */
    delta = -0x300;
    object->scaleX += delta;
    object->scaleY += delta;
    delta = -0x18000;
    object->y += delta;
    wait(1);
    if (i == 10)
      Func_02005854(204);
    if (i == 20) {
      *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 521;
      Event_FinishScreenTransition();
    }
  }
}
