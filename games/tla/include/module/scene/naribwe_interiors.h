#ifndef ALCHEMY_MODULE_SCENE_NARIBWE_INTERIORS_H
#define ALCHEMY_MODULE_SCENE_NARIBWE_INTERIORS_H

/// @file
/// What The Lost Age's code declares for the files that use it, written once.

#include "types.h"
#include "scene_tables.h"

extern struct Trigger gNaribweInteriors_Triggers[];
extern struct Trigger gNaribweInteriors_TriggersOnReturn[];

void NaribweInteriors_ReadFortune(void);

s32 Func_020011a0(s32 *unit, s32 *slot);

void NaribweInteriors_ReadProng(void);

void NaribweInteriors_ReadWeapon(void);

void NaribweInteriors_ReadArmor(void);

#endif
