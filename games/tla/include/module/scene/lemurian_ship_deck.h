#ifndef ALCHEMY_MODULE_SCENE_LEMURIAN_SHIP_DECK_H
#define ALCHEMY_MODULE_SCENE_LEMURIAN_SHIP_DECK_H

/// @file
/// What The Lost Age's code declares for the files that use it, written once.

#include "types.h"

struct Actor;

struct MapMode {
  u8 unk00[23];
  struct MapModeBits {
    u8 low : 2;
    u8 mode : 2;
  } __attribute__((packed)) bits;
};

extern u32 gLemurianShipDeck_FirstCrewLooksAround[];

extern u32 gLemurianShipDeck_SecondCrewLooksAround[];

extern u32 gLemurianShipDeck_ThirdCrewLooksAround[];

void LemurianShipDeck_RotateColors12To15(void);

void LemurianShipDeck_ScrollWaves(void);

void LemurianShipDeck_BobDeck(void);

void LemurianShipDeck_TakeTheTiller(void);

void FieldCamera_WaitForArrival(void);

extern s16 gLemurianShipDeck_Puddles[];

void FlagBlock_PlaceAll(const void *data);

void LemurianShipDeck_ShowArrival(void);

void FlagBlock_ApplyFlags(const void *data);

extern u32 gLemurianShipDeck_SparkleFades[];
extern u32 gLemurianShipDeck_ReturnToShipScript[];

void Actor_Release(struct Actor *object);

extern u32 gLemurianShipDeck_ReachPiersShipScript[];
extern u32 gLemurianShipDeck_SmokePuff[];

void LemurianShipDeck_EnterCabin(void);

void LemurianShipDeck_TryHeldDoor(void);

#endif
