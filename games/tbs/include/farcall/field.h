/// @file
/// Field object events, movement, map drawing and map services.

/// Allocates the 64 sprites and their animation layers, loads the basic sprite
/// palettes and shadow tiles, and puts the tile uploader in RAM; `mode` picks
/// EWRAM first on the world map.
void ObjectSystem_ConfigureFar(s32 mode);
/// Places a sprite for the scrolling camera and queues it, with its shadow, in
/// a draw slot by depth (Render_ApplyProjectedPlacement). `position` is in
/// 16.16 pixels; an affine matrix is built when the sprite is mirrored, scaled
/// or rotated.
void Object_ApplyProjectedPlacementFar(struct AnimationObject *sprite,
                                       const struct SpritePlacement *position,
                                       const struct SpriteScale *scale,
                                       s32 facing);
/// Starts animation `flags & 0x7f` on every layer of a sprite unless it plays
/// it already; bit 7 keeps each layer's place and timer. Returns 0.
s32 AnimationObject_SelectAnimationFar(struct AnimationObject *state,
                                       s32 flags);
/// Creates a sprite for animation `id` with a VRAM cache entry for its tiles,
/// or returns NULL when there's no room or the id has no image. The caller
/// keeps it until ResourceObject_ReleaseFar.
struct AnimationObject *ResourceObject_CreateFar(s32 id);
/// Gives back a sprite's VRAM cache entry and its four animation layers, then
/// zeroes it so it's free again.
void ResourceObject_ReleaseFar(struct AnimationObject *sprite);
/// Adds a layer playing animation `id` to a sprite, in its first free layer of
/// four. Returns the layer, 0 when no record is free, or -1 when all four are
/// taken.
s32 ResourceMetadata_RegisterFar(struct AnimationObject *state, s32 id);
/// Takes layer `handle` off a sprite and clears it; the battle's objects use it
/// to drop an extra layer.
void ResourceMetadata_UnregisterFar(struct AnimationObject *state, s32 handle);
/// Zeroes one animation layer, which frees its record; NULL is ignored.
void ResourceMetadata_ClearRecordFar(struct AnimationEntry *layer);
/// Starts animation `no` on one layer when the layer's metadata has it. Bit 7
/// keeps the command cursor and timer, and unlike the sprite-wide form it stays
/// part of the index.
void Animation_SetWorkEntryFar(struct AnimationEntry *work, s32 no);
/// Allocates the 64 field objects and registers their per-frame updates;
/// `mode` picks the battle's script-less motion and the world map's fixed
/// camera.
void ObjectSystem_InitializeFar(s32 mode);
/// Switches every sprite of an object, one or a group of four, to animation
/// `no`.
void ObjectDispatch_SelectAnimationFar(void *raw, s32 no);
/// Sets the timer step of every sprite of an object (16 is normal speed).
void ObjectDispatch_SetAnimationSpeedFar(struct DispatchObject *object,
                                         s32 val);
/// Gives an object `script` and runs it from the top, clearing the cursor, the
/// pause, the loop count and the last flag test.
void ObjectDispatch_InitializeFar(struct DispatchObject *object,
                                  const s32 *script);
/// Hands an object to the player's control on a layered map (SC_PLAYER).
void ObjectDispatch_StartPlayerControlFar(struct DispatchObject *object);
/// Hands an object to the player's control on the world map (SC_WORLD).
void ObjectDispatch_StartWorldMapControlFar(struct DispatchObject *object);
/// Runs the script that walks an object wherever the keys point (SC_KEYS),
/// which the leader gets when gGameState.movementMode is 1.
void ObjectDispatch_StartMoveByKeysFar(struct DispatchObject *object);
/// Runs SC_KEY_COUNT on an object every frame, which the leader gets when
/// gGameState.movementMode is 2.
void ObjectDispatch_StartKeyCheckFar(struct DispatchObject *object);
#ifndef OWN_Object_CreateFar
/// Creates a field object at (x, y, z) in 16.16 pixels, or returns 0 when none
/// is free (Object_CreateField). `id / 4096` is its enum ObjectCreateKind and
/// the low 12 bits its sprite's animation id.
struct Object *Object_CreateFar(s32 id, s32 x, s32 y, s32 z);
#endif
/// Releases an object's sprites and zeroes it, so it can be handed out again.
void ObjectDispatch_ReleaseFar(struct DispatchObject *object);
/// Puts an object on the camera script (SC_CAMERA); following a leader, it
/// moves at most 4 pixels a frame.
void ObjectDispatch_StartCameraFar(struct DispatchObject *object, s32 arg);
/// Places an object at (x, y, z) in 16.16 pixels and stops it, clearing its
/// movement target.
void Object_SetPositionAndResetMotionFar(struct Object *object, s32 x, s32 y,
                                         s32 z);
/// Clears one step of a sprite's pixel-by-pixel dissolve in its OBJ VRAM tiles
/// (SpriteDissolve_Step); `pos` is the step, 0 to 127.
void render_animated_tile_frameFar(u8 *obj, u32 pos);
/// Dissolves one sprite away over 32 frames; the auto battle uses it.
void SpriteDissolve_RunFar(u8 *obj);
/// Dissolves `cnt` sprites together over 32 frames; the battle's staging uses
/// it once a unit is off its lists.
void SpriteDissolve_RunManyFar(u8 **table, s32 cnt);
/// Loads layout `index` (metatiles, metatile behaviors, cells, tile animations,
/// blend script, markers and graphics) and starts its layers scrolling.
/// Returns 2.
s32 MapGrid_LoadLayoutFar(s32 index);
/// Sets up the world map's perspective view: the affine ground plane, its
/// camera and projection, and its per-frame and HBlank updates. XXX: returns no
/// value; the world map ignores it.
s32 WorldMap_InitializePerspectiveFar(void);
/// Centres the camera on the point the map follows, or on (0, 0) with none,
/// and scrolls the layers there.
void MapGrid_CenterOnFollowedPointFar(void);
/// Loads and draws the 2x2 world map tiles around the player in both layers.
void WorldMap_UpdateCurrentTileBlockFar(void);
/// Loads the world map's palette and tiles for the area at (x, z), taking the
/// snowfield set where the ground there is WORLD_MAP_REGION_SNOWFIELD.
void WorldMap_LoadGraphicsFar(s32 x, s32 z);
/// Leaves an object standing still on the idle script, a lone SC_END.
void ObjectDispatch_StartIdleFar(struct DispatchObject *object);
/// Waits until an object's script reaches SC_END, giving up after 300 frames
/// (five seconds).
void ObjectDispatch_WaitForValue16Far(void *raw);
#ifndef OWN_Object_SetMoveTargetFar
/// Sends an object towards (x, y, z) in 16.16 pixels, pulling the target in so
/// it coasts to a stop there; within a pixel it arrives at once.
void Object_SetMoveTargetFar(struct Object *object, s32 x, s32 y, s32 z);
#endif
/// Waits until a field object reaches its target, giving up after 600 frames
/// (ten seconds) with the object still moving.
void Object_WaitForMoveFar(void *fieldObject);
/// Puts the world map's default cells back after the still view and returns to
/// the perspective view, without reloading the area's palette or tiles.
void WorldMap_LoadDefaultCellsAndUpdateBlockFar(void);
/// Plays a map edit sequence, as doors and other changing scenery are drawn:
/// each record's metatile numbers go to the destination, then it waits that
/// record's delay. Returns only when the sequence ends.
void MapGrid_PlayMetatileCopySequenceFar(u16 *command, s32 destination_x,
                                         u32 destination_y);
/// Copies the metatile numbers of a rectangle of cells, keeping the
/// destination's attributes, and redraws what's on screen; the destination
/// comes before the size.
void MapGrid_CopyMetatileIndicesRectFar(s32 sourceX, s32 sourceY, u32 dst_x,
                                        u32 dst_y, u32 width, u32 height);
/// Restarts tile animation channel `no` by clearing its paused byte.
void TileAnimation_ResumeChannelFar(u32 no);
/// Pauses tile animation channel `no` by setting its paused byte.
void TileAnimation_PauseChannelFar(u32 no);
/// Lets the map's blend script run again after DisplayBlend_SuspendScriptFar.
void DisplayBlend_ResumeScriptFar(void);
/// Stops the map's blend script, as the lighthouse maps and Vale's Babi
/// scene do while they set the blend themselves.
void DisplayBlend_SuspendScriptFar(void);
/// Returns the ground height of `layer` at (x, y) in 16.16 pixels, where y is
/// the map's down axis (an object's z).
s32 MapGrid_GetTerrainHeightFar(s32 layer, s32 x, s32 y);
/// Returns the metatile behavior of the cell of `layer` under (x, y), given in
/// 16.16 pixels.
s32 MapGrid_GetMetatileBehaviorAtFar(s32 layer, s32 x, s32 y);
/// Copies a rectangle of cells whole, metatile numbers and attributes, and
/// draws the copied cells that are in view.
void MapGrid_CopyMetatileCellsRectFar(s32 sourceX, s32 sourceY, s32 width,
                                      s32 height, s32 dst_x, s32 dst_y);
/// Copies only the attributes (flags, metatile behavior and shape) of a
/// rectangle of cells. Nothing is redrawn, since attributes don't show.
void MapGrid_CopyCellAttributeRectFar(s32 sourceX, s32 sourceY, s32 width,
                                      s32 height, s32 dst_x, s32 dst_y);
/// Copies only the metatile numbers of a rectangle of cells, the size before
/// the destination, and redraws the cells in view.
void MapGrid_CopyCellIndexRectFar(s32 sourceX, s32 sourceY, s32 width,
                                  s32 height, s32 dst_x, s32 dst_y);
/// Puts world map tile (x / 16, y / 16) into the tile grid at
/// (px / 16, py / 16), drawing it at once when it's next to the view.
void WorldMap_SetWindowCellTileFar(s32 x, s32 y, s32 px, s32 py);
/// Returns whether an object can step to `position`, as an enum
/// MapGridStepCheck: a wall there, or ground too high or too low for one step.
s32 MapGrid_CheckObjectStepFar(struct Object *object,
                               struct FieldPosition *position);
/// Sets the flags of a one-sprite object's sprite (bit 0 draws its shadow).
void ObjectDispatch_SetSingleChildFlagsFar(struct DispatchObject *object,
                                           u32 flags);
/// Sets the OAM blend mode of a one-sprite object's sprite; an object with high
/// kind bits set is left alone.
void ObjectDispatch_SetChildBlendModeFar(struct DispatchObject *object,
                                         u32 mode);
/// Changes the screen shake, leaving alone an axis whose argument is negative.
/// Amplitudes are in 16.16 pixels, the decay a 16.16 factor per frame.
void MapGrid_SetScreenShakeFar(s32 value0, s32 value1, s32 value2);
/// Waits until the shake dies down below 1/256 pixel, or 300 frames pass, and
/// then stops it.
void MapGrid_WaitForShakeToSettleFar(void);
/// Stops the object camera updates, brings the palette back and shows the
/// sprites alone over BG0, with BG1 to BG3 hidden.
void ObjectSystem_SuspendProjectionAndShowSpritesFar(void);
/// Restarts the object camera updates and hides BG1 to BG3 and the sprites,
/// undoing ObjectSystem_SuspendProjectionAndShowSpritesFar.
void ObjectSystem_ResumeProjectionAndHideLayersFar(void);
/// Returns -1 when the overhead layer's cell under `position`, its height
/// taken off, is a wall (MB_WALL), else 0.
s32 MapGrid_IsWallAtFar(struct WorldPosition *position);
/// Adds a layer playing animation `no` to a one-sprite object's sprite; a
/// negative `no` does nothing.
void ObjectDispatch_RegisterChildMetadataFar(struct DispatchObject *object,
                                             s32 no);
/// Freezes the world map's perspective view as a still picture on BG1, so a
/// menu or script can use the window and VBlank.
void WorldMap_StartStillViewFar(void);
/// Reloads the world map area's palette and tiles after the still view and
/// returns to the perspective view.
void WorldMap_LoadAreaGraphicsFar(void);
/// Sets the colour mode (SpriteColorMode) of a one-sprite object's layers,
/// except hidden ones.
void Animation_ApplyChildValuesFar(struct DispatchObject *object, u32 val);
/// Sets every layer of a sprite to colour mode `value`, skipping those drawn
/// hidden (SPRITE_COLOR_HIDDEN), and marks the sprite for redrawing.
void ObjectGroup_SetChildValueUnlessFifteenFar(void *raw_parent, u32 value);
/// Returns -1 when an object at `values` would overlap a solid object
/// (OBJ_EVENT_SOLID), each radius less 2 pixels, else 0.
s32 ScriptObject_CheckOverlapFar(struct ScriptObjectEntry *object, s32 *values);
/// Sets which of BG1 to BG3 the map shows, in bits 9 to 11 of its display
/// flags as REG_DISPCNT lays them out.
void MapGrid_SetShownLayersFar(u32 v);
/// Returns how many frames the first `cnt` frames of animation `no` of `id`
/// last, stopping at its first play, hold, jump or end command.
s32 ResourceMetadata_SumCommandLengthsFar(s32 id, u32 no, s32 cnt);
/// Returns the world map's terrain behaviour at `position` and stores the
/// block's terrain kind in `terrain_kind`, for the world map's encounters.
s32 GetWorldMapTerrainBehaviorFar(struct WorldPosition *position,
                                  s32 *terrain_kind);
/// Decodes the frames of animation `number` into resource slot `slot`'s
/// `buffer`, remapping its colours for `variant` 1 to 5.
s32 ResourceSlot_LoadFar(u32 slot, u32 *buffer, s32 number, u32 variant);
/// Writes `collisionCode` into the cell of `layer` under (x, y), given in 16.16
/// pixels; before the map is set up it does nothing.
void MapGrid_SetMetatileBehaviorAtFar(u32 layer, s32 x, s32 y,
                                      u32 collisionCode);
/// Sets the timer step of a sprite's animated layers: 16 plays at normal
/// speed, 0 stops them.
void AnimationObject_SetActiveTimerStepsFar(struct AnimationObject *group,
                                            s32 val);
/// Remaps `cnt` bytes of `buf` in place through gRuntime_ByteRemapTable, as
/// the command icons' colours are.
void Runtime_RemapBytesByTableFar(u8 *buf, s32 cnt);
/// Pauses the tile animations while a menu is open, unless the world map's
/// still view is shown.
void TileAnimation_SuspendFar(void);
/// Restarts the tile animations when a menu closes, unless the world map's
/// still view is shown.
void TileAnimation_ResumeFar(void);
/// Returns the first solid object an object at `values` would overlap, as
/// ScriptObject_CheckOverlapFar tests, or NULL; the Move effect uses it to see
/// what stands in the way of the stone pillar it pushes.
struct ScriptObjectEntry *
ScriptObject_FindOverlappingEntryFar(struct ScriptObjectEntry *object,
                                     s32 *values);
/// Returns the terrain kind (the low four bits of the first collision byte) of
/// layer `index`'s cell under (x, y), given in 16.16 pixels.
s32 MapGrid_GetCellAttributeLowNibbleFar(s32 index, s32 x, s32 y);
