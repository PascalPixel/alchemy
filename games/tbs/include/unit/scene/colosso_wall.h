#ifndef ALCHEMY_UNIT_SCENE_COLOSSO_WALL_H
#define ALCHEMY_UNIT_SCENE_COLOSSO_WALL_H

#include "types.h"
#include "field_scene.h"

struct SceneEntrance;
struct SceneEvent;
struct ScenePlacement;
struct SceneRegion;

typedef struct SparkSource
{
	s32 f00;
	s32 f04;
	s32 f08;
	s32 f0c;
	s32 f10;
	u8	f14[20];
	s32 f28;
	u8	f2c[41];
	u8	f55;
} SparkSource;

typedef struct RaisedEffect
{
	s32 f00;
	s32 f04;
	s32 f08;
	s32 f0c;
	s32 f10;
	u8	f14[65];
	u8	f55;
	u8	f56[14];
	s16 f64;
} RaisedEffect;

const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneRegion *Scene_GetRegions(void);
const u32 *Scene_GetExits(void);
const struct ScenePlacement *Scene_GetPlacements(void);
const struct SceneEvent *Scene_GetEvents(void);
s32 KorosseoKabe_ApplyEntryState(void);
/* FieldScene_BuildDescriptorAndInstallTask is called before its definition with no prototype in scope. */
extern const u8 KorosseoKabe_RollLogScript[];
extern const u8 Korosseo_PortraitPaletteOffsets[];
extern const s16 KorosseoKabe_ModeRecordTwo[];
extern const s16 KorosseoKabe_ModeRecordThreeAlt[];
extern char HexDigits[20];
extern s32 KorosseoKabe_SparkScript[];
extern s32 KorosseoKabe_RaiseScript[];
extern u32 KorosseoKabe_DirectionSteps[];
extern struct SceneEntrance gKorosseoKabeEntrances[];
extern u32 gKorosseoKabeExits[];
extern struct ScenePlacement gKorosseoKabePlacements[];
extern s32 KorosseoKabe_SpectatorPhase;
extern s32 KorosseoKabe_SpectatorTimer;
extern struct SceneEvent gKorosseoKabeEvents[];
extern s16 Korosseo_PortraitSlot;
extern s16 KorosseoKabe_ModeRecordDefault[];
extern s16 KorosseoKabe_ModeRecordFour[];
extern s16 KorosseoKabe_ModeRecordThree[];
extern s16 Korosseo_MarkerSlot;
extern s32 Korosseo_RivalFinishScript[];
extern s32 KorosseoKabe_ApproachScript[];

#endif
