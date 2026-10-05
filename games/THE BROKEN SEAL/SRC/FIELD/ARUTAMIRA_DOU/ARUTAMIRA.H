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
/* The actor wheel's work, past the overlay's image; the scene start clears
   it. */
extern u16 *ArutamiraDou_ClearTarget;
extern u16 gArutamiraActorWheelWork[6];

/* The actor wheel's work read as its fields: the phase the wheel task is
   in (99 once it has stopped), the ticks spent in it, the ring's angle and
   turning speed, and the travel since the last cue. */
struct ActorWheel {
    s16 phase;
    s16 ticks;
    s16 unknown_04;
    s16 angle;
    s16 speed;
    s16 travel;
};

#define ArutamiraDou_Wheel ((struct ActorWheel *)ArutamiraDou_ClearTarget)

/* The rock puzzle's bytes in the scene state, after the count of rounds
   won: how far the present round has come, and the rock the wheel chose. */
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

/*
 * Distance between two three-component 16.16 fixed-point positions.
 *
 * Each argument walks three consecutive 16.16 words in x, y, z order. The
 * per-axis deltas are taken in fixed point, shifted down to integers, squared,
 * and summed; the total is passed to the resident IWRAM integer square root.
 *
 * Expressions are preserved exactly as reconstructed: the walking-pointer form
 * is load-bearing for byte-identity and must not become struct field access.
 */

/*
 * Primary cutscene script -- resource_3bd. A no-argument, void, almost-linear
 * sequence whose one branch rejoins after both arms.
 */

/*
 * Each alias is the per-site call word the overlay image holds -- one word can
 * serve two sites with different targets -- and the macro names the engine
 * function the site reaches, keeping the site's own calling form. Names
 * without a binding elsewhere in the tree are provisional.
 */

/* Signed coordinate halfwords in a placed-actor record, named by offset. */
s32 StagedActor_FindClearPosition(struct StagedActorProbe *probe);

#endif
