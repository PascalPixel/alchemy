#ifndef FUNKA_H
#define FUNKA_H

#include "types.h"
#include "field_event.h"
#include "field_scene.h"

enum CollapseActor
{
	ACTOR_SUKURETA = ACTOR_FIRST_PLACED + 1,
	ACTOR_SATUROS,
	ACTOR_MENARDI,
	ACTOR_GARCIA = ACTOR_FIRST_PLACED + 5,
	ACTOR_ALEX
};

#include "facing_object.h"

typedef struct
{
	u8	filler0[6];
	u16 unk6;
	u8	filler8[72];
	u8 *unk50;
} Ent;

typedef struct
{
	u8 unk[9];
	u8 unk0 : 2;
	u8 mode : 2;
	u8 unk4 : 4;
} SpriteMode;

typedef struct
{
	u8	filler0[12];
	s32 unkC;
	u8	filler10[8];
	s32 unk18;
	s32 unk1C;
	u8	filler20[28];
	s32 unk3C;
} Ent_02002ba0;

typedef struct
{
	u8	filler0[8];
	s32 unk8;
	u8	fillerC[4];
	s32 unk10;
	u8	filler14[16];
	s32 unk24;
	u8	filler28[4];
	s32 unk2C;
	u8	filler30[8];
	s32 unk38;
	u8	filler3C[4];
	s32 unk40;
} Ent_02002c1c;

struct Actor_02002e0c
{
	u8					   filler00[8];
	s32					   x;
	s32					   y;
	s32					   z;
	u8					   filler14[4];
	s32					   amplitude_x;
	s32					   amplitude_y;
	u8					   filler20[0x44];
	u16					   frame;
	u8					   filler66[2];
	struct Actor_02002e0c *anchor;
};

struct Actor_02002e5c
{
	u8					   filler00[8];
	s32					   x;
	s32					   y;
	s32					   z;
	u8					   filler14[4];
	s32					   amplitude_x;
	s32					   amplitude_y;
	u8					   filler20[0x44];
	u16					   frame;
	u8					   filler66[2];
	struct Actor_02002e5c *anchor;
};

extern u8 *gParticleWork[];

void SoruFunka_ThrowEruptionRing(void);
void Party_RemoveOwnerRestored();
void SceneState_InitStateWordsAndSlots();
s32	 Engine_TaskAddCallback(void (*)(), s32);
void FieldScene_RunVariantStep();
void FieldEffect_InitSparkles(void);
void SceneState_ForwardByRuntimeWordBits(s32 object);
void SoruFunka_PlaceBeforeStar(s32 actor);
void SceneState_ApplyRectsByCondition(s32 a);
void SceneState_ApplyRectPairByFlag(s32 a);
void FieldScene_RunRandomHalfBranch(void);
void FieldScene_RunLateRandomHalfBranch(void);
void FieldScene_RunFourWayEffectSequence(u32 mode);
void SceneActor_PlaceAtTileAndRunSteps(s32 a, s32 b);
void SceneState_ConfigureEightCornerRegions(void);
void FieldScene_RunStepByRuntimeBits(s32 a);
void SceneState_SetValue140Mode0(void);
void SoruFunka_CleanupSceneObjects(void);
void FieldScene_RunActor15TwoStep(void);
void BattleEffect_CleanupSceneObjects(void);

void SceneEffect_AdvanceTenEntryTimers(void);

extern struct SceneEntrance Placement_Scripts[];
extern u32 Placement_Messages[];
extern struct ScenePlacement Placement_Actors[];
extern struct SceneEvent Placement_Effects[];
void	  SoruFunka_StepEmbers();
void	  SceneState_UpdateRandomTimerLevel();

extern s32 gEmberState[16];
extern s32 gEmberTimer;
extern s32 gArcEffectTimers[10];
extern s32 gEmberMask;
extern s32 gEmberLevel;
extern s32 gEmberLevelTimer;

#endif
