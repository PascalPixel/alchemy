#ifndef ALCHEMY_FIELD_FUNCTIONS_H
#define ALCHEMY_FIELD_FUNCTIONS_H

/* The field module's functions, declared once each in the order of its stub
   table, without the inlines field.h brings in with its other headers. It is
   for the units that must not see those inlines: the field effects'
   field_effect_advance_motion_priority.c and field_effect_advance_spin.c and
   the scenes' shared scene_set_arrival_flags.c, which have it through script.h,
   and field_effect_spawn.c. field.h includes it for everyone else. */

#include "types.h"
#include "fixed_math.h"

struct ACTOR;
struct ANIMATION_METADATA;
struct POS;
struct RESOURCE_OBJECT;
struct SPR;
struct SPR_ANIMATION;
struct WORLD_POS;

/* The field module's functions in the order of its stub table
   (src/field/stubs.s). Returns the animation metadata of the sprite id in id's
   low twelve bits; an id past the last sprite reads the first's. */
struct ANIMATION_METADATA *fdResourceGetMetadataRecordStub(Uint32 id);
/* Takes the sprite and sprite animation pools (from EWRAM first on the World
   Map, ACTOR_SYSTEM_WORLD_MAP, and IWRAM first otherwise) and clears them,
   loads the shadow tiles into VRAM cache entry VRAM_CACHE_ENTRY_ACTOR_SHADOW
   and copies the strided tile copier into RAM. */
void fdActorSystemConfigure(Sint32 mode);
/* Places a sprite for the scrolling camera and queues it for drawing: the
   sprite at the screen point of a projected position (x, height, y, ground),
   scaled by scale[0] and scale[1] and turned by its rotation, and its shadow on
   the ground below; both go into the depth list by their screen row. */
void fdRenderApplyProjectedPlacement(
	struct SPR *sprite, Sint32 *pos, Sint32 *scale, Sint32 mode);
/* fdRenderApplyProjectedPlacement into the given draw slot: the sprite and its
   shadow both go to depth, and mode chooses the picture as facing does there.
   It also moves a double-size sprite's shadow by twice its half size, and its
   tick and locals differ, so the two stay separate functions. */
void fdRenderApplyProjectedPlacementAtDepth(
	struct SPR *sprite, Sint32 *pos, Sint32 *scale, Sint32 mode, Sint32 depth);
/* Places a sprite at a 3D point for the fixed camera and queues it for drawing:
   projected to the screen and scaled by its distance (unless useOwnScale keeps
   its own scale), with an affine matrix as needed and its shadow on the ground
   below. depth 0 sorts it by distance; any other value is its draw slot. */
void fdRenderPlaceProjectedSprite(struct SPR *sprite, Sint32 *point,
	Sint32 *scale, Sint32 facing, Sint32 depth);
/* Resets each sprite animation of a sprite from its animation's metadata: its
   facing kind, pictures, scripts and codec, with no picture shown and no script
   running. */
Sint32 fdSpriteResetAnimations(struct SPR *sprite);
/* Starting, timing, resetting and adding a sprite's animations and setting
   their colour modes; a timer step of 16 is normal speed. */
Sint32 fdSpriteStartAnimation(struct SPR *sprite, Sint32 animation);
/* Sets each playing sprite animation's timer to the given duration in ticks.
   Returns 0. */
Sint32 fdSpriteSetAnimationTimers(struct SPR *sprite, Sint32 ticks);
/* Creates a sprite for animation id, taking one of the 64 free sprites and a
   VRAM cache entry for its tiles. */
struct RESOURCE_OBJECT *fdSpriteCreate(Sint32 id);
/* Releases a sprite: gives back its VRAM cache entry unless it keeps it
   (retainCacheEntry), clears its four sprite animations and zeroes it. */
void fdSpriteDestroy(struct SPR *sprite);
/* Positions a sprite's image (part 0) and shadow (part 1) on the screen: the
   image centred on screenX and raised height above screenY, the shadow at
   groundHeight. scale is the sprite's (x, y) scale (16.16); above 1.0 both
   parts use the double-size affine mode. */
void fdRenderPlaceSpritePartPair(struct SPR *sprite, FIXED screenX,
	Sint32 height, Sint32 screenY, Sint32 groundHeight, FIXED *scale);
/* Adds a sprite animation of animation id to a sprite, in the first of its four
   slots that is free; returns it, 0 when none could be taken, or -1 when all
   four are in use. */
Sint32 fdSpriteAddAnimation(struct SPR *sprite, Sint32 id);
/* Removes the sprite animation handle from a sprite and clears it, trimming the
   sprite's animation count when no later sprite animation remains. */
void fdSpriteRemoveAnimation(struct SPR *sprite, Sint32 spriteAnimation);
/* Removes and clears the sprite animation in a sprite's slot index (0 to 3),
   trimming the sprite's animation count when no later sprite animation
   remains. */
void fdSpriteRemoveAnimationAt(struct SPR *sprite, Uint32 index);
/* Zeroes one sprite animation (SPR_ANIMATION); does nothing for NULL. Zeroing
   kind frees the sprite animation for fdSpriteAnimationAllocate. */
void fdSpriteAnimationClear(void *record);
/* Resets a sprite animation from its metadata: its pictures, scripts, codec and
   drawing kind, with no script running and no picture shown. */
void fdSpriteAnimationReset(struct SPR_ANIMATION *spriteAnimation);
/* Starts the indexed animation on a sprite animation when its metadata contains
   it. Unlike fdSpriteStartAnimation it does not mask
   SPRITE_ANIMATION_KEEP_PLACE off, so with it set the index would pass
   animationCount only in a set of more than 128 animations. */
void fdSpriteAnimationStart(
	struct SPR_ANIMATION *spriteAnimation, Sint32 animation);
/* Sets up the actor system: allocates and zeroes the 64 actors and the system's
   state, sets the outline's colours and registers the per-tick updates (mode
   is an ACTOR_SYSTEM_MODE). A battle moves its actors with
   fdActorUpdateAllMotion; the World Map and a battle place their sprites with
   the fixed camera. */
void fdActorSystemInit(Sint32 mode);
/* Starts animation argument on an actor's sprite, or on each of the four
   sprites of a group (fdSpriteStartAnimation). */
void fdActorSelectAnimation(struct ACTOR *actor, Sint32 argument);
/* Sets the timer step, the animation speed, of an actor's sprite, or of each
   sprite of a group (ANIM_STEP_NORMAL is normal speed). */
void fdActorSetAnimationSpeed(struct ACTOR *actor, Sint32 value);
/* Both of the above: switches all of an actor's sprites to animation and sets
   their timer step to step. */
void fdActorSetAnimationAndSpeed(
	struct ACTOR *actor, Sint32 animation, Sint32 step);
/* Starting an actor's script and setting its sprite's flags, colour mode and
   priority. */
void fdActorStartScript(struct ACTOR *actor, const Sint32 *script);
/* Starts the leader's control script for the way the party moves now
   (MainRoutineStateWork.movementMode), waiting a tick first for Sand on the
   World Map. */
void fdActorStartMovementModeScript(struct ACTOR *actor);
/* Returns an unused actor (one without a script) from the 64 slots, or 0 if
   none is free. */
void *fdActorFindFree(void);
/* Creates an actor at (x, y, z), in 16.16 pixels, or returns 0 when every slot
   is in use. encodedSprite / 4096 is the kind of actor (enum ACTOR_CREATE) and
   its low 12 bits the sprite's animation id. */
struct ACTOR *fdActorCreate(Sint32 encodedSprite, FIXED x, FIXED y, FIXED z);
/* Releases an actor's sprites and zeroes it, releasing its slot. Clearing the
   script lets fdActorFindFree hand the actor out again. */
void fdActorRelease(struct ACTOR *actor);
/* Sets the rotation of an actor's sprite, when it has just one. */
void fdActorSetSpriteRotation(struct ACTOR *actor, Uint32 value);
/* Starts an actor on the camera script (SC_CAMERA). With a leader to follow, it
   also gets a speed limit of 4 pixels a tick and an acceleration of 0.5. */
void fdActorStartCamera(struct ACTOR *actor, Sint32 watchedActorHandle);
/* Starts an actor on the script that approaches its watched actor
   (SC_APPROACH_WATCHED), watching watchedActorHandle. */
void fdActorStartApproachWatched(
	struct ACTOR *actor, Sint32 watchedActorHandle);
/* Places the actor at (x, y, z), in 16.16 pixels, clears its movement target
   and stops it. */
void fdActorSetPosAndResetMotion(
	struct ACTOR *actor, FIXED x, FIXED y, FIXED z);
/* One step of dissolving a sprite pixel by pixel. */
void fdSpriteDissolveStep(Uint8 *object, Uint32 pos);
/* Dissolves a sprite pixel by pixel, four steps a tick, over 32 ticks. */
void fdSpriteDissolveRun(Uint8 *sprite);
/* Dissolves count sprites together, pixel by pixel, over 32 ticks. */
void fdSpriteDissolveRunMany(Uint8 **sprites, Sint32 count);
/* Loads layout index from MapsWork: its metatile tiles, collision (cleared to
   -1 first), metatiles, shapes, tileset animations, blend script and script;
   sets up the three background layers' origins, parallax and wrap from the
   layout header; loads the palette and tile graphics unless
   FLAG_KEEP_LAYER_GRAPHICS says they are loaded; and starts the layer scrolling
   (fdMapGridUpdateLayerScroll). Returns 2, as The Broken Seal's does; no caller
   reads it. */
Sint32 fdMapGridLoadMap(Sint32 index);
/* Sets up the world map's perspective view: BG2 and BG3 in affine mode, the
   camera state, the projection, the per-scanline ground plane function in RAM
   (fdWorldMapUpdateAffineLines) and its first tick, and the per-tick view and
   HBlank updates. */
UNREAD_RESULT fdWorldMapInitPerspective(void);
/* Centres the view on (x, y) in 16.16 pixels, clamped to the map's bounds, and
   redraws every enabled layer's BG map around it. */
void fdMapGridSetCameraCenter(FIXED x, FIXED y);
/* Centres the camera on the point the map follows (origin: x, height, z), or on
   (0, 0) when there is none, and scrolls the layers there. */
void fdMapGridCenterOnFollowedPoint(void);
/* Loads and draws the 2x2 world map tiles around the player in both layers. */
void fdWorldMapUpdateCurrentTileBlock(void);
/* Loads the world map's palette and tiles for the area at (x, z): the set its
   region picks, its tint, and the plain BG maps the world map draws over. */
void fdWorldMapLoadGraphics(Sint32 x, Sint32 z);
/* Starts an actor on the idle script (a lone SC_END). A unit whose ROM code is
   compiled against another form defines OWN_fdActorStartIdle and declares that
   form itself. */
#ifndef OWN_fdActorStartIdle
void fdActorStartIdle(struct ACTOR *actor);
#endif
/* Waits until the actor's script reaches SC_END, at most ACTOR_WAIT_TICKS_MAX
   ticks; it returns as soon as the script idles, and after the limit whether
   or not it has. */
void fdActorWaitForScriptEnd(struct ACTOR *actor);
/* Sends an actor towards (x, y, z), in 16.16 pixels. Within a pixel it arrives
   at once. */
void fdActorSetMoveTarget(struct ACTOR *actor, FIXED x, FIXED y, FIXED z);
/* Waits, at most ACTOR_WAIT_TICKS_MAX ticks, for an actor to reach where it
   is going. */
void fdActorWaitForMove(struct ACTOR *actor);
/* Writes the actor's screen position, in pixels, to screenPos[0..1] and
   returns 0 when it is on or near the screen; otherwise writes 0, 0 and returns
   -1. */
Sint32 fdMapGridGetScreenRelativePos(struct ACTOR *actor, Sint32 *screenPos);
/* Each tick: follows the followed point with the screen shake, draws the rows
   and columns that came into view, and recomputes the projection and the ground
   plane for the current pitch and yaw; while the still image shows, only BG1
   shakes. */
void fdWorldMapUpdateView(void);
/* Puts the default world map metatiles back after the still image and returns
   to the perspective view, without reloading the area's palette or tiles. */
void fdWorldMapLoadDefaultMetatilesAndUpdateBlock(void);
/* Plays a map edit (MAP_GRID_METATILE_ANIMATION_STEP entries, ended by a source
   of MAPGRID_ANIMATION_STEPS_END): copies each entry's rectangle of metatile
   picture numbers to (destX, destY) and waits its delay in
   ticks. */
void fdMapGridPlayMapEdit(Uint16 *steps, Sint32 destX, Uint32 destY);
/* Plays every edit of the map whose flag is set, as fdMapApplyFlagEdits does,
   MAP_EDIT_ATTRIBUTES in an edit's flag word choosing attributes over pictures:
   the map as the story has left it, as the player enters. */
void fdMapApplySetFlagEdits(void);
/* Sets flag (its low 12 bits) and plays its map edits on the metatiles'
   pictures. */
void fdMapSetFlagAndApplyEdits(Sint32 flag);
/* Sets flag (its low 12 bits) and plays its map edits on the metatiles'
   attributes. */
void fdMapSetFlagAndApplyAttributeEdits(Sint32 flag);
/* Resumes tileset animation channelIndex. */
void fdTilesetAnimationResume(Uint32 channelIndex);
/* Pauses tileset animation channelIndex. */
void fdTilesetAnimationPause(Uint32 channelIndex);
/* Resumes the blend script, through fdBlendScriptResumeStub. */
void fdBlendScriptResume(void);
/* Suspends the blend script; scenes reach it through
   fdBlendScriptSuspendStub. */
void fdBlendScriptSuspend(void);
/* Starts a map's blend script, unless it is empty. Clears the map state's
   three-word blend state first, then adds fdBlendScriptRun as an event. */
void fdBlendScriptStart(Uint16 *script);
/* Returns the ground height of a layer at (x, z) in 16.16 pixels, as The Broken
   Seal's: the height function of the metatile's terrain kind applied to its
   samples. */
FIXED fdMapGridGetTerrainHeight(Sint32 layer, FIXED x, FIXED z);
/* Returns the terrain kind of a layer's metatile at (x, z) in 16.16 pixels, as
   The Broken Seal's. */
Sint32 fdMapGridGetTerrainKindAt(Sint32 layer, FIXED x, FIXED z);
/* As fdMapGridGetTerrainKindAt, but a rounded square whose two samples differ
   reads as TERRAIN_BUMP. */
Sint32 fdMapGridGetTerrainKindOrBumpAt(Sint32 layer, Sint32 x, Sint32 z);
/* Copies a rectangle of width by height metatiles from (srcX, srcY) to (destX,
   destY), their pictures and their attributes both. */
void fdMapCopyMetatilesRect(Sint32 srcX, Sint32 srcY, Sint32 width,
	Sint32 height, Sint32 destX, Sint32 destY);
/* Copies the attributes and shapes of a rectangle of metatiles from (srcX,
   srcY) to (destX, destY), width by height metatiles, keeping each
   destination's picture. */
void fdMapCopyMetatileAttributeRect(Sint32 srcX, Sint32 srcY, Sint32 width,
	Sint32 height, Sint32 destX, Sint32 destY);
/* Copies the drawn metatile tiles numbers (MAPGRID_DRAWN_PICTURE_MASK) of a
   rectangle of metatiles from (srcX, srcY) to (destX, destY), width by height
   metatiles, keeping each destination's bits above them, and redraws the copied
   metatiles that are on screen. fdMapGridCopyMetatileTilesIndicesRect's loop
   with the narrower mask and the size before the destination: the ROM keeps
   both. */
void fdMapGridCopyMetatileTilesMetatilesRect(Sint32 srcX, Sint32 srcY,
	Sint32 width, Sint32 height, Sint32 destX, Sint32 destY);
/* Copies a rectangle of map metatiles as they are, with their shape grid bytes,
   from (srcX, srcY) to (destX, destY), width by height metatiles. */
void fdMapCopyRawMetatilesRect(Sint32 srcX, Sint32 srcY, Sint32 width,
	Sint32 height, Sint32 destX, Sint32 destY);
/* Puts world map tile (x / 16, y / 16) at metatile (metatileX / 16, metatileY /
   16) of the tile id grid, and draws it at once when it is next to the view.
   metatileX and metatileY count 32-pixel world map blocks, so a grid metatile
   is a 512-pixel tile; it is next to the view when metatileX / 8 and metatileY
   / 8, in 256-pixel steps, are within one step of the followed point's. */
void fdWorldMapSetWindowMetatileTile(
	Sint32 x, Sint32 y, Sint32 metatileX, Sint32 metatileY);
/* fdWorldMapSetWindowMetatileTile without its view test: puts world map tile (x
   / 16, y / 16) at metatile (metatileX / 16, metatileY / 16) of the tile grid
   and draws it in both layers. */
void fdWorldMapSetWindowMetatileTileAndDraw(
	Sint32 x, Sint32 y, Sint32 metatileX, Sint32 metatileY);
/* The collision at an actor's probe point (tile_animation.c): 2 for a metatile
   with no tile, 1 a rise of more than 8 pixels, -1 a drop of more than 12, else
   0. */
Sint32 fdMapGridCheckCollision(struct ACTOR *actor, struct POS *pos);
/* Sets the flags of a one-sprite actor's sprite (SPR_FLAG). */
void fdActorSetSpriteFlags(struct ACTOR *actor, Uint32 value);
/* Sets the OAM blend mode of a one-sprite actor's sprite. */
void fdActorSetBlendMode(struct ACTOR *actor, Uint32 mode);
/* Changes the shake without restarting an axis whose argument is negative.
   Amplitudes are 16.16 pixels and decay is a 16.16 multiplier per tick. */
void fdMapGridSetScreenShake(FIXED shakeX, FIXED shakeY, FIXED shakeDecay);
/* Waits until both shake amplitudes are below 1/256 pixel, with a 300-tick
   limit for a shake whose decay never settles. */
void fdMapGridWaitForShakeToSettle(void);
/* Gives a one-sprite actor's first sprite animation animation id animationId
   and restarts its animations. A negative animationId does nothing. */
void fdActorSetFirstSpriteAnimation(struct ACTOR *actor, Sint32 animationId);
/* Suspends the actor camera updates, starts the palette back to normal and, a
   tick later, shows the sprites with BG1-3 hidden, leaving BG0 unchanged. */
void fdActorSystemSuspendProjectionAndShowSprites(void);
/* Resumes the actor camera updates and hides BG1-3 and the sprites. */
void fdActorSystemResumeProjectionAndHideLayers(void);
/* Sets a one-sprite actor's sprite's own-scale bit: drawn at its own scale, not
   scaled by distance. */
void fdActorSetOwnScale(struct ACTOR *actor, Uint32 value);
/* Returns -1 when the metatile under position (its x, and z less its height) in
   the overhead layer is a wall, else 0; also 0 before the map state exists. */
Sint32 fdMapGridIsWallAt(Sint32 *pos);
/* Adds a sprite animation of animation id animationId to a one-sprite actor's
   sprite. A negative animationId does nothing. */
void fdActorRegisterSpriteMetadata(struct ACTOR *actor, Sint32 animationId);
/* Freezes the world map's perspective view as a still image on BG1: decodes the
   area's tiles again where a field Psynergy used their buffer, saves BG1's
   tiles, draws the current affine frame into pixels and hands the window and
   VBlank to it. */
void fdWorldMapStartStillImage(void);
/* Reloads the world map area's palette and tiles after the still image, puts
   the default metatiles back and returns to the perspective view. */
void fdWorldMapLoadAreaGraphics(void);
/* As field/actor_view.h declares it; that header cannot be included here, since
   its fdActorStartScript takes the actor field_effect_spawn.c passes as a field
   effect. */
void fdActorSetSpriteColorMode(struct ACTOR *actor, Uint32 value);
/* Sets the colour mode of every sprite animation of the sprite that is not
   hidden (SPRITE_COLOR_HIDDEN) and has it drawn again. */
void fdSpriteSetAnimationColorModes(struct SPR *sprite, Uint32 colorMode);
/* Sets the colour mode (SPR_COL_MODE) of the sprite's first sprite animation
   and has it drawn again. */
void fdSpriteSetFirstAnimationColorMode(struct SPR *sprite, Uint32 colorMode);
/* Fixes the priority of a one-sprite actor's sprite and its shadow, which then
   no longer follow the metatiles under it (ACTOR_PRIORITY_AUTOMATIC). */
void fdActorSetSpritePriorityDirect(struct ACTOR *actor, Sint32 priority);
/* Returns -1 when an actor at position values would overlap a solid actor, each
   radius less 2 pixels; otherwise 0. */
Sint32 fdActorScriptCheckOverlap(struct ACTOR *actor, Sint32 *values);
/* Sets which of BG1-BG3 the map shows (bits 9-11 of its display flags, as in
   REG_DISPCNT) from layerFlags, keeping its other flags. */
void fdMapGridSetShownLayers(Uint32 layerFlags);
/* Returns how many ticks the first timedCount timed steps of animation number
   animation of sprite id last. Commands are two bytes (command, value);
   ANIMCMD_PLAY, ANIMCMD_HOLD, ANIMCMD_JUMP and ANIMCMD_END end the count, and
   the values of ANIMCMD_WAIT, ANIMCMD_BLANK and the pictures below ANIMCMD_END
   count. */
Sint32 fdSpriteAnimationSumCommandLengths(
	Sint32 id, Uint32 animation, Sint32 timedCount);
/* Returns the world map's battle environment at position from
   WorldMapTerrainBehaviorTable, by its collision value and its block's rows,
   and stores the block's encounter region in encounterRegion. */
Uint8 fdWorldMapGetBattleEnvironment(
	struct WORLD_POS *pos, Sint32 *encounterRegion);
/* Loads the pictures of animation id number into picture cache slot's buffer:
   the resource listed for it in SpritePictureCacheSources, decoded, its
   picture offsets turned into pointers, and for variant 1-5 its pixel colours
   (those up to 223, the basic sprite palettes 0 to 13) remapped through that
   variant's conversion table; a variant above 5 uses variant 1's. */
Sint32 fdSpritePictureCacheLoad(
	Uint32 slot, Uint32 *buf, Sint32 number, Uint32 variant);
/* Sets the metatile behavior of a layer's metatile at (x, y) in 16.16 pixels.
   Writes the metatile in the map's grid; does nothing before the map is set
   up. */
void fdMapGridSetMetatileBehaviorAt(
	Uint32 layer, FIXED x, FIXED y, Uint32 collisionCode);
/* Sets the timer step, the animation speed, of each playing sprite animation of
   a sprite (16 is normal speed). */
void fdSpriteSetAnimationSpeed(struct SPR *sprite, Sint32 step);
/* Remaps byteCount bytes in place through SpriteColorGreyTable. */
void fdSpriteColorRemapPixels(Uint8 *bytes, Sint32 byteCount);
/* Suspends the tileset animations, unless the world map's still image is
   shown. */
void fdTilesetAnimationSuspendAll(void);
/* Resumes the tileset animations, unless the world map's still image is
   shown. */
void fdTilesetAnimationResumeAll(void);
/* Returns the first solid actor that an actor at position values would overlap,
   as fdActorScriptCheckOverlap tests, or NULL. */
struct ACTOR *fdActorScriptFindOverlappingActor(
	struct ACTOR *actor, Sint32 *values);
/* As fdMapGridGetTerrainKindAt: the ROM keeps two functions with this body,
   each with callers of its own (the scenes' overlays import only this one). */
Sint32 fdMapGridGetTerrainKind(Sint32 layer, Sint32 x, Sint32 z);
/* fdActorIsTargetUnset, for the scenes: 1 once the actor has reached its
   destination. Airs Rock waits on it while the lift carries the leader up. */
Sint32 fdActorHasArrived(struct ACTOR *actor);
/* Shows item id's icon on its owner's sprite, 16 pixels square at scale 1.0 in
   16-colour palette 0. */
void fdActorSetItemIcon(struct ACTOR *owner, Sint32 id);
/* The map's collisions and the shape entries its metatiles take. */
Sint32 fdMapGridCheckCollisionAt(
	FIXED px, FIXED pz, Sint32 layer, Sint32 height);
/* The height of the ground at (px, pz) (16.16 pixels) on the given layer, less
   base; a layer past 2 reads the shape grid. With no map, or on the World Map,
   it is 0. */
FIXED fdMapGridGetTerrainHeightAbove(
	FIXED px, FIXED pz, Sint32 layer, FIXED base);
/* Returns 0 when the world map at position can be walked on (a place there, and
   collision WORLD_MAP_COLLISION_WALKABLE_FIRST to
   WORLD_MAP_COLLISION_WALKABLE_LAST), else -1. */
Sint32 fdWorldMapIsWalkable(void *mover, struct WORLD_POS *pos);
/* Returns 0 where the ship can sail (the sea or shallow water), else -1. */
Sint32 fdWorldMapIsSailable(void *mover, struct WORLD_POS *pos);
/* Reads map's camera bounds from its header, in 8-pixel tiles: bounds[0] its
   left, bounds[1] its right, bounds[2] its top and bounds[3] its bottom, a
   right or bottom of 0 standing for the map's whole MAPGRID_TILES. */
void fdMapGetCameraBounds(Sint32 map, Sint32 *bounds);
/* The sprite id an actor is drawn with (the first halfword of its sprite's
   definition), or 0 for none. */
Sint32 fdActorGetSpriteId(struct ACTOR *actor);
/* Waits, at most TRACK_END_WAIT_TICKS, for a sprite's animation to change or
   for its first layer's script to reach ANIMCMD_HOLD. */
void fdSpriteWaitForTrackEnd(struct SPR *sprite);
/* fdSpriteWaitForTrackEnd on an actor's sprite. */
void fdActorWaitForTrackEnd(struct ACTOR *actor);
/* Returns 0 when two spheres overlap: at firstPos and secondPos
   (16.16 pixels) with radii radiusA and radiusB (pixels); -1 otherwise. */
Sint32 fdCheckSpheresOverlap(
	FIXED *firstPos, Sint32 radiusA, FIXED *secondPos, Sint32 radiusB);
/* The metatile behavior of the world map block under position (blocks 32 pixels
   square on a 32 by 32 grid), which the World Map's step cues answer
   (world_map.c: STEP_CUE_AREA(AREA_DAILA) enters Daila,
   METATILE_BEHAVIOR_WORLD_MAP_IRON meets Iron), or METATILE_BEHAVIOR_WALL where
   the map has no block. */
Sint32 fdWorldMapGetBlockMetatileBehavior(struct WORLD_POS *pos);
/* Returns the world map's collision value (1-15) at position: from the upper
   collision layer where it has one, else the lower, else
   WORLD_MAP_COLLISION_DEFAULT. */
Sint32 fdWorldMapGetCollision(struct WORLD_POS *pos);
/* Gives the metatile at (x, z) of layer the ground height `height`, in 8-pixel
   steps, by tagging its shape with terrain kind 0 at that height. */
void fdMapGridSetMetatileHeight(
	Sint32 x, Sint32 z, Sint32 layer, Sint32 height);
/* The ground height at (x, z) of layer, in 8-pixel steps. */
Sint32 fdMapGridGetMetatileHeight(FIXED x, FIXED z, Sint32 layer);
/* Runs each of the sprite's sprite animations on to its next stop (an end,
   hold, jump or play command), taking the pictures and settings on the way, and
   leaves the stop to run next; marks the sprite and each sprite animation
   changed. The battle's motion calls it before it releases a borrowed sprite
   buffer. */
void fdSpriteSkipToStop(struct SPR *sprite);
/* Decodes the current map's metatiles into the map metatile buffer and converts
   them, then decodes its metatile shapes into MapShapeGrid. */
void fdMapLoadMetatilesAndShapes(void);
/* Frees the actors' shadow tiles (VRAM_CACHE_ENTRY_ACTOR_SHADOW). */
void fdActorShadowFreeTiles(void);
/* Loads the actors' shadow tiles into VRAM_CACHE_ENTRY_ACTOR_SHADOW. */
void fdActorShadowLoadTiles(void);
/* Decodes the palette resource of a map's header row (its second halfword) and
   copies its 224 colors to destination, keeping the current backdrop color in
   the first entry. */
void fdMapCopyPalette(Sint32 map, void *dest);
/* The debug sprite viewer, in The Broken Seal's form with this game's changes;
   it never returns. */
void fdSpriteViewerRun(void);
/* A splash at the feet of an actor wading through water (MAP_METATILE_WATER):
   SPRITE_WADING_SPLASH started on its splash script, its sound every other
   step; mode is the map's kind of wading (GROUND_EFFECTS_WADING). In Taopo
   Swamp's mud the splash is drawn in the mud's colour, at half speed. */
void fdActorSpawnWaterSplash(struct ACTOR *actor, Sint32 mode);

#endif
