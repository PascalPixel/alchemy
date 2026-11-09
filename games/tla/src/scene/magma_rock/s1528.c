/// @file
/// One of the four parts Func_02001738 builds: rises and falls with its owner's action counter.
#include "types.h"
#include "actor.h"

#define BYTE_AT(base, offset) (*(u8 *)((u8 *)(base) + (offset)))

/// A 16.16 position word, read whole or by halves.
union Position {
  s32 word;
  s16 half[2];
};

#define PART_Y(part) (((union Position *)&(part)->y)->word)

void Func_02003a6c(struct Actor *part);

void Func_02001528(struct Actor *part) {
  struct Actor *owner = part->watchedObject;

  if (owner->action > 35 && owner->action < BYTE_AT(part, 98) * 2 + 36) {
    PART_Y(part) = owner->y + 0x200000;
    PART_Y(part) += (owner->action << 19) + (-35 << 19);
  } else if (BYTE_AT(part, 98) * 2 + 36 < owner->action && owner->action < 332) {
    PART_Y(part) = owner->y + (BYTE_AT(part, 98) << 20) + 0x200000;
  } else if (owner->action > 332 && owner->action < BYTE_AT(part, 98) * 2 + 333) {
    PART_Y(part) = owner->y + (BYTE_AT(part, 98) << 20) + 0x200000;
    PART_Y(part) -= (owner->action << 19) - (332 << 19);
  } else if (BYTE_AT(part, 98) * 2 + 333 < owner->action) {
    Func_02003a6c(part);
  }
}
