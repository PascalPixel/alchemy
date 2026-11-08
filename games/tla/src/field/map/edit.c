#include "types.h"
#include "ram_buffer.h"
#include "map.h"
#include "mapcopy.h"
#include "game_flags.h"
#include "resource.h"
#include "main_routine_work.h"

extern u8 gMapShapeGrid[];

/// Copies a rectangle of map cells as they are, with their shape grid bytes,
/// from (srcX, srcY) to (dstX, dstY), width by height cells.
void Func_0802b590(s32 srcX, s32 srcY, s32 width, s32 height, s32 dstX,
                   s32 dstY) {
  u32 *cells = (u32 *)Ram_MapCellBuffer;
  s32 dst = (dstY << 7) + dstX;
  u8 *dstShapes = gMapShapeGrid + dst;
  s32 src = (srcY << 7) + srcX;
  u8 *srcShapes = gMapShapeGrid + src;
  u32 *dstCells = cells + dst;
  u16 row;
  u16 col;

  cells += src;
  for (row = 0; row < height; row++) {
    u32 *dstCell = dstCells + (row << 7);
    u32 *srcCell = cells + (row << 7);
    u8 *dstShape = dstShapes + (row << 7);
    u8 *srcShape = srcShapes + (row << 7);

    for (col = 0; col < width; col++) {
      *dstCell++ = *srcCell++;
      *dstShape++ = *srcShape++;
    }
  }
}

void Map_SetShownLayers(u32 v) {
  struct MapWork *state = Ram_HeapSlots->mapWork;
  u32 mask = v & 0xe00;
  u32 flags = state->flags;

  flags = (flags & 0xf1ff) | mask;
  state->flags = flags;
}

extern u8 gData_0202e000[];
extern u8 gData_0202e002[];
u32 Map_ReadEditU16(u8 **cursor);

void Map_ApplySetFlagEdits(void) {
  u8 *cursor = gData_0202e002;
  u32 entry;
  s32 srcX, srcY, dstX, dstY, width, height;

  cursor = gData_0202e000 + (u16)Map_ReadEditU16(&cursor);
next:
  entry = (u16)Map_ReadEditU16(&cursor);
  if (((entry + 1) & 0xffff) == 0)
    return;
  if (GameFlag_Test(entry & 0xfff) != 0) {
    srcX = *cursor++;
    srcY = *cursor++;
    dstX = *cursor++;
    dstY = *cursor++;
    width = *cursor++;
    height = *cursor++;
    if (entry & 0x1000)
      Map_CopyCellAttributeRect(srcX, srcY, width, height, dstX, dstY);
    else
      Map_CopyCellPictureIndicesRect(srcX, srcY, dstX, dstY, width, height);
  } else {
    cursor += 6;
  }
  goto next;
}

u32 Map_ReadEditU16(u8 **cursor) {
  u32 low = *(*cursor)++;
  u32 high = *(*cursor)++;

  return low | (high << 8);
}

void Map_ApplyFlagEdits(s32 flag, s32 attributes);

void Map_SetFlagAndApplyEdits(s32 flag) {
  flag &= 0xfff;
  GameFlag_SetBit(flag);
  Map_ApplyFlagEdits(flag, 0);
}

void Map_ApplyFlagEdits(s32 flag, s32 attributes);

void Map_SetFlagAndApplyAttributeEdits(s32 flag) {
  flag &= 0xfff;
  GameFlag_SetBit(flag);
  Map_ApplyFlagEdits(flag, 1);
}

extern u8 gData_0202e000[];
extern u8 gData_0202e002[];
u32 Map_ReadEditU16(u8 **cursor);

void Map_ApplyFlagEdits(s32 flag, s32 attributes) {
  u8 *cursor = gData_0202e002;
  u32 entry;
  s32 srcX, srcY, dstX, dstY, width, height;

  cursor = gData_0202e000 + (u16)Map_ReadEditU16(&cursor);
next:
  entry = (u16)Map_ReadEditU16(&cursor);
  if (((entry + 1) & 0xffff) == 0)
    return;
  if ((entry & 0xfff) == flag) {
    srcX = *cursor++;
    srcY = *cursor++;
    dstX = *cursor++;
    dstY = *cursor++;
    width = *cursor++;
    height = *cursor++;
    if (attributes != 0)
      Map_CopyCellAttributeRect(srcX, srcY, width, height, dstX, dstY);
    else
      Map_CopyCellPictureIndicesRect(srcX, srcY, dstX, dstY, width, height);
  } else {
    cursor += 6;
  }
  goto next;
}

extern const u16 gData_0802f380[][6];
extern char ResourceId_Map000;

void Map_GetHeaderSize(s32 map, s32 *size) {
  u8 *header;

  // Indexed as gData_0802f380[map][0], the load reads [table, offset].
  header = Resource_GetTableEntry(
      *(const u16 *)(map * 12 + (u32)gData_0802f380) + (u32)&ResourceId_Map000);
  size[0] = header[0];
  if (header[2] == 0)
    size[1] = 256;
  else
    size[1] = header[2];
  size[2] = header[1];
  if (header[3] == 0)
    size[3] = 256;
  else
    size[3] = header[3];
}

struct MapHeader {
  u8 unk00[44];
  s32 cells;
  s32 shapes;
};
void Tilemap_ConvertBuffer(void);

void Map_LoadCellsAndShapes(void) {
  u8 *source;
  struct MapHeader *header = Resource_GetTableEntry(
      *(const u16 *)(gMainRoutineWork.map * 12 + (u32)gData_0802f380) +
      (u32)&ResourceId_Map000);

  source = (u8 *)header + header->cells;
  Resource_DecodeType01(source, Ram_MapCellBuffer);
  Tilemap_ConvertBuffer();
  source = (u8 *)header + header->shapes;
  Resource_DecodeType01(source, gMapShapeGrid);
}
