#ifndef ALCHEMY_UNIT_SCENE_COLOSSO_WALL_H
#define ALCHEMY_UNIT_SCENE_COLOSSO_WALL_H

#include "types.h"
#include "field_scene.h"

struct SceneEntrance;
struct SceneEvent;
struct ScenePlacement;
struct SceneRegion;

typedef struct SparkSource {
  s32 f00;
  s32 f04;
  s32 f08;
  s32 f0c;
  s32 f10;
  u8 f14[20];
  s32 f28;
  u8 f2c[41];
  u8 f55;
} SparkSource;

typedef struct RaisedEffect {
  s32 f00;
  s32 f04;
  s32 f08;
  s32 f0c;
  s32 f10;
  u8 f14[65];
  u8 f55;
  u8 f56[14];
  s16 f64;
} RaisedEffect;

const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneRegion *Scene_GetRegions(void);
const u32 *Scene_GetExits(void);
const struct ScenePlacement *Scene_GetPlacements(void);
const struct SceneEvent *Scene_GetEvents(void);
s32 ColossoWall_Initialize(void);
// FieldScene_BuildDescriptorAndInstallTask is called before its definition with
// no prototype in scope.
extern const u8 gColossoWall_RollLogScript[];
extern const u8 gColosso_LetteringPaletteOffsets[];
extern const s16 gColossoWall_ModeRecordTwo[];
extern const s16 gColossoWall_ModeRecordThreeAlt[];
extern char gHexDigits[20];
extern s32 gColossoWall_SparkScript[];
extern s32 gColossoWall_RaiseScript[];
extern u32 gColossoWall_DirectionSteps[];
extern struct SceneEntrance gColossoWallEntrances[];
extern u32 gColossoWallExits[];
extern struct ScenePlacement gColossoWallPlacements[];
extern s32 gColossoWall_SpectatorPhase;
extern s32 gColossoWall_SpectatorTimer;
extern struct SceneEvent gColossoWallEvents[];
extern s16 gColosso_LetteringSlot;
extern s16 gColossoWall_ModeRecordDefault[];
extern s16 gColossoWall_ModeRecordFour[];
extern s16 gColossoWall_ModeRecordThree[];
extern s16 gColosso_MarkerSlot;
extern s32 gColosso_RivalFinishScript[];
extern s32 gColossoWall_ApproachScript[];

#endif
