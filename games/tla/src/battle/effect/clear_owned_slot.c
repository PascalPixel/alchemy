#include "types.h"
#include "effect_slot.h"
#include "resource.h"
#include "dma.h"
#include "io_reg.h"

void BattleEffect_ClearOwnedSlot(struct FieldParticle *slot) {
  if (slot->object)
    ResourceObject_ReleaseFar((struct ResourceObjectWork *)slot->object);
  Dma_Fill32(0, slot, sizeof *slot);
}
