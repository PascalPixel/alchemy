#ifndef ALCHEMY_MODULE_SCENE_SHAMAN_VILLAGE_H
#define ALCHEMY_MODULE_SCENE_SHAMAN_VILLAGE_H

/// @file
/// What games/tla/src/scene/ declares for the files that use it, written once.

#include "types.h"

struct ItemDefinition;

struct ShamanVillageSceneBlock {
  u8 unk00[22];
  u8 field16;
};

extern s16 gShamanVillage_WaveLines[];

void ShamanVillage_WaveWhirlwindLines(void);

extern u32 gShamanVillage_KradenStandsByRodScript[];
extern u32 gShamanVillage_PiersStandsByRodScript[];
extern u32 gShamanVillage_ShebaStandsByRodScript[];
extern u32 gShamanVillage_MoapaLeavesHouseScript[];
extern u32 gShamanVillage_OtherGuardLeavesHouseScript[];
extern u32 gShamanVillage_RejoinLeaderAtHouseScript[];

struct ItemDefinition *Import_ItemGet(s32 item);

extern u32 gShamanVillage_LetteringSprites[];

extern s16 gShamanVillage_LetteringVramEntry[];

extern s16 gShamanVillage_LetteringWait;
extern s16 *gShamanVillage_LetteringCursor;
extern s32 gShamanVillage_LetteringX;
extern s16 gShamanVillage_LetteringSpeed;
extern s16 gShamanVillage_LetteringSpeedFrames;
extern s16 gShamanVillage_LetteringSpeedFrame;
extern s16 gShamanVillage_LetteringScale;
extern s16 gShamanVillage_LetteringScaleFrom;
extern s16 gShamanVillage_LetteringScaleFrames;
extern s16 gShamanVillage_LetteringScaleFrame;
extern s16 gShamanVillage_LetteringBlend;
extern s16 gShamanVillage_LetteringBlendFrom;
extern s16 gShamanVillage_LetteringBlendFrames;
extern s16 gShamanVillage_LetteringBlendFrame;
extern s16 gShamanVillage_LetteringMode;
extern s16 gShamanVillage_LetteringTileOffset;

extern s16 gShamanVillage_LetteringSlideScript[];
extern s16 gShamanVillage_LetteringCountScript[];
extern s16 gShamanVillage_LetteringGoScript[];
extern s16 gShamanVillage_LetteringThrobScript[];
extern u32 gShamanVillage_LetteringThrobFadeScript[];
extern u8 gShamanVillage_LetteringPalettes[];
extern char ResourceId_LetteringBanner;

void Import_DecompressTaggedLz(const void *source, void *destination);

void ShamanVillage_CastWhirlwindOnSand(void);
extern u32 gShamanVillage_KradenWatchesShebaScript[];
extern u32 gShamanVillage_LeaderWatchesShebaScript[];

extern u32 gShamanVillage_JennaWatchesShebaScript[];

extern u32 gShamanVillage_PiersWatchesShebaScript[];
extern u32 gShamanVillage_HeadForTrialRoadScript[];
extern u32 gShamanVillage_RejoinLeaderAtSandScript[];
void ShamanVillage_SetUpVillage(void);
void ShamanVillage_SetUpRoom3(void);
void ShamanVillage_SetUpRoom2(void);

void ShamanVillage_SetUpRoom4(void);

void ShamanVillage_SetUpRoom6(void);

void Party_RestoreHpPp(s32 mode);

void ShamanVillage_TalkToGuard(void);

void ShamanVillage_TellOutsidersToLeave(void);

void ShamanVillage_WarnOfMoapasPower(void);

void ShamanVillage_SendToMoapa(void);

void ShamanVillage_TalkToPriest(void);

void ShamanVillage_AskAboutOtherSide(void);

void ShamanVillage_TalkToMoapa(void);

void ShamanVillage_TalkToShopkeeper(void);

void ShamanVillage_TellFortune(void);

void ShamanVillage_OfferToGiveUp(void);

void ShamanVillage_ChallengeMoapa(void);
void ShamanVillage_TakeLeftPath(void);

void ShamanVillage_TakeRightPath(void);

void ShamanVillage_DropPillar(void);

void ShamanVillage_MeetMoapa(void);

void ShamanVillage_TalkToInnkeeper(void);

#endif
