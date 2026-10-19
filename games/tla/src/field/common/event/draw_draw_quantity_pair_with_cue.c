//--------------------------------------------------------------------------
// draw_draw_quantity_pair_with_cue.c
// Messages: teach an ability and display the learning message.
//--------------------------------------------------------------------------

#include "types.h"
#include "edition.h"
#include "scene.h"
#include "text/msg_ids.h"

void Unit_LearnPsynergyFar(void);
void Audio_PlayCue(s32);
void UiText_DrawQuantity(s32, s32);
void UiText_ShowPositionedMessageAndWaitFar(void *, s32);

//--------------------------------------------------------------------------
// Teach the ability, play a sound, show a message, and wait for advancement.
// unit: character ID (message name 1).
// ability: ability ID (message ability 4).
//--------------------------------------------------------------------------
void UiText_DrawQuantityPairWithCue(s32 unit, s32 ability) {
  // Declared without arguments, but unit and ability pass through in r0 and r1.
  Unit_LearnPsynergyFar();
  Audio_PlayCue(83);
  UiText_DrawQuantity(unit, 1);
  UiText_DrawQuantity(ability, 4);
  UiText_ShowPositionedMessageAndWaitFar(MsgAbilityLearned, 3);
}
