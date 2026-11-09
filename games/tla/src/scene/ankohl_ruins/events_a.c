#include "ankohl_ruins.h"

void Func_02002e7c(s32 id, s32 a, s32 b);
void Sound_Play(s32 a);
void Import_EventSetExitRequest(s32 a);

static __inline__ void Actor_Start(s32 id, s32 a, s32 b) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02002e7c(id, a, b);
}

void Func_02000384(void) {
  Actor_SetSpritePriority(gMainRoutineWork.currentOwner, 2);
  Actor_Start(gMainRoutineWork.currentOwner, 6, 0x1000);
  Sound_Play(0x136);
  GameFlag_SetBit(0x122);
  Import_EventSetExitRequest(13);
}
