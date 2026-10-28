/// @file
/// Messages: teach an ability and display the learning message.

#include "types.h"
#include "edition.h"
#include "scene.h"
#include "text/msg_ids.h"

void Unit_LearnPsynergyFar(void);
void Sound_PlayFar(s32);
void MessageArgument_PushFar(s32, s32);
void FieldMessage_ShowFar(void *, s32);

/// Teaches the ability, plays a sound, shows a message and waits for it to
/// advance.
/// unit: character ID (message name 1).
/// ability: ability ID (message ability 4).
void Party_LearnAbility(s32 unit, s32 ability) {
  // Declared without arguments, but unit and ability pass through in r0 and r1.
  Unit_LearnPsynergyFar();
  Sound_PlayFar(83);
  MessageArgument_PushFar(unit, 1);
  MessageArgument_PushFar(ability, 4);
  FieldMessage_ShowFar(MsgAbilityLearned, 3);
}
