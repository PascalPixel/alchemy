// Draft of overlay 64f's Func_02000494 (0x02008494), the sibling of
// Func_020003ac (s03ac.c) that sends the party to Suhalla Gate 2: differ 51
// of 130, from the listing. Every call's constants matched the native once
// routed through local.h's inline layer except the repeated ones: 920 and
// 0x4000 still end in callee-saved registers shared between calls (here r6
// and r5), which moves gAdventure out of r6 and its owner address out of
// r5. Four minutes of alchemy permute: 28, by a char temporary.
#include "../../../../games/tla/src/scene/suhalla_gate/local.h"

extern char SceneId_SuhallaGate2;
extern char SceneId_VenusLighthouseEntrance;
extern u8 MsgHaThatWas[];

void Actor_ArmCallback(s32 id, s32 angle, s32 flags);
void Actor_SetPositionAndReset(s32 id, s32 x, s32 z);
void Party_SetFields1eeAnd1f0(s32 scene, s32 entrance);
void Party_SetFields1f2And1f4(s32 scene, s32 entrance);
void Func_02000a40(s32 a, s32 b);

static __inline__ void Actor_SetPositionAndReset_(s32 id, s32 x, s32 z) {
  Actor_SetPositionAndReset_(id, x, z);
}

static __inline__ void Actor_ArmCallback_(s32 id, s32 angle, s32 flags) {
  Actor_ArmCallback_(id, angle, flags);
}

void Func_02000494(void) {
  Func_02000988();
  Func_02000a70(0);
  Func_02000a00((s32)MsgHaThatWas);
  Actor_CommitPositionAndActivate_(gAdventure.currentOwner, 0, -4);
  FieldEvent_WaitFrames(30);
  Actor_GetById(gAdventure.currentOwner)->actionFlags &= 0xfe;
  Actor_CommitPositionAndActivate_(gAdventure.currentOwner, 0, 12);
  FieldEvent_WaitFrames(10);
  Actor_CommitPositionAndActivate_(gAdventure.currentOwner, 0, 16);
  FieldEvent_WaitFrames(30);
  Func_020009d0_(13, 0x2680000, 0x3900000);
  Actor_SetPositionAndReset_(13, 600, 920);
  Func_020009d0_(15, 0x2680000, 0x3900000);
  Actor_ArmCallback_(13, 0x4000, 0);
  Actor_SetPositionAndReset_(15, 632, 920);
  Actor_ArmCallback_(15, 0x4000, 0);
  Func_020009d0_(14, 0x2680000, 0x3900000);
  Actor_SetPositionAndReset_(14, 616, 920);
  Motion_SetVarCbAndRefresh_(13, 2);
  FieldEvent_WaitFrames(20);
  Func_02000a08(13, 0, 10);
  Func_02000a08(14, 0, 10);
  Motion_SetModeAndWaitAnimation_(15, 3);
  FieldEvent_WaitFrames(30);
  Func_02000a08(15, 0, 10);
  Motion_SetModeAndWaitAnimation_(13, 3);
  Actor_SetModeById_(14, 3);
  Motion_SetModeAndWaitAnimation_(15, 3);
  Party_SetFields1eeAnd1f0((s32)&SceneId_SuhallaGate2, 99);
  Party_SetFields1f2And1f4((s32)&SceneId_VenusLighthouseEntrance, 98);
  gAdventure.battleRule24b = 2;
  Func_02000a40(9, 2);
  Func_02000990();
}
