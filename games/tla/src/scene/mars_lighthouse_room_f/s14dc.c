#include "room.h"

extern s32 gData_02007040;

void Func_020014dc(s32 value) { gData_02007040 = value; }

#include "../../../../games/tla/src/scene/jupiter_lighthouse_aerie/jupiter_lighthouse_aerie.h"
#include "iwram_call.h"
#include "dma.h"
#include "io_reg.h"

extern s32 gData_0200703c;
extern u16 gData_02007068[];

void Func_020014e8(void) {
  u16 colors[16];
  s32 phase = (gData_0200703c / 4) % 5;
  s32 i;
  s32 offset;
  u16 *p;

  if (gData_02007040 != 0) {
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
    /* FAKEMATCH(temporary): the slot and the table address held in locals;
     * as colors[(phase + i) % 5] = *(u16 *)((u8 *)gData_02007068 + offset),
     * 2 of 83 instructions differ in tla-en. */
    s32 slot = (phase + i) % 5;
    u8 *table = (u8 *)gData_02007068;

    colors[slot] = *(u16 *)(table + offset);
  }
  i++;
  offset += 2;
  if (i <= 4)
    goto next;
  Dma_Set(p, (void *)0x05000184, 0x84000002, REG_DMA3);
  gData_0200703c++;
}

#include "types.h"
#include "ram_buffer.h"
#include "field_event.h"

#define BYTE_AT(base, offset) (*(u8 *)((u8 *)(base) + (offset)))
#define WORD_AT(base, offset) (*(s32 *)((u8 *)(base) + (offset)))

extern u16 gData_02007088[];
extern u16 gData_0200708c[];

void Func_02005540();
void Func_020056a8();

void Func_020015a4(s32 mode) {
  gData_02007040 = 1;
  if (mode != 2) {
    Iwram_CopyWords(gData_02007068, (void *)&BG_PLTT_COLOR(12, 0), 32);
    Iwram_CopyWords(gData_02007088, (void *)&BG_PLTT_COLOR(13, 0), 32);
  }
  if (GameFlag_Test(0xa00) || mode == 1) {
    /* FAKEMATCH(temporary): the zero held in a local, so it is built in r2 and
     * the counter's address in r3; as a plain store of 0, 6 of 44
     * instructions differ in tla-en. */
    s32 zero = 0;

    gData_0200703c = zero;
    Task_Add(Func_020014e8, 1152);
  } else {
    Dma_Set(gData_0200708c, (void *)&BG_PLTT_COLOR(12, 2), DMA_COPY_WORDS(2),
            REG_DMA3);
  }
}

#include "event_inline.h"

void Func_02001628(s32 id) {
  struct FieldActor *object;
  s32 height;

  object = (struct FieldActor *)Actor_Get(id);
  object->motionFlags = 4;
  height = object->terrainHeight;
  WORD_AT(object, 68) = 0;
  object->y = height + 0x200000;
  {
    u8 *flags = &object->priorityFlags;
    /* FAKEMATCH(temporary): the bit held in a local, so it is built in the
     * register it ORs into; as *flags |= 128, 4 of 38 instructions differ in
     * tla-en. */
    s32 bit = 128;

    *flags = bit | *flags;
  }
  height = Map_GetTerrainHeight(BYTE_AT(object, 34), object->x, object->z);
  Func_02005540(object->x, object->z, 0, (height >> 19) + 6);
  Func_020056a8(object->x, object->z, 0, 128);
}
