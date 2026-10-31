// Near miss, differ 319 of 537 (tla-en, 2026-10-31): the calls, branches,
// loops, fields and constants follow the native; what is left is GCSE and
// register allocation. GCSE's PRE forms the coordinates' address once in each
// room's branch and carries that one pseudo (r6) through the whole function,
// where the native builds it before the room tests (r2), again in the burst
// loop (r6, kept through the rise, the rows and the flag-set path), and afresh
// in each pass of the spark and last loops; PRE also hoists the row's i << 18
// out of the column loop into r8, which pushes the options address to r10 and
// t to r8 (native: options r8, t r10, the row counter r5, the column r7).
// The native spills baseX (sp + 20) and the layer pointer (sp + 16) and keeps
// srcX in r11 and srcZ in r9; here baseX takes r9 and the layer r11 (17 refs
// over 756 insns against srcX's 13 over 738), srcX and srcZ go to the stack,
// so rooms 2 and 4 no longer share the GameFlag_Test tail. After the opening
// the native rebuilds the options address (add r6, sp, #36); here r10 again.
// Tried (2026-10-31): calls made directly instead of through the forwarding
// wrappers (388), for loops (431), t apart from the flag result (340),
// baseX declared before srcX and srcZ (325), srcZ before srcX or j before i
// (319), the coordinates as a struct Vector3 (319), a pointer to them set
// before the room tests for the rooms' stores (307, a temporary: PRE then
// unifies it with every later use, not the native's two pseudos).
// A Magma Rock room's event at cell 222 (reached from Func_02000ed0), by room
// (2, 3 or 4) with its place and flag (768 to 770): sixteen effects burst with
// sound 145 and shaking, eight rise (sound 148), the cells are copied open and
// closed again; with the flag set the effects carry on briefly, otherwise
// twelve rows of six rise as the opening widens, then over 160 frames the
// map's layer 1 scrolls, the blend fades and the cells step back, with more
// effects every fourth frame; finally sounds 288 and 136 with six more
// effects, the cells close, layer 1 returns and the room's own closing runs
// (Func_020034bc, Func_020034f0 or Func_02003548).
#include "types.h"
#include "io_reg.h"
#include "ram_buffer.h"
#include "main_routine_work.h"
#include "field_effect.h"
#include "map.h"

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

static __inline__ void Func_02004d10_(s32 a, s32 b, s32 c) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02004d10(a, b, c);
}

static __inline__ void Func_02004cd8_(s32 a, s32 b, s32 c, s32 d, s32 e,
                                      s32 f) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02004cd8(a, b, c, d, e, f);
}

void Func_02001350(void) {
  s32 srcX = 0;
  s32 srcZ = 0;
  s32 baseX = 0;
  struct MapLayer *layer = &Ram_HeapSlots->mapWork->layers[1];
  struct EffectOptions options;
  s32 position[3];
  // 212 when the room's flag is set, then the frame count of the opening: the
  // native keeps both in r10 (as two variables: differ 340).
  u32 t = 0;
  s32 scene;
  s32 x;
  u32 i;
  u32 j;

  options.type = 403;
  options.priority = 1;
  options.script = gData_02005050;
  options.startScaleX = 0x1cccc;
  options.startScaleY = 0x1cccc;
  scene = gMainRoutineWork.scene;
  if (scene == (s32)&SceneId_Ov692Room2) {
    baseX = 164 << 17;
    position[1] = 0x200000;
    position[2] = 200 << 16;
    srcX = 20;
    srcZ = 69;
    if (GameFlag_Test(768))
      t = 212;
  } else if (scene == (s32)&SceneId_Ov692Room3) {
    baseX = 232 << 16;
    position[1] = 0x200000;
    position[2] = 248 << 16;
    srcX = 14;
    srcZ = 72;
    if (GameFlag_Test(769))
      t = 212;
  } else if (scene == (s32)&SceneId_Ov692Room4) {
    baseX = 196 << 17;
    position[1] = 0x200000;
    position[2] = 134 << 18;
    srcX = 24;
    srcZ = 90;
    if (GameFlag_Test(770))
      t = 212;
  }
  // The loops are written with goto: they carry no loop notes, so the masks,
  // the shake constants and the counters' shifts stay in each pass, as in the
  // native; as for loops, 431 of 537 instructions differ in tla-en.
  i = 0;
burst:
  position[0] = baseX + ((Random16Far() & 31) << 16);
  Func_02004f30(145);
  Func_0200015c(position[0], position[1], position[2], 0,
                (Random16Far() & 31) << 12, 0, 0x3a0001, &options);
  Func_02004d10_(0x50000, 0x50000, 0x10000);
  Func_02004d10_(-1, -1, 0xe666);
  i++;
  FieldEvent_Wait(8);
  if (i != 16)
    goto burst;
  Func_02004f30(148);
  options.priority = 1;
  options.startScaleX = 0x20000;
  options.startScaleY = 0x20000;
  options.targetScaleX = 0x8000;
  options.targetScaleY = 0x8000;
  baseX -= 0x80000;
  i = 0;
  x = baseX;
rise:
  position[0] = x;
  Func_0200015c(position[0], position[1], position[2], 0, 0,
                (Random16Far() & 31) << 10, 0xe0001, &options);
  i++;
  x += 0x60000;
  if (i != 8)
    goto rise;
  Func_02004cd8_(68, 70, srcX, srcZ, 3, 6);
  Func_02004d10_(0x50000, 0x70000, 0x10000);
  Func_02004d10_(-1, -1, 0xe666);
  FieldEvent_Wait(30);
  Func_02004cd8_(71, 70, srcX, srcZ, 3, 6);
  options.type = 403;
  options.priority = 1;
  options.palette = 10;
  options.script = gData_0200502c;
  if (t != 0) {
    Func_02004cd8_(68, 70, srcX, srcZ, 3, 6);
    FieldEvent_Wait(30);
    position[2] -= 0x280000;
  } else {
    Func_02004d10_(0x20000, 0x10000, 0x10000);
    Func_02004f30(107);
    i = 0;
  row:
    j = 0;
  column:
    position[0] = baseX + ((Random16Far() % 48) << 16);
    j++;
    Func_0200015c(position[0], position[1], position[2] + (i << 18) + 0x80000,
                  0, 0x10000, 0, 0x330000, &options);
    if (j != 6)
      goto column;
    wait(3);
    Func_02004cd8_(74, 70, srcX, srcZ, 3, (i >> 2) + 6);
    i++;
    if (i != 12)
      goto row;
    Func_02004cd8_(74, 70, srcX, srcZ, 3, 8);
    t = 0;
  frame:
    if ((t & 3) == 0) {
      j = 0;
    spark:
      position[0] = baseX + ((Random16Far() % 48) << 16);
      j++;
      Func_0200015c(position[0], position[1], position[2] + 0x300000, 0,
                    0x10000, 0, 0x330000, &options);
      if (j != 4)
        goto spark;
    }
    if (t == 0) {
      *(s32 *)&layer->unk08[4] -= 0x200000;
      Func_02004cc0();
      FieldEvent_Wait(1);
    } else if (t <= 63) {
      REG_BLDALPHA = ((16 - (t >> 2)) << 8) | (t >> 2);
      REG_BLDCNT = 0x3f44;
    } else if (t == 64) {
      REG_BLDCNT = 0;
    } else if (t <= 127) {
      *(s32 *)&layer->unk08[4] += 0x8000;
      position[2] -= 0x8000;
      Func_02004cd8_(71, 76, srcX, srcZ - ((t - 47) >> 5) + 8, 3, 1);
    } else if (t == 128) {
      Func_02004d10_(-1, -1, 0xe666);
    }
    t++;
    wait(1);
    if (t != 160)
      goto frame;
    Func_02004d18();
  }
  Func_02004f30(288);
  Func_02004f30(136);
  options.startScaleY = 0x1cccc;
  options.startScaleX = 0x1cccc;
  i = 0;
last:
  position[0] = baseX + ((Random16Far() % 48) << 16);
  i++;
  Func_0200015c(position[0], position[1], position[2] + 0x300000, 0, 0, 0,
                0x3b0000, &options);
  if (i != 6)
    goto last;
  Func_02004d10_(0x50000, 0x70000, 0x10000);
  Func_02004d10_(-1, -1, 0xe666);
  Func_02004cd8_(68, 64, srcX, srcZ, 3, 6);
  Func_02004d18();
  FieldEvent_Wait(30);
  *(s32 *)&layer->unk08[4] = 0;
  scene = gMainRoutineWork.scene;
  if (scene == (s32)&SceneId_Ov692Room2)
    Func_020034bc();
  else if (scene == (s32)&SceneId_Ov692Room3)
    Func_020034f0();
  else if (scene == (s32)&SceneId_Ov692Room4)
    Func_02003548();
}
