#ifndef ALCHEMY_UNIT_FIELD_COMMON_SCRIPT_ICONMON_H
#define ALCHEMY_UNIT_FIELD_COMMON_SCRIPT_ICONMON_H

#include "types.h"

struct AnimationPosition;

void Ui_RunIconMonitor(void);
s32 Ui_FindNextNumberWithMetadata(s32 start, s32 step);
void Map_BuildProbeRing(s32 x, s32 z, struct AnimationPosition *out);
void Ui_FillGridColumnFromMetadata(s32 slot, s32 value);
void Ui_SetGridColumnNumber(s32 slot, s32 no);
void Battle_PlaceActorsByFormationKind(void);

#endif
