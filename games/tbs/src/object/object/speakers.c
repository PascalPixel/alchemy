/// @file
/// Speaker definitions: voices and idle lines by sprite.
/// Part of object.c, the field object, compiled only through it.

#include "../object_internal.h"

/// Returns the speaker definition of sprite id from gBattleFx_DefinitionTable,
/// or its last entry, the default, when there is none. The search stops after
/// 0x82 entries, all but the table's last of its 131.
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
/// that actor has no animated sprite or its sprite-id pointer is absent. The
/// actor must exist: a NULL from ObjectTable_Get is read through. Called by
/// BattleFx_GetFlags.
s32 BattleFx_GetAnimationValue(s32 id) {
  struct BattleRenderObject *object = ObjectTable_Get(id);

  if (object->kind != 1 || object->animation == NULL ||
      object->animation->value28 == NULL) {
    return 0;
  }
  return *object->animation->value28;
}

/// Returns the voice of sprite id's messages: the first of its four blip
/// sounds, 0x100 plus its definition's value, or 0 for none (value 0xff) or
/// with the Speech option off (gGameState.speech). UiText_OpenMessageWindowFar
/// takes it in the high half of its last argument and
/// Ui_SetMessageVoiceFromActor as the window's speakerVoiceSound
/// (psynergy_effects/messages.c); the shops ask it for their keepers.
s32 BattleFx_GetResourceId(u32 id) {
  u8 portraitId;

  if (gGameState.speech == 0 ||
      (portraitId = BattleFx_FindDefinition(id)->value) == 0xFF) {
    return 0;
  }
  return portraitId + 0x100;
}

/// Returns the definition flags of actor id's sprite, which choose its line
/// when it has nothing to say: Scene_TalkToActor picks a Halted actor's pair
/// of lines with them, and Halt cast with no scene event (BattleFx_Run) stops
/// nothing when the nearest actor's flags are 0xff
/// (BattleFx_FilterObjectIdByFlags).
s32 BattleFx_GetFlags(s32 id) {
  return BattleFx_FindDefinition(BattleFx_GetAnimationValue(id))->flags;
}
