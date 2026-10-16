// Near misses of overlay 6ad, measured with alchemy diff --symbol:
// Func_02000170 differ 6: three loads sit one row from the listing (object->y
// twice, the position argument). Also tried: the position argument copied
// through an asm-pinned pointer (6), permuter 4 minutes (6). (Func_02000280,
// Func_020002b8 and Func_020002ec are C in s01f4.c.)
#include "../../../../games/tla/src/scene/mars_lighthouse_room_f/room.h"

extern char SceneId_Ov6adRoom1;
extern char SceneId_Ov6adRoom2;
extern char SceneId_Ov6adRoom3;
extern char SceneId_Ov6adRoom4;
extern const u8 gData_02006518[];
extern const u8 gData_02006530[];
extern const u8 gData_02006710[];
extern const u8 gData_02006890[];
extern const u8 gData_02006a0c[];

void Func_02000170(void) {
  s32 position[3];
  struct ObjectRuntime *object = Object_GetById(gPartyState.currentOwner);
  s32 angle = object->angle >> 12;

  angle = ((angle + 2) & 12) << 12;
  position[0] = object->x;
  position[1] = (object->y + 0x40000) & 0xfff80000;
  position[2] = object->z;
  Vector_AddPolar(0x100000, angle, position);
  object = Func_02005680(position, 1);
  if (object != NULL) {
    position[0] = object->x;
    position[1] = object->y;
    position[2] = object->z;
    Vector_AddPolar(0x100000, angle, position);
    if (Func_02005680(position, 1) != NULL)
      return;
  }
  Func_02005688();
}
