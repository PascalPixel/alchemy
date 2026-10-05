/* Declarations shared by the Mercury Lighthouse aerie's sources. */
#ifndef MAKYURI_CHOJO_AERIE_H
#define MAKYURI_CHOJO_AERIE_H

#include "GLOBAL_CELLS.H"
#include "TYPES.H"
#include "FIELD_SCENE.H"

#define NULL ((void *)0)

/* A sprite's priority bits, read and written through the byte that holds
   them. */
struct SpritePriorityByte {
    u8 unknown_00[9];
    u8 tile_high : 2;
    u8 priority : 2;
    u8 palette : 4;
};

extern const struct SceneEntrance gMakyuriChojoEntrances[];
extern const u32 gMakyuriChojoExits[];
extern const struct ScenePlacement gMakyuriChojoPlacements[];
extern const struct SceneEvent gMakyuriChojoEvents[];

/* The pillar the leader last stepped onto. */
extern s32 MakyuriChojo_NearestActor;

s32 FixedPoint_Distance(s32 *first_position, s32 *second_position);
void RunOverlayObjectCommand1(void *object, s32 command);
void Owner_RefreshRatiosOnFlag();
void BattleEffect_CleanupSceneObjects(void);
void Audio_PlayCueFromEventWork();

void MakyuriChojo_SinkActorPair(void);
void MakyuriChojo_RiseActorPair(void);
void MakyuriChojo_LowerCollectedActors(void);
void MakyuriChojo_RaiseCollectedActors(void);

#endif
