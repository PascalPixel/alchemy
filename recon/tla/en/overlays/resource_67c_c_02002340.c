// Draft, differ 483 of 931: Aqua Rock's map preparation: for each room and
// entrance it sets the water level, places the statues and boulders the
// flags have moved, opens or closes the cells around them, and in the rooms
// whose water drains it shows the drained state once its flag is set.
#include "types.h"
#include "object_runtime.h"
#include "party_state.h"
#include "ram_buffer.h"

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

extern char SceneId_Ov67cRoom1;
extern char SceneId_Ov67cRoom2;
extern char SceneId_Ov67cRoom3;
extern char SceneId_Ov67cRoom4;
extern char SceneId_Ov67cRoom5;
extern char SceneId_Ov67cRoom6;
extern char SceneId_Ov67cRoom7;
extern char SceneId_Ov67cRoom8;
extern char SceneId_Ov67cRoom9;
extern char SceneId_Ov67cRoom10;

void Func_02003278(s32 mode);
void Func_020032d4(s32 layer, s32 level, s32 offset);
struct ObjectRuntime *Object_GetById(s32 id);
void Func_02003cc4(struct ObjectRuntime *object, s32 mode);
s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void Func_02003d24(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_02003d1c(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_0200349c(s32 column, s32 value);
void Func_02003ddc(s32 id, s32 x, s32 z);
void Func_02003ea4(s32 mode);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object,
                                        s32 flags);
void Func_02003628(void);
void Func_02003688(s32 a, s32 b, s32 c, s32 d);
void Func_020036b8(s32 a, s32 b);
void Func_020007a8(void);
void Func_02003420(s32 id, s32 a, s32 b);
void Func_02000b84(void);
s32 Func_02000b24(s32 id, s32 value);
void Func_02000ad8(void);
void Func_02003edc(void);
void Func_02003ee4(s32 a, s32 b, s32 c);

s32 Func_02002340(s32 mode) {
  s32 scene = gPartyState.scene;
  s32 entrance = gPartyState.entrance;
  struct ObjectRuntime *object;
  s32 i;

  FIELD_AT(Ram_HeapSlots->mapWork, u8, 23) =
      (FIELD_AT(Ram_HeapSlots->mapWork, u8, 23) & ~12) | 4;
  Func_02003278(mode);
  if (scene == (s32)&SceneId_Ov67cRoom1) {
    if ((u32)(entrance - 3) <= 1 || entrance == 6 || entrance == 8) {
      Func_020032d4(33, 0, -2);
    } else if (entrance == 5 || entrance == 7) {
      object = Object_GetById(12);
      object->motionFlags = 0;
      object->terrainHeight = 0;
      object->y = 0;
      Func_02003cc4(object, 0);
      object = Object_GetById(13);
      object->motionFlags = 0;
      object->terrainHeight = 0;
      object->y = 0;
      if (!GameFlag_Test(0x230)) {
        Func_02003d24(104, 80, 16, 10, 104, 40);
        Func_02003d24(110, 116, 18, 12, 39, 105);
        Func_02003d1c(64, 32, 64, 32, 0, 32);
        Func_020032d4(33, 0, 0);
      } else {
        Func_020032d4(33, 4, 0);
        Func_0200349c((Object_GetById(12)->x >> 20) - 42, 0);
      }
      if (GameFlag_Test(0x250))
        Func_02003d1c(41, 42, 1, 1, 43, 44);
    } else if ((u32)(entrance - 9) <= 1) {
      if (!GameFlag_Test(0x231)) {
        Func_02003d24(77, 80, 23, 20, 77, 32);
        Func_02003d24(112, 96, 16, 20, 12, 98);
        Func_02003d1c(64, 32, 64, 32, 0, 32);
        Func_020032d4(33, 0, 0);
      } else {
        Func_020032d4(33, 4, 0);
      }
    }
    if (GameFlag_Test(0x95a)) {
      Func_02003ddc(10, 0xb80000, 0xc80000);
      Func_02003d1c(0, 12, 1, 1, 9, 12);
      Func_02003d1c(0, 12, 1, 1, 13, 12);
    }
    if (GameFlag_Test(0x95b))
      Func_02003ddc(11, 0x3380000, 0xd80000);
  } else if (scene == (s32)&SceneId_Ov67cRoom2) {
    if ((u32)(entrance - 1) <= 1)
      Func_020032d4(33, 0, -2);
    object = Object_GetById(8);
    object->motionFlags = 0;
    object->terrainHeight = 0;
    object->y = 0;
  } else if (scene == (s32)&SceneId_Ov67cRoom3) {
    Func_02003ea4(0);
    ObjectDispatch_SetSingleChildFlags(Object_GetById(9), 0);
    ObjectDispatch_SetSingleChildFlags(Object_GetById(10), 0);
    Func_020032d4(33, 8, -2);
    Func_020032d4(40, 8, 8);
    if (GameFlag_Test(0xa66)) {
      Func_02003628();
      Func_02003688(16, 4, 16, 2);
      Func_02003688(22, 4, 16, 0);
      for (i = 5; i >= 0; i--)
        Func_020036b8(33, 55);
    }
    if (GameFlag_Test(0x95d)) {
      Func_02003d24(75, 96, 7, 13, 80, 27);
      Func_02003d24(91, 96, 7, 10, 16, 31);
      Func_02003d24(99, 96, 7, 14, 16, 91);
      Func_02003d1c(43, 0, 5, 9, 17, 31);
      Func_020007a8();
    } else {
      Func_02003d1c(17, 32, 5, 1, 17, 31);
    }
  } else if (scene == (s32)&SceneId_Ov67cRoom4) {
    Func_02003ea4(0);
    if ((u32)(entrance - 6) <= 1) {
      Object_GetById(gPartyState.currentOwner)->priorityFlags |= 32;
      Func_02003420(12, 5, 4);
      Func_02003420(13, 3, 4);
      Func_02003420(14, 4, 4);
      Func_020032d4(40, 4, 4);
      if (!GameFlag_Test(0x249) && !GameFlag_Test(0xa64)) {
        u8 *map = (u8 *)Ram_HeapSlots->mapWork;
        volatile u16 control;

        Func_02003d24(64, 64, 16, 13, 71, 39);
        Func_02003d1c(71, 39, 16, 15, 7, 39);
        Func_02000b84();
        map[262] = 1;
        control = *(volatile u16 *)0x0400000a & ~3;
        *(volatile u16 *)0x0400000a = control | 1;
      } else if (GameFlag_Test(0xa64) || GameFlag_Test(0x248)) {
        if (GameFlag_Test(0xa64))
          Func_02003ddc(12, 0x1180000, 0x2c80000);
        Func_02000b24(12, 38);
        Func_02003628();
        Func_02003688(12, 2, 45, 1);
        for (i = 7; i >= 0; i--)
          Func_020036b8(33, 55);
        GameFlag_SetBit(0x249);
      }
      for (i = 12; i <= 14; i++) {
        object = Object_GetById(i);
        object->motionFlags = 0;
        object->terrainHeight = 0;
        object->y = 0;
      }
    } else {
      Func_020032d4(33, 0, -4);
    }
    if (GameFlag_Test(0xa6a))
      Func_02000ad8();
  } else if (scene == (s32)&SceneId_Ov67cRoom5) {
    Func_020032d4(33, 0, -2);
    Func_020032d4(40, 0, 0);
    Func_02003ea4(0);
    if (((u32)(entrance - 1) <= 3 || entrance == 7) && GameFlag_Test(0xa67)) {
      Func_02003628();
      Func_02003688(44, 0, 34, 2);
      for (i = 0; i <= 4; i++)
        Func_020036b8(33, 55);
    }
  } else if (scene == (s32)&SceneId_Ov67cRoom6) {
    Func_02003ea4(0);
    if (entrance == 1 || entrance == 13)
      Func_020032d4(33, 0, -2);
    else if (entrance == 9 || entrance == 11 || entrance == 12)
      Func_020032d4(33, 0, -2);
    if (!GameFlag_Test(0x238)) {
      for (i = 13; i <= 15; i++) {
        object = Object_GetById(i);
        object->motionFlags = 0;
        object->x = 0;
        object->z = 0;
      }
    } else {
      Func_02003d24(114, 39, 1, 2, 113, 39);
      Func_02003d1c(105, 41, 12, 13, 41, 41);
      Func_02003d24(105, 105, 12, 13, 105, 41);
      for (i = 13; i <= 15; i++) {
        object = Object_GetById(i);
        if (GameFlag_Test(i + 580))
          Func_02003d1c(47, 45, 1, 1, object->x >> 20, object->z >> 20);
      }
    }
  } else if (scene == (s32)&SceneId_Ov67cRoom7) {
    Func_020032d4(33, 0, -2);
  } else if (scene == (s32)&SceneId_Ov67cRoom8) {
    Func_02003ea4(0);
    Func_020032d4(33, 0, -2);
    Func_02003edc();
    Func_02003ee4(0, 8, 9);
  } else if (scene == (s32)&SceneId_Ov67cRoom9) {
    if ((u32)(entrance - 3) <= 1) {
      Object_GetById(gPartyState.currentOwner)->priorityFlags |= 32;
      Func_02003420(10, 6, 4);
      Func_02003420(11, 3, 4);
      Func_02003420(12, 5, 4);
      Func_02003420(13, 6, 4);
      Func_02003420(14, 5, 4);
      Func_02003420(15, 4, 4);
      Func_02003420(16, 4, 4);
      Func_02003420(17, 3, 4);
      Func_02003420(18, 6, 4);
      Func_020032d4(40, 4, 4);
      if (!GameFlag_Test(0x24b) && !GameFlag_Test(0xa65)) {
        u8 *map = (u8 *)Ram_HeapSlots->mapWork;
        volatile u16 control;

        Func_02003d24(64, 64, 19, 17, 100, 13);
        Func_02003d24(35, 105, 23, 24, 35, 73);
        Func_02003d1c(100, 13, 19, 19, 36, 13);
        map[262] = 1;
        control = *(volatile u16 *)0x0400000a & ~3;
        *(volatile u16 *)0x0400000a = control | 1;
      } else if (GameFlag_Test(0xa65) || GameFlag_Test(0x24a)) {
        if (GameFlag_Test(0xa65)) {
          Func_02003ddc(12, 0x3280000, 0x1280000);
          Func_02003ddc(13, 0x2a80000, 0x1680000);
          Func_02003ddc(15, 0x2e80000, 0x1a80000);
        }
        Func_02000b24(12, 38);
        Func_02000b24(13, 39);
        Func_02000b24(15, 37);
        Func_02003628();
        Func_02003688(38, 2, 19, 1);
        for (i = 7; i >= 0; i--)
          Func_020036b8(33, 55);
        GameFlag_SetBit(0x24b);
      }
    }
    for (i = 10; i <= 20; i++) {
      object = Object_GetById(i);
      object->motionFlags = 0;
      object->terrainHeight = 0;
      object->y = 0;
    }
    Func_02003d1c(23, 39, 1, 1, Object_GetById(19)->x >> 20, 39);
    if ((u32)(entrance - 1) <= 1)
      Func_02003ea4(0);
  } else if (scene == (s32)&SceneId_Ov67cRoom10) {
    Func_02003ea4(0);
    Func_020032d4(33, 0, -2);
    GameFlag_SetBit(0x144);
    if (GameFlag_Test(0x349)) {
      Func_02003d1c(38, 9, 7, 6, 13, 9);
    } else {
      Func_02003d24(77, 8, 7, 6, 50, 8);
      Func_02003d24(70, 8, 7, 6, 77, 8);
      Func_02003d1c(38, 17, 7, 6, 13, 9);
    }
  }
  return 0;
}
