/// @file
/// Aqua Rock's map preparation: for each room and entrance it sets the water
/// level, places the statues and boulders the flags have moved, opens or
/// closes the cells around them, and in the rooms whose water drains it shows
/// the drained state once its flag is set. Room 6 clears its actors' x and z
/// where the other rooms clear terrainHeight and y.

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
#include "event_inline.h"

struct MapBits {
  u8 unk00[23];
  u8 low : 2;
  u8 mode : 2;
};

// The original keeps the masked BG1CNT in a stack slot it never reads again:
// control is volatile, and the register is written in a once-only block.
#define BG1_SET_PRIORITY_1()                                                   \
  {                                                                            \
    u16 value;                                                                 \
                                                                               \
    value = (control = REG_BG1CNT & 0xfffc);                                   \
    do {                                                                       \
      REG_BG1CNT = value | 1;                                                  \
    } while (0);                                                               \
  }

void Func_02003278(void);
void Func_020032d4(u32 first, s32 level, s32 baseLevel);
void Func_02003cc4(struct Actor *actor, s32 mode);
void Func_0200349c(u32 index, s32 height);
void Func_02003ea4();
void Actor_SetSingleChildFlags();
void Func_02003628(void);
s32 Func_02003688(s32 x, s32 y, s32 z, s32 kind);
s32 Func_020036b8(s32 a, s32 b);
void Func_020007a8(void);
void Func_02003420(s32 id, s32 kind, s32 height);
void Func_02000b84(void);
s32 Func_02000b24(s32 id, s32 value);
void Func_02000ad8(void);
void Func_02003edc();
void Func_02003ee4();

s32 Func_02002340(void) {
  s32 scene = gMainRoutineWork.scene;
  s32 entrance = gMainRoutineWork.entrance;
  volatile u16 control;
  // i1, i3, i4, i6, i7, i12, i14, i17 and b each hold one constant for their
  // whole life, i2 holds a flag test and the others each count one loop.
  s32 i1, i2, i3, i4, i5, i6, i7, i8, i9, i10, i11, i12, i13, i14, i15, i16,
      i17;
  struct Actor *actor;
  s32 a, b;
  u8 *map;

  ((struct MapBits *)Ram_HeapSlots->mapWork)->mode = 1;
  Func_02003278();
  if (scene == (s32)&SceneId_Ov67cRoom1) {
    if ((u32)(entrance - 3) <= 1 || entrance == 6 || entrance == 8) {
      Func_020032d4(33, 0, -2);
    } else if (entrance == 5 || entrance == 7) {
      actor = Actor_GetById(12);
      i1 = 0;
      actor->motionFlags = i1;
      actor->terrainHeight = i1;
      actor->y = i1;
      Call2((void (*)())Func_02003cc4, (s32)actor, 0);
      actor = Actor_GetById(13);
      actor->motionFlags = i1;
      actor->terrainHeight = i1;
      actor->y = i1;
      i2 = GameFlag_Test(0x230);
      if (i2 == 0) {
        MapGrid_CopyCellsLayered(104, 80, 16, 10, 104, 40);
        MapGrid_CopyCellsLayered(110, 116, 18, 12, 39, 105);
        // i2 is the flag test's 0.
        MapGrid_CopyCellAttributes(64, 32, 64, 32, i2, 32);
        Func_020032d4(33, 0, 0);
      } else {
        Call3((void (*)())Func_020032d4, 33, 4, 0);
        actor = Actor_GetById(12);
        Call2((void (*)())Func_0200349c, (actor->x >> 20) - 42, 0);
      }
      if (GameFlag_Test(0x250)) {
        MapGrid_CopyCellAttributes(41, 42, 1, 1, 43, 44);
      }
    } else if ((u32)(entrance - 9) <= 1) {
      a = GameFlag_Test(0x231);
      if (a == 0) {
        i3 = 32;
        MapGrid_CopyCellsLayered(77, 80, 23, 20, 77, i3);
        MapGrid_CopyCellsLayered(112, 96, 16, 20, 12, 98);
        // a is the flag test's 0.
        MapGrid_CopyCellAttributes(64, 32, 64, 32, a, i3);
        Func_020032d4(33, 0, 0);
      } else {
        Func_020032d4(33, 4, 0);
      }
    }
    if (GameFlag_Test(0x95a)) {
      Actor_SetPosition(10, PIXELS(184), PIXELS(200));
      i4 = 12;
      MapGrid_CopyCellAttributes(0, 12, 1, 1, 9, i4);
      MapGrid_CopyCellAttributes(0, 12, 1, 1, 13, i4);
    }
    if (GameFlag_Test(0x95b)) {
      Actor_SetPosition(11, PIXELS(824), PIXELS(216));
    }
  } else if (scene == (s32)&SceneId_Ov67cRoom2) {
    if ((u32)(entrance - 1) <= 1) {
      Func_020032d4(33, 0, -2);
    }
    actor = Actor_GetById(8);
    actor->motionFlags = 0;
    actor->terrainHeight = 0;
    actor->y = 0;
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
      MapGrid_CopyCellsLayered(75, 96, 7, 13, 80, 27);
      i6 = 16;
      b = 31;
      MapGrid_CopyCellsLayered(91, 96, 7, 10, i6, b);
      MapGrid_CopyCellsLayered(99, 96, 7, 14, i6, 91);
      MapGrid_CopyCellAttributes(43, 0, 5, 9, 17, b);
      Func_020007a8();
    } else {
      MapGrid_CopyCellAttributes(17, 32, 5, 1, 17, 31);
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
      if (!GameFlag_Test(0x249) && !Event_GetFlag(0xa64)) {
        map = (u8 *)Ram_HeapSlots->mapWork;
        i7 = 39;
        MapGrid_CopyCellsLayered(64, 64, 16, 13, 71, i7);
        MapGrid_CopyCellAttributes(71, 39, 16, 15, 7, i7);
        Func_02000b84();
        map[262] = 1;
        BG1_SET_PRIORITY_1();
      } else if (Event_GetFlag(0xa64) ||
                 GameFlag_Test(0x248)) {
        if (Event_GetFlag(0xa64)) {
          Actor_SetPosition(12, PIXELS(280), PIXELS(712));
        }
        Func_02000b24(12, 38);
        Func_02003628();
        Value4((s32 (*)())Func_02003688, 12, 2, 45, 1);
        for (i8 = 0; i8 < 8; i8++)
          Func_020036b8(33, 55);
        Event_SetFlag(0x249);
      }
      i9 = 12;
      do {
        actor = Actor_GetById(i9);
        actor->motionFlags = 0;
        actor->terrainHeight = 0;
        actor->y = 0;
        i9++;
      } while (i9 <= 14);
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
      do {
        actor = Actor_GetById(i11);
        actor->motionFlags = 0;
        actor->x = 0;
        actor->z = 0;
        i11++;
      } while (i11 <= 15);
      goto done;
    } else {
      MapGrid_CopyCellsLayered(114, 39, 1, 2, 113, 39);
      i12 = 41;
      MapGrid_CopyCellAttributes(105, 41, 12, 13, i12, i12);
      MapGrid_CopyCellsLayered(105, 105, 12, 13, 105, i12);
      for (i13 = 13; i13 <= 15; i13++) {
        actor = Actor_GetById(i13);
        if (GameFlag_Test(i13 + 580)) {
          MapGrid_CopyCellAttributes(47, 45, 1, 1, actor->x >> 20, actor->z >> 20);
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
      if (!GameFlag_Test(0x24b) && !Event_GetFlag(0xa65)) {
        map = (u8 *)Ram_HeapSlots->mapWork;
        i14 = 13;
        MapGrid_CopyCellsLayered(64, 64, 19, 17, 100, i14);
        MapGrid_CopyCellsLayered(35, 105, 23, 24, 35, 73);
        MapGrid_CopyCellAttributes(100, 13, 19, 19, 36, i14);
        map[262] = 1;
        BG1_SET_PRIORITY_1();
      } else if (Event_GetFlag(0xa65) ||
                 GameFlag_Test(0x24a)) {
        if (Event_GetFlag(0xa65)) {
          Actor_SetPosition(12, PIXELS(808), PIXELS(296));
          Actor_SetPosition(13, PIXELS(680), PIXELS(360));
          Actor_SetPosition(15, PIXELS(744), PIXELS(424));
        }
        Call2((void (*)())Func_02000b24, 12, 38);
        Call2((void (*)())Func_02000b24, 13, 39);
        Func_02000b24(15, 37);
        Func_02003628();
        Value4((s32 (*)())Func_02003688, 38, 2, 19, 1);
        for (i15 = 0; i15 < 8; i15++)
          Func_020036b8(33, 55);
        Event_SetFlag(0x24b);
      }
    }
    i16 = 10;
  clear_pillars:
    actor = Actor_GetById(i16);
    actor->motionFlags = 0;
    actor->terrainHeight = 0;
    actor->y = 0;
    i16++;
    if (i16 <= 20)
      goto clear_pillars;
    actor = Actor_GetById(19);
    MapGrid_CopyCellAttributes(23, 39, 1, 1, actor->x >> 20, 39);
    if ((u32)(entrance - 1) <= 1) {
      Call1((void (*)())Func_02003ea4, 0);
    }
  } else if (scene == (s32)&SceneId_Ov67cRoom10) {
    Call1((void (*)())Func_02003ea4, 0);
    Func_020032d4(33, 0, -2);
    Event_SetFlag(0x144);
    if (GameFlag_Test(0x349)) {
      MapGrid_CopyCellAttributes(38, 9, 7, 6, 13, 9);
    } else {
      i17 = 8;
      MapGrid_CopyCellsLayered(77, 8, 7, 6, 50, i17);
      MapGrid_CopyCellsLayered(70, 8, 7, 6, 77, i17);
      MapGrid_CopyCellAttributes(38, 17, 7, 6, 13, 9);
    }
  }
done:
  return 0;
}
