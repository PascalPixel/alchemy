#include "version.h"
#include "../../../../games/tla/src/scene/mars_lighthouse_room_f/room.h"

extern char SceneId_Ov6adRoom1;
extern char SceneId_Ov6adRoom2;
extern char SceneId_Ov6adRoom3;
extern char SceneId_Ov6adRoom4;
void Func_02000444(s32 state, s32 a1);
void Func_02000480(s32 state, s32 a1);
void Func_020004bc(s32 state, s32 a1);
void Func_020006f4(s32 burst);
void Func_020009ec(void);
void Func_02000c80(s32 id);
void Func_02000d80(void);
void Func_020015a4(s32 mode);
void Func_020025b0(void);
void Func_02002604(void);
s32 Func_02002624(const void *targets, const void *stops, void *a2, s32 a3);
void Func_020027cc(s32 index, s16 value);
void Func_02003d80(void);
s32 Func_02004938(s32 id);
void Func_02005284(void);
void Func_0200538c(s32 id, s32 value);
void Func_020053bc(s32 value);
void Func_020055b8(s32 id);
void Func_02005638(s32 a0);
void Func_02005670(void);
void Func_02005678(s32 a0, s32 a1, s32 a2);
void Func_020056b0(s32 id);
void Func_020056b8(s32 id);
void Event_StartScreenTransition(void);
extern u8 Func_02000514[];
extern u8 Func_02000e68[];
extern u8 Func_02001468[];
extern u8 Func_02001490[];
extern const u16 gData_02005bf0[];
extern const u16 gData_02005bf6[];
extern const u16 gData_02005bfc[];
extern const u16 gData_02005c0e[];
extern const u16 gData_02005c14[];
extern const u16 gData_02005c26[];
extern const u16 gData_02005c2c[];
extern const u16 gData_02005c32[];
extern const u16 gData_02005c38[];
extern const u16 gData_02005c46[];
extern const u16 gData_02005c4c[];
extern const u16 gData_02005c5a[];
extern const u16 gData_02005c60[];
extern const u16 gData_02005c6e[];

s32 Func_02000e7c(void) {
  struct MainRoutineWork *work = &gMainRoutineWork;
  s32 scene = work->scene;
  s32 entrance = work->entrance;
  struct HeapSlots *heap = Ram_HeapSlots;
  s32 value;
  u8 *flags;
  s32 bits;
  s32 other;

  /* FAKEMATCH(temporary): the first flag update through other; as *flags |=
   * bits, 49 of 559 instructions differ in tla-en. */

  ((struct SceneWork *)heap->eventWork)->unk1ac = 516;
  value = 516;
  Flag_Set(0x112);
  Func_020025b0();
  if (scene == (s32)&SceneId_Ov6adRoom1) {
    Func_02005638(0);
    ((struct SceneWork *)heap->eventWork)->unk1ac = value;
    if (entrance - 1 <= 1U) {
      Func_02002624(gData_02005bf0, gData_02005bf6, Func_02000514, 88);
      if (Flag_IsSet(0xa42))
        Func_02005598_(10, 0, 0);
      else
        Func_02005598_(9, 0, 0);
      if (Flag_IsSet(0xa43))
        Func_020006f4(0);
      if (Flag_IsSet(0xa44))
        Func_02000444(1, 0);
      if (Flag_IsSet(0xa45))
        Func_02000480(1, 0);
      if (Flag_IsSet(0xa46))
        Func_020004bc(1, 0);
    } else {
      Func_02002624(gData_02005bfc, gData_02005c0e, 0, 88);
      Func_020055b8(15);
      Func_020055b8(16);
      Func_02004938(15);
      Func_02004938(16);
      Func_020003b4();
    }
  } else if (scene == (s32)&SceneId_Ov6adRoom2) {
    Func_02005638(0);
    ((struct SceneWork *)heap->eventWork)->unk1ac = value;
    Func_02002624(gData_02005c14, gData_02005c26, Func_02001468, 88);
    Func_020027cc(3, 1);
    if (entrance == 2)
      Func_02002604();
    if (entrance == 9) {
      Flag_Set(0x351);
      Func_02005560();
      Event_ResolvePendingActions(0);
      Camera_SetBounds(-1, -1, -1, 0);
      Func_02005598_(work->currentOwner, 0, 0);
      Func_02005598_(17, 0, 0);
      Event_StartScreenTransition();
      Event_WaitTransitionFrames();
      Event_Wait(30);
      Func_02005598_(18, 0, 0);
      Func_02005598_(17, 0x1080000, 0x2080000);
      Sound_Play(136);
      Event_Wait(60);
      Flag_Set(0x109);
      return ((s32 (*)())Import_EventSetExitRequest)(3);
    }
    if (Flag_IsSet(0x350)) {
      Func_02005598_(8, 0x2580000, 0x900000);
      Func_02005598_(9, 0x2200000, 0xd00000);
      Func_02005598_(10, 0x2200000, 0xf00000);
      Flag_Set(0x220);
      Flag_Set(0x221);
    }
    Func_020055b8(8);
    Func_020055b8(9);
    Func_020055b8(10);
    Func_02004938(8);
    Func_02004938(9);
    Func_02004938(10);
    flags = &Actor_GetById(11)->priorityFlags;
    bits = 2;
    other = *flags;
    other |= bits;
    *flags = other;
    flags = &Actor_GetById(12)->priorityFlags;
    bits |= *flags;
    *flags = bits;
    if (Flag_IsSet(0x220))
      Func_02000c80(11);
    if (Flag_IsSet(0x221))
      Func_02000c80(12);
    Func_020009ec();
    if (Flag_IsSet(0x350))
      Func_02005598_(18, 0, 0);
    else
      Func_02005598_(17, 0, 0);
    if (Flag_IsSet(0xa40))
      Func_02000d80();
    Task_Add(Func_02000e68, 0x480);
  } else if (scene == (s32)&SceneId_Ov6adRoom3) {
    Func_02005638(0);
    ((struct SceneWork *)heap->eventWork)->unk1ac = value;
    Func_020015a4(1);
    Func_02005284();
    Func_020053bc(2);
    Func_0200538c(11, 6);
    if (Flag_IsSet(0x358)) {
      Func_020054f0_(71, 24, 7, 1, 71, 5);
      Func_020054e8_(7, 24, 7, 1, 7, 7);
      Func_020054e8_(7, 88, 7, 1, 7, 71);
      Func_02005598_(11, 0, 0);
    }
    if (Flag_IsSet(0xa37))
      Func_02005598_(12, 0x780000, 0x1280000);
    if (entrance - 14 <= 1U) {
      Func_02002624(gData_02005c2c, gData_02005c32, Func_02001490, 88);
      Func_020027cc(0, 1);
    } else {
      Func_02002624(gData_02005c38, gData_02005c46, 0, 88);
    }
    if (!Flag_IsSet(0x109)) {
      if (entrance == 2 || entrance == 4 || entrance == 6 || entrance == 7 ||
          entrance == 8 || entrance == 10 || entrance == 11 || entrance == 13)
        Func_020056b0(gMainRoutineWork.currentOwner);
      if (entrance == 3 || entrance == 5 || entrance == 9 || entrance == 12 ||
          entrance == 14)
        Func_020056b8(gMainRoutineWork.currentOwner);
      if (entrance == 16 && !Flag_IsSet(0x109))
        Func_02003d80();
    }
  } else if (scene == (s32)&SceneId_Ov6adRoom4) {
    Func_02005638(0);
    ((struct SceneWork *)heap->eventWork)->unk1ac = value;
    Func_02005670();
    Func_02005678(0, 8, 9);
    BYTE_AT(Actor_GetById(8), 98) = 1;
    BYTE_AT(Actor_GetById(9), 98) = 1;
    if (entrance - 5 <= 1U || entrance == 8) {
      Func_02002624(gData_02005c4c, gData_02005c5a, 0, 88);
#if TLA_PATCH_1
      Func_020002ec();
#endif
    } else if (entrance == 11 || entrance == 13 || entrance == 14 ||
               entrance == 1 || entrance == 15) {
      Func_02002624(gData_02005c60, gData_02005c6e, 0, 88);
    }
    if (!Flag_IsSet(0x109)) {
      if (entrance == 15)
        Func_02005598_(23, 0x2980000, 0x2c80000);
      if (entrance == 11)
        Func_02005598_(17, 0x2180000, 0x2280000);
    }
    Actor_GetById(16)->motionFlags = 0;
    if (Flag_IsSet(0xa38)) {
      Func_02005598_(16, 0x3280000, 0x1380000);
      Actor_GetById(16)->y = 0;
      Actor_GetById(16)->terrainHeight = 0;
    }
    Actor_GetById(gMainRoutineWork.currentOwner)->priorityFlags |= 64;
    Actor_GetById(10)->priorityFlags |= 192;
    Actor_GetById(11)->priorityFlags |= 128;
    Actor_GetById(12)->priorityFlags |= 128;
  }
  return 0;
}
