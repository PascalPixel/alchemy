// Draft, differ 233 of 439: a pillar of light rises in Air's Rock: BG0 is
// hidden while resources 0x1c0 (its tiles) and 449 (its map, the palette
// bits cleared, copied in 16 rows of 8 bytes) are loaded into VRAM; object
// 10 glows (Func_02002cc0) and sheds a spark (object type 0x2a0,
// Func_02002c40) every fourth frame for 45 frames; the camera moves to the
// light, BG0 is shown blended (cue 138), the light flashes, the cells at
// (68, 84) open, the blend fades over 32 frames, then (cue 148) the light
// reaches object 8, which flashes and leaves (Func_02001084); the display
// is put back and flag 0x899 set.
// Left: the native saves only r8 and r10 with a 12-byte frame (the fill's halfword at sp+10; the draft spills the first object to the stack), builds BG0CNT's 0x400 with movs where alchemy-gcc loads it from the pool, and keeps the frame count's low bits in r8 as the sparks' 0.
#include "types.h"
#include "object_runtime.h"
#include "party_state.h"
#include "bg_scroll.h"
#include "field_sprite.h"
#include "system/dma.h"
#include "system/io_reg.h"

extern s32 gFrameCount;
extern const u8 gData_02005fd8[];
void *MemAlloc(s32 size);
void MemFree(void *block);
void Func_02005e14(void);
void Func_02005e1c(void);
void Func_02005f2c(s32 skip);
void *Resource_GetTableEntry(s32 id);
void Func_02005d24(void *resource, void *dest);
void Func_02005f94(s32 cue);
struct ObjectRuntime *Object_GetById(u32 id);
void Func_02002cc0(void);
void Func_02002c40(void);
struct ObjectRuntime *Engine_ObjectCreate(s32 type, s32 x, s32 y, s32 z);
u32 Random16Far(void);
void Func_02005d5c(struct ObjectRuntime *object, s32 mode);
void Func_02005d6c(struct ObjectRuntime *object, const u8 *script);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object,
                                        u32 value);
void Object_SetPartAttribute(struct ObjectRuntime *object, s32 value);
void FieldEvent_WaitFrames(s32 frames);
void WaitFrames(s32 frames);
void Func_02005f0c(s32 a, s32 b);
s32 Func_02005f04(s32 a, s32 b);
s32 Func_02005f14(s32 frames);
s32 Camera_FollowActor(s32 id, s32 a);
void Func_02005ee4(void);
void Func_02005ed4(s32 a, s32 b);
void Camera_MoveWithinMapBounds(s32 x, s32 y, s32 z, s32 mode);
void Func_02005d9c(s32 x, s32 z, s32 width, s32 height, s32 a, s32 b);
void Func_02005dcc(s32 a, s32 b, s32 c);
void Func_02002aa8(s32 x, s32 z);
void Func_02005eec(s32 actor, s32 a);
void Func_02005e94(s32 actor, s32 a);
void Func_02002b58(s32 x, s32 z);
void Func_02001084(struct ObjectRuntime *object, s32 a, s32 b);
void Func_02005d34(void);
void Func_02005d2c(void);
void Func_02005e04(s32 hue, s32 brightness);
void GameFlag_SetBit(s32 flag);

void Func_02003654(void) {
  u16 *buffer = MemAlloc(0x800);
  u16 *cell;
  struct ObjectRuntime *spark;
  s32 i;

  Func_02005e14();
  Func_02005f2c(0);
  REG_DISPCNT &= ~0x100;
  REG_BG0CNT = 0x400;
  Func_02005d24(Resource_GetTableEntry(0x1c0), buffer);
  Dma_Set(buffer, (void *)0x6000000, 0x84000200, (volatile u32 *)0x040000d4);
  MemFree(buffer);
  Dma_Fill16(0, (void *)0x6002000, 0x800);
  buffer = MemAlloc(0x80);
  Func_02005d24(Resource_GetTableEntry(449), buffer);
  i = 0;
  cell = buffer;
mask:
  *cell &= 0xfcff;
  i++;
  cell++;
  if (i != 64)
    goto mask;
  cell = buffer;
  i = 0;
row:
  Dma_Set(cell, (void *)(0x6002000 + (i << 6)), 0x84000002,
          (volatile u32 *)0x040000d4);
  i++;
  cell += 4;
  if (i != 16)
    goto row;
  MemFree(buffer);
  Func_02005f94(246);
  ((u8 *)Object_GetById(10))[98] = 9;
  ((u8 *)Object_GetById(10))[99] = 1;
  Object_GetById(10)->update = Func_02002cc0;
  for (i = 0; i != 45; i++) {
    if ((gFrameCount & 3) == 0) {
      spark = Engine_ObjectCreate(0x2a0, Object_GetById(10)->x,
                                  Object_GetById(10)->y, Object_GetById(10)->z);
      ((struct FieldSprite *)spark->animation)->priority =
          ((struct FieldSprite *)Object_GetById(10)->animation)->priority;
      spark->priorityFlags |= 1;
      spark->motionFlags = 0;
      spark->bounce = spark->x;
      spark->gravity = 0x18000;
      spark->unk4c = spark->z;
      spark->speedLimit = Random16Far() & 0xffff;
      Func_02005d5c(spark, 7);
      Func_02005d6c(spark, gData_02005fd8);
      ObjectDispatch_SetSingleChildFlags(spark, 0);
      Object_SetPartAttribute(spark, 2);
      spark->update = Func_02002c40;
    }
    FieldEvent_WaitFrames(1);
  }
  Func_02005f0c(0x10000, 0);
  Func_02005f04(0x10003, 0);
  Func_02005f14(60);
  Camera_FollowActor(gPartyState.currentOwner, 1);
  Func_02005ee4();
  Func_02005ed4(0x10000, 0x2000);
  Camera_MoveWithinMapBounds(0xd80000, -1, 0xe00000, 1);
  Func_02005ee4();
  Object_GetById(10)->update = 0;
  Object_SetPartAttribute(Object_GetById(10), 6);
  REG_BLDALPHA = 0x1010;
  REG_BLDCNT = 0x3f41;
  gBgScroll[0].x = 0xff98;
  gBgScroll[0].y = 42;
  WaitFrames(30);
  Func_02005f94(138);
  Func_02005f0c(0x10003, 0);
  Func_02005f04(0x7fff, 0);
  Func_02005f14(1);
  WaitFrames(1);
  Func_02005f0c(0x7fff, 0);
  Func_02005f04(0x10003, 0);
  Func_02005f14(8);
  REG_DISPCNT |= 0x100;
  Func_02005d9c(68, 84, 12, 77, 3, 3);
  Func_02005dcc(0x50000, 0x70000, 0x10000);
  Func_02005dcc(-1, -1, 0xe666);
  i = 0;
fade:
  REG_BLDALPHA = (16 - (i >> 1)) | 0x1000;
  i++;
  WaitFrames(1);
  if (i != 32)
    goto fade;
  Func_02005f0c(0x10003, 0);
  Func_02005f04(0x10000, 0);
  Func_02005f14(60);
  WaitFrames(30);
  Func_02005f94(148);
  Func_02002aa8(0xd80000, 0x12d0000);
  Func_02005d9c(68, 84, 12, 77, 3, 6);
  Func_02005ed4(0x18000, 0x3000);
  Func_02005eec(8, 1);
  Func_02005ee4();
  Func_02005f94(146);
  Func_02005e94(8, 0x100);
  Func_02002b58(Object_GetById(8)->x, Object_GetById(8)->z + 0x200000);
  Func_02005e94(8, 0);
  Object_SetPartAttribute(Object_GetById(8), 7);
  WaitFrames(3);
  Object_SetPartAttribute(Object_GetById(8), 0);
  WaitFrames(40);
  Func_02001084(Object_GetById(8), 8, 0);
  Func_02005d34();
  Func_02005d2c();
  Func_02005e04(gPartyState.paletteGlow[0], gPartyState.paletteGlow[1]);
  REG_BLDALPHA = 0x1008;
  REG_BLDCNT = 0x3f10;
  REG_DISPCNT |= 0x100;
  gBgScroll[0].x = 0;
  gBgScroll[0].y = 0;
  GameFlag_SetBit(0x899);
  Func_02005e1c();
}
