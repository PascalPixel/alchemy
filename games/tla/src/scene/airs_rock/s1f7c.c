#include "types.h"
#include "actor.h"
#include "sequence_parent_work.h"
#include "bg_scroll.h"
#include "field_sprite.h"
#include "system/dma.h"
#include "system/io_reg.h"
#include "call.h"

/// A field sprite's byte 9: the top of its first tile and its priority (OAM
/// attribute 2), read as bit-fields.
struct SpriteAttr2High {
  u8 unk00[9];
  u8 tileHigh : 2;
  u8 priority : 2;
};
/// An object's priority flags read as a whole-byte bit-field.
struct ObjectPriorityByte {
  u8 unk00[35];
  u8 priorityFlags : 8;
};
extern s32 gFrameCount;
extern char ResourceId_LightningMap;
extern char ResourceId_LightningTiles;
extern const u8 gData_02003d1c[];
void *Heap_Alloc(s32 size);
void Heap_Release(void *block);
void Func_02003bc8(void);
void Func_02003bd0(void);
void Func_02003c88(s32 skip);
void *Resource_GetTableEntry(s32 id);
void Func_02003af0(void *resource, void *dest);
void Func_02003cd8(s32 cue);
struct Actor *Actor_GetById(u32 id);
void Func_0200092c(void);
void Func_020008ac(void);
struct Actor *Import_ActorCreate(s32 type, s32 x, s32 y, s32 z);
u32 Random16Far(void);
void Func_02003b28(struct Actor *object, s32 mode);
void Func_02003b38(struct Actor *object, const u8 *script);
void Actor_SetSingleChildFlags(struct Actor *object,
                                        u32 value);
void Actor_SetPartAttribute(struct Actor *object, s32 value);
void FieldEvent_Wait(s32 frames);
void wait(s32 frames);
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

/// The pillar of light in Air's Rock, overlay 664's Func_02003654 with object
/// 14 glowing: the camera moves to (0x218, 0xd8), the leader turns, and the
/// light flashes twice (cue 138), each time copying cells into the map
/// (Func_02003b68 at (52, 12) and (52, 22)) and lighting a spot
/// (Func_02001dc4), then (cue 145) three more spots light and the cells at
/// (62, 2) open; sets flag 0x87b.
void Func_02001f7c(void) {
  u16 *buffer = Heap_Alloc(0x800);
  u16 *cell;
  struct Actor *spark;
  u32 i;

  Func_02003bc8();
  Func_02003c88(0);
  REG_DISPCNT &= ~0x100;
  /* FAKEMATCH(one-pass): BG0CNT's value in a block of its own, built before
   * its address; as a plain block, 2 of 520 instructions differ in tla-en. */
  do {
    s32 control = 0x400;

    REG_BG0CNT = control;
  } while (0);
  Func_02003af0(Resource_GetTableEntry((s32)&ResourceId_LightningTiles),
                buffer);
  Dma_Set(buffer, (void *)0x6000000, 0x84000200, (volatile u32 *)0x040000d4);
  Heap_Release(buffer);
  Dma_Fill16(0, (void *)0x6002000, 0x800);
  buffer = Heap_Alloc(0x80);
  Func_02003af0(Resource_GetTableEntry((s32)&ResourceId_LightningMap), buffer);
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
  Heap_Release(buffer);
  Func_02003cd8(246);
  ((u8 *)Actor_GetById(14))[98] = 9;
  ((u8 *)Actor_GetById(14))[99] = 1;
  Actor_GetById(14)->update = Func_0200092c;
  for (i = 0; i != 45; i++) {
    if ((gFrameCount & 3) == 0) {
      spark = Import_ActorCreate(0x2a0, Actor_GetById(14)->x,
                                  Actor_GetById(14)->y, Actor_GetById(14)->z);
      {
        s32 priority = ((s8 *)Actor_GetById(14)->animation)[9] & 12;

        /* FAKEMATCH(temporary): the sprite held in the row pointer; as
         * spark->animation, 20 of 520 instructions differ in tla-en. */
        cell = spark->animation;
        ((struct SpriteAttr2High *)cell)->priority = priority >> 2;
      }
      // Through the bit-field views the flags' load is issued before the
      // priority store, as the native does; as plain bytes it waits for it.
      ((struct ObjectPriorityByte *)spark)->priorityFlags |= 1;
      spark->motionFlags = 0;
      spark->bounce = spark->x;
      spark->gravity = 0x18000;
      spark->unk4c = spark->z;
      spark->speedLimit = Random16Far() & 0xffff;
      Func_02003b28(spark, 7);
      Func_02003b38(spark, gData_02003d1c);
      Actor_SetSingleChildFlags(spark, 0);
      Actor_SetPartAttribute(spark, 2);
      spark->update = Func_020008ac;
    }
    FieldEvent_Wait(1);
  }
  Call2((void (*)())Func_02003c58, 0x10000, 0);
  Call2((void (*)())Func_02003c50, 0x10003, 0);
  Func_02003c60(60);
  Camera_FollowActor(gSequenceParentWork.currentOwner, 1);
  Func_02003c30();
  Func_02003c20(0x10000, 0x2000);
  Camera_MoveWithinMapBounds(0x2180000, -1, 0xd80000, 1);
  Func_02003c30();
  Actor_GetById(14)->update = 0;
  Actor_SetPartAttribute(Actor_GetById(14), 6);
  REG_BLDALPHA = 0x1010;
  REG_BLDCNT = 0x3f41;
  gBgScroll[0].x = 0xff88;
  gBgScroll[0].y = 0x10;
  wait(30);
  Func_02003cd8(138);
  Call2((void (*)())Func_02003c58, 0x10003, 0);
  Call2((void (*)())Func_02003c50, 0x7fff, 0);
  Func_02003c60(1);
  Actor_GetById(gSequenceParentWork.currentOwner)->angle = 0x4000;
  wait(1);
  Func_02003c58(0x7fff, 0);
  Func_02003c50(0x10003, 0);
  Func_02003c60(8);
  REG_DISPCNT |= 0x100;
  Func_02003b68(52, 12, 29, 9, 9, 9);
  Func_02003b68(52, 76, 29, 73, 9, 9);
  Func_02001dc4(0x2280000, 0xe40000);
  i = 0;
fade1:
  REG_BLDALPHA = (16 - (i >> 1)) | 0x1000;
  wait(1);
  i++;
  if (i != 32)
    goto fade1;
  gBgScroll[0].x = 0xffac;
  gBgScroll[0].y = 0;
  wait(1);
  Func_02003cd8(138);
  Call2((void (*)())Func_02003c58, 0x10003, 0);
  Call2((void (*)())Func_02003c50, 0x7fff, 0);
  Func_02003c60(1);
  wait(1);
  Func_02003c58(0x7fff, 0);
  Func_02003c50(0x10003, 0);
  Func_02003c60(8);
  REG_DISPCNT |= 0x100;
  Func_02003b68(52, 22, 29, 9, 9, 9);
  Func_02003b68(52, 86, 29, 73, 9, 9);
  Func_02001dc4(0x2080000, 0xf20000);
  i = 0;
fade2:
  REG_BLDALPHA = (16 - (i >> 1)) | 0x1000;
  wait(1);
  i++;
  if (i != 32)
    goto fade2;
  Func_02003cd8(145);
  Call2((void (*)())Func_02003c58, 0x10003, 0);
  Call2((void (*)())Func_02003c50, 0x7fff, 0);
  Func_02003c60(1);
  REG_DISPCNT &= ~0x100;
  FieldEvent_Wait(20);
  Func_02001dc4(0x2000000, 0xc60000);
  Func_02001dc4(0x2320000, 0xf20000);
  Func_02001dc4(0x2090000, 0xdf0000);
  Func_02003b68(62, 2, 29, 9, 9, 9);
  Func_02003b80(62, 2, 9, 9, 29, 9);
  Func_02003b68(62, 66, 29, 73, 9, 9);
  Func_02003c58(0x7fff, 0);
  Call2((void (*)())Func_02003c50, 0x10003, 0);
  Func_02003c60(60);
  wait(60);
  Func_02003c58(0x10003, 0);
  Func_02003c50(0x10000, 0);
  Func_02003c60(60);
  wait(70);
  Func_02003b00();
  Func_02003af8();
  Func_02003bb8(gSequenceParentWork.paletteGlow[0], gSequenceParentWork.paletteGlow[1]);
  REG_BLDCNT = 0;
  REG_DISPCNT |= 0x100;
  gBgScroll[0].x = 0;
  gBgScroll[0].y = 0;
  GameFlag_SetBit(0x87b);
  Func_02003bd0();
}
