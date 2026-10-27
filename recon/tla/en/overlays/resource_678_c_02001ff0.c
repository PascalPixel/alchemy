// Draft, differ 26 of 66: points the map work's focus (0x178) at cell (x, z):
// its position in 16.16 pixels, the cell's halfword coordinates, its
// pointers into gMapCellBuffer and gMapShapeGrid (cells two to a step), unit
// scales and cleared motion, then its scrolled position from the map's
// scroll; redraws with Func_02002810 and waits a frame. Left: registers for the
// field offsets and the order of the zero stores against the index arithmetic.
#include "types.h"
#include "ram_buffer.h"
#include "iwram_call.h"

struct MapFocus {
  s32 scrolledX;
  s32 scrolledZ;
  s32 x;
  s32 z;
  s32 scaleX;
  s32 scaleZ;
  s32 unk18[4];
  u16 unk28;
  u16 unk2a;
  u16 cellX;
  u16 row;
  u8 *cell;
  u8 *shape;
};

extern u8 gMapCellBuffer[];
extern u8 gMapShapeGrid[];
void Func_02002810(void);
void WaitFrames(s32 frames);

void Func_02001ff0(s32 x, s32 z) {
  u8 *map = (u8 *)Ram_HeapSlots->mapWork;
  struct MapFocus *focus = (struct MapFocus *)(map + 0x178);
  s32 index;

  focus->x = x << 19;
  focus->z = z << 19;
  focus->unk28 = 127;
  focus->unk2a = 0x3f80;
  focus->row = z << 7;
  focus->cellX = x;
  focus->unk18[0] = 0;
  focus->unk18[1] = 0;
  focus->unk18[2] = 0;
  focus->unk18[3] = 0;
  index = (z / 2 << 7) + x / 2;
  focus->cell = gMapCellBuffer + index * 4;
  focus->scaleX = focus->scaleZ = 0x10000;
  focus->shape = gMapShapeGrid + index;
  focus->scrolledX = Iwram_MulQ16(*(s32 *)(map + 0xe4), 0x10000) + focus->x;
  focus->scrolledZ =
      Iwram_MulQ16(*(s32 *)(map + 0xe8), focus->scaleZ) + focus->z;
  Func_02002810();
  WaitFrames(1);
}
