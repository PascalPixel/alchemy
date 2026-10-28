#ifndef ALCHEMY_MODULE_SCENE_SHARED_COLOSSO_SOLO_H
#define ALCHEMY_MODULE_SCENE_SHARED_COLOSSO_SOLO_H

#include "types.h"

void Colosso_PlaceFriendsAtCheeringSpots(void);

void Colosso_ControlCharacterAlone(s32 id);

void Colosso_RaiseTakePlaceOnPsynergy(void);

void Colosso_AskDoneCheering(s32 id);

void Colosso_ClearCheeringSpots(void);

/// What a stage's guide does next (Colosso_GreetAtStage), and so how
/// Colosso_OfferCheeringSpot offers a friend's cheering there.
enum ColossoGuideReply {
  /// Describe the stage: yes to "Would you like to hear a description of this
  /// stage?", asked the first time, or "Descriptions" afterwards.
  COLOSSO_GUIDE_DESCRIBE = 0,
  /// No description, then offer a friend's cheering.
  COLOSSO_GUIDE_OFFER_CHEERING = 1,
  /// "Cheer" chosen, or a friend already cheers here: the offer is taken as
  /// yes.
  COLOSSO_GUIDE_CHEER = 2,
  /// "Nothing", or the choice cancelled: no offer.
  COLOSSO_GUIDE_NOTHING = 3
};

s32 Colosso_GreetAtStage(s32 speaker, s32 warrior);
void Colosso_RepeatStageNumber(s32 speaker, s32 warrior);
// Native calls precede this definition; see MATCHING.md.
#ifdef UBFIX
void Colosso_OfferCheeringSpot(s32 mode, s32 guide, s32 stage);
#endif

#endif
