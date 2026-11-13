#ifndef ALCHEMY_MODULE_SCENE_LOHO_H
#define ALCHEMY_MODULE_SCENE_LOHO_H

/// @file
/// What games/tla/src/scene/ declares for the files that use it, written once.

#include "types.h"
#include "module/scene/scene.h"
#include "scene_tables.h"

extern u32 gLoho_VillagerMovesNorth[];
extern u32 gLoho_VillagerMovesWest[];

extern u32 gLoho_VillagerReturns[];

void Loho_ClimbDownLadder(void);
extern u32 gLoho_Regions[];
extern struct ActorPlacement gLoho_Placements[];
extern struct ActorPlacement gLoho_Room2Placements[];
extern u32 gLoho_FirstMinerGoesToWall[];
extern s32 gLoho_SecondMinerStepsUp[];

extern s32 gLoho_ThirdMinerStepsUp[];

extern u32 gLoho_FirstMinerWalksBack[];
extern u32 gLoho_FirstMinerFetchesCannon[];
extern u32 gLoho_FirstMinerReturns[];
extern s32 gLoho_ForemanComes[];
extern struct Trigger gLoho_Triggers[];
extern struct Trigger gLoho_Room1Triggers[];
extern struct Trigger gLoho_Room2Triggers[];
extern struct LohoPicture gLoho_HouseDoor;
extern u32 gLoho_VillagerPaces[];

extern s16 gLoho_DoorOpening[];

#endif
