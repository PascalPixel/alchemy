#include "room.h"
#include "input.h"
#include "macros.h"
#include "scene_tables.h"

extern s16 gData_02005f18[];

void Func_02000cb8(s32 id);

void Func_020009ec(void) {
  Func_020027ac(2, 65);
  Func_020027ac(1, 65);
  Func_020027ac(0, 65);
  Flag_Clear(0x350);
  if (Actor_Get(10)->z >> 20 != 15 && Actor_Get(9)->z >> 20 != 15) {
    Func_020027ac(1, 33);
    return;
  }
  if (Actor_Get(9)->z >> 20 != 13) {
    Func_020027ac(0, 17);
    return;
  }
  if (Actor_Get(8)->x >> 20 != 37) {
    Func_020027ac(2, 49);
    return;
  }
  Flag_Set(0x350);
  if (!Flag_IsSet(0x351)) {
    FieldCamera_SetSpeed(0x20000, 0x4000);
    FieldCamera_MoveTo(PIXELS(616), -1, PIXELS(24), 1);
    FieldEvent_Wait(60);
    Func_02005660();
    FieldEvent_SetExitRequest(9);
  }
}

void Func_02000aac(void) {
  Func_020027ac(3, 3);
  FieldCamera_SetSpeed(0x10000, 0x2000);
  FieldCamera_MoveTo(PIXELS(184), -1, PIXELS(560), 1);
}

void Func_02000ad8(void) {
  struct SceneWork *work = Ram_HeapSlots->eventWork;

  Flag_Set(0x106);
  Func_02005560();
  FieldEvent_ResolvePendingActions(0);
  work->unk166 = 0;
  Flag_Clear(0x106);
  Flag_Clear(0x104);
  FieldEvent_Wait(60);
  FieldCamera_SetSpeed(0x30000, 0x6000);
  FieldCamera_MoveTo(PIXELS(184), -1, PIXELS(744), 1);
  Func_020055e0();
  FieldEvent_Wait(60);
  Func_02005568();
}

void Func_02000b40(void) {
  struct Box box;

  if (Func_02004a8c(&box)) {
    Func_02004d10(box);
    Func_020009ec();
    if (Func_020027dc(0) != 64 && Flag_IsSet(0x221)) {
      struct Actor *object = Actor_Get(12);

      object->update = NULL;
      Animation_ApplyChildValues(object, 0);
      Actor_SetAnimation(12, 6);
      Flag_Clear(0x221);
      Func_02000cb8(12);
    }
    if (Func_020027dc(2) != 64 && Flag_IsSet(0x220)) {
      struct Actor *object = Actor_Get(11);

      object->update = NULL;
      Animation_ApplyChildValues(object, 0);
      Actor_SetAnimation(11, 6);
      Flag_Clear(0x220);
      Func_02000cb8(11);
    }
  } else {
    s32 angle;
    const s16 *table;
    u32 *keys;

    Actor_Get(gMainRoutineWork.currentOwner);
    keys = &gInput.held;
    table = gData_02005f18;
    angle = *(s16 *)((u8 *)table + (((*keys >> 4) & 15) << 1));
    if (angle != -1) {
      struct MapLayers *map = (struct MapLayers *)Ram_HeapSlots->mapWork;
      struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
      s32 position[3];
      s32 x;
      s32 z;
      u8 *cells;

      SET_VECTOR(position, (leader->x & 0xfff00000) + 0x80000, leader->y,
                 (leader->z & 0xfff00000) + 0x80000);
      FieldVector_AddPolarOffset(0x100000, angle, position);
      x = position[0] >> 20;
      z = position[2] >> 20;
      cells = Ram_MapCellBuffer;
      if (map != NULL)
        cells = map->layers[2].cells;
      cells += (x + (z << 7)) << 2;
      if (cells[2] != 255)
        Func_02005600();
    }
  }
}
