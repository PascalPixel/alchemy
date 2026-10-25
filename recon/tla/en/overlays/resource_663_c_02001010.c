// Draft, differ 275 of 543: the first beam of light in Air's Rock, the same
// loading and sparks as Func_020026b8's with object 8 glowing (the leader
// and object 8 take action variant 1, BG0 sits on BG1's priority 2): the
// camera moves to (0x14a, 0x49), the light glows in over 64 frames
// (Func_02000ef4), then for 255 frames it throws sparks (Effect_Spawn with
// Func_02000f9c) at random places every fourth frame while the blend
// fades; the cells at (68, 0) and (38, 9) open (Func_02003b50), the light
// fades over 65 frames (cue 390), the display is put back and flag 0x858
// set.
// Left: the native's 72-byte frame holds the spawn options at sp+16, the
// velocity at sp+56 and the fill's halfword at sp+70 (the draft's fill lands
// first, at sp+18), it saves only r8, takes resource 446's id and BG0CNT's
// 0x40d the other way round between pool and movs, and keeps the counters in
// r7.
#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "party_state.h"
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
void Scheduler_RemoveCallbackFar(void (*callback)(void));
void Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 priority);
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
  struct EffectOptions options;
  struct Vector3 velocity;
  u16 *buffer = MemAlloc(0x680);
  u16 *cell;
  struct ObjectRuntime *spark;
  s32 i, x;
  u32 u;

  Func_02003bc8();
  Func_02003c88(0);
  REG_DISPCNT &= ~0x100;
  Func_02003af0(Resource_GetTableEntry(446), buffer);
  Dma_Set(buffer, (void *)0x600e800, 0x840000d0, (volatile u32 *)0x040000d4);
  Dma_Set((u8 *)buffer + 0x340, (void *)0x600ec00, 0x840000d0,
          (volatile u32 *)0x040000d4);
  MemFree(buffer);
  ObjectMotion_SetActionVariant(gPartyState.currentOwner, 1);
  ObjectMotion_SetActionVariant(8, 1);
  REG_BG0CNT = 0x40d;
  REG_BG1CNT &= ~3;
  REG_BG1CNT |= 2;
  Dma_Fill16(0x10, (void *)0x6002000, 0x500);
  buffer = MemAlloc(0x240);
  Func_02003af0(Resource_GetTableEntry(447), buffer);
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
  MemFree(buffer);
  Func_02003cd8(246);
  ((u8 *)Object_GetById(8))[98] = 9;
  ((u8 *)Object_GetById(8))[99] = 1;
  Object_GetById(8)->update = Func_0200092c;
  for (i = 0; i != 45; i++) {
    if ((gFrameCount & 3) == 0) {
      spark = (struct ObjectRuntime *)Engine_ObjectCreate(
          0x2a0, Object_GetById(8)->x, Object_GetById(8)->y,
          Object_GetById(8)->z);
      ((struct FieldSprite *)spark->animation)->priority =
          ((struct FieldSprite *)Object_GetById(8)->animation)->priority;
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
  Camera_FollowActor(gPartyState.currentOwner, 1);
  Func_02003c30();
  Func_02003c20(0x10000, 0x2000);
  Camera_MoveWithinMapBounds(0x14a0000, -1, 0x490000, 1);
  Func_02003c30();
  Scheduler_AddOrUpdateCallback(Func_02000ef4, 0xc80);
  Func_02003cd8(163);
  REG_BLDALPHA = 0xf00;
  REG_BLDCNT = 0x3f41;
  REG_DISPCNT |= 0x100;
  Func_02003b98(0x10000, 0x10000, 0x10000);
  u = 0;
glow:
  REG_BLDALPHA = ((15 - (u >> 2)) << 8) | (u >> 2);
  ((u8 *)Object_GetById(8))[98] = Random16Far() & 7;
  u++;
  FieldEvent_WaitFrames(1);
  if (u != 64)
    goto glow;
  FieldEvent_WaitFrames(60);
  REG_BLDALPHA = 15;
  REG_BLDCNT = 0xc42;
  Func_02003b98(0x30000, 0x30000, 0x10000);
  u = 0;
burst:
  u++;
  if ((u & 3) == 0) {
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
  REG_BLDALPHA = (((u >> 4) & 15) << 8) | (16 - ((u >> 4) & 15));
  ((u8 *)Object_GetById(8))[98] = Random16Far() & 7;
  FieldEvent_WaitFrames(1);
  if (u != 255)
    goto burst;
  Func_02003b68(68, 0, 79, 0, 11, 10);
  Func_02003b80(38, 9, 1, 1, 16, 9);
  Func_02003b80(38, 9, 1, 1, 24, 9);
  Func_02003b50();
  FieldEvent_WaitFrames(1);
  REG_BLDCNT = 0x3f41;
  REG_BLDALPHA = 15;
  Func_02003b98(-1, -1, 0xe666);
  Func_02003cd8(390);
  ((u8 *)Object_GetById(8))[98] = 6;
  u = 0;
fade:
  REG_BLDALPHA = ((u >> 2) << 8) | (16 - (u >> 2));
  u++;
  FieldEvent_WaitFrames(1);
  if (u != 65)
    goto fade;
  Scheduler_RemoveCallbackFar(Func_02000ef4);
  ((u8 *)Object_GetById(8))[99] = 0;
  WaitFrames(30);
  Object_GetById(8)->update = 0;
  Object_SetPartAttribute(Object_GetById(8), 6);
  Func_02003ba0();
  FieldEvent_WaitFrames(60);
  REG_BLDCNT = 0;
  Func_02003b00();
  Func_02003af8();
  Func_02003bb8(gPartyState.paletteGlow[0], gPartyState.paletteGlow[1]);
  REG_BG1CNT &= ~3;
  REG_BG1CNT |= 1;
  Object_GetById(gPartyState.currentOwner)->priorityFlags &= ~32;
  Object_GetById(gPartyState.currentOwner)->priorityFlags |= 1;
  Object_GetById(8)->priorityFlags |= 1;
  Camera_FollowActor(gPartyState.currentOwner, 1);
  Func_02003c30();
  GameFlag_SetBit(0x858);
  Func_02003bd0();
}
