// Draft, differ 243 of 190: draws the Gabomba Statue grid's movable pieces
// (objects from gSceneState's halfword 0x164 up to 0x166): each live piece
// on screen gets an OAM entry at gSceneState + 0x178 on, its picture by the
// kind of the grid cell under it (four bytes a cell, 11 cells a row from
// the origin at 0x16c), and, while the rotation at 0x174 is set, the
// rotation matrix built from it; Func_020055ec draws it at its depth.
// Left: a rough reading: the native's loop is entered at its test, keeps the OAM cursor in r8, the scroll in r9 and r10 and the tile in r11, reloads the rotation for each piece, and orders the screen position's arithmetic otherwise; the alignment is poor.
#include "types.h"
#include "actor.h"
#include "ram_buffer.h"
#include "graphics/vram_tab.h"

extern u8 gSceneState[];
struct Actor *Actor_GetById(u32 id);
s32 AffineMatrix_BuildForEffect(u32 *matrix);
void Func_020055ec(u32 *oam, s32 depth);

void Func_02004710(void) {
  u8 *state = gSceneState;
  u8 *map = (u8 *)Ram_HeapSlots->mapWork;
  u32 *oam = (u32 *)(state + 0x178);
  s32 scrollX = *(s32 *)(map + 0xe4) & 0xffff0000;
  s32 scrollZ = *(s32 *)(map + 0xe8) & 0xffff0000;
  u32 tile = gResourceTableEntries[*(s16 *)(state + 0x168)].offset >> 5;
  struct Actor *piece;
  u32 matrix[2];
  s32 affine = 0, id, x, y, z, depth, cellX, cellZ, kind;

  if (*(s32 *)(state + 0x174) != 0) {
    matrix[1] = (matrix[1] & 0xffff0000) | (u16) * (s32 *)(state + 0x174);
    matrix[0] = (((matrix[0] & 0xffff0000) | 0x100) & 0xffff) | 0x1000000;
    affine = AffineMatrix_BuildForEffect(matrix);
  }
  for (id = *(s16 *)(state + 0x164); id < *(s16 *)(state + 0x166); id++) {
    piece = Actor_GetById(id);
    if (piece->x == 0)
      continue;
    x = (piece->x - scrollX) >> 16;
    z = piece->z - scrollZ;
    y = piece->y + 0x100000;
    depth = ((y + z) >> 16) + 58;
    cellX = (piece->x >> 20) - *(s16 *)(state + 0x16c);
    cellZ = (piece->z >> 20) - *(s16 *)(state + 0x16e);
    kind = state[(cellZ * 11 + cellX) * 4];
    oam[0] = 0;
    oam[1] = (((z - y) >> 16) - 8 & 255) | (((x - 8) & 0x1ff) << 16) |
             0x40002000;
    oam[2] = (tile + (kind << 3)) | 0x800;
    if (*(s32 *)(state + 0x174) != 0) {
      ((u8 *)oam)[5] = (((u8 *)oam)[5] & ~3) | 1;
      ((u8 *)oam)[7] = (((u8 *)oam)[7] & -63) | ((affine & 31) << 1);
    }
    Func_020055ec(oam, depth);
    oam += 3;
  }
}
