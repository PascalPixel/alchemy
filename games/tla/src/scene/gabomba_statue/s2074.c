#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "macros.h"

struct EventWork1cc {
  u8 unk000[0x1cc];
  struct Actor *object;
};

struct IoReg16 {
  u16 value;
};

#define REG_BLDALPHA ((struct IoReg16 *)0x04000052)

/// Makes the scene more translucent the farther inside the object is.
void Func_02002074(void) {
  s32 blend;
  struct Actor *object =
      ((struct EventWork1cc *)Ram_HeapSlots->eventWork)->object;
  s32 level = object->z;

  level = (level - 0xe80000) >> 19;
  CLAMP(level, 0, 16);
  blend = ((16 - level) << 8) + level;
  REG_BLDALPHA->value = blend;
}
