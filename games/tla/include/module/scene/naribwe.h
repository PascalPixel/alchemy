#ifndef ALCHEMY_MODULE_SCENE_NARIBWE_H
#define ALCHEMY_MODULE_SCENE_NARIBWE_H

/// @file
/// What The Lost Age's code declares for the files that use it, written once.

#include "types.h"

void Naribwe_PrepareBlaze(void);
void Naribwe_AskAboutKibomboExpedition(void);

void Naribwe_AskWhereFrom(void);

void Naribwe_AskGoingToKibombo(void);

void Naribwe_EnterDoor(void);

void Naribwe_BlowAwayIvy(void);

void Naribwe_RevealStone(void);

extern u32 gNaribwe_BrazierBurns[];
extern u32 gNaribwe_FlameRises[];

extern u32 gNaribwe_IvyAnimation[];
extern s16 gNaribwe_DoorOpening[];

#endif
