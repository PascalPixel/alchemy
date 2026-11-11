#include "types.h"
#include "ram_buffer.h"
#include "field_event.h"
#include "iwram_call.h"
#include "dma.h"
#include "io_reg.h"
#include "event_inline.h"

#define BYTE_AT(base, offset) (*(u8 *)((u8 *)(base) + (offset)))
#define WORD_AT(base, offset) (*(s32 *)((u8 *)(base) + (offset)))

extern s32 gData_020033d8;
extern s32 gData_020033dc;
extern u16 gData_02003404[];
extern u16 gData_02003424[];
extern u16 gData_02003428[];

s32 Map_GetTerrainHeight();
void Func_02002888();
void Func_02002a30();
void Func_02000cc4(void);

void Func_02000d80(s32 mode) {
  gData_020033dc = 1;
  if (mode != 2) {
    Iwram_CopyWords(gData_02003404, (void *)&BG_PLTT_COLOR(12, 0), 32);
    Iwram_CopyWords(gData_02003424, (void *)&BG_PLTT_COLOR(13, 0), 32);
  }
  if (Event_GetFlag(0xa00) || mode == 1) {
    /* FAKEMATCH(temporary): a local holding 0, so the zero is built in r2
     * and the counter's address in r3; as the 0 stored directly, 6 of 44
     * instructions differ in tla-en. */
    s32 zero = 0;

    gData_020033d8 = zero;
    Task_Add(Func_02000cc4, 1152);
  } else {
    Dma_Set(gData_02003428, (void *)&BG_PLTT_COLOR(12, 2), DMA_COPY_WORDS(2),
            REG_DMA3);
  }
}

void Func_02000e04(s32 id) {
  struct FieldActor *object;
  s32 height;

  object = ((struct FieldActor *)Actor_Get(id));
  object->motionFlags = 4;
  height = object->terrainHeight;
  WORD_AT(object, 68) = 0;
  object->y = height + 0x200000;
  {
    u8 *flags = &object->priorityFlags;
    /* FAKEMATCH(temporary): a local holding 128, so the bit is built in the
     * register it is ORed into; as *flags |= 128, 4 of 38 instructions differ
     * in tla-en. */
    s32 bit = 128;

    *flags = bit | *flags;
  }
  height = Map_GetTerrainHeight(BYTE_AT(object, 34), object->x, object->z);
  Func_02002888(object->x, object->z, 0, (height >> 19) + 6);
  Func_02002a30(object->x, object->z, 0, 128);
}
