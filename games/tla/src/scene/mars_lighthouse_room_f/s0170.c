#include "version.h"

#if TLA_PATCH_1

#include "room.h"

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
  struct Actor *object = Actor_GetById(gMainRoutineWork.currentOwner);
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

#else

#include "room.h"

void Func_02000170(void) { Func_02005688(); }

#endif
