#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "party_state.h"
#include "call.h"

#define EVENT_WORK ((u8 *)Ram_HeapSlots->eventWork)

extern char SceneId_Ov664Room1, SceneId_Ov664Room2, SceneId_Ov664Room3,
    SceneId_Ov664Room4, SceneId_Ov664Room7, SceneId_Ov664Room8,
    SceneId_Ov664Room9;
void Func_02005e14(void);
void Func_02005f2c(s32 skip);
void Func_02005f7c(void);
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
void OverlayObject_WaitUntilIdle(struct ObjectRuntime *object);
void Func_02005f94(s32 cue);
void GameFlag_SetBit(s32 flag);
void GameFlag_ClearBit(s32 flag);
void Func_02005df4(s32 x, s32 z, s32 terrain, s32 height);
struct ObjectRuntime *Object_GetById(u32 id);
void Func_0200062c(s32 layer, s32 x, s32 z, s32 cell);
void Func_02005e1c(void);

/// After the pushed object (the event work's object at 0xcbc) comes to rest:
/// if it stopped above the ground it drops onto it with cue 188; then, by
/// room, the place where it stopped sets the flag for that pillar's spot
/// (Ov664Room1 to 4 and 9), in Ov664Room7 marks two cells and shows objects
/// 13 and 14 by where objects 9, 10 and 11 stand, and in Ov664Room8 and 9
/// sets or clears the flags of the spots along its rows and redraws the
/// pillars' cells (Func_0200062c).
void Func_0200332c(void) {
  struct ObjectRuntime *pushed = *(struct ObjectRuntime **)(EVENT_WORK + 0xcbc);
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
  if (gPartyState.scene == (s32)&SceneId_Ov664Room1) {
    if (x == 103 && z == 41)
      GameFlag_SetBit(0xa8e);
  } else if (gPartyState.scene == (s32)&SceneId_Ov664Room2) {
    if (x == 13 && z == 49)
      GameFlag_SetBit(0x898);
    else if (x == 41 && z == 49)
      GameFlag_SetBit(0x897);
  } else if (gPartyState.scene == (s32)&SceneId_Ov664Room3) {
    if (x == 33 && z == 79)
      GameFlag_SetBit(0x89b);
    else if (x == 77 && z == 71)
      GameFlag_SetBit(0x89c);
    else if (x == 59 && z == 65)
      GameFlag_SetBit(0x89d);
  } else if (gPartyState.scene == (s32)&SceneId_Ov664Room4) {
    if (x == 81 && z == 37)
      GameFlag_SetBit(0x89e);
  } else if (gPartyState.scene == (s32)&SceneId_Ov664Room7) {
    Call4((void (*)())Func_02005df4, 0x2600000, 0x1500000, 2, -4);
    Call4((void (*)())Func_02005df4, 0x2700000, 0x1500000, 2, -4);
    if (x == 43 && z == 87)
      GameFlag_SetBit(0x8d0);
    if ((Object_GetById(9)->x >> 19 == 73 &&
         Object_GetById(9)->z >> 19 == 43) ||
        (Object_GetById(10)->x >> 19 == 73 &&
         Object_GetById(10)->z >> 19 == 43))
      ((u8 *)Object_GetById(13))[99] = 1;
    else
      ((u8 *)Object_GetById(13))[99] = 0;
    if (pushed->x == Object_GetById(11)->x &&
        pushed->z == Object_GetById(11)->z) {
      if (x == 83 && z == 43)
        ((u8 *)Object_GetById(14))[99] = 1;
      else
        ((u8 *)Object_GetById(14))[99] = 0;
    }
  } else if (gPartyState.scene == (s32)&SceneId_Ov664Room8) {
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
    Func_0200062c(2, Object_GetById(8)->x >> 20, Object_GetById(8)->z >> 20, 0);
    Func_0200062c(2, Object_GetById(9)->x >> 20, Object_GetById(9)->z >> 20, 0);
  } else if (gPartyState.scene == (s32)&SceneId_Ov664Room9) {
    if (x == 45 && z == 33)
      GameFlag_SetBit(0x8d3);
    Func_0200062c(2, Object_GetById(10)->x >> 20, Object_GetById(10)->z >> 20,
                  0);
  }
  Func_02005e1c();
}
