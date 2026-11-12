/// @file
/// Scene code of overlay 0x69b, in ROM order, from s09e8's offset up to the next object the overlay links.

#include "../jupiter_lighthouse_aerie/jupiter_lighthouse_aerie.h"
#include "iwram_call.h"
#include "dma.h"
#include "io_reg.h"
#include "types.h"
#include "ram_buffer.h"
#include "event_inline.h"
#include "field_event.h"

extern s32 gData_02002b34;
extern s32 gData_02002b38;
extern u16 gData_02002b98[];

void Func_020009e8(void) {
  u16 colors[16];
  s32 phase = (gData_02002b34 / 4) % 5;
  s32 i;
  s32 offset;
  u16 *p;

  if (gData_02002b38 != 0) {
    u8 *work = Ram_HeapSlots->eventWork;
    s16 *state = (s16 *)(work + 358);

    if (*state != 0 && (*state & 0x3ff) != 153)
      return;
    if (*(s8 *)(work + 0xca4) != 0)
      return;
    if (*(s16 *)(work + 350) != 0)
      return;
  }
  i = 0;
  offset = 4;
next:
  p = colors;
  {
    s32 slot = (phase + i) % 5;
    u8 *table = (u8 *)gData_02002b98;

    colors[slot] = *(u16 *)(table + offset);
  }
  i++;
  offset += 2;
  if (i <= 4)
    goto next;
  Dma_Set(p, (void *)0x05000184, 0x84000002, REG_DMA3);
  gData_02002b34++;
}

extern u16 gData_02002bb8[];
extern u16 gData_02002bbc[];

void Func_02000aa4(s32 mode) {
  gData_02002b38 = 1;
  if (mode != 2) {
    Iwram_CopyWords(gData_02002b98, (void *)&BG_PLTT_COLOR(12, 0), 32);
    Iwram_CopyWords(gData_02002bb8, (void *)&BG_PLTT_COLOR(13, 0), 32);
  }
  if (Event_GetFlag(0xa00) || mode == 1) {
    /* FAKEMATCH(temporary): a local holding 0, so the zero is built in r2
     * and the counter's address in r3; as the 0 stored directly, 6 of 44
     * instructions differ in tla-en. */
    s32 zero = 0;

    gData_02002b34 = zero;
    Task_Add(Func_020009e8, 1152);
  } else {
    Dma_Set(gData_02002bbc, (void *)&BG_PLTT_COLOR(12, 2), DMA_COPY_WORDS(2),
            REG_DMA3);
  }
}

#define BYTE_AT(base, offset) (*(u8 *)((u8 *)(base) + (offset)))
#define WORD_AT(base, offset) (*(s32 *)((u8 *)(base) + (offset)))

s32 Map_GetTerrainHeight();
void Func_0200223c();
void Func_0200230c();

void Func_02000b28(s32 id) {
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
  Func_0200223c(object->x, object->z, 0, (height >> 19) + 6);
  Func_0200230c(object->x, object->z, 0, 128);
}
