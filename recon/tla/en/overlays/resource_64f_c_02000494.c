/* Draft of overlay 64f's Func_02000494 (0x02008494), the sibling of
 * Func_020003ac (s03ac.c) that sends the party to Suhara Gate 2: differ 51
 * of 130, from the listing. Every call's constants matched the native once
 * routed through local.h's inline layer except the repeated ones: 920 and
 * 0x4000 still end in callee-saved registers shared between calls (here r6
 * and r5), which moves gPartyState out of r6 and its owner address out of
 * r5. Four minutes of alchemy permute: 28, by a char temporary. */
#include "../../../../games/tla/src/field/suhalla_gate/local.h"

extern char SceneId_SuharaGate2;
extern char SceneId_VinasuIriguchi;
extern u8 MsgHaThatWas[];

void ObjectMotion_ArmCallback(s32 id, s32 angle, s32 flags);
void ObjectMotion_SetPositionAndReset(s32 id, s32 x, s32 z);
void Party_SetFields1eeAnd1f0(s32 scene, s32 entrance);
void Party_SetFields1f2And1f4(s32 scene, s32 entrance);
void Func_02000a40(s32 a, s32 b);

static __inline__ void ObjectMotion_SetPositionAndReset_(s32 id, s32 x, s32 z)
{
    ObjectMotion_SetPositionAndReset_(id, x, z);
}

static __inline__ void ObjectMotion_ArmCallback_(s32 id, s32 angle, s32 flags)
{
    ObjectMotion_ArmCallback_(id, angle, flags);
}

void Func_02000494(void)
{
    Func_02000988();
    Func_02000a70(0);
    Func_02000a00((s32)MsgHaThatWas);
    ObjectMotion_CommitPositionAndActivate_(gPartyState.current_owner, 0, -4);
    Battle_WaitMode0(30);
    Object_GetById(gPartyState.current_owner)->action_flags &= 0xfe;
    ObjectMotion_CommitPositionAndActivate_(gPartyState.current_owner, 0, 12);
    Battle_WaitMode0(10);
    ObjectMotion_CommitPositionAndActivate_(gPartyState.current_owner, 0, 16);
    Battle_WaitMode0(30);
    Func_020009d0_(13, 0x2680000, 0x3900000);
    ObjectMotion_SetPositionAndReset_(13, 600, 920);
    Func_020009d0_(15, 0x2680000, 0x3900000);
    ObjectMotion_ArmCallback_(13, 0x4000, 0);
    ObjectMotion_SetPositionAndReset_(15, 632, 920);
    ObjectMotion_ArmCallback_(15, 0x4000, 0);
    Func_020009d0_(14, 0x2680000, 0x3900000);
    ObjectMotion_SetPositionAndReset_(14, 616, 920);
    Motion_SetVarCbAndRefresh_(13, 2);
    Battle_WaitMode0(20);
    Func_02000a08(13, 0, 10);
    Func_02000a08(14, 0, 10);
    Motion_SetModeAndWaitAnimation_(15, 3);
    Battle_WaitMode0(30);
    Func_02000a08(15, 0, 10);
    Motion_SetModeAndWaitAnimation_(13, 3);
    Object_SetModeById_(14, 3);
    Motion_SetModeAndWaitAnimation_(15, 3);
    Party_SetFields1eeAnd1f0((s32)&SceneId_SuharaGate2, 99);
    Party_SetFields1f2And1f4((s32)&SceneId_VinasuIriguchi, 98);
    gPartyState.battle_rule_24b = 2;
    Func_02000a40(9, 2);
    Func_02000990();
}
