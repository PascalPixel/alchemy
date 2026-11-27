#ifndef ALCHEMY_FIELD_STUBS_H
#define ALCHEMY_FIELD_STUBS_H

/* The stub calls into the field module, as the other modules' code calls
   them. */

#include "fixed_math.h"
#include "types.h"

/* Loads the world map's palette and tiles for the area at (x, z). */
void fdWorldMapLoadGraphicsStub(Sint32 x, Sint32 z);
/* Sets up the actor system: its 64 actors, its state and its per-tick updates
   (mode is an ACTOR_SYSTEM_MODE). */
void fdActorSystemInitStub(Sint32 mode);
/* Starts an animation on a sprite, unless the sprite plays it already. */
Sint32 fdSpriteStartAnimationStub(void *sprite, Sint32 animation);
/* The height of the ground at (x, z) (16.16 pixels) on the given layer, less
   base. */
FIXED fdMapGridGetTerrainHeightAboveStub(
	FIXED x, FIXED z, Sint32 layer, FIXED base);
/* Creates an actor at (x, y, z), in 16.16 pixels, or returns 0 when every slot
   is in use; encodedSprite is the kind of actor and its sprite. */
struct ACTOR *fdActorCreateStub(
	Sint32 encodedSprite, FIXED x, FIXED y, FIXED z);
/* Loads the pictures of animation id number into picture cache slot's buffer,
   their colours remapped for variant. */
Sint32 fdSpritePictureCacheLoadStub(
	Uint32 slot, Uint32 *buf, Sint32 number, Uint32 variant);
/* Starts animation on a sprite animation when its set has it. */
void fdSpriteAnimationStartStub(void *layer, Sint32 animation);
/* Frees the actors' shadow tiles. */
void fdActorShadowFreeTilesStub(void);
/* Loads the actors' shadow tiles. */
void fdActorShadowLoadTilesStub(void);
/* Returns the world map's battle environment at position, and its encounter
   region in encounterRegion. */
Sint32 fdWorldMapGetBattleEnvironmentStub(void *pos, Sint32 *encounterRegion);
/* Sets up the world map's perspective view. */
void fdWorldMapInitPerspectiveStub(void);
/* Loads map index from MapsWork: its pictures, collision, metatiles and
   scripts, and starts its layers scrolling. */
void fdMapGridLoadMapStub(Sint32 index);
/* Loads and draws the 2x2 world map tiles around the player in both layers. */
void fdWorldMapUpdateCurrentTileBlockStub(void);
/* Plays every edit of the map whose flag is set. */
void fdMapApplySetFlagEditsStub(void);
/* Gives the leader the control script of the party's movement mode
   (MainRoutineStateWork.movementMode). */
void fdActorStartMovementModeScriptStub(struct ACTOR *actor);
/* Freezes the world map's perspective view as a still image on BG1. */
void fdWorldMapStartStillImageStub(void);
/* Reloads the world map area's palette and tiles after the still image, puts
   the default metatiles back and returns to the perspective view. */
void fdWorldMapLoadAreaGraphicsStub(void);

#endif
