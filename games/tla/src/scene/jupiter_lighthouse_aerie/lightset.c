#include "types.h"
#include "ram_buffer.h"
#include "field_event.h"
#include "iwram_call.h"
#include "dma.h"
#include "io_reg.h"

extern s32 gData_02003314;
extern s32 gData_02003318;
extern u16 gData_02003340[];
extern u16 gData_02003360[];
extern u16 gData_02003364[];

s32 GameFlag_Test();
void Task_Add();
void Func_0200079c(void);

void Func_02000858(s32 mode) {
  gData_02003318 = 1;
  if (mode != 2) {
    Iwram_CopyWords(gData_02003340, (void *)&BG_PLTT_COLOR(12, 0), 32);
    Iwram_CopyWords(gData_02003360, (void *)&BG_PLTT_COLOR(13, 0), 32);
  }
  if (GameFlag_Test(0xa00) || mode == 1) {
    s32 zero = 0; /* FAKEMATCH: ⚓️ builds the zero in r2 and the counter address
                     in r3 */

    gData_02003314 = zero;
    Task_Add(Func_0200079c, 1152);
  } else {
    Dma_Set(gData_02003364, (void *)&BG_PLTT_COLOR(12, 2), DMA_COPY_WORDS(2), REG_DMA3);
  }
}

#define BYTE_AT(base, offset) (*(u8 *)((u8 *)(base) + (offset)))
#define WORD_AT(base, offset) (*(s32 *)((u8 *)(base) + (offset)))

struct FieldActor *Actor_GetById();
s32 Map_GetTerrainHeight();
void Func_02002a78();
void Func_02002b58();

void Func_020008dc(s32 id) {
  struct FieldActor *object;
  s32 height;

  object = Actor_GetById(id);
  object->motionFlags = 4;
  height = object->terrainHeight;
  /* FAKEMATCH: ⚓️ loads the ground height before it clears the word */
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
  Func_02002a78(object->x, object->z, 0, (height >> 19) + 6);
  Func_02002b58(object->x, object->z, 0, 128);
}
