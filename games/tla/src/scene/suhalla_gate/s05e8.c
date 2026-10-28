#include "types.h"
#include "adventure.h"
#include "actor.h"

extern char SceneId_VenusLighthouseEntrance;
extern char SceneId_SuhallaGate3;

struct Actor *Actor_GetById(s32 id);
void FieldEvent_Wait(s32 frames);
void Actor_Jump(s32 actor, s32 speed, s32 frames);
void Actor_ResetAndSetPosition(s32 id, s32 x, s32 z);
void Party_SetFields1eeAnd1f0(s32 scene, s32 entrance);
void Party_SetFields1f2And1f4(s32 scene, s32 entrance);
void Func_02000990(void);
void Func_02000a40(s32 a, s32 b);

void Func_020005e8(void) {
  struct Actor *owner;

  Actor_Jump(8, 4, 0);
  owner = Actor_GetById(gAdventure.currentOwner);
  if (owner != NULL)
    Actor_ResetAndSetPosition(8, *(s16 *)((u8 *)owner + 10),
                                     *(s16 *)((u8 *)owner + 18));
  FieldEvent_Wait(20);
  Party_SetFields1eeAnd1f0((s32)&SceneId_SuhallaGate3, 99);
  Party_SetFields1f2And1f4((s32)&SceneId_VenusLighthouseEntrance, 98);
  gAdventure.battleRule24b = 2;
  Func_02000a40(9, 3);
  Func_02000990();
}
