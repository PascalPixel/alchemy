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

extern s16 gData_02006350[];

void Func_02000378(void);

extern u32 gData_02004cfc[];
extern u32 gData_02004d40[];
extern u32 gData_02004dc8[];
extern u32 gData_02004e0c[];
extern u32 gData_02004eac[];
extern u32 gData_02004f08[];

struct ItemDefinition *Func_02004894(s32 item);

extern u32 gData_020064d0[];

extern s16 gData_02005158[];

extern s16 gData_020064bc;
extern s16 *gData_020064c0;
extern s32 gData_020064a0;
extern s16 gData_02006504;
extern s16 gData_0200649c;
extern s16 gData_020064a8;
extern s16 gData_020064a4;
extern s16 gData_02006498;
extern s16 gData_02006490;
extern s16 gData_02006508;
extern s16 gData_020064b4;
extern s16 gData_020064b8;
extern s16 gData_020064c4;
extern s16 gData_020064ac;
extern s16 gData_020064b0;
extern s16 gData_02006494;

extern s16 gData_0200515a[];
extern s16 gData_02004b58[];
extern s16 gData_02005186[];
extern s16 gData_02004b80[];
extern u32 gData_02005204[];
extern u8 gData_02004bfe[];
extern char ResourceId_LetteringBanner;

void Import_DecompressTaggedLz(const void *source, void *destination);

void ShamanVillage_CastWhirlwindOnSand(void);
extern u32 gData_02004f3c[];
extern u32 gData_02004f80[];

extern u32 gData_02004fc4[];

extern u32 gData_02005008[];
extern u32 gData_0200504c[];
extern u32 gData_020050bc[];
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
