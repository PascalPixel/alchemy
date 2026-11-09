#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "map.h"
#include "event_inline.h"

#define LAYER_WORD(layer, offset) (*(s32 *)((u8 *)(layer) + (offset)))

extern s32 gData_020024b4;
extern s32 gData_020024b0;


void Func_02000258(void) {
  s32 *layer = (s32 *)((u8 *)Ram_HeapSlots->mapWork + 0x108);
  s32 phase = gData_020024b4;

  layer[2] = phase;
  phase += 0x8000;
  gData_020024b4 = phase;
  if (phase == 0x200000)
    gData_020024b4 = 0;
}

/// Bobs the object returned by Func_080d4568 and the first map layer's word 12
/// together on a sine wave, its phase advancing 0x200 a frame.
void Func_02000288(void) {
  struct MapLayer *layer = &Ram_HeapSlots->mapWork->layers[0];
  struct Actor *object = FieldCamera_GetActor();
  s32 offset = Math_Sin(gData_020024b0) >> 2;

  object->y += offset;
  LAYER_WORD(layer, 12) += offset;
  gData_020024b0 += 0x200;
}
