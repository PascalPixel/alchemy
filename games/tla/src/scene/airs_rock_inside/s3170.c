/// @file
/// Scene code of overlay 0x664, in ROM order, from s3170's offset up to the next object the overlay links.

#include "airs_rock_inside.h"

s32 Func_02000690(u32 layer, s32 x, s32 y);
void Func_02005efc(s32 countdown);
void Actor_ResetAndSetPositionInMode2(s32 actor, s32 x, s32 z);

/// When layer 1's cell at the leader's x and z - 1 reads 30, runs the four
/// steps 65, 66, 67 and 62 and moves the leader to (312, 792).
void Func_02003170(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  s32 width;
  s32 height;

  if (Func_02000690(1, leader->x >> 20, (leader->z >> 20) - 1) >> 8 != 30)
    return;
  Func_02005e14();
  Func_02005f2c(0);
  Func_02005f94(188);
  width = 1;
  height = 2;
  Func_02005d9c(65, 76, 19, 92, width, height);
  FieldEvent_Wait(8);
  Func_02005d9c(66, 76, 19, 92, width, height);
  FieldEvent_Wait(8);
  Func_02005d9c(67, 76, 19, 92, width, height);
  FieldEvent_Wait(8);
  Func_02005d9c(62, 76, 19, 92, width, height);
  FieldEvent_Wait(15);
  Actor_GetById(gMainRoutineWork.currentOwner)->motionFlags = 0;
  Actor_GetById(gMainRoutineWork.currentOwner)->speedLimit =
      Actor_GetById(gMainRoutineWork.currentOwner)->acceleration = 0x8000;
  Func_02005f94(123);
  Actor_ResetAndSetPositionInMode2(gMainRoutineWork.currentOwner, 312, 792);
  FieldEvent_Wait(5);
  Func_02005efc(15);
  Func_02005e1c();
}

extern s16 gData_02006050[];
extern s16 gData_02006054[];
extern s16 gData_0200605a[];
extern s16 gData_02006064[];
extern s16 gData_02006068[];
extern s16 gData_02006072[];
extern s16 gData_0200607e[];

void Func_02005df4(s32 x, s32 z, s32 layer, s32 value);
void Func_02005f74(const s16 *list);

static inline void ClearLayer2Shape(s32 x, s32 z) {
  /* FAKEMATCH(wrapper): the call forwarded through an inline, which builds the
   * shared z constant again for the second call, as the ROM does; as a direct
   * call, the constant is kept in r5 across the first call, 12 of 68
   * instructions differ in tla-en. */
  Func_02005df4(x, z, 2, 0);
}

/// Runs each room's list on entering it; room 7 also clears two cells' shapes
/// and room 9 stops object 10's timer.
void Func_02003254(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Ov664Room1) {
    Func_02005f74(gData_02006050);
  } else if (scene == (s32)&SceneId_Ov664Room2) {
    Func_02005f74(gData_02006054);
  } else if (scene == (s32)&SceneId_Ov664Room3) {
    Func_02005f74(gData_0200605a);
  } else if (scene == (s32)&SceneId_Ov664Room4) {
    Func_02005f74(gData_02006064);
  } else if (scene == (s32)&SceneId_Ov664Room7) {
    ClearLayer2Shape(0x2600000, 0x1500000);
    Func_02005df4(0x2700000, 0x1500000, 2, 0);
    Func_02005f74(gData_02006068);
  } else if (scene == (s32)&SceneId_Ov664Room8) {
    Func_02005f74(gData_02006072);
  } else if (scene == (s32)&SceneId_Ov664Room9) {
    Actor_GetById(10)->timer = 0;
    Func_02005f74(gData_0200607e);
  }
}

#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "call.h"

#define EVENT_WORK ((u8 *)Ram_HeapSlots->eventWork)

extern char SceneId_Ov664Room1, SceneId_Ov664Room2, SceneId_Ov664Room3,
    SceneId_Ov664Room4, SceneId_Ov664Room7, SceneId_Ov664Room8,
    SceneId_Ov664Room9;
void Func_02005f7c(void);
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
void Func_0200062c(s32 layer, s32 x, s32 z, s32 cell);

/// After the pushed object (the event work's object at 0xcbc) comes to rest:
/// if it stopped above the ground it drops onto it with cue 188; then, by
/// room, the place where it stopped sets the flag for that pillar's spot
/// (Ov664Room1 to 4 and 9), in Ov664Room7 marks two cells and shows objects
/// 13 and 14 by where objects 9, 10 and 11 stand, and in Ov664Room8 and 9
/// sets or clears the flags of the spots along its rows and redraws the
/// pillars' cells (Func_0200062c).
void Func_0200332c(void) {
  struct Actor *pushed = *(struct Actor **)(EVENT_WORK + 0xcbc);
  s32 x, z;

  Func_02005e14();
  Func_02005f2c(0);
  Func_02005f7c();
  if (Map_GetTerrainHeight(2, pushed->x, pushed->z) != pushed->y) {
    pushed->terrainId = 2;
    pushed->motionFlags = 3;
    OverlayObject_WaitUntilIdle(pushed);
    Func_02005f94(188);
    OverlayObject_WaitUntilIdle(pushed);
    pushed->motionFlags = 0;
    pushed->terrainId = 0;
  }
  x = pushed->x >> 19;
  z = pushed->z >> 19;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov664Room1) {
    if (x == 103 && z == 41)
      GameFlag_SetBit(0xa8e);
  } else if (gMainRoutineWork.scene == (s32)&SceneId_Ov664Room2) {
    if (x == 13 && z == 49)
      GameFlag_SetBit(0x898);
    else if (x == 41 && z == 49)
      GameFlag_SetBit(0x897);
  } else if (gMainRoutineWork.scene == (s32)&SceneId_Ov664Room3) {
    if (x == 33 && z == 79)
      GameFlag_SetBit(0x89b);
    else if (x == 77 && z == 71)
      GameFlag_SetBit(0x89c);
    else if (x == 59 && z == 65)
      GameFlag_SetBit(0x89d);
  } else if (gMainRoutineWork.scene == (s32)&SceneId_Ov664Room4) {
    if (x == 81 && z == 37)
      GameFlag_SetBit(0x89e);
  } else if (gMainRoutineWork.scene == (s32)&SceneId_Ov664Room7) {
    Call4((void (*)())Func_02005df4, 0x2600000, 0x1500000, 2, -4);
    Call4((void (*)())Func_02005df4, 0x2700000, 0x1500000, 2, -4);
    if (x == 43 && z == 87)
      GameFlag_SetBit(0x8d0);
    if ((Actor_GetById(9)->x >> 19 == 73 &&
         Actor_GetById(9)->z >> 19 == 43) ||
        (Actor_GetById(10)->x >> 19 == 73 &&
         Actor_GetById(10)->z >> 19 == 43))
      ((u8 *)Actor_GetById(13))[99] = 1;
    else
      ((u8 *)Actor_GetById(13))[99] = 0;
    if (pushed->x == Actor_GetById(11)->x &&
        pushed->z == Actor_GetById(11)->z) {
      if (x == 83 && z == 43)
        ((u8 *)Actor_GetById(14))[99] = 1;
      else
        ((u8 *)Actor_GetById(14))[99] = 0;
    }
  } else if (gMainRoutineWork.scene == (s32)&SceneId_Ov664Room8) {
    if (x == 35 && z == 59)
      GameFlag_SetBit(0x8d1);
    if (x == 45 && z == 105)
      GameFlag_SetBit(0x8d2);
    if (x == 23 && z == 77) {
      GameFlag_SetBit(0x200);
      GameFlag_SetBit(0xa8f);
    }
    if (z == 59 && (x == 67 || x == 71 || x == 73))
      GameFlag_SetBit(513);
    if (z == 19 && (x == 75 || x == 79))
      GameFlag_SetBit(514);
    if (x == 69 && z == 59)
      GameFlag_ClearBit(513);
    if (x == 77 && z == 19)
      GameFlag_ClearBit(514);
    Func_0200062c(2, Actor_GetById(8)->x >> 20, Actor_GetById(8)->z >> 20, 0);
    Func_0200062c(2, Actor_GetById(9)->x >> 20, Actor_GetById(9)->z >> 20, 0);
  } else if (gMainRoutineWork.scene == (s32)&SceneId_Ov664Room9) {
    if (x == 45 && z == 33)
      GameFlag_SetBit(0x8d3);
    Func_0200062c(2, Actor_GetById(10)->x >> 20, Actor_GetById(10)->z >> 20,
                  0);
  }
  Func_02005e1c();
}
