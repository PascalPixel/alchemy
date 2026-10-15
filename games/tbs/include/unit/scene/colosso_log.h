#ifndef ALCHEMY_UNIT_SCENE_COLOSSO_LOG_H
#define ALCHEMY_UNIT_SCENE_COLOSSO_LOG_H

#include "types.h"
#include "field_scene.h"

typedef struct SceneParticle
{
	u8	filler00[8];
	s32 x;
	s32 y;
	s32 z;
	u8	filler14[4];
	s32 scaleX;
	s32 scaleY;
	u8	filler20[0x1c];
	s32 anchorY;
	u8	filler40[0x15];
	u8	state;
	u8	filler56[0xe];
	s16 velocityX;
	s16 velocityY;
} SceneParticle;

typedef struct StageEffect
{
	s32 reserved00;
	s32 reserved04;
	s32 x;
	s32 y;
	s32 z;
	u8	reserved14[20];
	s32 verticalMotion;
	u8	reserved2c[41];
	u8	state;
} StageEffect;

typedef struct LinkedStageEffect
{
	s32 reserved00;
	s32 reserved04;
	s32 x;
	s32 y;
	s32 z;
	u8	reserved14[65];
	u8	state;
	u8	reserved56[14];
	s16 linkedEffectSlot;
} LinkedStageEffect;

u8 *ColossoLogRollingStage_GetScriptData(void);
s32 ColossoLogRollingStage_GetMessageData(void);
u8 *ColossoLogRollingStage_GetActorData(void);
u8 *ColossoLogRollingStage_GetEffectData(void);
s32 ColossoLogRollingStage_GetSceneEventState(void);
s32 StageSetup_BuildAndDispatch(void);
extern const s32 ColossoLog_Record30Script[];
extern const s8 ColossoLog_LogPattern[][6];
extern const u16 gColossoEarlySequenceData[];
extern const s32 gColossoSceneEventEffect[];
extern const u8 Colosso_PortraitPaletteOffsets[];
extern const s16 gColossoModeScript2[];
extern const s16 gColossoModeScript3B[];
extern char gColossoHexChars[20];
extern s32 gColossoRandomEffect[];
extern s32 gColossoLinkedEffect[];
extern u32 StagedActor_DirectionSteps[];
extern struct SceneEntrance gColossoLogEntrances[];
extern u32 gColossoLogExits[];
extern struct ScenePlacement gColossoLogPlacements[];
extern s32 gColossoSceneTaskStatus;
extern s32 gColossoSceneTaskState;
extern struct SceneEvent gColossoLogEvents[];
extern s32 ColossoLog_Actor8Action[];
extern s32 ColossoLog_LeaderActionA[];
extern s32 ColossoLog_LeaderActionB[];
extern s32 ColossoLog_LeaderActionC[];
extern s32 gColossoMultiPhaseData[];
extern s32 gColossoParticleKinds[];
extern s16 Colosso_PortraitSlot;
extern s16 gColossoModeScriptDefault[];
extern s16 gColossoModeScript4[];
extern s16 gColossoModeScript3A[];
extern s16 Colosso_MarkerSlot;
extern s32 Colosso_RivalFinishScript[];
extern s32 ColossoLog_PlaceScript[];

#endif
