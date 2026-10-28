#include "types.h"
#include "ram_buffer.h"
#include "field_event.h"
#include "iwram_call.h"
#include "dma.h"
#include "io_reg.h"

#define BYTE_AT(base, offset) (*(u8 *)((u8 *)(base) + (offset)))
#define WORD_AT(base, offset) (*(s32 *)((u8 *)(base) + (offset)))

extern s32 gData_020069f4;
extern s32 gData_020069f8;
extern u16 gData_02006a2c[];
extern u16 gData_02006a4c[];
extern u16 gData_02006a50[];

s32 GameFlag_Test();
void Task_Add();
struct FieldActor *Actor_GetById();
s32 Map_GetTerrainHeight();
void Func_020057c4();
void Func_020059ec();
void Func_02003f60(void);

void Func_0200401c(s32 mode) {
  gData_020069f8 = 1;
  if (mode != 2) {
    Iwram_CopyWords(gData_02006a2c, (void *)&BG_PLTT_COLOR(12, 0), 32);
    Iwram_CopyWords(gData_02006a4c, (void *)&BG_PLTT_COLOR(13, 0), 32);
  }
  if (GameFlag_Test(0xa00) || mode == 1) {
    s32 zero = 0; /* FAKEMATCH: ⚓️ builds the zero in r2 and the counter address
                     in r3 */

    gData_020069f4 = zero;
    Task_Add(Func_02003f60, 1152);
  } else {
    Dma_Set(gData_02006a50, (void *)&BG_PLTT_COLOR(12, 2), DMA_COPY_WORDS(2), REG_DMA3);
  }
}

void Func_020040a0(s32 id) {
  struct FieldActor *object;
  s32 height;

  object = Actor_GetById(id);
  object->motionFlags = 4;
  height = object->terrainHeight;
  WORD_AT(object, 68) = 0;
  object->y = height + 0x200000;
  /* FAKEMATCH: the one-pass block keeps the ground height load before the word
   * clear; as a bare block the clear comes first (differ 2) */
  do {
    u8 *flags = &object->priorityFlags;
    s32 bit =
        128; /* FAKEMATCH: ⚓️ builds the bit in the register it ORs into */

    *flags = bit | *flags;
  } while (0);
  height = Map_GetTerrainHeight(BYTE_AT(object, 34), object->x, object->z);
  Func_020057c4(object->x, object->z, 0, (height >> 19) + 6);
  Func_020059ec(object->x, object->z, 0, 128);
}
