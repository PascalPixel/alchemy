#ifndef ALCHEMY_MODULE_SCENE_SHARED_COLOSSO_MARKER_H
#define ALCHEMY_MODULE_SCENE_SHARED_COLOSSO_MARKER_H

#include "types.h"

// Native calls precede these definitions; see MATCHING.md.
#ifdef UBFIX
void Colosso_FadeInWarrior(s32 id, s32 x, s32 z);
void Colosso_ShowPointer(u32 x, u32 y, u32 style);
void Colosso_WalkActorToAndStand(s32 actorId, s32 x, s32 z);
#endif
void Colosso_RestoreWarrior(s32 id);

void Actor_ResetMotionFields(s32 actorId);

void Colosso_MovePointer(u32 x, u32 y, u32 duration);
void Colosso_HidePointer(void);
void Colosso_WalkActorTo(s32 a, s32 b, s32 c);

#endif
