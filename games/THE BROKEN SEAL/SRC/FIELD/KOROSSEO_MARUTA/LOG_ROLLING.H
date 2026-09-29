#ifndef KOROSSEO_MARUTA_LOG_ROLLING_H
#define KOROSSEO_MARUTA_LOG_ROLLING_H

#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

#include "COLOSSO_LOG_ROLLING_STAGE.H"
extern u8 gKeysHeld[];

enum LogRollingMessage {
    MSG_ROBIN_GOT = 0x96a,
    MSG_WOULD_LIKE_FRIEND_CHEER_FOR = 0x207d,
    MSG_IF_KNOW_WHO_WANT_CHEER = 0x207e,
    MSG_DO_YOUR_BEST = 0x2083,
    MSG_UNFORTUNATELY_WE_HAVE_FULL_HOUSE = 0x2084,
    MSG_MATCH_ABOUT_BEGIN_PLEASE_TAKE = 0x2085,
    MSG_YOUR_GOAL_IN_STAGE_SIMPLE = 0x20b6,
    MSG_STEPPING_STONE_STAGE = 0x20b7,
    MSG_CLEAR_STAGE_MUST_ABLE_CHANGE = 0x20be,
    MSG_PLACE_NORMALLY_CALLED_FREE_CLIMB = 0x20bf,
    MSG_IN_STAGE_MUST_TRY_OUTPACE = 0x20c2,
    MSG_CALLED_MOVING_SIDEWALK_STAGE = 0x20c3,
    MSG_HERE_MUST_FIGURE_OUT_HOW = 0x20c6,
    MSG_ANOTHER_LOG_ROLLING_AREA = 0x20c7,
    MSG_ROBIN_DID_GET_GOOD_LOOK = 0x20e5,
    MSG_WAIT_SHOULDNT_DECIDE_WHERE_BEST = 0x20e8,
    MSG_ROBIN = 0x20f1,
    MSG_ROBIN_FELL_ASLEEP = 0x214c
};

extern u8 gColossoHexChars[];

typedef struct ActorSpriteRef {
    u8 filler00[0x28];
    s16 *entry;
} ActorSpriteRef;

typedef struct StageActor {
    u8 filler00[8];
    s32 x;
    u8 filler0C[4];
    s32 z;
    u8 filler14[0x3c];
    ActorSpriteRef *sprite;
    u8 state;
} StageActor;

typedef struct ScaledStageObject {
    u8 pad00[12];
    s32 y;                      /* +12 */
    u8 pad10[32];
    s32 scale_x;                /* +48 */
    s32 scale_z;                /* +52 */
    u8 pad38[35];
    u8 state;                   /* +91 */
} ScaledStageObject;

typedef struct StageEffect {
    s32 reserved_00;
    s32 reserved_04;
    s32 x;
    s32 y;
    s32 z;
    u8 reserved_14[20];
    s32 vertical_motion;
    u8 reserved_2c[41];
    u8 state;
} StageEffect;

typedef struct StageEffect_02003d88 {
    s32 reserved_00;
    s32 reserved_04;
    s32 x;
    s32 y;
    s32 z;
    u8 reserved_14[65];
    u8 state;
    u8 reserved_56[14];
    s16 linked_effect_slot;
} StageEffect_02003d88;

typedef struct SceneControl {
    s16 enabled;
    s16 active;
    s16 scene_variant;
    s16 phase;
    s16 timer;
} SceneControl;

typedef struct SceneRecord {
    u8 pad_00[6];
    u16 facing;
    s32 x;
    s32 y;
    s32 z;
    u8 pad_14[14];
    u8 state;
    u8 pad_23;
    s32 motion_24;
    u8 pad_28[4];
    s32 motion_2c;
    s32 rate_x;
    s32 rate_z;
    u8 pad_38[33];
    u8 flags;
} SceneRecord;

typedef struct Position3 {
    s32 x;
    s32 y;
    s32 z;
} Position3;

typedef struct ActiveSubjectSlot {
    u8 pad[500];
    void *handle;
} ActiveSubjectSlot;

extern u16 Korosseo_ModeTaskMode;
extern u16 Korosseo_ModeTaskParam;
extern u16 Korosseo_ModeTaskTimer;
extern s32 Korosseo_ModeTaskScript;
extern u16 Korosseo_ModeMoveTarget;
extern u16 Korosseo_ModeMoveDuration;
extern s32 Korosseo_ModeTaskPosition;
extern s32 gColossoModeScriptDefault;   /* default handler (mode not 2/3/4) */
extern s32 gColossoModeScript2;   /* handler for mode == 2 */
extern s32 gColossoModeScript4;   /* handler for mode == 4 */
extern struct ModeRecord gColossoModeScript3B;
extern struct ModeRecord gColossoModeScript3A;
extern s16 gColossoPaletteHandle;
extern u16 gColossoPaletteFirst;
extern u16 gColossoPaletteSecond;
extern u16 gColossoPaletteMode;
extern u16 gColossoPaletteStep;
extern u16 gColossoPaletteFlags;
extern u16 gColossoPaletteNextFirst;
extern u16 gColossoPaletteNextSecond;
extern u16 gColossoPaletteSavedFirst;
extern u16 gColossoPaletteSavedSecond;
extern u16 gColossoPaletteProgress;
extern u8 gColossoRandomEffect[];
extern u8 gColossoLinkedEffect[];
extern u8 gColossoSceneDescriptor[];
extern u32 StagedActor_DirectionSteps[];


static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    return f(a0, a1, a2);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ s32 Value0(s32 (*f)())
{
    return f();
}


/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

extern u8 gColossoMultiPhaseData[];


static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

typedef struct StageObstacleActor {
    u8 unk_00[6];
    u16 attributes;
    s32 x;
    s32 y;
    s32 z;
    u8 unk_14[15];
    u8 direction_and_kind;
    u8 unk_24[12];
    s32 move_rate_x;
    s32 move_rate_z;
    u8 unk_38[29];
    u8 state;
} StageObstacleActor;

typedef struct SceneParticle {
    u8 filler00[8];
    s32 x;
    s32 y;
    s32 z;
    u8 filler14[4];
    s32 scale_x;
    s32 scale_y;
    u8 filler20[0x1c];
    s32 anchor_y;
    u8 filler40[0x15];
    u8 state;
    u8 filler56[0xe];
    s16 velocity_x;
    s16 velocity_y;
} SceneParticle;

typedef void (*SceneTaskEntry)(void);

/* The scene task's status and state words. */
extern s32 gColossoSceneTaskStatus;
extern s32 gColossoSceneTaskState;

typedef struct PrimaryStageObject {
    u8 pad00[8];
    s32 x;
    u8 pad0C[4];
    s32 y;
    u8 pad14[4];
    s32 scale_x;
    s32 scale_z;
    u8 pad20[0x10];
    s32 move_rate_x;
    s32 move_rate_z;
} PrimaryStageObject;

typedef struct SecondaryStageObject {
    u8 filler00[8];
    s32 x;
    u8 filler0C[4];
    s32 z;
    u8 filler14[0x1c];
    s32 move_rate_x;
    s32 move_rate_z;
    u8 filler38[0x1d];
    u8 state;
} SecondaryStageObject;

typedef struct StageMotionEffect {
    u8 filler00[0x30];
    s32 move_rate_x;
    s32 move_rate_z;
    u8 filler38[0x1d];
    u8 state;
} StageMotionEffect;


typedef struct PartyInteractionRecord {
    u8 padding_00[10];
    s16 x;
    u8 padding_0c[6];
    s16 y;
} PartyInteractionRecord;

s32 GetPartyMemberCount(void);

static inline void InitializeActorZero(void)
{
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
}

static inline void InitializeSelectedActor(s32 actorId)
{
    Actor_SetSpeed(actorId, 0x10000, 0x8000);
}



/* The two mode records the entry point seeds; the halfword at +26 holds the
 * per-mode span in sixtieths. */
struct ModeRecord {
    u8 pad[26];
    u16 span;
};
extern struct ModeRecord gColossoModeScript3A;
extern struct ModeRecord gColossoModeScript3B;

/* The stage's tables, scripts and state, in the overlay's read-only data:
 * the tables the entry hooks return, the scene task's status words, the
 * scene event effect and particle kinds, and the mode scripts. */
extern s32 gKorosseoMarutaEntrances[];
extern s32 gKorosseoMarutaExits[];
extern s32 gKorosseoMarutaPlacements[];
extern s32 gKorosseoMarutaEvents[];
extern u8 gColossoSceneEventEffect[];
extern u8 gColossoParticleKinds[];

/* The stage's variables after the overlay's image. */
extern s32 gColossoParticleCount;

/* The stage's per-frame tasks, still in the listing. */
void ColossoLogRollingStage_SceneTask(void);
void Korosseo_UpdateModeTask(void);
void ColossoLogRollingStage_PaletteTask(void);

#endif
