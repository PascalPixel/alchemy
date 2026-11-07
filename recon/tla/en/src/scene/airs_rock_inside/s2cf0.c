#include "types.h"
#include "scene_tables.h"
#include "actor.h"
#include "main_routine_work.h"
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
extern const u8 gData_02005fd8[];
void *Heap_Alloc(s32 size);
void Heap_Release(void *block);
void Func_02005e14(void);
void Func_02005e1c(void);
void Func_02005f2c(s32 skip);
void *Resource_GetTableEntry(s32 id);
void Func_02005d24(void *resource, void *dest);
void Func_02005f94(s32 cue);
struct Actor *Actor_GetById(u32 id);
void Func_02002cc0(void);
void Func_02002c40(void);
struct Actor *Import_ActorCreate(s32 type, s32 x, s32 y, s32 z);
u32 Random16Far(void);
void Func_02005d5c(struct Actor *object, s32 mode);
void Func_02005d6c(struct Actor *object, const u8 *script);
void Actor_SetSingleChildFlags(struct Actor *object,
                                        u32 value);
void Actor_SetPartAttribute(struct Actor *object, s32 value);
void FieldEvent_Wait(s32 frames);
void wait(s32 frames);
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
void Func_02001084(struct Actor *object, s32 a, s32 b);
void Func_02005d34(void);
void Func_02005d2c(void);
void Func_02005e04(s32 hue, s32 brightness);
void GameFlag_SetBit(s32 flag);

/// The second pillar of light, Func_02003654 with object 11 glowing, the camera
/// at (0x138, 0x2a8), the cells at (0x40, 78) opening and the light reaching
/// object 10 (objects 8 and 9 lit with it); sets flag 0x89a.
void Func_02002cf0(void) {
  u16 *buffer = Heap_Alloc(0x800);
  u16 *cell;
  struct Actor *spark;
  u32 i;

  Func_02005e14();
  Func_02005f2c(0);
  REG_DISPCNT &= ~DISPCNT_BG0_ON;
  /* FAKEMATCH(one-pass): BG0CNT's value in a block of its own, built before
   * its address; as a plain block, 2 of 446 instructions differ in tla-en. */
  do {
    s32 control = BGCNT_SCREEN_BASE(4);

    REG_BG0CNT = control;
  } while (0);
  Func_02005d24(Resource_GetTableEntry((s32)&ResourceId_LightningTiles),
                buffer);
  Dma_Set(buffer, (void *)BG_CHAR_BLOCK(0), DMA_COPY_WORDS(512), REG_DMA3);
  Heap_Release(buffer);
  Dma_Fill16(0, (void *)BG_SCREEN_BLOCK(4), 0x800);
  buffer = Heap_Alloc(0x80);
  Func_02005d24(Resource_GetTableEntry((s32)&ResourceId_LightningMap), buffer);
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
  Dma_Set(cell, (void *)(BG_SCREEN_BLOCK(4) + (i << 6)), DMA_COPY_WORDS(2),
          REG_DMA3);
  i++;
  cell += 4;
  if (i != 16)
    goto row;
  Heap_Release(buffer);
  Func_02005f94(246);
  ((u8 *)Actor_GetById(11))[98] = 9;
  ((u8 *)Actor_GetById(11))[99] = 1;
  Actor_GetById(11)->update = Func_02002cc0;
  for (i = 0; i != 45; i++) {
    if ((gFrameCount & 3) == 0) {
      spark = Import_ActorCreate(0x2a0, Actor_GetById(11)->x,
                                  Actor_GetById(11)->y, Actor_GetById(11)->z);
      {
        s32 priority = ((s8 *)Actor_GetById(11)->animation)[9] & 12;

        /* FAKEMATCH(temporary): the sprite held in the row pointer; as
         * spark->animation, 28 of 446 instructions differ in tla-en. */
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
      Func_02005d5c(spark, 7);
      Func_02005d6c(spark, gData_02005fd8);
      Actor_SetSingleChildFlags(spark, 0);
      Actor_SetPartAttribute(spark, 2);
      spark->update = Func_02002c40;
    }
    FieldEvent_Wait(1);
  }
  Call2((void (*)())Func_02005f0c, 0x10000, 0);
  Call2((void (*)())Func_02005f04, 0x10003, 0);
  Func_02005f14(60);
  Camera_FollowActor(gMainRoutineWork.currentOwner, 1);
  Func_02005ee4();
  Call2((void (*)())Func_02005ed4, 0x10000, 0x2000);
  Call4((void (*)())Camera_MoveWithinMapBounds, PIXELS(312), -1, PIXELS(680),
        1);
  Func_02005ee4();
  Actor_GetById(11)->update = 0;
  Actor_SetPartAttribute(Actor_GetById(11), 6);
  REG_BLDALPHA = BLEND_ALPHA(16, 16);
  REG_BLDCNT = BLDCNT_TGT1_BG0 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
  gBgScroll[0].x = 0xff98;
  gBgScroll[0].y = 42;
  wait(30);
  Func_02005f94(138);
  Call2((void (*)())Func_02005f0c, 0x10003, 0);
  Call2((void (*)())Func_02005f04, 0x7fff, 0);
  Func_02005f14(1);
  wait(1);
  Func_02005f0c(0x7fff, 0);
  Func_02005f04(0x10003, 0);
  Func_02005f14(8);
  REG_DISPCNT |= DISPCNT_BG0_ON;
  Func_02005d9c(64, 78, 18, 105, 3, 3);
  Func_02005dcc(0x50000, 0x70000, 0x10000);
  Call3((void (*)())Func_02005dcc, -1, -1, 0xe666);
  i = 0;
fade:
  REG_BLDALPHA = BLEND_ALPHA(16 - (i >> 1), 16);
  wait(1);
  i++;
  if (i != 32)
    goto fade;
  Func_02005f0c(0x10003, 0);
  Func_02005f04(0x10000, 0);
  Func_02005f14(60);
  wait(30);
  Func_02005f94(148);
  Func_02002aa8(0x1380000, 0x2d00000);
  Call6((void (*)())Func_02005d9c, 64, 78, 18, 105, 3, 5);
  Func_02005ed4(0x18000, 0x3000);
  Func_02005eec(10, 1);
  Func_02005ee4();
  Func_02005f94(146);
  Call2((void (*)())Func_02005e94, 10, 0x100);
  Func_02002b58(Actor_GetById(10)->x, Actor_GetById(10)->z);
  Func_02005e94(10, 0);
  Actor_SetPartAttribute(Actor_GetById(10), 7);
  wait(3);
  Actor_SetPartAttribute(Actor_GetById(8), 0);
  Actor_SetPartAttribute(Actor_GetById(9), 0);
  Actor_SetPartAttribute(Actor_GetById(10), 0);
  wait(40);
  Func_02001084(Actor_GetById(10), 8, 0);
  Func_02005d34();
  Func_02005d2c();
  Func_02005e04(gMainRoutineWork.paletteGlow[0], gMainRoutineWork.paletteGlow[1]);
  REG_BLDALPHA = BLEND_ALPHA(8, 16);
  REG_BLDCNT = BLDCNT_TGT1_OBJ | BLDCNT_TGT2_ALL;
  /* FAKEMATCH(barrier): DISPCNT's address built after the blend store;
   * without it, 2 of 446 instructions differ in tla-en. */
  asm volatile("");
  REG_DISPCNT |= DISPCNT_BG0_ON;
  gBgScroll[0].x = 0;
  gBgScroll[0].y = 0;
  GameFlag_SetBit(0x89a);
  Func_02005e1c();
}
