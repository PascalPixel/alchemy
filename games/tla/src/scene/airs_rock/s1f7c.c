#include "types.h"
#include "scene_tables.h"
#include "actor.h"
#include "main_routine_work.h"
#include "bg_scroll.h"
#include "field_sprite.h"
#include "system/dma.h"
#include "system/io_reg.h"
#include "event_inline.h"

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
void *Resource_GetTableEntry(s32 id);
void Func_02003af0(void *resource, void *dest);
void Func_0200092c(void);
void Func_020008ac(void);
u32 Random16Far(void);
void Func_02003b28(struct Actor *object, s32 mode);
void Func_02003b38(struct Actor *object, const u8 *script);
void Actor_SetSingleChildFlags(struct Actor *object,
                                        u32 value);
void Func_02003c30(void);
void Func_02005dcc(s32 a, s32 b, s32 c);
void Func_02001dc4(s32 x, s32 z);
void Func_02003b00(void);
void Func_02003af8(void);

/// The pillar of light in Air's Rock, overlay 664's Func_02003654 with object
/// 14 glowing: the camera moves to (0x218, 0xd8), the leader turns, and the
/// light flashes twice (cue 138), each time copying cells into the map
/// (Map_CopyCellPictureIndicesRect at (52, 12) and (52, 22)) and lighting a spot
/// (Func_02001dc4), then (cue 145) three more spots light and the cells at
/// (62, 2) open; sets flag 0x87b.
void Func_02001f7c(void) {
  u16 *buffer = Heap_Alloc(0x800);
  u16 *cell;
  struct Actor *spark;
  u32 i;

  Func_02003bc8();
  FieldEvent_ResolvePendingActions(0);
  REG_DISPCNT &= ~DISPCNT_BG0_ON;
  /* FAKEMATCH(dead): BG0CNT's value in a once-only block of its own, built
   * before its address; as a plain block, 2 of 520 instructions differ in
   * tla-en. */
  do {
    s32 control = BGCNT_SCREEN_BASE(4);

    REG_BG0CNT = control;
  } while (0);
  Func_02003af0(Resource_GetTableEntry((s32)&ResourceId_LightningTiles),
                buffer);
  Dma_Set(buffer, (void *)BG_CHAR_BLOCK(0), DMA_COPY_WORDS(512), REG_DMA3);
  Heap_Release(buffer);
  Dma_Fill16(0, (void *)BG_SCREEN_BLOCK(4), 0x800);
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
  Dma_Set(cell, (void *)(BG_SCREEN_BLOCK(4) + (i << 6)), DMA_COPY_WORDS(2),
          REG_DMA3);
  i++;
  cell += 4;
  if (i != 16)
    goto row;
  Heap_Release(buffer);
  Event_PlaySound(246);
  ((u8 *)Actor_Get(14))[98] = 9;
  ((u8 *)Actor_Get(14))[99] = 1;
  Actor_Get(14)->update = Func_0200092c;
  for (i = 0; i != 45; i++) {
    if ((gFrameCount & 3) == 0) {
      spark = (FieldActor_Create(0x2a0, Actor_Get(14)->x,
                                  Actor_Get(14)->y, Actor_Get(14)->z));
      {
        s32 priority = ((s8 *)Actor_Get(14)->animation)[9] & 12;

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
      Actor_SetSpriteColor(spark, 2);
      spark->update = Func_020008ac;
    }
    FieldEvent_Wait(1);
  }
  PaletteFade_ApplySource(0x10000, 0);
  PaletteFade_ApplyTarget(0x10003, 0);
  PaletteFade_Interpolate(60);
  FieldCamera_FollowActor(gMainRoutineWork.currentOwner, 1);
  Func_02003c30();
  FieldCamera_SetSpeed(0x10000, 0x2000);
  FieldCamera_MoveTo(PIXELS(536), -1, PIXELS(216), 1);
  Func_02003c30();
  Actor_Get(14)->update = 0;
  Actor_SetSpriteColor(Actor_Get(14), 6);
  REG_BLDALPHA = BLEND_ALPHA(16, 16);
  REG_BLDCNT = BLDCNT_TGT1_BG0 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
  gBgScroll[0].x = 0xff88;
  gBgScroll[0].y = 0x10;
  System_Wait(30);
  Event_PlaySound(138);
  PaletteFade_ApplySource(0x10003, 0);
  PaletteFade_ApplyTarget(0x7fff, 0);
  PaletteFade_Interpolate(1);
  Actor_Get(gMainRoutineWork.currentOwner)->angle = FACING_SOUTH;
  System_Wait(1);
  PaletteFade_ApplySource(0x7fff, 0);
  PaletteFade_ApplyTarget(0x10003, 0);
  PaletteFade_Interpolate(8);
  REG_DISPCNT |= DISPCNT_BG0_ON;
  MapGrid_CopyCellsTo(52, 12, 29, 9, 9, 9);
  MapGrid_CopyCellsTo(52, 76, 29, 73, 9, 9);
  Func_02001dc4(0x2280000, 0xe40000);
  i = 0;
fade1:
  REG_BLDALPHA = BLEND_ALPHA(16 - (i >> 1), 16);
  System_Wait(1);
  i++;
  if (i != 32)
    goto fade1;
  gBgScroll[0].x = 0xffac;
  gBgScroll[0].y = 0;
  System_Wait(1);
  Event_PlaySound(138);
  PaletteFade_ApplySource(0x10003, 0);
  PaletteFade_ApplyTarget(0x7fff, 0);
  PaletteFade_Interpolate(1);
  System_Wait(1);
  PaletteFade_ApplySource(0x7fff, 0);
  PaletteFade_ApplyTarget(0x10003, 0);
  PaletteFade_Interpolate(8);
  REG_DISPCNT |= DISPCNT_BG0_ON;
  MapGrid_CopyCellsTo(52, 22, 29, 9, 9, 9);
  MapGrid_CopyCellsTo(52, 86, 29, 73, 9, 9);
  Func_02001dc4(0x2080000, 0xf20000);
  i = 0;
fade2:
  REG_BLDALPHA = BLEND_ALPHA(16 - (i >> 1), 16);
  System_Wait(1);
  i++;
  if (i != 32)
    goto fade2;
  Event_PlaySound(145);
  PaletteFade_ApplySource(0x10003, 0);
  PaletteFade_ApplyTarget(0x7fff, 0);
  PaletteFade_Interpolate(1);
  REG_DISPCNT &= ~DISPCNT_BG0_ON;
  FieldEvent_Wait(20);
  Func_02001dc4(0x2000000, 0xc60000);
  Func_02001dc4(0x2320000, 0xf20000);
  Func_02001dc4(0x2090000, 0xdf0000);
  MapGrid_CopyCellsTo(62, 2, 29, 9, 9, 9);
  MapGrid_CopyCellAttributes(62, 2, 9, 9, 29, 9);
  MapGrid_CopyCellsTo(62, 66, 29, 73, 9, 9);
  PaletteFade_ApplySource(0x7fff, 0);
  PaletteFade_ApplyTarget(0x10003, 0);
  PaletteFade_Interpolate(60);
  System_Wait(60);
  PaletteFade_ApplySource(0x10003, 0);
  PaletteFade_ApplyTarget(0x10000, 0);
  PaletteFade_Interpolate(60);
  System_Wait(70);
  Func_02003b00();
  Func_02003af8();
  WindowColor_Apply(gMainRoutineWork.paletteGlow[0], gMainRoutineWork.paletteGlow[1]);
  REG_BLDCNT = 0;
  REG_DISPCNT |= DISPCNT_BG0_ON;
  gBgScroll[0].x = 0;
  gBgScroll[0].y = 0;
  Event_SetFlag(0x87b);
  Func_02003bd0();
}
