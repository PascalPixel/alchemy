// Draft, differ 59 of 69: the epilogue's setup: loads its palette and tiles,
// sets up its sprite and schedules Func_0200005c. Left: the native loads the
// resource id 0x1f8 and the stored zero from the pool (as reload does for a
// spilled constant), keeps the buffer in r8 and the sprite's address in r6 from
// before the first resource call, and builds 0x800 again for VramCache_Load;
// here the buffer is in r6 and 0x800 is shared. The resource id as u16 changes
// nothing.
#include "types.h"
#include "dma.h"
#include "io_reg.h"

struct EpilogueSprite {
  u8 unk00[5];
  u8 unk05_0 : 5;
  u8 unk05_5 : 1;
  u8 unk05_6 : 2;
  u8 unk06[3];
  u8 unk09_0 : 2;
  u8 unk09_2 : 2;
  u8 unk09_4 : 4;
  u8 unk0a[0x14];
  u16 unk1e;
};

extern const u16 Data_02000874[];
extern s32 Data_020008f0;
extern s32 Data_02000a30;
extern struct EpilogueSprite gData_02000a00;
extern s8 gData_02000a34;

void *MemAlloc(s32 size);
void MemFree(void *block);
void *Resource_GetTableEntry(u16 id);
s32 Resource_FindFreeEntry(void);
s32 VramCache_Load(s32 entry, s32 size, void *data);
void Task_Add(void (*callback)(void), s32 priority);
void Func_020007ac(void *resource, void *destination);
void Func_02000834(struct EpilogueSprite *sprite, s32 a, s32 b, u32 c,
                   s32 tile);
void Func_0200005c(void);

void Func_02000098(void) {
  void *buffer = MemAlloc(0x800);
  s32 entry;
  s32 tile;

  Dma_Set(Data_02000874, (void *)0x050003c0, 0x80000010, REG_DMA3);
  Func_020007ac(Resource_GetTableEntry(0x1f8), buffer);
  entry = Resource_FindFreeEntry();
  Data_020008f0 = entry;
  tile = VramCache_Load(entry, 0x800, buffer);
  Data_02000a30 = tile;
  Func_02000834(&gData_02000a00, 0, 0, 0xc0000000, tile);
  gData_02000a00.unk1e = 240;
  gData_02000a00.unk09_2 = 0;
  gData_02000a00.unk05_5 = 0;
  gData_02000a00.unk09_4 = 14;
  MemFree(buffer);
  gData_02000a34 = 0;
  Task_Add(Func_0200005c, 0x480);
}
