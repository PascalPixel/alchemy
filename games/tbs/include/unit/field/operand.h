#ifndef ALCHEMY_UNIT_FIELD_OPERAND_H
#define ALCHEMY_UNIT_FIELD_OPERAND_H

#include "types.h"

struct ScriptOperands;

/// A work value's handler: by operation (enum ScriptOperandOperation in
/// script_commands.c) it sets the object field it names to value, adds value
/// to it, or stores whether it equals value in conditionResult.
typedef void (*OperandFunc)(struct ScriptOperands *state, s32 operation,
                            s32 value);

extern const OperandFunc gObjectScript_OperandHandlerTable[41];

#endif
