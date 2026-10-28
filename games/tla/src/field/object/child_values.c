#include "types.h"
#include "actor_view.h"
#include "sprite.h"

void Animation_ApplyChildValueToFirst(struct ActorSpriteView *record, u32 value) {
  if (record != NULL) {
    record->entries[0]->param = value;
    record->dirty = 1;
  }
}

void Animation_ApplyChildValuesToRecord(struct ActorSpriteView *record,
                                        u32 value) {
  s32 i;

  if (record != NULL) {
    for (i = 0; i < record->count; i++) {
      /* FAKEMATCH: the byte store may alias the count, forcing its reload. */
      u8 *layer = (u8 *)record->entries[i];

      if (layer[5] != 15)
        layer[5] = value;
    }
    record->dirty = 1;
  }
}
