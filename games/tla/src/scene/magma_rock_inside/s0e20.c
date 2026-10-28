#include "local.h"

/// The cell watch's work: the object found on the leader's cell.
struct CellWatch {
  u8 unk00[0x14];
  struct ObjectRuntime *object;
};

extern u16 gData_02005500[];
extern u16 gData_0200550c[];

s32 Func_02000dcc(u16 *table);
void Func_02000da4(struct ObjectRuntime *object);

/// The cell watch's callback: in modes 2 and 3 it finds the object on the
/// leader's cell in the room's table (room 4 has its own) and sets it blinking
/// (Func_02000da4) by frame-count bit 2 in mode 2 or bit 1 in mode 3.
void Func_02000e20(s32 mode) {
  u16 *table;
  struct CellWatch *watch = Ram_HeapSlots->effectWork;
  s32 scene;

  table = gData_02005500;
  Func_02004e90(0);
  scene = gAdventure.scene;
  if (scene == (s32)&SceneId_Ov692Room2) {
  } else if (scene == (s32)&SceneId_Ov692Room3) {
  } else if (scene == (s32)&SceneId_Ov692Room4) {
    table = gData_0200550c;
  }
  if (mode == 2) {
    struct ObjectRuntime *object;

    Func_02000dcc(table);
    object = watch->object;
    object->unk5c[7] = 4;
    object->update = (void (*)(void))Func_02000da4;
  }
  if (mode == 3) {
    struct ObjectRuntime *object;

    Func_02000dcc(table);
    object = watch->object;
    object->unk5c[7] = 2;
    object->update = (void (*)(void))Func_02000da4;
  }
}
