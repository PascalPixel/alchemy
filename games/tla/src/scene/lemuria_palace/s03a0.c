#include "types.h"
#include "task.h"
#include "call.h"
#include "io_reg.h"
#include "system/io_write.h"
#include "event_inline.h"

struct Unk_020003a0Sprite {
  u8 unk00[5];
  u8 unk05_0 : 2;
  u8 shape : 2;
  u8 unk05_4 : 1;
  u8 blend : 1;
  u8 unk05_6 : 2;
  u8 unk06[2];
  u16 tile : 10;
  u16 priority : 2;
  u16 palette : 4;
  u16 unk0a;
  // XXX (no evidence): what these hold; read as s32, whose alias set orders
  // the stores as the listing does.
  s32 unk0c[4];
  u16 unk1c;
  u16 unk1e;
};

extern const u8 Data_02002620[];
extern const u8 Data_02002af8[];
extern s32 gData_02003020;
extern s32 gData_02003094;
extern s32 Data_02003088;
extern s32 Data_02003058;
extern struct Unk_020003a0Sprite Data_02003060;
extern struct Unk_020003a0Sprite Data_02003030;
extern s32 Data_02003090;
extern s32 Data_02003024;
extern s32 Data_02003098;
extern u8 gData_0200308c;

void *Heap_Alloc(s32 size);
void Heap_Release(void *p);
void Func_02001f6c(const u8 *source, void *destination);
s32 Resource_FindFreeEntry(void);
void Func_02002104(struct Unk_020003a0Sprite *sprite, s32 a, s32 b, u32 attr,
                   s32 tile);
void Func_020002b4(void);

void Func_020003a0(void) {
  /* FAKEMATCH(wrapper): forwarded, so that its 0x1000 goes straight to r0; as a
   * direct call, 47 of 126 instructions differ in tla-en. */
  void *buffer = (void *)Value1((s32 (*)())Heap_Alloc, 0x1000);
  s32 *firstSlot;
  s32 *firstTile;
  s32 *secondSlot;
  s32 *secondTile;

  Io_Write16(0x3f40, &REG_BLDCNT);
  Io_Write16(0x1000, &REG_BLDALPHA);
  Func_02001f6c(Data_02002620, buffer);
  firstSlot = &gData_02003020;
  *firstSlot = Resource_FindFreeEntry();
  firstTile = &Data_02003088;
  *firstTile = Vram_Load(gData_02003020, 0x1000, buffer);
  Func_02002104(&Data_02003060, 0, 0, 0xc0000000, Data_02003088);
  Data_02003060.unk1e = 1;
  Data_02003060.priority = 2;
  Data_02003060.shape = 1;
  Data_02003060.blend = 1;
  Data_02003060.palette = 0;
  Func_02001f6c(Data_02002af8, buffer);
  secondSlot = &gData_02003094;
  *secondSlot = Resource_FindFreeEntry();
  secondTile = &Data_02003058;
  *secondTile = Vram_Load(gData_02003094, 0x1000, buffer);
  Func_02002104(&Data_02003030, 0, 0, 0xc0000000, Data_02003058);
  Data_02003030.unk1e = 2;
  Data_02003030.priority = 2;
  Data_02003030.shape = 1;
  Data_02003030.blend = 1;
  Data_02003030.palette = 0;
  Data_02003090 = 0;
  Data_02003024 = 0;
  Data_02003098 = 0;
  gData_0200308c = 0;
  Task_AddFrame(Func_020002b4, TASK_PASS_VBLANK);
  Heap_Release(buffer);
}

s32 Resource_ResetEntry(u32 slot);

void Func_020004e8(void) {
  FieldTask_Remove(Func_020002b4);
  Resource_ResetEntry(gData_02003020);
  Resource_ResetEntry(gData_02003094);
}

static __inline__ void Blend_WriteAlpha(s32 value) {
  // The s32 parameter keeps each constant a word built with shifts; a u16 store
  // loads it from the literal pool.
  REG_BLDALPHA = value;
}

// Each alpha write is the macro's own once-only block; with the plain inline
// the fades differ 4 and 2.
#define BLEND_SET_ALPHA(value)                                                 \
  do {                                                                         \
    Blend_WriteAlpha(value);                                                   \
  } while (0)

/// Fades the second target out and the first in over 16 steps of four frames.
void Func_02000510(void) {
  s32 i;

  // The loop is written with goto: a for, do-while or while (1) loop differs
  // 39, as the loop pass hoists the register address and 16 out of it.
  BLEND_SET_ALPHA(0x1000);
  gData_0200308c = 0;
  i = 0;
loop:
  BLEND_SET_ALPHA(((16 - i) << 8) | i);
  System_Wait(4);
  i++;
  if (i < 16)
    goto loop;
  BLEND_SET_ALPHA(16);
  gData_0200308c = 1;
}

/// The reverse fade, in steps of three frames.
void Func_02000550(void) {
  s32 i;

  BLEND_SET_ALPHA(16);
  gData_0200308c = 0;
  i = 0;
loop:
  BLEND_SET_ALPHA((i << 8) | (16 - i));
  System_Wait(3);
  i++;
  if (i < 16)
    goto loop;
  BLEND_SET_ALPHA(0x1000);
}
