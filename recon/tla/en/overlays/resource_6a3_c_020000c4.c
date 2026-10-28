// Draft, differ 38 of 281 (was 198): Jupiter Lighthouse room M's change: loads
// a picture into background 1 and blends it in over the scene with flashes
// (Func_02004924, Func_0200491c) and sound 144 while the screen shakes; object
// 4 is launched back 16 pixels, object 20 takes its update (Func_0200354c), the
// blend fades over 32 frames, the room's cells are stamped open (Func_0200474c)
// and the display is put back as it was. The resources are directory entries
// 448 and 449 (Field_LightningTiles, Field_LightningMap, unnamed in
// resource/directory.s: name them there on adoption), passed as their linker
// ids. The three loops are goto loops: the native hoists nothing out of them.
// The background control goes through Io_Write16, the fill through Dma_Fill16.
// Left: the counter's 0 is set before the tiles are freed, so its pseudo spans
// the tiles' and global.c puts it in r6 as the native does (set at the first
// loop, the counter outranks the map buffer and takes r5); set there, CSE
// stores the fill word from it, where the native loads its halfword 0 from the
// pool (38). Tried: the 0 at each statement from the start to the second
// MemAlloc (38 to 76), permute 8 minutes.
#include "../../../../games/tla/src/scene/jupiter_lighthouse_room_m/jupiter_lighthouse_room_m.h"
#include "io_reg.h"
#include "dma.h"
#include "iowrite.h"

extern u8 gCpuLoadDisplayEnabled;
extern char ResourceId_LightningTiles;
extern char ResourceId_LightningMap;
extern s16 gBgScroll[];

void *MemAlloc(s32 size);
void MemFree(void *block);
void *Resource_GetTableEntry(s32 id);
void Func_0200468c(void *resource, void *destination);
void Func_02004924(s32 a, s32 b);
void Func_0200491c(s32 a, s32 b);
void Func_0200492c(s32 frames);
void Func_020049c4(s32 sound);
void Func_0200475c(s32 a, s32 b, s32 c);
void ObjectMotion_Launch(s32 id, s32 mode, s32 value);
void ObjectMotion_OffsetPositionAndReset(s32 id, s32 dx, s32 dz);
void Func_0200474c(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_020049ac(s32 x, s32 z, s32 a, s32 b);
void Func_020046b4(void);
void Func_020046a4(void);
void Func_02004784(s32 hue, s32 brightness);
void Func_0200354c(void);

static __inline__ void Func_0200474c_(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f) {
  Func_0200474c(a, b, c, d, e, f);
}

static __inline__ void Func_0200475c_(s32 a, s32 b, s32 c) {
  Func_0200475c(a, b, c);
}

static __inline__ void Func_0200491c_(s32 a, s32 b) {
  Func_0200491c(a, b);
}

static __inline__ void Func_02004924_(s32 a, s32 b) {
  Func_02004924(a, b);
}

void Func_020000c4(void) {
  u16 *tiles;
  u16 *buffer;
  u16 *p;
  u32 i;

  gCpuLoadDisplayEnabled = 0;
  REG_DISPCNT &= ~0x100;
  Io_Write16(0x400, &REG_BG0CNT);
  tiles = MemAlloc(0x800);
  Func_0200468c(Resource_GetTableEntry((s32)&ResourceId_LightningTiles), tiles);
  Dma_Set(tiles, (void *)(0x06000000), 0x84000200, REG_DMA3);
  i = 0;
  MemFree(tiles);
  Dma_Fill16(0, (void *)0x06002000, 0x800);
  buffer = MemAlloc(128);
  Func_0200468c(Resource_GetTableEntry((s32)&ResourceId_LightningMap), buffer);
  p = buffer;
mask:
  *p++ &= 0xfcff;
  i++;
  if (i != 64)
    goto mask;
  p = buffer;
  i = 0;
copy:
  Dma_Set(p, (void *)(0x06002000 + (i << 6)), 0x84000002, REG_DMA3);
  i++;
  p += 4;
  if (i != 16)
    goto copy;
  MemFree(buffer);
  REG_BLDALPHA = 0x1010;
  REG_BLDCNT = 0x3f41;
  gBgScroll[0] = -104;
  gBgScroll[1] = 16;
  WaitFrames(30);
  Func_02004924_(0x10003, 0);
  Func_0200491c_(0x7fff, 0);
  Func_0200492c(1);
  WaitFrames(1);
  Func_02004924_(0x7fff, 0);
  Func_0200491c_(0x10003, 0);
  Func_0200492c(8);
  REG_DISPCNT |= 0x100;
  Func_020049c4(144);
  Func_0200475c_(0x50000, 0x70000, 0x10000);
  Func_0200475c_(-1, -1, 0xe666);
  ObjectMotion_SetSpeedParameters(gAdventure.currentOwner, 0x20000, 0x10000);
  ObjectMotion_Launch(4, 6, 0);
  Object_GetById(4)->actionFlags &= ~1;
  ObjectMotion_OffsetPositionAndReset(4, -16, 0);
  Object_GetById(20)->update = Func_0200354c;
  i = 0;
fade:
  REG_BLDALPHA = (16 - (i >> 1)) | 0x1000;
  i++;
  WaitFrames(1);
  if (i != 32)
    goto fade;
  Func_0200474c_(104, 42, 5, 11, 61, 42);
  Func_0200474c_(106, 52, 1, 1, 63, 84);
  Func_020049ac(130 << 19, 160 << 16, 2, 236);
  Func_02004924_(0x10003, 0);
  Func_0200491c_(0x10000, 0);
  Func_0200492c(60);
  Func_020046b4();
  Func_020046a4();
  Func_02004784(gAdventure.paletteGlow[0], gAdventure.paletteGlow[1]);
  REG_BLDALPHA = 0x1008;
  REG_BLDCNT = 0x3f10;
  REG_DISPCNT |= 0x100;
  gBgScroll[0] = 0;
  gBgScroll[1] = 0;
  WaitFrames(1);
  gCpuLoadDisplayEnabled = 1;
  Func_0200475c_(0, 0, 0);
}
