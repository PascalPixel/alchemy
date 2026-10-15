#include "types.h"
#include "ram_buffer.h"

struct MenuRuntimeCursor {
  u8 unk00[0x24];
  s32 unk24;
  s32 unk28;
  s32 unk2c;
  s32 unk30;
  u8 unk34[0xc0];
  u8 unkf4;
  u8 unkf5;
  u8 unkf6;
  u8 unkf7;
};

s32 Func_080f88d0(void *work);
void Func_08104fe0(s32 value, s32 a, s32 b);

void Func_080fee04(void) {
  struct MenuRuntimeCursor *work = Ram_HeapSlots->menuRuntime;

  Func_08104fe0(Func_080f88d0(work), 2, 2);
  work->unk2c = 0;
  work->unk28 = 0;
  work->unk30 = 0;
  work->unk24 = 0;
  work->unkf4 = 0;
  work->unkf5 = 0;
  work->unkf6 = 8;
  work->unkf7 = 2;
}
