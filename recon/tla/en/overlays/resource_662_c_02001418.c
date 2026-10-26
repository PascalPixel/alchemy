// Draft, differ 85 of 390 (was 86; ja 90 of 386 before): Yampi Desert Cave's
// map setup for its three rooms: pillars, palette cycle, Retreat destination,
// BG priorities and the chest follower. Left: register choice and the native
// reloading gPartyState after room 1. Func_020018c8(10) goes through Call1
// (86 to 85).
#include "../../../../games/tla/src/scene/yampi_desert_cave/yampi_desert_cave.h"
#include "object_runtime.h"
#include "event_work.h"
#include "io_reg.h"
#include "game/edition.h"

extern char SceneId_Ov661Room1;
extern char SceneId_Ov661Room8;
extern char SceneId_Ov661Room9;
extern u16 gData_020019ec[];
extern u16 gData_020019fc[];
extern u16 gData_02001a28[];
extern u16 gData_02001a54[];
extern const s16 gData_020019cc[];

s32 GameFlag_Test(s32 flag);
s32 GameFlag_GetByte(s32 flag);
void GameFlag_SetByte(s32 flag, s32 value);
void GameFlag_SetBit(s32 flag);
struct ObjectRuntime *Object_GetById(s32 id);
void Object_SetWideSprite(s32 id, s32 wide);
s32 Func_02000038(s32 id);
void Func_020007dc(u16 *slots, u16 *colours, u16 *a, u16 *b);
void Func_02000b54(const u8 *entries);
void Func_020011e0(void);
void Func_020018a8(s32 id, s32 x, s32 z);
void Func_020018c8(s32 id);
void Func_02001928(s32 value);

s32 Func_02001418(void) {
  ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x204;
  Func_02001928(0);
  if (gPartyState.scene == (s32)&SceneId_Ov662Room1) {
    Func_020007dc(gData_020019ec, gData_020019fc, gData_02001a28,
                  gData_02001a54);
    Func_020018c8(8);
    Func_020018c8(9);
    Call1((void (*)())Func_020018c8, 10);
    Func_02000038(8);
    Func_02000038(9);
    Func_02000038(10);
    if (GameFlag_Test(0x300)) {
      Func_020018a8(11, 0, 0);
      YampiDesertCave_Call1848(32, 13, 1, 1, 34, 13);
    }
    if (gPartyState.entrance == 1) {
      if (GameFlag_Test(0xa29)) {
        gPartyState.retreatScene = (s32)&SceneId_Ov661Room1;
        gPartyState.retreatEntrance = 1;
      } else if (GameFlag_Test(0xa2a)) {
        gPartyState.retreatScene = (s32)&SceneId_Ov661Room8;
        gPartyState.retreatEntrance = 2;
      } else if (GameFlag_Test(0xa2b)) {
        gPartyState.retreatScene = (s32)&SceneId_Ov661Room9;
        gPartyState.retreatEntrance = 2;
      }
    }
  }
  if (gPartyState.scene == (s32)&SceneId_Ov662Room2) {
    if (GameFlag_Test(0x303)) {
      Func_020018a8(8, 138 << 18, 216 << 16);
      YampiDesertCave_Call1848(32, 13, 1, 1, 34, 13);
      YampiDesertCave_Call1848(95, 3, 3, 3, 98, 13);
    } else {
      YampiDesertCave_Call1848(32, 13, 1, 1, 34, 12);
    }
    if (!GameFlag_Test(0x302))
      Object_SetWideSprite(64, 1);
  }
  if (gPartyState.scene == (s32)&SceneId_Ov662Room3) {
    if ((u16)(gPartyState.entrance - 3) <= 2) {
      REG_BG1CNT &= ~3;
      REG_BG1CNT |= 1;
      REG_BG2CNT &= ~3;
      REG_BG2CNT |= 3;
      REG_BG3CNT &= ~3;
      REG_BG3CNT |= 3;
    }
    Func_020018c8(8);
    Func_020018c8(9);
    Func_020018c8(10);
    Func_020018c8(11);
    Func_02000038(8);
    Func_02000038(9);
    Func_02000038(10);
    Func_02000038(11);
    Func_02000b54(gData_02001d20);
    if (!GameFlag_Test(0x206))
      YampiDesertCave_Call1848(48, 12, 1, 1, 46, 12);
    if (GameFlag_Test(0x301))
      Func_020018a8(15, 0, 0);
    if (GameFlag_Test(0x304)) {
      Func_020018a8(13, 168 << 16, 184 << 16);
      YampiDesertCave_Call1848(12, 11, 1, 1, 10, 11);
    } else {
#if (LANGUAGE == LANGUAGE_JAPANESE)
      YampiDesertCave_Call1848(12, 11, 1, 1, 8, 10);
#else
      struct ObjectRuntime *object = Object_GetById(13);

      YampiDesertCave_Call1848(12, 11, 1, 1, object->x >> 20, object->z >> 20);
#endif
    }
    if (GameFlag_Test(0x305)) {
      Func_020018a8(12, 154 << 18, 132 << 17);
      YampiDesertCave_Call1848(38, 15, 1, 1, 38, 16);
    } else {
      YampiDesertCave_Call1848(38, 15, 1, 1, 39, 16);
    }
    if (!GameFlag_Test(65) && !GameFlag_Test(145)) {
      s32 step = GameFlag_GetByte(0x278);
      s32 placed = GameFlag_Test(0x271);

      if (placed) {
        s32 index = step / 32;

        Func_020018a8(16, 0, 0);
        Func_020018a8(17, gData_020019cc[index * 2] << 19,
                      gData_020019cc[index * 2 + 1] << 19);
      } else {
        if (GameFlag_Test(0x109))
          GameFlag_SetByte(0x278, 0);
        Scheduler_AddOrUpdateCallback(Func_020011e0, 0x480);
        GameFlag_SetBit(0x270);
        ObjectMotion_SetActionVariant(16, 3);
        Object_GetById(16)->collisionFlags = placed;
      }
    }
  }
  Func_02001928(0);
  return 0;
}
