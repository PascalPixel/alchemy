#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "field_sprite.h"
#include "system/dma.h"
#include "system/io_reg.h"
#include "event_inline.h"

struct SpriteParts {
  u8 unk00[9];
  u8 unk09Low : 2;
  u8 part : 2;
};
struct ObjBits {
  u8 unk00[35];
  u8 flags : 8;
};
extern s32 gFrameCount;
extern char ResourceId_WhirlwindMap;
extern char ResourceId_WhirlwindTiles;
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
void Actor_SetSingleChildFlags(struct Actor *object, u32 value);
void Func_02003c30(void);
void Func_020024b0(void);
void Func_0200257c(void);
void Func_02000ef4(void);
void Func_02003b00(void);
void Func_02003af8(void);

void Func_020026b8(void) {
  u16 *buffer = Heap_Alloc(0x680);
  s32 *beam = (s32 *)((u8 *)Ram_HeapSlots->mapWork + 0x178);
  u16 *cell;
  struct Actor *spark;
  s32 i;
  s32 z;

  Func_02003bc8();
  FieldEvent_ResolvePendingActions(0);
  REG_DISPCNT &= ~0x100;
  Func_02003af0(Resource_GetTableEntry((s32)&ResourceId_WhirlwindTiles),
                buffer);
  Dma_Set(buffer, (void *)0x600e800, 0x840000d0, (volatile u32 *)0x040000d4);
  Dma_Set((u8 *)buffer + 0x340, (void *)0x600ec00, 0x840000d0,
          (volatile u32 *)0x040000d4);
  Heap_Release(buffer);
  {
    s32 control = 0x40e;

    REG_BG0CNT = control;
  }
  // A once-only loop: its loop notes order the fill's address ahead of its
  // value.
  do {
    Dma_Fill16(0x10, (void *)0x6002000, 0x500);
  } while (0);
  buffer = Heap_Alloc(0x240);
  Func_02003af0(Resource_GetTableEntry((s32)&ResourceId_WhirlwindMap), buffer);
  cell = buffer;
  for (i = 0; i != 0x120; i++) {
    *cell += 0xff40;
    cell++;
  }
  cell = buffer;
  for (i = 0; i != 18; i++) {
    Dma_Set(cell, (void *)(0x600200f + (i << 6)), 0x84000008,
            (volatile u32 *)0x040000d4);
    cell += 16;
  }
  Heap_Release(buffer);
  Event_PlaySound(246);
  ((u8 *)Actor_Get(12))[98] = 9;
  ((u8 *)Actor_Get(12))[99] = 1;
  Actor_Get(12)->update = Func_0200092c;
  for (i = 0; i != 45; i++) {
    s32 low = gFrameCount & 3;

    if (low == 0) {
      spark = (FieldActor_Create(0x2a0, Actor_Get(12)->x,
                                                  Actor_Get(12)->y,
                                                  Actor_Get(12)->z));
      {
        s32 priority = ((s8 *)Actor_Get(12)->animation)[9] & 12;

        cell = spark->animation;
        ((struct SpriteParts *)cell)->part = priority >> 2;
      }
      ((struct ObjBits *)spark)->flags |= 1;
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
  FieldCamera_FollowActor(gMainRoutineWork.currentOwner, 1);
  Func_02003c30();
  FieldCamera_SetSpeed(0x10000, 0x2000);
  FieldCamera_MoveTo(0x980000, -1, 0x900000, 1);
  Func_02003c30();
  FieldTask_Remove(Func_020024b0);
  REG_BG1CNT &= ~3;
  REG_BG1CNT |= 0;
  PaletteFade_ApplySource(0x10000, 0);
  PaletteFade_ApplyTarget(0x7fff, 0);
  PaletteFade_Interpolate(1);
  System_Wait(1);
  PaletteFade_ApplySource(0x7fff, 0);
  PaletteFade_ApplyTarget(0x10000, 0);
  PaletteFade_Interpolate(8);
  Task_AddFrame(Func_0200257c, 0xc80);
  Event_PlaySound(163);
  REG_DISPCNT |= 0x100;
  MapGrid_SetShake(0x30000, 0x30000, 0x10000);
  z = 0xffff8000;
  for (i = 0x4000; i != 0x48000; i += 0x400, z += 0xfffffc00) {
    beam[6] = i;
    beam[7] = z;
    ((u8 *)Actor_Get(12))[98] = Random16Far() & 7;
    System_Wait(1);
  }
  FieldTask_Remove(Func_0200257c);
  Task_AddFrame(Func_02000ef4, 0xc80);
  REG_BLDALPHA = 8;
  REG_BLDCNT = 0x3f42;
  for (i = 0; i != 257; i++) {
    REG_BLDALPHA = (8 - (i >> 5)) | 0x1000;
    ((u8 *)Actor_Get(12))[98] = Random16Far() & 7;
    FieldEvent_Wait(1);
  }
  REG_DISPCNT &= ~0x200;
  System_Wait(30);
  Event_PlaySound(390);
  REG_BLDALPHA = 0x10;
  REG_BLDCNT = 0x3f41;
  MapGrid_SetShake(-1, -1, 0xe666);
  ((u8 *)Actor_Get(12))[98] = 6;
  for (i = 0; i != 129; i++) {
    REG_BLDALPHA = ((i >> 3) << 8) | (0x10 - (i >> 3));
    FieldEvent_Wait(1);
  }
  FieldTask_Remove(Func_02000ef4);
  {
    s32 zero = 0;

    ((u8 *)Actor_Get(12))[99] = zero;
    System_Wait(30);
    Actor_Get(12)->update = (void (*)(void))zero;
    Actor_SetSpriteColor(Actor_Get(12), 6);
    MapGrid_WaitForShakeToSettle();
    FieldEvent_Wait(60);
    REG_BLDCNT = zero;
  }
  Func_02003b00();
  Func_02003af8();
  WindowColor_Apply(gMainRoutineWork.paletteGlow[0],
                gMainRoutineWork.paletteGlow[1]);
  REG_BG1CNT &= ~3;
  REG_BG1CNT |= 1;
  Actor_Get(gMainRoutineWork.currentOwner)->priorityFlags &= ~32;
  Actor_Get(gMainRoutineWork.currentOwner)->priorityFlags |= 1;
  Actor_Get(12)->priorityFlags |= 1;
  FieldCamera_FollowActor(gMainRoutineWork.currentOwner, 1);
  Func_02003c30();
  Event_SetFlag(0x87a);
  Func_02003bd0();
}
