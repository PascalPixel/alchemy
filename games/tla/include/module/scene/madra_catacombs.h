#ifndef ALCHEMY_MODULE_SCENE_MADRA_CATACOMBS_H
#define ALCHEMY_MODULE_SCENE_MADRA_CATACOMBS_H

/// @file
/// What The Lost Age's code declares for the files that use it, written once.

#include "types.h"
#include "scene_tables.h"

struct Actor;

s32 Func_020011f4(s32 layer, s32 shape, s32 height);

extern struct SceneEntrance gMadraCatacombs_Entrances[];
extern struct SceneEntrance gMadraCatacombs_Room4Entrances[];

extern struct SceneEntrance gMadraCatacombs_Room5Entrances[];

extern s16 gMadraCatacombs_LadderRoomRegions[];

extern struct ActorPlacement gMadraCatacombs_Placements[];

extern struct ActorPlacement gMadraCatacombs_Room1Placements[];

extern struct ActorPlacement gMadraCatacombs_Room3Placements[];

extern struct ActorPlacement gMadraCatacombs_Room5Placements[];
extern struct ActorPlacement gMadraCatacombs_Room6Placements[];
extern struct Trigger gMadraCatacombs_Room1Triggers[];
extern struct Trigger gMadraCatacombs_Room2Triggers[];
extern struct Trigger gMadraCatacombs_Room3Triggers[];
extern struct Trigger gMadraCatacombs_Room4Triggers[];
extern struct Trigger gMadraCatacombs_Room5Triggers[];
extern struct Trigger gMadraCatacombs_Room6Triggers[];

void MadraCatacombs_SetUpVillage(void);

void MadraCatacombs_SetUpChampaTunnel(void);

void MadraCatacombs_SetUpBookshelfRoom(void);

void MadraCatacombs_SetUpTabletRoom(void);

extern s16 gMadraCatacombs_PushedPillar[];
extern s16 gMadraCatacombs_Puddles[];

extern u32 gMadraCatacombs_FlameFlaresUp[];

extern s32 gMadraCatacombs_GreatFlameFlaresUp[];
extern s16 gMadraCatacombs_ClimbablePillar[];
extern s32 gMadraCatacombs_WalkIntoDoorway[];

void Func_0200115c(s32 id);

s32 Field_FindNearbyTalkTarget(s32 owner);

s32 Func_020011fc(void);

void MadraCatacombs_RockChestWithBookshelf(void);

void MadraCatacombs_LeaveBlockedVillage(void);

void Func_020011bc(s32 a, s32 b, s32 c, s32 d);

#endif
