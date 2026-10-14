#ifndef ALCHEMY_UNIT_FIELD_OPERAND_H
#define ALCHEMY_UNIT_FIELD_OPERAND_H

#include "types.h"

struct ScriptOperands;

typedef void (*OperandFunc)(struct ScriptOperands *state, s32 operation, s32 value);

extern const OperandFunc Script_OperandHandlerTable[41];

#endif
