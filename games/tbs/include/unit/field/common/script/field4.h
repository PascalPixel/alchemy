#ifndef ALCHEMY_UNIT_FIELD_COMMON_SCRIPT_FIELD4_H
#define ALCHEMY_UNIT_FIELD_COMMON_SCRIPT_FIELD4_H

#include "types.h"

struct ScriptOperands;

void Script_SetOrCompareCursor(struct ScriptOperands *state, s32 operation, s32 value);
void Script_SetOrCompareRadius(struct ScriptOperands *state, s32 operation, s32 value);
void Script_SetOrCompareHalfword5e(struct ScriptOperands *work, s32 operation, s32 value);
s32 Script_ApplyOperandSet(struct ScriptOperands *work);
s32 Script_ApplyOperandAdd(struct ScriptOperands *work);
s32 Script_ApplyOperandCompare(struct ScriptOperands *work);

#endif
