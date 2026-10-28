/// @file
/// Actors, movement, map drawing and map routines.

/// Allocates the 64 sprites and their sprite animations, loads the basic sprite
/// palettes and shadow tiles, and puts the tile uploader in RAM; `mode` picks
/// EWRAM first on the world map.
void ActorSystem_ConfigureFar(s32 mode);
/// Places a sprite for the scrolling camera and queues it, with its shadow, in
/// a draw slot by depth (Render_ApplyProjectedPlacement). `position` is in
/// 16.16 pixels; an affine matrix is built when the sprite is mirrored, scaled
/// or rotated.
void Actor_ApplyProjectedPlacementFar(struct Sprite *sprite,
                                      const struct SpritePlacement *position,
                                      const struct SpriteScale *scale,
                                      s32 facing);
/// Starts animation `flags & 0x7f` on every sprite animation of a sprite unless
/// it plays it already; bit 7 keeps each sprite animation's place and timer.
/// Returns 0.
s32 Sprite_StartAnimationFar(struct Sprite *state, s32 flags);
/// Creates a sprite for animation `id` with a VRAM cache entry for its tiles,
/// or returns NULL when there's no room or the id has no image. The caller
/// keeps it until Sprite_DestroyFar.
struct Sprite *Sprite_CreateFar(s32 id);
/// Gives back a sprite's VRAM cache entry and its four sprite animations, then
/// zeroes it so it's free again.
void Sprite_DestroyFar(struct Sprite *sprite);
/// Adds a sprite animation playing animation `id` to a sprite, the first free
/// one of four. Returns the sprite animation, 0 when none is free, or -1 when
/// all four are taken.
s32 Sprite_AddAnimationFar(struct Sprite *state, s32 id);
/// Takes sprite animation `handle` off a sprite and clears it; the battle's
/// objects use it to drop an extra one.
void Sprite_RemoveAnimationFar(struct Sprite *state, s32 handle);
/// Zeroes one sprite animation, which frees it; NULL is ignored.
void SpriteAnimation_ClearFar(struct SpriteAnimation *layer);
/// Starts animation `no` on one sprite animation when its metadata has it. Bit
/// 7 keeps the command cursor and timer, and unlike the sprite-wide form it
/// stays part of the index.
void SpriteAnimation_StartFar(struct SpriteAnimation *work, s32 no);
/// Allocates the 64 actors and registers their per-frame updates;
/// `mode` picks the battle's script-less motion and the world map's fixed
/// camera.
void ActorSystem_InitializeFar(s32 mode);
/// Switches every sprite of an actor, one or a group of four, to animation
/// `no`.
void Actor_SelectAnimationFar(void *raw, s32 no);
/// Sets the timer step of every sprite of an actor (16 is normal speed).
void Actor_SetAnimationSpeedFar(struct Actor *actor, s32 val);
/// Gives an actor `script` and runs it from the top, clearing the cursor, the
/// pause, the loop count and the last flag test.
void Actor_StartScriptFar(struct Actor *actor, const s32 *script);
/// Hands an actor to the player's control on a layered map (SC_PLAYER).
void Actor_StartPlayerControlFar(struct Actor *actor);
/// Hands an actor to the player's control on the world map (SC_WORLD).
void Actor_StartWorldMapControlFar(struct Actor *actor);
/// Runs the script that walks an actor wherever the keys point (SC_KEYS),
/// which the leader gets when gSequenceParentWork.movementMode is 1.
void Actor_StartMoveByKeysFar(struct Actor *actor);
/// Runs SC_KEY_COUNT on an actor every frame, which the leader gets when
/// gSequenceParentWork.movementMode is 2.
void Actor_StartKeyCheckFar(struct Actor *actor);
#ifndef OWN_Actor_CreateFar
/// Creates an actor at (x, y, z) in 16.16 pixels, or returns 0 when every
/// slot is in use (Actor_Create). `id / 4096` is its enum ActorCreateKind and
/// the low 12 bits its sprite's animation id.
struct Actor *Actor_CreateFar(s32 id, s32 x, s32 y, s32 z);
#endif
/// Releases an actor's sprites and zeroes it, so it can be handed out again.
void Actor_ReleaseFar(struct Actor *actor);
/// Puts an actor on the camera script (SC_CAMERA); following a leader, it
/// moves at most 4 pixels a frame.
void Actor_StartCameraFar(struct Actor *actor, s32 arg);
/// Places an actor at (x, y, z) in 16.16 pixels and stops it, clearing its
/// movement target.
void Actor_SetPositionAndResetMotionFar(struct Actor *actor, s32 x, s32 y,
                                        s32 z);
/// Clears one step of a sprite's pixel-by-pixel dissolve in its OBJ VRAM tiles
/// (SpriteDissolve_Step); `pos` is the step, 0 to 127.
void SpriteDissolve_StepFar(u8 *obj, u32 pos);
/// Dissolves one sprite away over 32 frames; the auto battle uses it.
void SpriteDissolve_RunFar(u8 *obj);
/// Dissolves `cnt` sprites together over 32 frames; the battle's staging uses
/// it once a unit is off its lists.
void SpriteDissolve_RunManyFar(u8 **table, s32 cnt);
/// Loads layout `index` (cell pictures, cell behaviors, cells, tileset
/// animations, blend script, treasure spots and graphics) and starts its layers
/// scrolling. Returns 2.
s32 MapGrid_LoadMapFar(s32 index);
// XXX: returns no value; the world map ignores it.
/// Sets up the world map's perspective view: the affine ground plane, its
/// camera and projection, and its per-frame and HBlank updates.
s32 WorldMap_InitializePerspectiveFar(void);
/// Centres the camera on the point the map follows, or on (0, 0) with none,
/// and scrolls the layers there.
void MapGrid_CenterOnFollowedPointFar(void);
/// Loads and draws the 2x2 world map tiles around the player in both layers.
void WorldMap_UpdateCurrentTileBlockFar(void);
/// Loads the world map's palette and tiles for the area at (x, z), taking the
/// snowfield set where the ground there is
/// WORLD_MAP_ENCOUNTER_REGION_SNOWFIELD.
void WorldMap_LoadGraphicsFar(s32 x, s32 z);
/// Leaves an actor standing still on the idle script, a lone SC_END.
void Actor_StartIdleFar(struct Actor *actor);
/// Waits until an actor's script reaches SC_END, giving up after 300 frames
/// (five seconds).
void Actor_WaitForScriptEndFar(void *raw);
#ifndef OWN_Actor_SetMoveTargetFar
/// Sends an actor towards (x, y, z) in 16.16 pixels, pulling the target in so
/// it coasts to a stop there; within a pixel it arrives at once.
void Actor_SetMoveTargetFar(struct Actor *actor, s32 x, s32 y, s32 z);
#endif
/// Waits until an actor reaches its target, giving up after 600 frames
/// (ten seconds) with the actor still moving.
void Actor_WaitForMoveFar(void *actor);
/// Puts the world map's default cells back after the still picture and returns
/// to the perspective view, without reloading the area's palette or tiles.
void WorldMap_LoadDefaultCellsAndUpdateBlockFar(void);
/// Plays a map edit, as doors and other changing scenery are drawn:
/// each entry's cell picture numbers go to the destination, then it waits that
/// entry's delay. Returns only when the edit ends.
void MapGrid_PlayMapEditFar(u16 *command, s32 destination_x, u32 destination_y);
/// Copies the cell picture numbers of a rectangle of cells, keeping the
/// destination's attributes, and redraws what's on screen; the destination
/// comes before the size.
void MapGrid_CopyCellPictureIndicesRectFar(s32 sourceX, s32 sourceY, u32 dst_x,
                                           u32 dst_y, u32 width, u32 height);
/// Restarts tileset animation `no` by clearing its paused byte.
void TilesetAnim_ResumeFar(u32 no);
/// Pauses tileset animation `no` by setting its paused byte.
void TilesetAnim_PauseFar(u32 no);
/// Lets the map's blend script run again after BlendScript_SuspendFar.
void BlendScript_ResumeFar(void);
/// Stops the map's blend script, as the lighthouse scenes and Vale's Babi
/// event do while they set the blend themselves.
void BlendScript_SuspendFar(void);
/// Returns the ground height of `layer` at (x, y) in 16.16 pixels, where y is
/// the map's down axis (an actor's z).
s32 MapGrid_GetTerrainHeightFar(s32 layer, s32 x, s32 y);
/// Returns the cell behavior of the cell of `layer` under (x, y), given in
/// 16.16 pixels.
s32 MapGrid_GetCellBehaviorAtFar(s32 layer, s32 x, s32 y);
/// Copies a rectangle of cells whole, cell picture numbers and attributes, and
/// draws the copied cells that are in view.
void MapGrid_CopyCellPictureCellsRectFar(s32 sourceX, s32 sourceY, s32 width,
                                         s32 height, s32 dst_x, s32 dst_y);
/// Copies only the attributes (flags, cell behavior and shape) of a
/// rectangle of cells. Nothing is redrawn, since attributes don't show.
void MapGrid_CopyCellAttributeRectFar(s32 sourceX, s32 sourceY, s32 width,
                                      s32 height, s32 dst_x, s32 dst_y);
/// Copies only the cell picture numbers of a rectangle of cells, the size
/// before the destination, and redraws the cells in view.
void MapGrid_CopyCellIndexRectFar(s32 sourceX, s32 sourceY, s32 width,
                                  s32 height, s32 dst_x, s32 dst_y);
/// Puts world map tile (x / 16, y / 16) into the tile grid at
/// (px / 16, py / 16), drawing it at once when it's next to the view.
void WorldMap_SetWindowCellTileFar(s32 x, s32 y, s32 px, s32 py);
/// Returns whether an actor can step to `position`, as an enum
/// Collision: a wall there, or ground too high or too low for one step.
s32 MapGrid_CheckCollisionFar(struct Actor *actor,
                              struct FieldPosition *position);
/// Sets the flags of a one-sprite actor's sprite (bit 0 draws its shadow).
void Actor_SetSpriteFlagsFar(struct Actor *actor, u32 flags);
/// Sets the OAM blend mode of a one-sprite actor's sprite; an actor with high
/// kind bits set is left alone.
void Actor_SetBlendModeFar(struct Actor *actor, u32 mode);
/// Changes the screen shake, leaving alone an axis whose argument is negative.
/// Amplitudes are in 16.16 pixels, the decay a 16.16 factor per frame.
void MapGrid_SetScreenShakeFar(s32 shakeX, s32 shakeY, s32 shakeDecay);
/// Waits until the shake dies down below 1/256 pixel, or 300 frames pass, and
/// then stops it.
void MapGrid_WaitForShakeToSettleFar(void);
/// Stops the actor camera updates, brings the palette back and shows the
/// sprites alone over BG0, with BG1 to BG3 hidden.
void ActorSystem_SuspendProjectionAndShowSpritesFar(void);
/// Restarts the actor camera updates and hides BG1 to BG3 and the sprites,
/// undoing ActorSystem_SuspendProjectionAndShowSpritesFar.
void ActorSystem_ResumeProjectionAndHideLayersFar(void);
/// Returns -1 when the overhead layer's cell under `position`, its height
/// taken off, is a wall (CELL_BEHAVIOR_WALL), else 0.
s32 MapGrid_IsWallAtFar(struct WorldPosition *position);
/// Adds a sprite animation playing animation `no` to a one-sprite actor's
/// sprite; a negative `no` does nothing.
void Actor_RegisterSpriteMetadataFar(struct Actor *actor, s32 no);
/// Freezes the world map's perspective view as a still picture on BG1, so a
/// menu or event can use the window and VBlank.
void WorldMap_StartStillPictureFar(void);
/// Reloads the world map area's palette and tiles after the still picture and
/// returns to the perspective view.
void WorldMap_LoadAreaGraphicsFar(void);
/// Sets the colour mode (SpriteColorMode) of a one-sprite actor's sprite
/// sprite animations, except hidden ones.
void Actor_SetSpriteColorModeFar(struct Actor *actor, u32 val);
/// Sets every sprite animation of a sprite to colour mode `value`, skipping
/// those drawn hidden (SPRITE_COLOR_HIDDEN), and marks the sprite for
/// redrawing.
void Sprite_SetAnimationColorModesFar(void *raw_parent, u32 value);
/// Returns -1 when an actor at `values` would overlap a solid actor
/// (ACTOR_SOLID), each radius less 2 pixels, else 0.
s32 ActorScript_CheckOverlapFar(struct Actor *actor, s32 *values);
/// Sets which of BG1 to BG3 the map shows, in bits 9 to 11 of its display
/// flags as REG_DISPCNT lays them out.
void MapGrid_SetShownLayersFar(u32 v);
/// Returns how many frames the first `cnt` frames of animation `no` of `id`
/// last, stopping at its first play, hold, jump or end command.
s32 SpriteAnimation_SumCommandLengthsFar(s32 id, u32 no, s32 cnt);
/// Returns the world map's battle environment at `position` and stores the
/// block's terrain kind in `terrain_kind`, for the world map's encounters.
s32 WorldMap_GetBattleEnvironmentFar(struct WorldPosition *position,
                                     s32 *terrain_kind);
/// Decodes the pictures of animation `number` into sprite picture cache slot
/// `slot`'s `buffer`, remapping its colours for `variant` 1 to 5.
s32 SpritePictureCache_LoadFar(u32 slot, u32 *buffer, s32 number, u32 variant);
/// Writes `behavior` into the cell of `layer` under (x, y), given in 16.16
/// pixels; before the map is set up it does nothing.
void MapGrid_SetCellBehaviorAtFar(u32 layer, s32 x, s32 y, u32 behavior);
/// Sets the timer step of a sprite's animated sprite animations: 16 plays at
/// normal speed, 0 stops them.
void Sprite_SetAnimationSpeedFar(struct Sprite *group, s32 val);
/// Remaps `cnt` bytes of `buf` in place through gSpriteColorRemapTables, as
/// the command icons' colours are.
void SpriteColor_RemapPixelsFar(u8 *buf, s32 cnt);
/// Pauses the tileset animations while a menu is open, unless the world map's
/// still picture is shown.
void TilesetAnim_SuspendAllFar(void);
/// Restarts the tileset animations when a menu closes, unless the world map's
/// still picture is shown.
void TilesetAnim_ResumeAllFar(void);
/// Returns the first solid actor an actor at `values` would overlap, as
/// ActorScript_CheckOverlapFar tests, or NULL; Move's field Psynergy uses it to
/// see what stands in the way of the stone pillar it pushes.
struct Actor *ActorScript_FindOverlappingEntryFar(struct Actor *actor,
                                                  s32 *values);
/// Returns the terrain kind (the low four bits of the first collision byte) of
/// layer `index`'s cell under (x, y), given in 16.16 pixels.
s32 MapGrid_GetCellAttributeLowNibbleFar(s32 index, s32 x, s32 y);
