#ifndef ALCHEMY_MODULE_SCENE_SHRINE_OF_THE_SEA_GOD_H
#define ALCHEMY_MODULE_SCENE_SHRINE_OF_THE_SEA_GOD_H

/// @file
/// What The Lost Age's code declares for the files that use it, written once.

#include "types.h"

struct ShrineOfTheSeaGodObject;

/// A word of the object written through a one-member union (see the
/// FAKEMATCH(signature) tag in ShrineOfTheSeaGod_LashRope).
union ObjectWord {
  s32 value;
};

struct ShrineOfTheSeaGodSpot {
  s32 x;
  s32 z;
  s32 scaleX;
  s32 rotation;
};

struct ShrineOfTheSeaGodEffectParams {
  u8 unk00[0x18];
  u16 kind;
  u8 unk1a[0xe];
};

struct ShrineOfTheSeaGodPartyExtra {
  u8 unk000[0x256];
  s16 value;
};

void ShrineOfTheSeaGod_DjinniRunsWest(void);
void ShrineOfTheSeaGod_SetUpFlatProp(s32 actor);

void ShrineOfTheSeaGod_StartleCompanion(void);

void ShrineOfTheSeaGod_OpenFieldMenu(void);

void ShrineOfTheSeaGod_UseSeaGodsTear(void);

void ShrineOfTheSeaGod_SinkWithWater(void);

void ShrineOfTheSeaGod_FreezeFirstPuddle(void);

void ShrineOfTheSeaGod_FreezeSecondPuddle(void);

void ShrineOfTheSeaGod_FreezeThirdPuddle(void);

void ShrineOfTheSeaGod_LiftBlock(void);

void ShrineOfTheSeaGod_SetDownTorch(void);

void ShrineOfTheSeaGod_MovePillar(void);

void ShrineOfTheSeaGod_DrawLeaderInFront(void);

void ShrineOfTheSeaGod_DrawLeaderBehind(void);

void ShrineOfTheSeaGod_TopplePillarIntoWater(void);

void ShrineOfTheSeaGod_ToppleFirstPillar(void);

void ShrineOfTheSeaGod_HideStone(void);
void ShrineOfTheSeaGod_ThrowRopeAgain(void);
void ShrineOfTheSeaGod_LashRope(void);
void ShrineOfTheSeaGod_DjinniFleesEast(void);

void ShrineOfTheSeaGod_DjinniFleesSouth(void);

void ShrineOfTheSeaGod_DjinniRunsNorth(void);

void ShrineOfTheSeaGod_DjinniHopsOntoLedge(void);
void ShrineOfTheSeaGod_ChaseDjinni(void);
void ShrineOfTheSeaGod_CastSand(void);
void ShrineOfTheSeaGod_EndSand(void);

extern u32 gFieldEffect_MediumScript[];

void ShrineOfTheSeaGod_LandPillarAsLogs(struct ShrineOfTheSeaGodObject *object,
                                        s32 base);
void ShrineOfTheSeaGod_KeepFirstPillarWithLeader(void);

void ShrineOfTheSeaGod_KeepSecondPillarWithLeader(void);

void ShrineOfTheSeaGod_HideSlideActors(void);
void ShrineOfTheSeaGod_TrackUpperLevel(void);
void ShrineOfTheSeaGod_LayerRoom3Props(void);

void ShrineOfTheSeaGod_KeepFlameOnTorch(void);

void ShrineOfTheSeaGod_SetUpPuddle(s32 actor);

void ShrineOfTheSeaGod_InitializeRoom1(void);
void ShrineOfTheSeaGod_InitializeRoom2(void);

void ShrineOfTheSeaGod_InitializeRoom3(void);

void ShrineOfTheSeaGod_InitializeRoom4(void);
void ShrineOfTheSeaGod_InitializeRoom5(void);

void ShrineOfTheSeaGod_InitializeRoom6(void);

void ShrineOfTheSeaGod_InitializeRoom7(void);
void ShrineOfTheSeaGod_InitializeRoom8(void);

void ShrineOfTheSeaGod_InitializeRoom9(void);

void ShrineOfTheSeaGod_InitializeRoom11(void);

void ShrineOfTheSeaGod_LowerWater(void);

void ShrineOfTheSeaGod_RiseOnWater(void);

#endif
