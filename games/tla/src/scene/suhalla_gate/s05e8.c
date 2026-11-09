#include "types.h"
#include "main_routine_work.h"
#include "actor.h"
#include "event_inline.h"

extern char SceneId_VenusLighthouseEntrance;
extern char SceneId_SuhallaGate3;

void Func_02000990(void);
void Func_02000a40(s32 a, s32 b);

void Func_020005e8(void) {
  struct Actor *owner;

  Actor_Jump(8, 4, 0);
  owner = Actor_GetById(gMainRoutineWork.currentOwner);
  if (owner != NULL)
    Actor_ResetAndSetPosition(8, *(s16 *)((u8 *)owner + 10),
                                     *(s16 *)((u8 *)owner + 18));
  Event_Wait(20);
  Party_SetFields1eeAnd1f0((s32)&SceneId_SuhallaGate3, 99);
  Party_SetFields1f2And1f4((s32)&SceneId_VenusLighthouseEntrance, 98);
  gMainRoutineWork.battleRule24b = 2;
  Func_02000a40(9, 3);
  Func_02000990();
}
