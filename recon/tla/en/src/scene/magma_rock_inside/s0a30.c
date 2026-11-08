#include "../../../../../../games/tla/src/scene/magma_rock_inside/local.h"
#include "io_reg.h"

extern const u8 gData_02005528[];
extern const u8 gData_0200552e[];

void Func_02004d40(void);
void Func_02004d48(void);
void Func_02004ee8(void);
void Func_020006dc(const u8 *data);
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
struct Actor *OverlayObject_PrepareObject(s32 x, s32 y, s32 z, s32 type);
void Func_0200063c(struct Actor *object);
void Func_02004f30(s32 sound);
void Func_02004cb0(struct Actor *object);
void Func_02004ce8(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);

void Func_02000a30(void) {
  struct Actor *object =
      *(struct Actor **)((u8 *)Ram_HeapSlots->eventWork + 0xcbc);
  /* FAKEMATCH(pin): copy in r5, as the native keeps it; as a plain local,
   * 78 of 281 instructions differ in tla-en. */
  register struct Actor *copy asm("r5");
  s32 scene;
  s32 x;
  s32 z;

  Func_02004d40();
  Func_02004e90(0);
  Func_02004ee8();
  scene = gMainRoutineWork.scene;
  if (scene == (s32)&SceneId_Ov692Room6) {
    Func_020006dc(gData_02005528);
    if (Map_GetTerrainHeight(2, object->x, object->z) != object->y) {
      object->terrainId = 2;
      object->motionFlags = 3;
      wait(1);
      if (GameFlag_Test(SPLIT_CONSTANT(0x302))) {
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
  scene = gMainRoutineWork.scene;
  if (scene == (s32)&SceneId_Ov692Room2) {
    if (z == 16)
      GameFlag_SetBit(2544);
  } else if (scene == (s32)&SceneId_Ov692Room3) {
    if (z == 41)
      GameFlag_SetBit(SPLIT_CONSTANT(0x9f1));
  } else if (scene == (s32)&SceneId_Ov692Room4) {
    if (x == 21) {
      if (z == 43)
        GameFlag_SetBit(SPLIT_CONSTANT(0x9f2));
      if (z == 45)
        GameFlag_SetBit(SPLIT_CONSTANT(0x9f3));
      if (z == 47)
        GameFlag_SetBit(SPLIT_CONSTANT(0x9f4));
    }
  } else if (scene == (s32)&SceneId_Ov692Room6) {
    if (x == 47) {
      GameFlag_SetBit(SPLIT_CONSTANT(0x9f5));
      {
        /* FAKEMATCH(temporary): the column and row in locals, so each stack
         * word builds in its own register; as constants, 4 of 281 instructions
         * differ in tla-en. */
        s32 column = 46;
        s32 row = 73;

        Func_02004ce8(column, 74, 1, 1, column, row);
      }
    }
    if (x == 57)
      GameFlag_SetBit(SPLIT_CONSTANT(0x9f6));
  } else if (scene == (s32)&SceneId_Ov692Room7) {
    if (x == 43)
      GameFlag_SetBit(SPLIT_CONSTANT(0x9f7));
  }
  Func_02004d48();
}
