#ifndef ALCHEMY_ARUTAMIRA_H
#define ALCHEMY_ARUTAMIRA_H

#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

#define RECORD_COORD_X_OFFSET 10
#define RECORD_COORD_Y_OFFSET 18
#define FIELD(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

#include "STAGED_ACTOR.H"
#include "SELECT_OVERLAY_DATA_BY_RUNTIME_SELECTOR.H"

typedef struct {
    u8 filler0[0x28];
    s16 *unk28;
} Sub;

typedef struct {
    u8 filler0[6];
    u16 unk6;
    s32 unk8;
    s32 unkC;
    s32 unk10;
    u8 filler14[0x10];
    s32 unk24;
    u8 filler28[4];
    s32 unk2C;
    u8 filler30[8];
    s32 unk38;
    u8 filler3C[4];
    s32 unk40;
    u8 filler44[0xC];
    Sub *unk50;
} Ent;

typedef struct {
    s32 a, b, c, d, e, f;
} S6;

typedef struct {
    u8 filler0[8];
    s32 unk8;
    s32 unkC;
    s32 unk10;
} Ent_02000d58;

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
s32 Object_CheckMovementCollision(Ent *, Vec *);
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
