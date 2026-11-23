#ifndef ALCHEMY_FAR_CALL_FIELD_H
#define ALCHEMY_FAR_CALL_FIELD_H

/// @file
/// The far calls into the field bank, as the other banks' code calls them.

#include "fixed_math.h"
#include "types.h"

/// Loads the world map's palette and tiles for the area at (x, z).
void WorldMap_LoadGraphicsFar(s32 x, s32 z);
/// Sets up the map animation work for map mode.
void ActorSystem_InitializeFar(s32 mode);
s32 Sprite_StartAnimationFar(void *, s32);
/// The height of the ground at (px, pz) (16.16 pixels) on the given layer, less
/// base.
FIXED MapGrid_GetTerrainHeightAboveFar(FIXED x, FIXED z, s32 layer, FIXED base);
struct Actor *Actor_CreateFar(s32 kind, s32 x, s32 y, s32 z);
s32 SpritePictureCache_LoadFar(u32 slot, u32 *buffer, s32 number, u32 variant);
void SpriteAnimation_StartFar(void *work, s32 no);
void ActorShadow_FreeTilesFar(void);
void ActorShadow_LoadTilesFar(void);
s32 WorldMap_GetBattleEnvironmentFar(void *position, s32 *kind);
/// Sets up the world map's perspective view.
void WorldMap_InitializePerspectiveFar(void);
void MapGrid_LoadMapFar(s32 map);
/// Loads and draws the 2x2 world map tiles around the player in both layers.
void WorldMap_UpdateCurrentTileBlockFar(void);
void Map_ApplySetFlagEditsFar(void);
/// Starts the dispatch script for gMainRoutineWork's render mode, waiting a
/// frame first in mode 9.
void Actor_StartRenderModeScriptFar(void);
/// Freezes the world map's perspective view as a still image on BG1.
void WorldMap_StartStillImageFar();
/// Reloads the world map area's palette and tiles after the still image, puts
/// the default cells back and returns to the perspective view.
void WorldMap_LoadAreaGraphicsFar(void);

#endif
