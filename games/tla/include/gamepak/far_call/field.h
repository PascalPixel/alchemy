#ifndef ALCHEMY_FAR_CALL_FIELD_H
#define ALCHEMY_FAR_CALL_FIELD_H

/// @file
/// The far calls into the field bank, as the other banks' code calls them.

#include "fixed_math.h"
#include "types.h"

/// Loads the world map's palette and tiles for the area at (x, z).
void WorldMap_LoadGraphicsFar(s32 x, s32 z);
/// Sets up the actor system: its 64 actors, its work and its per-frame updates
/// (mode is an enum ActorSystemMode).
void ActorSystem_InitializeFar(s32 mode);
/// Starts an animation on a sprite, unless the sprite plays it already.
s32 Sprite_StartAnimationFar(void *sprite, s32 animation);
/// The height of the ground at (x, z) (16.16 pixels) on the given layer, less
/// base.
FIXED MapGrid_GetTerrainHeightAboveFar(FIXED x, FIXED z, s32 layer, FIXED base);
/// Creates an actor at (x, y, z), in 16.16 pixels, or returns 0 when every slot
/// is in use; encodedSprite is the kind of actor and its sprite.
struct Actor *Actor_CreateFar(s32 encodedSprite, FIXED x, FIXED y, FIXED z);
/// Loads the pictures of animation id number into picture cache slot's buffer,
/// their colours remapped for variant.
s32 SpritePictureCache_LoadFar(u32 slot, u32 *buffer, s32 number, u32 variant);
/// Starts animation on a sprite animation when its set has it.
void SpriteAnimation_StartFar(void *layer, s32 animation);
/// Frees the actors' shadow tiles.
void ActorShadow_FreeTilesFar(void);
/// Loads the actors' shadow tiles.
void ActorShadow_LoadTilesFar(void);
/// Returns the world map's battle environment at position, and its encounter
/// region in encounterRegion.
s32 WorldMap_GetBattleEnvironmentFar(void *position, s32 *encounterRegion);
/// Sets up the world map's perspective view.
void WorldMap_InitializePerspectiveFar(void);
/// Loads map index from gMaps: its pictures, collision, cells and scripts, and
/// starts its layers scrolling.
void MapGrid_LoadMapFar(s32 index);
/// Loads and draws the 2x2 world map tiles around the player in both layers.
void WorldMap_UpdateCurrentTileBlockFar(void);
/// Plays every edit of the map whose flag is set.
void Map_ApplySetFlagEditsFar(void);
/// Gives the leader the control script of the party's movement mode
/// (gMainRoutineWork.movementMode). Declared without the actor the routine
/// starts the script on: its callers call it just after finding the leader.
void Actor_StartRenderModeScriptFar(void);
/// Freezes the world map's perspective view as a still picture on BG1.
void WorldMap_StartStillPictureFar(void);
/// Reloads the world map area's palette and tiles after the still picture, puts
/// the default cells back and returns to the perspective view.
void WorldMap_LoadAreaGraphicsFar(void);

#endif
