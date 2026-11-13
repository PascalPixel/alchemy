#ifndef ALCHEMY_MODULE_SCENE_YALLAM_H
#define ALCHEMY_MODULE_SCENE_YALLAM_H

/// @file
/// What games/tla/src/scene/ declares for the files that use it, written once.

#include "types.h"

void Yallam_SingMoonSong(void);

void Yallam_SingSunSong(void);
void Yallam_StartWatchers(void);

void Yallam_DigFirstHole(void);

void Yallam_DigSecondHole(void);

void Yallam_DigFourthHole(void);

void Yallam_DigSixthHole(void);

void Yallam_OfferYeppSong(void);

void Yallam_TalkAboutSongGame(void);

void Yallam_ForcePillarDown(void);

void Yallam_DoNothing(void);

extern s32 gYallam_PillarRaised[];
extern s32 gYallam_PillarSunk[];
extern u32 gYallam_FirstVillagerPaces[];
extern u32 gYallam_SecondVillagerPaces[];
extern s16 gYallam_DoorOpening[];
extern s16 gYallam_SecondDoorOpening[];
extern s16 gYallam_ThirdDoorOpening[];
extern s16 gYallam_FourthDoorOpening[];

#endif
