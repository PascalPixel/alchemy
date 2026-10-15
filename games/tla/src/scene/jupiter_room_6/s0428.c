#include "jupiter_room_6.h"
#include "iwram_call.h"
#include "dma.h"
#include "io_reg.h"

#define BYTE_AT(base, offset) (*(u8 *)((u8 *)(base) + (offset)))
#define HALF_AT(base, offset) (*(u16 *)((u8 *)(base) + (offset)))
#define WORD_AT(base, offset) (*(s32 *)((u8 *)(base) + (offset)))

extern s32 gData_020023bc;
extern s32 gData_020023c0;
extern u16 gData_020023e8[];

s32 __modsi3();
s32 Map_GetTerrainHeight();
void Func_02001b34();
void Func_02001be4();
void Func_02001250(s32 a, s32 b, s32 c, s32 d, s32 e);
void Func_0200196c(s32 a, s32 b, s32 c);

void Func_02000428(s32 id) {
  struct JupiterRoom6Object *object;
  s32 height;

  /* FAKEMATCH: the one-pass block loads the ground height before the word is
   * cleared; as plain statements the ARM9TDMI schedule clears it first (differ
   * 2). */
  do {
    object = Object_GetById(id);
    object->motionFlags = 4;
    height = object->height;
    WORD_AT(object, 68) = 0;
    object->y = height + 0x200000;
  } while (0);
  {
    u8 *flags = &object->priorityFlags;
    s32 bit = 128; /* FAKEMATCH: the native builds the bit in the register it
                      ORs into */

    *flags = bit | *flags;
  }
  height = Map_GetTerrainHeight(BYTE_AT(object, 34), object->x, object->z);
  Func_02001b34(object->x, object->z, 0, (height >> 19) + 6);
  Func_02001be4(object->x, object->z, 0, 128);
}
