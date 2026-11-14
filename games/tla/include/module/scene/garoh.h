#ifndef ALCHEMY_MODULE_SCENE_GAROH_H
#define ALCHEMY_MODULE_SCENE_GAROH_H

/// @file
/// What The Lost Age's code declares for the files that use it, written once.

#include "types.h"

struct SpawnObject;

extern s16 gGaroh_Room1DayPlacements[];
extern s32 gGaroh_Room3Triggers[];
void Garoh_WerewolvesRevealThemselves(void);
void Garoh_MahaTellsTheLegend(void);

void Garoh_InitializeRoom1(void);
void Garoh_InitializeRoom3(void);

extern u32 gGaroh_WerewolfLookAroundScript[];
s32 Garoh_MetWerewolves();

void Garoh_OpenDoorAndEnter(void);
void Garoh_WerewolfPeersFromCrevice(void);

void Garoh_EnterCrevice(void);

void Garoh_MeetInTheCave(void);
void Garoh_PlaceFirstBlock(void);
void Garoh_PlaceSecondBlock(void);

void Garoh_PlaceThirdBlock(void);

void Garoh_PlaceFourthBlock(void);

void Garoh_PlaceBlockByLash(void);

void Garoh_RevealRoom4Passage(void);
void Garoh_HideSanctuaryCrevice(void);
void Garoh_RestoreRoom3Dark(void);

void Garoh_HideRoom4Passage(void);

void Garoh_HideCavePassage(void);
void Garoh_CrossRoom3West(void);

void Garoh_CrossRoom3East(void);

extern u32 gFieldEffect_ShortScript[];

extern u32 gFieldEffect_LongScript[];
extern s32 gGaroh_ShrunkenSpriteScript[];
extern s16 gGaroh_DoorFrames[];
extern s16 gGaroh_WideDoorFrames[];
extern s16 gGaroh_TallDoorFrames[];
void Garoh_LashRope(void);

extern u16 gGaroh_WaveLines[];
extern char SceneId_Garoh1;
extern char SceneId_Garoh2;
extern char SceneId_Garoh3;
extern char SceneId_Garoh4;
extern char SceneId_Garoh5;

#endif
