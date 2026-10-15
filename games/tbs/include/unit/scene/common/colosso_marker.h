#ifndef ALCHEMY_UNIT_SCENE_COMMON_COLOSSO_MARKER_H
#define ALCHEMY_UNIT_SCENE_COMMON_COLOSSO_MARKER_H

#include "types.h"

/* Korosseo_FadeInCompetitor is called before its definition with no prototype in scope. */
void Korosseo_RestoreCompetitor(s32 id);

void OverlayObject_ResetMotionFields(s32 actor);

/* SceneEffect_SeedMarkerAndInstallTask is called before its definition with no prototype in scope. */
void SceneState_StartMarkerMove(u32 x, u32 y, u32 duration);
void SceneEffect_RemoveMarkerTask(void);
void SceneActor_StartMode5MoveToTile(s32 a, s32 b, s32 c);
/* OverlayObject_PlaceWithScale14000 is called before its definition with no prototype in scope. */

#endif
