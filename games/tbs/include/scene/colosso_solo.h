#ifndef ALCHEMY_COLOSSO_SOLO_H
#define ALCHEMY_COLOSSO_SOLO_H

#include "flags.h"
#include "types.h"

/// The exits shared/colosso_solo.c takes, numbered alike in the three Colosso
/// match scenes (scene_exits.h): its way from lending a hand to the race (each
/// scene's LEND_A_HAND_TO_RACE), and its way back out to the Colosseum.
enum ColossoSoloExit {
  COLOSSO_SOLO_TO_RACE = 10,
  COLOSSO_SOLO_TO_COLOSSEUM = 11
};

/// Closes the step trigger at each match scene's arena. Nothing in the game
/// sets it.
#define FLAG_COLOSSEUM_ARENA_CLOSED FLAG_SCENE_TEMP_10

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
// UB: the scenes call this before its definition with no prototype, as a
// function returning int.
#ifdef UBFIX
void Colosso_OfferCheeringSpot(s32 mode, s32 guide, s32 stage);
#endif

#endif
