#ifndef ALCHEMY_FAR_CALL_FIELD_H
#define ALCHEMY_FAR_CALL_FIELD_H

/// @file
/// The far calls into the field bank, as the other banks' code calls them.

#include "types.h"

void WorldMap_LoadGraphicsFar(s32 x, s32 z);
void ActorSystem_InitializeFar(s32 mode);
void Animation_ApplyChildArgumentFar(void *, s32);
s32 MapGrid_GetTerrainHeightAboveFar(s32 x, s32 z, s32 layer);
struct Actor *Actor_CreateFar(s32 kind, s32 x, s32 y, s32 z);
s32 SpritePictureCache_LoadFar(u32 slot, u32 *buffer, s32 number, u32 variant);
void SpriteAnimation_StartFar(void *work, s32 no);
void VramBlock_Reset93Far(void);
void VramBlock_Load93Far(void);
s32 WorldMap_GetBattleEnvironmentFar(void *position, s32 *kind);
void WorldMap_InitializePerspectiveFar(void);
void MapGrid_LoadMapFar(s32 map);
void WorldMap_UpdateCurrentTileBlockFar(void);
void Map_ApplySetFlagEditsFar(void);
void Actor_StartRenderModeScriptFar(void);
void MapAnimation_StartFar();
void WorldMap_LoadAreaGraphicsFar(void);

#endif
