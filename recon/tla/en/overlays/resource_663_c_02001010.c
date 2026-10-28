// Draft, differ 41 of 543 (was 253): the first beam of light in Air's Rock,
// the same loading and sparks as Func_020026b8's with object 8 glowing (the
// leader and object 8 take action variant 1, BG0 sits on BG1's priority 2):
// the camera moves to (0x14a, 0x49), the light glows in over 64 frames
// (Func_02000ef4), then for 255 frames it throws sparks (Effect_Spawn with
// Func_02000f9c) at random places every fourth frame while the blend fades;
// the cells at (68, 0) and (38, 9) open (Func_02003b50), the light fades over
// 65 frames (cue 390), the display is put back and flag 0x858 set.
// Written as overlay 664's Func_02003654 (adopted): the spark loop's forms,
// calls through CallN, the spark options and velocity declared in the spark
// block (so the frame matches: options at sp+16, velocity at sp+56, the fill
// at sp+70) and the spark loop counting in a counter of its own (with one
// counter the native's r7 goes to the frame count's low bits).
// Left: the end keeps the zero of the glow byte apart from the later zeros
// (the native builds one 0 in r5 for the glow byte, the update and BLDCNT)
// and swaps r5 and r6 for gAdventure; order around the BLDCNT store (the
// one-pass block there is a trial), the spark call's options and the fade.
#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "adventure.h"
#include "field_sprite.h"
#include "system/dma.h"
#include "system/io_reg.h"
#include "lib/call.h"

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
struct ObjectRuntime *Object_GetById(u32 id);
void Func_0200092c(void);
void Func_020008ac(void);

u32 Random16Far(void);
void Func_02003b28(struct ObjectRuntime *object, s32 mode);
void Func_02003b38(struct ObjectRuntime *object, const u8 *script);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object,
                                        u32 value);
void Object_SetPartAttribute(struct ObjectRuntime *object, s32 value);
void FieldEvent_WaitFrames(s32 frames);
void WaitFrames(s32 frames);
s32 Camera_FollowActor(s32 id, s32 a);
void Func_02003c30(void);
void Func_02003c20(s32 a, s32 b);
void Camera_MoveWithinMapBounds(s32 x, s32 y, s32 z, s32 mode);
void Task_RemoveFar(void (*callback)(void));
void Task_Add(void (*callback)(void), s32 priority);
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
  u16 *buffer = Heap_Alloc(0x680);
  u16 *cell;
  struct ObjectRuntime *spark;
  u32 i;
  s32 x;

  Func_02003bc8();
  Func_02003c88(0);
  REG_DISPCNT &= ~0x100;
  Func_02003af0(Resource_GetTableEntry((s32)&ResourceId_WhirlwindTiles), buffer);
  Dma_Set(buffer, (void *)0x600e800, 0x840000d0, (volatile u32 *)0x040000d4);
  Dma_Set((u8 *)buffer + 0x340, (void *)0x600ec00, 0x840000d0,
          (volatile u32 *)0x040000d4);
  Heap_Release(buffer);
  ObjectMotion_SetActionVariant(gAdventure.currentOwner, 1);
  ObjectMotion_SetActionVariant(8, 1);
  do {
    s32 control = 0x40d;

    REG_BG0CNT = control;
  } while (0);
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
  ((u8 *)Object_GetById(8))[98] = 9;
  ((u8 *)Object_GetById(8))[99] = 1;
  Object_GetById(8)->update = Func_0200092c;
  for (i = 0; i != 45; i++) {
    if ((gFrameCount & 3) == 0) {
      spark = (struct ObjectRuntime *)Import_ActorCreate(
          0x2a0, Object_GetById(8)->x, Object_GetById(8)->y,
          Object_GetById(8)->z);
      {
        s32 priority = ((s8 *)Object_GetById(8)->animation)[9] & 12;

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
      ObjectDispatch_SetSingleChildFlags(spark, 0);
      Object_SetPartAttribute(spark, 2);
      spark->update = Func_020008ac;
    }
    FieldEvent_WaitFrames(1);
  }
  Camera_FollowActor(gAdventure.currentOwner, 1);
  Func_02003c30();
  Call2((void (*)())Func_02003c20, 0x10000, 0x2000);
  Call4((void (*)())Camera_MoveWithinMapBounds, 0x14a0000, -1, 0x490000, 1);
  Func_02003c30();
  Task_Add(Func_02000ef4, 0xc80);
  Func_02003cd8(163);
  REG_BLDALPHA = 0xf00;
  do {
    REG_BLDCNT = 0x3f41;
  } while (0);
  REG_DISPCNT |= 0x100;
  Call3((void (*)())Func_02003b98, 0x10000, 0x10000, 0x10000);
  i = 0;
glow:
  REG_BLDALPHA = ((15 - (i >> 2)) << 8) | (i >> 2);
  ((u8 *)Object_GetById(8))[98] = Random16Far() & 7;
  i++;
  FieldEvent_WaitFrames(1);
  if (i != 64)
    goto glow;
  FieldEvent_WaitFrames(60);
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
    Effect_Spawn(x, 0, (Random16Far() % 144) << 16, velocity.x, velocity.y,
                 velocity.z, EFFECT_USE_UPDATE | EFFECT_USE_PRIORITY | 1,
                 &options);
  }
  REG_BLDALPHA = (((j >> 4) & 15) << 8) | (16 - ((j >> 4) & 15));
  ((u8 *)Object_GetById(8))[98] = Random16Far() & 7;
  FieldEvent_WaitFrames(1);
    if (j != 255)
      goto burst;
  }
  Call6((void (*)())Func_02003b68, 68, 0, 79, 0, 11, 10);
  Call6((void (*)())Func_02003b80, 38, 9, 1, 1, 16, 9);
  Call6((void (*)())Func_02003b80, 38, 9, 1, 1, 24, 9);
  Func_02003b50();
  FieldEvent_WaitFrames(1);
  REG_BLDCNT = 0x3f41;
  REG_BLDALPHA = 15;
  Call3((void (*)())Func_02003b98, -1, -1, 0xe666);
  Call1((void (*)())Func_02003cd8, 390);
  ((u8 *)Object_GetById(8))[98] = 6;
  i = 0;
fade:
  REG_BLDALPHA = ((i >> 2) << 8) | (16 - (i >> 2));
  i++;
  FieldEvent_WaitFrames(1);
  if (i != 65)
    goto fade;
  Task_RemoveFar(Func_02000ef4);
  ((u8 *)Object_GetById(8))[99] = 0;
  WaitFrames(30);
  Object_GetById(8)->update = 0;
  Object_SetPartAttribute(Object_GetById(8), 6);
  Func_02003ba0();
  FieldEvent_WaitFrames(60);
  REG_BLDCNT = 0;
  Func_02003b00();
  Func_02003af8();
  Func_02003bb8(gAdventure.paletteGlow[0], gAdventure.paletteGlow[1]);
  REG_BG1CNT &= ~3;
  REG_BG1CNT |= 1;
  Object_GetById(gAdventure.currentOwner)->priorityFlags &= ~32;
  Object_GetById(gAdventure.currentOwner)->priorityFlags |= 1;
  Object_GetById(8)->priorityFlags |= 1;
  Camera_FollowActor(gAdventure.currentOwner, 1);
  Func_02003c30();
  GameFlag_SetBit(0x858);
  Func_02003bd0();
}
