/// @file
/// Speaker definitions: portraits and idle lines by sprite.
/// Part of object.c, the field object, compiled only through it.

#include "../object_internal.h"

/// Returns the speaker definition of sprite id from gBattleFx_DefinitionTable,
/// or its last entry when there is none.
struct BattleEffectEntry *BattleFx_FindDefinition(u32 id) {
  struct BattleEffectEntry *entry = gBattleFx_DefinitionTable;
  u32 definitionIndex = 0;

  if (entry->id != id) {
    do {
      definitionIndex += 1;
      entry++;
      if (definitionIndex > 0x81)
        break;
    } while (entry->id != id);
  }
  return entry;
}

/// Returns the sprite id stored by a live actor's animated sprite, or 0 when
/// that actor has no animated sprite or its sprite-id pointer is absent.
s32 BattleFx_GetAnimationValue(s32 id) {
  struct BattleRenderObject *object = ObjectTable_Get(id);

  if (object->kind != 1 || object->animation == NULL ||
      object->animation->value28 == NULL) {
    return 0;
  }
  return *object->animation->value28;
}

/// Returns the portrait shown beside the messages of sprite id (0x100 plus
/// its definition's value), or 0 with no portrait or with gGameState.speech
/// off.
s32 BattleFx_GetResourceId(u32 id) {
  u8 portraitId;

  if (gGameState.speech == 0 ||
      (portraitId = BattleFx_FindDefinition(id)->value) == 0xFF) {
    return 0;
  }
  return portraitId + 0x100;
}

/// Returns the definition flags of actor id's sprite, which choose its line
/// when it has nothing to say.
s32 BattleFx_GetFlags(s32 id) {
  return BattleFx_FindDefinition(BattleFx_GetAnimationValue(id))->flags;
}
