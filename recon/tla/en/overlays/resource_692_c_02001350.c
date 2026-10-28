// Draft, differ 553 of 537: a Magma Rock room's event at cell 222 (reached from
// Func_02000ed0), by room (2, 3 or 4) with its place and flag (768 to 770):
// sixteen effects burst with sound 145 and shaking, eight rise (sound 148), the
// cells are copied open and closed again; with the flag set the effects carry
// on briefly, otherwise twelve rows of six rise as the opening widens, then
// over 160 frames the map's layer 1 scrolls, the blend fades and the cells
// step back, with more effects every fourth frame; finally sounds 288 and 136
// with six more effects, the cells close, layer 1 returns and the room's own
// closing runs (Func_020034bc, Func_020034f0 or Func_02003548).
#include "types.h"
#include "io_reg.h"
#include "ram_buffer.h"
#include "sequence_parent_work.h"
#include "field_effect.h"

extern char SceneId_Ov692Room2;
extern char SceneId_Ov692Room3;
extern char SceneId_Ov692Room4;
extern const s32 gData_02005050[];
extern const s32 gData_0200502c[];

s32 GameFlag_Test(s32 flag);
void wait(s32 frames);
void FieldEvent_Wait(s32 frames);
void Func_02004f30(s32 sound);
void Func_02004d10(s32 a, s32 b, s32 c);
void Func_02004d18(void);
void Func_02004cc0(void);
void Func_02004cd8(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
u32 Random16Far(void);
void Func_0200015c(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags,
                   const struct EffectOptions *options);
void Func_020034bc(void);
void Func_020034f0(void);
void Func_02003548(void);

static __inline__ void Shake(void) {
  Func_02004d10(0x50000, 0x70000, 0x10000);
  Func_02004d10(-1, -1, 0xe666);
}

void Func_02001350(void) {
  u8 *layer = (u8 *)Ram_HeapSlots->mapWork + 320;
  struct EffectOptions options;
  s32 position[3];
  s32 baseX = 0;
  s32 srcX = 0;
  s32 srcZ = 0;
  s32 open = 0;
  s32 scene;
  s32 x;
  s32 i;
  s32 j;
  s32 t;

  options.type = 403;
  options.priority = 1;
  options.script = gData_02005050;
  options.startScaleX = 0x1cccc;
  options.startScaleY = 0x1cccc;
  scene = gSequenceParentWork.scene;
  if (scene == (s32)&SceneId_Ov692Room2) {
    baseX = 164 << 17;
    position[1] = 0x200000;
    position[2] = 200 << 16;
    srcX = 20;
    srcZ = 69;
    if (GameFlag_Test(768))
      open = 212;
  } else if (scene == (s32)&SceneId_Ov692Room3) {
    baseX = 232 << 16;
    position[1] = 0x200000;
    position[2] = 248 << 16;
    srcX = 14;
    srcZ = 72;
    if (GameFlag_Test(769))
      open = 212;
  } else if (scene == (s32)&SceneId_Ov692Room4) {
    baseX = 196 << 17;
    position[1] = 0x200000;
    position[2] = 134 << 18;
    srcX = 24;
    srcZ = 90;
    if (GameFlag_Test(770))
      open = 212;
  }
  for (i = 0; i != 16; i++) {
    position[0] = baseX + ((Random16Far() & 31) << 16);
    Func_02004f30(145);
    Func_0200015c(position[0], position[1], position[2],
                  (Random16Far() & 31) << 12, 0, 0, 0x3a0001, &options);
    Func_02004d10(0x50000, 0x50000, 0x10000);
    Func_02004d10(-1, -1, 0xe666);
    FieldEvent_Wait(8);
  }
  Func_02004f30(148);
  options.priority = 1;
  options.startScaleX = 0x20000;
  options.startScaleY = 0x20000;
  options.targetScaleX = 0x8000;
  options.targetScaleY = 0x8000;
  baseX -= 0x80000;
  x = baseX;
  for (i = 0; i != 8; i++) {
    position[0] = x;
    Func_0200015c(position[0], position[1], position[2], 0,
                  (Random16Far() & 31) << 10, 0, 0x380001, &options);
    x += 0x60000;
  }
  Func_02004cd8(68, 70, srcX, srcZ, 3, 6);
  Shake();
  FieldEvent_Wait(30);
  Func_02004cd8(71, 70, srcX, srcZ, 3, 6);
  options.type = 403;
  options.priority = 1;
  options.palette = 10;
  options.script = gData_0200502c;
  if (open != 0) {
    Func_02004cd8(68, 70, srcX, srcZ, 3, 6);
    FieldEvent_Wait(30);
    position[2] -= 0x280000;
  } else {
    Func_02004d10(0x20000, 0x10000, 0x10000);
    Func_02004f30(107);
    for (i = 0; i != 12; i++) {
      for (j = 0; j != 6; j++) {
        position[0] = baseX + ((Random16Far() % 48) << 16);
        Func_0200015c(position[0], position[1],
                      position[2] + (i << 18) + 0x80000, 0, 0x10000, 0,
                      0x330000, &options);
      }
      wait(3);
      Func_02004cd8(74, 70, srcX, srcZ, 3, (i >> 2) + 6);
    }
    Func_02004cd8(74, 70, srcX, srcZ, 3, 8);
    for (t = 0; t != 160; t++) {
      if ((t & 3) == 0) {
        for (j = 0; j != 4; j++) {
          position[0] = baseX + ((Random16Far() % 48) << 16);
          Func_0200015c(position[0], position[1], position[2] + 0x300000, 0,
                        0x10000, 0, 0x330000, &options);
        }
      }
      if (t == 0) {
        *(s32 *)(layer + 12) -= 0x200000;
        Func_02004cc0();
        FieldEvent_Wait(1);
      } else if ((u32)t <= 63) {
        REG_BLDALPHA = ((t >> 2) << 8) | (16 - (t >> 2));
        REG_BLDCNT = 0x3f44;
      } else if (t == 64) {
        REG_BLDCNT = 0;
      } else if ((u32)t <= 127) {
        *(s32 *)(layer + 12) += 0x8000;
        position[2] -= 0x8000;
        Func_02004cd8(71, 76, srcX, srcZ - ((t - 47) >> 5) + 8, 3, 1);
      } else if (t == 128) {
        Func_02004d10(-1, -1, 0xe666);
      }
      wait(1);
    }
    Func_02004d18();
  }
  Func_02004f30(288);
  Func_02004f30(136);
  options.startScaleY = 0x1cccc;
  options.startScaleX = 0x1cccc;
  for (i = 0; i != 6; i++) {
    position[0] = baseX + ((Random16Far() % 48) << 16);
    Func_0200015c(position[0], position[1], position[2] + 0x300000, 0, 0, 0,
                  0x3b0000, &options);
  }
  Shake();
  Func_02004cd8(68, 64, srcX, srcZ, 3, 6);
  Func_02004d18();
  FieldEvent_Wait(30);
  *(s32 *)(layer + 12) = 0;
  scene = gSequenceParentWork.scene;
  if (scene == (s32)&SceneId_Ov692Room2)
    Func_020034bc();
  else if (scene == (s32)&SceneId_Ov692Room3)
    Func_020034f0();
  else if (scene == (s32)&SceneId_Ov692Room4)
    Func_02003548();
}
