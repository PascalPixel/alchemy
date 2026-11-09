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
      if (record->entries[i]->param != 15)
        record->entries[i]->param = value;
    }
    record->dirty = 1;
  }
}
