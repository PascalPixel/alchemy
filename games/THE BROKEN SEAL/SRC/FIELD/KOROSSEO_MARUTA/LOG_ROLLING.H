#ifndef KOROSSEO_MARUTA_LOG_ROLLING_H
#define KOROSSEO_MARUTA_LOG_ROLLING_H

#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

#include "COLOSSO_LOG_ROLLING_STAGE.H"
extern u8 gKeysHeld[];

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
extern s16 Korosseo_MarkerSlot;
extern u16 Korosseo_MarkerX;
extern u16 Korosseo_MarkerY;
extern u16 Korosseo_MarkerPriority;
extern u16 Korosseo_MarkerBlink;
extern u16 Korosseo_MarkerSteps;
extern u16 Korosseo_MarkerEndX;
extern u16 Korosseo_MarkerEndY;
extern u16 Korosseo_MarkerStartX;
extern u16 Korosseo_MarkerStartY;
extern u16 Korosseo_MarkerStep;
extern u8 gColossoRandomEffect[];
extern u8 gColossoLinkedEffect[];
extern u8 Korosseo_GaugeGraphics[];
extern u32 StagedActor_DirectionSteps[];

extern u8 gColossoMultiPhaseData[];
/* The action scripts the multi-phase sequence gives actor 8 and the leader. */
extern u8 KorosseoMaruta_Actor8Action[];
extern u8 KorosseoMaruta_LeaderActionA[];
extern u8 KorosseoMaruta_LeaderActionB[];
extern u8 KorosseoMaruta_LeaderActionC[];

/* COMMON/KOROSSEO/SOLO_COMPETITOR.C: enter one competitor's trial. */
void Korosseo_SelectSoloCompetitor(s32 competitor);

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
void BattleEffect_PauseObject(s32 id);
void Korosseo_UpdateModeTask(void);
void Korosseo_UpdateMarker(void);

#endif
