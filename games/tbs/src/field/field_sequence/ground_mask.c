/// @file
/// The fade overlay drawn on the ground beside an actor.
// Part of field_sequence.c.

#include "../field_sequence_internal.h"

/// Each frame of the fade overlay: on each of the two cells beside the actor
/// (left and right of it), where the ground one or two cells south rises above
/// the actor's floor, draws a semi-transparent 16-pixel-wide sprite of colour 1
/// at the height of that ground.
void FadeOverlay_DrawMarks(void) {
  struct FadeOverlayWork *work = HEAP_SLOT_BLOCK(HEAP_SLOT_FADE_OVERLAY);
  struct MapGridScrollWork *grid = HEAP_SLOT_BLOCK(HEAP_SLOT_MAP_GRID);
  s32 *view = grid->view;
  s32 cameraX = (s16)(view[0] >> 16);
  s32 cameraZ = (s16)(view[1] >> 16);
  struct AnimationSpritePart *mark = work->marks;
  struct Actor *actor = work->actor;
  s32 x;
  s32 z;
  s32 terrainFloor;
  s32 terrainLayer;
  u32 tile;
  s32 height;
  s32 secondHeight;
  s32 screenRow;
  /* LEFTOVER(frame): an unused local the ROM's frame reserves and never
   * touches: of the 16 bytes it reserves (sub sp, #16), no instruction
   * addresses the 4 at sp+12 to sp+15; as without it, 4 of 200 instructions
   * differ in tbs-en. */
  UNUSED s16 unused[2];

  if (actor == 0)
    return;
  x = actor->x.fixed;
  z = actor->z.fixed;
  terrainFloor = actor->groundHeight >> 16;
  terrainLayer = actor->layer;
  // The first tile of the solid block FieldEffect_StartFadeOverlay loaded (VRAM
  // cache entry VRAM_CACHE_ENTRY_FADE_OVERLAY; tiles are 32 bytes).
  tile = gVramCacheEntries[VRAM_CACHE_ENTRY_FADE_OVERLAY].offset >> 5;
  // The left cell, 8 pixels left of the actor.
  x -= px(8);
  height = MapGrid_GetTerrainHeightFar(terrainLayer, x, z + px(16)) >> 16;
  secondHeight =
      (MapGrid_GetTerrainHeightFar(terrainLayer, x, z + px(32)) >> 16) - 16;
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
    // Snapped to the cell's 16-pixel grid, then moved up by the ground's
    // height.
    mark->x = ((u32)(x >> 16) & 0xfff0) - cameraX;
    screenRow = (z >> 16) & 0xf0;
    mark->y = screenRow - cameraZ - height + 16;
    Oam_AddSpriteAtDepth(mark, 0);
  }
  // The right cell, 8 pixels right of the actor, read the same way.
  x += px(16);
  height = MapGrid_GetTerrainHeightFar(terrainLayer, x, z + px(16)) >> 16;
  mark = &work->marks[1];
  secondHeight =
      (MapGrid_GetTerrainHeightFar(terrainLayer, x, z + px(32)) >> 16) - 16;
  if (secondHeight > height)
    height = secondHeight;
  if (height > 0 && height > terrainFloor) {
    ((u32 *)mark)[1] = OBJ_SIZE_16X16 | OBJ_MODE(OBJ_MODE_WINDOW);
    ((u32 *)mark)[2] = 0;
    mark->priority = 0;
    mark->tile = tile;
    mark->objectMode = OBJ_MODE_SEMI_TRANSPARENT;
    mark->x = ((u32)(x >> 16) & 0xfff0) - cameraX;
    screenRow = (z >> 16) & 0xf0;
    mark->y = screenRow - cameraZ - height + 16;
    Oam_AddSpriteAtDepth(mark, 0);
  }
}

/// Starts the fade overlay on object (the party leader when NULL): loads its
/// solid tiles, sets the blending and draws it each frame.
void FieldEffect_StartFadeOverlay(void *object) {
  struct FadeOverlayWork *work;
  u8 *tiles;
  volatile u32 fill;

  work = HeapSlot_AllocEwram(HEAP_SLOT_FADE_OVERLAY, sizeof(*work));
  // Upload the solid sprite tiles before enabling the drawing task.
  tiles = HeapSlot_Alloc(HEAP_SLOT_TILE_SCRATCH, 1024);
  Dma_Fill32Cell(fill, 0x11111111, tiles, 512);
  VramCache_Load(VRAM_CACHE_ENTRY_FADE_OVERLAY, 512, tiles);
  HeapSlot_Release(HEAP_SLOT_TILE_SCRATCH);
  Task_Add(FadeOverlay_DrawMarks, TASK_PASS_FRAME);
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

/// Stops drawing the fade overlay, keeping its work.
void FadeOverlay_Suspend(void) { Task_SuspendAll(FadeOverlay_DrawMarks); }

/// Draws the fade overlay again after FadeOverlay_Suspend.
void FadeOverlay_Resume(void) { Task_ResumeAll(FadeOverlay_DrawMarks); }
