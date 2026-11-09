#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "system/io_reg.h"
#include "system/dma.h"
#include "event_inline.h"

extern char SceneId_Ov663Room1, SceneId_Ov663Room2, SceneId_Ov663Room3;
extern const u8 gData_02003d94[], gData_02003e4c[];
void Func_02002c24(s32 offset);
void Actor_SetSingleChildFlags(struct Actor *object, u32 value);
void Func_02001b54(void);
void Func_02003cb0(const u8 *table);
void Func_020003e8(const u8 *table);
void Func_02003ca0(const u8 *table);
void Func_02002b88(void);
void Func_020024b0(void);
void Func_02000620(void);
void Func_02003b50(void);
void Func_02003c80(s32 a);
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_02001f7c(void);

#define LEADER() Actor_GetById(gMainRoutineWork.currentOwner)
#define BEAM ((s32 *)((u8 *)Ram_HeapSlots->mapWork + 0x178))

/// An object that sways (Func_02001b54) from the given phase.
#define SWAY(id, phase)                                                        \
  (Actor_GetById(id)->action = (phase),                                        \
   Actor_GetById(id)->update = Func_02001b54,                                  \
   Actor_SetSingleChildFlags(Actor_GetById(id), 0))

/// A step that hangs at the given height and sways.
#define HANG(id, height, phase)                                                \
  (Actor_GetById(id)->motionFlags = 0, Actor_GetById(id)->y = (height),        \
   SWAY(id, phase))

s32 Func_02002ca0(void) {
  struct Actor *leader;
  s32 *beam;
  volatile struct DmaRegisters *dma;

  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 516;
  gMainRoutineWork.retreatScene = (s32)&SceneId_Ov663Room1;
  gMainRoutineWork.retreatEntrance = 1;
  LEADER()->priorityFlags |= 32;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov663Room1) {
    switch (gMainRoutineWork.entrance) {
    case 2:
    case 3:
    case 4:
      Func_02002c24(0x580000);
    }
    if (GameFlag_Test(0x850)) {
      MapGrid_CopyCellsTo(0x40, 0, 113, 11, 3, 3);
      MapGrid_CopyCellAttributes(0x40, 0, 3, 2, 113, 12);
      MapGrid_CopyCellAttributes(0x40, 0, 3, 2, 49, 12);
    }
    if (GameFlag_Test(0x851)) {
      MapGrid_CopyCellsTo(0x40, 0, 70, 39, 1, 4);
      MapGrid_CopyCellAttributes(0x40, 0, 1, 3, 70, 40);
      MapGrid_CopyCellAttributes(0x40, 0, 1, 3, 6, 40);
    }
    if (GameFlag_Test(0x852)) {
      MapGrid_CopyCellsTo(0x40, 0, 103, 38, 1, 4);
      MapGrid_CopyCellAttributes(0x40, 0, 1, 3, 103, 39);
      MapGrid_CopyCellAttributes(0x40, 0, 1, 3, 39, 39);
    }
    if (GameFlag_Test(0x853)) {
      MapGrid_CopyCellsTo(0x40, 0, 87, 43, 1, 4);
      MapGrid_CopyCellAttributes(0x40, 0, 1, 3, 87, 44);
      MapGrid_CopyCellAttributes(0x40, 0, 1, 3, 23, 44);
    }
    if (GameFlag_Test(0x854)) {
      MapGrid_CopyCellsTo(0x40, 0, 70, 52, 1, 4);
      MapGrid_CopyCellAttributes(0x40, 0, 1, 3, 70, 52);
      MapGrid_CopyCellAttributes(0x40, 0, 1, 3, 6, 53);
    }
    if (GameFlag_Test(0x855)) {
      MapGrid_CopyCellsTo(0x40, 0, 79, 49, 3, 3);
      MapGrid_CopyCellAttributes(0x40, 0, 3, 2, 79, 50);
      MapGrid_CopyCellAttributes(0x40, 0, 3, 2, 15, 50);
    }
    if (GameFlag_Test(0x856)) {
      MapGrid_CopyCellsTo(0x40, 0, 103, 0x30, 1, 4);
      MapGrid_CopyCellAttributes(0x40, 0, 1, 3, 103, 49);
      MapGrid_CopyCellAttributes(0x40, 0, 1, 3, 39, 49);
    }
    if (GameFlag_Test(0x857)) {
      MapGrid_CopyCellsTo(0x40, 0, 111, 52, 3, 3);
      MapGrid_CopyCellAttributes(0x40, 0, 3, 2, 111, 53);
      MapGrid_CopyCellAttributes(0x40, 0, 3, 2, 47, 53);
    }
    if (GameFlag_Test(0x858)) {
      MapGrid_CopyCellsTo(68, 0, 79, 0, 11, 10);
      MapGrid_CopyCellAttributes(38, 9, 1, 1, 0x10, 9);
      MapGrid_CopyCellAttributes(38, 9, 1, 1, 24, 9);
      Actor_SetPartAttribute(Actor_GetById(8), 6);
    }
  }
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov663Room2) {
    switch (gMainRoutineWork.entrance) {
    case 3:
      if (!GameFlag_Test(265))
        LEADER()->z += 0x100000;
      Func_02002c24(0xffb80000);
      goto swaying;
    case 1:
    case 2:
      if (!GameFlag_Test(265))
        LEADER()->z += 0x100000;
      Func_02002c24(0xffd80000);
      goto swaying;
    case 5:
      if (!GameFlag_Test(265)) {
        LEADER()->x += 0x300000;
        LEADER()->y += 0x100000;
      }
    case 6:
      Func_02002c24(0x380000);
    case 4:
    swaying:
      SWAY(8, 60);
      SWAY(9, 180);
      SWAY(10, 0);
      SWAY(11, 60);
      if (GameFlag_Test(0x85f))
        Actor_SetPosition(12, 0x1080000, 0x2b80000);
      if (GameFlag_Test(0x860))
        Actor_SetPosition(13, 0x2d80000, 0x2d80000);
      if (GameFlag_Test(0x861))
        Actor_SetPosition(15, 0x1580000, 0x3080000);
      if (GameFlag_Test(0x862))
        Actor_SetPosition(0x10, 0x2780000, 0x3180000);
      Func_02003cb0(gData_02003d94);
      Func_020003e8(gData_02003d94);
      Func_02003ca0(gData_02003e4c);
      break;
    case 11:
      if (!GameFlag_Test(265)) {
        LEADER()->x += 0x100000;
        LEADER()->y += 0xfff00000;
      }
    case 10:
      if (!GameFlag_Test(265))
        LEADER()->z += 0x100000;
      Func_02002c24(0xffc80000);
    case 12:
      if (GameFlag_Test(0x887))
        Actor_SetPosition(22, 0x3180000, 0x1280000);
      Func_02003cb0(gData_02003d94);
      Func_020003e8(gData_02003d94);
      HANG(17, 0x100000, 60);
      HANG(19, 0x500000, 90);
      HANG(18, 0xffd00000, 100);
      HANG(20, 0x900000, 180);
      Actor_GetById(21)->motionFlags = 0;
      Actor_GetById(21)->y = 0xff700000;
      Actor_SetSpritePriority(21, 2);
      Actor_GetById(21)->priorityFlags |= 2;
      Actor_GetById(21)->action = 180;
      ((u8 *)Actor_GetById(21))[98] = 1;
      Actor_GetById(21)->update = Func_02001b54;
      Actor_SetSingleChildFlags(Actor_GetById(21), 0);
      break;
    }
  }
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov663Room3) {
    switch (gMainRoutineWork.entrance) {
    case 1:
    case 2:
    case 3:
      Task_Add(Func_02002b88, 0x480);
      Actor_GetById(8)->motionFlags = 0;
      Actor_GetById(8)->y = 0xfff80000;
      Actor_SetSpritePriority(8, 2);
      Actor_GetById(8)->action = 60;
      ((u8 *)Actor_GetById(8))[98] = 0;
      Actor_GetById(8)->update = Func_02001b54;
      Actor_SetSingleChildFlags(Actor_GetById(8), 0);
      Actor_GetById(9)->motionFlags = 0;
      Actor_GetById(9)->y = 0xfff80000;
      Actor_SetSpritePriority(9, 2);
      Actor_GetById(9)->action = 180;
      ((u8 *)Actor_GetById(9))[98] = 0;
      Actor_GetById(9)->update = Func_02001b54;
      Actor_SetSingleChildFlags(Actor_GetById(9), 0);
      Actor_GetById(10)->terrainId = 2;
      Actor_SetSpritePriority(10, 1);
      Actor_GetById(10)->y = 0xd00000;
      Actor_GetById(10)->terrainHeight = Actor_GetById(10)->y;
      Actor_GetById(11)->terrainId = 2;
      Actor_SetSpritePriority(11, 1);
      Actor_GetById(11)->y = 0xd00000;
      Actor_GetById(11)->terrainHeight = Actor_GetById(11)->y;
      if (GameFlag_Test(0x310))
        Actor_SetPosition(10, 0x1f80000, 0x3680000);
      if (GameFlag_Test(785))
        Actor_SetPosition(11, 0x2080000, 0x3680000);
      Func_02000620();
      beam = (s32 *)Ram_HeapSlots->mapWork;
      beam = (s32 *)((u8 *)beam + 0x178);
      beam[2] = 0x5800000;
      beam[3] = 0x2400000;
      beam[4] = 0x4000;
      beam[5] = 0x4000;
      Func_02003b50();
      wait(1);
      break;
    case 6:
      if (!GameFlag_Test(265))
        LEADER()->y += 0x180000;
    case 7:
      Func_02002c24(0x480000);
    case 4:
    case 5:
      Actor_SetSpritePriority(gMainRoutineWork.currentOwner, 1);
      break;
    case 8:
    case 9:
      if (!GameFlag_Test(265))
        LEADER()->z += 0x100000;
      Func_02002c24(0xffe80000);
      goto beam;
    case 10:
      if (!GameFlag_Test(265))
        LEADER()->x += 0x300000;
    case 11:
      Func_02002c24(0x900000);
    beam:
      REG_BG1CNT &= ~3;
      REG_BG1CNT |= 1;
      REG_BG2CNT &= ~3;
      REG_BG2CNT |= 2;
      REG_BG3CNT &= ~3;
      REG_BG3CNT |= 3;
      Actor_GetById(12)->priorityFlags |= 2;
      Func_02003c80(0);
      if (GameFlag_Test(0x879))
        Actor_SetPosition(13, 0xb80000, 0x1180000);
      if (!GameFlag_Test(0x87a)) {
        *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 0x100;
        REG_BLDALPHA = 0x1008;
        REG_BLDCNT = 0x3f42;
        dma = (volatile struct DmaRegisters *)0x040000b0;
        while ((s32)dma->control < 0)
          ;
        Task_Add(Func_020024b0, 0xc80);
        beam = (s32 *)Ram_HeapSlots->mapWork;
        beam = (s32 *)((u8 *)beam + 0x178);
        beam[6] = 0x4000;
        beam[7] = 0xffff8000;
        {
          s16 *scale = (s16 *)beam;
          s32 value = 3;

          scale[20] = value;
          value = 0x180;
          scale[21] = value;
        }
      } else {
        REG_BLDALPHA = 0x1000;
        REG_BLDCNT = 0x3f42;
        REG_DISPCNT &= ~0x200;
        Actor_SetPartAttribute(Actor_GetById(12), 6);
      }
      break;
    case 12:
      if (!GameFlag_Test(265))
        LEADER()->y += 0xffe00000;
    case 13:
      if (!GameFlag_Test(265))
        LEADER()->z += 0x100000;
      Func_02002c24(0xffd00000);
    case 14:
      beam = (s32 *)Ram_HeapSlots->mapWork;
      beam = (s32 *)((u8 *)beam + 0x178);
      beam[2] = 0x4800000;
      beam[3] = 0x3200000;
      beam[4] = 0xd000;
      beam[5] = 0xd000;
      if (!GameFlag_Test(0x87b)) {
        MapGrid_CopyCellsTo(29, 9, 62, 2, 9, 9);
        MapGrid_CopyCellAttributes(29, 9, 9, 9, 62, 2);
        MapGrid_CopyCellsTo(29, 73, 62, 66, 9, 9);
        MapGrid_CopyCellsTo(52, 2, 29, 9, 9, 9);
        MapGrid_CopyCellAttributes(52, 2, 9, 9, 29, 9);
        MapGrid_CopyCellsTo(52, 66, 29, 73, 9, 9);
      } else {
        Actor_SetPartAttribute(Actor_GetById(14), 6);
      }
      Actor_SetSpritePriority(14, 1);
      Func_02003b50();
      wait(1);
      if (GameFlag_Test(265)) {
        leader = LEADER();
        leader->terrainId = 0;
        leader->y = leader->terrainHeight =
            Map_GetTerrainHeight(0, leader->x, leader->z - leader->y);
        Camera_FollowActor(gMainRoutineWork.currentOwner, 0);
      }
      break;
    case 99:
      MapGrid_CopyCellsTo(29, 9, 62, 2, 9, 9);
      MapGrid_CopyCellAttributes(29, 9, 9, 9, 62, 2);
      MapGrid_CopyCellsTo(29, 73, 62, 66, 9, 9);
      MapGrid_CopyCellsTo(52, 2, 29, 9, 9, 9);
      MapGrid_CopyCellAttributes(52, 2, 9, 9, 29, 9);
      MapGrid_CopyCellsTo(52, 66, 29, 73, 9, 9);
      Event_StartScreenTransition();
      Event_WaitTransitionFrames();
      Func_02001f7c();
      break;
    }
  }
  return 0;
}
