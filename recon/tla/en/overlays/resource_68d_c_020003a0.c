// Draft, differ 4 of 126 (tla-en, 5 November 2026). Sets the blend and loads two pictures
// into VRAM for the sprites at Data_02003060 and Data_02003030, then schedules
// Func_020002b4. One untagged barrier holds the first global store behind the last
// bit-field store. Left: in the second sprite, ands r5, r2 comes after strb r3, [r6, #9] in
// the native.
#include "types.h"
#include "call.h"
#include "io_reg.h"
#include "system/io_write.h"

#define TASK_PASS_VBLANK 0x480

struct LemuriaPalaceSprite {
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
  u8 unk0a[0x14];
  u16 unk1e;
};

extern const u8 Data_02002620[];
extern const u8 Data_02002af8[];
extern s32 gData_02003020;
extern s32 gData_02003094;
extern s32 Data_02003088;
extern s32 Data_02003058;
extern struct LemuriaPalaceSprite Data_02003060;
extern struct LemuriaPalaceSprite Data_02003030;
extern s32 Data_02003090;
extern s32 Data_02003024;
extern s32 Data_02003098;
extern u8 gData_0200308c;

void *Heap_Alloc(s32 size);
void Heap_Release(void *p);
void Func_02001f6c(const u8 *source, void *destination);
s32 Resource_FindFreeEntry(void);
s32 VramCache_Load(u32 slot, u32 size, const void *source);
void Func_02002104(struct LemuriaPalaceSprite *sprite, s32 a, s32 b, u32 attr,
                   s32 tile);
s32 Task_Add(void (*func)(void), s32 priority);
void Func_020002b4(void);

void Func_020003a0(void) {
  /* FAKEMATCH(wrapper): forwarded, so that its 0x1000 goes straight to r0; as a
   * direct call, 53 of 126 instructions differ in tla-en. */
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
  *firstTile = VramCache_Load(gData_02003020, 0x1000, buffer);
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
  *secondTile = VramCache_Load(gData_02003094, 0x1000, buffer);
  Func_02002104(&Data_02003030, 0, 0, 0xc0000000, Data_02003058);
  Data_02003030.unk1e = 2;
  Data_02003030.priority = 2;
  Data_02003030.shape = 1;
  Data_02003030.blend = 1;
  Data_02003030.palette = 0;
  Data_02003090 = 0;
  __asm__("");
  Data_02003024 = 0;
  Data_02003098 = 0;
  gData_0200308c = 0;
  Task_Add(Func_020002b4, TASK_PASS_VBLANK);
  Heap_Release(buffer);
}
