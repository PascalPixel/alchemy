#ifndef ALCHEMY_UNIT_MAP_COMMON_COLOSSO_MARKER_H
#define ALCHEMY_UNIT_MAP_COMMON_COLOSSO_MARKER_H

#include "types.h"

// Native calls precede these definitions; see MATCHING.md.
#ifdef UBFIX
void Colosso_FadeInWarrior(s32 id, s32 x, s32 z);
void Colosso_ShowPointer(u32 x, u32 y, u32 style);
void Colosso_WalkObjectEventToAndStand(s32 localId, s32 x, s32 z);
#endif
void Colosso_RestoreWarrior(s32 id);

void ObjectEvent_ResetMotionFields(s32 localId);

void Colosso_MovePointer(u32 x, u32 y, u32 duration);
void Colosso_HidePointer(void);
void Colosso_WalkObjectEventTo(s32 a, s32 b, s32 c);

#endif
