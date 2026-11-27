#ifndef ALCHEMY_COLOSSO_SOLO_H
#define ALCHEMY_COLOSSO_SOLO_H

// Lending a hand in the three Colosso finals matches (shared/colosso_solo.c): Garet, Ivan or Mia
// looks round the stages alone, and the stages' guides offer them a place to cheer from.

#include "flags.h"
#include "types.h"

// The exits shared/colosso_solo.c takes, numbered alike in the three Colosso match scenes
// (scene_exits.h): its way from lending a hand to the race (each scene's LEND_A_HAND_TO_RACE), and
// its way back out to the Colosseum.
typedef enum COLOSSO_SOLO_EXIT
{
    COLOSSO_SOLO_TO_RACE      = 10,
    COLOSSO_SOLO_TO_COLOSSEUM = 11
} COLOSSO_SOLO_EXIT;

// Closes the step cue at each match scene's arena. Nothing in the game sets it.
#define FLAG_COLOSSEUM_ARENA_CLOSED FLAG_SCENE_TEMP_10
// Someone has reached the arena: whoever comes second, Isaac (scColossoTakePrize) or the opponent
// (scColossoUpdatePathOpponent), walks up to the second prize.
#define FLAG_ARENA_REACHED          FLAG_SCENE_TEMP_11

// Puts Garet, Ivan and Mia back where they chose to cheer.
void scColossoPlaceFriendsAtCheeringSpots(void);

// Makes the character the only party member, the one the player moves.
void scColossoControlCharacterAlone(Sint32 id);

// An event: a friend looking round alone takes a place when Psynergy is aimed at them.
void scColossoRaiseTakePlaceOnPsynergy(void);

// A stage's guide asks a friend looking round alone whether they are done.
void scColossoAskDoneCheering(Sint32 id);

// Forgets where Garet, Ivan and Mia cheer from.
void scColossoClearCheeringSpots(void);

// What a stage's guide does next (scColossoGreetAtStage), and so how scColossoOfferCheeringSpot
// offers a friend's cheering there.
typedef enum COLOSSO_GUIDE_REPLY
{
    // Describe the stage: yes to "Would you like to hear a description of this stage?", asked the
    // first time, or "Descriptions" afterwards.
    COLOSSO_GUIDE_DESCRIBE       = 0,
    // No description, then offer a friend's cheering.
    COLOSSO_GUIDE_OFFER_CHEERING = 1,
    // "Cheer" chosen, or a friend already cheers here: the offer is taken as yes.
    COLOSSO_GUIDE_CHEER          = 2,
    // "Nothing", or the choice cancelled: no offer.
    COLOSSO_GUIDE_NOTHING        = 3
} COLOSSO_GUIDE_REPLY;

// A stage's guide names the stage; returns a COLOSSO_GUIDE_REPLY.
Sint32 scColossoGreetAtStage(Sint32 speaker, Sint32 warrior);
// A stage's guide names the stage again after describing it.
void   scColossoRepeatStageNumber(Sint32 speaker, Sint32 warrior);
// UB: the scenes call this before its definition with no prototype, as a function returning int.
#if FIX_FUNCTION_SIGNATURES
void scColossoOfferCheeringSpot(Sint32 mode, Sint32 guide, Sint32 stage);
#endif

#endif
