// Draft, differ 98 of 1266 (was 125): Air's Rock's outer map setup: sets the
// screen effect and Retreat (Ov663Room1's entrance 1) and the leader's priority
// bit 5, then by room: in Ov663Room1 the cells the Whirlwind has cleared
// (flags 0x850 to 0x857) and the beam's opening (0x858), with the leader
// shifted in by entrances 2 to 4 (Func_02002c24); in Ov663Room2, by
// entrance, the leader stepped onto the ledge he came from (before flag
// 265) and the swaying objects 8 to 11 or 17 to 21 (Func_02001b54, their
// phase in their action halfword) and the pushed pillars (flags 0x85f to
// 0x862, 0x887); in Ov663Room3, by entrance, the rising objects 8 to 11,
// the beam (the map work's at 0x178) unless flag 0x87a says it has gone,
// the doors flag 0x87b opens, and entrance 99 runs the beam scene
// Func_02001f7c. Returns 0.
// The DMA wait reads channel 0's control through struct DmaRegisters, as the
// native addresses it from 0x040000b0; the beam's two halfwords are stored
// from an s32 through an s16 pointer, so they are built with moves (as
// halfword constants they come from the pool).
// Left: register numbering: the native builds gAdventure's 0x214 offset and
// the leader's moves in r0 and r1 where the draft uses r1 and r2, loads the
// Retreat scene's id earlier and keeps the 1 for the cell calls' stack
// arguments in a fresh register; the leader reached through an inline
// changes none of it.
// Measured: entrances 2 to 4 as a switch give the native's bound tests (cmp
// #4, bgt, cmp #2, blt); nested ifs fold the lower bound to above 1.
#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "adventure.h"
#include "call.h"
#include "system/io_reg.h"
#include "system/dma.h"

extern char SceneId_Ov663Room1, SceneId_Ov663Room2, SceneId_Ov663Room3;
extern const u8 gData_02003d94[], gData_02003e4c[];
struct ObjectRuntime *Object_GetById(u32 id);
s32 GameFlag_Test(s32 flag);
void Func_02002c24(s32 offset);
void Func_02003b68(s32 x, s32 z, s32 width, s32 height, s32 a, s32 b);
void Func_02003b80(s32 x, s32 z, s32 a, s32 b, s32 width, s32 height);
void Object_SetPartAttribute(struct ObjectRuntime *object, s32 value);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object,
                                        u32 value);
void Func_02001b54(void);
void Func_02003bf8(s32 id, s32 x, s32 z);
void Func_02003cb0(const u8 *table);
void Func_020003e8(const u8 *table);
void Func_02003ca0(const u8 *table);
void Task_Add(void (*callback)(void), s32 priority);
void Func_02002b88(void);
void Func_020024b0(void);
void Func_02000620(void);
void Func_02003b50(void);
void WaitFrames(s32 frames);
void Func_02003c80(s32 a);
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
s32 Camera_FollowActor(s32 id, s32 a);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_02001f7c(void);

#define LEADER() Object_GetById(gAdventure.currentOwner)
#define BEAM ((s32 *)((u8 *)Ram_HeapSlots->mapWork + 0x178))

/// An object that sways (Func_02001b54) from the given phase.
#define SWAY(id, phase)                                                        \
  (Object_GetById(id)->action = (phase),                                       \
   Object_GetById(id)->update = Func_02001b54,                                 \
   ObjectDispatch_SetSingleChildFlags(Object_GetById(id), 0))

/// A step that hangs at the given height and sways.
#define HANG(id, height, phase)                                                \
  (Object_GetById(id)->motionFlags = 0, Object_GetById(id)->y = (height),      \
   SWAY(id, phase))

s32 Func_02002ca0(void) {
  struct ObjectRuntime *leader;
  s32 *beam;

  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 516;
  gAdventure.retreatScene = (s32)&SceneId_Ov663Room1;
  gAdventure.retreatEntrance = 1;
  LEADER()->priorityFlags |= 32;
  if (gAdventure.scene == (s32)&SceneId_Ov663Room1) {
    switch (gAdventure.entrance) {
    case 2:
    case 3:
    case 4:
      Func_02002c24(0x580000);
    }
    if (GameFlag_Test(0x850)) {
      Call6(Func_02003b68, 0x40, 0, 113, 11, 3, 3);
      Call6(Func_02003b80, 0x40, 0, 3, 2, 113, 12);
      Call6(Func_02003b80, 0x40, 0, 3, 2, 49, 12);
    }
    if (GameFlag_Test(0x851)) {
      Call6(Func_02003b68, 0x40, 0, 70, 39, 1, 4);
      Call6(Func_02003b80, 0x40, 0, 1, 3, 70, 40);
      Call6(Func_02003b80, 0x40, 0, 1, 3, 6, 40);
    }
    if (GameFlag_Test(0x852)) {
      Call6(Func_02003b68, 0x40, 0, 103, 38, 1, 4);
      Call6(Func_02003b80, 0x40, 0, 1, 3, 103, 39);
      Call6(Func_02003b80, 0x40, 0, 1, 3, 39, 39);
    }
    if (GameFlag_Test(0x853)) {
      Call6(Func_02003b68, 0x40, 0, 87, 43, 1, 4);
      Call6(Func_02003b80, 0x40, 0, 1, 3, 87, 44);
      Call6(Func_02003b80, 0x40, 0, 1, 3, 23, 44);
    }
    if (GameFlag_Test(0x854)) {
      Call6(Func_02003b68, 0x40, 0, 70, 52, 1, 4);
      Call6(Func_02003b80, 0x40, 0, 1, 3, 70, 52);
      Call6(Func_02003b80, 0x40, 0, 1, 3, 6, 53);
    }
    if (GameFlag_Test(0x855)) {
      Call6(Func_02003b68, 0x40, 0, 79, 49, 3, 3);
      Call6(Func_02003b80, 0x40, 0, 3, 2, 79, 50);
      Call6(Func_02003b80, 0x40, 0, 3, 2, 15, 50);
    }
    if (GameFlag_Test(0x856)) {
      Call6(Func_02003b68, 0x40, 0, 103, 0x30, 1, 4);
      Call6(Func_02003b80, 0x40, 0, 1, 3, 103, 49);
      Call6(Func_02003b80, 0x40, 0, 1, 3, 39, 49);
    }
    if (GameFlag_Test(0x857)) {
      Call6(Func_02003b68, 0x40, 0, 111, 52, 3, 3);
      Call6(Func_02003b80, 0x40, 0, 3, 2, 111, 53);
      Call6(Func_02003b80, 0x40, 0, 3, 2, 47, 53);
    }
    if (GameFlag_Test(0x858)) {
      Call6(Func_02003b68, 68, 0, 79, 0, 11, 10);
      Call6(Func_02003b80, 38, 9, 1, 1, 0x10, 9);
      Call6(Func_02003b80, 38, 9, 1, 1, 24, 9);
      Object_SetPartAttribute(Object_GetById(8), 6);
    }
  }
  if (gAdventure.scene == (s32)&SceneId_Ov663Room2) {
    switch (gAdventure.entrance) {
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
        Call3(Func_02003bf8, 12, 0x1080000, 0x2b80000);
      if (GameFlag_Test(0x860))
        Call3(Func_02003bf8, 13, 0x2d80000, 0x2d80000);
      if (GameFlag_Test(0x861))
        Call3(Func_02003bf8, 15, 0x1580000, 0x3080000);
      if (GameFlag_Test(0x862))
        Call3(Func_02003bf8, 0x10, 0x2780000, 0x3180000);
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
        Call3(Func_02003bf8, 22, 0x3180000, 0x1280000);
      Func_02003cb0(gData_02003d94);
      Func_020003e8(gData_02003d94);
      HANG(17, 0x100000, 60);
      HANG(19, 0x500000, 90);
      HANG(18, 0xffd00000, 100);
      HANG(20, 0x900000, 180);
      Object_GetById(21)->motionFlags = 0;
      Object_GetById(21)->y = 0xff700000;
      ObjectMotion_SetActionVariant(21, 2);
      Object_GetById(21)->priorityFlags |= 2;
      Object_GetById(21)->action = 180;
      ((u8 *)Object_GetById(21))[98] = 1;
      Object_GetById(21)->update = Func_02001b54;
      ObjectDispatch_SetSingleChildFlags(Object_GetById(21), 0);
      break;
    }
  }
  if (gAdventure.scene == (s32)&SceneId_Ov663Room3) {
    switch (gAdventure.entrance) {
    case 1:
    case 2:
    case 3:
      Task_Add(Func_02002b88, 0x480);
      Object_GetById(8)->motionFlags = 0;
      Object_GetById(8)->y = 0xfff80000;
      ObjectMotion_SetActionVariant(8, 2);
      Object_GetById(8)->action = 60;
      ((u8 *)Object_GetById(8))[98] = 0;
      Object_GetById(8)->update = Func_02001b54;
      ObjectDispatch_SetSingleChildFlags(Object_GetById(8), 0);
      Object_GetById(9)->motionFlags = 0;
      Object_GetById(9)->y = 0xfff80000;
      ObjectMotion_SetActionVariant(9, 2);
      Object_GetById(9)->action = 180;
      ((u8 *)Object_GetById(9))[98] = 0;
      Object_GetById(9)->update = Func_02001b54;
      ObjectDispatch_SetSingleChildFlags(Object_GetById(9), 0);
      Object_GetById(10)->terrainId = 2;
      ObjectMotion_SetActionVariant(10, 1);
      Object_GetById(10)->y = 0xd00000;
      Object_GetById(10)->terrainHeight = Object_GetById(10)->y;
      Object_GetById(11)->terrainId = 2;
      ObjectMotion_SetActionVariant(11, 1);
      Object_GetById(11)->y = 0xd00000;
      Object_GetById(11)->terrainHeight = Object_GetById(11)->y;
      if (GameFlag_Test(0x310))
        Call3(Func_02003bf8, 10, 0x1f80000, 0x3680000);
      if (GameFlag_Test(785))
        Call3(Func_02003bf8, 11, 0x2080000, 0x3680000);
      Func_02000620();
      beam = BEAM;
      beam[2] = 0x5800000;
      beam[3] = 0x2400000;
      beam[4] = 0x4000;
      beam[5] = 0x4000;
      Func_02003b50();
      WaitFrames(1);
      break;
    case 6:
      if (!GameFlag_Test(265))
        LEADER()->y += 0x180000;
    case 7:
      Func_02002c24(0x480000);
    case 4:
    case 5:
      ObjectMotion_SetActionVariant(gAdventure.currentOwner, 1);
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
      Object_GetById(12)->priorityFlags |= 2;
      Func_02003c80(0);
      if (GameFlag_Test(0x879))
        Call3(Func_02003bf8, 13, 0xb80000, 0x1180000);
      if (!GameFlag_Test(0x87a)) {
        *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 0x100;
        REG_BLDALPHA = 0x1008;
        REG_BLDCNT = 0x3f42;
        while ((s32)((volatile struct DmaRegisters *)0x040000b0)->control < 0)
          ;
        Task_Add(Func_020024b0, 0xc80);
        beam = BEAM;
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
        Object_SetPartAttribute(Object_GetById(12), 6);
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
      beam = BEAM;
      beam[2] = 0x4800000;
      beam[3] = 0x3200000;
      beam[4] = 0xd000;
      beam[5] = 0xd000;
      if (!GameFlag_Test(0x87b)) {
        Call6(Func_02003b68, 29, 9, 62, 2, 9, 9);
        Call6(Func_02003b80, 29, 9, 9, 9, 62, 2);
        Call6(Func_02003b68, 29, 73, 62, 66, 9, 9);
        Call6(Func_02003b68, 52, 2, 29, 9, 9, 9);
        Call6(Func_02003b80, 52, 2, 9, 9, 29, 9);
        Call6(Func_02003b68, 52, 66, 29, 73, 9, 9);
      } else {
        Object_SetPartAttribute(Object_GetById(14), 6);
      }
      ObjectMotion_SetActionVariant(14, 1);
      Func_02003b50();
      WaitFrames(1);
      if (GameFlag_Test(265)) {
        leader = LEADER();
        leader->terrainId = 0;
        leader->terrainHeight = leader->y =
            Map_GetTerrainHeight(0, leader->x, leader->z - leader->y);
        Camera_FollowActor(gAdventure.currentOwner, 0);
      }
      break;
    case 99:
      Call6(Func_02003b68, 29, 9, 62, 2, 9, 9);
      Call6(Func_02003b80, 29, 9, 9, 9, 62, 2);
      Call6(Func_02003b68, 29, 73, 62, 66, 9, 9);
      Call6(Func_02003b68, 52, 2, 29, 9, 9, 9);
      Call6(Func_02003b80, 52, 2, 9, 9, 29, 9);
      Call6(Func_02003b68, 52, 66, 29, 73, 9, 9);
      Event_StartScreenTransition();
      Event_WaitTransitionFrames();
      Func_02001f7c();
      break;
    }
  }
  return 0;
}
