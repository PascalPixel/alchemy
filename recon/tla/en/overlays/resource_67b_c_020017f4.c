// Draft, differ 4 of 534: Aqua Rock's map setup: sets Retreat to
// Ov67bRoom1's entrance 1 and the screen effect, then in Ov67bRoom1 sets
// up the gates' objects (8 to 23), objects 25 and 24, placed by flag 0x950 and
// the bytes 0x310 and 0x318, the water and its objects (Func_02002550,
// Func_0200276c, flag 0xa68), the steps flag 0x955 raises and the gate
// flag 0xa6b opens, then the gates (Func_02000844); in Ov67bRoom2 it opens
// the cells flags 545 to 547 and 0x951 and 0x952 mark, and sets up the
// water by the entrance; entrances 1 and 6 there (before flag 265) and 5
// and 4 in Ov67bRoom1 start their scenes.
// Left: scheduling only: the native loads the Retreat scene's id (kept in r5
// for the room test) before 516 is formed from the screen effect's offset;
// a struct field for the screen effect does not move it.
#include "../../../../games/tla/src/scene/aqua_rock/local.h"
#include "call.h"

void Func_02002a58(s32 a);
void Func_02001f60(s32 id);
s32 GameFlag_GetByte(s32 flag);
void Func_020013f8(void);
void Func_02002550(void);
void Func_0200276c(void);
void Func_020025a8(s32 id);
void Func_02002834(s32 id);
void Func_02002948(s32 a);
void Func_02002940(s32 a);
void Func_02000b58(void);
void Func_0200169c(void);
void Func_02001670(void);
void Func_02002968(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_02000844(void);
void Func_02002158(void);
void Func_02001d98(void);
void Func_02001460(void);
void Func_0200153c(void);

s32 Func_020017f4(void) {
  s32 scene = gPartyState.scene;
  u8 *map = (u8 *)Ram_HeapSlots->mapWork;
  s32 id, x, z;
  s16 *entrance;

  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 516;
  gPartyState.retreatScene = (s32)&SceneId_Ov67bRoom1;
  gPartyState.retreatEntrance = 1;
  ((s8 *)map)[23] = (((s8 *)map)[23] & ~12) | 4;
  Func_02002a58(170);
  if (scene == (s32)&SceneId_Ov67bRoom1) {
    Object_SetPartAttribute(Object_GetById(8), 4);
    Object_SetPartAttribute(Object_GetById(9), 4);
    Object_SetPartAttribute(Object_GetById(10), 4);
    Object_SetPartAttribute(Object_GetById(11), 4);
    if (GameFlag_Test(0xa6b))
      Object_SetPartAttribute(Object_GetById(12), 5);
    else
      Object_SetPartAttribute(Object_GetById(12), 6);
    Func_02001f60(8);
    Func_02001f60(9);
    Func_02001f60(10);
    Func_02001f60(11);
    Func_02001f60(13);
    Func_02001f60(14);
    for (id = 15; id <= 23; id++)
      Func_02001f60(id);
    for (id = 8; id <= 14; id++)
      ObjectMotion_SetActionVariant(id, 3);
    for (id = 15; id <= 23; id++)
      ObjectMotion_SetActionVariant(id, 1);
    Object_GetById(15)->unk5c[0] = 2;
    Object_SetModeById(15, 3);
    for (id = 16; id <= 23; id++) {
      Object_SetModeById(id, 1);
      Object_SetModeById(id, 0);
    }
    if (GameFlag_Test(0x950))
      Call3(Func_020029d8, 25, 0x2080000, 0x2480000);
    x = GameFlag_GetByte(0x310);
    z = GameFlag_GetByte(0x318);
    if (z != 0)
      Func_020029d8(24, (x << 20) + 0x80000, (z << 20) + 0x80000);
    Object_GetById(Func_02002a68())->priorityFlags |= 32;
    Scheduler_AddOrUpdateCallback(Func_020013f8, 0x480);
    ObjectMotion_SetActionVariant(28, 1);
    Value2((s32 (*)())ObjectMotion_SetActionVariant, 24, 1);
    Func_02002550();
    Func_0200276c();
    if (GameFlag_Test(0xa68)) {
      Func_020025a8(26);
      Func_02002834(26);
      Func_02002948(5);
      Func_02002948(6);
      Func_02002940(7);
      Func_02002940(8);
      Object_GetById(26)->update = Func_02000b58;
    }
    ObjectMotion_SetActionVariant(26, 1);
    if (gPartyState.entrance == 5)
      Func_0200169c();
    if (GameFlag_Test(0x955)) {
      Call3(Func_020029d8, 17, 0x1880000, 0x3380000);
      Call3(Func_020029d8, 18, 0x1a80000, 0x3380000);
      Call3(Func_020029d8, 19, 0x1c80000, 0x3380000);
      Call3(Func_020029d8, 20, 0x1e80000, 0x3380000);
      Call3(Func_020029d8, 21, 0x2080000, 0x3380000);
      Call6(Func_02002960, 0x40, 66, 9, 3, 24, 53);
      Call6(Func_02002958, 0x40, 66, 11, 7, 87, 50);
      Object_GetById(21)->update = Func_02001670;
    }
    if (GameFlag_Test(0xa6b)) {
      Call6(Func_02002960, 62, 41, 1, 2, 33, 41);
      Call6(Func_02002968, 116, 95, 3, 13, 0x60, 0x20);
      Call6(Func_02002968, 122, 95, 3, 13, 0x20, 0x60);
      Func_02001f60(12);
    }
    Func_02000844();
    if (gPartyState.entrance == 4)
      Func_02002158();
  } else if (scene == (s32)&SceneId_Ov67bRoom2) {
    ObjectDispatch_SetSingleChildFlags(Object_GetById(14), 0);
    if (GameFlag_Test(545))
      Call6(Func_02002960, 42, 0x20, 1, 1, 44, 0x20);
    if (GameFlag_Test(546))
      Call6(Func_02002960, 0x30, 0x20, 1, 1, 46, 0x20);
    if (GameFlag_Test(547))
      Call6(Func_02002960, 44, 44, 1, 1, 46, 44);
    Object_SetModeById(12, 2);
    if (GameFlag_Test(0x951)) {
      Call3(Func_020029d8, 12, 0x3580000, 0x3480000);
      Call6(Func_02002960, 54, 58, 1, 1, 53, 52);
    }
    Object_SetModeById(13, 2);
    if (GameFlag_Test(0x952)) {
      Call3(Func_020029d8, 13, 0x3580000, 0x3a80000);
      Call6(Func_02002960, 54, 58, 1, 1, 53, 58);
    }
    if (gPartyState.entrance == 5 || gPartyState.entrance == 7) {
      Func_02002550();
      Func_0200276c();
      if (GameFlag_Test(0xa68)) {
        Func_020025a8(14);
        Func_02002834(14);
      } else {
        Func_02002948(7);
        Func_02002948(8);
        Func_02002940(5);
        Func_02002940(6);
      }
      ObjectDispatch_SetSingleChildFlags(Object_GetById(14), 0);
    } else {
      Func_02002550();
      Func_0200276c();
      Object_GetById(8)->update = Func_02000b58;
      if (GameFlag_Test(0xa69)) {
        Func_020025a8(8);
        Func_02002834(8);
        Func_020029d8(15, 0x1080000, 0x3180000);
        ObjectDispatch_SetSingleChildFlags(Object_GetById(15), 0);
        Object_GetById(15)->collisionFlags = 0;
        Object_GetById(15)->priorityFlags |= 2;
        Object_GetById(15)->update = Func_02001d98;
        Call6(Func_02002960, 15, 0x30, 1, 1, 0x10, 0x30);
        Call6(Func_02002960, 17, 50, 1, 1, 0x10, 50);
        Call6(Func_02002968, 0x40, 68, 5, 13, 14, 94);
        Call6(Func_02002960, 0x40, 68, 5, 8, 14, 34);
      }
    }
    if (!GameFlag_Test(265)) {
      if (gPartyState.entrance == 1)
        Func_02001460();
      if (*(entrance = &gPartyState.entrance) == 6)
        Func_0200153c();
    }
  }
  return 0;
}
