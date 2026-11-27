#ifndef ALCHEMY_FIELD_STUBS_H
#define ALCHEMY_FIELD_STUBS_H

/* @file */
/* The stub calls into the field module, as the other modules' code calls them.
 */

#include "fixed_math.h"
#include "types.h"

/* Loads the world map's palette and tiles for the area at (x, z). */
void scWorldMapLoadGraphicsStub(s32 x, s32 z);
/* Sets up the actor system: its 64 actors, its state and its per-frame updates
 */
/* (mode is an ACTOR_SYSTEM_MODE). */
void fdActorSystemInitStub(s32 mode);
/* Starts an animation on a sprite, unless the sprite plays it already. */
s32 fdSprStartAnimationStub(void *sprite, s32 animation);
/* The height of the ground at (x, z) (16.16 pixels) on the given layer, less */
/* base. */
FIXED fdMapGridGetTerrainHeightAboveStub(FIXED x, FIXED z, s32 layer,
										 FIXED base);
/* Creates an actor at (x, y, z), in 16.16 pixels, or returns 0 when every slot
 */
/* is in use; encodedSprite is the kind of actor and its sprite. */
struct ACTOR *fdActorCreateStub(s32 encodedSprite, FIXED x, FIXED y, FIXED z);
/* Loads the pictures of animation id number into picture cache slot's buffer,
 */
/* their colours remapped for variant. */
s32 fdSprPictureCacheLoadStub(u32 slot, u32 *buffer, s32 number, u32 variant);
/* Starts animation on a sprite animation when its set has it. */
void fdSprAnimationStartStub(void *layer, s32 animation);
/* Frees the actors' shadow tiles. */
void fdActorShadowFreeTilesStub(void);
/* Loads the actors' shadow tiles. */
void fdActorShadowLoadTilesStub(void);
/* Returns the world map's battle environment at position, and its encounter */
/* region in encounterRegion. */
s32 scWorldMapGetBattleEnvironmentStub(void *position, s32 *encounterRegion);
/* Sets up the world map's perspective view. */
void scWorldMapInitPerspectiveStub(void);
/* Loads map index from MapsWork: its pictures, collision, cells and scripts, */
/* and starts its layers scrolling. */
void fdMapGridLoadMapStub(s32 index);
/* Loads and draws the 2x2 world map tiles around the player in both layers. */
void scWorldMapUpdateCurrentTileBlockStub(void);
/* Plays every edit of the map whose flag is set. */
void fdMapApplySetFlagEditsStub(void);
/* Gives the leader the control script of the party's movement mode */
/* (MainRoutineStateWork.movementMode). */
void fdActorStartMovementModeScriptStub(struct ACTOR *actor);
/* Freezes the world map's perspective view as a still image on BG1. */
void scWorldMapStartStillImageStub(void);
/* Reloads the world map area's palette and tiles after the still image, puts */
/* the default cells back and returns to the perspective view. */
void scWorldMapLoadAreaGraphicsStub(void);

#endif
