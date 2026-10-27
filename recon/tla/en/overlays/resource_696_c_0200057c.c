// Draft, differ 499 of 720: a Shaman Village scene around object 14: loads the
// scene's pictures (resources 0x1be and 0x1bf) into background 1, sets the
// leader and object 14 on their action variants, sound 246 while for 45
// frames object 14 sheds circling, blinking objects (type 672) every fourth
// frame; then the camera to (168, 424), sound 207 with the screen shaking and
// a blend over 64 frames, the map's cells redrawn in stages (Func_02004844)
// while effects (Func_020000b8) rise and object 14 flickers, eight rounds of
// the background's halves swapped, sound 390, the blend cleared, the palette
// restored (Func_0200488c from the options' hue and brightness) and flag 2380
// set.
#include "types.h"
#include "io_reg.h"
#include "dma.h"
#include "party_state.h"
#include "frame.h"
#include "field_effect.h"

extern const s32 gData_02004ae0[];

void *MemAlloc(s32 size);
void MemFree(void *block);
void *Resource_GetTableEntry(s32 id);
void Func_0200479c(void *resource, void *destination);
void ObjectMotion_SetActionVariant(u32 id, s32 variant);
void Engine_AudioPlayCue(s32 cue);
struct ObjectRuntime *Object_GetById(s32 id);
void FieldEvent_WaitTicks(s32 ticks);
void WaitTicks(s32 ticks);
u32 Random16Far(void);
void ObjectDispatch_SetSingleChildFlags(struct FieldActor *object, s32 flags);
void Object_SetPartAttribute(void *object, s32 value);
void Effect_CircleAndBlink(union FieldObject *object);
void Camera_FollowActor(s32 id, s32 flag);
void Camera_MoveWithinMapBounds(s32 x, s32 y, s32 z, s32 mode);
void Func_020049f4(void);
void Func_020049e4(s32 a, s32 b);
void Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 priority);
void Scheduler_RemoveCallbackFar(void (*callback)(void));
void Func_02004864(s32 a, s32 b, s32 c);
void Func_02004844(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_0200484c(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_020000b8(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags,
                   const struct EffectOptions *options);
void Func_0200486c(void);
void Func_020047d4(void);
void Func_020047bc(void);
void Func_0200488c(s32 hue, s32 brightness);
void Func_0200482c(void);
void GameFlag_SetBit(s32 flag);
void Func_02000458(void);
void Func_02000378(void);
void Func_02000420(void);
void Func_02000508(union FieldObject *object);

#define OBJ14_BYTE(offset) (((u8 *)Object_GetById(14))[offset])

void Func_0200057c(void) {
  u16 *buffer;
  u16 zero;
  struct FieldEffect *effect;
  struct ObjectRuntime *source;
  struct EffectOptions options;
  s32 velocity[3];
  s32 x;
  u32 i;
  u32 j;
  s32 k;

  buffer = MemAlloc(0x680);
  REG_DISPCNT &= ~0x100;
  Func_0200479c(Resource_GetTableEntry(0x1be), buffer);
  Dma_Set(buffer, (void *)0x0600e800, 0x840000d0, REG_DMA3);
  Dma_Set(buffer + 0x1a0, (void *)0x0600ec00, 0x840000d0, REG_DMA3);
  MemFree(buffer);
  ObjectMotion_SetActionVariant(gPartyState.currentOwner, 1);
  ObjectMotion_SetActionVariant(14, 1);
  *(volatile u16 *)0x04000008 = 0x40d;
  *(volatile u16 *)0x04000008 &= ~3;
  *(volatile u16 *)0x04000008 |= 1;
  zero = 16;
  Dma_Set(&zero, (void *)0x06002000, 0x81000280, REG_DMA3);
  buffer = MemAlloc(576);
  Func_0200479c(Resource_GetTableEntry(0x1bf), buffer);
  for (i = 0; i != 288; i++)
    buffer[i] = ((buffer[i] - 0xc0) & 0xfff) | 0x1000;
  for (i = 0; i != 18; i++)
    Dma_Set(buffer + i * 16, (void *)(0x0600200f + (i << 6)), 0x84000008,
            REG_DMA3);
  MemFree(buffer);
  Engine_AudioPlayCue(246);
  OBJ14_BYTE(0x62) = 9;
  OBJ14_BYTE(0x63) = 1;
  Object_GetById(14)->update = Func_02000458;
  for (i = 0; i != 45; i++) {
    u32 frame = gFrameCount & 3;

    if (frame == 0) {
      effect = (struct FieldEffect *)Engine_ObjectCreate(
          672, Object_GetById(14)->x, Object_GetById(14)->y,
          Object_GetById(14)->z);
      source = Object_GetById(14);
      ((struct FieldSprite *)effect->sprite)->priority =
          ((struct FieldSprite *)source->animation)->priority;
      effect->priorityFlags |= 1;
      effect->motionFlags = frame;
      effect->velocity.x = effect->x;
      effect->velocity.y = 0x18000;
      effect->velocity.z = effect->z;
      effect->scaleRateX = Random16Far() & 0xffff;
      Engine_ObjectSetMode((struct FieldActor *)effect, 7);
      Engine_ObjectSetScript((struct FieldActor *)effect, gData_02004ae0);
      ObjectDispatch_SetSingleChildFlags((struct FieldActor *)effect, 0);
      Object_SetPartAttribute(effect, 2);
      effect->update = Effect_CircleAndBlink;
    }
    FieldEvent_WaitTicks(1);
  }
  Camera_FollowActor(gPartyState.currentOwner, 1);
  Func_020049f4();
  Func_020049e4(0x10000, 0x2000);
  Camera_MoveWithinMapBounds(168 << 16, -1, 212 << 17, 1);
  Func_020049f4();
  Scheduler_AddOrUpdateCallback(Func_02000378, 0xc80);
  Engine_AudioPlayCue(207);
  REG_BLDALPHA = 0xf00;
  REG_BLDCNT = 0x3f41;
  REG_DISPCNT |= 0x100;
  Func_02004864(0x10000, 0x10000, 0x10000);
  for (k = 0; k != 64; k++) {
    REG_BLDALPHA = ((15 - (k >> 2)) << 8) | (k >> 2);
    OBJ14_BYTE(0x62) = Random16Far() & 7;
    FieldEvent_WaitTicks(1);
  }
  FieldEvent_WaitTicks(60);
  Func_02004864(0x30000, 0x30000, 0x10000);
  REG_BLDALPHA = 0x1000;
  REG_BLDCNT = 0x3f42;
  Func_02004844(2, 83, 36, 64, 16, 16);
  Func_02004844(64, 64, 2, 83, 16, 16);
  *(volatile u16 *)0x0400000a &= ~3;
  Scheduler_AddOrUpdateCallback(Func_02000420, 0x480);
  for (k = 0; k != 68; k++) {
    REG_BLDALPHA = ((k >> 2) << 8) | (16 - (k >> 2));
    OBJ14_BYTE(0x62) = Random16Far() & 7;
    FieldEvent_WaitTicks(1);
  }
  Func_02004844(45, 90, 4, 19, 13, 11);
  *(volatile u16 *)0x0400000c = 0x602;
  Dma_Set((void *)0x06002800, (void *)0x06003000, 0x84000200, REG_DMA3);
  REG_BLDALPHA = 16;
  REG_BLDCNT = 0xa44;
  for (i = 0; i != 8; i++) {
    Func_02004844((i % 4) * 14 + 45, (i / 4) * 12 + 90, 4, 19, 13, 11);
    for (j = 0; j != 64; j++) {
      if ((j & 3) == 0) {
        options.priority = 1;
        options.update = Func_02000508;
        velocity[0] = 0;
        velocity[1] = 0;
        velocity[2] = -0x20000;
        x = ((Random16Far() & 127) << 16) + (208 << 15);
        Func_020000b8(x, 0, (472 - (Random16Far() & 127)) << 16, velocity[0],
                      velocity[1], velocity[2], 0x1020001, &options);
      }
      REG_BLDALPHA = ((j >> 2) << 8) | (16 - (j >> 2));
      OBJ14_BYTE(0x62) = Random16Far() & 7;
      WaitTicks(1);
    }
    Dma_Set((void *)0x06002800, (void *)0x06003000, 0x84000200, REG_DMA3);
    REG_BLDALPHA = 16;
  }
  *(volatile u16 *)0x0400000c = 0x60a;
  REG_DISPCNT &= ~0x400;
  Func_02004844(47, 19, 38, 19, 9, 11);
  REG_BLDALPHA = 16;
  REG_BLDCNT = 0x3f42;
  for (k = 0; k != 68; k++) {
    REG_BLDALPHA = ((k >> 2) << 8) | (16 - (k >> 2));
    OBJ14_BYTE(0x62) = Random16Far() & 7;
    FieldEvent_WaitTicks(1);
  }
  Scheduler_RemoveCallbackFar(Func_02000420);
  *(volatile u16 *)0x0400000a &= ~3;
  *(volatile u16 *)0x0400000a |= 1;
  Func_02004844(36, 64, 2, 83, 16, 16);
  REG_BLDCNT = 0x3f41;
  REG_BLDALPHA = 15;
  Func_02004864(-1, -1, 0xe666);
  Engine_AudioPlayCue(390);
  OBJ14_BYTE(0x62) = 6;
  for (k = 0; k != 65; k++) {
    REG_BLDALPHA = ((k >> 2) << 8) | (16 - (k >> 2));
    FieldEvent_WaitTicks(1);
  }
  Scheduler_RemoveCallbackFar(Func_02000378);
  OBJ14_BYTE(0x63) = 0;
  WaitTicks(30);
  Object_GetById(14)->update = 0;
  Object_SetPartAttribute(Object_GetById(14), 6);
  Func_0200486c();
  FieldEvent_WaitTicks(60);
  REG_BLDCNT = 0;
  Func_020047d4();
  Func_020047bc();
  Func_0200488c(gPartyState.paletteGlow[0], gPartyState.paletteGlow[1]);
  Object_GetById(gPartyState.currentOwner)->priorityFlags &= ~32;
  Object_GetById(gPartyState.currentOwner)->priorityFlags |= 1;
  Object_GetById(14)->priorityFlags |= 1;
  REG_DISPCNT |= 0x400;
  Func_0200482c();
  WaitTicks(1);
  Camera_FollowActor(gPartyState.currentOwner, 1);
  Func_020049f4();
  Func_0200484c(1, 5, 9, 1, 6, 29);
  GameFlag_SetBit(2380);
}
