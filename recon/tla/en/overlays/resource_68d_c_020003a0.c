// Draft, differ 59 of 126, skeleton gap 19 (tla-en, 2026-11-01)
// Sets the blend and loads two pictures (Data_02002620 and Data_02002af8) into
// VRAM for the sprites at Data_02003060 and Data_02003030, then schedules
// Func_020002b4.
// Left: the 0x1000 of Heap_Alloc, REG_BLDALPHA and both VramCache_Load calls is
// one pseudo here (CSE, then a subreg for the halfword store). With 5 refs it
// outranks the 8 of the sprite's priority field and takes r8, so 8 is rebuilt
// and the native's r8/0x1000 pair is swapped: the native rebuilds 0x1000 at each
// use and keeps 8 in r8. Almost all of the skeleton gap is this one thing.
// Measured: giving Heap_Alloc and alpha other values (diagnostic only) puts 8
// in r8; forwarding Heap_Alloc through a static inline, so the argument
// goes straight to r0, measures differ 28, skeleton 11. That is a `wrapper`
// device (MemAlloc_ in the scene local.h files, Heap_CallAlternatePool), left out
// of this no-new-device round; whether it is source here is a counting decision
// (S2). Failed to unshare it: a `size` local, s32 or u16, unprototyped or u16
// parameters, sizeof, and (diagnostic only) a one-pass block around the two
// register writes.
// After that the near miss is order, seen in the forwarded form: the native has
// mov r9, r0 first after Heap_Alloc, and movs r7, #32 before Func_02001f6c.
// sched2 gives both too low a priority here. Reading the tiles and slots back
// through the locals, sprite pointers, inner blocks, declaration order,
// non-volatile writes and field order all leave it or lose (field order 79-93).
// Measured forms: an s32 control and alpha give the native's movs/lsls and
// adds r3, #2 (a HImode constant loads from the pool). The address locals give
// the native's load before each call (a plain `x = f()` loads after it, also as
// a struct field or array element), one each: sharing two by reuse is 67, 8 worse.
#include "types.h"
#include "io_reg.h"

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
  void *buffer = Heap_Alloc(0x1000);
  s32 control = 0x3f40;
  s32 alpha = 0x1000;
  s32 *firstSlot;
  s32 *firstTile;
  s32 *secondSlot;
  s32 *secondTile;

  REG_BLDCNT = control;
  REG_BLDALPHA = alpha;
  Func_02001f6c(Data_02002620, buffer);
  *firstSlot = Resource_FindFreeEntry();
  firstSlot = &gData_02003020;
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
  Data_02003024 = 0;
  Data_02003098 = 0;
  gData_0200308c = 0;
  Task_Add(Func_020002b4, TASK_PASS_VBLANK);
  Heap_Release(buffer);
}
