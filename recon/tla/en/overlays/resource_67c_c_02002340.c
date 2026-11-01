// Draft, differ 59 of 931, skeleton gap 7/6 (tla-en, 2026-11-01)
// Aqua Rock's map preparation: for each room and entrance it sets the water
// level, places the statues and boulders the flags have moved, opens or closes
// the cells around them, and in the rooms whose water drains it shows the
// drained state once its flag is set.
// Left: BG1_SET_PRIORITY_1's stacked value. The listing keeps a u16 at sp+10
// (mov r1, sp; adds r1, #10; strh) that is stored and never reloaded. A scalar
// only stays in memory with its address taken (*&control gives differ 36, gap
// 7/7, with registers matched), and that is a device; u16 control[1] loses the
// store; u16 control[2] keeps it as add r1, sp, #8; strh r3, [r1, #2].
// Also left: the actor-clearing loops of rooms 4 and 6. As for loops loop.c
// hoists their movs #0, and the room 9 loop as a for also brings adds r1, #2
// into the prologue, so they are goto loops; a jump threads over their exit,
// giving bgt in room 4 and one extra b in room 6. A zero local spills.
// The Func_020036b8 loops are for loops counting up: loop.c reverses the i < N
// ones and leaves room 5's i <= 4, as the listing has them. Each i is a
// separate variable on purpose: one shared counter takes r6 and pushes object
// into r5.
#include "iwram_call.h"
#include "call.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "scene_ids.h"
#include "scene_tables.h"
#include "system/io_reg.h"
#include "text/msg_ids.h"
#include "types.h"

struct MapBits {
  u8 unk00[23];
  u8 low : 2;
  u8 mode : 2;
};

#define BG1_SET_PRIORITY_1()                                                   \
  do {                                                                         \
    u16 control[2];                                                            \
    control[1] = REG_BG1CNT & 0xfffc;                                          \
    REG_BG1CNT = control[1] | 1;                                               \
  } while (0)

void Func_02003278(void);
void Func_020032d4(u32 first, s32 level, s32 baseLevel);
struct Actor *Actor_GetById(s32 id);
void Func_02003cc4(struct Actor *object, s32 mode);
s32 GameFlag_Test(s32 flag);
void Func_02003d24(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_02003d1c(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_0200349c(u32 index, s32 height);
void Func_02003ddc(s32 id, s32 x, s32 z);
void Func_02003ea4();
void Actor_SetSingleChildFlags();
void Func_02003628(void);
s32 Func_02003688(s32 x, s32 y, s32 z, s32 kind);
s32 Func_020036b8(s32 a, s32 b);
void Func_020007a8(void);
void Func_02003420(s32 id, s32 kind, s32 height);
void Func_02000b84(void);
s32 Func_02000b24(s32 id, s32 value);
void GameFlag_SetBit(s32 flag);
void Func_02000ad8(void);
void Func_02003edc();
void Func_02003ee4();

s32 Func_02002340(void) {
  s32 scene = gMainRoutineWork.scene;
  s32 entrance = gMainRoutineWork.entrance;
  s32 i1, i2, i3, i4, i5, i6, i7, i8, i9, i10, i11, i12, i13, i14, i15, i16,
      i17;
  struct Actor *object;
  s32 a, b;
  u8 *map;

  ((struct MapBits *)Ram_HeapSlots->mapWork)->mode = 1;
  Func_02003278();
  if (scene == (s32)&SceneId_Ov67cRoom1) {
    if ((u32)(entrance - 3) <= 1 || entrance == 6 || entrance == 8) {
      Func_020032d4(33, 0, -2);
    } else if (entrance == 5 || entrance == 7) {
      object = Actor_GetById(12);
      i1 = 0;
      object->motionFlags = i1;
      object->terrainHeight = i1;
      object->y = i1;
      Call2((void (*)())Func_02003cc4, (s32)object, 0);
      object = Actor_GetById(13);
      object->motionFlags = i1;
      object->terrainHeight = i1;
      object->y = i1;
      i2 = GameFlag_Test(0x230);
      if (i2 == 0) {
        Call6((void (*)())Func_02003d24, 104, 80, 16, 10, 104, 40);
        Call6((void (*)())Func_02003d24, 110, 116, 18, 12, 39, 105);
        Call6((void (*)())Func_02003d1c, 64, 32, 64, 32, i2, 32);
        Func_020032d4(33, 0, 0);
      } else {
        Call3((void (*)())Func_020032d4, 33, 4, 0);
        object = Actor_GetById(12);
        Call2((void (*)())Func_0200349c, (object->x >> 20) - 42, 0);
      }
      if (GameFlag_Test(0x250)) {
        Call6((void (*)())Func_02003d1c, 41, 42, 1, 1, 43, 44);
      }
    } else if ((u32)(entrance - 9) <= 1) {
      a = GameFlag_Test(0x231);
      if (a == 0) {
        i3 = 32;
        Call6((void (*)())Func_02003d24, 77, 80, 23, 20, 77, i3);
        Call6((void (*)())Func_02003d24, 112, 96, 16, 20, 12, 98);
        Call6((void (*)())Func_02003d1c, 64, 32, 64, 32, a, i3);
        Func_020032d4(33, 0, 0);
      } else {
        Func_020032d4(33, 4, 0);
      }
    }
    if (GameFlag_Test(0x95a)) {
      Call3((void (*)())Func_02003ddc, 10, PIXELS(184), PIXELS(200));
      i4 = 12;
      Call6((void (*)())Func_02003d1c, 0, 12, 1, 1, 9, i4);
      Call6((void (*)())Func_02003d1c, 0, 12, 1, 1, 13, i4);
    }
    if (GameFlag_Test(0x95b)) {
      Call3((void (*)())Func_02003ddc, 11, PIXELS(824), PIXELS(216));
    }
  } else if (scene == (s32)&SceneId_Ov67cRoom2) {
    if ((u32)(entrance - 1) <= 1) {
      Func_020032d4(33, 0, -2);
    }
    object = Actor_GetById(8);
    object->motionFlags = 0;
    object->terrainHeight = 0;
    object->y = 0;
  } else if (scene == (s32)&SceneId_Ov67cRoom3) {
    Call1((void (*)())Func_02003ea4, 0);
    Call2((void (*)())Actor_SetSingleChildFlags, (s32)Actor_GetById(9), 0);
    Call2((void (*)())Actor_SetSingleChildFlags, (s32)Actor_GetById(10), 0);
    Func_020032d4(33, 8, -2);
    Call3((void (*)())Func_020032d4, 40, 8, 8);
    if (GameFlag_Test(0xa66)) {
      Func_02003628();
      Value4((s32 (*)())Func_02003688, 16, 4, 16, 2);
      Value4((s32 (*)())Func_02003688, 22, 4, 16, 0);
      for (i5 = 0; i5 < 6; i5++)
        Func_020036b8(33, 55);
    }
    if (GameFlag_Test(0x95d)) {
      Call6((void (*)())Func_02003d24, 75, 96, 7, 13, 80, 27);
      i6 = 16;
      b = 31;
      Call6((void (*)())Func_02003d24, 91, 96, 7, 10, i6, b);
      Call6((void (*)())Func_02003d24, 99, 96, 7, 14, i6, 91);
      Call6((void (*)())Func_02003d1c, 43, 0, 5, 9, 17, b);
      Func_020007a8();
    } else {
      Call6((void (*)())Func_02003d1c, 17, 32, 5, 1, 17, 31);
    }
  } else if (scene == (s32)&SceneId_Ov67cRoom4) {
    Call1((void (*)())Func_02003ea4, 0);
    if ((u32)(entrance - 6) <= 1)
      goto statues;
    goto no_statues;
  statues:
    {
      Actor_GetById(gMainRoutineWork.currentOwner)->priorityFlags |= 32;
      Value3((s32 (*)())Func_02003420, 12, 5, 4);
      Value3((s32 (*)())Func_02003420, 13, 3, 4);
      Value3((s32 (*)())Func_02003420, 14, 4, 4);
      Func_020032d4(40, 4, 4);
      if (!GameFlag_Test(0x249) && !Value1((s32 (*)())GameFlag_Test, 0xa64)) {
        map = (u8 *)Ram_HeapSlots->mapWork;
        i7 = 39;
        Call6((void (*)())Func_02003d24, 64, 64, 16, 13, 71, i7);
        Call6((void (*)())Func_02003d1c, 71, 39, 16, 15, 7, i7);
        Func_02000b84();
        map[262] = 1;
        BG1_SET_PRIORITY_1();
      } else if (Value1((s32 (*)())GameFlag_Test, 0xa64) ||
                 GameFlag_Test(0x248)) {
        if (Value1((s32 (*)())GameFlag_Test, 0xa64)) {
          Call3((void (*)())Func_02003ddc, 12, PIXELS(280), PIXELS(712));
        }
        Func_02000b24(12, 38);
        Func_02003628();
        Value4((s32 (*)())Func_02003688, 12, 2, 45, 1);
        for (i8 = 0; i8 < 8; i8++)
          Func_020036b8(33, 55);
        Call1((void (*)())GameFlag_SetBit, 0x249);
      }
      i9 = 12;
    clear_statues:
      object = Actor_GetById(i9);
      object->motionFlags = 0;
      object->terrainHeight = 0;
      object->y = 0;
      i9++;
      if (i9 <= 14)
        goto clear_statues;
      goto statues_cleared;
    }
  no_statues:
    Func_020032d4(33, 0, -4);
  statues_cleared:
    if (GameFlag_Test(0xa6a)) {
      Func_02000ad8();
    }
  } else if (scene == (s32)&SceneId_Ov67cRoom5) {
    Func_020032d4(33, 0, -2);
    Call3((void (*)())Func_020032d4, 40, 0, 0);
    Call1((void (*)())Func_02003ea4, 0);
    if (((u32)(entrance - 1) <= 3 || entrance == 7) && GameFlag_Test(0xa67)) {
      Func_02003628();
      Value4((s32 (*)())Func_02003688, 44, 0, 34, 2);
      for (i10 = 0; i10 <= 4; i10++)
        Func_020036b8(33, 55);
    }
  } else if (scene == (s32)&SceneId_Ov67cRoom6) {
    Call1((void (*)())Func_02003ea4, 0);
    if (entrance == 1 || entrance == 13) {
      Func_020032d4(33, 0, -2);
    } else if (entrance == 9 || entrance == 11 || entrance == 12) {
      Func_020032d4(33, 0, -2);
    }
    if (!GameFlag_Test(0x238)) {
      i11 = 13;
    clear_boulders:
      object = Actor_GetById(i11);
      object->motionFlags = 0;
      object->x = 0;
      object->z = 0;
      i11++;
      if (i11 <= 15)
        goto clear_boulders;
      goto done;
    } else {
      Call6((void (*)())Func_02003d24, 114, 39, 1, 2, 113, 39);
      i12 = 41;
      Call6((void (*)())Func_02003d1c, 105, 41, 12, 13, i12, i12);
      Call6((void (*)())Func_02003d24, 105, 105, 12, 13, 105, i12);
      for (i13 = 13; i13 <= 15; i13++) {
        object = Actor_GetById(i13);
        if (GameFlag_Test(i13 + 580)) {
          Call6((void (*)())Func_02003d1c, 47, 45, 1, 1, object->x >> 20,
                object->z >> 20);
        }
      }
    }
  } else if (scene == (s32)&SceneId_Ov67cRoom7) {
    Func_020032d4(33, 0, -2);
  } else if (scene == (s32)&SceneId_Ov67cRoom8) {
    Call1((void (*)())Func_02003ea4, 0);
    Func_020032d4(33, 0, -2);
    Func_02003edc();
    Call3((void (*)())Func_02003ee4, 0, 8, 9);
  } else if (scene == (s32)&SceneId_Ov67cRoom9) {
    if ((u32)(entrance - 3) <= 1) {
      Actor_GetById(gMainRoutineWork.currentOwner)->priorityFlags |= 32;
      Value3((s32 (*)())Func_02003420, 10, 6, 4);
      Value3((s32 (*)())Func_02003420, 11, 3, 4);
      Value3((s32 (*)())Func_02003420, 12, 5, 4);
      Value3((s32 (*)())Func_02003420, 13, 6, 4);
      Value3((s32 (*)())Func_02003420, 14, 5, 4);
      Value3((s32 (*)())Func_02003420, 15, 4, 4);
      Value3((s32 (*)())Func_02003420, 16, 4, 4);
      Value3((s32 (*)())Func_02003420, 17, 3, 4);
      Value3((s32 (*)())Func_02003420, 18, 6, 4);
      Func_020032d4(40, 4, 4);
      if (!GameFlag_Test(0x24b) && !Value1((s32 (*)())GameFlag_Test, 0xa65)) {
        map = (u8 *)Ram_HeapSlots->mapWork;
        i14 = 13;
        Call6((void (*)())Func_02003d24, 64, 64, 19, 17, 100, i14);
        Call6((void (*)())Func_02003d24, 35, 105, 23, 24, 35, 73);
        Call6((void (*)())Func_02003d1c, 100, 13, 19, 19, 36, i14);
        map[262] = 1;
        BG1_SET_PRIORITY_1();
      } else if (Value1((s32 (*)())GameFlag_Test, 0xa65) ||
                 GameFlag_Test(0x24a)) {
        if (Value1((s32 (*)())GameFlag_Test, 0xa65)) {
          Call3((void (*)())Func_02003ddc, 12, PIXELS(808), PIXELS(296));
          Call3((void (*)())Func_02003ddc, 13, PIXELS(680), PIXELS(360));
          Call3((void (*)())Func_02003ddc, 15, PIXELS(744), PIXELS(424));
        }
        Call2((void (*)())Func_02000b24, 12, 38);
        Call2((void (*)())Func_02000b24, 13, 39);
        Func_02000b24(15, 37);
        Func_02003628();
        Value4((s32 (*)())Func_02003688, 38, 2, 19, 1);
        for (i15 = 0; i15 < 8; i15++)
          Func_020036b8(33, 55);
        Call1((void (*)())GameFlag_SetBit, 0x24b);
      }
    }
    i16 = 10;
  clear_pillars:
    object = Actor_GetById(i16);
    object->motionFlags = 0;
    object->terrainHeight = 0;
    object->y = 0;
    i16++;
    if (i16 <= 20)
      goto clear_pillars;
    object = Actor_GetById(19);
    Call6((void (*)())Func_02003d1c, 23, 39, 1, 1, object->x >> 20, 39);
    if ((u32)(entrance - 1) <= 1) {
      Call1((void (*)())Func_02003ea4, 0);
    }
  } else if (scene == (s32)&SceneId_Ov67cRoom10) {
    Call1((void (*)())Func_02003ea4, 0);
    Func_020032d4(33, 0, -2);
    Call1((void (*)())GameFlag_SetBit, 0x144);
    if (GameFlag_Test(0x349)) {
      Call6((void (*)())Func_02003d1c, 38, 9, 7, 6, 13, 9);
    } else {
      i17 = 8;
      Call6((void (*)())Func_02003d24, 77, 8, 7, 6, 50, i17);
      Call6((void (*)())Func_02003d24, 70, 8, 7, 6, 77, i17);
      Call6((void (*)())Func_02003d1c, 38, 17, 7, 6, 13, 9);
    }
  }
done:
  return 0;
}
