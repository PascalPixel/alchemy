#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "event_inline.h"

struct FieldActor;

struct FieldActor *OverlayObject_PrepareObject(s32 x, s32 y, s32 z, s32 kind);
void Func_02004eec(s32 id, s32 a, s32 b);

static __inline__ void Func_02004eec_(s32 id, s32 a, s32 b) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02004eec(id, a, b);
}

void Func_0200036c(void) {
  Actor_SetSpritePriority(gMainRoutineWork.currentOwner, 3);
  OverlayObject_PrepareObject(0x2c80000, 0, 0x1d40000, 286);
  OverlayObject_PrepareObject(0x2c80000, 0, 0x1ea0000, 286);
  Func_02004eec_(gMainRoutineWork.currentOwner, 6, 0x800);
  Sound_Play(310);
  GameFlag_SetBit(290);
  Import_EventSetExitRequest(8);
}
