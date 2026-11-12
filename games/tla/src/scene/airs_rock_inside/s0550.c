/// @file
/// Scene code of overlay 0x664, in ROM order, from s0550's offset up to the next object the overlay links.

#include "airs_rock_inside.h"

extern u32 gData_0200654c[];
extern u32 gData_02006564[];
extern u32 gData_0200663c[];
extern u32 gData_020066b4[];
extern u32 gData_0200675c[];
extern u32 gData_020068ac[];
extern u32 gData_020068f4[];
extern u32 gData_020069e4[];
extern u32 gData_02006aec[];
extern u32 gData_02006bac[];

s32 Math_Cosine(s32 angle);
void Actor_SetAnimationSpeed(struct FieldActor *object, s32 value);
void Map_EnableUpdateCallback(void);
void Map_DisableUpdateCallback(void);
u32 Random16Far(void);

/// On most frames, moves the object to a random spot: x 176 to 431 pixels, y
/// and z up to 31 and 127 pixels past the leader's. Then it keeps its action as
/// the previous one and sets mode 9 after action 212, mode 10 otherwise.
void Func_020004d4(struct FieldActor *object) {
  struct FieldActor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct Actor *self = (struct Actor *)object;

  if (Random16Far() & 0xff) {
    object->x = ((Random16Far() & 0xff) << 16) + (176 << 16);
    {
      s32 r = Random16Far();
      /* FAKEMATCH(pin): the leader's y pinned to r3, which the ROM loads before
       * it builds the mask; as a plain local, 12 of 53 instructions differ in
       * tla-en (24 with z plain too). */
      register s32 y asm("r3") = leader->y;

      object->y = y + ((r & 31) << 16);
    }
    {
      s32 r = Random16Far();
      /* FAKEMATCH(pin): as for y, the leader's z pinned to r3, so it loads
       * before the mask; as a plain local, 12 of 53 instructions differ in
       * tla-en (24 with y plain too). */
      register s32 z asm("r3") = leader->z;

      object->z = z + ((r & 127) << 16);
    }
  }
  self->unk66 = self->action;
  if (self->action == 212)
    Func_02005d5c(object, 9);
  else
    Func_02005d5c(object, 10);
}

void Func_02000550(struct FieldActor *object) {
  u8 *work = Ram_HeapSlots->eventWork;

  if (*(s16 *)(work + 358) != 0 || *(s16 *)(work + 346) != 0 ||
      *(s16 *)(work + 350) != 0 || *(s16 *)(work + 360) != 0) {
    Actor_SetAnimationSpeed(object, 0);
    Map_EnableUpdateCallback();
    return;
  }
  Actor_SetAnimationSpeed(object, 16);
  Map_DisableUpdateCallback();
  if (BYTE_AT(object, 99) != 0) {
    if ((gFrameCount & 7) == 0)
      BYTE_AT(object, 99)--;
    return;
  }
  if (*(s16 *)((u8 *)object + 102) == 0)
    Func_020004d4(object);
  (*(s16 *)((u8 *)object + 102))--;
  /* FAKEMATCH(dead): an empty once-only block, whose loop note ends the
   * scheduling block so the timer is stored before z loads, as the ROM does; as
   * no block, 2 of 87 instructions differ in tla-en. */
  do {
  } while (0);
  object->x += Math_Cosine(((object->z >> 14) + (object->y >> 15)) << 8) / 2;
  object->y += WORD_AT(object, 48);
}

void Func_02000610(void) { Func_02005f2c(0); }

void Func_0200061c(void) { BYTE_AT(Ram_HeapSlots->effectWork, 52) = 1; }

s32 Func_0200062c(u32 layer, s32 x, s32 y, s32 value) {
  struct AirsRockInsideMapWork *map =
      (struct AirsRockInsideMapWork *)Ram_HeapSlots->mapWork;
  u8 *cell;
  s32 old;

  if (map == NULL)
    return 0;
  if (layer <= 2)
    cell = map->layers[layer].cells;
  else
    cell = gMapCellBuffer;
  cell += (x + (y << 7)) << 2;
  old = (cell[2] << 8) | cell[3];
  cell[2] = value >> 8;
  cell[3] = value;
  return old;
}

