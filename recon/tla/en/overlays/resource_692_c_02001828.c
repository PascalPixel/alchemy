// Draft, differ 503 of 496: a Magma Rock room's opening event: the leader is centred on its cell, sound 584, the screen shakes
// (sound 107); by room (2, 3 or 5, the last by entrance) it picks the cells to
// open, the flag that was set (768 to 770, cleared here) and where the effects
// play; sound 181 with the shaking, the cells are copied open; if that flag was
// set, ten rows of six effects rise along the opening while it widens and the
// blend fades over 64 steps; then sound 288 and 181, the cells close again,
// sound 584, and the leader steps back a cell.
#include "types.h"
#include "io_reg.h"
#include "object_runtime.h"
#include "adventure.h"
#include "field_effect.h"

extern char SceneId_Ov692Room2;
extern char SceneId_Ov692Room3;
extern char SceneId_Ov692Room5;
extern const s32 gData_0200502c[];

struct ObjectRuntime *Object_GetById(s32 id);
s32 GameFlag_Test(s32 flag);
void GameFlag_ClearBit(s32 flag);
void WaitFrames(s32 frames);
void FieldEvent_WaitFrames(s32 frames);
void Func_02004d40(void);
void Func_02004d48(void);
void Func_02004d18(void);
void Func_02004e90(s32 value);
void Func_02004f30(s32 sound);
void Func_02004d10(s32 a, s32 b, s32 c);
void ObjectMotion_SetPositionAndReset(s32 id, s32 x, s32 z);
void ObjectMotion_SetActionVariant(u32 id, s32 variant);
void Object_SetModeById(u32 id, s32 mode);
void Func_02000694(s32 layer, s32 x, s32 z, s32 mask);
void Func_02004cd8(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
u32 Random16Far(void);
void Func_0200015c(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags,
                   const struct EffectOptions *options);
void Vector_AddPolarOffsetFar(s32 distance, s32 angle, s32 *vector);

void Func_02001828(void) {
  struct ObjectRuntime *leader = Object_GetById(gAdventure.currentOwner);
  u32 facing = (leader->angle + 0x2000) & 0xc000;
  struct EffectOptions options;
  s32 position[3];
  s32 baseX = 0;
  s32 dstX = 0;
  s32 dstZ = 0;
  s32 open = 0;
  s32 srcX = 0;
  s32 srcZ = 0;
  u8 *flags;
  s32 scene;
  s32 i;
  s32 j;

  Func_02004d40();
  Func_02004e90(0);
  position[0] = (leader->x & 0xfff00000) + 0x80000;
  position[2] = (leader->z & 0xfff00000) + 0x80000;
  ObjectMotion_SetPositionAndReset(gAdventure.currentOwner,
                                   position[0] >> 16, position[2] >> 16);
  FieldEvent_WaitFrames(20);
  Func_02004f30(584);
  flags = &leader->priorityFlags;
  *flags &= ~64;
  FieldEvent_WaitFrames(20);
  Func_02004d10(0x10000, 0x20000, 0x10000);
  Func_02004f30(107);
  FieldEvent_WaitFrames(30);
  scene = gAdventure.scene;
  if (scene == (s32)&SceneId_Ov692Room2) {
    dstZ = 26;
    srcX = 20;
    dstX = 84;
    baseX = 160 << 17;
    position[1] = 0;
    position[2] = 212 << 17;
    srcZ = 89;
    if (GameFlag_Test(768))
      open = 212;
    GameFlag_ClearBit(768);
    facing = 0;
    Func_02000694(0, 15, 13, 64);
    ObjectMotion_SetActionVariant(15, 3);
    for (i = 0; i != 7; i++)
      Object_SetModeById(i + 9, 1);
  } else if (scene == (s32)&SceneId_Ov692Room3) {
    dstX = 92;
    baseX = 224 << 17;
    dstZ = 21;
    position[1] = 0;
    position[2] = 172 << 17;
    srcX = 28;
    srcZ = 83;
    if (GameFlag_Test(769))
      open = 212;
    GameFlag_ClearBit(769);
    Func_02000694(0, 24, 15, 64);
    Func_02000694(0, 9, 16, 64);
    Object_SetModeById(11, 4);
  } else if (scene == (s32)&SceneId_Ov692Room5) {
    if (gAdventure.entrance <= 8) {
      dstZ = 13;
      srcX = 49;
      dstX = 113;
      baseX = 198 << 18;
      position[1] = 0;
      position[2] = 216 << 16;
      srcZ = 75;
    } else {
      dstX = 115;
      baseX = 206 << 18;
      dstZ = 42;
      position[1] = 0;
      position[2] = 170 << 18;
      srcX = 51;
      srcZ = 104;
    }
    if (GameFlag_Test(770))
      open = 212;
    GameFlag_ClearBit(770);
  }
  Func_02004f30(181);
  Func_02004d10(0x50000, 0x70000, 0x10000);
  Func_02004d10(-1, -1, 0xe666);
  Func_02004cd8(srcX, srcZ, srcX, srcZ - 1, 3, 4);
  if (open == 0) {
    Func_02004d18();
    FieldEvent_WaitFrames(20);
  } else {
    Func_02004d10(0x20000, 0x10000, 0x10000);
    options.type = 403;
    options.priority = 2;
    options.palette = 10;
    options.script = gData_0200502c;
    for (i = 0; i != 10; i++) {
      for (j = 0; j != 6; j++) {
        position[0] = baseX + ((Random16Far() % 48) << 16);
        Func_0200015c(position[0], position[1],
                      position[2] + (i << 18) + 0x170000, 0, 0x10000, 0,
                      0x330000, &options);
      }
      WaitFrames(3);
      Func_02004cd8(75, 64, dstX, dstZ, 3, (i >> 1) + 1);
    }
    for (i = 0; i != 64; i++) {
      REG_BLDALPHA = ((i >> 2) << 8) | (16 - (i >> 2));
      REG_BLDCNT = 0x3f44;
      WaitFrames(3);
    }
    Func_02004cd8(78, 64, dstX, dstZ, 3, 5);
  }
  if (gAdventure.scene == (s32)&SceneId_Ov692Room2)
    Object_GetById(15)->priorityFlags |= 1;
  Func_02004f30(288);
  Func_02004f30(181);
  Func_02004d10(0x50000, 0x70000, 0x10000);
  Func_02004d10(-1, -1, 0xe666);
  Func_02004cd8(72, 63, srcX, srcZ - 1, 3, 4);
  Func_02004d18();
  Func_02004f30(584);
  *flags |= 64;
  FieldEvent_WaitFrames(20);
  Object_GetById(gAdventure.currentOwner)->actionFlags &= ~1;
  position[0] = (leader->x & 0xfff00000) + 0x80000;
  position[1] = leader->y;
  position[2] = (leader->z & 0xfff00000) + 0x80000;
  Vector_AddPolarOffsetFar(0x100000, facing ^ 0x8000, position);
  leader->angle = facing;
  ObjectMotion_SetPositionAndReset(gAdventure.currentOwner,
                                   ((s16 *)position)[1], ((s16 *)position)[5]);
  Object_GetById(gAdventure.currentOwner)->actionFlags |= 1;
  Func_02004d48();
}
