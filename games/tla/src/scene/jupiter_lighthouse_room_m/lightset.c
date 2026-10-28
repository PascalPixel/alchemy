#include "types.h"
#include "ram_buffer.h"
#include "field_event.h"
#include "iwram_call.h"
#include "dma.h"
#include "io_reg.h"

extern s32 gData_02005a14;
extern s32 gData_02005a18;
extern u16 gData_02005a40[];
extern u16 gData_02005a60[];
extern u16 gData_02005a64[];

s32 GameFlag_Test();
void Task_Add();
void Func_02002f08(void);
struct FieldActor *Object_GetById();
s32 Map_GetTerrainHeight();
void Func_02004774();
void Func_020049b4();

#define BYTE_AT(base, offset) (*(u8 *)((u8 *)(base) + (offset)))
#define WORD_AT(base, offset) (*(s32 *)((u8 *)(base) + (offset)))

void Func_02002fc4(s32 mode) {
  gData_02005a18 = 1;
  if (mode != 2) {
    Iwram_CopyWords(gData_02005a40, (void *)0x05000180, 32);
    Iwram_CopyWords(gData_02005a60, (void *)0x050001a0, 32);
  }
  if (GameFlag_Test(0xa00) || mode == 1) {
    s32 zero = 0; /* FAKEMATCH: ⚓️ builds the zero in r2 and the counter address
                     in r3 */

    gData_02005a14 = zero;
    Task_Add(Func_02002f08, 1152);
  } else {
    Dma_Set(gData_02005a64, (void *)0x05000184, 0x84000002, REG_DMA3);
  }
}

void Func_02003048(s32 id) {
  struct FieldActor *object;
  s32 height;

  object = Object_GetById(id);
  object->motionFlags = 4;
  height = object->terrainHeight;
  /* FAKEMATCH: ⚓️ loads the ground height before it clears the word; without it
   * the zero is stored first (2 differ), and storing it after the sum is worse
   * (13) */
  asm volatile("" : "+l"(height));
  WORD_AT(object, 68) = 0;
  object->y = height + 0x200000;
  {
    u8 *flags = &object->priorityFlags;
    s32 bit =
        128; /* FAKEMATCH: ⚓️ builds the bit in the register it ORs into */

    *flags = bit | *flags;
  }
  height = Map_GetTerrainHeight(BYTE_AT(object, 34), object->x, object->z);
  Func_02004774(object->x, object->z, 0, (height >> 19) + 6);
  Func_020049b4(object->x, object->z, 0, 128);
}
