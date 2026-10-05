#ifndef ALCHEMY_ARUTAMIRA_H
#define ALCHEMY_ARUTAMIRA_H

#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

#define FIELD(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

#include "STAGED_ACTOR.H"
#include "SELECT_OVERLAY_DATA_BY_RUNTIME_SELECTOR.H"

typedef struct {
    s32 x;
    s32 y;
    s32 z;
} Vec;

extern u8 ArutamiraDou_SceneTableA[];
extern u8 ArutamiraDou_SceneTableB[];
extern u16 *ArutamiraDou_ClearTarget;
extern u16 gArutamiraActorWheelWork[6];

struct ActorWheel {
    s16 phase;
    s16 ticks;
    s16 unknown_04;
    s16 angle;
    s16 speed;
    s16 travel;
};

#define ArutamiraDou_Wheel ((struct ActorWheel *)ArutamiraDou_ClearTarget)

struct RockPuzzle {
    u8 state;
    s8 chosen;
};

#define ArutamiraDou_RockPuzzle ((struct RockPuzzle *)&gSceneState[1])

void BattleFx_SetPhaseRequest(s32, s32);
void Battle_ResetEffectCounter();
void Motion_LaunchFromFocusedObject();
void Audio_PlayCueFromEventWork();
void ArutamiraDou_SettleActorOnCell(void);
void DisplayTransition_InitializeBattleEffectState(s32);
void ArutamiraDou_ApplyFadeBlend(void);
void ArutamiraDou_SetFadeLevel(s32 level);
s32 Object_CheckMovementCollision(struct FieldActor *actor, void *position);
void Vector_AddPolarOffset(s32 first, s32 second, s32 *position);
s32 BattleFx_HasReachedTarget(void *object);
void BattleFx_ClearOwnedSlot(void *object);

s32 StagedActor_FindClearPosition(struct StagedActorProbe *probe);

#endif
