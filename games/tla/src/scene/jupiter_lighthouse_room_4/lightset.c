#include "types.h"
#include "ram_buffer.h"
#include "field_event.h"
#include "iwram_call.h"
#include "dma.h"
#include "io_reg.h"
#include "event_inline.h"

#define BYTE_AT(base, offset) (*(u8 *)((u8 *)(base) + (offset)))
#define WORD_AT(base, offset) (*(s32 *)((u8 *)(base) + (offset)))

extern s32 gData_020026f0;
extern s32 gData_020026f4;
extern u16 gData_0200271c[];
extern u16 gData_0200273c[];
extern u16 gData_02002740[];

s32 Map_GetTerrainHeight();
void Func_02001e28();
void Func_02001ef8();
void Func_020005e4(void);

void Func_020006a0(s32 mode) {
  gData_020026f4 = 1;
  if (mode != 2) {
    Iwram_CopyWords(gData_0200271c, (void *)&BG_PLTT_COLOR(12, 0), 32);
    Iwram_CopyWords(gData_0200273c, (void *)&BG_PLTT_COLOR(13, 0), 32);
  }
  if (Event_GetFlag(0xa00) || mode == 1) {
    /* FAKEMATCH(temporary): a local holding 0, so the zero is built in r2
     * and the counter's address in r3; as the 0 stored directly, not
     * measured. */
    s32 zero = 0;

    gData_020026f0 = zero;
    Task_Add(Func_020005e4, 1152);
  } else {
    Dma_Set(gData_02002740, (void *)&BG_PLTT_COLOR(12, 2), DMA_COPY_WORDS(2),
            REG_DMA3);
  }
}

void Func_02000724(s32 id) {
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
     * register it is ORed into; as *flags |= 128, not measured. */
    s32 bit = 128;

    *flags = bit | *flags;
  }
  height = Map_GetTerrainHeight(BYTE_AT(object, 34), object->x, object->z);
  Func_02001e28(object->x, object->z, 0, (height >> 19) + 6);
  Func_02001ef8(object->x, object->z, 0, 128);
}
