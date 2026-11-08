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

s32 Func_0200004c(struct Actor *object) {
  Actor_SetSingleChildFlags(object, 0);
  return 0;
}

const u8 *Func_02000058(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6adRoom4)
    return gData_02006530;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6adRoom2)
    return gData_02006a0c;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6adRoom1)
    return gData_02006710;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6adRoom3)
    return gData_02006890;
  return gData_02006518;
}

void Func_020000bc(void) {
  u8 *event = Ram_HeapSlots->eventWork;

  /* FAKEMATCH: ⚓️ loads the event work before it builds the offset */
  asm volatile("");
  event[3384] = 1;
}

void Func_020000d0(void) {
  struct Actor *object = Actor_GetById(gMainRoutineWork.currentOwner);
  u32 facing = object->angle;

  if (facing == 0 || facing == 0x8000) {
    s32 x = object->x >> 20;
    s32 z = object->z >> 20;

    if (facing != 0)
      x--;
    else
      x++;
    object = Actor_GetById(10);
    if (object->x >> 20 != x || object->z >> 20 != z) {
      object = Actor_GetById(11);
      if (object->x >> 20 != x || object->z >> 20 != z) {
        object = Actor_GetById(12);
        if (object->x >> 20 != x || object->z >> 20 != z) {
          Func_02005600();
          return;
        }
      }
    }
  }
  if (Func_020055f8(gMainRoutineWork.currentOwner) != -1)
    Func_02005688();
}

void Func_02000168(void) { Func_02005600(); }
