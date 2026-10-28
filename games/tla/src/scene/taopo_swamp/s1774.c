#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "adventure.h"
#include "call.h"
#include "edition.h"

extern char SceneId_Ov678Room1, SceneId_Ov678Room2, SceneId_Ov678Room3,
    SceneId_Ov678Room4, SceneId_Ov678Room5, SceneId_Ov678Room6,
    SceneId_Ov678Room7, SceneId_Ov678Room8, SceneId_Ov678Room9,
    SceneId_Ov678Room10, SceneId_Ov678Room11;
extern u8 gDecodeFillByte;
extern const u8 gData_02002a78[], gData_02002ab6[], gData_02002abc[],
    gData_02002aca[], gData_02002ad8[];
s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
struct Actor *Actor_GetById(u32 id);
void Func_02002120(void);
void Func_020028d0(s32 id, s32 x, s32 z);
void Func_02002840(s32 srcX, s32 srcZ, s32 width, s32 height, s32 destX,
                   s32 destZ);
void Func_02002838(s32 srcX, s32 srcZ, s32 width, s32 height, s32 destX,
                   s32 destZ);
void Task_Add(void (*callback)(void), s32 priority);
void Func_02001288(void);
void Func_02001304(void);
void Func_02001424(void);
void Func_020014a0(void);
void Func_0200151c(void);
void Func_02001684(void);
void Func_020016fc(void);
void Func_02001618(void);
void Actor_SetWideSprite(s32 id, s32 wide);
void Func_02002960(s32 a);
void Func_02002614(const u8 *holes, const u8 *more, void (*update)(void));
void Actor_SetSingleChildFlags(struct Actor *object,
                                        u32 value);
void Func_020027f8(struct Actor *object, s32 mode);
void Func_02002980(void);
void Func_02002988(s32 a, s32 flag, s32 b, s32 c);
void Func_02002084(s32 a);
void Func_02002114(s32 a);
void Func_02002828(void);

// The JA edition has its own form of this function; it stays on its listing in
// recon/tla/raw/overlays (S4).
#if (LANGUAGE != LANGUAGE_JAPANESE)
s32 Func_02001774(void) {
  s32 scene = gAdventure.scene;
  u8 *map = (u8 *)Ram_HeapSlots->mapWork;
  s32 *effect = (s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac);
  struct Actor *object, *dummy, *piece;
  s32 id, entrance;

  *effect = 0;
  if (scene == (s32)&SceneId_Ov678Room2) {
    gAdventure.retreatScene = (s32)&SceneId_Ov678Room1;
    gAdventure.retreatEntrance = 1;
    Func_02002120();
    for (id = 8; id <= 12; id++) {
      if (GameFlag_Test(id + 0x8d8))
        Func_020028d0(id, 0, 0);
      else
        Actor_GetById(id)->scaleY = 0x14000;
    }
    Actor_SetSpritePriority(13, 3);
    Actor_GetById(13)->collisionFlags = 0;
    if (GameFlag_Test(0x220))
      Call6(Func_02002840, 50, 16, 1, 2, 47, 16);
    dummy = Actor_GetById(14);
    if (dummy != 0) {
      dummy->scaleX = 0xc000;
      dummy->scaleY = 0xc000;
      dummy->motionFlags = 0;
    }
    Task_Add(Func_02001288, 0x480);
    ((s8 *)map)[23] = (((s8 *)map)[23] & ~12) | 8;
    gDecodeFillByte = 93;
  } else if (scene == (s32)&SceneId_Ov678Room3) {
    *effect = 516;
    if (!GameFlag_Test(0x300))
      Actor_SetWideSprite(0x40, 1);
    if (!GameFlag_Test(769))
      Actor_SetWideSprite(65, 1);
  } else if (scene == (s32)&SceneId_Ov678Room4) {
    *effect = 516;
    Func_02002960(0);
    if (!GameFlag_Test(0x310))
      Func_020028d0(8, 0, 0);
  } else if (scene == (s32)&SceneId_Ov678Room5) {
    *effect = 516;
    Func_02002960(0);
    if (GameFlag_Test(0x310))
      Func_020028d0(8, 0, 0);
    if (GameFlag_Test(0xa6d))
      Call3(Func_020028d0, 9, 0x1180000, 0xe80000);
    if (GameFlag_Test(0xa6e))
      Call3(Func_020028d0, 10, 0x1d80000, 0xf80000);
    Func_02002614(gData_02002a78, gData_02002ab6, Func_02001304);
  } else if (scene == (s32)&SceneId_Ov678Room6) {
    *effect = 516;
  } else if (scene == (s32)&SceneId_Ov678Room7) {
    *effect = 516;
    Func_02002960(0);
    Task_Add(Func_02001684, 0x480);
    if (gAdventure.entrance == 3 || gAdventure.entrance == 5)
      Func_02002614(gData_02002abc, 0, Func_02001424);
    piece = Actor_GetById(8);
    piece->motionFlags = 0;
    Actor_SetSingleChildFlags(piece, 0);
    if (GameFlag_Test(545)) {
      Call6(Func_02002840, 28, 8, 1, 1, 26, 8);
      Func_020027f8(piece, 0);
    } else {
      Call6(Func_02002840, 22, 8, 1, 1, 26, 8);
      Actor_SetSpritePriority(8, 3);
      piece->priorityFlags |= 2;
      if (!GameFlag_Test(0x220))
        Func_020027f8(piece, 7);
    }
    piece = Actor_GetById(9);
    piece->motionFlags = 0;
    if (GameFlag_Test(0xa6f)) {
      Call3(Func_020028d0, 9, 0x1080000, 0x1a80000);
      piece->terrainHeight = 0;
      piece->y = 0;
      Call6(Func_02002840, 15, 26, 1, 1, 16, 26);
    }
  } else if (scene == (s32)&SceneId_Ov678Room8) {
    *effect = 516;
    if (gAdventure.entrance != 2 && gAdventure.entrance != 4 &&
        gAdventure.entrance != 9) {
      Func_02002980();
      Call4(Func_02002988, 0, 776, 8, 9);
    } else {
      Value3((s32 (*)())Func_02002614, (s32)gData_02002aca, 0, (s32)Func_020014a0);
      Actor_GetById(gAdventure.currentOwner)->priorityFlags |= 32;
      Call2(Task_Add, Func_020016fc, 0x480);
      Call2(Task_Add, Func_02001618, 0x480);
      object = Actor_GetById(10);
      object->motionFlags = 0;
      Actor_SetSingleChildFlags(object, 0);
      if (GameFlag_Test(545)) {
        Func_020027f8(object, 0);
      } else {
        Actor_SetSpritePriority(10, 3);
        object->priorityFlags |= 2;
        if (!GameFlag_Test(0x220))
          Func_020027f8(object, 7);
      }
      Call6(Func_02002838, 50, 36, 13, 13, 60, 5);
    }
  } else if (scene == (s32)&SceneId_Ov678Room9) {
    *effect = 516;
    Func_02002614(gData_02002ad8, 0, Func_0200151c);
    if (!GameFlag_Test(824)) {
      GameFlag_SetBit(825);
    } else if (GameFlag_Test(826)) {
      Actor_SetModeById(8, 3);
      Call3(Func_020028d0, 8, 0xb80000, 0x1580000);
      Actor_GetById(8)->collisionFlags = 0;
      Actor_GetById(8)->priorityFlags |= 2;
      Actor_SetSpritePriority(8, 3);
      Call6(Func_02002840, 11, 19, 1, 1, 11, 21);
    }
    if (!GameFlag_Test(827)) {
      GameFlag_SetBit(828);
    } else if (GameFlag_Test(829)) {
      Actor_SetModeById(9, 3);
      Call3(Func_020028d0, 9, 0x1f80000, 0xb80000);
      Actor_GetById(9)->collisionFlags = 0;
      Actor_GetById(9)->priorityFlags |= 2;
      Call6(Func_02002840, 30, 13, 1, 1, 31, 11);
    }
  } else if (scene == (s32)&SceneId_Ov678Room10) {
    *effect = 516;
    piece = Actor_GetById(19);
    if (GameFlag_Test(0x330)) {
      Call3(Func_020028d0, 19, 0x1e00000, 0xf80000);
      if (!GameFlag_Test(265))
        Call3(Func_020028d0, 10, 0x1b80000, 0xe80000);
      Call6(Func_02002838, 47, 50, 2, 14, 29, 1);
      Call6(Func_02002838, 50, 59, 13, 5, 18, 15);
    } else {
      piece->motionFlags = 0;
      piece->y = 0x200000;
    }
    if (!GameFlag_Test(817)) {
      GameFlag_SetBit(818);
    } else if (GameFlag_Test(819)) {
      Actor_SetModeById(9, 3);
      Call3(Func_020028d0, 9, 0x780000, 0xf80000);
      Actor_GetById(9)->collisionFlags = 0;
      Actor_GetById(9)->priorityFlags |= 2;
      Call6(Func_02002840, 7, 17, 1, 1, 7, 15);
    } else {
      Actor_SetModeById(9, 0);
    }
    if (!GameFlag_Test(820)) {
      GameFlag_SetBit(821);
    } else {
      Call6(Func_02002840, 27, 14, 1, 1, 27, 13);
      if (GameFlag_Test(822)) {
        Actor_SetModeById(10, 3);
        Call3(Func_020028d0, 10, 0x1980000, 0x1080000);
        Actor_GetById(10)->collisionFlags = 0;
        Actor_GetById(10)->priorityFlags |= 2;
        Call6(Func_02002840, 25, 18, 1, 1, 25, 16);
      } else {
        Actor_SetModeById(10, 0);
      }
    }
  } else if (scene == (s32)&SceneId_Ov678Room11) {
    *effect = 516;
    if (!GameFlag_Test(0x340)) {
      GameFlag_SetBit(833);
    } else {
      Actor_SetModeById(8, 0);
      Call6(Func_02002840, 7, 39, 1, 1, 7, 38);
    }
    if (!GameFlag_Test(834))
      GameFlag_SetBit(835);
    else
      Actor_SetModeById(9, 0);
    if (!GameFlag_Test(840))
      Actor_SetWideSprite(0x40, 0);
    entrance = gAdventure.entrance;
    if ((u32)(entrance - 2) <= 1) {
      if (GameFlag_Test(0x340))
        Func_02002084(0);
      else
        Func_02002114(0);
    }
    if (entrance == 6) {
      if (GameFlag_Test(834))
        Func_02002084(0);
      else
        Func_02002114(0);
    }
    if ((u32)(entrance - 4) <= 1)
      Func_02002828();
  }
  return 0;
}
#endif
