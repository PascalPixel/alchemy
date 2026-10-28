#include "room.h"
#include "edition.h"

extern s16 gData_02005b5c[];

void Func_020001f4(s32 unused, s32 id) {
  struct Actor *object = Actor_GetById(id);

  if (object->x >> 20 == 50) {
    GameFlag_SetBit(0xa38);
    object->motionFlags = 3;
  }
}

void Func_0200021c(void) {
  struct Actor *object = Actor_GetById(16);

  Func_02005688();
  if (object->x >> 20 == 50) {
    GameFlag_SetBit(0xa38);
    object->motionFlags = 3;
  }
}

void Func_02000248(s32 unused, s32 id) {
  if (id == 10)
    Actor_GetById(10)->motionFlags = 0;
}

void Func_0200025c(s32 unused, s32 id) {
  if (id == 16) {
    Func_020001f4(unused, 16);
  } else if (id == 10) {
    Actor_GetById(10)->motionFlags = 3;
#if (LANGUAGE != LANGUAGE_JAPANESE) /* Absent from the Japanese edition. */
    Func_020002ec();
#endif
  }
}

void Func_02000280(s32 unused, s32 id) {
  struct Actor *object = Actor_GetById(id);
  s32 kind;

  /* FAKEMATCH: the one-pass block keeps the table's address loaded before the
   * id is scaled (differ 2 without it). */
  do {
    kind = gData_02005b5c[id - 18];
  } while (0);
  Func_02005598(kind, S16_AT(object, 100) << 16,
                (S16_AT(object, 102) << 16) - 0x80000);
}

void Func_020002b8(s32 unused, s32 id) {
  struct Actor *object = Actor_GetById(id);
  const s16 *table;
  s32 offset;
  s32 x;
  s32 kind;

  table = gData_02005b5c;
  /* FAKEMATCH: the barrier and the byte offset load the table's address first
   * and x between the offset's shift and subtraction; plainly, 6 instructions
   * differ in tla-en. */
  asm volatile("" : "+l"(table));
  x = object->x;
  offset = (id - 18) * 2;
  kind = *(s16 *)((u8 *)table + offset);
  if (x >> 20 == object->action >> 4)
    Func_02005598(kind, 0, 0);
}

#if (LANGUAGE != LANGUAGE_JAPANESE) /* not in the Japanese edition */
void Func_020002ec(void) {
  struct Actor *object;
  s32 id;
  s32 x;
  s32 z;

  Func_020054e8(53, 72, 1, 1, 38, 72);
  Func_020054e8(53, 72, 1, 1, 43, 72);
  Func_020054e8_(53, 8, 1, 1, 47, 8);
  for (id = 10; id <= 12; id++) {
    object = Actor_GetById(id);
    x = object->x >> 20;
    z = object->z >> 20;
    if (x == 38 && z == 8)
      Func_020054e8(54, 72, 1, 1, 38, 72);
  }
  for (id = 10; id <= 12; id++) {
    object = Actor_GetById(id);
    x = object->x >> 20;
    z = object->z >> 20;
    if (x == 43 && z == 8)
      Func_020054e8(54, 72, 1, 1, 43, 72);
  }
  object = Actor_GetById(10);
  x = object->x >> 20;
  z = object->z >> 20;
  if (x == 47 && z == 8)
    Func_020054e8(54, 8, 1, 1, 47, 8);
}
#endif
