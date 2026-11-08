#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "field_sprite.h"
#include "system/dma.h"
#include "system/io_reg.h"
#include "call.h"

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
void Func_02003c88(s32 skip);
void *Resource_GetTableEntry(s32 id);
void Func_02003af0(void *resource, void *dest);
void Func_02003cd8(s32 cue);
struct Actor *Actor_GetById(u32 id);
void Func_0200092c(void);
void Func_020008ac(void);

u32 Random16Far(void);
void Func_02003b28(struct Actor *object, s32 mode);
void Func_02003b38(struct Actor *object, const u8 *script);
void Actor_SetSingleChildFlags(struct Actor *object, u32 value);
void Actor_SetPartAttribute(struct Actor *object, s32 value);
void FieldEvent_Wait(s32 frames);
void wait(s32 frames);
s32 Camera_FollowActor(s32 id, s32 a);
void Func_02003c30(void);
void Func_02003c20(s32 a, s32 b);
void Camera_MoveWithinMapBounds(s32 x, s32 y, s32 z, s32 mode);
void Task_RemoveFar(void (*func)(void));
void Task_Add(void (*func)(void), s32 priority);
void Func_020024b0(void);
void Func_0200257c(void);
void Func_02000ef4(void);
void Func_02003c58(s32 a, s32 b);
s32 Func_02003c50(s32 a, s32 b);
s32 Func_02003c60(s32 frames);
void Func_02003b98(s32 a, s32 b, s32 c);
void Func_02003ba0(void);
void Func_02003b00(void);
void Func_02003af8(void);
void Func_02003bb8(s32 hue, s32 brightness);
void GameFlag_SetBit(s32 flag);

#include "system/field_effect.h"
#include "system/vector3.h"

void Func_02000f9c(union FieldObject *object);
void Func_02003b68(s32 x, s32 z, s32 width, s32 height, s32 a, s32 b);
void Func_02003b80(s32 x, s32 z, s32 a, s32 b, s32 width, s32 height);
void Func_02003b50(void);

void Func_02001010(void) {
  struct MainRoutineWork *work;
  u16 *buffer = Heap_Alloc(0x680);
  u16 *cell;
  struct Actor *spark;
  u32 i;
  s32 x;

  Func_02003bc8();
  Func_02003c88(0);
  REG_DISPCNT &= ~0x100;
  Func_02003af0(Resource_GetTableEntry((s32)&ResourceId_WhirlwindTiles),
                buffer);
  Dma_Set(buffer, (void *)0x600e800, 0x840000d0, (volatile u32 *)0x040000d4);
  Dma_Set((u8 *)buffer + 0x340, (void *)0x600ec00, 0x840000d0,
          (volatile u32 *)0x040000d4);
  Heap_Release(buffer);
  Actor_SetSpritePriority(gMainRoutineWork.currentOwner, 1);
  Actor_SetSpritePriority(8, 1);
  {
    s32 control = 0x40d;

    REG_BG0CNT = control;
    do {
    } while (0);
  }
  REG_BG1CNT &= ~3;
  REG_BG1CNT |= 2;
  Dma_Fill16(0x10, (void *)0x6002000, 0x500);
  buffer = Heap_Alloc(0x240);
  Func_02003af0(Resource_GetTableEntry((s32)&ResourceId_WhirlwindMap), buffer);
  i = 0;
  cell = buffer;
offset:
  *cell += 0xff40;
  i++;
  cell++;
  if (i != 0x120)
    goto offset;
  cell = buffer;
  i = 0;
row:
  Dma_Set(cell, (void *)(0x600200f + (i << 6)), 0x84000008,
          (volatile u32 *)0x040000d4);
  i++;
  cell += 16;
  if (i != 18)
    goto row;
  Heap_Release(buffer);
  Func_02003cd8(246);
  ((u8 *)Actor_GetById(8))[98] = 9;
  ((u8 *)Actor_GetById(8))[99] = 1;
  Actor_GetById(8)->update = Func_0200092c;
  for (i = 0; i != 45; i++) {
    if ((gFrameCount & 3) == 0) {
      spark = (struct Actor *)Import_ActorCreate(
          0x2a0, Actor_GetById(8)->x, Actor_GetById(8)->y, Actor_GetById(8)->z);
      {
        s32 priority = ((s8 *)Actor_GetById(8)->animation)[9] & 12;

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
      Actor_SetPartAttribute(spark, 2);
      spark->update = Func_020008ac;
    }
    FieldEvent_Wait(1);
  }
  Camera_FollowActor(gMainRoutineWork.currentOwner, 1);
  Func_02003c30();
  Call2((void (*)())Func_02003c20, 0x10000, 0x2000);
  Call4((void (*)())Camera_MoveWithinMapBounds, 0x14a0000, -1, 0x490000, 1);
  Func_02003c30();
  Task_Add(Func_02000ef4, 0xc80);
  Func_02003cd8(163);
  REG_BLDALPHA = 0xf00;
  REG_BLDCNT = 0x3f41;
  REG_DISPCNT |= 0x100;
  Call3((void (*)())Func_02003b98, 0x10000, 0x10000, 0x10000);
  i = 0;
glow:
  REG_BLDALPHA = ((15 - (i >> 2)) << 8) | (i >> 2);
  ((u8 *)Actor_GetById(8))[98] = Random16Far() & 7;
  i++;
  FieldEvent_Wait(1);
  if (i != 64)
    goto glow;
  FieldEvent_Wait(60);
  REG_BLDALPHA = 15;
  REG_BLDCNT = 0xc42;
  Call3((void (*)())Func_02003b98, 0x30000, 0x30000, 0x10000);
  {
    u32 j = 0;

  burst:
    j++;
    if ((j & 3) == 0) {
      struct Vector3 velocity;
      struct EffectOptions options;

      options.priority = 1;
      options.update = Func_02000f9c;
      velocity.x = 0;
      velocity.y = 0;
      velocity.z = 0xfffe0000;
      x = ((Random16Far() % 144) << 16) + 0x1000000;
      Func_0200015c(x, 0, (Random16Far() % 144) << 16, velocity.x, velocity.y,
                   velocity.z, EFFECT_USE_UPDATE | EFFECT_USE_PRIORITY | 1,
                   &options);
    }
    REG_BLDALPHA = (((j >> 4) & 15) << 8) | (16 - ((j >> 4) & 15));
    ((u8 *)Actor_GetById(8))[98] = Random16Far() & 7;
    FieldEvent_Wait(1);
    if (j != 255)
      goto burst;
  }
  Call6((void (*)())Func_02003b68, 68, 0, 79, 0, 11, 10);
  Call6((void (*)())Func_02003b80, 38, 9, 1, 1, 16, 9);
  Call6((void (*)())Func_02003b80, 38, 9, 1, 1, 24, 9);
  Func_02003b50();
  FieldEvent_Wait(1);
  REG_BLDCNT = 0x3f41;
  REG_BLDALPHA = 15;
  Call3((void (*)())Func_02003b98, -1, -1, 0xe666);
  Call1((void (*)())Func_02003cd8, 390);
  ((u8 *)Actor_GetById(8))[98] = 6;
  i = 0;
fade:
  REG_BLDALPHA = ((i >> 2) << 8) | (16 - (i >> 2));
  FieldEvent_Wait(1);
  i++;
  if (i != 65)
    goto fade;
  Task_RemoveFar(Func_02000ef4);
  {
    s32 zero = 0;

    ((u8 *)Actor_GetById(8))[99] = zero;
    wait(30);
    Actor_GetById(8)->update = (void (*)(void))zero;
    Actor_SetPartAttribute(Actor_GetById(8), 6);
    Func_02003ba0();
    FieldEvent_Wait(60);
    REG_BLDCNT = zero;
  }
  Func_02003b00();
  Func_02003af8();
  work = &gMainRoutineWork;
  Func_02003bb8(work->paletteGlow[0], work->paletteGlow[1]);
  REG_BG1CNT &= ~3;
  REG_BG1CNT |= 1;
  {
    s32 *ownerId = &work->currentOwner;

    Actor_GetById(*ownerId)->priorityFlags &= ~32;
    Actor_GetById(*ownerId)->priorityFlags |= 1;
    {
      u8 *flags = &Actor_GetById(8)->priorityFlags;
      u8 v = *flags | 1;

      *flags = v;
    }
    Camera_FollowActor(*ownerId, 1);
  }
  Func_02003c30();
  GameFlag_SetBit(0x858);
  Func_02003bd0();
}
