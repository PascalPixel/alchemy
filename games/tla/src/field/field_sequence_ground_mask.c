/// @file
/// The ground mask: the marks drawn over the ground in front of the actor,
/// in fade and blank forms, and the effect sprites that draw them.
// Part of field_sequence.c.

/// A screen row's map cell, in pixels, in an OAM entry's 8-bit y, as
/// MAPGRID_CELL_PIXEL_MASK is for a whole pixel coordinate.
#define CELL_GRID_ROW_MASK 0xf0
// XXX: what the player sees of this bit; a render of a map whose third layer
// sets it, with FieldEffect_StartBlankOverlay running, would settle it.
/// MapCell.attributeB's lowest bit, on the map's third layer: a cell the blank
/// overlay marks.
#define CELL_BLANK_OVERLAY 0x01

/// Each frame of the fade overlay: on each of the two cells beside the actor
/// (left and right of it), where the ground one or two cells south rises above
/// the actor's floor, draws a semi-transparent 16-pixel-wide sprite of colour 1
/// at the height of that ground.
void FadeOverlay_DrawMarks(void) {
  struct FadeOverlayState *state = Ram_HeapSlots->fadeOverlay;
  s16 *camera = ((struct FadeView *)Ram_HeapSlots->mapGrid)->camera;
  s32 cameraX = camera[1];
  s32 cameraZ = camera[3];
  struct AnimationSpritePart *mark = state->marks;
  struct FadeActor *actor = state->actor;
  FIXED x;
  FIXED z;
  s32 terrainFloor;
  u8 terrainLayer;
  u32 tile;
  s32 height;
  s32 secondHeight;
  s32 screenRow;
  /* LEFTOVER(frame): an unused local the ROM's frame reserves and never
   * touches. */
  UNUSED s16 unused[2];

  if (actor == 0)
    return;
  z = actor->z;
  x = actor->x;
  terrainFloor = actor->floor;
  terrainLayer = actor->terrainId;
  // The first tile of the solid block FieldEffect_StartFadeOverlay loaded.
  tile =
      gVramCacheEntries[VRAM_CACHE_ENTRY_FADE_OVERLAY].offset / TILE_SIZE_4BPP;
  // The left cell, 8 pixels left of the actor.
  x -= toFIXED(8);
  height =
      FIXEDtoINT(MapGrid_GetTerrainHeightFar(terrainLayer, x, z + toFIXED(16)));
  secondHeight = FIXEDtoINT(MapGrid_GetTerrainHeightFar(terrainLayer, x,
                                                        z + toFIXED(32))) -
                 16;
  if (secondHeight > height)
    height = secondHeight;
  if (height > 0 && height > terrainFloor) {
    // The sprite mode and priority set next replace what the two words put
    // there, so the left and right marks end up alike although their third
    // words differ.
    ((u32 *)mark)[1] = OBJ_SIZE_16X16 | OBJ_MODE(OBJ_MODE_WINDOW);
    ((u32 *)mark)[2] = OBJ_PRIORITY(1);
    mark->priority = 0;
    mark->tile = tile;
    mark->objectMode = OBJ_MODE_SEMI_TRANSPARENT;
    // Snapped to the cell's grid, then moved up by the ground's height.
    mark->x = ((u32)FIXEDtoINT(x) & MAPGRID_CELL_PIXEL_MASK) - cameraX;
    screenRow = FIXEDtoINT(z) & CELL_GRID_ROW_MASK;
    mark->y = screenRow - cameraZ - height + 16;
    Oam_AddSpriteAtDepthClamped(mark, 0);
  }
  // The right cell, 8 pixels right of the actor, read the same way.
  x += toFIXED(16);
  height =
      FIXEDtoINT(MapGrid_GetTerrainHeightFar(terrainLayer, x, z + toFIXED(16)));
  mark = &state->marks[1];
  secondHeight = FIXEDtoINT(MapGrid_GetTerrainHeightFar(terrainLayer, x,
                                                        z + toFIXED(32))) -
                 16;
  if (secondHeight > height)
    height = secondHeight;
  if (height > 0 && height > terrainFloor) {
    ((u32 *)mark)[1] = OBJ_SIZE_16X16 | OBJ_MODE(OBJ_MODE_WINDOW);
    ((u32 *)mark)[2] = 0;
    mark->priority = 0;
    mark->tile = tile;
    mark->objectMode = OBJ_MODE_SEMI_TRANSPARENT;
    mark->x = ((u32)FIXEDtoINT(x) & MAPGRID_CELL_PIXEL_MASK) - cameraX;
    screenRow = FIXEDtoINT(z) & CELL_GRID_ROW_MASK;
    mark->y = screenRow - cameraZ - height + 16;
    Oam_AddSpriteAtDepthClamped(mark, 0);
  }
}

/// Starts the fade overlay on an actor (the party leader when NULL): loads its
/// solid tiles, sets the blending and draws it each frame.
void FieldEffect_StartFadeOverlay(void *actor) {
  struct FadeOverlayState *state;
  u8 *tiles;
  volatile u32 fill;

  state = HeapSlot_AllocEwram(HEAP_SLOT_FADE_OVERLAY, sizeof(*state));
  tiles = HeapSlot_Alloc(HEAP_SLOT_TILE_SCRATCH, 1024);
  Dma_Fill32Cell(fill, PIXELS_4BPP(1), tiles, 512);
  VramCache_Load(VRAM_CACHE_ENTRY_FADE_OVERLAY, 512, tiles);
  HeapSlot_Release(HEAP_SLOT_TILE_SCRATCH);
  Task_Add(FadeOverlay_DrawMarks, TASK_PASS_FRAME);
  Io_Write16(BLDCNT_TGT1_BG1 | BLDCNT_TGT1_BG2 | BLDCNT_TGT1_BG3 |
                 BLDCNT_TGT1_OBJ | BLDCNT_EFFECT_LIGHTEN | BLDCNT_TGT2_ALL,
             &REG_BLDCNT);
  Io_Write16(BLEND_ALPHA(BLEND_WEIGHT_MAX, 0), &REG_BLDALPHA);
  // 31 is above the register's useful maximum, BLEND_FADE_MAX, and acts as it.
  Io_Write16(31, &REG_BLDY);
  Dma_Fill32Cell(fill, 0, state, sizeof(*state));
  if (actor == NULL)
    actor = Actor_Lookup(gMainRoutineState.leaderActorId);
  state->actor = actor;
}

/// Stops drawing the fade overlay, keeping its state.
void FadeOverlay_Suspend(void) { Task_SuspendAll(FadeOverlay_DrawMarks); }

/// Draws the fade overlay again after FadeOverlay_Suspend.
void FadeOverlay_Resume(void) { Task_ResumeAll(FadeOverlay_DrawMarks); }

/// The blank overlay's task, which FieldEffect_StartBlankOverlay starts: each
/// frame it draws a 16-pixel mark beside the actor where the map's third layer
/// flags a cell two rows up (left or right of the actor), and another for the
/// row one up.
void BlankOverlay_DrawMarks(void) {
  struct AnimationSpritePart *mark = Ram_HeapSlots->fadeOverlay;
  struct MapState *map = Ram_HeapSlots->mapGrid;
  s16 *camera = (s16 *)&map->viewX;
  s16 cameraX = camera[1];
  s16 cameraZ = camera[3];
  struct FadeActor *actor = ((struct FadeOverlayState *)mark)->actor;
  s32 x;
  s32 row;
  u32 tile;
  struct MapCell *cells;
  struct MapCell *right;
  struct MapCell *left;
  struct AnimationSpritePart *m;

  if (actor == 0)
    return;
  x = ((s16 *)&actor->x)[1];
  row = FIXEDtoINT(actor->z - actor->y);
  tile =
      gVramCacheEntries[VRAM_CACHE_ENTRY_FADE_OVERLAY].offset / TILE_SIZE_4BPP;
  cells = map->layers[2].cells;
  right = cells + (x + 8) / MAPGRID_CELL_PIXELS +
          (row / MAPGRID_CELL_PIXELS - 2) * MAPGRID_SIZE;
  left = cells + (x - 8) / MAPGRID_CELL_PIXELS +
         (row / MAPGRID_CELL_PIXELS - 2) * MAPGRID_SIZE;
  if ((right->attributeB & CELL_BLANK_OVERLAY) ||
      (left->attributeB & CELL_BLANK_OVERLAY)) {
    m = mark;
    ((u32 *)m)[1] = OBJ_SIZE_16X16;
    ((u32 *)m)[2] = OBJ_PRIORITY(1);
    m->tile = tile;
    m->x = (u16)(x - cameraX) - 8;
    m->y = (u16)(((u16)row & CELL_GRID_ROW_MASK) - cameraZ) - 32;
    mark = m + 1;
    Oam_AddSpriteAtDepthClamped(m, 1);
  }
  right = cells + (x + 8) / MAPGRID_CELL_PIXELS +
          (row / MAPGRID_CELL_PIXELS - 1) * MAPGRID_SIZE;
  left = cells + (x - 8) / MAPGRID_CELL_PIXELS +
         (row / MAPGRID_CELL_PIXELS - 1) * MAPGRID_SIZE;
  if ((right->attributeB & CELL_BLANK_OVERLAY) ||
      (left->attributeB & CELL_BLANK_OVERLAY)) {
    m = mark;
    ((u32 *)m)[1] = OBJ_SIZE_16X16;
    ((u32 *)m)[2] = OBJ_PRIORITY(1);
    m->tile = tile;
    m->x = (u16)(x - cameraX) - 8;
    m->y = (u16)(((u16)row & CELL_GRID_ROW_MASK) - cameraZ) - 16;
    Oam_AddSpriteAtDepthClamped(m, 1);
  }
}

/// Like FieldEffect_StartFadeOverlay with blank tiles and no blending:
/// loads 512 bytes of blank tiles into VRAM_CACHE_ENTRY_FADE_OVERLAY and starts
/// BlankOverlay_DrawMarks on an actor (the party leader when 0).
void FieldEffect_StartBlankOverlay(s32 actor) {
  struct FadeOverlayState *state =
      HeapSlot_AllocEwram(HEAP_SLOT_FADE_OVERLAY, sizeof(*state));
  void *buffer = HeapSlot_Alloc(HEAP_SLOT_TILE_SCRATCH, 1024);

  Dma_Fill32(0, buffer, 512);
  VramCache_Load(VRAM_CACHE_ENTRY_FADE_OVERLAY, 512, buffer);
  Task_Add(BlankOverlay_DrawMarks, TASK_PASS_FRAME);
  HeapSlot_Release(HEAP_SLOT_TILE_SCRATCH);
  if (actor == 0)
    actor = Actor_Lookup(gMainRoutineState.leaderActorId);
  state->actor = (struct FadeActor *)actor;
}

/// Suspends the BlankOverlay_DrawMarks overlay task.
void BlankOverlay_Suspend(void) { Task_SuspendAll(BlankOverlay_DrawMarks); }

/// Resumes the BlankOverlay_DrawMarks overlay task.
void BlankOverlay_Resume(void) { Task_ResumeAll(BlankOverlay_DrawMarks); }

/// Fills the overlay's 512 bytes of sprite tiles
/// (VRAM_CACHE_ENTRY_FADE_OVERLAY) with PIXELS_8BPP(1) when solid is set, which
/// in these 16-colour tiles is colour 1 in every other column (no caller sets
/// it), else clears them.
void FadeOverlay_FillTiles(s32 solid) {
  void *buffer = HeapSlot_Alloc(HEAP_SLOT_TILE_SCRATCH, 1024);

  if (solid == 0) {
    Dma_Fill32(0, buffer, 512);
  } else {
    Dma_Fill32(PIXELS_8BPP(1), buffer, 512);
  }
  VramCache_Load(VRAM_CACHE_ENTRY_FADE_OVERLAY, 512, buffer);
  HeapSlot_Release(HEAP_SLOT_TILE_SCRATCH);
}

/// Sets up sprite through EffectSprite_Init with the overlay's tiles
/// (VRAM_CACHE_ENTRY_FADE_OVERLAY), at priority 1 in palette 0.
void EffectSprite_InitWithFadeOverlay(struct EffectSprite *sprite, s32 pivotX,
                                      s32 pivotY, u32 attributes) {
  EffectSprite_Init(sprite, pivotX, pivotY, attributes,
                    gVramCacheEntries[VRAM_CACHE_ENTRY_FADE_OVERLAY].offset /
                        TILE_SIZE_4BPP);
  sprite->depth = 0;
  sprite->oam.priority = 1;
  sprite->oam.fullColor = 0;
  sprite->oam.palette = 0;
}

/// Projects the world position (x, y, z) to the screen, stores it as the
/// effect sprite's position and draws the sprite.
void EffectSprite_DrawAtWorldXYZ(struct EffectSprite *sprite, s32 x, s32 y,
                                 s32 z) {
  s32 position[3];

  SET_VECTOR(position, x, y, z);
  Camera_WorldToScreen(position);
  sprite->x = position[0];
  sprite->y = position[2];
  EffectSprite_Draw(sprite);
}

/// As EffectSprite_InitWithFadeOverlay, semi-transparent and at priority 0.
void EffectSprite_InitWithFadeOverlayBlended(struct EffectSprite *sprite,
                                             s32 pivotX, s32 pivotY,
                                             u32 attributes) {
  EffectSprite_Init(sprite, pivotX, pivotY, attributes,
                    gVramCacheEntries[VRAM_CACHE_ENTRY_FADE_OVERLAY].offset /
                        TILE_SIZE_4BPP);
  sprite->depth = 0;
  sprite->oam.priority = 0;
  sprite->oam.fullColor = 0;
  sprite->oam.palette = 0;
  sprite->oam.objectMode = OBJ_MODE_SEMI_TRANSPARENT;
}

#undef CELL_GRID_ROW_MASK
#undef CELL_BLANK_OVERLAY
