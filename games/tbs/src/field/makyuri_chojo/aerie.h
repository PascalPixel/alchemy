#ifndef MAKYURI_CHOJO_AERIE_H
#define MAKYURI_CHOJO_AERIE_H

#include "global_cells.h"
#include "types.h"
#include "field_scene.h"

#define NULL ((void *)0)

extern const struct SceneEntrance gMakyuriChojoEntrances[];
extern const u32 gMakyuriChojoExits[];
extern const struct ScenePlacement gMakyuriChojoPlacements[];
extern const struct SceneEvent gMakyuriChojoEvents[];

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
