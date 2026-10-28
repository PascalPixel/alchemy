#include "object_runtime.h"

struct BattleFxDefinition {
  u16 id;
  u8 value;
  u8 flags;
};

extern struct BattleFxDefinition gData_080f0308[];

/// Finds the battle effect definition of id among the first 243 entries; when
/// none matches it returns entry 243.
struct BattleFxDefinition *BattleEffect_FindDefinition(u32 id) {
  struct BattleFxDefinition *entry = gData_080f0308;
  u32 index = 0;

  if (entry->id != id) {
    do {
      index += 1;
      entry++;
      if (index > 242)
        break;
    } while (entry->id != id);
  }
  return entry;
}

struct BattleRenderObject {
  u8 unk00[0x50];
  struct ObjectAnimationState *animation;
  u8 kind;
};

s32 BattleEffect_GetAnimationValue(u32 objectId) {
  struct BattleRenderObject *object =
      (struct BattleRenderObject *)ObjectTable_Get(objectId);

  if (object->kind != 1 || object->animation == NULL ||
      object->animation->value28 == NULL) {
    return 0;
  }
  return *object->animation->value28;
}
