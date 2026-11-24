/// @file
/// The ground mask: raised ground in front of an actor hides the part of it
/// behind, as a ledge in the Kalay Tunnel's treasure room hides Isaac's legs.
// Part of field_sequence.c.

#include "field_sequence_internal.h"

/// A screen row's map cell, in pixels, in an OAM entry's 8-bit y, as
/// MAPGRID_CELL_PIXEL_MASK takes a whole pixel coordinate's.
#define CELL_GRID_ROW_MASK 0xf0

/// Each frame of the ground mask: on each of the two cells beside the actor
/// (left and right of it), where the ground one or two cells south rises above
/// the actor's floor, draws a semi-transparent 16-pixel-wide sprite of colour 1
/// at the height of that ground, which hides what of the actor is below that
/// ground's edge.
void GroundMask_DrawMarks(void) {
  struct GroundMaskState *state = HEAP_SLOT_BLOCK(HEAP_SLOT_GROUND_MASK);
  struct MapGridScrollState *grid = gMapGridState[0];
  s32 *view = grid->view;
  s32 cameraX = (s16)FIXEDtoINT(view[0]);
  s32 cameraZ = (s16)FIXEDtoINT(view[1]);
  struct AnimationSpritePart *mark = state->marks;
  struct Actor *actor = state->actor;
  FIXED x;
  FIXED z;
  s32 terrainFloor;
  s32 terrainLayer;
  u32 tile;
  s32 height;
  s32 secondHeight;
  s32 screenRow;
  /* LEFTOVER(frame): an unused local the ROM's frame reserves and never
   * touches. */
  UNUSED s16 unused[2];

  if (actor == 0)
    return;
  x = actor->x.fixed;
  z = actor->z.fixed;
  terrainFloor = FIXEDtoINT(actor->groundHeight);
  terrainLayer = actor->layer;
  // The first tile of the solid block FieldEffect_StartGroundMask loaded.
  tile =
      gVramCacheEntries[VRAM_CACHE_ENTRY_GROUND_MASK].offset / TILE_SIZE_4BPP;
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
    Oam_AddSpriteAtDepth(mark, 0);
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
    Oam_AddSpriteAtDepth(mark, 0);
  }
}

/// Starts the ground mask on object (the party leader when NULL): loads its
/// solid tiles, sets the blending and draws it each frame. Kalay's tunnel,
/// Mercury and Venus Lighthouse, the Fuchin Falls cave, the Tunnel Ruins and
/// Crossbone Isle start it.
void FieldEffect_StartGroundMask(void *object) {
  struct GroundMaskState *state;
  u8 *tiles;
  volatile u32 fill;

  state = HeapSlot_AllocEwram(HEAP_SLOT_GROUND_MASK, sizeof(*state));
  // Upload the solid sprite tiles before enabling the drawing task.
  tiles = HeapSlot_Alloc(HEAP_SLOT_TILE_SCRATCH, 1024);
  Dma_Fill32Cell(fill, PIXELS_4BPP(1), tiles, 512);
  VramCache_Load(VRAM_CACHE_ENTRY_GROUND_MASK, 512, tiles);
  HeapSlot_Release(HEAP_SLOT_TILE_SCRATCH);
  Task_Add(GroundMask_DrawMarks, TASK_PASS_FRAME);
  Io_Write16(BLDCNT_TGT1_BG1 | BLDCNT_TGT1_BG2 | BLDCNT_TGT1_BG3 |
                 BLDCNT_TGT1_OBJ | BLDCNT_EFFECT_LIGHTEN | BLDCNT_TGT2_ALL,
             &REG_BLDCNT);
  Io_Write16(BLEND_ALPHA(16, 0), &REG_BLDALPHA);
  // 31 is above the register's useful maximum of 16, which it acts as.
  Io_Write16(31, &REG_BLDY);
  Dma_Fill32Cell(fill, 0, state, sizeof(*state));
  if (object == NULL)
    object = Actor_Lookup(gMainRoutineState.leaderActorId);
  state->actor = object;
}

/// Stops drawing the ground mask, keeping its state.
void GroundMask_Suspend(void) { Task_Suspend(GroundMask_DrawMarks); }

/// Draws the ground mask again after GroundMask_Suspend.
void GroundMask_Resume(void) { Task_Resume(GroundMask_DrawMarks); }

#undef CELL_GRID_ROW_MASK
