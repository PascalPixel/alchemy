// Near miss, differ 4 of 200 (tla-en, 2026-10-31): the actor's x load
// (ldr r7, [r3, #8]) and adds r3, #34 issue two slots late, after the
// floor's spill store (str r2, [sp, #0]): -mthumb-slow-loads makes that
// whole load wait a cycle after a store it may read. Read with x before the
// floor (this draft's earlier order or ☀️'s), x loads early but the terrain
// layer's ldrb issues before movs r2, #189 and mov r0, r8 sinks to the first
// call: differ 8. The native's ldrb waits after the store although it is
// zero-extended, which the model exempts; the layer as u8 (field.h's
// prototype) or s32, and every other type and statement order tried keep 4
// or more; two permuter runs found nothing better. No empty asm before any
// one statement lowers it.
// ☀️'s FadeOverlay_DrawMarks. The unused halfwords give the stack frame its
// 16 bytes, as in ☀️: without them it is 12.
#include "types.h"
#include "ram_buffer.h"
#include "sprite.h"
#include "resource.h"
#include "vram_tab.h"

s32 Map_GetTerrainHeightFar(u8 layer, s32 x, s32 z);
void Func_080140d8(void *entry, s32 priority);

struct FadeActor {
  u8 unk00[8];
  s32 x;
  s32 y;
  s32 z;
  u16 unk14;
  s16 floor;
  u8 unk18[10];
  u8 terrainId;
};

struct FadeOverlayWork {
  struct AnimationSpritePart marks[2];
  struct FadeActor *actor;
};

struct FadeView {
  u8 unk00[228];
  s16 camera[4];
};

// ☀️'s FadeOverlay_DrawMarks: each frame, on the two cells beside the actor
// where the ground one or two cells south rises above the actor's floor,
// draws a 16-pixel-wide mark at the height of that ground.
void Func_080d1840(void) {
  struct FadeOverlayWork *work = Ram_HeapSlots->unk84[3];
  s16 *camera = ((struct FadeView *)Ram_HeapSlots->mapWork)->camera;
  s32 cameraX = camera[1];
  s32 cameraZ = camera[3];
  struct AnimationSpritePart *mark = work->marks;
  struct FadeActor *actor = work->actor;
  s32 x;
  s32 z;
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
  terrainFloor = actor->floor;
  x = actor->x;
  terrainLayer = actor->terrainId;
  tile = gResourceTableEntries[94].offset >> 5;
  x -= 0x80000;
  height = Map_GetTerrainHeightFar(terrainLayer, x, z + 0x100000) >> 16;
  secondHeight =
      (Map_GetTerrainHeightFar(terrainLayer, x, z + 0x200000) >> 16) - 16;
  if (secondHeight > height)
    height = secondHeight;
  if (height > 0 && height > terrainFloor) {
    ((u32 *)mark)[1] = 0x40000800;
    ((u32 *)mark)[2] = 0x400;
    mark->priority = 0;
    mark->tile = tile;
    mark->objectMode = 1;
    mark->x = ((u32)(x >> 16) & 0xfff0) - cameraX;
    screenRow = (z >> 16) & 0xf0;
    mark->y = screenRow - cameraZ - height + 16;
    Func_080140d8(mark, 0);
  }
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
    Func_080140d8(mark, 0);
  }
}
