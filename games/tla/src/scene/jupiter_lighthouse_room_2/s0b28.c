#include "types.h"
#include "field_event.h"

#define BYTE_AT(base, offset) (*(u8 *)((u8 *)(base) + (offset)))
#define WORD_AT(base, offset) (*(s32 *)((u8 *)(base) + (offset)))

struct FieldActor *Actor_GetById();
s32 Map_GetTerrainHeight();
void Func_0200223c();
void Func_0200230c();

void Func_02000b28(s32 id) {
  struct FieldActor *object;
  s32 height;

  /* FAKEMATCH: the one-pass block loads the ground height before the word is
   * cleared; as plain statements the ARM9TDMI schedule clears it first (differ
   * 2). */
  do {
    object = Actor_GetById(id);
    object->motionFlags = 4;
    height = object->terrainHeight;
    WORD_AT(object, 68) = 0;
    object->y = height + 0x200000;
  } while (0);
  {
    u8 *flags = &object->priorityFlags;
    s32 bit =
        128; /* FAKEMATCH: ⚓️ builds the bit in the register it ORs into */

    *flags = bit | *flags;
  }
  height = Map_GetTerrainHeight(BYTE_AT(object, 34), object->x, object->z);
  Func_0200223c(object->x, object->z, 0, (height >> 19) + 6);
  Func_0200230c(object->x, object->z, 0, 128);
}
