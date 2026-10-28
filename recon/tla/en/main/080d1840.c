// Under -mthumb-slow-loads (2026-10-26): differ 8 of 200 in tla-en, instruction
// order and register choice. No empty asm before any one statement lowers it.
// Near miss, differ 14 of 200: ☀️'s FadeOverlay_DrawMarks. The native loads
// the terrain layer byte (ldrb r3, [r3, #0]) a slot before movs r2, #189 and
// the resource table after lsls, issues mov r0, r8 for the first terrain call
// before its add, and loads attribute byte 9 before movs r1, #13 in each mark:
// loads a slot earlier than agscc's ARM9TDMI model. The unused halfwords are
// ☀️'s frame device: without them the stack frame is 12 bytes, not 16.
#include "types.h"
#include "ram_buffer.h"
#include "sprite.h"
#include "resource.h"
#include "vram_tab.h"

s32 Map_GetTerrainHeightFar(s32 layer, s32 x, s32 z);
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
  s32 terrainLayer;
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
