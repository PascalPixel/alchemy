#ifndef ALCHEMY_UNIT_SCENE_COMMON_COLOSSO_MARKER_H
#define ALCHEMY_UNIT_SCENE_COMMON_COLOSSO_MARKER_H

#include "types.h"

// Native calls precede these definitions; see MATCHING.md.
#ifdef UBFIX
void Colosso_FadeInCompetitor(s32 id, s32 x, s32 z);
void Colosso_ShowPointer(u32 x, u32 y, u32 style);
void Colosso_WalkActorToAndStand(s32 actorId, s32 x, s32 z);
#endif
void Colosso_RestoreCompetitor(s32 id);

void OverlayObject_ResetMotionFields(s32 actor);

void Colosso_MovePointer(u32 x, u32 y, u32 duration);
void Colosso_HidePointer(void);
void Colosso_WalkActorTo(s32 a, s32 b, s32 c);

#endif
