#include "types.h"
#include "party_state.h"
#include "object_runtime.h"

extern char SceneId_Ov64dRoom1;
extern char SceneId_SuharaGate3;

struct ObjectRuntime *Object_GetById(s32 id);
void Battle_WaitMode0(s32 frames);
void ObjectMotion_Launch(s32 actor, s32 speed, s32 frames);
void ObjectMotion_ResetAndSetPosition(s32 id, s32 x, s32 z);
void Party_SetFields1eeAnd1f0(s32 scene, s32 entrance);
void Party_SetFields1f2And1f4(s32 scene, s32 entrance);
void Func_02000990(void);
void Func_02000a40(s32 a, s32 b);

void Func_020005e8(void)
{
    struct ObjectRuntime *owner;

    ObjectMotion_Launch(8, 4, 0);
    owner = Object_GetById(gPartyState.current_owner);
    if (owner != NULL)
        ObjectMotion_ResetAndSetPosition(8, *(s16 *)((u8 *)owner + 10), *(s16 *)((u8 *)owner + 18));
    Battle_WaitMode0(20);
    Party_SetFields1eeAnd1f0((s32)&SceneId_SuharaGate3, 99);
    Party_SetFields1f2And1f4((s32)&SceneId_Ov64dRoom1, 98);
    gPartyState.battle_rule_24b = 2;
    Func_02000a40(9, 3);
    Func_02000990();
}
