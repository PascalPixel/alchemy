#ifndef ALCHEMY_UNIT_SCENE_KOROSSEO_MARUTA_H
#define ALCHEMY_UNIT_SCENE_KOROSSEO_MARUTA_H

#include "types.h"
#include "field_scene.h"

typedef struct SceneParticle
{
	u8	filler00[8];
	s32 x;
	s32 y;
	s32 z;
	u8	filler14[4];
	s32 scale_x;
	s32 scale_y;
	u8	filler20[0x1c];
	s32 anchor_y;
	u8	filler40[0x15];
	u8	state;
	u8	filler56[0xe];
	s16 velocity_x;
	s16 velocity_y;
} SceneParticle;

typedef struct StageEffect
{
	s32 reserved_00;
	s32 reserved_04;
	s32 x;
	s32 y;
	s32 z;
	u8	reserved_14[20];
	s32 vertical_motion;
	u8	reserved_2c[41];
	u8	state;
} StageEffect;

typedef struct StageEffect_02003d88
{
	s32 reserved_00;
	s32 reserved_04;
	s32 x;
	s32 y;
	s32 z;
	u8	reserved_14[65];
	u8	state;
	u8	reserved_56[14];
	s16 linked_effect_slot;
} StageEffect_02003d88;

u8 *ColossoLogRollingStage_GetScriptData(void);
s32 ColossoLogRollingStage_GetMessageData(void);
u8 *ColossoLogRollingStage_GetActorData(void);
u8 *ColossoLogRollingStage_GetEffectData(void);
s32 ColossoLogRollingStage_GetSceneEventState(void);
s32 StageSetup_BuildAndDispatch(void);
extern const s32 KorosseoMaruta_Record30Script[];
extern const s8 KorosseoMaruta_LogPattern[][6];
extern const u16 gColossoEarlySequenceData[];
extern const s32 gColossoSceneEventEffect[];
extern const u8 Korosseo_PortraitPaletteOffsets[];
extern const s16 gColossoModeScript2[];
extern const s16 gColossoModeScript3B[];
extern char gColossoHexChars[20];
extern s32 gColossoRandomEffect[];
extern s32 gColossoLinkedEffect[];
extern u32 StagedActor_DirectionSteps[];
extern struct SceneEntrance gKorosseoMarutaEntrances[];
extern u32 gKorosseoMarutaExits[];
extern struct ScenePlacement gKorosseoMarutaPlacements[];
extern s32 gColossoSceneTaskStatus;
extern s32 gColossoSceneTaskState;
extern struct SceneEvent gKorosseoMarutaEvents[];
extern s32 KorosseoMaruta_Actor8Action[];
extern s32 KorosseoMaruta_LeaderActionA[];
extern s32 KorosseoMaruta_LeaderActionB[];
extern s32 KorosseoMaruta_LeaderActionC[];
extern s32 gColossoMultiPhaseData[];
extern s32 gColossoParticleKinds[];
extern s16 Korosseo_PortraitSlot;
extern s16 gColossoModeScriptDefault[];
extern s16 gColossoModeScript4[];
extern s16 gColossoModeScript3A[];
extern s16 Korosseo_MarkerSlot;
extern s32 Korosseo_RivalFinishScript[];
extern s32 KorosseoMaruta_PlaceScript[];

#endif
