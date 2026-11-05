// Draft, differ 56 (tla-en, 2026-11-05). Func_02000098 of overlay 6b3.
// Barrier and one-pass gone (a rise from 50); pointers set before the calls,
// resource id from its table, a byte local for the zero, Value forms where
// the constant is not shared. Left: rank: buffer natively in r8 with r7
// unused, sprite r6, slot r5.
#include "types.h"
#include "call.h"
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

void *Heap_Alloc(s32 size);
void Heap_Release(void *block);
void *Resource_GetTableEntry(s32 id);
extern char ResourceId_GreenPanel;
s32 Resource_FindFreeEntry(void);
s32 VramCache_Load(s32 entry, s32 size, void *data);
void Task_Add(void (*func)(void), s32 priority);
void Func_020007ac(void *resource, void *destination);
void Func_02000834(struct EpilogueSprite *sprite, s32 a, s32 b, u32 c,
                   s32 tile);
void Func_0200005c(void);

void Func_02000098(void) {
  void *buffer = (void *)Value1((s32 (*)())Heap_Alloc, 0x800);
  struct EpilogueSprite *sprite;
  s32 *slot;
  s32 value;

  Dma_Set(Data_02000874, (void *)0x050003c0, 0x80000010, REG_DMA3);
  sprite = &gData_02000a00;
  Func_020007ac(Resource_GetTableEntry((s32)&ResourceId_GreenPanel), buffer);
  slot = &Data_020008f0;
  value = Resource_FindFreeEntry();
  *slot = value;
  slot = &Data_02000a30;
  value = Value3((s32 (*)())VramCache_Load, value, 0x800, (s32)buffer);
  *slot = value;
  Func_02000834(sprite, 0, 0, 0xc0000000, value);
  sprite->unk1e = 240;
  sprite->unk09_2 = 0;
  sprite->unk05_5 = 0;
  sprite->unk09_4 = 14;
  Heap_Release(buffer);
  {
    s8 zero = 0;

    gData_02000a34 = zero;
  }
  Task_Add(Func_0200005c, 0x480);
}
