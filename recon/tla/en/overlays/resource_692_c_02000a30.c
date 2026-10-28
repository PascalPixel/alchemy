// Draft, differ 104 of 281: Magma Rock's handler after an object is pushed (the
// object at event work 0xcbc): in rooms 6 and 7, off the ground it drops onto
// layer 2 (with flag 770, or always in room 7, it sinks under a blended copy
// with sound 213 and mode 4; else sound 188); then by room and the object's
// cell it sets the room's flags (2544, 2545 to 2551) and in room 6 at column 47
// copies a cell.
#include "../../../../games/tla/src/scene/magma_rock_inside/local.h"
#include "io_reg.h"

extern const u8 gData_02005528[];
extern const u8 gData_0200552e[];

void Func_02004d40(void);
void Func_02004d48(void);
void Func_02004ee8(void);
void Func_020006dc(const u8 *data);
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
struct Actor *OverlayObject_PrepareObject(s32 x, s32 y, s32 z,
                                                  s32 type);
void Func_0200063c(struct Actor *object);
void Func_02004f30(s32 sound);
void Func_02004cb0(struct Actor *object);
void Func_02004ce8(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);

void Func_02000a30(void) {
  struct Actor *object =
      *(struct Actor **)((u8 *)Ram_HeapSlots->eventWork + 0xcbc);
  struct Actor *copy;
  s32 scene;
  s32 x;
  s32 z;

  Func_02004d40();
  Func_02004e90(0);
  Func_02004ee8();
  scene = gAdventure.scene;
  if (scene == (s32)&SceneId_Ov692Room6) {
    Func_020006dc(gData_02005528);
    if (Map_GetTerrainHeight(2, object->x, object->z) != object->y) {
      object->terrainId = 2;
      object->motionFlags = 3;
      wait(1);
      if (GameFlag_Test(770)) {
        object->gravity = 0x1000;
        object->priorityFlags |= 2;
        REG_BLDALPHA = 0x1000;
        REG_BLDCNT = 0x3f10;
        copy = OverlayObject_PrepareObject(object->x, object->terrainHeight,
                                           object->z + 0x100000, 312);
        Func_0200063c(object);
        Func_02004f30(213);
        Func_02004cb0(copy);
        Func_02004c98(object, 4);
        wait(1);
        REG_BLDCNT = 0;
      } else {
        Func_0200063c(object);
        Func_02004f30(188);
      }
      object->motionFlags = 0;
      object->terrainId = 0;
    }
  } else if (scene == (s32)&SceneId_Ov692Room7) {
    Func_020006dc(gData_0200552e);
    if (Map_GetTerrainHeight(2, object->x, object->z) != object->y) {
      object->terrainId = 2;
      object->motionFlags = 3;
      wait(1);
      object->gravity = 0x1000;
      object->priorityFlags |= 2;
      REG_BLDALPHA = 0x1000;
      REG_BLDCNT = 0x3f10;
      copy = OverlayObject_PrepareObject(object->x, object->terrainHeight,
                                         object->z + 0x100000, 312);
      Func_0200063c(object);
      Func_02004f30(213);
      Func_02004cb0(copy);
      Func_02004c98(object, 4);
      wait(1);
      REG_BLDCNT = 0;
      Func_0200063c(object);
      object->motionFlags = 0;
      object->terrainId = 0;
    }
  }
  x = object->x >> 20;
  z = object->z >> 20;
  scene = gAdventure.scene;
  if (scene == (s32)&SceneId_Ov692Room2) {
    if (z == 16)
      GameFlag_SetBit(2544);
  } else if (scene == (s32)&SceneId_Ov692Room3) {
    if (z == 41)
      GameFlag_SetBit(2545);
  } else if (scene == (s32)&SceneId_Ov692Room4) {
    if (x == 21) {
      if (z == 43)
        GameFlag_SetBit(2546);
      if (z == 45)
        GameFlag_SetBit(2547);
      if (z == 47)
        GameFlag_SetBit(2548);
    }
  } else if (scene == (s32)&SceneId_Ov692Room6) {
    if (x == 47) {
      GameFlag_SetBit(2549);
      Func_02004ce8(46, 74, 1, 1, 46, 73);
    }
    if (x == 57)
      GameFlag_SetBit(2550);
  } else if (scene == (s32)&SceneId_Ov692Room7) {
    if (x == 43)
      GameFlag_SetBit(2551);
  }
  Func_02004d48();
}
