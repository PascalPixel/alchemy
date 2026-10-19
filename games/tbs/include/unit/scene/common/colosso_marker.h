#ifndef ALCHEMY_UNIT_SCENE_COMMON_COLOSSO_MARKER_H
#define ALCHEMY_UNIT_SCENE_COMMON_COLOSSO_MARKER_H

#include "types.h"

// Colosso_FadeInCompetitor is called before its definition with no prototype in
// scope.
void Colosso_RestoreCompetitor(s32 id);

void OverlayObject_ResetMotionFields(s32 actor);

// Colosso_ShowPointer is called before its definition with no
// prototype in scope.
void Colosso_MovePointer(u32 x, u32 y, u32 duration);
void Colosso_HidePointer(void);
void Colosso_WalkActorTo(s32 a, s32 b, s32 c);
// Colosso_WalkActorToAndStand is called before its definition with no
// prototype in scope.

#endif
