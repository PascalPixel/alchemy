// Draft, differ 253 of 520: the pillar of light in Air's Rock, overlay 663's
// copy of overlay 664's Func_02003654 with object 14 glowing: the camera
// moves to (0x218, 0xd8), the leader turns, and the light flashes twice
// (cue 138), each time copying cells into the map (Func_02003b68 at
// (52, 12) and (52, 22)) and lighting a spot (Func_02001dc4), then (cue
// 145) three more spots light and the cells at (62, 2) open; sets flag
// 0x87b.
// Left: as overlay 664's Func_02003654 draft: the native's 12-byte frame,
// BG0CNT's 0x400 built with movs, the frame count's low bits kept as the
// sparks' 0, and the fades' counter kept in r7 across the later calls.
#include "types.h"
#include "object_runtime.h"
#include "party_state.h"
#include "bg_scroll.h"
#include "field_sprite.h"
#include "system/dma.h"
#include "system/io_reg.h"

extern s32 gFrameCount;
extern const u8 gData_02003d1c[];
void *MemAlloc(s32 size);
void MemFree(void *block);
void Func_02003bc8(void);
void Func_02003bd0(void);
void Func_02003c88(s32 skip);
void *Resource_GetTableEntry(s32 id);
void Func_02003af0(void *resource, void *dest);
void Func_02003cd8(s32 cue);
struct ObjectRuntime *Object_GetById(u32 id);
void Func_0200092c(void);
void Func_020008ac(void);
struct ObjectRuntime *Engine_ObjectCreate(s32 type, s32 x, s32 y, s32 z);
u32 Random16Far(void);
void Func_02003b28(struct ObjectRuntime *object, s32 mode);
void Func_02003b38(struct ObjectRuntime *object, const u8 *script);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object,
                                        u32 value);
void Object_SetPartAttribute(struct ObjectRuntime *object, s32 value);
void FieldEvent_WaitFrames(s32 frames);
void WaitFrames(s32 frames);
void Func_02003c58(s32 a, s32 b);
s32 Func_02003c50(s32 a, s32 b);
s32 Func_02003c60(s32 frames);
s32 Camera_FollowActor(s32 id, s32 a);
void Func_02003c30(void);
void Func_02003c20(s32 a, s32 b);
void Camera_MoveWithinMapBounds(s32 x, s32 y, s32 z, s32 mode);
void Func_02005dcc(s32 a, s32 b, s32 c);
void Func_02001dc4(s32 x, s32 z);
void Func_02003b68(s32 x, s32 z, s32 width, s32 height, s32 a, s32 b);
void Func_02003b80(s32 x, s32 z, s32 a, s32 b, s32 width, s32 height);
void Func_02003b00(void);
void Func_02003af8(void);
void Func_02003bb8(s32 hue, s32 brightness);
void GameFlag_SetBit(s32 flag);

void Func_02001f7c(void) {
  u16 *buffer = MemAlloc(0x800);
  u16 *cell;
  struct ObjectRuntime *spark;
  s32 i;
  u32 u;

  Func_02003bc8();
  Func_02003c88(0);
  REG_DISPCNT &= ~0x100;
  REG_BG0CNT = 0x400;
  Func_02003af0(Resource_GetTableEntry(0x1c0), buffer);
  Dma_Set(buffer, (void *)0x6000000, 0x84000200, (volatile u32 *)0x040000d4);
  MemFree(buffer);
  Dma_Fill16(0, (void *)0x6002000, 0x800);
  buffer = MemAlloc(0x80);
  Func_02003af0(Resource_GetTableEntry(449), buffer);
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
  Func_02003cd8(246);
  ((u8 *)Object_GetById(14))[98] = 9;
  ((u8 *)Object_GetById(14))[99] = 1;
  Object_GetById(14)->update = Func_0200092c;
  for (i = 0; i != 45; i++) {
    if ((gFrameCount & 3) == 0) {
      spark = Engine_ObjectCreate(0x2a0, Object_GetById(14)->x,
                                  Object_GetById(14)->y, Object_GetById(14)->z);
      ((struct FieldSprite *)spark->animation)->priority =
          ((struct FieldSprite *)Object_GetById(14)->animation)->priority;
      spark->priorityFlags |= 1;
      spark->motionFlags = 0;
      spark->bounce = spark->x;
      spark->gravity = 0x18000;
      spark->unk4c = spark->z;
      spark->speedLimit = Random16Far() & 0xffff;
      Func_02003b28(spark, 7);
      Func_02003b38(spark, gData_02003d1c);
      ObjectDispatch_SetSingleChildFlags(spark, 0);
      Object_SetPartAttribute(spark, 2);
      spark->update = Func_020008ac;
    }
    FieldEvent_WaitFrames(1);
  }
  Func_02003c58(0x10000, 0);
  Func_02003c50(0x10003, 0);
  Func_02003c60(60);
  Camera_FollowActor(gPartyState.currentOwner, 1);
  Func_02003c30();
  Func_02003c20(0x10000, 0x2000);
  Camera_MoveWithinMapBounds(0x2180000, -1, 0xd80000, 1);
  Func_02003c30();
  Object_GetById(14)->update = 0;
  Object_SetPartAttribute(Object_GetById(14), 6);
  REG_BLDALPHA = 0x1010;
  REG_BLDCNT = 0x3f41;
  gBgScroll[0].x = 0xff88;
  gBgScroll[0].y = 0x10;
  WaitFrames(30);
  Func_02003cd8(138);
  Func_02003c58(0x10003, 0);
  Func_02003c50(0x7fff, 0);
  Func_02003c60(1);
  Object_GetById(gPartyState.currentOwner)->angle = 0x4000;
  WaitFrames(1);
  Func_02003c58(0x7fff, 0);
  Func_02003c50(0x10003, 0);
  Func_02003c60(8);
  REG_DISPCNT |= 0x100;
  Func_02003b68(52, 12, 29, 9, 9, 9);
  Func_02003b68(52, 76, 29, 73, 9, 9);
  Func_02001dc4(0x2280000, 0xe40000);
  u = 0;
fade1:
  REG_BLDALPHA = (16 - (u >> 1)) | 0x1000;
  u++;
  WaitFrames(1);
  if (u != 32)
    goto fade1;
  gBgScroll[0].x = 0xffac;
  gBgScroll[0].y = 0;
  WaitFrames(1);
  Func_02003cd8(138);
  Func_02003c58(0x10003, 0);
  Func_02003c50(0x7fff, 0);
  Func_02003c60(1);
  WaitFrames(1);
  Func_02003c58(0x7fff, 0);
  Func_02003c50(0x10003, 0);
  Func_02003c60(8);
  REG_DISPCNT |= 0x100;
  Func_02003b68(52, 22, 29, 9, 9, 9);
  Func_02003b68(52, 86, 29, 73, 9, 9);
  Func_02001dc4(0x2080000, 0xf20000);
  u = 0;
fade2:
  REG_BLDALPHA = (16 - (u >> 1)) | 0x1000;
  u++;
  WaitFrames(1);
  if (u != 32)
    goto fade2;
  Func_02003cd8(145);
  Func_02003c58(0x10003, 0);
  Func_02003c50(0x7fff, 0);
  Func_02003c60(1);
  REG_DISPCNT &= ~0x100;
  FieldEvent_WaitFrames(20);
  Func_02001dc4(0x2000000, 0xc60000);
  Func_02001dc4(0x2320000, 0xf20000);
  Func_02001dc4(0x2090000, 0xdf0000);
  Func_02003b68(62, 2, 29, 9, 9, 9);
  Func_02003b80(62, 2, 9, 9, 29, 9);
  Func_02003b68(62, 66, 29, 73, 9, 9);
  Func_02003c58(0x7fff, 0);
  Func_02003c50(0x10003, 0);
  Func_02003c60(60);
  WaitFrames(60);
  Func_02003c58(0x10003, 0);
  Func_02003c50(0x10000, 0);
  Func_02003c60(60);
  WaitFrames(70);
  Func_02003b00();
  Func_02003af8();
  Func_02003bb8(gPartyState.paletteGlow[0], gPartyState.paletteGlow[1]);
  REG_BLDCNT = 0;
  REG_DISPCNT |= 0x100;
  gBgScroll[0].x = 0;
  gBgScroll[0].y = 0;
  GameFlag_SetBit(0x87b);
  Func_02003bd0();
}
