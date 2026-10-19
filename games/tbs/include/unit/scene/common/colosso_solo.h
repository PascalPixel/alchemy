#ifndef ALCHEMY_UNIT_SCENE_COMMON_COLOSSO_SOLO_H
#define ALCHEMY_UNIT_SCENE_COMMON_COLOSSO_SOLO_H

#include "types.h"

void Colosso_PlaceFriendsAtCheeringSpots(void);

void Colosso_ControlCharacterAlone(s32 id);

void Colosso_RaiseTakePlaceOnPsynergy(void);

void Colosso_AskDoneCheering(s32 id);

void Colosso_ClearCheeringSpots(void);
s32 Colosso_GreetAtStage(s32 speaker, s32 competitor);
void Colosso_RepeatStageNumber(s32 speaker, s32 competitor);
// Colosso_OfferCheeringSpot is called before its definition with no
// prototype in scope.

#endif
