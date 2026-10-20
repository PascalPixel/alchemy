/// @file
/// The fade overlay drawn on the ground beside an actor.
/// Part of object.c, the field object, compiled only through it.

#include "../object_internal.h"

#ifndef UBFIX
/// The map work in heap slot 8, seen as the halfwords of its view position:
/// the high halves are the view's x and z in whole pixels.
struct FadeView {
  u8 unk00[228];
  s16 camera[4];
};
#endif

/// Each frame of the fade overlay: on each of the two cells beside the actor
/// (left and right of it), where the ground one or two cells south rises above
/// the actor's floor, draws a semi-transparent 16-pixel-wide sprite of colour 1
/// at the height of that ground. Registered as a scheduler callback by
/// BattleFx_StartFadeOverlay; reads the work in heap slot 36 and queues the
/// marks with Runtime_PushSlotEntry.
void FadeOverlay_DrawMarks(void) {
  struct FadeOverlayWork *work = WORK_SLOT(36);
#ifdef UBFIX
  struct MapScrollWork *map = WORK_SLOT(8);
  s32 cameraX = map->viewX >> 16;
  s32 cameraZ = map->viewY >> 16;
#else
  // UB: the legacy halfword overlay aliases the map's s32 coordinates.
  // Named-member high halves keep 0x1da bytes but change load scheduling
  // from .text+0x6cce in English; the actor's named fields match plainly.
  s16 *camera = ((struct FadeView *)WORK_SLOT(8))->camera;
  s32 cameraX = camera[1];
  s32 cameraZ = camera[3];
#endif
  struct AnimationSpritePart *mark = work->marks;
  struct FieldActor *actor = work->actor;
  s32 x;
  s32 z;
  s32 terrainFloor;
  s32 terrainLayer;
  u32 tile;
  s32 height;
  s32 secondHeight;
  s32 screenRow;
  /* FAKEMATCH(dead): four frame bytes nothing reads or writes, which make the
   * native 16-byte frame; without them, 4 of 200 instructions differ in
   * tbs-en. */
  UNUSED s16 unused[2];

  if (actor == 0)
    return;
  x = actor->x.fixed;
  z = actor->z.fixed;
  terrainFloor = actor->terrainHeight >> 16;
  terrainLayer = actor->terrainId;
  // The first tile of the solid block BattleFx_StartFadeOverlay loaded (VRAM
  // cache block 94; tiles are 32 bytes).
  tile = gVramBlockCache[94].offset >> 5;
  // The left cell, 8 pixels left of the actor. The ground is read one and two
  // cells (16 and 32 pixels) south, the further one counting 16 pixels lower,
  // and the higher of the two is used.
  x -= 0x80000;
  height = Map_GetTerrainHeightFar(terrainLayer, x, z + 0x100000) >> 16;
  secondHeight =
      (Map_GetTerrainHeightFar(terrainLayer, x, z + 0x200000) >> 16) - 16;
  if (secondHeight > height)
    height = secondHeight;
  if (height > 0 && height > terrainFloor) {
    // Attributes 0 and 1 give a 16 by 16 square (size 1); the object mode
    // and priority set next replace what the two words put there, so the
    // left and right marks end up alike although their third words differ.
    ((u32 *)mark)[1] = 0x40000800;
    ((u32 *)mark)[2] = 0x400;
    mark->priority = 0;
    mark->tile = tile;
    mark->objectMode = 1;
    // Snapped to the cell's 16-pixel grid, then moved up by the ground's
    // height.
    mark->x = ((u32)(x >> 16) & 0xfff0) - cameraX;
    screenRow = (z >> 16) & 0xf0;
    mark->y = screenRow - cameraZ - height + 16;
    Runtime_PushSlotEntry(mark, 0);
  }
  // The right cell, 8 pixels right of the actor, read the same way.
  x += 0x100000;
  height = Map_GetTerrainHeightFar(terrainLayer, x, z + 0x100000) >> 16;
  mark = &work->marks[1];
  secondHeight =
      (Map_GetTerrainHeightFar(terrainLayer, x, z + 0x200000) >> 16) - 16;
  if (secondHeight > height)
    height = secondHeight;
  if (height > 0 && height > terrainFloor) {
    ((u32 *)mark)[1] = 0x40000800;
    ((u32 *)mark)[2] = 0;
    mark->priority = 0;
    mark->tile = tile;
    mark->objectMode = 1;
    mark->x = ((u32)(x >> 16) & 0xfff0) - cameraX;
    screenRow = (z >> 16) & 0xf0;
    mark->y = screenRow - cameraZ - height + 16;
    Runtime_PushSlotEntry(mark, 0);
  }
}

/// Starts the fade overlay on object (the party leader when NULL): loads its
/// solid tiles, sets the blending and draws it each frame. Allocates heap slot
/// 36 for its work, borrows heap block 14 for the tiles (0x200 bytes of colour
/// 1, VRAM cache block 94) and registers FadeOverlay_DrawMarks. Called by
/// seven scene overlays (Venus and Mercury Lighthouses, Kalay, the tunnel
/// ruins, Crossbone Isle, the ice room by Imil and Fuchin Falls); Crossbone
/// Isle then switches it on and off by setting the work's actor.
void BattleFx_StartFadeOverlay(void *object) {
  struct FadeOverlayWork *work;
  u8 *tiles;
  volatile u32 fill;

  work = Runtime_AllocateBlock(36, sizeof(*work));
  // Upload the solid sprite tiles before enabling the drawing callback.
  tiles = Runtime_AllocateHeapBlock(14, 0x400);
  Dma_Fill32Cell(fill, 0x11111111, tiles, 0x200);
  VramCache_Load(94, 0x200, tiles);
  Runtime_ReleaseHeapBlock(14);
  Scheduler_AddOrUpdateCallback(FadeOverlay_DrawMarks, 0xc80);
  Io_Write16(BLDCNT_TGT1_BG1 | BLDCNT_TGT1_BG2 | BLDCNT_TGT1_BG3 |
                 BLDCNT_TGT1_OBJ | BLDCNT_EFFECT_LIGHTEN | BLDCNT_TGT2_ALL,
             &REG_BLDCNT);
  Io_Write16(BLEND_ALPHA(16, 0), &REG_BLDALPHA);
  // 31 is above the register's useful maximum of 16, which it acts as.
  Io_Write16(31, &REG_BLDY);
  Dma_Fill32Cell(fill, 0, work, sizeof(*work));
  if (object == NULL)
    object = ObjectTable_Get(gGameState.selectedActor);
  work->actor = object;
}

/// Stops drawing the fade overlay, keeping its work. Reached only through a
/// bare far-call veneer; no ☀️ code names it.
void FadeOverlay_Suspend(void) {
  Scheduler_SuspendTasks(FadeOverlay_DrawMarks);
}

/// Draws the fade overlay again after FadeOverlay_Suspend. Reached only
/// through a bare far-call veneer.
void FadeOverlay_Resume(void) { Scheduler_ResumeTasks(FadeOverlay_DrawMarks); }
