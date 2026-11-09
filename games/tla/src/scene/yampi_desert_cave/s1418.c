/// @file
/// Sets up Yampi Desert Cave's three rooms as each one loads: the first
/// room's palette cycle and Retreat destination, the third room's background
/// priorities, and in each room the objects its flags place.

#include "yampi_desert_cave.h"
#include "actor.h"
#include "event_work.h"
#include "io_reg.h"
#include "version.h"
#include "event_inline.h"

extern char SceneId_Ov661Room1;
extern char SceneId_Ov661Room8;
extern char SceneId_Ov661Room9;
extern u16 gData_020019ec[];
extern u16 gData_020019fc[];
extern u16 gData_02001a28[];
extern u16 gData_02001a54[];
extern const s16 gData_020019cc[];

void GameFlag_SetByte(s32 flag, s32 value);
void Actor_SetWideSprite(s32 id, s32 wide);
s32 Func_02000038(s32 id);
void Func_020007dc(u16 *slots, u16 *colours, u16 *a, u16 *b);
void Func_02000b54(const u8 *entries);
void Func_020011e0(void);
void Func_020018c8(s32 id);
void Func_02001928(s32 value);

s32 Func_02001418(void) {
  ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x204;
  Func_02001928(0);
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov662Room1) {
    Func_020007dc(gData_020019ec, gData_020019fc, gData_02001a28,
                  gData_02001a54);
    Func_020018c8(8);
    Func_020018c8(9);
    Func_020018c8(10);
    Func_02000038(8);
    Func_02000038(9);
    Func_02000038(10);
    if (GameFlag_Test(0x300)) {
      Actor_SetHorizontalPositionWithTerrain(11, 0, 0);
      MapGrid_CopyCellAttributes(32, 13, 1, 1, 34, 13);
    }
    if (gMainRoutineWork.entrance == 1) {
      if (GameFlag_Test(0xa29)) {
        gMainRoutineWork.retreatScene = (s32)&SceneId_Ov661Room1;
        gMainRoutineWork.retreatEntrance = 1;
      } else if (GameFlag_Test(0xa2a)) {
        gMainRoutineWork.retreatScene = (s32)&SceneId_Ov661Room8;
        gMainRoutineWork.retreatEntrance = 2;
      } else if (GameFlag_Test(0xa2b)) {
        gMainRoutineWork.retreatScene = (s32)&SceneId_Ov661Room9;
        gMainRoutineWork.retreatEntrance = 2;
      }
    }
  }
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov662Room2) {
    if (GameFlag_Test(0x303)) {
      /* FAKEMATCH(wrapper): the form kept; as a direct call, 2 of 390
       * instructions differ in tla-en. */
      Actor_SetPosition(8, 138 << 18, 216 << 16);
      MapGrid_CopyCellAttributes(32, 13, 1, 1, 34, 13);
      MapGrid_CopyCellAttributes(95, 3, 3, 3, 98, 13);
    } else {
      MapGrid_CopyCellAttributes(32, 13, 1, 1, 34, 12);
    }
    if (!GameFlag_Test(0x302))
      Actor_SetWideSprite(64, 1);
  }
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov662Room3) {
    if ((u16)(gMainRoutineWork.entrance - 3) <= 2) {
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
      MapGrid_CopyCellAttributes(48, 12, 1, 1, 46, 12);
    if (GameFlag_Test(0x301))
      Actor_SetHorizontalPositionWithTerrain(15, 0, 0);
    if (GameFlag_Test(0x304)) {
      /* FAKEMATCH(wrapper): the form kept; as a direct call, 2 of 390
       * instructions differ in tla-en. */
      Actor_SetPosition(13, 168 << 16, 184 << 16);
      MapGrid_CopyCellAttributes(12, 11, 1, 1, 10, 11);
    } else {
#if TLA_PATCH_1
      struct Actor *actor = Actor_GetById(13);

      MapGrid_CopyCellAttributes(12, 11, 1, 1, actor->x >> 20, actor->z >> 20);
#else
      MapGrid_CopyCellAttributes(12, 11, 1, 1, 8, 10);
#endif
    }
    if (GameFlag_Test(0x305)) {
      /* FAKEMATCH(wrapper): the form kept; as a direct call, 2 of 390
       * instructions differ in tla-en. */
      Actor_SetPosition(12, 154 << 18, 132 << 17);
      MapGrid_CopyCellAttributes(38, 15, 1, 1, 38, 16);
    } else {
      MapGrid_CopyCellAttributes(38, 15, 1, 1, 39, 16);
    }
    if (!GameFlag_Test(65) && !GameFlag_Test(145)) {
      /* FAKEMATCH(wrapper): the form kept; as ((s32)GameFlag_GetByte(0x278)),
       * 18 of 390 instructions differ in tla-en. */
      s32 step = FieldGameFlag_GetByte(0x278);
      s32 placed = GameFlag_Test(0x271);

      if (placed) {
        step = step / 32 * 2;
        Actor_SetHorizontalPositionWithTerrain(16, 0, 0);
        Actor_SetHorizontalPositionWithTerrain(17, gData_020019cc[step] << 19,
                                               gData_020019cc[step + 1] << 19);
      } else {
        if (GameFlag_Test(0x109))
          GameFlag_SetByte(0x278, 0);
        Task_Add(Func_020011e0, 0x480);
        GameFlag_SetBit(0x270);
        Actor_SetSpritePriority(16, 3);
        Actor_GetById(16)->collisionFlags = 0;
      }
    }
  }
  Func_02001928(0);
  return 0;
}
