/* Declarations shared by the Mars lighthouse eruption scene's sources (resource_381). */
#ifndef FUNKA_H
#define FUNKA_H

#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

/*
 * The Mars Star chamber shakes apart after Robin and Gerald take the Star.
 * The Wise One lifts a boulder over the chamber; Saturos, Menardi, Garcia
 * and Alex give up the Mars Star and leave, taking Sukureta and Jasmine.
 * The placed actors keep the Elemental Star Room's numbering: Menardi calls
 * Saturos by name here, and Alex agrees with Garcia.
 */

enum CollapseActor {
    ACTOR_SUKURETA = ACTOR_FIRST_PLACED + 1,
    ACTOR_SATUROS,
    ACTOR_MENARDI,
    ACTOR_GARCIA = ACTOR_FIRST_PLACED + 5,
    ACTOR_ALEX
};

#include "FACING_OBJECT.H"

typedef struct {
    u8 filler0[6];
    u16 unk6;
    u8 filler8[72];
    u8 *unk50;
} Ent;

typedef struct {
    u8 filler0[6];
    u16 unk6;
} Ent_02002820;

typedef struct {
    u8 unk[9];
    u8 unk0 : 2;
    u8 mode : 2;
    u8 unk4 : 4;
} SpriteMode;

typedef struct {
    u8 filler0[12];
    s32 unkC;
    u8 filler10[8];
    s32 unk18;
    s32 unk1C;
    u8 filler20[28];
    s32 unk3C;
} Ent_02002ba0;

typedef struct {
    u8 filler0[8];
    s32 unk8;
    u8 fillerC[4];
    s32 unk10;
    u8 filler14[16];
    s32 unk24;
    u8 filler28[4];
    s32 unk2C;
    u8 filler30[8];
    s32 unk38;
    u8 filler3C[4];
    s32 unk40;
} Ent_02002c1c;

/* resource_381 owner at 0x02002e0c (80 bytes), the twin of resource_371:3fb4.
 * Ported from that byte-exact owner: same field layout, same two calls. */
struct Actor_02002e0c {
    u8 filler00[8];
    s32 x;                          /* 0x08 */
    s32 y;                          /* 0x0c, only ever advanced by 0x10000 */
    s32 z;                          /* 0x10 */
    u8 filler14[4];
    s32 amplitude_x;                /* 0x18 */
    s32 amplitude_y;                /* 0x1c */
    u8 filler20[0x44];
    u16 frame;                      /* 0x64 */
    u8 filler66[2];
    struct Actor_02002e0c *anchor;  /* 0x68 */
};

/* resource_381 owner at 0x02002e5c (82 bytes), the twin of resource_371:4004.
 * Ported from that byte-exact owner: same field layout, same two calls. */
struct Actor_02002e5c {
    u8 filler00[8];
    s32 x;                          /* 0x08 */
    s32 y;                          /* 0x0c, only ever advanced by 0x10000 */
    s32 z;                          /* 0x10 */
    u8 filler14[4];
    s32 amplitude_x;                /* 0x18 */
    s32 amplitude_y;                /* 0x1c */
    u8 filler20[0x44];
    u16 frame;                      /* 0x64 */
    u8 filler66[2];
    struct Actor_02002e5c *anchor;  /* 0x68 */
};

extern u8 *gParticleWork[];

void *SoruFunka_ThrowEruptionRing();
void Party_RemoveOwnerRestored();
void SceneState_InitStateWordsAndSlots();
s32 Engine_TaskAddCallback(void (*)(), s32);
void FieldScene_RunVariantStep();
void FieldEffect_InitSparkles(void);
void SceneState_ForwardByRuntimeWordBits(s32 object);
void SceneActor_MoveTo232_125AndFace4000(s32 no);
void SceneState_ApplyRectsByCondition(s32 a);
void SceneState_ApplyRectPairByFlag(s32 a);
void FieldScene_RunRandomHalfBranch(void);
void FieldScene_RunLateRandomHalfBranch(void);
void FieldScene_RunFourWayEffectSequence(u32 mode);
void SceneActor_PlaceAtTileAndRunSteps(s32 a, s32 b);
void SceneState_ConfigureEightCornerRegions(void);
void FieldScene_RunStepByRuntimeBits(s32 a);
void SceneState_SetValue140Mode0(void);
void FieldScene_CallHelper6620(void);
void FieldScene_RunActor15TwoStep(void);
void BattleEffect_CleanupSceneObjects(void);

/*
 * Table getters published from the header words of overlay resource_381.
 * Only one overlay is resident at 0x02000000 at a time, so an address that
 * matches another overlay's names different bytes, not the same table.
 */

/* Set up the scene actors, run the two presentation phases, and dismiss the
 * actors through the shared exit sequence. Retained pending an exact build. */

void SceneEffect_AdvanceTenEntryTimers(void);

/* The scene's tables, laid out after the code. */
extern u8 Placement_Scripts[];
extern u8 Placement_Messages[];
extern u8 Placement_Actors[];
extern u8 Placement_Effects[];
void SoruFunka_StepEmbers();
void SceneState_UpdateRandomTimerLevel();

/* The scene's own variables after the image:
 * the sixteen embers' states and the ember timer, the ten arcing effects and
 * their timers, and the ember spawn mask with the level and timer that pick
 * it. */
extern s32 gEmberState[16];
extern s32 gEmberTimer;
extern s32 gArcEffectTimers[10];
extern s32 gEmberMask;
extern s32 gEmberLevel;
extern s32 gEmberLevelTimer;

#endif
