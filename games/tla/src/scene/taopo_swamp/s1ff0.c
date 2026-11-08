#include "types.h"
#include "call.h"
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
void wait(s32 frames);

void Func_02001ff0(s32 x, s32 z) {
  u8 *map = (u8 *)Ram_HeapSlots->mapWork;
  struct MapFocus *focus;
  s32 index, fx = x << 19;

  focus = (struct MapFocus *)(map + 0x178);
  focus->x = fx;
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
  focus->scaleZ = focus->scaleX = 0x10000;
  focus->cell = gMapCellBuffer + index * 4;
  focus->shape = gMapShapeGrid + index;
  focus->scrolledX = Iwram_MulQ16(*(s32 *)(map + 0xe4), 0x10000) + focus->x;
  focus->scrolledZ =
      Iwram_MulQ16(*(s32 *)(map + 0xe8), focus->scaleZ) + focus->z;
  Func_02002810();
  wait(1);
}
