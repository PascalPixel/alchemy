/// @file
/// The ground mask: the marks drawn over the ground in front of the actor,
/// in fade and blank forms, and the effect sprites that draw them.
// Part of field_sequence.c.

/// Each frame, on the two cells beside the actor
/// where the ground one or two cells south rises above the actor's floor,
/// draws a 16-pixel-wide mark at the height of that ground.
void FadeOverlay_DrawMarks(void) {
  struct FadeOverlayWork *work = Ram_HeapSlots->fadeOverlay;
  s16 *camera = ((struct FadeView *)Ram_HeapSlots->mapGrid)->camera;
  s32 cameraX = camera[1];
  s32 cameraZ = camera[3];
  struct AnimationSpritePart *mark = work->marks;
  struct FadeActor *actor = work->actor;
  FIXED x;
  FIXED z;
  s32 terrainFloor;
  u8 terrainLayer;
  u32 tile;
  s32 height;
  s32 secondHeight;
  s32 screenRow;
  s16 unused[2];

  if (actor == 0)
    return;
  z = actor->z;
  x = actor->x;
  terrainFloor = actor->floor;
  terrainLayer = actor->terrainId;
  tile = gVramCacheEntries[VRAM_CACHE_ENTRY_FADE_OVERLAY].offset >> 5;
  x -= toFIXED(8);
  height =
      FIXEDtoINT(MapGrid_GetTerrainHeightFar(terrainLayer, x, z + toFIXED(16)));
  secondHeight = FIXEDtoINT(MapGrid_GetTerrainHeightFar(terrainLayer, x,
                                                        z + toFIXED(32))) -
                 16;
  if (secondHeight > height)
    height = secondHeight;
  if (height > 0 && height > terrainFloor) {
    ((u32 *)mark)[1] = OBJ_SIZE_16X16 | OBJ_MODE(OBJ_MODE_WINDOW);
    ((u32 *)mark)[2] = OBJ_PRIORITY(1);
    mark->priority = 0;
    mark->tile = tile;
    mark->objectMode = OBJ_MODE_SEMI_TRANSPARENT;
    mark->x = ((u32)FIXEDtoINT(x) & 0xfff0) - cameraX;
    screenRow = FIXEDtoINT(z) & 0xf0;
    mark->y = screenRow - cameraZ - height + 16;
    Oam_AddSpriteAtDepthClamped(mark, 0);
  }
  x += toFIXED(16);
  height =
      FIXEDtoINT(MapGrid_GetTerrainHeightFar(terrainLayer, x, z + toFIXED(16)));
  mark = &work->marks[1];
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
    mark->x = ((u32)FIXEDtoINT(x) & 0xfff0) - cameraX;
    screenRow = FIXEDtoINT(z) & 0xf0;
    mark->y = screenRow - cameraZ - height + 16;
    Oam_AddSpriteAtDepthClamped(mark, 0);
  }
}

/// Starts the fade overlay on object (the party leader when NULL): loads its
/// solid tiles, sets the blending and draws it each frame.
void FieldEffect_StartFadeOverlay(void *object) {
  struct FadeOverlayWork *work;
  u8 *tiles;
  volatile u32 fill;

  work = HeapSlot_AllocEwram(HEAP_SLOT_FADE_OVERLAY, sizeof(*work));
  tiles = HeapSlot_Alloc(HEAP_SLOT_TILE_SCRATCH, 1024);
  Dma_Fill32Cell(fill, 0x11111111, tiles, 512);
  VramCache_Load(VRAM_CACHE_ENTRY_FADE_OVERLAY, 512, tiles);
  HeapSlot_Release(HEAP_SLOT_TILE_SCRATCH);
  Task_Add(FadeOverlay_DrawMarks, TASK_PASS_VBLANK);
  Io_Write16(BLDCNT_TGT1_BG1 | BLDCNT_TGT1_BG2 | BLDCNT_TGT1_BG3 |
                 BLDCNT_TGT1_OBJ | BLDCNT_EFFECT_LIGHTEN | BLDCNT_TGT2_ALL,
             &REG_BLDCNT);
  Io_Write16(BLEND_ALPHA(16, 0), &REG_BLDALPHA);
  // 31 is above the register's useful maximum of 16, which it acts as.
  Io_Write16(31, &REG_BLDY);
  Dma_Fill32Cell(fill, 0, work, sizeof(*work));
  if (object == NULL)
    object = Actor_Lookup(gMainRoutineWork.leaderActorId);
  work->actor = object;
}

/// Suspends the fade overlay's drawing callback.
void FadeOverlay_Suspend(void) { Task_SuspendAll(FadeOverlay_DrawMarks); }

/// Resumes the fade overlay's drawing callback.
void FadeOverlay_Resume(void) { Task_ResumeAll(FadeOverlay_DrawMarks); }

/// The blank overlay's task, which FieldEffect_StartBlankOverlay starts: each
/// frame it draws a 16-pixel mark beside the actor where the map's third layer
/// flags a cell two rows up (left or right of the actor), and another for the
/// row one up.
void BlankOverlay_DrawMarks(void) {
  struct AnimationSpritePart *mark = Ram_HeapSlots->fadeOverlay;
  struct MapWork *map = Ram_HeapSlots->mapGrid;
  s16 *camera = (s16 *)&map->viewX;
  s16 cameraX = camera[1];
  s16 cameraZ = camera[3];
  struct FadeActor *actor = ((struct FadeOverlayWork *)mark)->actor;
  s32 x;
  s32 row;
  u32 tile;
  struct MapCell *cells;
  struct MapCell *a;
  struct MapCell *b;
  struct AnimationSpritePart *m;

  if (actor == 0)
    return;
  x = ((s16 *)&actor->x)[1];
  row = FIXEDtoINT(actor->z - actor->y);
  tile = gVramCacheEntries[VRAM_CACHE_ENTRY_FADE_OVERLAY].offset >> 5;
  cells = map->layers[2].cells;
  a = cells + (x + 8) / 16 + (row / 16 - 2) * 128;
  b = cells + (x - 8) / 16 + (row / 16 - 2) * 128;
  if ((a->attributeB & 1) || (b->attributeB & 1)) {
    m = mark;
    ((u32 *)m)[1] = OBJ_SIZE_16X16;
    ((u32 *)m)[2] = OBJ_PRIORITY(1);
    m->tile = tile;
    m->x = (u16)(x - cameraX) - 8;
    m->y = (u16)(((u16)row & 0xf0) - cameraZ) - 32;
    mark = m + 1;
    Oam_AddSpriteAtDepthClamped(m, 1);
  }
  a = cells + (x + 8) / 16 + (row / 16 - 1) * 128;
  b = cells + (x - 8) / 16 + (row / 16 - 1) * 128;
  if ((a->attributeB & 1) || (b->attributeB & 1)) {
    m = mark;
    ((u32 *)m)[1] = OBJ_SIZE_16X16;
    ((u32 *)m)[2] = OBJ_PRIORITY(1);
    m->tile = tile;
    m->x = (u16)(x - cameraX) - 8;
    m->y = (u16)(((u16)row & 0xf0) - cameraZ) - 16;
    Oam_AddSpriteAtDepthClamped(m, 1);
  }
}

/// Like FieldEffect_StartFadeOverlay with blank tiles and no blending:
/// loads 512 bytes of blank tiles into VRAM_CACHE_ENTRY_FADE_OVERLAY and starts
/// BlankOverlay_DrawMarks on object (the controlled owner's when 0).
void FieldEffect_StartBlankOverlay(s32 object) {
  struct FadeOverlayWork *work =
      HeapSlot_AllocEwram(HEAP_SLOT_FADE_OVERLAY, sizeof(*work));
  void *buffer = HeapSlot_Alloc(HEAP_SLOT_TILE_SCRATCH, 1024);

  Dma_Fill32(0, buffer, 512);
  VramCache_Load(VRAM_CACHE_ENTRY_FADE_OVERLAY, 512, buffer);
  Task_Add(BlankOverlay_DrawMarks, TASK_PASS_VBLANK);
  HeapSlot_Release(HEAP_SLOT_TILE_SCRATCH);
  if (object == 0)
    object = Actor_Lookup(gMainRoutineWork.leaderActorId);
  work->actor = (struct FadeActor *)object;
}

/// Suspends the BlankOverlay_DrawMarks overlay task.
void BlankOverlay_Suspend(void) { Task_SuspendAll(BlankOverlay_DrawMarks); }

/// Resumes the BlankOverlay_DrawMarks overlay task.
void BlankOverlay_Resume(void) { Task_ResumeAll(BlankOverlay_DrawMarks); }

/// Fills the overlay's 512 bytes of sprite tiles
/// (VRAM_CACHE_ENTRY_FADE_OVERLAY) with colour 1 when solid is set, else clears
/// them.
void BattleEffect_FillOverlayTiles(s32 solid) {
  void *buffer = HeapSlot_Alloc(HEAP_SLOT_TILE_SCRATCH, 1024);

  if (solid == 0) {
    Dma_Fill32(0, buffer, 512);
  } else {
    Dma_Fill32(0x01010101, buffer, 512);
  }
  VramCache_Load(VRAM_CACHE_ENTRY_FADE_OVERLAY, 512, buffer);
  HeapSlot_Release(HEAP_SLOT_TILE_SCRATCH);
}

/// Sets up sprite through EffectSprite_Init with resource 94's tiles, at
/// priority 1 in palette 0.
void EffectSprite_InitWithFadeOverlay(struct EffectSprite *sprite, s32 a, s32 b,
                                      s32 c) {
  EffectSprite_Init(sprite, a, b, c,
                    gVramCacheEntries[VRAM_CACHE_ENTRY_FADE_OVERLAY].offset >>
                        5);
  sprite->depth = 0;
  sprite->oam.priority = 1;
  sprite->oam.fullColor = 0;
  sprite->oam.palette = 0;
}

/// Projects the world position (x, y, z) to the screen, stores it as the
/// effect sprite's position and draws the sprite.
void BattleEffect_DrawSpriteAtWorldPosition(struct EffectSprite *sprite, s32 x,
                                            s32 y, s32 z) {
  s32 position[3];

  SET_VECTOR(position, x, y, z);
  Camera_WorldToScreen(position);
  sprite->x = position[0];
  sprite->y = position[2];
  EffectSprite_Draw(sprite);
}

/// The twin of EffectSprite_InitWithFadeOverlay with object mode 1 and priority
/// 0.
void EffectSprite_InitWithFadeOverlayBlended(struct EffectSprite *sprite, s32 a,
                                             s32 b, s32 c) {
  EffectSprite_Init(sprite, a, b, c,
                    gVramCacheEntries[VRAM_CACHE_ENTRY_FADE_OVERLAY].offset >>
                        5);
  sprite->depth = 0;
  sprite->oam.priority = 0;
  sprite->oam.fullColor = 0;
  sprite->oam.palette = 0;
  sprite->oam.objectMode = 1;
}
