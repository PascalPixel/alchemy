#include "room.h"

extern char SceneId_Ov6adRoom1;
extern char SceneId_Ov6adRoom2;
extern char SceneId_Ov6adRoom3;
extern char SceneId_Ov6adRoom4;
extern const u8 gData_02006b8c[];
extern const u8 gData_02006b98[];
extern const u8 gData_02006d78[];
extern const u8 gData_02006e38[];
extern const u8 gData_02006f64[];
s32 Func_02002cfc(void);
void Func_02002ce0(void);
void Func_020043ec(void);
void Func_02004700(s32 a0);

void Func_02000c80(s32 id) {
  struct ObjectRuntime *object = Object_GetById(id);
  struct MapLayers *map = (struct MapLayers *)Ram_HeapSlots->mapWork;
  s32 z = object->z >> 20;
  s32 x = object->x;
  u8 *cell = map->layers[2].cells;
  s32 offset = (x >> 20) + (z << 7);

  /* FAKEMATCH: the one-pass block keeps the cell row's address loaded before x
   * is shifted (differ 6 without it). */
  do {
    cell += offset << 2;
    cell[2] = 255;
  } while (0);
  Object_GetById(id)->priorityFlags = 0;
}

void Func_02000cb8(s32 id) {
  struct ObjectRuntime *object = Object_GetById(id);
  struct MapLayers *map = (struct MapLayers *)Ram_HeapSlots->mapWork;
  s32 z = object->z >> 20;
  s32 x = object->x;
  u8 *cell = map->layers[2].cells;
  s32 offset = (x >> 20) + (z << 7);

  /* FAKEMATCH: as in Func_02000c80 (differ 6 without it). */
  do {
    cell += offset << 2;
    cell[2] = 0;
  } while (0);
  {
    u8 *flags = &Object_GetById(id)->priorityFlags;
    s32 value = 2;

    value |= *flags;
    *flags = value;
  }
}

void Func_02000cf4(s32 unused, s32 id) {
  Func_02000c80(id);
  Flag_Set(0x220);
  if (Func_020027dc(2) != 64) {
    struct ObjectRuntime *object = Object_GetById(id);

    WORD_AT(object, 108) = 0;
    Animation_ApplyChildValues(object, 0);
    Object_SetModeById(id, 6);
    Flag_Clear(0x220);
    Func_02000cb8(id);
  }
}

void Func_02000d38(s32 unused, s32 id) {
  Func_02000c80(id);
  Flag_Set(0x221);
  if (Func_020027dc(0) != 64) {
    struct ObjectRuntime *object = Object_GetById(id);

    WORD_AT(object, 108) = 0;
    Animation_ApplyChildValues(object, 0);
    Object_SetModeById(id, 6);
    Flag_Clear(0x221);
    Func_02000cb8(id);
  }
}

void Func_02000d80(void) {
  Flag_Set(0xa40);
  Func_020054e8(24, 38, 3, 9, 10, 38);
  Func_020054f0(64, 64, 3, 9, 74, 36);
  Func_020054f0(64, 64, 3, 9, 10, 100);
  Func_020054e8(64, 64, 3, 9, 74, 38);
  Func_020054e8(64, 64, 3, 9, 10, 102);
}

void Func_02000df8(void) {
  Func_020043ec();
  Func_02002ce0();
}

const u8 *Func_02000e04(void) {
  if (gPartyState.scene == (s32)&SceneId_Ov6adRoom4)
    return gData_02006b98;
  if (gPartyState.scene == (s32)&SceneId_Ov6adRoom2)
    return gData_02006f64;
  if (gPartyState.scene == (s32)&SceneId_Ov6adRoom1)
    return gData_02006d78;
  if (gPartyState.scene == (s32)&SceneId_Ov6adRoom3)
    return gData_02006e38;
  return gData_02006b8c;
}

void Func_02000e68(void) {
  if (Func_02002cfc())
    Func_02004700(1);
}
