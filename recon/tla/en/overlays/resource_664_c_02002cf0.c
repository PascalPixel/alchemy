// Draft, differ 213 of 446: overlay 664's second pillar of light, Func_02003654
// with object 11 glowing, the camera at (0x138, 0x2a8), the cells at
// (0x40, 78) opening and the light reaching object 10 (objects 8 and 9
// lit with it); sets flag 0x89a.
// Left: as Func_02003654's draft: the native saves only r8 and r10 with a 12-byte frame, builds BG0CNT's 0x400 with movs, and keeps the frame count's low bits in r8 as the sparks' 0.
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

void Func_02002cf0(void) {
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
  ((u8 *)Object_GetById(11))[98] = 9;
  ((u8 *)Object_GetById(11))[99] = 1;
  Object_GetById(11)->update = Func_02002cc0;
  for (i = 0; i != 45; i++) {
    if ((gFrameCount & 3) == 0) {
      spark = Engine_ObjectCreate(0x2a0, Object_GetById(11)->x,
                                  Object_GetById(11)->y, Object_GetById(11)->z);
      ((struct FieldSprite *)spark->animation)->priority =
          ((struct FieldSprite *)Object_GetById(11)->animation)->priority;
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
  Camera_MoveWithinMapBounds(0x1380000, -1, 0x2a80000, 1);
  Func_02005ee4();
  Object_GetById(11)->update = 0;
  Object_SetPartAttribute(Object_GetById(11), 6);
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
  Func_02005d9c(0x40, 78, 18, 105, 3, 3);
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
  Func_02002aa8(0x1380000, 0x2d00000);
  Func_02005d9c(0x40, 78, 18, 105, 3, 5);
  Func_02005ed4(0x18000, 0x3000);
  Func_02005eec(10, 1);
  Func_02005ee4();
  Func_02005f94(146);
  Func_02005e94(10, 0x100);
  Func_02002b58(Object_GetById(10)->x, Object_GetById(10)->z);
  Func_02005e94(10, 0);
  Object_SetPartAttribute(Object_GetById(10), 7);
  WaitFrames(3);
  Object_SetPartAttribute(Object_GetById(8), 0);
  Object_SetPartAttribute(Object_GetById(9), 0);
  Object_SetPartAttribute(Object_GetById(10), 0);
  WaitFrames(40);
  Func_02001084(Object_GetById(10), 8, 0);
  Func_02005d34();
  Func_02005d2c();
  Func_02005e04(gPartyState.paletteGlow[0], gPartyState.paletteGlow[1]);
  REG_BLDALPHA = 0x1008;
  REG_BLDCNT = 0x3f10;
  REG_DISPCNT |= 0x100;
  gBgScroll[0].x = 0;
  gBgScroll[0].y = 0;
  GameFlag_SetBit(0x89a);
  Func_02005e1c();
}
